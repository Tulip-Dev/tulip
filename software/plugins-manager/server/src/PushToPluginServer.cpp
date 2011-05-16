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

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QBuffer>
#include <QtGui/QApplication>

#include <tulip/PluginLister.h>
#include <tulip/TulipRelease.h>
#include <tulip/PluginInformations.h>
#include <tulip/PluginLoaderTxt.h>
#include <QDomDocument>

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  QApplication app(argc, argv);

  if(argc < 2) {
    std::cout << "pluginPusher pluginPath serverUrl base64credentials" << std::endl;
    exit(0);
  }
  
  QString pluginPath = argv[1];
//   QString serverURL = argv[2];
//   QString base64credentials = argv[3];
  
  QString pluginName;
  QString tulipVersion;
  QString platform;
  QString architecture;
  QString compiler;

  QString pluginLibrary;
  
  const AbstractPluginInfo* info;
  
  tulipVersion = QString(TULIP_RELEASE);
  
  #if defined(_WIN32) || defined(_WIN64)
    platform = "win";
  #elif defined(__APPLE__)
    platform = "mac";
  #else
    platform = "linux";
  #endif
  
  #if _WIN64 || __amd64__
    architecture = "64";
  #else
    architecture = "32";
  #endif
  
  #if defined(__clang__)
    compiler = "Clang";
  #elif defined(__GNUC__)
    compiler="gcc";
  #elif defined(_MSC_VER)
    compiler = "MSVC";
  #endif

  initTulipLib(pluginPath.toStdString().c_str());
  PluginLoaderTxt loader;
  tlp::loadPlugins(&loader); // library side plugins

  std::list<tlp::Dependency> dependencies;
  std::string pluginType;
  
  for(std::map<std::string, PluginListerInterface*>::const_iterator it = PluginListerInterface::allFactories->begin(); it != PluginListerInterface::allFactories->end(); ++it) {
    PluginListerInterface* currentLister = it->second;
    Iterator<string>* plugins = currentLister->availablePlugins();
    while(plugins->hasNext()) {
      string currentPluginName = plugins->next();
      if(currentLister->getPluginLibrary(currentPluginName) != "libTulip") {
        pluginName = QString::fromStdString(currentPluginName);
        dependencies = currentLister->getPluginDependencies(currentPluginName);
        pluginType = currentLister->getPluginsClassName();
        info = currentLister->pluginInformations(currentPluginName);
        pluginLibrary = QString::fromStdString(currentLister->getPluginLibrary(currentPluginName));
      }
//       const AbstractPluginInfo* info = currentLister->pluginInformations(currentPluginName);
//       PluginInformationsInterface* localinfo = new LocalPluginInformations(info, currentLister->getPluginsClassName(),
//                                                                            currentLister->getPluginDependencies(currentPluginName), currentLister->getPluginLibrary(currentPluginName));
    }
    delete plugins;
  }

  pluginName = pluginName.simplified().remove(' ').toLower();
  
  QString archiveName = pluginName + "-" + tulipVersion + "-" + platform + architecture + "-" + compiler + ".zip";

  std::cout << archiveName.toStdString() << std::endl;

  //TODO
  QString pluginDocRootPath = pluginPath + "/doc";

  /** Generate the documentation and upload it **/
  if(!pluginDocRootPath.isEmpty()) {
    QDomDocument pluginInfoDocument("pluginInfoDocument");
    QDomElement infoElement = pluginInfoDocument.createElement("pluginInfo");
    pluginInfoDocument.appendChild(infoElement);
    infoElement.setAttribute("name", pluginName);
    infoElement.setAttribute("type", QString::fromStdString(pluginType));
//     infoElement.setAttribute("displayType", QString::fromStdString(pluginInfo.displayType));
    infoElement.setAttribute("author", QString::fromStdString(info->getAuthor()));
    infoElement.setAttribute("date", QString::fromStdString(info->getDate()));
    infoElement.setAttribute("info", QString::fromStdString(info->getInfo()));
//     infoElement.setAttribute("fileName", QString::fromStdString(pluginInfo.fileName));
    infoElement.setAttribute("version", QString::fromStdString(info->getRelease()));

    for(std::list<Dependency>::const_iterator it = dependencies.begin(); it != dependencies.end(); ++it) {
      QDomElement dependencyElement = pluginInfoDocument.createElement("dependency");
      dependencyElement.setAttribute("name", QString::fromStdString(it->pluginName));
      dependencyElement.setAttribute("type", QString::fromStdString(it->factoryName));
      dependencyElement.setAttribute("version", QString::fromStdString(it->pluginRelease));
    }

    QString fileName = pluginLibrary;
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
//         pluginInfoDevice.open(QBuffer::ReadOnly);
//         manager.putFile(pluginFolderName + "/" + "/" + QString::fromStdString(pluginInfo.fileName) + ".xml", &pluginInfoDevice);

    QByteArray tempDoc(pluginDocDocument.toByteArray());
    QBuffer pluginDocDevice(&tempDoc);
//         pluginDocDevice.open(QBuffer::ReadOnly);
//         manager.putFile(pluginFolderName + "/" + "/" + QString::fromStdString(pluginInfo.fileName) + ".doc", &pluginDocDevice);
  }

//   WebDavManager manager(serverURL, pluginServerURL, base64credentials);
//   
//   foreach(const QString& file, pluginsDir.entryList(filters)) {
//     std::cout << "treating plugin library: " << file.toStdString() << std::endl;
// 
//     QString filepath = pluginPath + file;
// //     std::cout << "mkdir " << pluginFolderName.toStdString() << std::endl;
//     manager.mkdir(pluginFolderName);
//     
//     QFile pluginFile(filepath);
//     bool isOpen = pluginFile.open(QIODevice::ReadOnly);
// 
//     if(!isOpen) {
//       std::cout << "could not open file for reading: " << filepath.toStdString() << std::endl;
//       continue;
//     }
//     QFileInfo fileInfo(filepath);
//     manager.putFile(pluginFolderName + "/" + architectureDir + "/" + fileInfo.fileName(), &pluginFile);
// 

//   }
//   manager.finish();
  
  return 0;
}


