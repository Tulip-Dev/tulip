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

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtXml/QDomDocument>
#include <QtCore/QLibrary>
#include <QtCore/QDebug>
#include <QtGui/QApplication>

#include <tulip/SystemDefinition.h>
#include <tulip/PluginLister.h>
#include <tulip/PluginInformations.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/QuaZIPFacade.h>

#include <tulip/Interactor.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>

#include <fcntl.h>
#include <tulip/TlpQtTools.h>

using namespace std;
using namespace tlp;

struct PluginInformationsCollector : public PluginLoader {

  virtual void loaded(const tlp::Plugin* info, const std::list< Dependency >&) {
    _directoryPlugins[_currentDirectory].push_back(info->name().c_str());
  }

  virtual void aborted(const std::string& plugin, const std::string& message) {
    std::cout << "failed to load plugin " << plugin << ": " << message << std::endl;
  }
  virtual void loading(const std::string&) {}
  virtual void finished(bool, const std::string&) {}
  virtual void start(const std::string&) {}

  QString _currentDirectory;
  QMap<QString,QStringList> _directoryPlugins;
};

int main(int argc,char **argv) {
  QApplication app(argc,argv);

  if(argc < 2) {
    qFatal("packagePlugin pluginPath [destinationDir]");
  }

  QString destinationDir = QDir::currentPath();
  if(argc >= 3)
    destinationDir = argv[2];
  QFileInfo destInfo(destinationDir);
  QDir::home().mkpath(destInfo.absoluteFilePath());

  // First we initialize Tulip with basic plugins to ensure dependencies consistency
  tlp::initTulipLib(QApplication::applicationDirPath().toUtf8().data());
  tlp::TulipPluginsPath = tlp::TulipPluginsPath;
  tlp::PluginLibraryLoader::loadPlugins();
  tlp::PluginLister::checkLoadedPluginsDependencies(0);
  tlp::InteractorLister::initInteractorsDependencies();
  tlp::GlyphManager::getInst().loadGlyphPlugins();
  tlp::EdgeExtremityGlyphManager::getInst().loadGlyphPlugins();

  QDir outputDir(destinationDir);

  // Next: we load additional plugins from external project and ZIP data into output directory
  PluginInformationsCollector collector;
  QDir pluginServerDir(argv[1]);
  PluginLister::currentLoader = &collector;
  foreach(const QFileInfo component, pluginServerDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    collector._currentDirectory = component.fileName();
    QDir pluginDir(component.absoluteFilePath());
    QDir::home().mkpath(outputDir.absoluteFilePath(component.fileName()));
    if (!QuaZIPFacade::zipDir(pluginDir.absolutePath(),outputDir.absoluteFilePath(component.fileName() + QDir::separator() + "data-" + OS_PLATFORM + OS_ARCHITECTURE + ".zip"))) {
      qFatal("Failed to zip archive");
    }
    pluginDir.cd("lib");
    pluginDir.cd("tulip");
    foreach(QFileInfo pluginFile, pluginDir.entryInfoList(QDir::Files | QDir::NoSymLinks)) {
      if (QLibrary::isLibrary(pluginFile.absoluteFilePath())) {
        PluginLibraryLoader::loadPluginLibrary(pluginFile.absoluteFilePath().toStdString(),&collector);
      }
    }
  }

  QDomDocument serverDocument;
  QDomElement rootNode = serverDocument.createElement("server");
  serverDocument.appendChild(rootNode);
  rootNode.setAttribute("serverName", destinationDir);
  rootNode.setAttribute("lastUpdate", QDateTime::currentDateTime().toString(Qt::ISODate));
  rootNode.setAttribute("release",TULIP_RELEASE);
  QDomElement pluginsNode = serverDocument.createElement("plugins");
  rootNode.appendChild(pluginsNode);

  foreach(QString component, collector._directoryPlugins.keys()) {
    foreach(QString plugin, collector._directoryPlugins[component]) {
      // Server description
      QDomElement componentDesc = serverDocument.createElement("plugin");
      componentDesc.setAttribute("name",plugin);
      componentDesc.setAttribute("path",component);
      pluginsNode.appendChild(componentDesc);

      // Plugin informations
      const Plugin* info = PluginLister::pluginInformations(plugin.toStdString());
      QDomDocument pluginDocument;
      QDomElement rootPluginNode = pluginDocument.createElement("plugin");
      rootPluginNode.setAttribute("name",info->name().c_str());
      rootPluginNode.setAttribute("category",info->category().c_str());
      rootPluginNode.setAttribute("author",info->author().c_str());
      rootPluginNode.setAttribute("date",info->date().c_str());
      rootPluginNode.setAttribute("desc",info->info().c_str());
      rootPluginNode.setAttribute("release",info->release().c_str());
      rootPluginNode.setAttribute("tulip",info->tulipRelease().c_str());
      QDomElement depsNode = pluginDocument.createElement("dependencies");
      std::list<Dependency> deps = PluginLister::getPluginDependencies(info->name());
      for(std::list<Dependency>::iterator it = deps.begin(); it != deps.end(); ++it) {
        QDomElement dep = pluginDocument.createElement("dependency");
        dep.setAttribute("name",it->pluginName.c_str());
        depsNode.appendChild(dep);
      }
      rootPluginNode.appendChild(depsNode);
      pluginDocument.appendChild(rootPluginNode);

      delete info;
      QFile outputPluginXML(outputDir.absoluteFilePath(component + QDir::separator() + plugin + ".xml"));
      outputPluginXML.open(QIODevice::ReadWrite | QIODevice::Truncate);
      outputPluginXML.write(pluginDocument.toByteArray());
      outputPluginXML.close();
    }
  }

  QFile outputXML(outputDir.absoluteFilePath("server.xml"));
  outputXML.open(QIODevice::ReadWrite | QIODevice::Truncate);
  outputXML.write(serverDocument.toByteArray());
  outputXML.close();

  qDebug() << "Output stored in" << outputDir.absolutePath();

  return 0;
}
