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
#include <tulip/ControllerPluginsManager.h>

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  if(argc<=2)  {
    cout << "How to use :" << endl;
    cout << " First arg : doc source directory (where directory \"doxygen\" are)" << endl;
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
  ControllerPluginsManager::getInst().loadPlugins(&plug);

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
  dir.mkpath(targetPath+"/plugins/");

  for(vector<LocalPluginInfo>::iterator it=pluginsList.begin();it!=pluginsList.end();++it) {

    QString path=(*it).fileName.c_str();

    QDir dstDir(targetPath+"/plugins/"+path);
    dstDir.mkpath(targetPath+"/plugins/"+path);

    if (!generatePluginInfoFile((*it), dstDir))
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
      UpdatePlugin::copyFile(srcDir,QString((*it).fileName.c_str())+".so",dstDir,QString((*it).fileName.c_str())+".so."+QString((*it).version.c_str()).replace(" ",".")+".i386");
    }
    if(argc==4) {
      if(secondSrcDir.exists(QString((*it).fileName.c_str())+".dylib")) {
	UpdatePlugin::copyFile(secondSrcDir,QString((*it).fileName.c_str())+".dylib",dstDir,QString((*it).fileName.c_str())+".dylib."+QString((*it).version.c_str()).replace(" ","."));
      }
      if(secondSrcDir.exists(QString((*it).fileName.c_str())+".dll")) {
	UpdatePlugin::copyFile(secondSrcDir,QString((*it).fileName.c_str())+".dll",dstDir,QString((*it).fileName.c_str())+".dll."+QString((*it).version.c_str()).replace(" ","."));
      }
    }
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
    
    while(iter != pluginsDocs.end()){
      
      QString docFile(*iter);
      docFile = (docFile.mid(5,docFile.size()-9));
            
      if(docFile.compare(libName,Qt::CaseInsensitive)==0){
	QFile srcFile(docPath+"/doxygen/xml/" + (*iter));
	QString version((*it).version.c_str());
	generatePluginDocFile(fileName, version, srcFile, dstDir);
      }
      ++iter;
    }
  }
    
    
  return EXIT_SUCCESS;
}
