/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#ifndef _Tulip_UPDATEPLUGIN_H_
#define _Tulip_UPDATEPLUGIN_H_

#include <iostream>

#include <QtCore/QDir>
#include "Server.h"
#include "Request.h"
#include "PluginInfo.h"

namespace tlp {

  class MultiServerManager;
  class InstallPluginDialog;

  class TLP_PLUGINSMANAGER_SCOPE UpdatePlugin : public QObject {

    Q_OBJECT

    Server *serverWS;
    Server *serverGet;
    DistPluginInfo distPluginInfo;
    LocalPluginInfo localPluginInfo;
    InstallPluginDialog *pluginDialog;
    QList<UpdatePlugin*> pluginUpdaters;
    MultiServerManager *msm;
    bool openDialog;
    std::string version;
    std::string installPath;
    int partNumber;
    int currentPart;

    UpdatePlugin(UpdatePlugin &);

  public :

    virtual void endInstallation();
    virtual void endUninstallation(){
      emit pluginUninstalled(this,localPluginInfo);
    }

    UpdatePlugin(QObject *parent=0);

    int pluginsCheckAndUpdate(MultiServerManager *msm,std::set<DistPluginInfo,PluginCmp> &pluginsToInstall, std::set<LocalPluginInfo,PluginCmp> &pluginsToRemove,QWidget *parent);
    static void windowToDisplayError(std::string pluginName,QWidget *parent);
    std::string getAddr(std::string name);

    void install(const std::string &serverAddr,const DistPluginInfo &pluginInfo); // Asynchronous
    void updatePartNumber();
    bool uninstall(const LocalPluginInfo &pluginInfo); // Synchronous

    static void copyFile(const QDir& oldDir,const QString& oldName,const QDir& newDir,const QString& newName);
    static void installWhenRestartTulip();
    static bool isInstallDirWritable();
    static bool pluginUpdatesPending();

  protected slots:

    void terminatePluginInstall(UpdatePlugin*,const DistPluginInfo &);
    void terminatePluginUninstall(UpdatePlugin*,const LocalPluginInfo &);

  signals :

    void pluginInstalled();
    void pluginUninstalled();
    void pluginInstalled(UpdatePlugin*,const DistPluginInfo &);
    void installPart(const std::string&,int,int);
    void pluginUninstalled(UpdatePlugin*,const LocalPluginInfo &);

  };

  class TLP_PLUGINSMANAGER_SCOPE PluginDownloadFinish : public RequestFinish {
    UpdatePlugin* up;

  public :
    PluginDownloadFinish(UpdatePlugin* up) : up(up){};

    void operator()(){
      up->updatePartNumber();
    }
  };

  class TLP_PLUGINSMANAGER_SCOPE EndPluginDownloadFinish : public RequestFinish {
    UpdatePlugin* up;

  public :
    EndPluginDownloadFinish(UpdatePlugin* up) : up(up){};

    void operator()(){
      up->updatePartNumber();
      up->endInstallation();
    }
  };

}

#endif // _UPDATEPLUGIN_H_
