//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/


#ifndef _Tulip_PLUGINSUPDATECHECKER_H_
#define _Tulip_PLUGINSUPDATECHECKER_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtCore/QObject>
#include <vector>

#include "PluginInfo.h"
#include "UpdatePlugin.h"

namespace tlp {

  class MultiServerManager;

  class TLP_PLUGINSMANAGER_SCOPE PluginsUpdateChecker : public QObject {

    Q_OBJECT

    QWidget *parent;
    MultiServerManager *msm;
    int serverNumber;
    UpdatePlugin *updatePlugin;
    int numberOfPluginsToUpdate;

  public :

    PluginsUpdateChecker(std::vector<LocalPluginInfo> &pluginsList,QWidget *parent);
    ~PluginsUpdateChecker();
    MultiServerManager *getMultiServerManager();

  private slots :

    void getResponse();
    void pluginInstalled();
    void pluginUninstalled();

  private :

    void displayPopup(const std::vector<DistPluginInfo*> &pluginsOutOfDate);

  signals :

    void updateFinished();

  };

}
#endif //_Tulip_PLUGINSUPDATECHECKER_H_

