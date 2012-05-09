#include <tulip/ImportModule.h>

using namespace tlp;

class EmptyGraphImport: public ImportModule {
public:
  PLUGININFORMATIONS("Empty graph","Tulip team", "05/09/2012","A no-op plugin to import empty graphs","1.0","")

  EmptyGraphImport(const tlp::PluginContext* context): ImportModule(context) {
  }

  virtual bool importGraph() {
    return true;
  }
};
PLUGIN(EmptyGraphImport)
