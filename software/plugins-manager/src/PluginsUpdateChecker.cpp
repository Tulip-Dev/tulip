#include "PluginsUpdateChecker.h"

#include "MultiServerManager.h"
#include "UpdatePluginsDialog.h"

#include <iostream>
#include <QtCore/QSettings>
#include <QtGui/QMessageBox>

using namespace std;

namespace tlp {
  PluginsUpdateChecker::PluginsUpdateChecker(vector<LocalPluginInfo> &pluginsList,QWidget *parent):parent(parent),updatePlugin(NULL) {
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

  PluginsUpdateChecker::~PluginsUpdateChecker() {
    if(updatePlugin)
      delete updatePlugin;
  }

  MultiServerManager *PluginsUpdateChecker::getMultiServerManager(){
    return msm;
  }

  void PluginsUpdateChecker::displayPopup(const vector<DistPluginInfo*> &pluginsOutOfDate) {
    vector<DistPluginInfo*> plugins;
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("UpdatePlugins");
    QStringList keys = settings.allKeys();
    settings.endGroup();

    for(vector<DistPluginInfo*>::const_iterator it=pluginsOutOfDate.begin();it!=pluginsOutOfDate.end();++it){
      if(!keys.contains((*it)->name.c_str()))
        plugins.push_back(*it);
    }

    settings.beginGroup("UpdatePlugins");
    for(QStringList::iterator it=keys.begin();it!=keys.end();++it) {
      bool find=false;
      for(vector<DistPluginInfo*>::const_iterator it2=pluginsOutOfDate.begin();it2!=pluginsOutOfDate.end();++it2){
        if((*it2)->name==(*it).toStdString()){
          find=true;
          break;
        }
      }
      if(!find){
        settings.remove(*it);
      }
    }
    settings.endGroup();

    if(plugins.size()==0){
      return;
    }

    UpdatePluginsDialog dialog(plugins,parent);
    if(dialog.exec()==QDialog::Rejected){
      return;
    }

    set<DistPluginInfo,PluginCmp> pluginsToUpdate;
    set<LocalPluginInfo,PluginCmp> pluginsToRemove;
    dialog.getPluginsToUpdate(pluginsToUpdate);

    if(pluginsToUpdate.size()==0){
      return;
    }

    updatePlugin = new UpdatePlugin();
    connect(updatePlugin,SIGNAL(pluginInstalled()),this,SLOT(pluginInstalled()));
    connect(updatePlugin,SIGNAL(pluginUninstalled()),this,SLOT(pluginUninstalled()));
    numberOfPluginsToUpdate=updatePlugin->pluginsCheckAndUpdate(msm,pluginsToUpdate, pluginsToRemove,parent);
  }

  void PluginsUpdateChecker::pluginInstalled(){
    numberOfPluginsToUpdate--;
    if(numberOfPluginsToUpdate==0){
      emit updateFinished();
    }
  }

  void PluginsUpdateChecker::pluginUninstalled(){
    pluginInstalled();
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
#if defined(__APPLE__)
#if defined(MACPPC)
            if(pluginInfo->macVersion_ppc){
#else
            if(pluginInfo->macVersion_intel){
#endif
#elif defined(_WIN32)
            if(pluginInfo->windowsVersion){
#elif defined(I64)
            if(pluginInfo->linuxVersion_i64){
#else
            if(pluginInfo->linuxVersion){
#endif
              pluginsOutOfDate.push_back(pluginInfo);
            }
          }
        }
      }

      displayPopup(pluginsOutOfDate);
    }
  }
}
