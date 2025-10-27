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

///@cond DOXYGEN_HIDDEN
struct LessThan {
  LessThan(const tlp::NumericProperty *m) : metric(m) {}
  const tlp::NumericProperty *metric;
  bool operator()(const node &n1, const node &n2) const {
    return (metric->getNodeDoubleValue(n1) < metric->getNodeDoubleValue(n2));
  }
  bool operator()(const edge &e1, const edge &e2) const {
    return (metric->getEdgeDoubleValue(e1) < metric->getEdgeDoubleValue(e2));
  }
};

struct LessThanEdgeTargetMetric {
  LessThanEdgeTargetMetric(const Graph *sg, const tlp::NumericProperty *metric)
      : metric(metric), sg(sg) {}
  bool operator()(const edge &e1, const edge &e2) const {
    return (metric->getNodeDoubleValue(sg->target(e1)) <
            metric->getNodeDoubleValue(sg->target(e2)));
  }

private:
  const tlp::NumericProperty *metric;
  const Graph *sg;
};

struct LessThanEdgeSourceMetric {
  LessThanEdgeSourceMetric(const Graph *sg, const tlp::NumericProperty *metric)
      : metric(metric), sg(sg) {}
  bool operator()(const edge &e1, const edge &e2) const {
    return (metric->getNodeDoubleValue(sg->source(e1)) <
            metric->getNodeDoubleValue(sg->source(e2)));
  }

private:
  const tlp::NumericProperty *metric;
  const Graph *sg;
};

struct LessThanEdgeExtremitiesMetric {
  LessThanEdgeExtremitiesMetric(const Graph *sg, const tlp::NumericProperty *metric)
      : metric(metric), sg(sg) {}
  bool operator()(const edge &e1, const edge &e2) const {
    std::pair<node, node> ends = sg->ends(e1);
    Vec2d v1(metric->getNodeDoubleValue(ends.first), metric->getNodeDoubleValue(ends.second));
    ends = sg->ends(e2);
    Vec2d v2(metric->getNodeDoubleValue(ends.first), metric->getNodeDoubleValue(ends.second));
    return v1 < v2;
  }

private:
  const tlp::NumericProperty *metric;
  const Graph *sg;
};
///@endcond

/**
 * @brief This Iterator sorts the nodes in a sequence based on their values in a NumericProperty.
 **/
struct SortNodeIterator : public StableIterator<tlp::node> {

  SortNodeIterator(Iterator<tlp::node> *itIn, const tlp::NumericProperty *metric,
                   bool ascendingOrder = true)
      : StableIterator<tlp::node>(itIn) {
    LessThan tmp(metric);
    sort(sequenceCopy.begin(), sequenceCopy.end(), tmp);

    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(), sequenceCopy.end());
    }

    copyIterator = sequenceCopy.begin();
  }

  ~SortNodeIterator() override {}
};

/**
 * @brief This Iterator sorts the edges in a sequence based on their values in a NumericProperty.
 **/
struct SortEdgeIterator : public StableIterator<tlp::edge> {

  SortEdgeIterator(Iterator<tlp::edge> *itIn, const tlp::NumericProperty *metric,
                   bool ascendingOrder = true)
      : StableIterator<tlp::edge>(itIn) {
    LessThan tmp(metric);
    sort(sequenceCopy.begin(), sequenceCopy.end(), tmp);

    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(), sequenceCopy.end());
    }

    copyIterator = sequenceCopy.begin();
  }

  ~SortEdgeIterator() override {}
};

/**
 * @brief This Iterator sorts the edges based on the values of their target nodes in a
 *NumericProperty.
 **/
struct SortTargetEdgeIterator : public StableIterator<tlp::edge> {

  SortTargetEdgeIterator(Iterator<tlp::edge> *itIn, const Graph *sg,
                         const tlp::NumericProperty *metric, bool ascendingOrder = true)
      : StableIterator<tlp::edge>(itIn) {
    LessThanEdgeTargetMetric tmp(sg, metric);
    sort(sequenceCopy.begin(), sequenceCopy.end(), tmp);

    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(), sequenceCopy.end());
    }

    copyIterator = sequenceCopy.begin();
  }

  ~SortTargetEdgeIterator() override {}
};

