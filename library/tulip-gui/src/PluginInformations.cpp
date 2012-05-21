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
#include <tulip/PluginInformations.h>

#include <tulip/WithDependency.h>
#include <tulip/Plugin.h>
#include <tulip/PluginLister.h>

#include <QtCore/QFileInfo>
#include <tulip/TlpQtTools.h>
#include <tulip/DownloadManager.h>
#include <QDesktopServices>
#include <tulip/QuaZIPFacade.h>
#include <qtextstream.h>
#include <tulip/TulipSettings.h>
#include <QNetworkReply>

using namespace tlp;

PluginInformations::PluginInformations(const tlp::Plugin* info, const std::string& type, const std::string& library)
  :_lastVersion(info->release().c_str()), _type(type.c_str()), _iconPath(info->icon().c_str()), _longDescriptionPath("about:blank"), _isLocal(true),
   _installedVersion(info->release().c_str()), _updateAvailable(false), _version(info->release().c_str()), _infos(info), _library(library.c_str()) {
}

PluginInformations::PluginInformations(const tlp::Plugin* info, const QString& type, const QString& basePath, const QString& remotepluginName)
  :_lastVersion(info->release().c_str()), _type(type), _iconPath(basePath + "/icon.png"), _longDescriptionPath(basePath + "/html/index.html"), _isLocal(false), _installedVersion(QString::null),
   _updateAvailable(false), _remoteLocation(basePath + "/" + remotepluginName), _remoteArchive(_remoteLocation + "/" + tlp::getPluginPackageName(remotepluginName)), _version(info->release().c_str()), _infos(info) {
}

void PluginInformations::AddPluginInformations(const tlp::Plugin* info) {
  QString newVersion = info->release().c_str();

  if(_installedVersion < newVersion) {
    _updateAvailable = true;
    _lastVersion = newVersion;
  }
}

QString PluginInformations::identifier() const {
  return _infos->name().c_str();
}

QString PluginInformations::name() const {
  return _infos->name().c_str();
}

QString PluginInformations::author() const {
  return _infos->author().c_str();
}

QString PluginInformations::group() const {
  return _infos->group().c_str();
}

QString PluginInformations::shortDescription() const {
  return _infos->info().c_str();
}

QString PluginInformations::longDescriptionPath() const {
  return _longDescriptionPath;
}

QString PluginInformations::iconPath() const {
  return _iconPath;
}

QDateTime PluginInformations::installDate() const {
  return QDateTime::currentDateTime();
}

QString PluginInformations::type() const {
  return _type;
}

const QStringList PluginInformations::dependencies() const {
  QStringList result;
  result.reserve(_infos->dependencies().size());

  for(std::list<tlp::Dependency>::const_iterator it = _infos->dependencies().begin(); it != _infos->dependencies().end(); ++it) {
    result.append(it->pluginName.c_str());
  }

  return result;
}

const QString& PluginInformations::version() const {
  return _version;
}

QString PluginInformations::installedVersion() const {
  return _installedVersion;
}

bool PluginInformations::isInstalled() const {
  return !(installedVersion().isEmpty());
}

bool PluginInformations::updateAvailable() const {
  return _updateAvailable;
}

QString PluginInformations::latestVersion() const {
  return _lastVersion;
}

bool PluginInformations::fetch() const {
  bool result = false;
  const QString fullArchivePath = tlp::getPluginStagingDirectory() + "/" + tlp::getPluginPackageName(name());
  QNetworkReply* reply = DownloadManager::getInstance()->downloadPlugin(_remoteArchive, fullArchivePath);

  connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));

  emit(DownloadStarted(reply));

  return result;
}

void PluginInformations::remove() const {
  TulipSettings::instance().markPluginForRemoval(_library);
}

void PluginInformations::downloadFinished() {
  bool result = false;

  //TODO if the error reporting and/or plugin unpacking gets any bigger, move it to the PluginManager class

  //if this is a plugin upgrade, it will be performed at Tulip's next startup, otherwise it can be done now
  if(!isInstalled()) {
    const QString fullArchivePath = tlp::getPluginStagingDirectory() + "/" + tlp::getPluginPackageName(name());
    tlp::SimplePluginProgress* progress = new tlp::SimplePluginProgress();
    result = QuaZIPFacade::unzip(tlp::getPluginLocalInstallationDir(), fullArchivePath, progress);

    //if the extraction was successfull, remove the archive
    if(result) {
      QFile::remove(fullArchivePath);
    }
    else {
      //TODO proper error reporting
      qDebug() << "error while extracting plugin: " << progress->getError();
    }

    //TODO load plugin ? should not be necessary as the PluginCenter does not need it. usefull for error reporting ?
  }
}
