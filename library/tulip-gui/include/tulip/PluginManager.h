#include <tulip/PluginInformations.h>
#include <tulip/WithDependency.h>

#include <QtCore/QList>
#include <QtCore/QMap>

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

namespace tlp {

  class AbstractPluginInfo;

  typedef QMap<QString, tlp::PluginInformations*> LocationPlugins;
  
  /**
   * @brief This class allows to easily install, remove, update, and list plugins (both installed and available).
   *
   * For now only listing is available, the rest is coming soon.
   **/
  class TLP_QT_SCOPE PluginManager :QObject {
  Q_OBJECT
public:
  
  enum PluginLocation {
    All = 3,
    Local = 1,
    Remote = 2
  };
  Q_DECLARE_FLAGS(Location, PluginLocation);
  
  /**
   * @brief Lists plugins from the specified locations.
   *
   * @param list The locations from which to list plugins (All, Remote or Local). Defaults to All.
   * @return :PluginInformations* > The list of plugins available (or installed) at the specified location.
   **/
  QList< PluginInformations* > pluginsList(tlp::PluginManager::Location list = All);

  /**
   * @brief Adds a remote location from which to list plugins.
   *
   * @param location The URL of the remote location (e.g. http://www.labri.fr/perso/huet/archive/ for testing purposes)
   * @return void
   **/
  static void addRemoteLocation(const QString& location);
  
  /**
   * @brief Retrieves the serverDescription.xml file from the specified location and returns its contents.
   *
   * @param location The location from which to retrieve the serverDescription.xml file.
   * @return QString The contents of the serverDescription.xml file.
   **/
  static QString getPluginServerDescription(const QString& location);
  
  /**
   * @brief Parses the a server description's xml file and creates a list of PluginInformations from it.
   *
   * @param description The contents of a serverDescvription.xml file.
   * @return LocationPlugins The list of plugins contained in this server description.
   **/
  static LocationPlugins parseDescription(const QString& xmlDescription);

private:
  /**
   * @brief Contains all the remote locations added, and for each of them the list of plugins on the location.
   **/
  static QMap<QString, LocationPlugins> _remoteLocations;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManager::Location)

}
#endif //PLUGINMANAGER_H
