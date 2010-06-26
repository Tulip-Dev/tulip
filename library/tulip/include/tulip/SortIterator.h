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
#ifndef TULIP_SORTITERATOR_H
#define TULIP_SORTITERATOR_H
#include <vector>
#include <tulip/Iterator.h>
#include <tulip/StableIterator.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Graph.h>

namespace tlp {

#ifndef DOXYGEN_NOTFOR_DEVEL
struct LessThan {
  DoubleProperty* metric;
  bool operator() (node n1,node n2) {
    return (metric->getNodeValue(n1) < metric->getNodeValue(n2));
  } 
};

struct LessThanEdgeTargetMetric {
  LessThanEdgeTargetMetric(Graph *sg, DoubleProperty* metric):
    metric(metric), sg(sg) {
  }
  bool operator() (edge e1,edge e2){
    return (metric->getNodeValue(sg->target(e1)) < metric->getNodeValue(sg->target(e2)));
  }
private:
  DoubleProperty* metric;
  Graph *sg;
};
#endif // DOXYGEN_NOTFOR_DEVEL

///Interface of Sortiterator,
struct SortNodeIterator : public StableIterator<node> {
  ///
  SortNodeIterator(Iterator<node> *itIn, DoubleProperty* metric):StableIterator<node>(itIn) {
    LessThan tmp;
    tmp.metric=metric;
    sort(cloneIt.begin(),cloneIt.end(),tmp);
    itStl=cloneIt.begin();
  }
  ///
  ~SortNodeIterator(){};
};

///Interface of Sortiterator,
struct SortTargetEdgeIterator : public StableIterator<edge> {
  ///
  SortTargetEdgeIterator(Iterator<edge> *itIn, Graph* sg, DoubleProperty* metric):StableIterator<edge>(itIn) {
    LessThanEdgeTargetMetric tmp(sg,metric);
    sort(cloneIt.begin(),cloneIt.end(),tmp);
    itStl=cloneIt.begin();
  }
  ///
  ~SortTargetEdgeIterator(){};
};

}
#endif
