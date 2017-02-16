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

#ifndef TULIP_GGRAPHITERATOR_H
#define TULIP_GGRAPHITERATOR_H
#include <tulip/Iterator.h>
#include <tulip/memorypool.h>
#include <set>

#include <tulip/MutableContainer.h>
#include <tulip/Observable.h>
#include <tulip/tulipconf.h>
#include <iostream>

namespace tlp {

class Graph;
class GraphImpl;
class GraphView;
 
struct node;
struct edge;
class NodeIterator :public Iterator<node> {
};

class EdgeIterator :public Iterator<edge> {
};

#if !defined(NDEBUG) && !defined(_OPENMP)
class NodeIteratorObserver :public NodeIterator, public Observable {
private:
  // Observable interface
  void treatEvent(const Event&);
};

class EdgeIteratorObserver :public EdgeIterator, public Observable {
private:
  // Observable interface
  void treatEvent(const Event&);
};
#endif
//===========================================================
///Factorization of code for iterators
class FactorNodeIterator
#if defined(NDEBUG) || defined(_OPENMP)
  :public NodeIterator
#else
  :public NodeIteratorObserver
#endif
{
protected:
  Graph *_parentGraph;
public:
  FactorNodeIterator(const Graph *sG):
    _parentGraph(sG->getSuperGraph()) {
  }
};

class FactorEdgeIterator
#if defined(NDEBUG) || defined(_OPENMP)
  :public EdgeIterator
#else
  :public EdgeIteratorObserver
#endif
{
protected:
  Graph *_parentGraph;
public:
  FactorEdgeIterator(const Graph *sG):
    _parentGraph(sG->getSuperGraph()) {
  }
};
//============================================================
///Node iterator for GraphView
class SGraphNodeIterator:public FactorNodeIterator, public MemoryPool<SGraphNodeIterator> {
private:
  const Graph* sg;
  Iterator<node> *it;
  node curNode;
  bool value;
  const MutableContainer<bool>& _filter;                     

public:
  SGraphNodeIterator(const Graph *sG, const MutableContainer<bool>& filter,
                     bool value=true);
  ~SGraphNodeIterator();
  node next();
  bool hasNext();
protected:
  void prepareNext();
};
//============================================================
///Out node iterator for GraphView
class OutNodesIterator:public FactorNodeIterator, public MemoryPool<OutNodesIterator> {
private:
  Iterator<edge> *it;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  OutNodesIterator(const GraphView *sG, node n);
  ~OutNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In node iterator for GraphView
class InNodesIterator:public FactorNodeIterator, public MemoryPool<InNodesIterator> {
private:
  Iterator<edge> *it;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  InNodesIterator(const GraphView *sG, node n);
  ~InNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In Out node iterator for GraphView
class InOutNodesIterator:public FactorNodeIterator, public MemoryPool<InOutNodesIterator> {
private:
  Iterator<edge> *it;
  node n;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  InOutNodesIterator(const GraphView *sG, node n);
  ~InOutNodesIterator();
  node next();
  bool hasNext();
};
//=============================================================
///Edge iterator for GraphView
class SGraphEdgeIterator:public FactorEdgeIterator, public MemoryPool<SGraphEdgeIterator> {
private:
  const Graph* sg;
  Iterator<edge> *it;
  edge curEdge;
  bool value;
  const MutableContainer<bool>& _filter;

public:
  SGraphEdgeIterator(const Graph *sG, const MutableContainer<bool>& filter,
                     bool value = true);
 ~SGraphEdgeIterator();
  edge next();
  bool hasNext();
protected:
  void prepareNext();
};
//============================================================
///Out edge iterator for GraphView
class OutEdgesIterator:public FactorEdgeIterator, public MemoryPool<OutEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView* sg;

public:
  OutEdgesIterator(const GraphView *sG, node n);
  ~OutEdgesIterator();
  edge next();
  bool hasNext();
protected:
  void prepareNext();
};
//============================================================
///In edge iterator for GraphView
class InEdgesIterator:public FactorEdgeIterator, public MemoryPool<InEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView* sg;

public:
  InEdgesIterator(const GraphView *sG, node n);
  ~InEdgesIterator();
  edge next();
  bool hasNext();
protected:
  void prepareNext();
};
//============================================================
///In Out edge iterator for GraphView
class InOutEdgesIterator:public FactorEdgeIterator, public MemoryPool<InOutEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView* sg;
  
public:
  InOutEdgesIterator(const GraphView *sG, node n);
  ~InOutEdgesIterator();
  edge next();
  bool hasNext();
protected:
  void prepareNext();
};

//============================================================
//Iterator for the Graph
//============================================================
class GraphNodeIterator
#if defined(NDEBUG) || defined(_OPENMP)
  :public NodeIterator, public MemoryPool<GraphNodeIterator>
#else
  :public NodeIteratorObserver
#endif
{
private:
#if !defined(NDEBUG) && !defined(_OPENMP)
  const Graph *graph;
#endif
  Iterator<node> *itId;
public:
  GraphNodeIterator(const Graph *g, Iterator<node>* it);
  ~GraphNodeIterator();
  node next();
  bool hasNext();
};
//=============================================================
///Edge iterator for data sg
class GraphEdgeIterator
#if defined(NDEBUG) || defined(_OPENMP)
  :public EdgeIterator, public MemoryPool<GraphEdgeIterator>
#else
  :public EdgeIteratorObserver
#endif
{
private:
#if !defined(NDEBUG) && !defined(_OPENMP)
  const Graph *graph;
#endif
  Iterator<edge> *itId;
public:
  GraphEdgeIterator(const Graph *g, Iterator<edge>* it);
  ~GraphEdgeIterator();
  edge next();
  bool hasNext();
};
//============================================================

}
#endif

///@endcond
