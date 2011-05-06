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
#include "PushToPluginServer.h"

#include <PluginLoaderWithInfo.h>
#include <UpdatePlugin.h>
#include <tulip/GlyphManager.h>
#include <tulip/ViewPluginsManager.h>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QBuffer>
#include <QtGui/QApplication>

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  QApplication app(argc, argv);
  if(argc < 5 || argc > 7)  {
    cout << "How to use :" << endl;
    cout << " 1st arg : plugin folder" << endl;
    cout << " 2nd arg : server address" << endl;
    cout << " 3rd arg : url of the pluginserver" << endl;
    cout << " 4th arg : base64 encoded credentials" << endl;
    cout << " 5th arg : i386/i64 : default i386" << endl;
    cout << " 6th arg : doxygen root dir" << endl;
    exit(1);
  }

  /** setup the variables **/
  QString pluginPath = argv[1];
  QString serverURL = argv[2];
  QString pluginServerURL = argv[3];
  QString base64credentials = argv[4];

  QString pluginDocRootPath("");
  
  QString architectureDir("i386");
  if(argc>5)
    architectureDir=QString(argv[5]);
  if(argc>6)
    pluginDocRootPath = argv[6];

  QDir pluginsDir(pluginPath);
  QStringList filters = QStringList() << "*.so" << "*.dll" << "*.dylib";

  initTulipLib(NULL);
  WebDavManager manager(serverURL, pluginServerURL, base64credentials);
  
  foreach(const QString& file, pluginsDir.entryList(filters)) {
    std::cout << "treating plugin library: " << file.toStdString() << std::endl;

    QString filepath = pluginPath + file;
    
    //make sure that Tulip will not try to load plugins from the TLP_DIR env variable (legacy behavior)
    char *getEnvTlp=getenv("TLP_DIR");
    #if defined(_WIN32)
    putenv("TLP_DIR=");
    #else
    setenv("TLP_DIR","",true);
    #endif

    /** load the plugin in Tulip to retrieve a pluginInfo **/
    PluginLoaderWithInfo plug;
    bool res = tlp::loadPlugin(filepath.toStdString(), &plug);
    if(!res) {
      std::cout << "failed to load plugin: " << plug.errorMsgs << std::endl;
      continue;
    }
    //GlyphManager::getInst().loadPlugins(&plug); //Glyph plugins
//     InteractorManager::getInst().loadPlugins(&plug);
//     ViewPluginsManager::getInst().loadPlugins(&plug);
//     ControllerPluginsManager::getInst().loadPlugins(&plug);

    #if defined(_WIN32)
    if (getEnvTlp)
      putenv((string("TLP_DIR=") + getEnvTlp).c_str());
    else
      putenv("TLP_DIR=");
  #else
    if (getEnvTlp)
      setenv("TLP_DIR",getEnvTlp,true);
    else
      unsetenv("TLP_DIR");
  #endif
    
//     TemplateFactory<GlyphFactory, Glyph, GlyphContext>::ObjectCreator::const_iterator itGlyphs;
//     vector<string> glyphsName;
//     for (itGlyphs=GlyphFactory::factory->objMap.begin();itGlyphs != GlyphFactory::factory->objMap.end();++itGlyphs) {
//       glyphsName.push_back((itGlyphs)->first);
//     }
// 
//     TemplateFactory<InteractorFactory, Interactor, InteractorContext>::ObjectCreator::const_iterator itInteractors;
//     vector<string> interactorsName;
//     for (itInteractors=InteractorFactory::factory->objMap.begin();itInteractors != InteractorFactory::factory->objMap.end();++itInteractors) {
//       interactorsName.push_back((itInteractors)->first);
//     }
// 
//     TemplateFactory<ViewFactory, View, ViewContext>::ObjectCreator::const_iterator itViews;
//     vector<string> viewsName;
//     for (itViews=ViewFactory::factory->objMap.begin();itViews != ViewFactory::factory->objMap.end();++itViews) {
//       viewsName.push_back((itViews)->first);
//     }
// 
//     for(size_t i=0;i<plug.pluginsList.size();++i){
//       for(vector<string>::iterator it=glyphsName.begin();it!=glyphsName.end();++it){
//         if(plug.pluginsList[i].name==(*it)){
//           plug.pluginsList[i].type="Glyph";
//         }
//       }
//       for(vector<string>::iterator it=interactorsName.begin();it!=interactorsName.end();++it){
//         if(plug.pluginsList[i].name==(*it)){
//           plug.pluginsList[i].type="Interactor";
//         }
//       }
//       for(vector<string>::iterator it=viewsName.begin();it!=viewsName.end();++it){
//         if(plug.pluginsList[i].name==(*it)){
//           plug.pluginsList[i].type="View";
//         }
//       }
//       if(plug.pluginsList[i].type=="")
//         plug.pluginsList[i].type="Algorithm";
//     }

    LocalPluginInfo pluginInfo;
//     if(plug.pluginsList.size()>1)  {
//       bool equal=true;
//       for(size_t i=0;i<plug.pluginsList.size()-1;++i){
//         if(plug.pluginsList[i].type!=plug.pluginsList[i+1].type){
//           equal=false;
//           break;
//         }
//       }
// 
//       if(!equal){
//         for(size_t i=0;i<plug.pluginsList.size();++i){
//           if(plug.pluginsList[i].type=="View"){
//             pluginInfo = plug.pluginsList[i];
//             break;
//           }
//         }
//       }
//       else{
//         pluginInfo = plug.pluginsList[0];
//       }
//     }


    if(plug.pluginsList.size()==1){
      pluginInfo = plug.pluginsList[0];
    }
    else {
      std::cout << "more than on plugin in library: " << pluginPath.toStdString() << std::endl;
      continue;
    }

    pluginInfo.displayType= PluginInfo::getPluginDisplayType(pluginInfo.name);
    if(pluginInfo.displayType == "Glyph")
      pluginInfo.type = "Glyph";

    /** upload the plugin to the server **/
    QString pluginFolderName = pluginInfo.fileName.c_str()+QString(".")+(QString(pluginInfo.version.c_str()).split(" "))[1];

//     std::cout << "mkdir " << pluginFolderName.toStdString() << std::endl;
    manager.mkdir(pluginFolderName);
//     std::cout << "mkdir pluginFolder/" << architectureDir.toStdString() << std::endl;
    manager.mkdir(pluginFolderName + "/" + architectureDir);
    
    QFile pluginFile(filepath);
    bool isOpen = pluginFile.open(QIODevice::ReadOnly);

    if(!isOpen) {
      std::cout << "could not open file for reading: " << filepath.toStdString() << std::endl;
      continue;
    }
    QFileInfo fileInfo(filepath);
    manager.putFile(pluginFolderName + "/" + architectureDir + "/" + fileInfo.fileName(), &pluginFile);

    /** Generate the documentation and upload it **/
    if(!pluginDocRootPath.isEmpty()) {
      QDomDocument pluginInfoDocument("pluginInfoDocument");
      QDomElement infoElement = pluginInfoDocument.createElement("pluginInfo");
      pluginInfoDocument.appendChild(infoElement);
      infoElement.setAttribute("name", QString::fromStdString(pluginInfo.name));
      infoElement.setAttribute("type", QString::fromStdString(pluginInfo.type));
      infoElement.setAttribute("displayType", QString::fromStdString(pluginInfo.displayType));
      infoElement.setAttribute("author", QString::fromStdString(pluginInfo.author));
      infoElement.setAttribute("date", QString::fromStdString(pluginInfo.date));
      infoElement.setAttribute("info", QString::fromStdString(pluginInfo.info));
      infoElement.setAttribute("fileName", QString::fromStdString(pluginInfo.fileName));
      infoElement.setAttribute("version", QString::fromStdString(pluginInfo.version));
      
      for(vector<PluginDependency>::const_iterator it = pluginInfo.dependencies.begin(); it != pluginInfo.dependencies.end(); ++it) {
        QDomElement dependencyElement = pluginInfoDocument.createElement("dependency");
        dependencyElement.setAttribute("name", QString::fromStdString(it->name));
        dependencyElement.setAttribute("type", QString::fromStdString(it->type));
        dependencyElement.setAttribute("version", QString::fromStdString(it->version));
      }
      
      QString fileName = QString::fromStdString(pluginInfo.fileName);
      QString libName = fileName.split("-").first();
      libName=libName.split("lib").last();
      QString documentName(pluginDocRootPath + "/class" + libName + ".xml");

      QDir pluginsDocDir(pluginDocRootPath);
      QStringList filters;
      filters << "class*.xml";
      QStringList pluginsDocs = pluginsDocDir.entryList(filters);
      QList<QString>::iterator iter = pluginsDocs.begin();

      while(iter != pluginsDocs.end()){

        QString docFile(*iter);
        docFile = (docFile.mid(5,docFile.size()-9));

        if(docFile.compare(libName,Qt::CaseInsensitive)==0){
          documentName = QString(pluginDocRootPath + "/" + (*iter));
        }
        ++iter;
      }

      QDomDocument pluginClassDocumentationDocument;
      QFile documentationFile(documentName);
      documentationFile.open(QIODevice::ReadOnly);
      pluginClassDocumentationDocument.setContent(&documentationFile);

      documentationFile.close();

    //   std::cout << documentName.toStdString() << std::endl;
      QDomNodeList tempList = pluginClassDocumentationDocument.elementsByTagName("compounddef");
    //   std::cout << pluginClassDocumentationDocument.toString().toStdString() << std::endl;
      QDomNode classElement = tempList.at(0);
    //   std::cout << classElement.nodeName().toStdString() << ": " << classElement.nodeValue().toStdString() << std::endl;
      QDomNode briefDescriptionElement = classElement.firstChildElement("briefdescription");
      QDomNode detailedDescriptionElement = classElement.firstChildElement("detaileddescription");

      QDomDocument pluginDocDocument;
      QDomElement docElement = pluginDocDocument.createElement("doc");
      pluginDocDocument.appendChild(docElement);
    //   std::cout << briefDescriptionElement.nodeName().toStdString() << ": " << briefDescriptionElement.nodeValue().toStdString() << std::endl;
      docElement.appendChild(briefDescriptionElement);
      docElement.appendChild(detailedDescriptionElement);


      QByteArray tempInfo(pluginInfoDocument.toByteArray());
      QBuffer pluginInfoDevice(&tempInfo);
      pluginInfoDevice.open(QBuffer::ReadOnly);
      manager.putFile(pluginFolderName + "/" + "/" + QString::fromStdString(pluginInfo.fileName) + ".xml", &pluginInfoDevice);

      QByteArray tempDoc(pluginDocDocument.toByteArray());
      QBuffer pluginDocDevice(&tempDoc);
      pluginDocDevice.open(QBuffer::ReadOnly);
      manager.putFile(pluginFolderName + "/" + "/" + QString::fromStdString(pluginInfo.fileName) + ".doc", &pluginDocDevice);
    }
  }
  manager.finish();
  
  return 0;
}


