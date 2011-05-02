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

#include <QDate>
#include <QIcon>

/**
 * @brief This class describes the whereabouts of a plugin, be it locally installed, or on a remote server.
 *
 * This class is meant to describe a plug-in so a comprehensive list can be displayed, i.e. to manage plug-ins installation.
 *
 * This is a WIP for Tulip 4.0 and serves no real purpose as of now.
 **/
class PluginInformations {
  public:   
    QString identifier() const;
    QString name() const;

    QString shortDescription() const;
    QString longDescriptionPath() const;

    QString iconPath() const;
    QDate installDate() const;
    
    QString type() const;
    QStringList dependencies() const;
    QString version() const;
};

#endif //PLUGININFORMATIONS_H
