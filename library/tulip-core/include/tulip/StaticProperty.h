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
#ifndef STATICPROPERTY_H
#define STATICPROPERTY_H

#include <tulip/Graph.h>
#include <tulip/GraphParallelTools.h>
#include <tulip/NumericProperty.h>

namespace tlp {
template <typename TYPE>
class NodeStaticProperty : public std::vector<TYPE> {
  const Graph *graph;

public:
  // constructors
  NodeStaticProperty(const Graph *g) : std::vector<TYPE>(g->numberOfNodes()), graph(g) {}

  NodeStaticProperty(const Graph *g, TYPE val)
      : std::vector<TYPE>(g->numberOfNodes(), val), graph(g) {}

  inline typename std::vector<TYPE>::const_reference operator[](unsigned int i) const {
    return std::vector<TYPE>::operator[](i);
  }

  inline typename std::vector<TYPE>::reference operator[](unsigned int i) {
    return std::vector<TYPE>::operator[](i);
  }

  inline typename std::vector<TYPE>::const_reference operator[](node n) const {
    return (*this)[graph->nodePos(n)];
  }

  inline typename std::vector<TYPE>::reference operator[](node n) {
    return (*this)[graph->nodePos(n)];
  }

  // get the stored value of a node
  inline typename std::vector<TYPE>::const_reference getNodeValue(node n) const {
    return (*this)[n];
  }

  // set the stored value of a node
  inline void setNodeValue(node n, TYPE val) {
    (*this)[n] = val;
  }

  // set all to same values
  void setAll(const TYPE &val) {
    TLP_PARALLEL_MAP_INDICES(graph->numberOfNodes(), [&](unsigned int i) { (*this)[i] = val; });
  }

  // add a value for a newly created node
  void addNodeValue(node n, TYPE val) {
    unsigned int nPos = graph->nodePos(n);

    if (nPos + 1 > this->size())
      this->resize(nPos + 1);

    (*this)[nPos] = val;
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    TLP_PARALLEL_MAP_NODES_AND_INDICES(
        graph, [&](const node n, unsigned int i) { (*this)[i] = prop->getNodeValue(n); });
  }

  // get values from a NumericProperty
  void copyFromNumericProperty(const NumericProperty *prop) {
    TLP_PARALLEL_MAP_NODES_AND_INDICES(
        graph, [&](const node n, unsigned int i) { (*this)[i] = prop->getNodeDoubleValue(n); });
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

template <>
class NodeStaticProperty<bool> : public std::vector<unsigned char> {
  const Graph *graph;

public:
  // constructors
  NodeStaticProperty(const Graph *g) : std::vector<unsigned char>(g->numberOfNodes()), graph(g) {}

  NodeStaticProperty(const Graph *g, bool b)
      : std::vector<unsigned char>(g->numberOfNodes(), b), graph(g) {}

  inline const Graph *getGraph() const {
    return graph;
  }

  inline bool operator[](unsigned int i) const {
    return static_cast<bool>(std::vector<unsigned char>::operator[](i));
  }

  inline std::vector<unsigned char>::reference operator[](unsigned int i) {
    return std::vector<unsigned char>::operator[](i);
  }

  inline bool operator[](node n) const {
    return (*this)[graph->nodePos(n)];
  }

  inline std::vector<unsigned char>::reference operator[](node n) {
    return (*this)[graph->nodePos(n)];
  }

  // get the stored value of a node
  inline bool getNodeValue(node n) const {
    return (*this)[graph->nodePos(n)];
  }

  // set the stored value of a node
  inline void setNodeValue(node n, bool val) {
    (*this)[graph->nodePos(n)] = val;
  }

  // set all to same values
  void setAll(const bool &val) {
    TLP_PARALLEL_MAP_INDICES(graph->numberOfNodes(), [&](unsigned int i) { (*this)[i] = val; });
  }

  // add a value for a newly created node
  void addNodeValue(node n, bool val) {
    unsigned int nPos = graph->nodePos(n);

    if (nPos + 1 > this->size())
      this->resize(nPos + 1);

    (*this)[nPos] = val;
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    TLP_PARALLEL_MAP_NODES_AND_INDICES(
        graph, [&](const node n, unsigned int i) { (*this)[i] = prop->getNodeValue(n); });
  }

  // copy values into a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<node> &nodes = graph->nodes();
    unsigned int nbNodes = nodes.size();

    for (unsigned int i = 0; i < nbNodes; ++i)
      prop->setNodeValue(nodes[i], (*this)[i]);
  }
};

template <typename TYPE>
class EdgeStaticProperty : public std::vector<TYPE> {
  const Graph *graph;

public:
  // constructors
  EdgeStaticProperty(const Graph *g) : std::vector<TYPE>(g->numberOfEdges()), graph(g) {}
  EdgeStaticProperty(const Graph *g, TYPE val)
      : std::vector<TYPE>(g->numberOfEdges(), val), graph(g) {}

