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
#include "PluginsUpdateChecker.h"

#include "MultiServerManager.h"

#include <iostream>

using namespace std;

namespace tlp {
  PluginsUpdateChecker::PluginsUpdateChecker(vector<LocalPluginInfo> &pluginsList) {
    msm = new MultiServerManager(pluginsList);

    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    serverNumber=settings.value("serverNumber_v2",0).toInt();
    vector<string> serversAddr;
    for(int i=0;i<serverNumber;i++) {
      msm->addServer(settings.value("server_v2_"+QString::number(i),"").toString().toStdString());
    }
    settings.endGroup();
  }

  void PluginsUpdateChecker::getResponse() {
    serverNumber--;
  }
}
