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

#include <tulip/tulipconf.h>

#include <QtCore/QStringList>

class QNetworkReply;

namespace tlp {

class Plugin;

class TLP_QT_SCOPE PluginManager {
public:
  enum PluginLocation {
    Remote = 0b01,
    Local = 0b10
  };
  Q_DECLARE_FLAGS(PluginLocations, PluginLocation)

  struct PluginVersionInformations {
    bool isValid;

    QString libraryLocation;
    QString author;
    QString version;
    QString icon;
    QString description;
    QString date;

    QStringList dependencies;

    PluginVersionInformations();
    PluginVersionInformations(const PluginVersionInformations& copy);
  };
  typedef QList<PluginVersionInformations> PluginVersionInformationsList;

  struct PluginInformations {
    QString name;
    QString category;
    PluginVersionInformations installedVersion;
    PluginVersionInformationsList availableVersions;

    PluginInformations();
    PluginInformations(const PluginInformations& copy);

    void fillLocalInfos(const tlp::Plugin*info);
  };
  typedef QList<PluginInformations> PluginInformationsList;

  static void addRemoteLocation(const QString& location);
  static void removeRemoteLocation(const QString& location);
  static QStringList remoteLocations();

  static PluginInformationsList listPlugins(PluginLocations locations,const QString& nameFilter = QString(),const QString& categoryFilter = QString());

  static void markForRemoval(const QString& plugin);
  static void markForInstallation(const QString& plugin,const QString& version);

  static PluginVersionInformationsList markedForInstallation();
  static QStringList markedForRemoval();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManager::PluginLocations)
}

#endif //PLUGINMANAGER_H
