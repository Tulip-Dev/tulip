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
///@cond DOXYGEN_HIDDEN

#ifndef TULIP_GGRAPHITERATOR_H
#define TULIP_GGRAPHITERATOR_H
#include <tulip/Iterator.h>
#include <tulip/memorypool.h>
#include <set>

#include <tulip/MutableContainer.h>
#include <tulip/Observable.h>
#include <tulip/tulipconf.h>
#include <tulip/StoredType.h>
#include <iostream>

namespace tlp {

class Graph;
class GraphImpl;
class GraphView;

struct node;
struct edge;
class TLP_SCOPE NodeIterator : public Iterator<node> {};

class TLP_SCOPE EdgeIterator : public Iterator<edge> {};

#if !defined(NDEBUG) && !defined(_OPENMP)
class TLP_SCOPE NodeIteratorObserver : public NodeIterator, public Observable {
private:
  // Observable interface
  void treatEvent(const Event &);
};

class TLP_SCOPE EdgeIteratorObserver : public EdgeIterator, public Observable {
private:
  // Observable interface
  void treatEvent(const Event &);
};
#endif
//===========================================================
/// Factorization of code for iterators
class TLP_SCOPE FactorNodeIterator
#if defined(NDEBUG) || defined(_OPENMP)
    : public NodeIterator
#else
    : public NodeIteratorObserver
#endif
{
protected:
  Graph *_parentGraph;

public:
  FactorNodeIterator(const Graph *sG) : _parentGraph(sG->getSuperGraph()) {}
};

class TLP_SCOPE FactorEdgeIterator
#if defined(NDEBUG) || defined(_OPENMP)
    : public EdgeIterator
#else
    : public EdgeIteratorObserver
#endif
{
protected:
  Graph *_parentGraph;

public:
  FactorEdgeIterator(const Graph *sG) : _parentGraph(sG->getSuperGraph()) {}
};
//============================================================
/// Node iterator for GraphView
template <typename VALUE_TYPE>
class SGraphNodeIterator : public FactorNodeIterator,
                           public MemoryPool<SGraphNodeIterator<VALUE_TYPE>> {
private:
  const Graph *sg;
  Iterator<node> *it;
  node curNode;
  VALUE_TYPE value;
  const MutableContainer<VALUE_TYPE> &_filter;

protected:
  void prepareNext() {
    while (it->hasNext()) {
      curNode = it->next();

      if (_filter.get(curNode) == value)
        return;
    }

    // set curNode as invalid
    curNode = node();
  }

public:
  SGraphNodeIterator(const Graph *sG, const MutableContainer<VALUE_TYPE> &filter,
                     typename tlp::StoredType<VALUE_TYPE>::ReturnedConstValue val)
      : FactorNodeIterator(sG), sg(sG), value(val), _filter(filter) {
    it = sg->getNodes();
#if !defined(NDEBUG) && !defined(_OPENMP)
    sg->addListener(this);
#endif
    // anticipate first iteration
    prepareNext();
  }
  ~SGraphNodeIterator() override {
#if !defined(NDEBUG) && !defined(_OPENMP)
    sg->removeListener(this);
#endif
    delete it;
  }
  node next() override {
    assert(curNode.isValid());
    // we are already pointing to the next
    node tmp = curNode;
    // anticipate next iteration
    prepareNext();
    return tmp;
  }

  bool hasNext() override {
    return (curNode.isValid());
  }
};

//============================================================
/// Out node iterator for GraphView
class TLP_SCOPE OutNodesIterator : public FactorNodeIterator, public MemoryPool<OutNodesIterator> {
private:
  Iterator<edge> *it;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  OutNodesIterator(const GraphView *sG, node n);
  ~OutNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// In node iterator for GraphView
class InNodesIterator : public FactorNodeIterator, public MemoryPool<InNodesIterator> {
private:
  Iterator<edge> *it;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  InNodesIterator(const GraphView *sG, node n);
  ~InNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//============================================================
/// In Out node iterator for GraphView
class TLP_SCOPE InOutNodesIterator : public FactorNodeIterator,
                                     public MemoryPool<InOutNodesIterator> {
private:
  Iterator<edge> *it;
  node n;
#if !defined(NDEBUG) && !defined(_OPENMP)
  const GraphView *sg;
#endif
public:
  InOutNodesIterator(const GraphView *sG, node n);
  ~InOutNodesIterator() override;
  node next() override;
  bool hasNext() override;
};
//=============================================================
/// Edge iterator for GraphView
template <typename VALUE_TYPE>
class SGraphEdgeIterator : public FactorEdgeIterator,
                           public MemoryPool<SGraphEdgeIterator<VALUE_TYPE>> {
private:
  const Graph *sg;
  Iterator<edge> *it;
  edge curEdge;
  VALUE_TYPE value;
  const MutableContainer<VALUE_TYPE> &_filter;

protected:
  void prepareNext() {
    while (it->hasNext()) {
      curEdge = it->next();

      if (_filter.get(curEdge.id) == value)
        return;
    }

    // set curEdge as invalid
    curEdge = edge();
  }

public:
  SGraphEdgeIterator(const Graph *sG, const MutableContainer<VALUE_TYPE> &filter,
                     typename tlp::StoredType<VALUE_TYPE>::ReturnedConstValue val)
      : FactorEdgeIterator(sG), sg(sG), value(val), _filter(filter) {
    it = sg->getEdges();
#if !defined(NDEBUG) && !defined(_OPENMP)
    sg->addListener(this);
#endif
    // anticipate first iteration
    prepareNext();
  }

  ~SGraphEdgeIterator() override {
#if !defined(NDEBUG) && !defined(_OPENMP)
    sg->removeListener(this);
#endif
    delete it;
  }

  edge next() override {
    assert(curEdge.isValid());
    // we are already pointing to the next
    edge tmp = curEdge;
    // anticipating the next iteration
    prepareNext();
    return tmp;
  }

  bool hasNext() override {
    return (curEdge.isValid());
  }
};
//============================================================
/// Out edge iterator for GraphView
class TLP_SCOPE OutEdgesIterator : public FactorEdgeIterator, public MemoryPool<OutEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  OutEdgesIterator(const GraphView *sG, node n);
  ~OutEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
//============================================================
/// In edge iterator for GraphView
class TLP_SCOPE InEdgesIterator : public FactorEdgeIterator, public MemoryPool<InEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  InEdgesIterator(const GraphView *sG, node n);
  ~InEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};
//============================================================
/// In Out edge iterator for GraphView
class TLP_SCOPE InOutEdgesIterator : public FactorEdgeIterator,
                                     public MemoryPool<InOutEdgesIterator> {
private:
  Iterator<edge> *it;
  edge curEdge;
  const GraphView *sg;

public:
  InOutEdgesIterator(const GraphView *sG, node n);
  ~InOutEdgesIterator() override;
  edge next() override;
  bool hasNext() override;

protected:
  void prepareNext();
};

//============================================================
// Iterator for the Graph
//============================================================
class TLP_SCOPE GraphNodeIterator
#if defined(NDEBUG) || defined(_OPENMP)
    : public NodeIterator,
      public MemoryPool<GraphNodeIterator>
#else
    : public NodeIteratorObserver
#endif
{
private:
#if !defined(NDEBUG) && !defined(_OPENMP)
  const Graph *graph;
#endif
  Iterator<node> *itId;

public:
  GraphNodeIterator(const Graph *g, Iterator<node> *it);
  ~GraphNodeIterator() override;
  node next() override;
  bool hasNext() override;
};
//=============================================================
/// Edge iterator for data sg
class TLP_SCOPE GraphEdgeIterator
#if defined(NDEBUG) || defined(_OPENMP)
    : public EdgeIterator,
      public MemoryPool<GraphEdgeIterator>
#else
    : public EdgeIteratorObserver
#endif
{
private:
#if !defined(NDEBUG) && !defined(_OPENMP)
  const Graph *graph;
#endif
  Iterator<edge> *itId;

public:
  GraphEdgeIterator(const Graph *g, Iterator<edge> *it);
  ~GraphEdgeIterator() override;
  edge next() override;
  bool hasNext() override;
};
//============================================================
}
#endif

///@endcond
