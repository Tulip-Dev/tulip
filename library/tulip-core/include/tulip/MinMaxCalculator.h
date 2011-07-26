/**
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
#ifndef MINMAXCALCULATOR_H
#define MINMAXCALCULATOR_H

#include <tulip/tuliphash.h>
#include <tulip/Observable.h>

namespace tlp {

template<class Tnode, class Tedge, class TPROPERTY>
class AbstractProperty;

template<typename nodeType, typename edgeType, typename algorithmType>
class MinMaxCalculator {
public:
  MinMaxCalculator(tlp::AbstractProperty<nodeType, edgeType, algorithmType>* property, typename nodeType::RealType NodeMin,
                   typename nodeType::RealType NodeMax, typename edgeType::RealType EdgeMin, typename edgeType::RealType EdgeMax);
  MinMaxCalculator(tlp::AbstractProperty<nodeType, edgeType, algorithmType>* property, typename nodeType::RealType NodeMin,
                   typename nodeType::RealType NodeMax);
  virtual void treatEvent(const tlp::Event& ev);

  typename nodeType::RealType getNodeMin(Graph* graph);
  typename nodeType::RealType getNodeMax(Graph* graph);
  typename edgeType::RealType getEdgeMin(Graph* graph);
  typename edgeType::RealType getEdgeMax(Graph* graph);

  void updateNodeValue(tlp::node n, typename nodeType::RealType newValue);
  void updateEdgeValue(tlp::edge e, typename edgeType::RealType newValue);

  void updateAllNodesValues(typename nodeType::RealType newValue);
  void updateAllEdgesValues(typename edgeType::RealType newValue);
  
protected:
  tlp::AbstractProperty<nodeType, edgeType, algorithmType>* _property;
  TLP_HASH_MAP<unsigned int, typename nodeType::RealType> maxNode, minNode;
  TLP_HASH_MAP<unsigned int, typename edgeType::RealType> maxEdge, minEdge;
  TLP_HASH_MAP<unsigned int, bool> nodeValueUptodate;
  TLP_HASH_MAP<unsigned int, bool> edgeValueUptodate;

  typename nodeType::RealType _nodeMin;
  typename nodeType::RealType _nodeMax;
  typename edgeType::RealType _edgeMin;
  typename edgeType::RealType _edgeMax;
  
  void computeMinMaxNode(Graph* graph);
  void computeMinMaxEdge(Graph* graph);
};

}

#include "cxx/MinMaxCalculator.cxx"

#endif //MINMAXCALCULATOR_H
