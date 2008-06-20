#include "tulip/ViewPluginsManager.h"

#include <tulip/hash_string.h>
#include <ext/hash_map>

//====================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
tlp::ViewPluginsManager* tlp::ViewPluginsManager::inst=0;
#endif
#else
tlp::ViewPluginsManager* tlp::ViewPluginsManager::inst=0;
#endif

using namespace std;

namespace tlp
{
  static stdext::hash_map<int,std::string>   viewPluginIdToName;
  static stdext::hash_map<std::string, int>  nameToViewPluginId;

  ViewPluginsManager::ViewPluginsManager() {
    ViewCreatorFactory::initFactory();
  }
  //====================================================
  string ViewPluginsManager::viewPluginName(int id) {
    if (viewPluginIdToName.find(id)!=viewPluginIdToName.end())
      return viewPluginIdToName[id];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid view plugin id" << endl;
      return string("invalid");
    }
  }
  //====================================================
  int ViewPluginsManager::viewPluginId(string name) {
    if (nameToViewPluginId.find(name)!=nameToViewPluginId.end())
      return nameToViewPluginId[name];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid view plugin name" << endl;
      return 0;
    }
  }
  //====================================================
  void ViewPluginsManager::loadViewPlugins() {
    Iterator<string> *itS=ViewCreatorFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string pluginName=itS->next();
      cout << pluginName << endl;
      int pluginId=ViewCreatorFactory::factory->objMap[pluginName]->getId();
      viewPluginIdToName[pluginId]=pluginName;
      nameToViewPluginId[pluginName]=pluginId;
    } delete itS;
  }
  //====================================================
  void ViewPluginsManager::loadPlugins(PluginLoader *plug) {
    ViewCreatorFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    viewPluginIdToName.clear();
    nameToViewPluginId.clear();
    while (end!=tlp::TulipPluginsPath.end())
      if ((*end)==tlp::PATH_DELIMITER) {
	if (begin!=end)
	  tlp::loadPluginsFromDir(string(begin,end)+"/view", "View", plug);
	++end;
	begin=end;
      } else
	++end;
    if (begin!=end) {
      tlp::loadPluginsFromDir(string(begin,end)+"/view", "View", plug);
    }
    loadViewPlugins();
  }
  //
  void ViewPluginsManager::initViewPluginsList(MutableContainer<ViewCreator *>& view) {
    ViewCreatorContext ic;
    view.setAll(0);
    
    Iterator<string> *itS = ViewCreatorFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string viewPluginName=itS->next();
      //cerr << "loading : " << interactorName << endl;
    ViewCreator *newViewPlugin = ViewCreatorFactory::factory->getPluginObject(viewPluginName, &ic);
      //    cerr << newInteractor << endl;
      //if (InteractorFactory::factory->objMap[interactorName]->getId() == 0) 
      //  interactors.setAll(newInteractor);
      //    cerr << " id:" << InteractorFactory::factory->objMap[interactorName]->getId() << endl;
      view.set(ViewCreatorFactory::factory->objMap[viewPluginName]->getId(), newViewPlugin);
    //    cerr << interactors.get(InteractorFactory::factory->objMap[interactorName]->getId()) << endl;
    } delete itS;
  }
} 
