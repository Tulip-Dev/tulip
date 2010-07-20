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
    bool newVersionFound;

  public :

    PluginsUpdateChecker(std::vector<LocalPluginInfo> &pluginsList,QWidget *parent);
    ~PluginsUpdateChecker();
    MultiServerManager *getMultiServerManager();

  private slots :

    void getResponse();
    void pluginInstalled();
    void pluginUninstalled();
    void versionReceived(std::string);

  private :

    void displayPopup(const std::vector<DistPluginInfo*> &pluginsOutOfDate);

  signals :

    void updateFinished();

  };

}
#endif //_Tulip_PLUGINSUPDATECHECKER_H_

