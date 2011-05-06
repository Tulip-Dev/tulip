
#include "PluginManager.h"

QList<PluginInformations> PluginInformations::pluginsList(PluginLocation list = PluginLocation::All) const {
  QList<PluginInformations> list;
  PluginInformations info;
  list << info;
  
  return list;
}