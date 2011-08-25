#include "tulip/TulipSettings.h"

TulipSettings *TulipSettings::_instance = 0;

const QString TulipSettings::RemoteLocationsConfigEntry = "app/remote_locations";
const QString TulipSettings::RecentDocumentsConfigEntry = "app/recent_documents";
const QString TulipSettings::PluginsToRemoveConfigEntry = "app/pluginsToRemove";

TulipSettings::TulipSettings(): QSettings("TulipSoftware","Tulip") {
}

TulipSettings &TulipSettings::instance() {
  if (!_instance)
    _instance = new TulipSettings;

  return *_instance;
}

QStringList TulipSettings::recentDocuments() const {
  return value(RecentDocumentsConfigEntry).toStringList();
}

void TulipSettings::addToRecentDocuments(const QString &name) {
  QList<QVariant> recentDocumentsValue = value(RecentDocumentsConfigEntry).toList();

  if (recentDocumentsValue.contains(name))
    recentDocumentsValue.removeAll(name);

  recentDocumentsValue.push_front(name);

  while (recentDocumentsValue.size() > 5)
    recentDocumentsValue.pop_back();

  setValue(RecentDocumentsConfigEntry,recentDocumentsValue);
}

void TulipSettings::addRemoteLocation(const QString& remoteLocation) {
  QStringList remoteLocations = value(TulipSettings::RemoteLocationsConfigEntry).toStringList();

  if(!remoteLocations.contains(remoteLocation)) {
    remoteLocations.append(remoteLocation);
  }

  setValue(TulipSettings::RemoteLocationsConfigEntry, remoteLocations);
}

void TulipSettings::removeRemoteLocation(const QString& remoteLocation) {
  QStringList remoteLocations = value(TulipSettings::RemoteLocationsConfigEntry).toStringList();

  if(remoteLocations.contains(remoteLocation)) {
    remoteLocations.removeOne(remoteLocation);
  }

  setValue(TulipSettings::RemoteLocationsConfigEntry, remoteLocations);
}

const QStringList TulipSettings::remoteLocations() const {
  return value(TulipSettings::RemoteLocationsConfigEntry).toStringList();
}

const QStringList TulipSettings::pluginsToRemove() const {
//   QStringList result = value(PluginsToRemoveConfigEntry).toStringList();
//   result.removeAll("");
//   return result;
  return value(PluginsToRemoveConfigEntry).toStringList();
}

void TulipSettings::markPluginForRemoval(const QString& pluginLibrary) {
  QStringList markedPlugins = value(PluginsToRemoveConfigEntry).toStringList();

  if(!markedPlugins.contains(pluginLibrary)) {
    markedPlugins.append(pluginLibrary);
  }

  setValue(PluginsToRemoveConfigEntry, markedPlugins);
}

void TulipSettings::unmarkPluginForRemoval(const QString& pluginLibrary) {
  QStringList markedPlugins = value(PluginsToRemoveConfigEntry).toStringList();

  if(markedPlugins.contains(pluginLibrary)) {
    markedPlugins.removeAll(pluginLibrary);
  }

  setValue(PluginsToRemoveConfigEntry, markedPlugins);
}
