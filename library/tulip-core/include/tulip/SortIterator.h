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
///@cond DOXYGEN_HIDDEN

#ifndef TULIP_SORTITERATOR_H
#define TULIP_SORTITERATOR_H
#include <vector>
#include <algorithm>
#include <tulip/Iterator.h>
#include <tulip/StableIterator.h>
#include <tulip/DoubleProperty.h>

namespace tlp {
class Graph;

#ifndef DOXYGEN_NOTFOR_DEVEL
struct LessThan {
  LessThan(DoubleProperty* m): metric(m) {
  }
  DoubleProperty* metric;
  bool operator() (node n1,node n2) {
    return (metric->getNodeValue(n1) < metric->getNodeValue(n2));
  }
};

struct LessThanEdgeTargetMetric {
  LessThanEdgeTargetMetric(Graph *sg, DoubleProperty* metric):
    metric(metric), sg(sg) {
  }
  bool operator() (edge e1,edge e2) {
    return (metric->getNodeValue(sg->target(e1)) < metric->getNodeValue(sg->target(e2)));
  }
private:
  DoubleProperty* metric;
  Graph *sg;
};
#endif // DOXYGEN_NOTFOR_DEVEL


/**
* @brief This Iterator sorts the nodes in a sequence based on their values in a DoubleProperty.
**/
struct SortNodeIterator : public StableIterator<tlp::node> {
  ///
  SortNodeIterator(Iterator<tlp::node> *itIn, DoubleProperty* metric):StableIterator<tlp::node>(itIn) {
    LessThan tmp(metric);
    sort(sequenceCopy.begin(),sequenceCopy.end(),tmp);
    copyIterator=sequenceCopy.begin();
  }
  ///
  ~SortNodeIterator() {}
};

/**
* @brief This Iterator sorts the edges based on the values of their target nodes in a DoubleProperty.
**/
struct SortTargetEdgeIterator : public StableIterator<tlp::edge> {
  ///
  SortTargetEdgeIterator(Iterator<tlp::edge> *itIn, Graph* sg, DoubleProperty* metric):StableIterator<tlp::edge>(itIn) {
    LessThanEdgeTargetMetric tmp(sg,metric);
    sort(sequenceCopy.begin(),sequenceCopy.end(),tmp);
    copyIterator=sequenceCopy.begin();
  }
  ///
  ~SortTargetEdgeIterator() {}
};

}
#endif
///@endcond
