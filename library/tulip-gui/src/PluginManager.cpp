/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/PluginManager.h>

#include <QDir>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <tulip/TulipSettings.h>
#include <tulip/PluginLister.h>
#include <tulip/SystemDefinition.h>
#include <tulip/YajlFacade.h>
#include <tulip/QuaZIPFacade.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;

const QString PluginManager::STABLE_LOCATION = QString("http://tulip.labri.fr/pluginserver/stable/") + TULIP_MM_VERSION;
const QString PluginManager::TESTING_LOCATION = QString("http://tulip.labri.fr/pluginserver/testing/") + TULIP_MM_VERSION;

QDebug operator<<(QDebug dbg, const PluginVersionInformation &c) {
  dbg.nospace() << "(author " << c.author << ") "
                << "(version " << c.version << ") "
                << "(icon " << c.icon << ") "
                << "(description " << c.description << ") "
                << "(date " << c.date << ") "
                << "(librarylocation " << c.libraryLocation << ") "
                << "(dependencies " << c.dependencies << ") ";

  return dbg.space();
}

QDebug operator<<(QDebug dbg, const PluginInformation &c) {
  dbg.nospace() << "(name " << c.name << ") "
                << "(category " << c.category << ") "
                << "(installed " << c.installedVersion << ") "
                << "(available " << c.availableVersion << ") ";

  return dbg.space();
}

class PluginServerClient: public YajlParseFacade {
  QString _location;

  PluginManager::PluginInformationList _result;
  QString _currentKey;
  QMap<QString,QString> _currentMap;

public:
  PluginServerClient(const QString& location): _location(location) {
  }

