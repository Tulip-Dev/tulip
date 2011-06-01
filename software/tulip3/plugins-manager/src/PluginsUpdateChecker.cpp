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
#include "UpdatePluginsDialog.h"
#include "NewTulipVersionAvailableDialog.h"

#include <iostream>
#include <QtCore/QSettings>
#include <QtGui/QMessageBox>

using namespace std;

namespace tlp {
  PluginsUpdateChecker::PluginsUpdateChecker(vector<LocalPluginInfo> &pluginsList,QWidget *parent):parent(parent),updatePlugin(NULL),newVersionFound(false) {
    msm = new MultiServerManager(pluginsList);

    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    serverNumber=settings.value("serverNumber_v2",0).toInt();
    vector<string> serversAddr;

    if(serverNumber!=0) {
      for(int i=0;i<serverNumber;i++) {
        msm->addServer(settings.value("server_v2_"+QString::number(i),"").toString().toStdString());
      }
    }else{
      msm->addServer("http://tulip.labri.fr/pluginsServer_Stable/server.php");
      msm->addServer("http://tulip.labri.fr/pluginsServer_Testing/server.php");
    }
    settings.endGroup();

    msm->sendServerConnect();
    msm->requestTulipLastVersionNumber();

    connect(msm,SIGNAL(newPluginList()),this,SLOT(getResponse()));
    connect(msm,SIGNAL(versionReceived(std::string)),this,SLOT(versionReceived(std::string)));
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

    if(plugins.empty()){
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
          QString majorVersion(pluginInfo->localVersion.c_str());


          QStringList tmp=majorVersion.split(".");
          majorVersion="";
          if(tmp.size()>2){
            majorVersion=tmp[0]+"."+tmp[1];
          }

          if((pluginInfo->version.compare(pluginInfo->localVersion)>0) && QString(pluginInfo->version.c_str()).startsWith(majorVersion) && (pluginInfo->localVersion!="")) {
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
              bool find=false;
              for(vector<DistPluginInfo*>::iterator it2=pluginsOutOfDate.begin();it2!=pluginsOutOfDate.end();++it2){
                if((*it2)->name==pluginInfo->name){
                  find=true;
                  if((*it2)->version.compare(pluginInfo->version)<0){
                    (*it2)=pluginInfo;
                  }
                }
              }

              if(!find)
                pluginsOutOfDate.push_back(pluginInfo);
            }
          }
        }
      }

      displayPopup(pluginsOutOfDate);
    }
  }

  void PluginsUpdateChecker::versionReceived(std::string version)  {
    if(newVersionFound)
      return;
    QString versionNumber(version.c_str());
    if(versionNumber.count(".")!=2)
      return;

    if(versionNumber>QString(TULIP_RELEASE)){
      if(NewTulipVersionAvailableDialog::needDisplayDialog(versionNumber)){
        NewTulipVersionAvailableDialog dialog(versionNumber);
        dialog.exec();
        newVersionFound=true;
      }
    }
  }
}
