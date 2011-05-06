
#include <tulip/PluginInformations>

#include <QList>

#ifndef PluginManager_H
#define PluginManager_H

class PluginManager {
public:
  
  enum PluginLocation {
    All = 0,
    Local = 1,
    Remote = 2
  };
  static QList<PluginInformations> pluginsList(PluginLocation list = PluginLocation::All) const;
  
}

#endif //PluginManager_H
