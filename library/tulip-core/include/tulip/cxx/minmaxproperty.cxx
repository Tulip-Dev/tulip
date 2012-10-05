/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

template<typename nodeType, typename edgeType>
tlp::MinMaxProperty<nodeType, edgeType>::MinMaxProperty(tlp::Graph* graph, std::string name, typename nodeType::RealType NodeMin,
    typename nodeType::RealType NodeMax, typename edgeType::RealType EdgeMin, typename edgeType::RealType EdgeMax)
  : AbstractProperty<nodeType, edgeType>(graph, name), nodeValueUptodate(false), edgeValueUptodate(false), _nodeMin(NodeMin), _nodeMax(NodeMax), _edgeMin(EdgeMin), _edgeMax(EdgeMax) {
}

template<typename nodeType, typename edgeType>
typename nodeType::RealType tlp::MinMaxProperty<nodeType, edgeType>::getNodeMin(tlp::Graph* graph) {
  if(!graph) {
    graph = this->graph;
  }

  unsigned int graphID = graph->getId();
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = nodeValueUptodate.find(graphID);

  if ((it == nodeValueUptodate.end()) || ((*it).second == false))
    computeMinMaxNode(graph);

  return minNode[graphID];
}

template<typename nodeType, typename edgeType>
typename nodeType::RealType tlp::MinMaxProperty<nodeType, edgeType>::getNodeMax(tlp::Graph* graph) {
  if(!graph) {
    graph = this->graph;
  }

  unsigned int graphID = graph->getId();
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = nodeValueUptodate.find(graphID);

  if ((it == nodeValueUptodate.end()) || ((*it).second == false))
    computeMinMaxNode(graph);

  return maxNode[graphID];
}

template<typename nodeType, typename edgeType>
typename edgeType::RealType tlp::MinMaxProperty<nodeType, edgeType>::getEdgeMin(tlp::Graph* graph) {
  if(!graph) {
    graph = this->graph;
  }

  unsigned int graphID = graph->getId();
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = edgeValueUptodate.find(graphID);

  if ((it == edgeValueUptodate.end()) || ((*it).second == false))
    computeMinMaxEdge(graph);

  return minEdge[graphID];
}

template<typename nodeType, typename edgeType>
typename edgeType::RealType tlp::MinMaxProperty<nodeType, edgeType>::getEdgeMax(tlp::Graph* graph) {
  if(!graph) {
    graph = this->graph;
  }

  unsigned int graphID = graph->getId();
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = edgeValueUptodate.find(graphID);

  if ((it == edgeValueUptodate.end()) || ((*it).second == false))
    computeMinMaxEdge(graph);

  return maxEdge[graphID];
}

template<typename nodeType, typename edgeType>
void tlp::MinMaxProperty<nodeType, edgeType>::computeMinMaxNode(Graph* graph) {
  if(!graph) {
    graph = this->graph;
  }

  typename nodeType::RealType maxN2 = _nodeMin, minN2 = _nodeMax;

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

  unsigned int sgi = graph->getId();

  nodeValueUptodate[sgi]=true;
  minNode[sgi] = minN2;
  maxNode[sgi] = maxN2;
}

template<typename nodeType, typename edgeType>
void tlp::MinMaxProperty<nodeType, edgeType>::computeMinMaxEdge(Graph* graph) {
  typename edgeType::RealType maxE2 = _edgeMin, minE2 = _edgeMax;

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

  unsigned int sgi = graph->getId();

  edgeValueUptodate[sgi]=true;
  minEdge[sgi]=minE2;
  maxEdge[sgi]=maxE2;
}

template<typename nodeType, typename edgeType>
void tlp::MinMaxProperty<nodeType, edgeType>::updateNodeValue(tlp::node n, typename nodeType::RealType newValue) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = nodeValueUptodate.begin();

  if (it != nodeValueUptodate.end()) {
    typename nodeType::RealType oldV = this->getNodeValue(n);

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

template<typename nodeType, typename edgeType>
void tlp::MinMaxProperty<nodeType, edgeType>::updateEdgeValue(tlp::edge e, typename edgeType::RealType newValue) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = edgeValueUptodate.begin();

  if (it != edgeValueUptodate.end()) {
    typename edgeType::RealType oldV = this->getEdgeValue(e);

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

template<typename nodeType, typename edgeType>
void tlp::MinMaxProperty<nodeType, edgeType>::updateAllNodesValues(typename nodeType::RealType newValue) {
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

template<typename nodeType, typename edgeType>
void tlp::MinMaxProperty<nodeType, edgeType>::updateAllEdgesValues(typename edgeType::RealType newValue) {
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

template<typename nodeType, typename edgeType>
void tlp::MinMaxProperty<nodeType, edgeType>::treatEvent(const tlp::Event& ev) {
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
        typename nodeType::RealType oldV = this->getNodeValue(graphEvent->getNode());

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
        typename edgeType::RealType oldV = this->getEdgeValue(graphEvent->getEdge());

        // check if min or max has to be updated
        if ((oldV == minEdge[sgi]) || (oldV == maxEdge[sgi])) {
          edgeValueUptodate[sgi] = false;
        }
      }

      break;
    }

    case GraphEvent::TLP_AFTER_ADD_SUBGRAPH:
      graphEvent->getSubGraph()->addListener(this);
      break;

    case GraphEvent::TLP_BEFORE_DEL_SUBGRAPH:
      graphEvent->getSubGraph()->removeListener(this);
      break;

    default:
      // we don't care about the rest
      break;
    }
  }
}
