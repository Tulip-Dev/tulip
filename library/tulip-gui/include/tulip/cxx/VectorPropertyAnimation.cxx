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

template<typename PropType, typename RealType, typename VectorType, unsigned int SIZE>
VectorPropertyAnimation<PropType, RealType, VectorType, SIZE>::VectorPropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end,
    PropType *out, tlp::BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges,QObject* parent) :
  CachedPropertyAnimation<PropType, RealType, RealType> (graph, start, end, out, selection, frameCount, computeNodes, computeEdges,parent) {

  assert(frameCount > 1);

  if (this->_computeNodes) {
    tlp::node n;
    forEach(n, this->_graph->getNodes()) {
      if (this->_selection && !this->_selection->getNodeValue(n))
        continue;

      std::pair<PropVector, PropVector> values(PropVector(this->_start->getNodeValue(n)), PropVector(this->_end->getNodeValue(n)));

      if (steps.find(values) == steps.end()) {
        tlp::Vector<double, SIZE> stepsVector;

        for (unsigned int i = 0; i < SIZE; ++i)
          stepsVector[i] = ((double)values.second[i] - (double)values.first[i]) * 1. / (frameCount - 1);

        steps[values] = stepsVector;
      }
    }
  }

  if (this->_computeEdges) {
    tlp::edge e;
    forEach(e, this->_graph->getEdges()) {
      if (this->_selection && !this->_selection->getEdgeValue(e))
        continue;

      std::pair<PropVector, PropVector> values(PropVector(this->_start->getEdgeValue(e)), PropVector(this->_end->getEdgeValue(e)));

      if (steps.find(values) == steps.end()) {
        tlp::Vector<double, SIZE> stepsVector;

        for (unsigned int i = 0; i < SIZE; ++i)
          stepsVector[i] = ((double) (values.second[i] - (double)values.first[i])) * 1. / (frameCount - 1);

        steps[values] = stepsVector;
      }
    }
  }
}

template<typename PropType, typename RealType, typename VectorType, unsigned int SIZE>
RealType VectorPropertyAnimation<PropType, RealType, VectorType, SIZE>::getNodeFrameValue(const RealType &startValue, const RealType &endValue,
    int frame) {
  PropVector result((PropVector)startValue);
  std::pair<PropVector, PropVector> values(result, (PropVector) endValue);
  tlp::Vector<double, SIZE> stepVal(steps[values]);

  for (unsigned i=0; i < SIZE; ++i)
    result[i] += stepVal[i] * frame;

  return RealType(result);
}

template<typename PropType, typename RealType, typename VectorType, unsigned int SIZE>
RealType VectorPropertyAnimation<PropType, RealType, VectorType, SIZE>::getEdgeFrameValue(const RealType &startValue, const RealType &endValue,
    int frame) {
  return getNodeFrameValue(startValue, endValue, frame);
}
