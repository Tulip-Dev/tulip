#include <tulip/TulipPluginHeaders.h>

using namespace tlp;

namespace {
const char * paramHelp[] = {
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "PropertyInterface" ) \
  HTML_HELP_BODY() \
  "Property to copy on labels" \
  HTML_HELP_CLOSE(),
  // on nodes
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_BODY() \
  "Copy nodes values" \
  HTML_HELP_CLOSE(),
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_BODY() \
  "Copy edges values" \
  HTML_HELP_CLOSE()
};
}

class ToLabels: public tlp::StringAlgorithm {
public:
  PLUGININFORMATIONS("To labels","Ludwig Fiolka","2012/03/16","Map a property on nodes/edges labels","1.0","")
  ToLabels(const tlp::PluginContext* context): StringAlgorithm(context) {
    addInParameter<PropertyInterface*>("input",paramHelp[0],"viewMetric");
    addInParameter<bool>("nodes",paramHelp[1],"true");
    addInParameter<bool>("edges",paramHelp[2],"true");
  }

  bool run() {
    PropertyInterface* input = NULL;
    bool onNodes = true;
    bool onEdges = true;

    if (dataSet != NULL) {
      dataSet->get("input",input);
      dataSet->get("nodes",onNodes);
      dataSet->get("edges",onEdges);
    }

    if (onNodes) {
      pluginProgress->setComment("Copying nodes values");
      int step=0,max_step = graph->numberOfNodes();
      node n;
      forEach(n,graph->getNodes()) {
        pluginProgress->progress(step++,max_step);
        result->setNodeValue(n,input->getNodeStringValue(n));
      }
    }

    if (onEdges) {
      pluginProgress->setComment("Copying edges values");
      int step=0,max_step = graph->numberOfEdges();
      edge e;
      forEach(e,graph->getEdges()) {
        pluginProgress->progress(step++,max_step);
        result->setEdgeValue(e,input->getEdgeStringValue(e));
      }
    }

    return true;
  }
};

PLUGIN(ToLabels)
