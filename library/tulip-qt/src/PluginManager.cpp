#include <tulip/PluginManager.h>
#include <tulip/PluginLister.h>

using namespace std;
using namespace tlp;

QList<const tlp::PluginInformationsInterface*> PluginManager::pluginsList(Location list) {
  QList<const tlp::PluginInformationsInterface*> result;

  if(list.testFlag(Local)) {
    for(std::map<std::string, PluginListerInterface*>::const_iterator it = PluginListerInterface::allFactories->begin(); it != PluginListerInterface::allFactories->end(); ++it) {
      PluginListerInterface* currentLister = it->second;
      Iterator<string>* plugins = currentLister->availablePlugins();
      while(plugins->hasNext()) {
        string pluginName = plugins->next();
        const AbstractPluginInfo* info = currentLister->pluginInformations(pluginName);
        PluginInformationsInterface* localinfo = new LocalPluginInformations(info);
        result.push_back(localinfo);
      }
      delete plugins;
    }
  }

  if(list.testFlag(Remote)) {
  }

  return result;
}