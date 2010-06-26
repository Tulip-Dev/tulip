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

#ifndef _Tulip_TPWFONCTOR_H_
#define _Tulip_TPWFONCTOR_H_

namespace tlp {
  static const int POSITION_VERTOR_SIZE = 8;

  enum InfoPositionInVector {
    NAME_POSITION_IN_V = 0,
    TYPE_POSITION_IN_V,
    SERVER_POSITION_IN_V,
    VERSION_POSITION_IN_V,
    FILENAME_POSITION_IN_V,
    AUTHOR_POSITION_IN_V,
    LOCALVERSION_POSITION_IN_V,
    LOCALROOTVERSION_POSITION_IN_V
  };
  
  typedef std::pair<const PluginInfo *, std::vector<std::string> > CompletePluginsListElement;
  typedef std::vector<CompletePluginsListElement> CompletePluginsList;

  // ***********************************************************************************************
  // PluginsListClearLocalVersion
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE PluginsListClearLocalVersion{

  protected:

    std::vector<PluginInfo *> pluginsList;

  public :

    void operator()(PluginInfo* pluginInfo){
      if(!pluginInfo->local) {
	((DistPluginInfo*)pluginInfo)->localVersion="";
	
      }
      pluginsList.push_back(pluginInfo);
    };

    std::vector<PluginInfo*> getResult(){
      return pluginsList;
    }

  };

  // ***********************************************************************************************
  // PluginsListTransformer
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE PluginsListTransformer{

  protected :
    
    CompletePluginsList *pluginsList;
    
  public :
    
    PluginsListTransformer(CompletePluginsList *pluginsList):pluginsList(pluginsList) {}
    virtual ~PluginsListTransformer(){};
    
    virtual void operator()(const PluginInfo* pluginInfo)=0;
    
  };

  // ***********************************************************************************************
  // PluginsListTransformerByName
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE PluginsListTransformerByName : public PluginsListTransformer{
  public :

    PluginsListTransformerByName(CompletePluginsList *pluginsList)
      :PluginsListTransformer(pluginsList) {}

    static std::vector<int> getListArrangement(){
      std::vector<int> result(8,-1);
      result[NAME_POSITION_IN_V]=0;
      result[TYPE_POSITION_IN_V]=1;
      result[VERSION_POSITION_IN_V]=2;
      result[SERVER_POSITION_IN_V]=3;
      return result;
    }

    void operator() (const PluginInfo* pluginInfo){
      std::vector<std::string> tmpList;
      tmpList.push_back(pluginInfo->name);
      tmpList.push_back(pluginInfo->displayType);
      tmpList.push_back(pluginInfo->version);
      tmpList.push_back(pluginInfo->server);
      pluginsList->push_back(CompletePluginsListElement(pluginInfo,tmpList));
    }
  };
  
  // ***********************************************************************************************
  // PluginsListTransformerByType
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE PluginsListTransformerByType : public PluginsListTransformer{
    
  public:
    
    PluginsListTransformerByType(CompletePluginsList *pluginsList)
      :PluginsListTransformer(pluginsList) {}

    static std::vector<int> getListArrangement(){
      std::vector<int> result(8,-1);
      result[TYPE_POSITION_IN_V]=0;
      result[NAME_POSITION_IN_V]=1;
      result[VERSION_POSITION_IN_V]=2;
      result[SERVER_POSITION_IN_V]=3;
      return result;
    }
    
    void operator()(const PluginInfo* pluginInfo){
      std::vector<std::string> tmpList;
      tmpList.push_back(pluginInfo->displayType);
      tmpList.push_back(pluginInfo->name);
      tmpList.push_back(pluginInfo->version);
      tmpList.push_back(pluginInfo->server);
		      
      pluginsList->push_back(CompletePluginsListElement(pluginInfo,tmpList));
    }
  };

