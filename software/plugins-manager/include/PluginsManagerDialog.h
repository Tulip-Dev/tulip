//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_PLUGINSMANAGERDIALOG_H
#define _Tulip_PLUGINSMANAGERDIALOG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtGui/QDialog>

#include "PluginsManagerMainWindow.h"

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE PluginsManagerDialog : public QDialog{
    Q_OBJECT
    
  public:
    
    void createWidget(PluginsManagerMainWindow *pluginsManager);
    PluginsManagerDialog(std::vector<LocalPluginInfo> &plugins,QWidget *parent);
    PluginsManagerDialog(MultiServerManager *msm,QWidget *parent);

  };

}

#endif