  void fetch(const QString& name, QObject* recv, const char* progressSlot) {
    QNetworkAccessManager mgr;

    QNetworkReply* reply = NULL;
    QUrl url(_location + "/fetch.php?os=" + OS_PLATFORM + "&arch=" + OS_ARCHITECTURE + "&tulip=" + TULIP_MM_VERSION + "&name=" + name);

    do {
      QNetworkRequest request(url);
      reply = mgr.get(request);
      QObject::connect(reply,SIGNAL(downloadProgress(qint64,qint64)),recv,progressSlot);

      while (!reply->isFinished()) {
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
      }

      url = QUrl(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
    }
    while (reply->attribute(QNetworkRequest::RedirectionTargetAttribute).isValid());

    reply->open(QIODevice::ReadOnly);
    QString tmpOutPath = QDir::temp().absoluteFilePath("tulip_plugin_" + name + ".zip");
    QFile tmpOut(tmpOutPath);
    tmpOut.open(QIODevice::WriteOnly);
    tmpOut.write(reply->readAll());
    tmpOut.close();
    reply->close();
    QuaZIPFacade::unzip(tlp::localPluginsPath(),tmpOutPath);
    tmpOut.remove();
  }

  PluginManager::PluginInformationList list(const QString& nameFilter, const QString& categoryFilter) {
    _result.clear();
    QNetworkAccessManager mgr;
    QNetworkRequest request(QUrl(_location + "/list.php?os=" + OS_PLATFORM + "&arch=" + OS_ARCHITECTURE + "&tulip=" + TULIP_MM_VERSION + "&name=" + nameFilter + "&category=" + categoryFilter));
    QNetworkReply* reply = mgr.get(request);

    while (!reply->isFinished()) {
      QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    reply->open(QIODevice::ReadOnly);
    QByteArray contents = reply->readAll();
    reply->close();
    parse((const unsigned char *) contents.constData(), contents.size());
    return _result;
  }

  virtual void parseString(const std::string& value) {
    _currentMap[_currentKey] = QString::fromUtf8(value.c_str());
  }

  virtual void parseMapKey(const std::string& value) {
    _currentKey = value.c_str();
  }

  virtual void parseStartMap() {
    _currentMap.clear();
  }

  virtual void parseEndMap() {
    PluginInformation infos;
    infos.name = _currentMap["name"];
    infos.category = _currentMap["category"];

    PluginVersionInformation versionInfos;
    versionInfos.description = _currentMap["desc"];
    versionInfos.libraryLocation = _location;
    versionInfos.version = _currentMap["release"];
    versionInfos.author = _currentMap["author"];
    versionInfos.date = _currentMap["date"];
    versionInfos.isValid = true;
    // TODO fill icon
    infos.availableVersion = versionInfos;
    _result.push_back(infos);
  }
};

void PluginManager::addRemoteLocation(const QString &location) {
  TulipSettings::instance().addRemoteLocation(location);
}

void PluginManager::removeRemoteLocation(const QString &location) {
  TulipSettings::instance().removeRemoteLocation(location);
}

QStringList PluginManager::remoteLocations() {
  return TulipSettings::instance().remoteLocations();
}

QStringList PluginManager::_markedForInstallation = QStringList();

PluginManager::PluginInformationList PluginManager::listPlugins(PluginLocations locations, const QString &nameFilter, const QString &categoryFilter) {
  QMap<QString,PluginInformation> nameToInfos;

  if (locations.testFlag(Local)) {
    std::list<std::string> localResults = PluginLister::instance()->availablePlugins();

    for (std::list<std::string>::iterator it = localResults.begin(); it != localResults.end(); ++it) {
      const Plugin& info = PluginLister::instance()->pluginInformation(*it);

      if (QString(info.category().c_str()).contains(categoryFilter) && QString(info.name().c_str()).contains(nameFilter, Qt::CaseInsensitive)) {
        nameToInfos[info.name().c_str()].fillLocalInfos(info);
      }
    }
  }

  if (locations.testFlag(Remote)) {
    foreach(QString loc, remoteLocations()) {
      PluginServerClient client(loc);
      foreach(PluginInformation infos, client.list(nameFilter,categoryFilter)) {
        PluginInformation storedInfos = nameToInfos[infos.name];
        storedInfos.name = infos.name;
        storedInfos.category = infos.category;
        storedInfos.availableVersion = infos.availableVersion;
        nameToInfos[infos.name] = storedInfos;
      }
    }
  }

  PluginInformationList result;
  foreach(PluginInformation i, nameToInfos.values()) {
    result.push_back(i);
  }

  return result;
}

void PluginManager::markForRemoval(const QString &plugin) {
  TulipSettings::instance().markPluginForRemoval(plugin);
}

void PluginManager::markForInstallation(const QString& plugin, QObject* recv, const char *progressSlot) {
  PluginInformationList lst = listPlugins(Remote,plugin);

  if (lst.size() == 0 || !lst.first().availableVersion.isValid)
    return;

  PluginVersionInformation version = lst.first().availableVersion;
  PluginServerClient clt(version.libraryLocation);
  clt.fetch(plugin, recv, progressSlot);
  _markedForInstallation.push_back(plugin);
}

QStringList PluginManager::markedForInstallation() {
  return _markedForInstallation;
}

QStringList PluginManager::markedForRemoval() {
  return TulipSettings::instance().pluginsToRemove();
}

void PluginManager::unmarkForRemoval(const QString &file) {
  TulipSettings::instance().unmarkPluginForRemoval(file);
}

PluginInformation::PluginInformation() {
}

PluginInformation::PluginInformation(const PluginInformation &copy) {
  name = copy.name;
  category = copy.category;
  installedVersion = copy.installedVersion;
  availableVersion = copy.availableVersion;
}

void PluginInformation::fillLocalInfos(const Plugin& info) {
  name = tlp::tlpStringToQString(info.name());
  category = tlp::tlpStringToQString(info.category());
  installedVersion.description = tlp::tlpStringToQString(info.info());
  installedVersion.icon = tlp::tlpStringToQString(info.icon());
  installedVersion.version = info.release().c_str();
  installedVersion.date = info.date().c_str();
  installedVersion.author = tlp::tlpStringToQString(info.author());
  installedVersion.libraryLocation = tlp::tlpStringToQString(PluginLister::getPluginLibrary(info.name()));
  std::list<tlp::Dependency> dependencies = PluginLister::instance()->getPluginDependencies(info.name());

  for (std::list<tlp::Dependency>::iterator it = dependencies.begin(); it != dependencies.end(); ++it) {
    installedVersion.dependencies.push_back(it->pluginName.c_str());
  }

  installedVersion.isValid = true;
}


PluginVersionInformation::PluginVersionInformation(): isValid(false) {
}

PluginVersionInformation::PluginVersionInformation(const PluginVersionInformation &copy) {
  libraryLocation = copy.libraryLocation;
  author = copy.author;
  version = copy.version;
  icon = copy.icon;
  description = copy.description;
  date = copy.date;
  dependencies = copy.dependencies;
  isValid = copy.isValid;
}
