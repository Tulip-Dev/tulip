
#include <tulip/PluginInformations>

#include <QList>

#ifndef PluginLister_H
#define PluginLister_H

class PluginLister {
public:
  
  enum PluginLocation {
    All = 0,
    Local = 1,
    Remote = 2
  };
  static QList<PluginInformations> pluginsList(PluginLocation list = PluginLocation::All) const;
  
}

#endif //PluginLister_H
