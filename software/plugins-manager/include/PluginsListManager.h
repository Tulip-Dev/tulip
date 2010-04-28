//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef _PLUGINSLISTMANAGER_H_
#define _PLUGINSLISTMANAGER_H_

#include "PluginInfo.h"
#include "TPWFonctor.h"

#include <QtXml/QDomElement>
#include <set>

namespace tlp {

  enum ArrangementMethod {
    ARRANGEMENT_METHOD_BY_SERVER = 0,
    ARRANGEMENT_METHOD_BY_TYPE,
    ARRANGEMENT_METHOD_BY_NAME
  };

  enum ViewType {
    VIEW_BY_SERVER = 0,
    VIEW_BY_TYPE,
    VIEW_BY_NAME
  };


  class TLP_PLUGINSMANAGER_SCOPE PluginsListManager {

    ArrangementMethod arrangementMethod;

    std::vector<PluginInfo*> pluginsList;
    std::vector<LocalPluginInfo*> localPluginsList;
    //std::vector<std::vector<std::string> > globalInstalledPluginsList;
    bool errorIsDisplayed;

    void addServerListRecursion(const std::string& serverName, const QDomElement& xmlElem);
    void modifyListWithInstalledPlugins();
    void clearInstalledPluginsList();

    bool LocalInstalledPluginsListIsCharged;

  public:

    PluginsListManager(const std::vector<LocalPluginInfo> &pluginsList);

    void addServerList(const std::string& serverName, const std::string& xml);
    void addLocalPlugin(const DistPluginInfo *pluginInfo);
    void removeLocalPlugin(const LocalPluginInfo *pluginInfo);
    void removeServerList(const std::string& serverName);
    void modifyServerNameForEach(const std::string& serverName, const std::string& serverAddr);

    std::vector<int> getListPosition();
    void getPluginsList(CompletePluginsList &list);
    //std::vector<std::vector<std::string> > getPluginsListByType();

    void getPluginsInformation(const std::string &pluginName,std::vector<const PluginInfo *> &resultList);
    void getPluginsInformation(const std::string &pluginName,const std::string &pluginType,std::vector<const PluginInfo *> &resultList);
    void getPluginsInformation(const std::string &pluginName,const std::string &pluginType,const std::string &pluginVersion,std::vector<const PluginInfo *> &resultList);
    void getPluginsInformation(const std::string &pluginName,const std::string &pluginType,const std::string &pluginVersion,const std::string &serverName,std::vector<const PluginInfo *> &resultList);

    const PluginInfo* getPluginInformation(const std::string &pluginName);
    const PluginInfo* getPluginInformation(const std::string &pluginName,const std::string &pluginType);
    const PluginInfo* getPluginInformation(const std::string &pluginName,const std::string &pluginType,const std::string &pluginVersion);
    const PluginInfo* getPluginInformation(const std::string &pluginName,const std::string &pluginType,const std::string &pluginVersion,const std::string &serverName);

    bool pluginExist(const std::string &pluginName);
    bool pluginIsInstalled(const PluginInfo &plugin);
    bool getPluginDependencies(const PluginInfo &plugin, std::set<PluginDependency,PluginDependencyCmp> &dependencies,std::string &errorMessage);
    bool getPluginDependenciesNotInstalled(const PluginInfo &plugin, std::set<PluginDependency,PluginDependencyCmp> &dependencies,std::string &errorMessage);
    bool getPluginDependenciesToInstall(const PluginInfo &plugin, std::set<DistPluginInfo,PluginCmp> &toInstall,std::set<LocalPluginInfo,PluginCmp> &toRemove,std::string &errorMessage);
    bool getPluginDependenciesToRemove(const PluginInfo &plugin, std::set<LocalPluginInfo,PluginCmp> &toRemove);

    LocalPluginInfo *getLocalPlugin(const PluginInfo *plugin);

    void switchView(ViewType viewNumber);
    void updateInstalledPlugins();
    void newPluginsDirectory();

  };

}

#endif //_PLUGINSLISTMANAGER_H_