  // ***********************************************************************************************
  // PluginsListTransformerByServer
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE PluginsListTransformerByServer : public PluginsListTransformer{
  public :

    PluginsListTransformerByServer(CompletePluginsList *pluginsList)
      :PluginsListTransformer(pluginsList) {}

    static std::vector<int> getListArrangement(){
      std::vector<int> result(8,-1);
      result[SERVER_POSITION_IN_V]=0;
      result[TYPE_POSITION_IN_V]=1;
      result[NAME_POSITION_IN_V]=2;
      result[VERSION_POSITION_IN_V]=3;
      return result;
    }

    void operator()(const PluginInfo* pluginInfo){
      std::vector<std::string> tmpList;
      tmpList.push_back(pluginInfo->server);
      tmpList.push_back(pluginInfo->displayType);
      tmpList.push_back(pluginInfo->name);
      tmpList.push_back(pluginInfo->version);
		      
      pluginsList->push_back(CompletePluginsListElement(pluginInfo,tmpList));
    }
  };

  // ***********************************************************************************************
  // PluginsMatchNamePred
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE  PluginMatchNamePred {

    std::string pluginName;

  public :

    PluginMatchNamePred(const std::string& pluginName):pluginName(pluginName){};
    
    bool operator()(const PluginInfo* pluginInfo){
      return ((pluginInfo->name).compare(pluginName) == 0);
    }
  };

  // ***********************************************************************************************
  // PluginsMatchNameAndVersionPred
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE PluginMatchNameAndVersionPred
  {
    std::string pluginName;
    std::string pluginVersion;
  public :
    PluginMatchNameAndVersionPred(const std::string& pluginName,const std::string& pluginVersion):pluginName(pluginName),pluginVersion(pluginVersion){};
  
    bool operator()(const PluginInfo *pluginInfo){
      return ((pluginInfo->name.compare(pluginName) == 0) && (pluginInfo->version.compare(pluginVersion)==0));
    }
  };
  
  // ***********************************************************************************************
  // PluginsMatchNameAndTypePred
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE  PluginMatchNameAndTypePred {

    std::string pluginName;
    std::string pluginType;

  public :

    PluginMatchNameAndTypePred(const std::string& pluginName,const std::string& pluginType):pluginName(pluginName),pluginType(pluginType){};
    
    bool operator()(const PluginInfo* pluginInfo){
      return (((pluginInfo->name).compare(pluginName) ==0) && (((pluginInfo->type).compare(pluginType) ==0) || ((pluginInfo->displayType).compare(pluginType)==0))) ;
    }
  };
  
  // ***********************************************************************************************
  // PluginsMatchNameTypeAndVersionPred
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE  PluginMatchNameTypeAndVersionPred {

    std::string pluginName;
    std::string pluginType;
    std::string pluginVersion;

  public :

    PluginMatchNameTypeAndVersionPred(const std::string& pluginName,const std::string& pluginType,const std::string& pluginVersion):pluginName(pluginName),pluginType(pluginType),pluginVersion(pluginVersion){};
    
    bool operator()(const PluginInfo* pluginInfo){
      return (((pluginInfo->name).compare(pluginName) ==0) && (((pluginInfo->type).compare(pluginType) ==0) || ((pluginInfo->displayType).compare(pluginType)==0)) && ((pluginInfo->version).compare(pluginVersion) ==0)) ;
    }
    bool operator()(const PluginInfo& pluginInfo){
      return (((pluginInfo.name).compare(pluginName) ==0) && (((pluginInfo.type).compare(pluginType) ==0) || ((pluginInfo.displayType).compare(pluginType)==0)) && ((pluginInfo.version).compare(pluginVersion) ==0)) ;
    }
  };
  
  // ***********************************************************************************************
  // PluginsMatchNameTypeVersionAndServerPred
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE  PluginMatchNameTypeVersionAndServerPred {

    std::string pluginName;
    std::string pluginType;
    std::string pluginVersion;
    std::string serverName;

  public :

    PluginMatchNameTypeVersionAndServerPred(const std::string& pluginName,const std::string& pluginType,const std::string& pluginVersion,const std::string& serverName):pluginName(pluginName),pluginType(pluginType),pluginVersion(pluginVersion),serverName(serverName){};
    
    bool operator()(const PluginInfo* pluginInfo){
      return (((pluginInfo->name).compare(pluginName) ==0) && (((pluginInfo->type).compare(pluginType) ==0) || ((pluginInfo->displayType).compare(pluginType)==0)) && ((pluginInfo->version).compare(pluginVersion) ==0)&& ((pluginInfo->server).compare(serverName) ==0)) ;
    }
  };
  
