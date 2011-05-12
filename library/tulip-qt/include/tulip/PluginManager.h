#include <tulip/PluginInformations.h>

#include <QtCore/QList>

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

namespace tlp {

class TLP_QT_SCOPE PluginManager {
public:
  
  enum PluginLocation {
    All = 3,
    Local = 1,
    Remote = 2
  };
  Q_DECLARE_FLAGS(Location, PluginLocation);
  
  static QList<const tlp::PluginInformationsInterface*> pluginsList(Location list = All);
  
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManager::Location)

}
#endif //PLUGINMANAGER_H
