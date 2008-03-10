//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/


#ifndef _Tulip_CHECKUPDATE_H_
#define _Tulip_CHECKUPDATE_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtCore/QObject>

namespace tlp {

  class MultiServerManager;
 
  class TLP_PLUGINSMANAGER_SCOPE CheckUpdate : public QObject {

    Q_OBJECT

    MultiServerManager *msm;
    int serverNumber;
 
  public : 
    
    CheckUpdate(std::vector<LocalPluginInfo> &pluginsList);

  private slots :

    void getResponse();

  };
  
}
#endif //_Tulip_MULTISERVERMANAGER_H_

