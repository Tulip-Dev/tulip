#include "tulip/ControllerPluginsManager.h"

#include <tulip/hash_string.h>
#include <ext/hash_map>

#include "tulip/TlpQtTools.h"

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::ControllerPluginsManager* tlp::ControllerPluginsManager::inst=0;
#endif
#else
tlp::ControllerPluginsManager* tlp::ControllerPluginsManager::inst=0;
#endif

using namespace std;

namespace tlp
{
  static stdext::hash_map<int,std::string>   controllerPluginIdToName;
  static stdext::hash_map<std::string, int>  nameToControllerPluginId;

  ControllerPluginsManager::ControllerPluginsManager() {
    ControllerFactory::initFactory();
  }
  //====================================================
  string ControllerPluginsManager::controllerPluginName(int id) {
    if (controllerPluginIdToName.find(id)!=controllerPluginIdToName.end())
      return controllerPluginIdToName[id];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid controller plugin id" << endl;
      return string("invalid");
    }
  }
  //====================================================
  int ControllerPluginsManager::controllerPluginId(string name) {
    if (nameToControllerPluginId.find(name)!=nameToControllerPluginId.end())
      return nameToControllerPluginId[name];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid controller plugin name" << endl;
      return -1;
    }
  }
  //====================================================
  void ControllerPluginsManager::loadControllerPlugins() {
    Iterator<string> *itS=ControllerFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string pluginName=itS->next();
      int pluginId=ControllerFactory::factory->objMap[pluginName]->getId();
      controllerPluginIdToName[pluginId]=pluginName;
      nameToControllerPluginId[pluginName]=pluginId;
    } delete itS;
  }
  //====================================================
  void ControllerPluginsManager::loadPlugins(PluginLoader *plug) {
    ControllerFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    controllerPluginIdToName.clear();
    nameToControllerPluginId.clear();
    while (end!=tlp::TulipPluginsPath.end())
      if ((*end)==tlp::PATH_DELIMITER) {
	if (begin!=end)
	  tlp::loadControllerPluginsFromDir(string(begin,end)+"/controller", plug);
	++end;
	begin=end;
      } else
	++end;
    if (begin!=end) {
      tlp::loadControllerPluginsFromDir(string(begin,end)+"/controller", plug);
    }
    loadControllerPlugins();
  }
  //====================================================
  void ControllerPluginsManager::initControllerPluginsList(MutableContainer<Controller *> &controllers) {
    ControllerContext ic;
    controllers.setAll(0);

    Iterator<string> *itS = ControllerFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string controllerPluginName=itS->next();
      Controller *newControllerPlugin = ControllerFactory::factory->getPluginObject(controllerPluginName, &ic);
      //    cerr << newInteractor << endl;
      //if (InteractorFactory::factory->objMap[interactorName]->getId() == 0)
      //  interactors.setAll(newInteractor);
      //    cerr << " id:" << InteractorFactory::factory->objMap[interactorName]->getId() << endl;
      controllers.set(ControllerFactory::factory->objMap[controllerPluginName]->getId(), newControllerPlugin);
    //    cerr << interactors.get(InteractorFactory::factory->objMap[interactorName]->getId()) << endl;
    } delete itS;
  }
  //====================================================
  Controller* ControllerPluginsManager::createController(const string &name) {
    MutableContainer<Controller *> controllers;
    initControllerPluginsList(controllers);
    int id=controllerPluginId(name);
    if(id==-1)
      return NULL;
    else
      return controllers.get(id);
  }
}
