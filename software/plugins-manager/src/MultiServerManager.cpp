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
#include "MultiServerManager.h"

using namespace std;

namespace tlp {

  MultiServerManager::MultiServerManager(vector<LocalPluginInfo> &pluginsList):PluginsListManager(pluginsList) {
  }

  MultiServerManager::~MultiServerManager() {
    for(list<PluginsServer*>::iterator it=pluginsServersList.begin();it!=pluginsServersList.end();++it) {
      delete (*it);
    }
  }

  void MultiServerManager::addServer(const string& addr){
    //TODO: ajouter test de validité du serveur (connexion possible)
    if(!(addr=="")){
      PluginsServer *plugServer=new PluginsServer;
      plugServer->name = addr;
      Server* serv = new Server(addr);
      plugServer->serv = serv;
      
      pluginsServersList.push_back(plugServer);
      
      //Ask for pluginList
      requestServerName(serv);
      requestPluginList(serv);
    }
  }
  
  void MultiServerManager::modifyServer(int i, const string &name, const string &addr) {
    int k = 0;
    if((unsigned int)i >= pluginsServersList.size()) return;
    
    for (list<PluginsServer*>::iterator iter=pluginsServersList.begin(); iter != pluginsServersList.end() ; ++iter, ++k){
      if (k == i){
	delete (*iter)->serv;
	(*iter)->name = name;
	Server* serv = new Server(addr);
	(*iter)->serv = serv;
	return;
      }
    }
    throw "Out of bound";
  }
  
  void MultiServerManager::removeServer(int i){
    int k = 0;
    if((unsigned int)i >= pluginsServersList.size()) return;
    
    for (list<PluginsServer*>::iterator iter=pluginsServersList.begin(); iter != pluginsServersList.end() ; ++iter, ++k){
      if (k == i){
	removeServerList((*iter)->name);
	delete (*iter)->serv;
	pluginsServersList.erase(iter);
	return;
      }
    }
    throw "Out of bound";
  }
  
  void MultiServerManager::getAddr(const string &name, string &address){
    string addr="";
    for (list<PluginsServer*>::iterator iter=pluginsServersList.begin(); iter != pluginsServersList.end() ; ++iter){
      if(((*iter)->name).compare(name)==0){
	((*iter)->serv)->getAddress(addr);
      }
    }
    address=addr;
  }
  
  void MultiServerManager::getNames(vector<string> &names){
    for (list<PluginsServer*>::iterator iter=pluginsServersList.begin(); iter != pluginsServersList.end() ; ++iter){
      names.push_back((*iter)->name);
    }
  }

  string MultiServerManager::getName(const string &addr) {
    string tmp;
    for (list<PluginsServer*>::iterator iter=pluginsServersList.begin(); iter != pluginsServersList.end() ; ++iter){
      (*iter)->serv->getAddress(tmp);
      if(tmp==addr)
	return (*iter)->name;
    }
    return addr;
  }
  
  void MultiServerManager::getAddrs(vector<string> &address){
    string addr;
    for (list<PluginsServer*>::iterator iter=pluginsServersList.begin(); iter != pluginsServersList.end() ; ++iter){
      (*iter)->serv->getAddress(addr);
      address.push_back(addr);
    }
  }
  
  void MultiServerManager::modifyTreeView(int viewNumber){
    switchView((ViewType)viewNumber);
  }

  void MultiServerManager::sendServerConnect(){
    for (list<PluginsServer*>::iterator it=pluginsServersList.begin(); it != pluginsServersList.end() ; ++it){
      requestServerConnect((*it)->serv);
    }
  }
  
  bool MultiServerManager::requestPluginList(Server* serv){
    string address;
    serv->getAddress(address);
    GetXmlListTreatment* treatment = new GetXmlListTreatment(this, address);
    GetPluginsListRequest* request  = new GetPluginsListRequest(treatment);
    serv->send(request);
    // :TODO: where do we delete those requests and treatments ? see Server.cpp, requestDone
    return true;
  }
  
  bool MultiServerManager::requestServerName(Server* serv){
    string address;
    serv->getAddress(address);
    ServerNameTreatment* treatment = new ServerNameTreatment(address);
    connect(treatment,SIGNAL(nameReceived(ServerNameTreatment*,std::string,std::string)),this,SLOT(changeName(ServerNameTreatment*,std::string,std::string)));
    GetServerNameRequest* request = new GetServerNameRequest(treatment);
    serv->send(request);
    return true;
  }
  
  bool MultiServerManager::requestPluginLists(){
    for (list<PluginsServer*>::iterator iter=pluginsServersList.begin(); iter != pluginsServersList.end() ; ++iter){
      requestPluginList((*iter)->serv);
    }
    return true;
  }

  bool MultiServerManager::requestServerConnect(Server* serv){
    string address;
    serv->getAddress(address);
    serv->send(new ConnectServerRequest());
    return true;
  }
  
  void MultiServerManager::getResponse(){
    emit newPluginList();
  }
  
  
  void MultiServerManager::changeName(ServerNameTreatment* treatment,string addr,string name){
    string address;
    
    for (list<PluginsServer*>::iterator iter=pluginsServersList.begin(); iter != pluginsServersList.end() ; ++iter){
      ((*iter)->serv)->getAddress(address);
      if( address.compare(addr) == 0 && ((*iter)->name).compare(name)!=0 ){
	(*iter)->name = name;
	break;
      }
    }
    modifyServerNameForEach(name,addr);
    emit nameReceived(this,addr,name);
  }

}

