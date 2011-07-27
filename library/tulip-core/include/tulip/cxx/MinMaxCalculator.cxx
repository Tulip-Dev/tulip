#include <tulip/Graph.h>
#include <tulip/Coord.h>

template<typename nodeType, typename edgeType, typename algorithmType>
tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::MinMaxCalculator(tlp::AbstractProperty<nodeType, edgeType, algorithmType>* property, typename nodeType::RealType NodeMin,
    typename nodeType::RealType NodeMax, typename edgeType::RealType EdgeMin, typename edgeType::RealType EdgeMax)
  : _property(property), nodeValueUptodate(false), edgeValueUptodate(false), _nodeMin(NodeMin), _nodeMax(NodeMax), _edgeMin(EdgeMin), _edgeMax(EdgeMax) {
}

template<typename nodeType, typename edgeType, typename algorithmType>
tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::MinMaxCalculator(tlp::AbstractProperty<nodeType, edgeType, algorithmType>* property, typename nodeType::RealType NodeMin,
    typename nodeType::RealType NodeMax)
  : _property(property), nodeValueUptodate(false), edgeValueUptodate(false), _nodeMin(NodeMin), _nodeMax(NodeMax), _edgeMin(NodeMin), _edgeMax(NodeMax) {
}

template<typename nodeType, typename edgeType, typename algorithmType>
typename nodeType::RealType tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::getNodeMin(tlp::Graph* graph) {
  unsigned int graphID = graph->getId();
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = nodeValueUptodate.find(graphID);

  if ((it == nodeValueUptodate.end()) || ((*it).second == false))
    computeMinMaxNode(graph);

  return minNode[graphID];
}

template<typename nodeType, typename edgeType, typename algorithmType>
typename nodeType::RealType tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::getNodeMax(tlp::Graph* graph) {
  unsigned int graphID = graph->getId();
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = nodeValueUptodate.find(graphID);

  if ((it == nodeValueUptodate.end()) || ((*it).second == false))
    computeMinMaxNode(graph);

  return maxNode[graphID];
}

template<typename nodeType, typename edgeType, typename algorithmType>
typename edgeType::RealType tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::getEdgeMin(tlp::Graph* graph) {
  unsigned int graphID = graph->getId();
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = edgeValueUptodate.find(graphID);

  if ((it == edgeValueUptodate.end()) || ((*it).second == false))
    computeMinMaxEdge(graph);

  return minEdge[graphID];
}

template<typename nodeType, typename edgeType, typename algorithmType>
typename edgeType::RealType tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::getEdgeMax(tlp::Graph* graph) {
  unsigned int graphID = graph->getId();
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = edgeValueUptodate.find(graphID);

  if ((it == edgeValueUptodate.end()) || ((*it).second == false))
    computeMinMaxEdge(graph);

  return maxEdge[graphID];
}

template<typename nodeType, typename edgeType, typename algorithmType>
void tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::computeMinMaxNode(Graph* graph) {
  typename nodeType::RealType maxN2 = _nodeMin, minN2 = _nodeMax;

  Iterator<node>* nodeIterator = graph->getNodes();

  while (nodeIterator->hasNext()) {
    node n=nodeIterator->next();
    typename nodeType::RealType tmp = _property->getNodeValue(n);

    if (tmp > maxN2) {
      maxN2 = tmp;
    }

    if (tmp < minN2) {
      minN2 = tmp;
    }
  }

  delete nodeIterator;

  unsigned int sgi = graph->getId();

  nodeValueUptodate[sgi]=true;
  minNode[sgi] = minN2;
  maxNode[sgi] = maxN2;
}

template<typename nodeType, typename edgeType, typename algorithmType>
void tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::computeMinMaxEdge(Graph* graph) {
  typename edgeType::RealType maxE2 = _nodeMin, minE2 = _nodeMax;

  Iterator<edge>* edgeIterator = graph->getEdges();

  while (edgeIterator->hasNext()) {
    edge ite=edgeIterator->next();
    typename edgeType::RealType tmp = _property->getEdgeValue(ite);

    if (tmp>maxE2)
      maxE2 = tmp;

    if (tmp<minE2)
      minE2 = tmp;
  }

  delete edgeIterator;

  unsigned int sgi = graph->getId();

  edgeValueUptodate[sgi]=true;
  minEdge[sgi]=minE2;
  maxEdge[sgi]=maxE2;
}

