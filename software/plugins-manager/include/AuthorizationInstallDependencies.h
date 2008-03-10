//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_AUTHORIZATIONINSTALLDEPENDENCIES_H_
#define _Tulip_AUTHORIZATIONINSTALLDEPENDENCIES_H_

#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <vector>
#include "PluginsViewWidget.h"

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE AuthorizationInstallDependencies : public QDialog{
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
