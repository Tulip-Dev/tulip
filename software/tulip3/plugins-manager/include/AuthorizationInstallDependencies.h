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

#ifndef _Tulip_AUTHORIZATIONINSTALLDEPENDENCIES_H_
#define _Tulip_AUTHORIZATIONINSTALLDEPENDENCIES_H_

#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <vector>
#include "PluginsViewWidget.h"

namespace tlp {

  class AuthorizationInstallDependencies : public QDialog{
    Q_OBJECT
    
    private :
    bool install;
    std::set<DistPluginInfo,PluginCmp> *depToInstall;
    std::set<DistPluginInfo,PluginCmp> *toInstall;
    std::set<LocalPluginInfo,PluginCmp> *depToRemove;
    std::set<LocalPluginInfo,PluginCmp> *toRemove;

    QVBoxLayout box;
    QHBoxLayout subBox;

  public :
    AuthorizationInstallDependencies(std::set<DistPluginInfo,PluginCmp> *depToInstall,std::set<DistPluginInfo,PluginCmp> *toInstall);
    AuthorizationInstallDependencies(std::set<LocalPluginInfo,PluginCmp> *depToRemove,std::set<LocalPluginInfo,PluginCmp> *toRemove);

  public slots :
    void valid();
    void annule();
    
  };

}
#endif //_AUTHORIZATIONINSTALLDEPENDENCIES_H_
