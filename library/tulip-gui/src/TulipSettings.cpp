#include "tulip/TulipSettings.h"

TulipSettings *TulipSettings::_instance = 0;

TulipSettings::TulipSettings(): QSettings("TulipSoftware","Tulip") {
}

TulipSettings &TulipSettings::instance() {
  if (!_instance)
    _instance = new TulipSettings;

  return *_instance;
}

QStringList TulipSettings::recentDocuments() const {
  return value("app/recent_documents").toStringList();
}

void TulipSettings::addToRecentDocuments(const QString &name) {
  QList<QVariant> recentDocumentsValue = value("app/recent_documents").toList();

  if (recentDocumentsValue.contains(name))
    recentDocumentsValue.removeAll(name);

  recentDocumentsValue.push_front(name);

  while (recentDocumentsValue.size() > 5)
    recentDocumentsValue.pop_back();

  setValue("app/recent_documents",recentDocumentsValue);
}

void TulipSettings::addRemoteLocation(const QString& remoteLocation) {
  QStringList remoteLocations = value("app/remote_locations").toStringList();

  if(!remoteLocations.contains(remoteLocation)) {
    remoteLocations.append(remoteLocation);
  }

  setValue("app/remote_locations", remoteLocations);
}

void TulipSettings::removeRemoteLocation(const QString& remoteLocation) {
  QStringList remoteLocations = value("app/remote_locations").toStringList();

  if(remoteLocations.contains(remoteLocation)) {
    remoteLocations.removeOne(remoteLocation);
  }

  setValue("app/remote_locations", remoteLocations);
}

const QStringList TulipSettings::remoteLocations() const {
  return value("app/remote_locations").toStringList();
}

const QStringList TulipSettings::pluginsToRemove() const {
  return value("app/pluginsToRemove").toStringList();
}

void TulipSettings::markPluginForRemoval(const QString& pluginLibrary) {
  QStringList markedPlugins = value("app/pluginsToRemove").toStringList();

  if(!markedPlugins.contains(pluginLibrary)) {
    markedPlugins.append(pluginLibrary);
  }

  setValue("app/pluginsToRemove", markedPlugins);
}

void TulipSettings::unmarkPluginForRemoval(const QString& pluginLibrary) {
  QStringList markedPlugins = value("app/pluginsToRemove").toStringList();

  if(markedPlugins.contains(pluginLibrary)) {
    markedPlugins.removeAll(pluginLibrary);
  }

  setValue("app/pluginsToRemove", markedPlugins);
}
