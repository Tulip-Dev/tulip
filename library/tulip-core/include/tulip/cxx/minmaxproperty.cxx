/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

template <typename nodeType, typename edgeType, typename propType>
tlp::MinMaxProperty<nodeType, edgeType, propType>::MinMaxProperty(
    tlp::Graph *graph, const std::string &name, NODE_VALUE NodeMin, NODE_VALUE NodeMax,
    EDGE_VALUE EdgeMin, EDGE_VALUE EdgeMax)
    : AbstractProperty<nodeType, edgeType, propType>(graph, name), _nodeMin(NodeMin),
      _nodeMax(NodeMax), _edgeMin(EdgeMin), _edgeMax(EdgeMax), needGraphListener(false) {}

template <typename nodeType, typename edgeType, typename propType>
    const MINMAX_PAIR(nodeType) & tlp::MinMaxProperty<nodeType, edgeType, propType>::getNodeMinMax(
                                      const tlp::Graph *graph) {
  if (!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();
  auto it = minMaxNode.find(graphID);

  return (it == minMaxNode.end()) ? computeMinMaxNode(graph) : it->second;
}

template <typename nodeType, typename edgeType, typename propType>
    const MINMAX_PAIR(edgeType) & tlp::MinMaxProperty<nodeType, edgeType, propType>::getEdgeMinMax(
                                      const tlp::Graph *graph) {
  if (!graph) {
    graph = this->propType::graph;
  }

  unsigned int graphID = graph->getId();
  auto it = minMaxEdge.find(graphID);

  return (it == minMaxEdge.end()) ? computeMinMaxEdge(graph) : it->second;
}

template <typename nodeType, typename edgeType, typename propType>
    const MINMAX_PAIR(nodeType) &
    tlp::MinMaxProperty<nodeType, edgeType, propType>::computeMinMaxNode(const Graph *graph) {
  if (!graph) {
    graph = this->propType::graph;
  }

  NODE_VALUE maxN2 = _nodeMin, minN2 = _nodeMax;

  if (AbstractProperty<nodeType, edgeType, propType>::hasNonDefaultValuatedNodes(graph)) {
    for (auto n : graph->nodes()) {
      CONST_NODE_VALUE tmp = this->getNodeValue(n);

      if (tmp > maxN2)
        maxN2 = tmp;
      if (tmp < minN2)
        minN2 = tmp;
    }
  }

  if (maxN2 < minN2)
    maxN2 = minN2 = AbstractProperty<nodeType, edgeType, propType>::nodeDefaultValue;

  unsigned int sgi = graph->getId();

  // graph observation is now delayed
  // until we need to do some minmax computation
  // this will minimize the graph loading
  if (minMaxNode.find(sgi) == minMaxNode.end() && minMaxEdge.find(sgi) == minMaxEdge.end()) {
    // launch graph hierarchy observation
    graph->addListener(this);
  }

  return minMaxNode[sgi] = {minN2, maxN2};
}

template <typename nodeType, typename edgeType, typename propType>
    const MINMAX_PAIR(edgeType) &
    tlp::MinMaxProperty<nodeType, edgeType, propType>::computeMinMaxEdge(const Graph *graph) {
  EDGE_VALUE maxE2 = _edgeMin, minE2 = _edgeMax;

  if (AbstractProperty<nodeType, edgeType, propType>::hasNonDefaultValuatedEdges(graph)) {
    for (auto ite : graph->edges()) {
      CONST_EDGE_VALUE tmp = this->getEdgeValue(ite);

      if (tmp > maxE2)
        maxE2 = tmp;
      if (tmp < minE2)
        minE2 = tmp;
    }
  }

  if (maxE2 < minE2)
    maxE2 = minE2 = AbstractProperty<nodeType, edgeType, propType>::edgeDefaultValue;

  unsigned int sgi = graph->getId();

  // graph observation is now delayed
  // until we need to do some minmax computation
  // this will minimize the graph loading time
  if (minMaxNode.find(sgi) == minMaxNode.end() && minMaxEdge.find(sgi) == minMaxEdge.end()) {
    // launch graph hierarchy observation
    graph->addListener(this);
  }

  return minMaxEdge[sgi] = {minE2, maxE2};
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::removeListenersAndClearNodeMap() {
  // we need to clear one of our map
  // this will invalidate some minmax computations
  // so the graphs corresponding to these cleared minmax computations
  // may not have to be longer observed if they have no validated
  // minmax computation in the other map

  // loop to remove unneeded graph observation
  // it is the case if minmax computation
  //
  auto it = minMaxNode.begin();
  auto ite = minMaxNode.end();

  for (; it != ite; ++it) {
    unsigned int gi = it->first;
    auto itg = minMaxEdge.find(gi);

    if (itg == minMaxEdge.end()) {
      // no computation in the other map
      // we can stop observing the current graph
      Graph *g = (propType::graph->getId() == gi) ? (needGraphListener ? nullptr : propType::graph)
                                                  : propType::graph->getDescendantGraph(gi);

      if (g)
        g->removeListener(this);
    }
  }

  // finally clear the map
  minMaxNode.clear();
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::removeListenersAndClearEdgeMap() {
  // we need to clear one of our map
  // this will invalidate some minmax computations
  // so the graphs corresponding to these cleared minmax computations
  // may not have to be longer observed if they have no validated
  // minmax computation in the other map

  // loop to remove unneeded graph observation
  // it is the case if minmax computation
  //
  auto it = minMaxEdge.begin();
  auto ite = minMaxEdge.end();

  for (; it != ite; ++it) {
    unsigned int gi = it->first;
    auto itg = minMaxNode.find(gi);

    if (itg == minMaxNode.end()) {
      // no computation in the other map
      // we can stop observing the current graph
      Graph *g = (propType::graph->getId() == gi) ? (needGraphListener ? nullptr : propType::graph)
                                                  : propType::graph->getDescendantGraph(gi);

      if (g)
        g->removeListener(this);
    }
  }

  // finally clear the map
  minMaxEdge.clear();
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateNodeValue(tlp::node n,
                                                                        CONST_NODE_VALUE newValue) {
  auto it = minMaxNode.begin();

  if (it != minMaxNode.end()) {
    CONST_NODE_VALUE oldV = this->getNodeValue(n);

    if (newValue != oldV) {
      // loop on subgraph min/max
      for (; it != minMaxNode.end(); ++it) {
        auto sid = it->first;
        if (sid) {
          // check if n belongs to current subgraph
          auto sg = this->graph->getDescendantGraph(sid);
          // sg might be null in undo/redo context
          if (sg && !sg->isElement(n))
            continue;
        }
        // if min/max is ok for the current subgraph
        // check if min or max has to be updated
        CONST_NODE_VALUE minV = it->second.first;
        CONST_NODE_VALUE maxV = it->second.second;

        // check if min or max has to be updated
        if ((newValue < minV) || (newValue > maxV) || (oldV == minV) || (oldV == maxV)) {
          removeListenersAndClearNodeMap();
          break;
        }
      }
    }
  }
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateEdgeValue(tlp::edge e,
                                                                        CONST_EDGE_VALUE newValue) {
  auto it = minMaxEdge.begin();

  if (it != minMaxEdge.end()) {
    CONST_EDGE_VALUE oldV = this->getEdgeValue(e);

    if (newValue != oldV) {
      // loop on subgraph min/max
      for (; it != minMaxEdge.end(); ++it) {
        auto sid = it->first;
        if (sid) {
          // check if e belongs to current subgraph
          // sg might be null in undo/redo context
          auto sg = this->graph->getDescendantGraph(sid);
          if (sg && !sg->isElement(e))
            continue;
        }
        // if min/max is ok for the current subgraph
        // check if min or max has to be updated
        CONST_EDGE_VALUE minV = it->second.first;
        CONST_EDGE_VALUE maxV = it->second.second;

        // check if min or max has to be updated
        if ((newValue < minV) || (newValue > maxV) || (oldV == minV) || (oldV == maxV)) {
          removeListenersAndClearEdgeMap();
          break;
        }
      }
    }
  }
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateAllNodesValues(
    CONST_NODE_VALUE newValue) {
  auto it = minMaxNode.begin();
  // loop on subgraph min/max
  MINMAX_PAIR(nodeType) minmax(newValue, newValue);

  for (; it != minMaxNode.end(); ++it) {
    unsigned int gid = it->first;
    minMaxNode[gid] = minmax;
  }
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::updateAllEdgesValues(
    CONST_EDGE_VALUE newValue) {
  auto it = minMaxEdge.begin();
  // loop on subgraph min/max
  MINMAX_PAIR(edgeType) minmax(newValue, newValue);

  for (; it != minMaxEdge.end(); ++it) {
    unsigned int gid = it->first;
    minMaxEdge[gid] = minmax;
  }
}

template <typename nodeType, typename edgeType, typename propType>
void tlp::MinMaxProperty<nodeType, edgeType, propType>::treatEvent(const tlp::Event &ev) {
  const GraphEvent *graphEvent = dynamic_cast<const tlp::GraphEvent *>(&ev);

  if (graphEvent) {
    tlp::Graph *graph = graphEvent->getGraph();

    switch (graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      removeListenersAndClearNodeMap();
      break;

    case GraphEvent::TLP_BEFORE_DEL_NODE: {
      unsigned int sgi = graph->getId();
      auto it = minMaxNode.find(sgi);

      if (it != minMaxNode.end()) {
        CONST_NODE_VALUE oldV = this->getNodeValue(graphEvent->getNode());

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

    case GraphEvent::TLP_BEFORE_DEL_EDGE: {
      unsigned int sgi = graph->getId();
      auto it = minMaxEdge.find(sgi);

      if (it != minMaxEdge.end()) {
        EDGE_VALUE oldV = this->getEdgeValue(graphEvent->getEdge());

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
