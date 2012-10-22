#include "reverseedges.h"

#include <tulip/BooleanProperty.h>

PLUGIN(ReverseEdges)

using namespace tlp;

ReverseEdges::ReverseEdges(tlp::PluginContext* context): Algorithm(context) {
  addInParameter<BooleanProperty>("Selection","Only reverse edges selected in this property", "viewMetric");
}

ReverseEdges::~ReverseEdges() {
}

bool ReverseEdges::run() {
  bool result = true;

  BooleanProperty* selection = NULL;
  bool deleteSelection = false;

  if (dataSet->exist("Selection")) {
    dataSet->get<BooleanProperty*>("Selection",selection);
  }

  if (selection == NULL) {
    deleteSelection = true;
    selection = new BooleanProperty(graph);
    selection->setAllNodeValue(true);
    selection->setAllEdgeValue(true);
  }

  graph->push();
  int step = 0, max_step = graph->numberOfEdges();
  edge e;
  forEach(e, selection->getEdgesEqualTo(true)) {
    ProgressState state = pluginProgress->progress(step++, max_step);
    result = state == TLP_CONTINUE;

    if (state == TLP_CANCEL) {
      graph->pop();
      break;
    }
    else if (state == TLP_STOP) {
      break;
    }

    graph->reverse(e);
  }

  if (deleteSelection)
    delete selection;

  return result;
}

bool ReverseEdges::check(std::string &msg) {
  msg="";
  return true;
}

