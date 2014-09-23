/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <tulip/Graph.h>
#include <tulip/Coord.h>

template<typename nodeType, typename edgeType, typename propType>
tlp::MinMaxProperty<nodeType, edgeType, propType>::MinMaxProperty(tlp::Graph* graph, const std::string& name, typename nodeType::RealType NodeMin,
    typename nodeType::RealType NodeMax, typename edgeType::RealType EdgeMin, typename edgeType::RealType EdgeMax)
  : AbstractProperty<nodeType, edgeType, propType>(graph, name), _nodeMin(NodeMin), _nodeMax(NodeMax), _edgeMin(EdgeMin), _edgeMax(EdgeMax), needGraphListener(false) {
}

template<typename nodeType, typename edgeType, typename propType>
typename nodeType::RealType tlp::MinMaxProperty<nodeType, edgeType, propType>::getNodeMin(tlp::Graph* graph) {
  if(!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();
  MINMAX_MAP(nodeType)::const_iterator it = minMaxNode.find(graphID);

  if (it == minMaxNode.end())
    return computeMinMaxNode(graph).first;

  return it->second.first;
}

template<typename nodeType, typename edgeType, typename propType>
typename nodeType::RealType tlp::MinMaxProperty<nodeType, edgeType, propType>::getNodeMax(tlp::Graph* graph) {
  if(!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();
  MINMAX_MAP(nodeType)::const_iterator it = minMaxNode.find(graphID);

  if (it == minMaxNode.end())
    return computeMinMaxNode(graph).second;

  return it->second.second;
}

template<typename nodeType, typename edgeType, typename propType>
typename edgeType::RealType tlp::MinMaxProperty<nodeType, edgeType, propType>::getEdgeMin(tlp::Graph* graph) {
  if(!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();
  MINMAX_MAP(edgeType)::const_iterator it = minMaxEdge.find(graphID);

  if (it == minMaxEdge.end())
    return computeMinMaxEdge(graph).first;

  return it->second.first;
}

template<typename nodeType, typename edgeType, typename propType>
typename edgeType::RealType tlp::MinMaxProperty<nodeType, edgeType, propType>::getEdgeMax(tlp::Graph* graph) {
  if(!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();
  MINMAX_MAP(edgeType)::const_iterator it = minMaxEdge.find(graphID);

  if (it == minMaxEdge.end())
    return computeMinMaxEdge(graph).second;

  return it->second.second;
}

template<typename nodeType, typename edgeType, typename propType>
MINMAX_PAIR(nodeType) tlp::MinMaxProperty<nodeType, edgeType, propType>::computeMinMaxNode(Graph* graph) {
  if(!graph) {
    graph = this->propType::graph;
  }

  typename nodeType::RealType maxN2 = _nodeMin, minN2 = _nodeMax;

  if (AbstractProperty<nodeType,edgeType,propType>::numberOfNonDefaultValuatedNodes() == 0)
    maxN2 = minN2 = AbstractProperty<nodeType,edgeType,propType>::nodeDefaultValue;
  else {
    Iterator<node>* nodeIterator = graph->getNodes();

    while (nodeIterator->hasNext()) {
      node n=nodeIterator->next();
      typename nodeType::RealType tmp = this->getNodeValue(n);

      if (tmp > maxN2) {
        maxN2 = tmp;
      }

      if (tmp < minN2) {
        minN2 = tmp;
      }
    }

    delete nodeIterator;

    // be careful to empty graph
    if (maxN2 < minN2)
      minN2 = maxN2;
  }

  unsigned int sgi = graph->getId();

  // graph observation is now delayed
  // until we need to do some minmax computation
  // this will minimize the graph loading
  if (minMaxNode.find(sgi) == minMaxNode.end() &&
      minMaxEdge.find(sgi) == minMaxEdge.end()) {
    // launch graph hierarchy observation
    graph->addListener(this);
  }

  MINMAX_PAIR(nodeType) minmax(minN2, maxN2);
  return minMaxNode[sgi] = minmax;
}

template<typename nodeType, typename edgeType, typename propType>
MINMAX_PAIR(edgeType) tlp::MinMaxProperty<nodeType, edgeType, propType>::computeMinMaxEdge(Graph* graph) {
  typename edgeType::RealType maxE2 = _edgeMin, minE2 = _edgeMax;

  if (AbstractProperty<nodeType,edgeType,propType>::numberOfNonDefaultValuatedEdges() == 0)
    maxE2 = minE2 = AbstractProperty<nodeType,edgeType,propType>::edgeDefaultValue;
  else {
    Iterator<edge>* edgeIterator = graph->getEdges();

    while (edgeIterator->hasNext()) {
      edge ite=edgeIterator->next();
      typename edgeType::RealType tmp = this->getEdgeValue(ite);

      if (tmp>maxE2)
        maxE2 = tmp;

      if (tmp<minE2)
        minE2 = tmp;
    }

    delete edgeIterator;

    // be careful to no edges graph
    if (maxE2 < minE2)
      minE2 = maxE2;
  }

  unsigned int sgi = graph->getId();

  // graph observation is now delayed
  // until we need to do some minmax computation
  // this will minimize the graph loading time
  if (minMaxNode.find(sgi) == minMaxNode.end() &&
      minMaxEdge.find(sgi) == minMaxEdge.end()) {
    // launch graph hierarchy observation
    graph->addListener(this);
  }

  MINMAX_PAIR(edgeType) minmax(minE2, maxE2);
  return minMaxEdge[sgi] = minmax;
}

template<typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::removeListenersAndClearNodeMap() {
  // we need to clear one of our map
  // this will invalidate some minmax computations
  // so the graphs corresponding to these cleared minmax computations
  // may not have to be longer observed if they have no validated
  // minmax computation in the other map

  // loop to remove unneeded graph observation
  // it is the case if minmax computation
  //
  MINMAX_MAP(nodeType)::const_iterator it = minMaxNode.begin();
  MINMAX_MAP(nodeType)::const_iterator ite = minMaxNode.end();

  for(; it != ite; ++it) {
    unsigned int gi = it->first;
    MINMAX_MAP(edgeType)::const_iterator itg = minMaxEdge.find(gi);

    if (itg == minMaxEdge.end()) {
      // no computation in the other map
      // we can stop observing the current graph
      Graph* g =
        (propType::graph->getId() == gi) ?
        (needGraphListener ? NULL : propType::graph) :
          propType::graph->getDescendantGraph(gi);

      if (g)
        g->removeListener(this);
    }
  }

  // finally clear the map
  minMaxNode.clear();
}

template<typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::removeListenersAndClearEdgeMap() {
  // we need to clear one of our map
  // this will invalidate some minmax computations
  // so the graphs corresponding to these cleared minmax computations
  // may not have to be longer observed if they have no validated
  // minmax computation in the other map

  // loop to remove unneeded graph observation
  // it is the case if minmax computation
  //
  MINMAX_MAP(edgeType)::const_iterator it = minMaxEdge.begin();
  MINMAX_MAP(edgeType)::const_iterator ite = minMaxEdge.end();

  for(; it != ite; ++it) {
    unsigned int gi = it->first;
    MINMAX_MAP(nodeType)::const_iterator itg = minMaxNode.find(gi);

    if (itg == minMaxNode.end()) {
      // no computation in the other map
      // we can stop observing the current graph
      Graph* g =
        (propType::graph->getId() == gi) ?
        (needGraphListener ? NULL : propType::graph) :
          propType::graph->getDescendantGraph(gi);

      if (g)
        g->removeListener(this);
    }
  }

  // finally clear the map
  minMaxEdge.clear();
}

template<typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateNodeValue(tlp::node n, typename nodeType::RealType newValue) {
  MINMAX_MAP(nodeType)::const_iterator it = minMaxNode.begin();

  if (it != minMaxNode.end()) {
    typename nodeType::RealType oldV = this->getNodeValue(n);

    if (newValue != oldV) {
      // loop on subgraph min/max
      for(; it != minMaxNode.end(); ++it) {
        // if min/max is ok for the current subgraph
        // check if min or max has to be updated
        typename nodeType::RealType minV = it->second.first;
        typename nodeType::RealType maxV = it->second.second;

        // check if min or max has to be updated
        if ((newValue < minV) || (newValue > maxV) || (oldV == minV) || (oldV == maxV)) {
          removeListenersAndClearNodeMap();
          break;
        }
      }
    }
  }
}

template<typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateEdgeValue(tlp::edge e, typename edgeType::RealType newValue) {
  MINMAX_MAP(edgeType)::const_iterator it = minMaxEdge.begin();

  if (it != minMaxEdge.end()) {
    typename edgeType::RealType oldV = this->getEdgeValue(e);

    if (newValue != oldV) {
      // loop on subgraph min/max
      for(; it != minMaxEdge.end(); ++it) {
        // if min/max is ok for the current subgraph
        // check if min or max has to be updated
        typename edgeType::RealType minV = it->second.first;
        typename edgeType::RealType maxV = it->second.second;

        // check if min or max has to be updated
        if ((newValue < minV) || (newValue > maxV) || (oldV == minV) || (oldV == maxV)) {
          removeListenersAndClearEdgeMap();
          break;
        }
      }
    }
  }
}

template<typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateAllNodesValues(typename nodeType::RealType newValue) {
  MINMAX_MAP(nodeType)::const_iterator it = minMaxNode.begin();
  // loop on subgraph min/max
  MINMAX_PAIR(nodeType) minmax(newValue, newValue);

  for(; it != minMaxNode.end(); ++it) {
    unsigned int gid = it->first;
    minMaxNode[gid] = minmax;
  }
}

template<typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateAllEdgesValues(typename edgeType::RealType newValue) {
  MINMAX_MAP(edgeType)::const_iterator it = minMaxEdge.begin();
  // loop on subgraph min/max
  MINMAX_PAIR(edgeType) minmax(newValue, newValue);

  for(; it != minMaxEdge.end(); ++it) {
    unsigned int gid = it->first;
    minMaxEdge[gid] = minmax;
  }
}

template<typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::treatEvent(const tlp::Event& ev) {
  const GraphEvent* graphEvent = dynamic_cast<const tlp::GraphEvent*>(&ev);

  if (graphEvent) {
    tlp::Graph* graph = graphEvent->getGraph();

    switch(graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      removeListenersAndClearNodeMap();
      break;

    case GraphEvent::TLP_DEL_NODE: {
      unsigned int sgi = graph->getId();
      MINMAX_MAP(nodeType)::iterator it = minMaxNode.find(sgi);

      if (it != minMaxNode.end()) {
        typename nodeType::RealType oldV =
          this->getNodeValue(graphEvent->getNode());

        // check if min or max has to be updated
        if ((oldV == it->second.first) || (oldV == it->second.second)) {
          minMaxNode.erase(it);

          if ((minMaxEdge.find(sgi) == minMaxEdge.end()) &&
              (!needGraphListener || (graph != propType::graph)))
            // graph observation is no longer needed
            graph->removeListener(this);
        }
      }

      break;
    }

    case GraphEvent::TLP_ADD_EDGE:
      removeListenersAndClearEdgeMap();
      break;

    case GraphEvent::TLP_DEL_EDGE: {
      unsigned int sgi = graph->getId();
      MINMAX_MAP(edgeType)::iterator it = minMaxEdge.find(sgi);

      if (it != minMaxEdge.end()) {
        typename edgeType::RealType oldV =
          this->getEdgeValue(graphEvent->getEdge());

        // check if min or max has to be updated
        if ((oldV == it->second.first) || (oldV == it->second.second)) {
          minMaxEdge.erase(it);

          if ((minMaxNode.find(sgi) == minMaxNode.end()) &&
              (!needGraphListener || (graph != propType::graph)))
            // graph observation is no longer needed
            graph->removeListener(this);
        }
      }

      break;
    }

    default:
      // we don't care about the rest
      break;
    }
  }
}
