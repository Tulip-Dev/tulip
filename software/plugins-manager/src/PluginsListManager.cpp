#include "PluginsListManager.h"

#include <iostream>
#include <tulip/TlpTools.h>
#include <QtGui/QMessageBox>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QProcess>

#include "TPWFonctor.h"

using namespace std;

namespace tlp {

  PluginsListManager::PluginsListManager(const vector<LocalPluginInfo> &pluginsList):arrangementMethod(ARRANGEMENT_METHOD_BY_NAME){
    errorIsDisplayed=false;
    //LocalInstalledPluginsListIsCharged=false;
    AlgorithmContext tmp;
    for(vector<LocalPluginInfo>::const_iterator it=pluginsList.begin();it!=pluginsList.end();++it) {
      LocalPluginInfo *plugin=new LocalPluginInfo(*it);
      if(plugin->type!="Glyph")
	plugin->displayType=PluginInfo::getPluginDisplayType(plugin->name);
      else
	plugin->displayType=plugin->type;
      localPluginsList.push_back(plugin);
    }
    for(vector<LocalPluginInfo*>::iterator it=localPluginsList.begin();it!=localPluginsList.end();++it)
      this->pluginsList.push_back(*it);
  }
  
  void PluginsListManager::addServerListRecursion(const string& serverName, const QDomElement& xmlElem){
    if(!xmlElem.isNull()){
      if(xmlElem.tagName() == "pluginsList"){
	
	QDomNodeList childrenList = xmlElem.childNodes();

	for(unsigned int i=0;i<childrenList.length();++i){
	  QDomElement childElement = childrenList.at(i).toElement();
	  QDomNodeList dependenciesList=childElement.childNodes();
	  vector<PluginDependency> newDependency;
	  
	  for(unsigned int j=0;j<dependenciesList.length();++j){
	    QDomElement dependencyElement = dependenciesList.at(j).toElement();
	    if(dependencyElement.tagName() == "dependency" ){
	      newDependency.push_back(PluginDependency(dependencyElement.attribute("name").toStdString(),
						       dependencyElement.attribute("type").toStdString(),
						       dependencyElement.attribute("version").toStdString()));
	    }
	  }
	  pluginsList.push_back(new DistPluginInfo(childElement.attribute("name").toStdString(), 
						   childElement.attribute("type").toStdString(), 
						   childElement.attribute("displayType").toStdString(), 
						   serverName, 
						   childElement.attribute("version").toStdString(), 
						   childElement.attribute("fileName").toStdString(),
						   "",
						   newDependency,
						   childElement.attribute("linuxVersion").toStdString()=="true",
						   childElement.attribute("macVersion").toStdString()=="true",
						   childElement.attribute("windowsVersion").toStdString()=="true"));
	}
      }
    }
  }
  
  void PluginsListManager::modifyListWithInstalledPlugins(){
    pluginsList = for_each(pluginsList.begin(),pluginsList.end(),PluginsListClearLocalVersion()).getResult();

    ModifyPluginWithInstalledVersion mpwiv(&localPluginsList);
    for_each(pluginsList.begin(),pluginsList.end(),mpwiv);
  }
  
  void PluginsListManager::addServerList(const string& serverName, const string& xml){
    QDomDocument xmlDoc;
    xmlDoc.setContent(QString(xml.c_str()));
    
    QDomElement elem = xmlDoc.documentElement();
    
    addServerListRecursion(serverName, elem);
    modifyListWithInstalledPlugins();
  }

  void PluginsListManager::modifyServerNameForEach(const string& serverName,const string& serverAddr){
    vector<PluginInfo*>::iterator it;
    for(it=pluginsList.begin();it<pluginsList.end();it++){
      if( ((*it)->server).compare(serverAddr)==0 ){
	((*it)->server)=serverName;
      }
    }
  }

  void PluginsListManager::removeServerList(const string& serverName){
    PluginMatchServerInPluginsList pred(serverName);
    vector<PluginInfo*>::iterator new_end = remove_if(pluginsList.begin(), pluginsList.end(), pred);
    for(vector<PluginInfo*>::iterator it=new_end;it!=pluginsList.end();++it)
      delete (*it);
    pluginsList.erase(new_end, pluginsList.end());
  }

  //vector <string> PluginsListManager::getPluginsGlobalListArrangement(){
  // return PluginsListTransformerToGlobal::getListArrangement();
  //}

  void PluginsListManager::addLocalPlugin(const DistPluginInfo *pluginInfo){
    LocalPluginInfo *newPlugin=new LocalPluginInfo(pluginInfo->name,pluginInfo->type,pluginInfo->displayType,"Local",pluginInfo->version,pluginInfo->fileName,"","","",pluginInfo->dependencies);
    localPluginsList.push_back(newPlugin);
    pluginsList.push_back(newPlugin);
    modifyListWithInstalledPlugins();
  }

