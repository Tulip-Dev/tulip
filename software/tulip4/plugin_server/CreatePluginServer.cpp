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
#include "CreatePluginServer.h"

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QBuffer>
#include <QtXml/QDomDocument>
#include <QtGui/QApplication>

#include <tulip/PluginLister.h>
#include <tulip/TulipRelease.h>
#include <tulip/PluginInformations.h>
#include <tulip/PluginLoaderTxt.h>

#include <QuaZIPFacade.h>

using namespace std;
using namespace tlp;

int main(int argc,char **argv) {
  QApplication app(argc, argv);

  if(argc < 2) {
    std::cout << "packagePlugin pluginPath" << std::endl;
    exit(0);
  }
  
  QString pluginPath = argv[1];
  
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
  tlp::PluginLibraryLoader::loadPlugins(&loader); // library side plugins

  std::list<tlp::Dependency> dependencies;
  std::string pluginType;
  
  for(std::map<std::string, PluginListerInterface*>::const_iterator it = PluginListerInterface::allFactories->begin(); it != PluginListerInterface::allFactories->end(); ++it) {
    PluginListerInterface* currentLister = it->second;
    Iterator<string>* plugins = currentLister->availablePlugins();
    while(plugins->hasNext()) {
      string currentPluginName = plugins->next();
      if(currentLister->getPluginLibrary(currentPluginName) != "") {
        pluginName = QString::fromStdString(currentPluginName);
        dependencies = currentLister->getPluginDependencies(currentPluginName);
        pluginType = currentLister->getPluginsClassName();
        info = currentLister->pluginInformations(currentPluginName);
        pluginLibrary = QString::fromStdString(currentLister->getPluginLibrary(currentPluginName));
      }
    }
    delete plugins;
  }

  pluginName = pluginName.simplified().remove(' ').toLower();
  
  QString archiveName = pluginName + "-" + info->getRelease().c_str() + "-" + tulipVersion + "-" + platform + architecture + "-" + compiler + ".zip";

  std::cout << archiveName.toStdString() << std::endl;
  
  QuaZIPFacade::zipDir(pluginPath, pluginPath + "../" + archiveName);

  //creates the xml document for this plugin
  QDomDocument pluginInfoDocument;
  QDomElement infoElement = pluginInfoDocument.createElement("plugin");
  pluginInfoDocument.appendChild(infoElement);
  infoElement.setAttribute("name", pluginName);
  infoElement.setAttribute("type", QString::fromStdString(pluginType));
  infoElement.setAttribute("author", QString::fromStdString(info->getAuthor()));
  infoElement.setAttribute("date", QString::fromStdString(info->getDate()));
  infoElement.setAttribute("info", QString::fromStdString(info->getInfo()));
  infoElement.setAttribute("fileName", pluginLibrary);
  infoElement.setAttribute("version", QString::fromStdString(info->getRelease()));

  for(std::list<Dependency>::const_iterator it = dependencies.begin(); it != dependencies.end(); ++it) {
    QDomElement dependencyElement = pluginInfoDocument.createElement("dependency");
    dependencyElement.setAttribute("name", QString::fromStdString(it->pluginName));
    dependencyElement.setAttribute("type", QString::fromStdString(it->factoryName));
    dependencyElement.setAttribute("version", QString::fromStdString(it->pluginRelease));
  }

  QFile outputXML(pluginName + ".xml");
  outputXML.open(QIODevice::ReadWrite);
  outputXML.write(pluginInfoDocument.toByteArray());
  outputXML.close();
 
  return 0;
}


