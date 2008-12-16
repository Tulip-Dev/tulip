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

  ViewPluginsManager::ViewPluginsManager() {
    ViewFactory::initFactory();
  }
  //====================================================
  void ViewPluginsManager::loadPlugins(PluginLoader *plug) {
    ViewFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
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
  }
  //====================================================
  void ViewPluginsManager::initViewPluginsList(MutableContainer<View *> &views) {
    ViewContext ic;
    views.setAll(0);

    Iterator<string> *itS = ViewFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string viewPluginName=itS->next();
      View *newViewPlugin = ViewFactory::factory->getPluginObject(viewPluginName, &ic);
    } delete itS;
  }
  //====================================================
  View* ViewPluginsManager::createView(const string &name) {
  	ViewContext ic;
  	return ViewFactory::factory->getPluginObject(name, &ic);
  }
}
