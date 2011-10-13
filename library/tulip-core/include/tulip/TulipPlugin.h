#ifndef TULIPPLUGIN_H
#define TULIPPLUGIN_H

#include <tulip/WithParameter.h>
#include <tulip/WithDependency.h>
#include <tulip/tulipconf.h>
#include <tulip/TulipPluginsManager.h>

#include <QtCore/QString>

#ifdef Q_WS_WIN
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT
#endif

namespace tlp {

class TLP_SCOPE TulipPlugin: public tlp::WithParameter, tlp::WithDependency {
public:
  TulipPlugin();

  QString tulipRelease() const;

  virtual QString name() const=0;
  virtual QString description() const=0;
  virtual QString author() const=0;
  virtual QString date() const=0;
  virtual QString revision() const=0;
};

class TLP_SCOPE TulipPluginCreator {
public:

};

}

#define TULIP_PLUGIN(CLASS) extern "C" {\

extern "C" {
class MyClassPluginCreator {
public:
  MyClassPluginCreator() {

  }
};
}

#endif // TULIPPLUGIN_H
