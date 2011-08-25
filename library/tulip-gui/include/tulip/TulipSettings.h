#ifndef TULIPSETTINGS_H
#define TULIPSETTINGS_H

#include <QtCore/QSettings>
#include <tulip/tulipconf.h>
#include <QStringList>

/**
  * @brief This class provides convenience functions to access the Tulip settings file (using QSettings)
  * TulipSettings is a wrapper for QSettings providing quick access to common keys provided in the tulip configuration file.
  * This object does not mask any method from the QSettings class, which mean that the user can still access custom keys by invoking the QSettings::value method.
  */
class TLP_QT_SCOPE TulipSettings: public QSettings {
public:

  static const QString RemoteLocationsConfigEntry;
  static const QString RecentDocumentsConfigEntry;
  static const QString PluginsToRemoveConfigEntry;
  
  static TulipSettings &instance();

  /**
    @brief Retrieves the list of documents recently opened with tulip.
    */
  QStringList recentDocuments() const;

  /**
    @brief Registers a file in the list of recently opened documents.
    If the list already contains 5 elements, the last one (in chronological order) will be removed.
    */
  void addToRecentDocuments(const QString &);

  /**
   * @brief Adds a remote location from which to list plugins.
   *
   * @param remoteLocation The URL of the plugin server.
   * @return void
   **/
  void addRemoteLocation(const QString& remoteLocation);

  /**
   * @brief Removes a location from which to list plugins.
   *
   * @param remoteLocation The URL of the plugin server.
   * @return void
   **/
  void removeRemoteLocation(const QString& remoteLocation);

  /**
   * @brief Retrieves the list of locations from which to list plugins.
   *
   * @return const QStringList
   **/
  const QStringList remoteLocations() const;

  const QStringList pluginsToRemove() const;

  void markPluginForRemoval(const QString& pluginLibrary);
  void unmarkPluginForRemoval(const QString& pluginLibrary);

private:
  TulipSettings();

  static TulipSettings *_instance;
};

#endif // TULIPSETTINGS_H
