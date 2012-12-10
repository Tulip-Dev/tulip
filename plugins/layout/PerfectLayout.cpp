#include <tulip/PropertyAlgorithm.h>
#include <tulip/LayoutProperty.h>

class PerfectLayout: public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATIONS("Perfect aspect ratio","Tulip team","09/19/2010","","1.0","")

  PerfectLayout(const tlp::PluginContext* context): LayoutAlgorithm(context) {
    addInParameter<tlp::LayoutProperty>("layout",
                                        HTML_HELP_OPEN()  \
                                        HTML_HELP_DEF("type","LayoutProperty") \
                                        HTML_HELP_DEF("default","viewLayout") \
                                        HTML_HELP_BODY()    \
                                        "The layout property from which a perfect aspect ratio has to be computed." \
                                        HTML_HELP_CLOSE(),
                                        "viewLayout", false);
  }

  virtual ~PerfectLayout() {
  }

  bool run() {
    tlp::LayoutProperty *layout = NULL;

    if ( dataSet!=NULL ) {
      dataSet->get("layout", layout);
    }

    if (!layout)
      layout = graph->getProperty<tlp::LayoutProperty>("viewLayout");

    result->copy(layout);
    result->perfectAspectRatio();
    return true;
  }
};

PLUGIN(PerfectLayout)
