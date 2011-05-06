/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "PluginGenerate.h"

#include <PluginLoaderWithInfo.h>
#include <UpdatePlugin.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include <tulip/TlpTools.h>
#include <tulip/GlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  if(argc<=2)  {
    cout << "How to use :" << endl;
    cout << " First arg : doc source directory (where directory \"doxygen\" are), can be empty" << endl;
    cout << " Second arg : target directory" << endl;
    cout << " Third arg : source directory for mac/windows plugins" << endl;
    exit(1);
  }

  QString docPath=argv[1];
  QString targetPath=argv[2];

  PluginLoaderWithInfo plug;
  initTulipLib(NULL);
  loadPlugins(&plug);    // library side plugins
  GlyphManager::getInst().loadPlugins(&plug); //Glyph plugins 
  InteractorManager::getInst().loadPlugins(&plug);
  ViewPluginsManager::getInst().loadPlugins(&plug);
  tlp::loadPlugins(&plug, "/controller");

  if (!plug.errorMsgs.empty()) {
    cout << "Error when loading plugins:"<< endl;
    cout << plug.errorMsgs << endl;
    return EXIT_FAILURE;
  }

  vector<LocalPluginInfo> pluginsList=plug.pluginsList;

  for(vector<LocalPluginInfo>::iterator it=pluginsList.begin();it!=pluginsList.end();++it) {
    if((*it).type!="Glyph"){
      (*it).displayType=PluginInfo::getPluginDisplayType((*it).name);
    }else{
      (*it).displayType=(*it).type;
    }
  }

  QDir dir;
  dir.mkpath(targetPath);

#if defined(I64)
    QString archSubDir("i64");
#else
    QString archSubDir("i386");
#endif

  for(vector<LocalPluginInfo>::iterator it=pluginsList.begin();it!=pluginsList.end();++it) {
    QString path((*it).fileName.c_str());

    cout << "*** " << it->fileName.c_str() << " of type " << it->displayType << " found : " << endl;

    // version is X.Y.Z a.b with X.Y.Z the Tulip version
    path += '.' + QString((*it).version.substr(6).c_str());

    QDir dstDir(targetPath+"/pluginsV2/"+path);
    dstDir.mkpath(targetPath+"/pluginsV2/"+path);
    dstDir.mkpath(targetPath+"/pluginsV2/"+path + '/' + archSubDir);

    if (!docPath.isEmpty() && !generatePluginInfoFile((*it), dstDir))
      return EXIT_FAILURE;

    QDir srcDir;
    string subDir;
    if((*it).type=="Glyph")
      subDir="glyphs/";
    else if((*it).type=="Interactor")
      subDir="interactors/";
    else if((*it).type=="View")
      subDir="view/";

    srcDir = QDir((TulipLibDir+"tlp/"+subDir).c_str());
    QDir secondSrcDir;
    if(argc==4) {
      secondSrcDir=QDir((string(argv[3])+subDir).c_str());
    }

    if(srcDir.exists(QString((*it).fileName.c_str())+".so")) {
      UpdatePlugin::moveFile(srcDir,QString((*it).fileName.c_str())+".so",
			     QDir(dstDir.filePath(archSubDir)),
			     QString((*it).fileName.c_str())+".so");
    }
    if(argc==4) {
      dstDir.mkpath(targetPath+"/pluginsV2/"+path + "/i386");
      if(secondSrcDir.exists(QString((*it).fileName.c_str())+".dylib")) {
        UpdatePlugin::moveFile(secondSrcDir,QString((*it).fileName.c_str())+".dylib",
			       QDir(dstDir.filePath("i386")),
			       QString((*it).fileName.c_str())+".dylib");
      }
      if(secondSrcDir.exists(QString((*it).fileName.c_str())+".dll")) {
        UpdatePlugin::moveFile(secondSrcDir,QString((*it).fileName.c_str())+".dll",
			       QDir(dstDir.filePath("i386")),
			       QString((*it).fileName.c_str())+".dll");
      }
    }
    if (!docPath.isEmpty()) {
      // Documentation :
      QDir pluginsDocDir(docPath+"/doxygen/xml");

      QStringList filters;
      filters << "class*.xml";
      QStringList pluginsDocs = pluginsDocDir.entryList(filters);
      QString fileName = QString((*it).fileName.c_str());
      QString libName = fileName.split("-").first();
      libName=libName.split("lib").last();
      //cout << "lib name : " << libName.toStdString() << endl;
  
      QList<QString>::iterator iter = pluginsDocs.begin();
    
      bool xmlDocFound = false;
      while(iter != pluginsDocs.end()){
      
	QString docFile(*iter);
	docFile = (docFile.mid(5,docFile.size()-9));

	if(docFile.compare(libName, Qt::CaseInsensitive)==0){
	  QFile srcFile(docPath+"/doxygen/xml/" + (*iter));
	  QString version((*it).version.c_str());
	  generatePluginDocFile(fileName, version, srcFile, dstDir);
	  xmlDocFound = true;
	}
	++iter;
      }
      if (xmlDocFound == false)	
	cout << "no xml doc file found" << endl;
    }
  }
    
    
  return EXIT_SUCCESS;
}
