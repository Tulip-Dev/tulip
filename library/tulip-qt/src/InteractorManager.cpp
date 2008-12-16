#include "tulip/InteractorManager.h"

#include <tulip/hash_string.h>
#include <ext/hash_map>


#include "tulip/GWInteractor.h"

#include "tulip/TlpQtTools.h"


//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::InteractorManager* tlp::InteractorManager::inst=0;
#endif
#else
tlp::InteractorManager* tlp::InteractorManager::inst=0;
#endif

using namespace std;

namespace tlp
{

  InteractorManager::InteractorManager() {
    InteractorFactory::initFactory();
  }
  //====================================================
  void InteractorManager::loadPlugins(PluginLoader *plug) {
    InteractorFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    while (end!=tlp::TulipPluginsPath.end())
      if ((*end)==tlp::PATH_DELIMITER) {
	if (begin!=end)
	  tlp::loadInteractorPluginsFromDir(string(begin,end)+"/interactors",plug);
	++end;
	begin=end;
      } else
	++end;
    if (begin!=end) {
      tlp::loadInteractorPluginsFromDir(string(begin,end)+"/interactors",plug);
    }
  }
  //====================================================
  void InteractorManager::initInteractorList(MutableContainer<Interactor *>& interactors) {
    InteractorContext ic;
    interactors.setAll(0);

    Iterator<string> *itS = InteractorFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string interactorName=itS->next();
      Interactor *newInteractor = InteractorFactory::factory->getPluginObject(interactorName, &ic);
    } delete itS;
  }
  //====================================================
  Interactor *InteractorManager::getInteractor(const string &name){
    InteractorContext ic;
    return InteractorFactory::factory->getPluginObject(name, &ic);
  }
}
