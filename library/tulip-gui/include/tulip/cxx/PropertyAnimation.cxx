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
PropertyAnimation<PropType, NodeType, EdgeType>::PropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out,
    tlp::BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges, QObject* parent) :

  Animation(frameCount,parent), _graph(graph), _start(0), _end(0), _out(out), _computeNodes(computeNodes), _computeEdges(computeEdges) {

  assert(out);
  assert(start);
  assert(end);
  assert(end->getGraph()->getRoot() == start->getGraph()->getRoot());
  assert(end->getGraph()->getRoot() == out->getGraph()->getRoot());
  assert(graph);
  assert(frameCount > 0);

  this->_start = new PropType(start->getGraph());
  *(this->_start) = *start;
  this->_end = new PropType(end->getGraph());
  *(this->_end) = *end;

  if (!selection) {
    this->_selection = new tlp::BooleanProperty(graph);
    this->_selection->setAllNodeValue(true);
    this->_selection->setAllEdgeValue(true);
  }
  else {
    this->_selection = new tlp::BooleanProperty(selection->getGraph());
    *(this->_selection) = *selection;
  }
}

template<typename PropType, typename NodeType, typename EdgeType>
PropertyAnimation<PropType, NodeType, EdgeType>::~PropertyAnimation() {
  delete _start;
  delete _end;
  delete _selection;
}

template<typename PropType, typename NodeType, typename EdgeType>
void PropertyAnimation<PropType, NodeType, EdgeType>::frameChanged(int f) {
  if (_computeNodes) {
    tlp::node n;
    forEach(n, _graph->getNodes()) {
      if (_selection->getNodeValue(n))
        _out->setNodeValue(n, getNodeFrameValue(_start->getNodeValue(n), _end->getNodeValue(n), f));
    }
  }

  if (_computeEdges) {
    tlp::edge e;
    forEach(e, _graph->getEdges()) {
      if (_selection->getEdgeValue(e))
        _out->setEdgeValue(e, getEdgeFrameValue(_start->getEdgeValue(e), _end->getEdgeValue(e), f));
    }
  }
}
