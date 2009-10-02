#include "tulip/InteractorManager.h"
#include "tulip/Interactor.h"
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

    // if interactorMap is empty, put all interactors in the Map
    interactorsMap.clear();
    InteractorContext ic;

    Iterator<string> *itS = InteractorFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string interactorName=itS->next();
      interactorsMap[interactorName]=InteractorFactory::factory->getPluginObject(interactorName, &ic);
    } delete itS;
  }
  //====================================================
  Interactor *InteractorManager::getInteractor(const string &name){
    InteractorContext ic;
    return InteractorFactory::factory->getPluginObject(name, &ic);
  }
  //====================================================
  list<string> InteractorManager::getCompatibleInteractors(const string &viewName) {
    loadPlugins();
    list<string> compatibleInteractors;
    for(map<string,Interactor*>::iterator it=interactorsMap.begin();it!=interactorsMap.end();++it){
      if((*it).second->isCompatible(viewName))
        compatibleInteractors.push_back((*it).first);
    }
    return compatibleInteractors;
  }

  //====================================================
  multimap<int,string> InteractorManager::getSortedCompatibleInteractors(const string &viewName) {
    multimap<int,string> result;
    list<string> compatibleInteractors=getCompatibleInteractors(viewName);
    for(list<string>::iterator it=compatibleInteractors.begin();it!=compatibleInteractors.end();++it){
      result.insert(pair<int,string>(interactorsMap[*it]->getPriority(),(*it)));
    }
    return result;
  }
}
