#include "PluginsUpdateChecker.h"

#include "MultiServerManager.h"

#include <iostream>
#include <QtCore/QSettings>
#include <QtGui/QMessageBox>

using namespace std;

namespace tlp {
  PluginsUpdateChecker::PluginsUpdateChecker(vector<LocalPluginInfo> &pluginsList,QWidget *parent):parent(parent) {
    msm = new MultiServerManager(pluginsList);

    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    serverNumber=settings.value("serverNumber",0).toInt();
    vector<string> serversAddr;
    if(serverNumber!=0) {
      for(int i=0;i<serverNumber;i++) {
	msm->addServer(settings.value("server"+QString::number(i),"").toString().toStdString());
      }
    }else{
      msm->addServer("http://tulip.labri.fr/pluginsServer/server.php");
    }
    settings.endGroup();

    msm->sendServerConnect();

    connect(msm,SIGNAL(newPluginList()),this,SLOT(getResponse()));
  }

  MultiServerManager *PluginsUpdateChecker::getMultiServerManager(){
    return msm;
  }

  void PluginsUpdateChecker::displayPopup(const vector<DistPluginInfo*> &pluginsOutOfDate) {
    QMessageBox messageBox(QMessageBox::Information,"Update avalaible","Update available for plugins",QMessageBox::Ok,parent);
    messageBox.exec();
    emit checkFinished();
  }

  void PluginsUpdateChecker::getResponse() {
    serverNumber--;
    if(serverNumber==0) {
      vector<DistPluginInfo*> pluginsOutOfDate;
      CompletePluginsList pluginsList;
      msm->getPluginsList(pluginsList);
      for(CompletePluginsList::iterator it=pluginsList.begin();it!=pluginsList.end();++it) {
	if(!(*it).first->local) {
	  DistPluginInfo *pluginInfo=(DistPluginInfo*)((*it).first);
	  if(pluginInfo->version.compare(pluginInfo->localVersion)>0 && pluginInfo->localVersion!="") {
	    pluginsOutOfDate.push_back(pluginInfo);
	  }
	}
      }

      if(pluginsOutOfDate.size()!=0) {
        displayPopup(pluginsOutOfDate);
      }
    }
  }
}
