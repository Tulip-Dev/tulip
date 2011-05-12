#include <tulip/PluginInformations.h>

#include <QtCore/QList>

#ifndef PluginLister_H
#define PluginLister_H

namespace tlp {

class TLP_QT_SCOPE PluginManager {
public:
  
  enum PluginLocation {
    All = 0,
    Local = 1,
    Remote = 2
  };
  static QList<const tlp::PluginInformationsInterface*> pluginsList(PluginLocation list = All);
  
};

}
#endif //PluginLister_H
