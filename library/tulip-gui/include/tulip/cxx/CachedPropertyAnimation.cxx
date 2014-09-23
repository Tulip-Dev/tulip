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
#include <tulip/ForEach.h>

template<typename PropType, typename NodeType, typename EdgeType>
CachedPropertyAnimation<PropType, NodeType, EdgeType>::CachedPropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out,
    tlp::BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges,QObject* parent) :
  PropertyAnimation<PropType, NodeType, EdgeType> (graph, start, end, out, selection, frameCount, computeNodes, computeEdges,parent) {

  if (this->_computeNodes) {
    tlp::node n;
    forEach (n, this->_graph->getNodes()) {
      if (this->equalNodes(this->_end->getNodeValue(n), this->_start->getNodeValue(n))) {
        this->_selection->setNodeValue(n, false);
        //Init the out properties with the default value.
        this->_out->setNodeValue(n,this->_end->getNodeValue(n));
      }
    }
  }

  if (this->_computeEdges) {
    tlp::edge e;
    forEach (e, this->_graph->getEdges()) {
      if (this->equalEdges(this->_end->getEdgeValue(e), this->_start->getEdgeValue(e))) {
        this->_selection->setEdgeValue(e, false);
        //Init the out properties with the default value.
        this->_out->setEdgeValue(e,end->getEdgeValue(e));
      }
    }
  }
}

template<typename PropType, typename NodeType, typename EdgeType>
CachedPropertyAnimation<PropType, NodeType, EdgeType>::~CachedPropertyAnimation() {
}

template<typename PropType, typename NodeType, typename EdgeType>
void CachedPropertyAnimation<PropType, NodeType, EdgeType>::frameChanged(int f) {
  if (this->_computeNodes) {
    computedNodeSteps.clear();
    tlp::node n;
    forEach(n, this->_graph->getNodes()) {
      if (this->_selection && !this->_selection->getNodeValue(n))
        continue;

      std::pair<NodeType, NodeType> values(this->_start->getNodeValue(n), this->_end->getNodeValue(n));
      NodeType frameValue;

      if (computedNodeSteps.find(values) == computedNodeSteps.end()) {
        frameValue = this->getNodeFrameValue(values.first, values.second, f);
        computedNodeSteps[values] = frameValue;
      }
      else
        frameValue = computedNodeSteps[values];

      this->_out->setNodeValue(n, frameValue);
    }
  }

  if (this->_computeEdges) {
    computedEdgeSteps.clear();
    tlp::edge e;
    forEach(e, this->_graph->getEdges()) {
      if (this->_selection && !this->_selection->getEdgeValue(e))
        continue;

      std::pair<EdgeType, EdgeType> values(this->_start->getEdgeValue(e), this->_end->getEdgeValue(e));
      EdgeType frameValue;

      if (computedEdgeSteps.find(values) == computedEdgeSteps.end()) {
        frameValue = this->getEdgeFrameValue(values.first, values.second, f);
        computedEdgeSteps[values] = frameValue;
      }
      else
        frameValue = computedEdgeSteps[values];

      this->_out->setEdgeValue(e, frameValue);
    }
  }
}
