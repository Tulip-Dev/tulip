#include <tulip/ForEach.h>

template<typename PropType, typename NodeType, typename EdgeType>
PropertyAnimation<PropType, NodeType, EdgeType>::PropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out,
    tlp::BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges) :
  AbstractAnimation(frameCount), graph(graph), start(0), end(0), out(out), computeNodes(computeNodes), computeEdges(computeEdges) {
#ifndef NDEBUG
  assert(out);
  assert(start);
  assert(end);
  assert(end->getGraph()->getRoot() == start->getGraph()->getRoot());
  assert(end->getGraph()->getRoot() == out->getGraph()->getRoot());
  assert(graph);
  assert(frameCount > 0);
#endif /* NDEBUG */

  this->start = new PropType(start->getGraph());
  *(this->start) = *start;
  this->end = new PropType(end->getGraph());
  *(this->end) = *end;

  if (!selection) {
    this->selection = new tlp::BooleanProperty(graph);
    this->selection->setAllNodeValue(true);
    this->selection->setAllEdgeValue(true);
  }
  else {
    this->selection = new tlp::BooleanProperty(selection->getGraph());
    *(this->selection) = *selection;
  }
}

template<typename PropType, typename NodeType, typename EdgeType>
PropertyAnimation<PropType, NodeType, EdgeType>::~PropertyAnimation() {
  delete start;
  delete end;
  delete selection;
}

template<typename PropType, typename NodeType, typename EdgeType>
void PropertyAnimation<PropType, NodeType, EdgeType>::frameChanged(int f) {
  if (computeNodes) {
    tlp::node n;
    forEach(n, graph->getNodes()) {
      if (selection->getNodeValue(n))
        out->setNodeValue(n, getNodeFrameValue(start->getNodeValue(n), end->getNodeValue(n), f));
    }
  }

  if (computeEdges) {
    tlp::edge e;
    forEach(e, graph->getEdges()) {
      if (selection->getEdgeValue(e))
        out->setEdgeValue(e, getEdgeFrameValue(start->getEdgeValue(e), end->getEdgeValue(e), f));
    }
  }
}
