/*
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
///@cond DOXYGEN_HIDDEN

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QStringList>

#include <tulip/tulipconf.h>

class QNetworkReply;

namespace tlp {

class Plugin;

struct TLP_QT_SCOPE PluginVersionInformation {
  bool isValid;

  QString libraryLocation;
  QString author;
  QString version;
  QString icon;
  QString description;
  QString date;

  QStringList dependencies;

  PluginVersionInformation();
  PluginVersionInformation(const PluginVersionInformation& copy);
};

// deprecated name
_DEPRECATED_TYPEDEF(PluginVersionInformation, PluginVersionInformations);

struct TLP_QT_SCOPE PluginInformation {
  QString name;
  QString category;
  PluginVersionInformation installedVersion;
  PluginVersionInformation availableVersion;

  PluginInformation();
  PluginInformation(const PluginInformation& copy);

  void fillLocalInfos(const tlp::Plugin& info);
};

// deprecated name
_DEPRECATED_TYPEDEF(PluginInformation, PluginInformations);

class TLP_QT_SCOPE PluginManager {
  static QStringList _markedForInstallation;

public:
  enum PluginLocation {
    Remote = 0x01,
    Local = 0x02
  };
  Q_DECLARE_FLAGS(PluginLocations, PluginLocation)

  static const QString STABLE_LOCATION;
  static const QString TESTING_LOCATION;

  typedef QList<PluginVersionInformation> PluginVersionInformationList;
  _DEPRECATED_TYPEDEF(QList<PluginVersionInformation>, PluginVersionInformationsList);

  typedef QList<PluginInformation> PluginInformationList;
  _DEPRECATED_TYPEDEF(QList<PluginInformation>, PluginInformationsList);

  static void addRemoteLocation(const QString& location);
  static void removeRemoteLocation(const QString& location);
  static QStringList remoteLocations();

  static PluginInformationList listPlugins(PluginLocations locations,const QString& nameFilter = QString(),const QString& categoryFilter = QString());

  static void markForRemoval(const QString& plugin);
  static void markForInstallation(const QString& plugin, QObject *recv, const char* progressSlot);

  static QStringList markedForInstallation();
  static QStringList markedForRemoval();
  static void unmarkForRemoval(const QString& file);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManager::PluginLocations)
}

#endif //PLUGINMANAGER_H
///@endcond
