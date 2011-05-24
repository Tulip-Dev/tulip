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
#include <QtXml/QDomDocument>

#include <tulip/PluginLister.h>
#include <tulip/TulipRelease.h>
#include <tulip/PluginInformations.h>
#include <tulip/PluginLoaderTxt.h>

#include <QuaZIPFacade.h>

using namespace std;
using namespace tlp;

class ArchiveCreator : public PluginLoader {
  public:
    ArchiveCreator(const QString& archiveDestinationDir) : archiveDestinationDir(archiveDestinationDir), tulipVersion(TULIP_RELEASE) {
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
    }
    
    void setCurrentPluginDir(const QString& pluginDir) {
      currentPluginDir = pluginDir;
    }
    
    virtual void loaded(const tlp::AbstractPluginInfo* info, const std::list< Dependency >& dependencies) {
      std::cout <<"loaded: " << info->getName() << std::endl;
      QString pluginName = QString::fromStdString(info->getName());
      QString pluginLibrary;     
      std::string pluginType;
      
      for(std::map<std::string, PluginListerInterface*>::const_iterator it = PluginListerInterface::allFactories->begin(); it != PluginListerInterface::allFactories->end(); ++it) {
        PluginListerInterface* currentLister = it->second;
        if(currentLister->pluginExists(pluginName.toStdString())) {
          pluginLibrary = QString::fromStdString(currentLister->getPluginLibrary(pluginName.toStdString()));
          pluginType = currentLister->getPluginsClassName();
        }
      }
      
      QString pluginSimplifiedName = pluginName.simplified().remove(' ').toLower();
      
      QString archiveName = pluginSimplifiedName + "-" + info->getRelease().c_str() + "-" + tulipVersion + "-" + platform + architecture + "-" + compiler + ".zip";
      
      bool compressed = QuaZIPFacade::zipDir(currentPluginDir, archiveDestinationDir + "/" + archiveName);
      if(!compressed) {
        cout << "failed to compress folder " << currentPluginDir.toStdString() << " as archive: " << archiveDestinationDir.toStdString() << "/" << archiveName.toStdString() << endl;
      }
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
      
      QFile outputXML(archiveDestinationDir + "/" + pluginSimplifiedName + ".xml");
      outputXML.open(QIODevice::ReadWrite);
      outputXML.write(pluginInfoDocument.toByteArray());
      outputXML.close();
    }

    virtual void aborted(const std::string& filename, const std::string& error) {
      cout << "\taborted " << filename << " because: " << error << endl;
    }
    virtual void finished(bool, const std::string&) {
      cout << "done ! " << endl;
    }
    virtual void loading(const std::string& filename){
      cout << "\tloading: " << filename << endl;
    }
    virtual void start(const std::string& path){
      cout << "starting: " << path << endl;
    }
    
  private:
    QString archiveDestinationDir;
    QString currentPluginDir;
    QString platform;
    QString architecture;
    QString compiler;
    QString tulipVersion;
};

int main(int argc,char **argv) {
  //the plugin path is the root of the plugin folder. In the following example, it is the edgeseparation folder.
//   pluginserver/
//   ├── edgeseparation
//   │   ├── lib
//   │   │   └── tulip
//   │   │       └── libedgeseparation-4.0.0.so
//   │   └── share
//   │       └── doc
//   │           └── edgeseparation
//   │               └── index.html
//   
  if(argc < 2) {
    std::cout << "packagePlugin pluginPath [destinationDir]" << std::endl;
    exit(0);
  }

  QString destinationDir;
  if(argc < 3) {
    destinationDir = QDir::currentPath();
  }
  else {
    destinationDir = argv[2];
    QDir destination(QDir::current());
    if(!destination.exists(destinationDir)) {
      destination.mkdir(destinationDir);
    }
  }

  initTulipLib();
  ArchiveCreator creator(destinationDir);
  QDir pluginServerDir(argv[1]);
  PluginListerInterface::currentLoader = &creator;
  foreach(const QString folder, pluginServerDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    QString plugindir = pluginServerDir.canonicalPath() + "/" + folder + "/lib/tulip/";
    creator.setCurrentPluginDir(plugindir);
    
    //TODO find file using QDir instead of this hackish "solution"
    QString pluginFile = plugindir + "lib" + folder + "-" + TULIP_RELEASE + ".so";
//     cout << plugindir.toStdString() << ": " << pluginFile.toStdString() << endl;
    PluginLibraryLoader::loadPluginLibrary(pluginFile.toStdString(), &creator);
  }
 
  return 0;
}