/**
 * @brief This Iterator sorts the edges based on the values of their source nodes in a
 *NumericProperty.
 **/
struct SortSourceEdgeIterator : public StableIterator<tlp::edge> {

  SortSourceEdgeIterator(Iterator<tlp::edge> *itIn, const Graph *sg,
                         const tlp::NumericProperty *metric, bool ascendingOrder = true)
      : StableIterator<tlp::edge>(itIn) {
    LessThanEdgeSourceMetric tmp(sg, metric);
    sort(sequenceCopy.begin(), sequenceCopy.end(), tmp);

    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(), sequenceCopy.end());
    }

    copyIterator = sequenceCopy.begin();
  }

  ~SortSourceEdgeIterator() override {}
};

/**
 * @brief This Iterator sorts the edges based on the values of their extremities nodes in a
 *NumericProperty.
 **/
struct SortExtremitiesEdgeIterator : public StableIterator<tlp::edge> {

  SortExtremitiesEdgeIterator(Iterator<tlp::edge> *itIn, const Graph *sg,
                              const tlp::NumericProperty *metric, bool ascendingOrder = true)
      : StableIterator<tlp::edge>(itIn) {
    LessThanEdgeExtremitiesMetric tmp(sg, metric);
    sort(sequenceCopy.begin(), sequenceCopy.end(), tmp);

    if (!ascendingOrder) {
      reverse(sequenceCopy.begin(), sequenceCopy.end());
    }

    copyIterator = sequenceCopy.begin();
  }

  ~SortExtremitiesEdgeIterator() override {}
};

/**
 * @class SortIterator
 * @ingroup Iterators
 * @brief Iterator that wraps an existing one and sorts its iterated elements based on comparison
 *function.
 * @since Tulip 5.2
 * @param it the iterator to sort
 * @param comp functor or lambda function taking two parameters of type const T& and returning a
 * Boolean:
 * true if the first parameter is lesser or equal than the second one, false otherwise
 *
 **/
template <typename T, typename CompareFunction>
struct SortIterator : public tlp::StableIterator<T> {

  SortIterator(Iterator<T> *itIn, CompareFunction &&compFunc) : StableIterator<T>(itIn) {
    sort(this->sequenceCopy.begin(), this->sequenceCopy.end(), compFunc);
    this->copyIterator = this->sequenceCopy.begin();
  }

  ~SortIterator() {}
};

/**
 * @brief Convenient function for creating a SortIterator.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a SortIterator from another Iterator and a comparison function.
 * The returned Iterator takes ownership of the one provided as parameter.
 *
 * @param it a Tulip Iterator
 * @param compFunc functor or lambda function taking two parameters of type const T& and returning a
 * Boolean:
 * true if the first parameter is lesser or equal than the second one, false otherwise
 *
 * @return a SortIterator
 *
 **/
template <typename T, typename CompareFunction>
inline SortIterator<T, CompareFunction> *sortIterator(Iterator<T> *it, CompareFunction &&compFunc) {
  return new SortIterator<T, CompareFunction>(it, compFunc);
}

/**
 * @brief Convenient function for creating a SortIterator from a STL container.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a SortIterator from another Iterator and a comparison function.
 *
 * @param stlContainer any STL container
 * @param compFunc functor or lambda function taking two parameters of type const T& and returning a
 * Boolean:
 * true if the first parameter is lesser or equal than the second one, false otherwise
 *
 * @return a SortIterator
 *
 **/
template <typename Container, typename CompareFunction>
typename std::enable_if<has_const_iterator<Container>::value,
                        SortIterator<typename Container::value_type, CompareFunction>
                            *>::type inline sortIterator(const Container &stlContainer,
                                                         CompareFunction &&compFunc) {
  return new SortIterator<typename Container::value_type, CompareFunction>(
      stlIterator(stlContainer), compFunc);
}
} // namespace tlp
#endif