  void PluginsListManager::removeLocalPlugin(const LocalPluginInfo *pluginInfo){
    PluginMatchNameTypeVersionAndServerPred pred(pluginInfo->name,pluginInfo->type,pluginInfo->version,pluginInfo->server);

    vector<LocalPluginInfo*>::iterator new_end = remove_if(localPluginsList.begin(), localPluginsList.end(), pred);
    localPluginsList.erase(new_end, localPluginsList.end());

    vector<PluginInfo*>::iterator new_end2 = remove_if(pluginsList.begin(), pluginsList.end(), pred);
    for(vector<PluginInfo*>::iterator it=(++new_end2) ;it!=pluginsList.end();++it)
      delete (*it);
    pluginsList.erase(new_end2, pluginsList.end());

    modifyListWithInstalledPlugins();
  }


  vector<int> PluginsListManager::getListPosition(){
    switch(arrangementMethod){
    case ARRANGEMENT_METHOD_BY_SERVER :
      {
	return PluginsListTransformerByServer::getListArrangement();
	break;
      }
    case ARRANGEMENT_METHOD_BY_TYPE :
      {
	return PluginsListTransformerByType::getListArrangement();
	break;
      }
    case ARRANGEMENT_METHOD_BY_NAME :
      {
	return PluginsListTransformerByName::getListArrangement();
	break;
      }
    }
    return vector<int>();
  }

  void PluginsListManager::getPluginsList(CompletePluginsList &list){
  
    switch(arrangementMethod){
    case ARRANGEMENT_METHOD_BY_SERVER :
      {
	PluginsDefaultOrder pdo;
	sort(pluginsList.begin(), pluginsList.end(), pdo);
	for_each(pluginsList.begin(), pluginsList.end(),PluginsListTransformerByServer(&list));
	break;
      }
    case ARRANGEMENT_METHOD_BY_TYPE :
      {
	PluginsGlobalOrder pgo;
	sort(pluginsList.begin(), pluginsList.end(), pgo);
	PluginsListTransformerByType plt(&list);
	for_each(pluginsList.begin(), pluginsList.end(),plt);
	break;
      }
    case ARRANGEMENT_METHOD_BY_NAME :
      {
	PluginsNameDefaultOrder pndo;
	sort(pluginsList.begin(), pluginsList.end(), pndo);
	PluginsListTransformerByName splt(&list);
	for_each(pluginsList.begin(), pluginsList.end(),splt);
	break;
      }
    }
  }

  void PluginsListManager::switchView(ViewType viewNumber){
    if(viewNumber==VIEW_BY_TYPE){
      arrangementMethod=ARRANGEMENT_METHOD_BY_TYPE;
    }else if(viewNumber == VIEW_BY_NAME){
      arrangementMethod=ARRANGEMENT_METHOD_BY_NAME;
    }else{
      arrangementMethod=ARRANGEMENT_METHOD_BY_SERVER;
    }
  }

  void PluginsListManager::getPluginsInformation(const string &pluginName,vector<const PluginInfo *> &resultList){
    PluginMatchNamePred pred(pluginName);
    vector<PluginInfo*>::iterator it=find_if(pluginsList.begin(),pluginsList.end(),pred);
    if(it!=pluginsList.end()){
      while(it!=pluginsList.end()){
	resultList.push_back((*it));
	it++;
	it=find_if(it,pluginsList.end(),pred);
      }
    }
  }

  void PluginsListManager::getPluginsInformation(const std::string &pluginName,const std::string &pluginType,std::vector<const PluginInfo *> &resultList) {
    PluginMatchNameAndTypePred pred(pluginName,pluginType);
    vector<PluginInfo*>::iterator it=find_if(pluginsList.begin(),pluginsList.end(),pred);
    if(it!=pluginsList.end()){
      while(it!=pluginsList.end()){
	resultList.push_back((*it));
	it++;
	it=find_if(it,pluginsList.end(),pred);
      }
    }
  }

  void PluginsListManager::getPluginsInformation(const std::string &pluginName,const std::string &pluginType,const std::string &pluginVersion,std::vector<const PluginInfo *> &resultList) {
    PluginMatchNameTypeAndVersionPred pred(pluginName,pluginType,pluginVersion);
    vector<PluginInfo*>::iterator it=find_if(pluginsList.begin(),pluginsList.end(),pred);
    if(it!=pluginsList.end()){
      while(it!=pluginsList.end()){
	resultList.push_back((*it));
	it++;
	it = find_if(it,pluginsList.end(),pred);
      }
    }
  }

  void PluginsListManager::getPluginsInformation(const std::string &pluginName,const std::string &pluginType,const std::string &pluginVersion,const std::string &serverName,std::vector<const PluginInfo *> &resultList) {
    PluginMatchNameTypeVersionAndServerPred pred(pluginName,pluginType,pluginVersion,serverName);
    vector<PluginInfo*>::iterator it=find_if(pluginsList.begin(),pluginsList.end(),pred);
    if(it!=pluginsList.end()){
      while(it!=pluginsList.end()){
	resultList.push_back((*it));
	it++;
	it=find_if(it,pluginsList.end(),pred);
      }
    }
  }

