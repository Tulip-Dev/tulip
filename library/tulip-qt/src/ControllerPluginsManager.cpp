#include "tulip/ControllerPluginsManager.h"
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

  ControllerPluginsManager::ControllerPluginsManager() {
    ControllerFactory::initFactory();
  }
  //====================================================
  void ControllerPluginsManager::loadPlugins(PluginLoader *plug) {
    ControllerFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
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
  }
  //====================================================
  void ControllerPluginsManager::initControllerPluginsList(MutableContainer<Controller *> &controllers) {
    ControllerContext ic;
    controllers.setAll(0);

    Iterator<string> *itS = ControllerFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string controllerPluginName=itS->next();
      Controller *newControllerPlugin = ControllerFactory::factory->getPluginObject(controllerPluginName, &ic);
    } delete itS;
  }
  //====================================================
  Controller* ControllerPluginsManager::createController(const string &name) {
    ControllerContext ic;
    return ControllerFactory::factory->getPluginObject(name, &ic);
  }
}
