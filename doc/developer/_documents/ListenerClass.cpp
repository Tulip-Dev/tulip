#include <tulip/Graph.h>
#include "ListenerClass.h"

using namespace std;
using namespace tlp;

ListenerClass *ListenerClass::instance = nullptr;

ListenerClass::ListenerClass() {}

void ListenerClass::launchListenerClass(const tlp::Graph *graph) {
  if (instance == nullptr) {
    instance = new ListenerClass();
  }
  graph->addListener(instance);
}

void ListenerClass::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      cout << "Event : Node Created" << endl;
      break;

    case GraphEvent::TLP_BEFORE_DEL_NODE:
      cout << "Event : Node Deleted" << endl;
      break;

    case GraphEvent::TLP_ADD_EDGE:
      cout << "Event : Edge Created" << endl;
      break;

    case GraphEvent::TLP_BEFORE_DEL_EDGE:
      cout << "Event : Edge Deleted" << endl;
      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      cout << "Event : Edge Reversed" << endl;
      break;

    default:
      // We don't mind the other events
      break;
    }
  }
}
