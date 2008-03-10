//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/


#ifndef _Tulip_MULTISERVERMANAGER_H_
#define _Tulip_MULTISERVERMANAGER_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtCore/QObject>

#include "Server.h"
#include "Request.h"
#include "ResponseTreatment.h" 
#include "PluginsListManager.h"

namespace tlp {

  struct PluginsServer{
    std::string name;
    Server* serv;
    std::string xmlPluginList;

    PluginsServer():serv(NULL) {}
    ~PluginsServer() {
      if(serv)
	delete serv;
    }
  
  };
 
  class TLP_PLUGINSMANAGER_SCOPE MultiServerManager : public QObject, public PluginsListManager {

    Q_OBJECT
 
  public : 
    friend class GetXmlListTreatment;
    
    MultiServerManager(std::vector<LocalPluginInfo> &pluginsList);
    ~MultiServerManager();
    void addServer(const std::string &addr);
    void removeServer(int i);
    void modifyServer(int i, const std::string &name, const std::string &addr);
    void getNames(std::vector<std::string> &names);
    std::string getName(const std::string &addr);
    void getAddrs(std::vector<std::string> &address);
    void getAddr(const std::string &name, std::string &address);
    void modifyTreeView(int viewNumber);
    void sendServerConnect();
    
    void getResponse();
		      
  public slots:
    void changeName(ServerNameTreatment* treat,std::string addr,std::string name); 
    
  signals:
    
    void newPluginList();
    void nameReceived(MultiServerManager*,std::string,std::string);

  private :
    
    std::list<PluginsServer*> pluginsServersList;

    bool requestPluginList(Server* serv);
    bool requestServerName(Server* serv);
    bool requestServerConnect(Server* serv);
    bool requestPluginLists();
    
  };
  

  class TLP_PLUGINSMANAGER_SCOPE GetXmlListTreatment : public ResponseTreatment {
    
    MultiServerManager* msm;
    std::string serverAddr;
    
  public :
    
    GetXmlListTreatment(MultiServerManager* msm, std::string serverAddr) : msm(msm), serverAddr(serverAddr){
    };
    
    void operator()(char **response){
      std::string serverName;
      foreach(PluginsServer *ps,msm->pluginsServersList){
	std::string address;
	ps->serv->getAddress(address);
	if(address.compare(serverAddr)==0){
	  serverName=ps->name;
	}
      }
      std::string str=*response;
      msm->addServerList(serverName,str);
      msm->getResponse();
    };
    
  };
  
}
#endif //_Tulip_MULTISERVERMANAGER_H_

