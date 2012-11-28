#include "reverseedges.h"

#include <tulip/BooleanProperty.h>

PLUGIN(ReverseEdges)

using namespace tlp;

ReverseEdges::ReverseEdges(tlp::PluginContext* context): Algorithm(context) {
  addInParameter<BooleanProperty>("selection",
				  HTML_HELP_OPEN()			\
				  HTML_HELP_DEF("type","BooleanProperty") \
				  HTML_HELP_DEF("default","viewSelection") \
				  HTML_HELP_BODY()			\
				  "Only edges selected in this property (or all edges if no property is given) will be reversed." \
				  HTML_HELP_CLOSE(),
				  "viewSelection", false);
}

ReverseEdges::~ReverseEdges() {
}

bool ReverseEdges::run() {
  BooleanProperty* selection = NULL;
  if (dataSet)
    dataSet->get<BooleanProperty*>("selection", selection);

  Iterator<edge>* ite =
    selection ? selection->getEdgesEqualTo(true) : graph->getEdges();

  int step = 0, max_step = graph->numberOfEdges();
  edge e;
  forEach(e, ite) {
    if (pluginProgress && ((++step % 10) == 0)) {
      ProgressState state = pluginProgress->progress(step, max_step);
      if (state != TLP_CONTINUE)
	return state != TLP_CANCEL;
    }
    graph->reverse(e);
  }

  return true;
}
