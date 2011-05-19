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

#include <tulip/AbstractPluginInfo.h>

namespace tlp {

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

class TLP_QT_SCOPE LocalPluginInformations : public PluginInformationsInterface {
  public:
    LocalPluginInformations(const tlp::AbstractPluginInfo* info, const std::string& type, const std::list<Dependency>& dependencies, const std::string& library);

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
