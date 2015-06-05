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
#include <tulip/NumericProperty.h>
#include <tulip/Graph.h>
#include <tulip/Vector.h>

namespace tlp {
class Graph;

#ifndef DOXYGEN_NOTFOR_DEVEL
struct LessThan {
  LessThan(tlp::NumericProperty* m): metric(m) {
  }
  tlp::NumericProperty* metric;
  bool operator() (const node &n1, const node &n2) const {
    return (metric->getNodeDoubleValue(n1) < metric->getNodeDoubleValue(n2));
  }
  bool operator() (const edge &e1, const edge &e2) const {
    return (metric->getEdgeDoubleValue(e1) < metric->getEdgeDoubleValue(e2));
  }
};

struct LessThanEdgeTargetMetric {
  LessThanEdgeTargetMetric(Graph *sg, tlp::NumericProperty* metric):
    metric(metric), sg(sg) {
  }
  bool operator() (const edge &e1, const edge &e2) const {
    return (metric->getNodeDoubleValue(sg->target(e1)) < metric->getNodeDoubleValue(sg->target(e2)));
  }
private:
  tlp::NumericProperty* metric;
  Graph *sg;
};

struct LessThanEdgeSourceMetric {
  LessThanEdgeSourceMetric(Graph *sg, tlp::NumericProperty* metric):
    metric(metric), sg(sg) {
  }
  bool operator() (const edge &e1, const edge &e2) const {
    return (metric->getNodeDoubleValue(sg->source(e1)) < metric->getNodeDoubleValue(sg->source(e2)));
  }
private:
  tlp::NumericProperty* metric;
  Graph *sg;
};

struct LessThanEdgeExtremitiesMetric {
  LessThanEdgeExtremitiesMetric(Graph *sg, tlp::NumericProperty* metric):
    metric(metric), sg(sg) {
  }
  bool operator() (const edge &e1, const edge &e2) const {
    Vec2d v1(metric->getNodeDoubleValue(sg->source(e1)), metric->getNodeDoubleValue(sg->target(e1)));
    Vec2d v2(metric->getNodeDoubleValue(sg->source(e2)), metric->getNodeDoubleValue(sg->target(e2)));
    return v1 < v2;
  }
private:
  tlp::NumericProperty* metric;
  Graph *sg;
};

#endif // DOXYGEN_NOTFOR_DEVEL


/**
* @brief This Iterator sorts the nodes in a sequence based on their values in a NumericProperty.
**/
struct SortNodeIterator : public StableIterator<tlp::node> {
  ///
  SortNodeIterator(Iterator<tlp::node> *itIn, tlp::NumericProperty* metric, bool ascendingOrder = true) : StableIterator<tlp::node>(itIn) {
    LessThan tmp(metric);
    sort(sequenceCopy.begin(),sequenceCopy.end(),tmp);
    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(),sequenceCopy.end());
    }
    copyIterator=sequenceCopy.begin();
  }
  ///
  ~SortNodeIterator() {}
};

/**
* @brief This Iterator sorts the edges in a sequence based on their values in a NumericProperty.
**/
struct SortEdgeIterator : public StableIterator<tlp::edge> {
  ///
  SortEdgeIterator(Iterator<tlp::edge> *itIn, tlp::NumericProperty* metric, bool ascendingOrder = true) : StableIterator<tlp::edge>(itIn) {
    LessThan tmp(metric);
    sort(sequenceCopy.begin(),sequenceCopy.end(),tmp);
    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(),sequenceCopy.end());
    }
    copyIterator=sequenceCopy.begin();
  }
  ///
  ~SortEdgeIterator() {}
};

/**
* @brief This Iterator sorts the edges based on the values of their target nodes in a NumericProperty.
**/
struct SortTargetEdgeIterator : public StableIterator<tlp::edge> {
  ///
  SortTargetEdgeIterator(Iterator<tlp::edge> *itIn, Graph* sg, tlp::NumericProperty* metric, bool ascendingOrder = true) : StableIterator<tlp::edge>(itIn) {
    LessThanEdgeTargetMetric tmp(sg,metric);
    sort(sequenceCopy.begin(),sequenceCopy.end(),tmp);
    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(),sequenceCopy.end());
    }
    copyIterator=sequenceCopy.begin();
  }
  ///
  ~SortTargetEdgeIterator() {}
};

/**
* @brief This Iterator sorts the edges based on the values of their source nodes in a NumericProperty.
**/
struct SortSourceEdgeIterator : public StableIterator<tlp::edge> {
  ///
  SortSourceEdgeIterator(Iterator<tlp::edge> *itIn, Graph* sg, tlp::NumericProperty* metric, bool ascendingOrder = true) : StableIterator<tlp::edge>(itIn) {
    LessThanEdgeSourceMetric tmp(sg,metric);
    sort(sequenceCopy.begin(),sequenceCopy.end(),tmp);
    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(),sequenceCopy.end());
    }
    copyIterator=sequenceCopy.begin();
  }
  ///
  ~SortSourceEdgeIterator() {}
};

/**
* @brief This Iterator sorts the edges based on the values of their extremities nodes in a NumericProperty.
**/
struct SortExtremitiesEdgeIterator : public StableIterator<tlp::edge> {
  ///
  SortExtremitiesEdgeIterator(Iterator<tlp::edge> *itIn, Graph* sg, tlp::NumericProperty* metric, bool ascendingOrder = true) : StableIterator<tlp::edge>(itIn) {
    LessThanEdgeExtremitiesMetric tmp(sg,metric);
    sort(sequenceCopy.begin(),sequenceCopy.end(),tmp);
    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(),sequenceCopy.end());
    }
    copyIterator=sequenceCopy.begin();
  }
  ///
  ~SortExtremitiesEdgeIterator() {}
};

}
#endif
///@endcond