  inline const Graph *getGraph() const {
    return graph;
  }

  inline typename std::vector<TYPE>::const_reference operator[](unsigned int i) const {
    return std::vector<TYPE>::operator[](i);
  }

  inline typename std::vector<TYPE>::reference operator[](unsigned int i) {
    return std::vector<TYPE>::operator[](i);
  }

  inline typename std::vector<TYPE>::const_reference operator[](edge e) const {
    return (*this)[graph->edgePos(e)];
  }

  inline typename std::vector<TYPE>::reference operator[](edge e) {
    return (*this)[graph->edgePos(e)];
  }

  // get the stored value of a edge
  inline typename std::vector<TYPE>::const_reference getEdgeValue(edge e) const {
    return (*this)[e];
  }

  // set the stored value of a edge
  inline void setEdgeValue(edge e, TYPE val) {
    (*this)[e] = val;
  }

  void setAll(const TYPE &val) {
    TLP_PARALLEL_MAP_INDICES(graph->numberOfEdges(), [&](unsigned int i) { (*this)[i] = val; });
  }

  // add a value for a newly created edge
  void addEdgeValue(edge e, TYPE val) {
    unsigned int ePos = graph->edgePos(e);

    if (ePos + 1 > this->size())
      this->resize(ePos + 1);

    (*this)[ePos] = val;
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(
        graph, [&](const edge e, unsigned int i) { (*this)[i] = prop->getEdgeValue(e); });
  }

  // get values from a NumericProperty
  void copyFromNumericProperty(const NumericProperty *prop) {
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(
        graph, [&](const edge e, unsigned int i) { (*this)[i] = prop->getEdgeDoubleValue(e); });
  }

  // copy values into a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<edge> &edges = graph->edges();
    unsigned int nbEdges = edges.size();

    for (unsigned int i = 0; i < nbEdges; ++i)
      prop->setEdgeValue(edges[i], (*this)[i]);
  }
};

template <>
class EdgeStaticProperty<bool> : public std::vector<unsigned char> {
  const Graph *graph;

public:
  // constructors
  EdgeStaticProperty(const Graph *g) : std::vector<unsigned char>(g->numberOfEdges()), graph(g) {}

  EdgeStaticProperty(const Graph *g, bool b)
      : std::vector<unsigned char>(g->numberOfEdges(), b), graph(g) {}

  inline bool operator[](unsigned int i) const {
    return static_cast<bool>(std::vector<unsigned char>::operator[](i));
  }

  inline std::vector<unsigned char>::reference operator[](unsigned int i) {
    return std::vector<unsigned char>::operator[](i);
  }

  inline bool operator[](edge e) const {
    return (*this)[graph->edgePos(e)];
  }

  inline std::vector<unsigned char>::reference operator[](edge e) {
    return (*this)[graph->edgePos(e)];
  }

  // get the stored value of a edge
  inline bool getEdgeValue(edge e) const {
    return (*this)[graph->edgePos(e)];
  }

  // set the stored value of a edge
  inline void setEdgeValue(edge e, bool val) {
    (*this)[graph->edgePos(e)] = val;
  }

  // set all to same values
  void setAll(const bool &val) {
    TLP_PARALLEL_MAP_INDICES(graph->numberOfEdges(), [&](unsigned int i) { (*this)[i] = val; });
  }

  // add a value for a newly created edge
  void addEdgeValue(edge e, bool val) {
    unsigned int ePos = graph->edgePos(e);

    if (ePos + 1 > this->size())
      this->resize(ePos + 1);

    (*this)[ePos] = val;
  }

  // get values from a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyFromProperty(PROP_PTR prop) {
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(
        graph, [&](const edge e, unsigned int i) { (*this)[i] = prop->getEdgeValue(e); });
  }

  // copy values into a typed instance of PropertyInterface
  template <typename PROP_PTR>
  void copyToProperty(PROP_PTR prop) {
    const std::vector<edge> &edges = graph->edges();
    unsigned int nbEdges = edges.size();

    for (unsigned int i = 0; i < nbEdges; ++i)
      prop->setEdgeValue(edges[i], (*this)[i]);
  }
};
} // namespace tlp

#endif
