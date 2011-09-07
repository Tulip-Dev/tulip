#include <tulip/ForEach.h>

template<typename PropType, typename NodeType, typename EdgeType>
CachedPropertyAnimation<PropType, NodeType, EdgeType>::CachedPropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out,
    tlp::BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges) :
  PropertyAnimation<PropType, NodeType, EdgeType> (graph, start, end, out, selection, frameCount, computeNodes, computeEdges) {

  if (this->computeNodes) {
    tlp::node n;
    forEach (n, this->graph->getNodes()) {
      if (this->equalNodes(this->end->getNodeValue(n), this->start->getNodeValue(n))) {
        this->selection->setNodeValue(n, false);
        //Init the out properties with the default value.
        this->out->setNodeValue(n,this->end->getNodeValue(n));
      }
    }
  }

  if (this->computeEdges) {
    tlp::edge e;
    forEach (e, this->graph->getEdges()) {
      if (this->equalEdges(this->end->getEdgeValue(e), this->start->getEdgeValue(e))) {
        this->selection->setEdgeValue(e, false);
        //Init the out properties with the default value.
        this->out->setEdgeValue(e,end->getEdgeValue(e));
      }
    }
  }
}

template<typename PropType, typename NodeType, typename EdgeType>
CachedPropertyAnimation<PropType, NodeType, EdgeType>::~CachedPropertyAnimation() {
}

template<typename PropType, typename NodeType, typename EdgeType>
void CachedPropertyAnimation<PropType, NodeType, EdgeType>::frameChanged(int f) {
  if (this->computeNodes) {
    computedNodeSteps.clear();
    tlp::node n;
    forEach(n, this->graph->getNodes()) {
      if (this->selection && !this->selection->getNodeValue(n))
        continue;

      std::pair<NodeType, NodeType> values(this->start->getNodeValue(n), this->end->getNodeValue(n));
      NodeType frameValue;

      if (computedNodeSteps.find(values) == computedNodeSteps.end()) {
        frameValue = this->getNodeFrameValue(values.first, values.second, f);
        computedNodeSteps[values] = frameValue;
      }
      else
        frameValue = computedNodeSteps[values];

      this->out->setNodeValue(n, frameValue);
    }
  }

  if (this->computeEdges) {
    computedEdgeSteps.clear();
    tlp::edge e;
    forEach(e, this->graph->getEdges()) {
      if (this->selection && !this->selection->getEdgeValue(e))
        continue;

      std::pair<EdgeType, EdgeType> values(this->start->getEdgeValue(e), this->end->getEdgeValue(e));
      EdgeType frameValue;

      if (computedEdgeSteps.find(values) == computedEdgeSteps.end()) {
        frameValue = this->getEdgeFrameValue(values.first, values.second, f);
        computedEdgeSteps[values] = frameValue;
      }
      else
        frameValue = computedEdgeSteps[values];

      this->out->setEdgeValue(e, frameValue);
    }
  }
}