  // ***********************************************************************************************
  // PluginsMatchServerNamePred
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE  PluginMatchServerNamePred {

    std::string serverName;

  public :

    PluginMatchServerNamePred(const std::string& serverName):serverName(serverName){};
    
    bool operator()(const PluginInfo* pluginInfo){
      return ((pluginInfo->server).compare(serverName) == 0);
    }
  };

  // ***********************************************************************************************
  // PluginsMatchServerInPluginsList
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE PluginMatchServerInPluginsList{
	
    std::string serverName;
	
  public :
		
    PluginMatchServerInPluginsList(const std::string& serverName):serverName(serverName){};
	
    bool operator() (const PluginInfo* pluginInfo){
      return pluginInfo->server.compare(serverName)==0;
    }
	
  };
  
  // ***********************************************************************************************
  // ModifyPluginWithInstalledVersion
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE ModifyPluginWithInstalledVersion {
  
    std::vector<LocalPluginInfo*> *localList;

  public :
    ModifyPluginWithInstalledVersion(std::vector<LocalPluginInfo *> *localList):localList(localList) {};
    
    void operator() (PluginInfo *pluginInfo) {
      PluginMatchNameAndTypePred pred(pluginInfo->name,pluginInfo->type);
      std::vector<LocalPluginInfo*>::iterator iter=find_if(localList->begin(),localList->end(),pred);
      if(iter!=localList->end()){
	((DistPluginInfo*)pluginInfo)->localVersion=(*iter)->version;
      }
    }
  };
  

  // ***********************************************************************************************
  // PluginsDefaultOrder
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE  PluginsDefaultOrder {

  public :
    
    // Fonction de comparaison
    bool operator()(const PluginInfo* p1,const PluginInfo* p2) const{
      
      int serverNamesComparison = (p1->server).compare(p2->server);
      if(serverNamesComparison == 0){
	int typeNamesComparison = (p1->type).compare(p2->type);
	if(typeNamesComparison == 0){
	  int namesComparison = (p1->name).compare(p2->name);
	  if(namesComparison == 0){
	    int versionsComparison = (p1->version).compare(p2->version);
	    return (versionsComparison > 0);
	  }
	  return (namesComparison < 0);
	}
	return (typeNamesComparison < 0);
      }
      return (serverNamesComparison < 0);   
    };
  };
  
  // ***********************************************************************************************
  // PluginsGlobalOrder
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE  PluginsGlobalOrder {	
    
  public :
    
    // Fonction de comparaison
    bool 	operator()(const PluginInfo* p1,const PluginInfo* p2) const {
      
      int typeNamesComparison = (p1->type).compare(p2->type);
      if(typeNamesComparison == 0){
	int namesComparison = (p1->name).compare(p2->name);
	if(namesComparison == 0){
	  int versionsComparison = (p1->version).compare(p2->version);
	  if(versionsComparison == 0){
	    int serverNamesComparison = (p1->server).compare(p2->server);
	    return (serverNamesComparison < 0);
	  }
	  return (versionsComparison < 0);
	}
	return (namesComparison < 0);
      }
      return (typeNamesComparison < 0);
    };
    
  };
  
  // ***********************************************************************************************
  // PluginsNameDefaultOrder
  // ***********************************************************************************************
  class TLP_PLUGINSMANAGER_SCOPE  PluginsNameDefaultOrder {

  public :
    
    // Fonction de comparaison
    bool operator()(const PluginInfo* p1,const PluginInfo* p2) const{
      
      int namesComparison = (p1->name).compare(p2->name);
      if(namesComparison == 0){
	int versionsComparison = (p1->version).compare(p2->version);
	return (versionsComparison < 0);
      }
      return (namesComparison < 0);
      
    };
  };

}


#endif // _TPWFONCTOR_H_
