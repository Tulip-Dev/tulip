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
