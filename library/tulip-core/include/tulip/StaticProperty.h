/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#ifndef STATICPROPERTY_H
#define STATICPROPERTY_H

#include <tulip/Graph.h>

namespace tlp {
template <typename TYPE>
class NodeStaticProperty : public std::vector<TYPE> {
  const Graph *graph;

public:
  // constructor
  NodeStaticProperty(const Graph *g) : graph(g) {
    assert(g);
    // set the vector size to the number of graph nodes
    this->resize(graph->numberOfNodes());
  }

  // get the stored value of a node
  inline typename std::vector<TYPE>::const_reference getNodeValue(node n) const {
    return std::vector<TYPE>::operator[](graph->nodePos(n));
  }

  // set the stored value of a node
  inline void setNodeValue(node n, TYPE val) {
    std::vector<TYPE>::operator[](graph->nodePos(n)) = val;
  }

  inline typename std::vector<TYPE>::const_reference operator[](node n) const {
    return std::vector<TYPE>::operator[][graph->nodePos(n)];
  }

  inline typename std::vector<TYPE>::reference operator[](node n) {
    return std::vector<TYPE>::operator[](graph->nodePos(n));
  }

  // set all to same values
  void setAll(const TYPE &val) {
    const std::vector<node> &nodes = graph->nodes();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(graph->numberOfNodes()); ++i) {
      (*this)[nodes[i]] = val;
    }
  }

  // add a value for a newly created node
  void addNodeValue(node n, TYPE val) {
    unsigned int nPos = graph->nodePos(n);

    if (nPos + 1 > this->size())
      this->resize(nPos + 1);

    setNodeValue(n, val);
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    const std::vector<node> &nodes = graph->nodes();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(graph->numberOfNodes()); ++i) {
      (*this)[nodes[i]] = prop->getNodeValue(nodes[i]);
    }
  }

  // copy values into a typed typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    for (const node &n : graph->nodes())
      prop->setNodeValue(n, (*this)[n]);
  }
};

// vector<bool> does not support // update
// so do an iterative specialization
template <>
template <typename PROP_PTR>
void NodeStaticProperty<bool>::copyFromProperty(PROP_PTR prop) {
  for (const node &n : graph->nodes())
    (*this)[n] = prop->getNodeValue(n);
}

template <typename TYPE>
class EdgeStaticProperty : public std::vector<TYPE> {
  const Graph *graph;

public:
  // constructor
  EdgeStaticProperty(const Graph *g) : graph(g) {
    assert(g);
    // set the vector size to the number of graph edges
    this->resize(graph->numberOfEdges());
  }

  // get the stored value of a edge
  inline typename std::vector<TYPE>::const_reference getEdgeValue(edge n) const {
    return std::vector<TYPE>::operator[](graph->edgePos(n));
  }

  // set the stored value of a edge
  inline void setEdgeValue(edge n, TYPE val) {
    std::vector<TYPE>::operator[](graph->edgePos(n)) = val;
  }

  inline typename std::vector<TYPE>::const_reference operator[](edge e) const {
    return std::vector<TYPE>::operator[](graph->edgePos(e));
  }

  inline typename std::vector<TYPE>::reference operator[](edge e) {
    return std::vector<TYPE>::operator[](graph->edgePos(e));
  }

  void setAll(const TYPE &val) {
    const std::vector<edge> &edges = graph->edges();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(graph->numberOfEdges()); ++i) {
      (*this)[edges[i]] = val;
    }
  }

  // add a value for a newly created edge
  void addEdgeValue(edge n, TYPE val) {
    unsigned int nPos = graph->edgePos(n);

    if (nPos + 1 > this->size())
      this->resize(nPos + 1);

    setEdgeValue(n, val);
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    const std::vector<edge> &edges = graph->edges();
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(graph->numberOfEdges()); ++i) {
      (*this)[edges[i]] = prop->getNodeValue(edges[i]);
    }
  }

  // copy values into a typed typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    for (const edge &e : graph->edges())
      prop->setEdgeValue(e, (*this)[e]);
  }
};

// vector<bool> does not support // update
// so do an iterative specialization
template <>
template <typename PROP_PTR>
void EdgeStaticProperty<bool>::copyFromProperty(PROP_PTR prop) {
  for (const edge &e : graph->edges())
    (*this)[e] = prop->getEdgeValue(e);
}
}

#endif
