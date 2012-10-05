#include <tulip/PropertyAlgorithm.h>
#include <tulip/LayoutProperty.h>

class PerfectLayout: public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATIONS("Perfect aspect ratio","Tulip team","09/19/2010","","1.0","")

  PerfectLayout(const tlp::PluginContext* context): LayoutAlgorithm(context) {
  }

  virtual ~PerfectLayout() {
  }

  bool run() {
    result->perfectAspectRatio();
    return true;
  }
};

PLUGIN(PerfectLayout)
