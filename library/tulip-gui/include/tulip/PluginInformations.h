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

#ifndef PLUGININFORMATIONS_H
#define PLUGININFORMATIONS_H

#include <QtCore/QDateTime>
#include <QtGui/QIcon>

#include <tulip/tulipconf.h>
#include <tulip/Plugin.h>
#include <tulip/WithDependency.h>
#include <QMap>

class QNetworkReply;
namespace tlp {

/**
  * @brief This implementation of Plugin represents a plugin located on a remote server, thus not yet installed.
  * All the informations are 'manually' entered into this class.
  *
  **/
class TLP_QT_SCOPE DistantPluginInfo : public tlp::Plugin {
public:
  std::string category() const {
    return _type.toStdString();
  }

  DistantPluginInfo(const std::string& author, const std::string& date, const std::string& group, const std::string& name, const std::string& info, const std::string& release,
                    const std::string& tulipRelease, const std::list<tlp::Dependency> dependencies, const QString& type, const QString& location, const QString& remotepluginName)
    : _author(author), _date(date), _group(group), _name(name), _info(info), _release(release), _tulipRelease(tulipRelease),
      _type(type), _location(location), _remotePluginName(remotepluginName) {
    _dependencies = dependencies;
  }
  virtual std::string author() const {
    return _author;
  }
  virtual std::string date() const {
    return _date;
  }
  virtual std::string group() const {
    return _group;
  }
  virtual std::string name() const {
    return _name;
  }
  virtual std::string info() const {
    return _info;
  }
  virtual std::string release() const {
    return _release;
  }
  virtual std::string tulipRelease() const {
    return _tulipRelease;
  }
  virtual const QString& type() const {
    return _type;
  }
  virtual const QString& location() const {
    return _location;
  }
  virtual const QString& remotePluginName() const {
    return _remotePluginName;
  }

private:
  const std::string _author;
  const std::string _date;
  const std::string _group;
  const std::string _name;
  const std::string _info;
  const std::string _release;
  const std::string _tulipRelease;
  const QString _type;
  const QString _location;
  const QString _remotePluginName;
};

/**
 * @brief Simple wrapper around Plugin to provide informations about updates and installation/removal functions.
 **/
class TLP_QT_SCOPE PluginInformations : public QObject {
  Q_OBJECT
public:
  /**
    * @brief This constructor is used for local plugin description, and the library is used to determine the path of the icon and long description.
    *
    * @param info The Plugin representing the plugin to describe.
    * @param type The type of the plugin.
    * @param dependencies The dependencies of the plugin.
    * @param library The library file from which the plugin was loaded.
    **/
  PluginInformations(const tlp::Plugin* info, const std::string& type, const std::string& library);

  /**
    * @brief This constructor is used for remote plugin description, the long description and icon's paths are directly provided.
    *
    * @param info The Plugin representing the plugin to describe.
    * @param type The type of the plugin.
    * @param dependencies The dependencies of the plugin.
    * @param longDescriptionPath The URL where the long description resides.
    * @param iconPath The URL where the icon resides.
    **/
  PluginInformations(const tlp::Plugin* info, const QString& type, const QString& basePath, const QString& remotepluginName);

  /**
   * @brief This is used when a plugin is installed and available on a remote server.
   * If the remote version is greater than the local one, consider an update is available.
   *
   * @param info The PluginInfo of the remote plugin.
   * @return void
   **/
  void AddPluginInformations(const tlp::Plugin* info);

  QString identifier() const;
  //TODO this should be a displayname, not the name used to register into the plugin system
  QString name() const;
  QString author() const;
  QString group() const;

  QString shortDescription() const;
  QString longDescriptionPath() const;

  QString iconPath() const;
  QDateTime installDate() const;

  QString type() const;
  const QStringList dependencies() const;
  const QString& version() const;

  QString installedVersion() const;
  bool isInstalled() const;
  bool updateAvailable() const;

  QString latestVersion() const;

  /**
   * @brief Downloads the plugin and marks it for installation
   *
   * @return bool Whether the download was successful.
   **/
  bool fetch() const;

  /**
   * @brief Marks the plugin for removal (the plugin can only be removed once unloaded, so it will be removed at Tulip's next startup).
   *
   * @return void
   **/
  void remove() const;
signals:
  void DownloadStarted(QNetworkReply* reply) const;
private:
  QString _lastVersion;
  const QString _type;
  const QString _iconPath;
  const QString _longDescriptionPath;
  const bool _isLocal;
  const QString _installedVersion;
  bool _updateAvailable;
  const QString _remoteLocation;
  const QString _remoteArchive;
  const QString _version;
  const tlp::Plugin* _infos;
  const QString _library;
public slots:
  void downloadFinished();
};

}

#endif //PLUGININFORMATIONS_H