  const PluginInfo* PluginsListManager::getPluginInformation(const std::string &pluginName) {
    vector<const PluginInfo *> tmp;
    getPluginsInformation(pluginName,tmp);
    if(tmp.size()>=1)
     return tmp[0];
    else
      return NULL;
  }

  const PluginInfo* PluginsListManager::getPluginInformation(const std::string &pluginName,const std::string &pluginType) {
    vector<const PluginInfo *> tmp;
    getPluginsInformation(pluginName,pluginType,tmp);
    if(tmp.size()>=1)
     return tmp[0];
    else
      return NULL;
  }

  const PluginInfo* PluginsListManager::getPluginInformation(const std::string &pluginName,const std::string &pluginType,const std::string &pluginVersion) {
    vector<const PluginInfo *> tmp;
    getPluginsInformation(pluginName,pluginType,pluginVersion,tmp);
    if(tmp.size()>=1)
     return tmp[0];
    else
      return NULL;
  }

  const PluginInfo* PluginsListManager::getPluginInformation(const std::string &pluginName,const std::string &pluginType,const std::string &pluginVersion,const std::string &serverName) {
    vector<const PluginInfo *> tmp;
    getPluginsInformation(pluginName,pluginType,pluginVersion,serverName,tmp);
    if(tmp.size()>=1)
     return tmp[0];
    else
      return NULL;
  }


  bool PluginsListManager::pluginExist(const std::string &pluginName) {
    PluginMatchNamePred pred(pluginName);
    vector<PluginInfo*>::iterator it=find_if(pluginsList.begin(),pluginsList.end(),pred);
    if(it!=pluginsList.end()) 
      return true;
    else
      return false;
  }

  bool PluginsListManager::pluginIsInstalled(const PluginInfo &plugin) {
    PluginMatchNameTypeAndVersionPred pred(plugin.name,plugin.type,plugin.version);
    vector<LocalPluginInfo*>::iterator it=find_if(localPluginsList.begin(),localPluginsList.end(),pred);
    if(it!=localPluginsList.end())
      return true;
    else
      return false;
  }

  bool PluginsListManager::getPluginDependencies(const PluginInfo &plugin, set<PluginDependency,PluginDependencyCmp> &dependencies) {
    for(vector<PluginDependency>::const_iterator it=plugin.dependencies.begin();it!=plugin.dependencies.end();++it) {
      dependencies.insert(*it);
      const PluginInfo* nextPlugin=getPluginInformation((*it).name,(*it).type,(*it).version);
      if(!nextPlugin)
	return false;
      if(!getPluginDependencies(*nextPlugin,dependencies))
	return false;
    }
    return true;
  }

  bool PluginsListManager::getPluginDependenciesNotInstalled(const PluginInfo &plugin, set<PluginDependency,PluginDependencyCmp> &dependencies) {
    set<PluginDependency,PluginDependencyCmp> allDependencies;
    if(!getPluginDependencies(plugin,allDependencies))
      return false;
    for(set<PluginDependency,PluginDependencyCmp>::const_iterator it=allDependencies.begin();it!=allDependencies.end();++it) {
      const PluginInfo* nextPlugin=getPluginInformation((*it).name,(*it).type,(*it).version);
      if(!nextPlugin)
	return false;
      if(!pluginIsInstalled(*nextPlugin))
	dependencies.insert(*it);
    }
    return true;
  }

  bool PluginsListManager::getPluginDependenciesToInstall(const PluginInfo &plugin, set<DistPluginInfo,PluginCmp> &toInstall) {
    set<PluginDependency,PluginDependencyCmp> dependenciesNotInstalled;
    if(!getPluginDependenciesNotInstalled(plugin,dependenciesNotInstalled))
      return false;
    for(set<PluginDependency,PluginDependencyCmp>::iterator it=dependenciesNotInstalled.begin();it!=dependenciesNotInstalled.end();++it) {
      const PluginInfo* nextPlugin=getPluginInformation((*it).name,(*it).type,(*it).version);
      if(!nextPlugin)
	return false;
      assert(!nextPlugin->local);
      toInstall.insert(*(DistPluginInfo*)nextPlugin);
    }
    return true;
  }

  bool PluginsListManager::getPluginDependenciesToRemove(const PluginInfo &plugin, std::set<LocalPluginInfo,PluginCmp> &toRemove) {
    for(vector<LocalPluginInfo*>::iterator it=localPluginsList.begin();it!=localPluginsList.end();++it) {
      for(vector<PluginDependency>::iterator itD=(*it)->dependencies.begin();itD!=(*it)->dependencies.end();++itD) {
	if((*itD).name==plugin.name && (*itD).version==plugin.version) {
	  toRemove.insert(*(*it));
	  if(!getPluginDependenciesToRemove(*(*it),toRemove))
	    return false;
	}
      }
    }
    return true;
  }

  LocalPluginInfo *PluginsListManager::getLocalPlugin(const PluginInfo *plugin) {
    PluginMatchNameTypeAndVersionPred pred(plugin->name,plugin->type,plugin->version);
    vector<LocalPluginInfo *>::iterator it=find_if(localPluginsList.begin(),localPluginsList.end(),pred);
    return *it;
  }

}
