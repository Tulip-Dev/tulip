#include "tulip/ViewPluginsManager.h"

#include <tulip/hash_string.h>
#include <ext/hash_map>

#include "tulip/TlpQtTools.h"

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
    ViewFactory::initFactory();
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
    Iterator<string> *itS=ViewFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string pluginName=itS->next();
      int pluginId=ViewFactory::factory->objMap[pluginName]->getId();
      viewPluginIdToName[pluginId]=pluginName;
      nameToViewPluginId[pluginName]=pluginId;
    } delete itS;
  }
  //====================================================
  void ViewPluginsManager::loadPlugins(PluginLoader *plug) {
    ViewFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    viewPluginIdToName.clear();
    nameToViewPluginId.clear();
    while (end!=tlp::TulipPluginsPath.end())
      if ((*end)==tlp::PATH_DELIMITER) {
	if (begin!=end)
	  tlp::loadViewPluginsFromDir(string(begin,end)+"/view", plug);
	++end;
	begin=end;
      } else
	++end;
    if (begin!=end) {
      tlp::loadViewPluginsFromDir(string(begin,end)+"/view", plug);
    }
    loadViewPlugins();
  }
  //====================================================
  void ViewPluginsManager::initViewPluginsList(MutableContainer<View *> &views) {
    ViewContext ic;
    views.setAll(0);

    Iterator<string> *itS = ViewFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string viewPluginName=itS->next();
      //cerr << "loading : " << interactorName << endl;
    View *newViewPlugin = ViewFactory::factory->getPluginObject(viewPluginName, &ic);
      //    cerr << newInteractor << endl;
      //if (InteractorFactory::factory->objMap[interactorName]->getId() == 0)
      //  interactors.setAll(newInteractor);
      //    cerr << " id:" << InteractorFactory::factory->objMap[interactorName]->getId() << endl;
      views.set(ViewFactory::factory->objMap[viewPluginName]->getId(), newViewPlugin);
    //    cerr << interactors.get(InteractorFactory::factory->objMap[interactorName]->getId()) << endl;
    } delete itS;
  }
  //====================================================
  View* ViewPluginsManager::createView(const string &name) {
  	ViewContext ic;
  	return ViewFactory::factory->getPluginObject(name, &ic);
  }
}
