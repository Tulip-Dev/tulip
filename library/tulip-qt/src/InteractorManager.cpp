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
  static stdext::hash_map<int,std::string>   interactorIdToName;
  static stdext::hash_map<std::string, int>  nameToInteractorId;

  InteractorManager::InteractorManager() {
    InteractorFactory::initFactory();
  }
  //====================================================
  string InteractorManager::interactorName(int id) {
    if (interactorIdToName.find(id)!=interactorIdToName.end())
      return interactorIdToName[id];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid interactor id" << endl;
      return string("invalid");
    }
  }
  //====================================================
  int InteractorManager::interactorId(string name) {
    if (nameToInteractorId.find(name)!=nameToInteractorId.end())
      return nameToInteractorId[name];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid interactor name" << endl;
      return 0;
    }
  }
  //====================================================
  void InteractorManager::loadInteractorPlugins() {
    Iterator<string> *itS=InteractorFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string pluginName=itS->next();
      int pluginId=InteractorFactory::factory->objMap[pluginName]->getId();
      interactorIdToName[pluginId]=pluginName;
      nameToInteractorId[pluginName]=pluginId;
    } delete itS;
  }
  //====================================================
  void InteractorManager::loadPlugins(PluginLoader *plug) {
    InteractorFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    interactorIdToName.clear();
    nameToInteractorId.clear();
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
    loadInteractorPlugins();
  }
  //
  void InteractorManager::initInteractorList(MutableContainer<Interactor *>& interactors) {
    InteractorContext ic;
    interactors.setAll(0);
    
    Iterator<string> *itS = InteractorFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string interactorName=itS->next();
      //cerr << "loading : " << interactorName << endl;
      Interactor *newInteractor = InteractorFactory::factory->getPluginObject(interactorName, &ic);
      //    cerr << newInteractor << endl;
      //if (InteractorFactory::factory->objMap[interactorName]->getId() == 0) 
      //  interactors.setAll(newInteractor);
      //    cerr << " id:" << InteractorFactory::factory->objMap[interactorName]->getId() << endl;
      interactors.set(InteractorFactory::factory->objMap[interactorName]->getId(), newInteractor);
    //    cerr << interactors.get(InteractorFactory::factory->objMap[interactorName]->getId()) << endl;
    } delete itS;
  }
} 