template<typename nodeType, typename edgeType, typename algorithmType>
void tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::updateNodeValue(tlp::node n, typename nodeType::RealType newValue) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = nodeValueUptodate.begin();

  if (it != nodeValueUptodate.end()) {
    typename nodeType::RealType oldV = _property->getNodeValue(n);

    if (newValue != oldV) {
      // loop on subgraph min/max
      for(; it != nodeValueUptodate.end(); ++it) {
        // if min/max is ok for the current subgraph
        // check if min or max has to be updated
        if ((*it).second == true) {
          unsigned int gid = (*it).first;
          typename nodeType::RealType minV = minNode[gid];
          typename nodeType::RealType maxV = maxNode[gid];

          // check if min or max has to be updated
          if ((newValue < minV) || (newValue > maxV) || (oldV == minV) || (oldV == maxV)) {
            nodeValueUptodate.clear();
            break;
          }
        }
      }
    }
  }
}

template<typename nodeType, typename edgeType, typename algorithmType>
void tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::updateEdgeValue(tlp::edge e, typename edgeType::RealType newValue) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = edgeValueUptodate.begin();

  if (it != edgeValueUptodate.end()) {
    typename edgeType::RealType oldV = _property->getEdgeValue(e);

    if (newValue != oldV) {
      // loop on subgraph min/max
      for(; it != edgeValueUptodate.end(); ++it) {
        // if min/max is ok for the current subgraph
        // check if min or max has to be updated
        if ((*it).second == true) {
          unsigned int gid = (*it).first;
          typename edgeType::RealType minV = minEdge[gid];
          typename edgeType::RealType maxV = maxEdge[gid];

          // check if min or max has to be updated
          if ((newValue < minV) || (newValue > maxV) || (oldV == minV) || (oldV == maxV)) {
            edgeValueUptodate.clear();
            break;
          }
        }
      }
    }
  }
}

template<typename nodeType, typename edgeType, typename algorithmType>
void tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::updateAllNodesValues(typename nodeType::RealType newValue) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = nodeValueUptodate.begin();

  if (it != nodeValueUptodate.end()) {
    // loop on subgraph min/max
    for(; it != nodeValueUptodate.end(); ++it) {
      unsigned int gid = (*it).first;
      minNode[gid] = maxNode[gid] = newValue;
      nodeValueUptodate[gid] = true;
    }
  }
}

template<typename nodeType, typename edgeType, typename algorithmType>
void tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::updateAllEdgesValues(typename edgeType::RealType newValue) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = edgeValueUptodate.begin();

  if (it != edgeValueUptodate.end()) {
    // loop on subgraph min/max
    for(; it != edgeValueUptodate.end(); ++it) {
      unsigned int gid = (*it).first;
      minEdge[gid] = maxEdge[gid] = newValue;
      edgeValueUptodate[gid] = true;
    }
  }
}

template<typename nodeType, typename edgeType, typename algorithmType>
void tlp::MinMaxCalculator<nodeType, edgeType, algorithmType>::treatEvent(const tlp::Event& ev) {
  const GraphEvent* graphEvent = dynamic_cast<const tlp::GraphEvent*>(&ev);

  if (graphEvent) {
    tlp::Graph* graph = graphEvent->getGraph();

    switch(graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      nodeValueUptodate.clear();
      break;
    case GraphEvent::TLP_DEL_NODE: {
      unsigned int sgi = graph->getId();
      TLP_HASH_MAP<unsigned int, bool>::const_iterator it = nodeValueUptodate.find(sgi);

      if (it != nodeValueUptodate.end() && it->second) {
        typename nodeType::RealType oldV = _property->getNodeValue(graphEvent->getNode());

        // check if min or max has to be updated
        if ((oldV == minNode[sgi]) || (oldV == maxNode[sgi]))
          nodeValueUptodate[sgi] = false;
      }

      break;
    }
    case GraphEvent::TLP_ADD_EDGE:
      edgeValueUptodate.clear();
      break;
    case GraphEvent::TLP_DEL_EDGE: {
      unsigned int sgi = graph->getId();
      TLP_HASH_MAP<unsigned int, bool>::const_iterator it = edgeValueUptodate.find(sgi);

      if (it != edgeValueUptodate.end() && it->second) {
        typename edgeType::RealType oldV = _property->getEdgeValue(graphEvent->getEdge());

        // check if min or max has to be updated
        if ((oldV == minEdge[sgi]) || (oldV == maxEdge[sgi])) {
          edgeValueUptodate[sgi] = false;
        }
      }

      break;
    }
    case GraphEvent::TLP_ADD_SUBGRAPH:
      graphEvent->getSubGraph()->addListener(_property);
      break;
    case GraphEvent::TLP_DEL_SUBGRAPH:
      graphEvent->getSubGraph()->removeListener(_property);
      break;
    default:
      // we don't care about the rest
      break;
    }
  }
}
