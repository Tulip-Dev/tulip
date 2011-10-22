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
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <tulip/PluginInformations.h>

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtNetwork/QNetworkReply>

class QNetworkReply;
static const QString PROPERTY_ALGORITHM_PLUGIN_NAME = "PropertyAlgorithm";
static const QString ALGORITHM_PLUGIN_NAME = "Algorithm";
static const QString TEMPLATE_ALGORITHM_PLUGIN_NAME = "TemplateAlgorithm";

static const QString IMPORT_PLUGIN_NAME = "ImportModule";
static const QString EXPORT_PLUGIN_NAME = "ExportModule";

static const QString GLYPH_PLUGIN_NAME = "Glyph";
static const QString EE_GLYPH_PLUGIN_NAME = "EdgeExtremityGlyph";

static const QString VIEW_PLUGIN_NAME = "View";
static const QString INTERACTOR_PLUGIN_NAME = "Interactor";
static const QString PERSPECTIVE_PLUGIN_NAME = "Perspective";


namespace tlp {

class AbstractPluginInfo;

typedef QMap<QString, tlp::DistantPluginInfo*> LocationPlugins;

/**
 * @brief This class allows to easily install, remove, update, and list plugins (both installed and available).
 *
 * For now only listing is available, the rest is coming soon.
 **/
class TLP_QT_SCOPE PluginManager : public QObject {
  Q_OBJECT
public:

  enum PluginLocation {
    All = 3,
    Local = 1,
    Remote = 2
  };
  Q_DECLARE_FLAGS(Location, PluginLocation)

  /**
   * @brief Lists plugins from the specified locations.
   *
   * @param list The locations from which to list plugins (All, Remote or Local). Defaults to All.
   * @return :PluginInformations* > The list of plugins available (or installed) at the specified location.
   **/
  static QList< PluginInformations* > pluginsList(tlp::PluginManager::Location list = All);

  /**
   * @brief Adds a remote location from which to list plugins.
   *
   * @param location The URL of the remote location (e.g. http://www.labri.fr/perso/huet/archive/ for testing purposes)
   * @return bool whether the adding of the remote location suceeded.
   * TODO This needs to be changed to an enum or whatever so what happened can be known (could not contact remote server, location already in list, ...)
   **/
  static void addRemoteLocation(const QString& location);

  /**
   * @brief Removes a remote location from which to list plugins.
   *
   * @param location The URL of the remote location from which we do not want plugins to be listed anymore.
   * @return void
   **/
  static void removeRemoteLocation(const QString& location);

  /**
   * @brief Parses the a server description's xml file and creates a list of PluginInformations from it.
   *
   * @param description The contents of a serverDescvription.xml file.
   * @return LocationPlugins The list of plugins contained in this server description.
   **/
  static LocationPlugins parseDescription(const QString& xmlDescription, const QString& location);

  /**
   * @brief Removes the plugins marked for removal from the filesystem.
   *
   * Plugins are marked for removal as loaded plug-ins may be difficult to remove at run-time.
   * They are removed before the plugin loading occurs at the next Tulip startup.
   *
   * @return void
   **/
  static void removePlugins();

  /**
   * @brief Unpacks the plugins in the specified folder to Tulip's custom folder directory.
   *
   * @param inputFolder The folder from which to unpack the plugins.
   * @return void
   **/
  static void unpackPlugins(const QString& inputFolder);

  static PluginManager* getInstance();

signals:
  /**
   * @brief Emitted when a new remote location has been sucessfully added (the plugin server description has been downloaded and parsed).
   *
   * @return void
   **/
  void remoteLocationAdded(QString);
  void errorAddRemoteLocation(QNetworkReply::NetworkError,QString);

private:
  /**
   * @brief Contains all the remote locations added, and for each of them the list of plugins on the location.
   **/
  static QMap<QString, LocationPlugins> _remoteLocations;

  PluginManager();
  static PluginManager* _instance;

  static QMap<QNetworkReply*, QString> replyLocations;

protected slots:
  /**
   * @brief This slot is called when the pluginserver description file has finished downloading.
   * This allows for asynchronous calls, and does not blocks the UI while downloading (e.g. for slow connections).
   *
   * @param reply The QNetworkReply associated with the description download request.
   * @return void
   **/
  void serverDescriptionDownloaded(QNetworkReply* reply);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManager::Location)

}
#endif //PLUGINMANAGER_H
