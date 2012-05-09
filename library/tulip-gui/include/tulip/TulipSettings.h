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

#ifndef TULIPSETTINGS_H
#define TULIPSETTINGS_H

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <tulip/tulipconf.h>
#include <tulip/Graph.h>
#include <tulip/Color.h>
#include <tulip/Size.h>

/**
  * @brief This class provides convenience functions to access the Tulip settings file (using QSettings)
  * TulipSettings is a wrapper for QSettings providing quick access to common keys provided in the tulip configuration file.
  * This object does not mask any method from the QSettings class, which mean that the user can still access custom keys by invoking the QSettings::value method.
  */
class TLP_QT_SCOPE TulipSettings: public QSettings {
  Q_OBJECT
  Q_ENUMS(DisplayProperty)

public:

  static const QString RemoteLocationsConfigEntry;
  static const QString RecentDocumentsConfigEntry;
  static const QString PluginsToRemoveConfigEntry;
  static const QString DefaultColorConfigEntry;
  static const QString DefaultSizeConfigEntry;
  static const QString DefaultShapeConfigEntry;
  static const QString DefaultSelectionColorEntry;

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
    @brief Check recently opened documents list and remove non-existing ones
    */
  void checkRecentDocuments();

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

  tlp::Color defaultColor(tlp::ElementType elem);
  void setDefaultColor(tlp::ElementType elem, const tlp::Color& color);
  tlp::Size defaultSize(tlp::ElementType elem);
  void setDefaultSize(tlp::ElementType elem, const tlp::Size& size);
  int defaultShape(tlp::ElementType elem);
  void setDefaultShape(tlp::ElementType elem, int shape);

  tlp::Color defaultSelectionColor();
  void setDefaultSelectionColor(const tlp::Color& color);

private:
  TulipSettings();

  static TulipSettings *_instance;
};

#endif // TULIPSETTINGS_H
