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
#include <tulip/AbstractPluginInfo.h>

namespace tlp {
  class Dependency;

/**
  * @brief This implementation of AbstractPluginInfo represents a plugin located on a remote server, thus not yet installed.
  * All the informations are 'manually' entered into this class.
  *
  **/
class TLP_QT_SCOPE DistantPluginInfo : public tlp::AbstractPluginInfo {
public:
  DistantPluginInfo(const std::string& author, const std::string& date, const std::string& group, const std::string& name, const std::string& info, const std::string& release, const std::string& tulipRelease)
  : _author(author), _date(date), _group(group), _name(name), _info(info), _release(release), _tulipRelease(tulipRelease) {
  }
  virtual std::string getAuthor() const {
    return _author;
  }
  virtual std::string getDate() const {
    return _date;
  }
  virtual std::string getGroup() const {
    return _group;
  }
  virtual std::string getName() const {
    return _name;
  }
  virtual std::string getInfo() const {
    return _info;
  }
  virtual std::string getRelease() const {
    return _release;
  }
  virtual std::string getTulipRelease() const {
    return _tulipRelease;
  }
private:
  const std::string& _author;
  const std::string& _date;
  const std::string& _group;
  const std::string& _name;
  const std::string& _info;
  const std::string& _release;
  const std::string& _tulipRelease;
};
  
/**
 * @brief This class describes the whereabouts of a plugin, be it locally installed, or on a remote server.
 *
 * This class is meant to describe a plug-in so a comprehensive list can be displayed, i.e. to manage plug-ins installation.
 **/
class TLP_QT_SCOPE PluginInformationsInterface {
  public:
    virtual QString identifier() const = 0;
    virtual QString name() const = 0;

    virtual QString shortDescription() const = 0;
    virtual QString longDescriptionPath() const = 0;

    virtual QString iconPath() const = 0;
    virtual QDateTime installDate() const = 0;
    
    virtual QString type() const = 0;
    virtual const QStringList& dependencies() const = 0;
    virtual QString version() const = 0;
    
  protected:
    PluginInformationsInterface() {}
};

/**
 * @brief Straightforward implementation of PluginInformationsInterface, useable for both remote and local plugins.
 **/
class TLP_QT_SCOPE PluginInformations : public PluginInformationsInterface {
  public:
    /**
     * @brief This constructor is used for local plugin description, and the library is used to determine the path of the icon and long description.
     *
     * @param info The AbstractPluginInfo representing the plugin to describe.
     * @param type The type of the plugin.
     * @param dependencies The dependencies of the plugin.
     * @param library The library file from which the plugin was loaded.
     **/
    PluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list<tlp::Dependency>& dependencies, const std::string& library);

    /**
     * @brief This constructor is used for remote plugin description, the long description and icon's paths are directly provided.
     *
     * @param info The AbstractPluginInfo representing the plugin to describe.
     * @param type The type of the plugin.
     * @param dependencies The dependencies of the plugin.
     * @param longDescriptionPath The URL where the long description resides.
     * @param iconPath The URL where the icon resides.
     **/
    PluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list<tlp::Dependency>& dependencies, const QString& longDescriptionPath, const QString& iconPath);

    virtual QString identifier() const;
    virtual QString name() const;
    
    virtual QString shortDescription() const;
    virtual QString longDescriptionPath() const;
    
    virtual QString iconPath() const; 
    virtual QDateTime installDate() const;
    
    virtual QString type() const;
    virtual const QStringList& dependencies() const;
    virtual QString version() const;
    
  private:
    const AbstractPluginInfo* _infos;
    const QString _type;
    QStringList _dependenciesNames;
    const QString _iconPath;
    const QString _longDescriptionPath;

    const std::list<Dependency> _dependencies;
};

}

#endif //PLUGININFORMATIONS_H
