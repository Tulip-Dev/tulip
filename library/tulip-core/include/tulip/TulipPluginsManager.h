#ifndef TULIPPLUGINSMANAGER_H
#define TULIPPLUGINSMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QString>
#include <tulip/tulipconf.h>

#define DISP_COUNT std::cout << "counter= " << __COUNTER__ << std::endl;

class QLibrary;

namespace tlp {

class PluginProgress;
class TulipPlugin;

class TLP_SCOPE TulipPluginsManager {
public:
  static const char* PLUGIN_SYMBOL_NAME;
  typedef tlp::TulipPlugin* (*PluginCreator)();

  enum PluginLoadStatus {
    NoError,
    NotPlugin,
    InstantiationFailed,
    VersionMismatch
  };


  static void loadDirectory(const QString&, tlp::PluginProgress*);
  static PluginLoadStatus loadLibrary(const QString&);
  static void registerPluginCreator(tlp::TulipPlugin*);

//  tlp::TulipPlugin* createPluginObject(const QString&);
//  void destroyPluginObject(tlp::TulipPlugin*);

private:
  TulipPluginsManager();
  static QMap<QString,QLibrary*> _pluginLibraries;
};

}

#endif // TULIPPLUGINSMANAGER_H
