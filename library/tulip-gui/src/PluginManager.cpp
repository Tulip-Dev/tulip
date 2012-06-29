#include <tulip/PluginManager.h>

#include <tulip/TulipSettings.h>
#include <tulip/PluginLister.h>
#include <tulip/SystemDefinition.h>

#include <tulip/YajlFacade.h>

#include <QtGui/QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

using namespace tlp;

QDebug operator<<(QDebug dbg, const PluginManager::PluginVersionInformations &c) {
  dbg.nospace() << "(author " << c.author << ") "
                   << "(version " << c.version << ") "
                      << "(icon " << c.icon << ") "
                         << "(description " << c.description << ") "
                            << "(date " << c.date << ") "
                               << "(librarylocation " << c.libraryLocation << ") "
                                  << "(dependencies " << c.dependencies << ") ";

  return dbg.space();
}

QDebug operator<<(QDebug dbg, const PluginManager::PluginInformations &c) {
  dbg.nospace() << "(name " << c.name << ") "
                   << "(category " << c.category << ") "
                      << "(installed " << c.installedVersion << ") "
                         << "(available " << c.availableVersions << ") ";

  return dbg.space();
}

class PluginServerClient: public YajlParseFacade {
  QString _location;

  PluginManager::PluginInformationsList _result;
  QString _currentKey;
  QMap<QString,QString> _currentMap;

public:
  PluginServerClient(const QString& location): _location(location) {
  }

  PluginManager::PluginInformationsList list(const QString& nameFilter, const QString& categoryFilter) {
    _result.clear();
    QNetworkAccessManager mgr;
    QNetworkRequest request(QUrl(_location + "/list.php?os=" + OS_PLATFORM + "&arch=" + OS_ARCHITECTURE + "&tulip=" + TULIP_MM_RELEASE + "&name=" + nameFilter + "&category=" + categoryFilter));
    QNetworkReply* reply = mgr.get(request);
    while (!reply->isFinished()) {
      QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    parse(reply);
    return _result;
  }

  virtual void parseString(const std::string& value) {
    _currentMap[_currentKey] = value.c_str();
  }

  virtual void parseMapKey(const std::string& value) {
    _currentKey = value.c_str();
  }

  virtual void parseStartMap() {
    _currentMap.clear();
  }

  virtual void parseEndMap() {
    PluginManager::PluginInformations infos;
    infos.name = _currentMap["name"];
    infos.category = _currentMap["category"];

    PluginManager::PluginVersionInformations versionInfos;
    versionInfos.description = _currentMap["desc"];
    versionInfos.libraryLocation = _location;
    versionInfos.version = _currentMap["release"];
    versionInfos.author = _currentMap["author"];
    versionInfos.date = _currentMap["date"];
    versionInfos.isValid = true;
    // TODO fill icon
    infos.availableVersions += versionInfos;
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

PluginManager::PluginInformationsList PluginManager::listPlugins(PluginLocations locations, const QString &nameFilter, const QString &categoryFilter) {
  QMap<QString,PluginInformations> nameToInfos;

  if (locations.testFlag(Local)) {
    std::list<std::string> localResults = PluginLister::instance()->availablePlugins();
    for (std::list<std::string>::iterator it = localResults.begin(); it != localResults.end(); ++it) {
      const Plugin* info = PluginLister::instance()->pluginInformations(*it);
      if (QString(info->category().c_str()).contains(categoryFilter) && QString(info->name().c_str()).contains(nameFilter)) {
        nameToInfos[info->name().c_str()].fillLocalInfos(info);
      }
    }
  }
  if (locations.testFlag(Remote)) {
    foreach(QString loc, remoteLocations()) {
      PluginServerClient client(loc);
      foreach(PluginInformations infos, client.list(nameFilter,categoryFilter)) {
        PluginInformations storedInfos = nameToInfos[infos.name];
        storedInfos.name = infos.name;
        storedInfos.category = infos.category;
        storedInfos.availableVersions += infos.availableVersions;
        nameToInfos[infos.name] = storedInfos;
      }
    }
  }

  PluginInformationsList result;
  foreach(PluginInformations i, nameToInfos.values()) {
    result.push_back(i);
  }

  return result;
}

void PluginManager::markForRemoval(const QString &plugin) {
  TulipSettings::instance().markPluginForRemoval(plugin);
}

void PluginManager::markForInstallation(const QString& plugin, const QString &version) {
}

PluginManager::PluginVersionInformationsList PluginManager::markedForInstallation() {
  return PluginVersionInformationsList();
}

QStringList PluginManager::markedForRemoval() {
  return TulipSettings::instance().pluginsToRemove();
}

PluginManager::PluginInformations::PluginInformations() {
}

PluginManager::PluginInformations::PluginInformations(const PluginManager::PluginInformations &copy) {
  name = copy.name;
  category = copy.category;
  installedVersion = copy.installedVersion;
  availableVersions = copy.availableVersions;
}

void PluginManager::PluginInformations::fillLocalInfos(const Plugin* info) {
  name = info->name().c_str();
  category = info->category().c_str();
  installedVersion.description = info->info().c_str();
  installedVersion.icon = info->icon().c_str();
  installedVersion.version = info->release().c_str();
  installedVersion.date = info->date().c_str();
  installedVersion.author = info->author().c_str();
  installedVersion.libraryLocation = PluginLister::getPluginLibrary(info->name()).c_str();
  std::list<tlp::Dependency> dependencies = PluginLister::instance()->getPluginDependencies(info->name());
  for (std::list<tlp::Dependency>::iterator it = dependencies.begin(); it != dependencies.end(); ++it) {
    installedVersion.dependencies.push_back(it->pluginName.c_str());
  }
  installedVersion.isValid = true;
}


PluginManager::PluginVersionInformations::PluginVersionInformations(): isValid(false) {
}

PluginManager::PluginVersionInformations::PluginVersionInformations(const PluginManager::PluginVersionInformations &copy) {
  libraryLocation = copy.libraryLocation;
  author = copy.author;
  version = copy.version;
  icon = copy.icon;
  description = copy.description;
  date = copy.date;
  dependencies = copy.dependencies;
  isValid = copy.isValid;
}
