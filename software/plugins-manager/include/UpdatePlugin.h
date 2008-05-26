//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_UPDATEPLUGIN_H_
#define _Tulip_UPDATEPLUGIN_H_

#include <iostream>

#include <QtCore/QDir>
#include "Server.h"
#include "Request.h"
#include "PluginInfo.h"

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE UpdatePlugin : public QObject {
  
    Q_OBJECT

    Server *serverWS;
    Server *serverGet;
    DistPluginInfo distPluginInfo;
    LocalPluginInfo localPluginInfo;
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

    void install(const std::string &serverAddr,const DistPluginInfo &pluginInfo); // Asynchronous
    void updatePartNumber();
    bool uninstall(const LocalPluginInfo &pluginInfo); // Synchronous

    static void copyFile(const QDir& oldDir,const QString& oldName,const QDir& newDir,const QString& newName);
    static void installWhenRestartTulip();

  signals :

    void pluginInstalled(UpdatePlugin*,const DistPluginInfo &);
    void installPart(const std::string&,float);
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
