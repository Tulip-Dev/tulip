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

#ifdef _OPENMP
#include <omp.h>
#endif
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
    return (*this)[graph->nodePos(n)];
  }

  // set the stored value of a node
  inline typename std::vector<TYPE>::const_reference setNodeValue(node n, TYPE val) {
    return (*this)[graph->nodePos(n)] = val;
  }

  // set all to same values
  void setAll(const TYPE &val) {
#ifdef _OPENMP
#pragma omp parallel for
#endif

    for (OMP_ITER_TYPE i = 0; i < static_cast<OMP_ITER_TYPE>(this->size()); ++i)
      (*this)[i] = val;
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
    unsigned int nbNodes = nodes.size();
#ifdef _OPENMP
#pragma omp parallel for
#endif

    for (OMP_ITER_TYPE i = 0; i < nbNodes; ++i)
      (*this)[i] = prop->getNodeValue(nodes[i]);
  }

  // copy values into a typed typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<node> &nodes = graph->nodes();
    unsigned int nbNodes = nodes.size();

    for (unsigned int i = 0; i < nbNodes; ++i)
      prop->setNodeValue(nodes[i], (*this)[i]);
  }
};

// vector<bool> does not support // update
// so do an iterative specialization
template <>
template <typename PROP_PTR>
void NodeStaticProperty<bool>::copyFromProperty(PROP_PTR prop) {
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i)
    (*this)[i] = prop->getNodeValue(nodes[i]);
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
    return (*this)[graph->edgePos(n)];
  }

  // set the stored value of a edge
  inline typename std::vector<TYPE>::const_reference setEdgeValue(edge n, TYPE val) {
    return (*this)[graph->edgePos(n)] = val;
  }

  void setAll(const TYPE &val) {
#ifdef _OPENMP
#pragma omp parallel for
#endif

    for (OMP_ITER_TYPE i = 0; i < static_cast<OMP_ITER_TYPE>(this->size()); ++i)
      (*this)[i] = val;
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
    unsigned int nbEdges = edges.size();
#ifdef _OPENMP
#pragma omp parallel for
#endif

    for (OMP_ITER_TYPE i = 0; i < nbEdges; ++i)
      (*this)[i] = prop->getEdgeValue(edges[i]);
  }

  // copy values into a typed typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<edge> &edges = graph->edges();
    unsigned int nbEdges = edges.size();

    for (unsigned int i = 0; i < nbEdges; ++i)
      prop->setEdgeValue(edges[i], (*this)[i]);
  }
};

// vector<bool> does not support // update
// so do an iterative specialization
template <>
template <typename PROP_PTR>
void EdgeStaticProperty<bool>::copyFromProperty(PROP_PTR prop) {
  const std::vector<edge> &edges = graph->edges();
  unsigned int nbEdges = edges.size();

  for (unsigned int i = 0; i < nbEdges; ++i)
    (*this)[i] = prop->getEdgeValue(edges[i]);
}
}

#endif
