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
#ifndef TULIP_GGRAPHITERATOR_H
#define TULIP_GGRAPHITERATOR_H
#include "tulip/Iterator.h"
#include "tulip/Graph.h"
#include "tulip/GraphImpl.h"
#include "tulip/memorypool.h"
#include <set>

#include "tulip/tulipconf.h"
#include <iostream>

#ifndef DOXYGEN_NOTFOR_DEVEL
namespace tlp {

class NodeIterator :public Iterator<node> {
};

class EdgeIterator :public Iterator<edge> {
};

#ifndef NDEBUG
class NodeIteratorObserver :public NodeIterator, public GraphObserver {
public:
  // GraphObserver interface
  void addNode(Graph* g, node n);
  void delNode(Graph* g, node n);
};

class EdgeIteratorObserver :public EdgeIterator, public GraphObserver {
public:
  // GraphObserver interface
  void addEdge(Graph* g, edge e);
  void delEdge(Graph* g, edge e);
};
#endif
//===========================================================
///Factorization of code for iterators
class FactorNodeIterator
#ifdef NDEBUG
  :public NodeIterator
#else
  :public NodeIteratorObserver
#endif
 {
 protected:
  Graph *_parentGraph;
  const MutableContainer<bool>& _filter;
 public:
  FactorNodeIterator(const Graph *sG,const MutableContainer<bool>& filter):
    _parentGraph(sG->getSuperGraph()),
    _filter(filter)
  {}
};

class FactorEdgeIterator
#ifdef NDEBUG
  :public EdgeIterator
#else
  :public EdgeIteratorObserver
#endif
 {
 protected:
  Graph *_parentGraph;
  const MutableContainer<bool>& _filter;
 public:
  FactorEdgeIterator(const Graph *sG,const MutableContainer<bool>& filter):
    _parentGraph(sG->getSuperGraph()),
    _filter(filter)
  {}
};
//============================================================
///Node iterator for GraphView
class SGraphNodeIterator:public FactorNodeIterator, public MemoryPool<SGraphNodeIterator> {
 private:
  const Graph* sg;
  Iterator<node> *it;
  node curNode;
  bool value;

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
  #ifndef NDEBUG
  const Graph *sg;
  #endif
 public:
  OutNodesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n);
  ~OutNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In node iterator for GraphView
class InNodesIterator:public FactorNodeIterator, public MemoryPool<InNodesIterator> {
 private:
  Iterator<edge> *it;
  #ifndef NDEBUG
  const Graph *sg;
  #endif
 public:
  InNodesIterator(const Graph *sG, const MutableContainer<bool>& filter ,node n);
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
  #ifndef NDEBUG
  const Graph *sg;
  #endif
 public:
  InOutNodesIterator(const Graph *sG,const MutableContainer<bool>& filter,node n);
  ~InOutNodesIterator();
  node next();
  bool hasNext();
};
//=============================================================
///Edge iterator for GraphView
class SGraphEdgeIterator:public FactorEdgeIterator, public MemoryPool<SGraphEdgeIterator>
{
 private:
  const Graph* sg;
  Iterator<edge> *it;
  edge curEdge;
  bool value;

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

 public:
  OutEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n);
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

 public:
  InEdgesIterator(const Graph *sG,const MutableContainer<bool>& filter,node n);
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

 public:
  InOutEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n);
  ~InOutEdgesIterator();
  edge next();
  bool hasNext();
  protected:
  void prepareNext();
};



//============================================================
//Iterator for the GraphImpl
//============================================================
///Node iterator for data sg
class xSGraphNodeIterator
#ifdef NDEBUG
  :public NodeIterator, public MemoryPool<xSGraphNodeIterator>
#else
  :public NodeIteratorObserver
#endif
 {
 private:
#ifndef NDEBUG
  GraphImpl *spG;
#endif
  Iterator<unsigned int> *itId;
 public:
  xSGraphNodeIterator(const Graph *sG);
  ~xSGraphNodeIterator();
  node next();
  bool hasNext();
};
//============================================================
///Out Node iterator for data sg
class xOutNodesIterator
#ifdef NDEBUG
  :public NodeIterator, public MemoryPool<xOutNodesIterator>
#else
  :public NodeIteratorObserver
#endif
 {
 private:
  Iterator<edge> *it;
  GraphImpl *spG;
 public:
  xOutNodesIterator(const Graph *sG,const node n);
  ~xOutNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In Node iterator for data sg
class xInNodesIterator
#ifdef NDEBUG
  :public NodeIterator, public MemoryPool<xInNodesIterator>
#else
  :public NodeIteratorObserver
#endif
 {
 private:
  Iterator<edge> *it;
  const GraphImpl *spG;
 public:
  xInNodesIterator(const Graph *sG,const node n);
  ~xInNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In Out Node iterator for data sg
class xInOutNodesIterator
#ifdef NDEBUG
  :public NodeIterator, public MemoryPool<xInOutNodesIterator>
#else
  :public NodeIteratorObserver
#endif
 {
  GraphImpl::EdgeContainer::iterator it,itEnd;
  node n;
  const GraphImpl *spG;
 public:
  xInOutNodesIterator(const Graph *sG,const node n);
  ~xInOutNodesIterator();
  node next();
  bool hasNext();
};
//=============================================================
///Edge iterator for data sg
class xSGraphEdgeIterator
#ifdef NDEBUG
  :public EdgeIterator, public MemoryPool<xSGraphEdgeIterator>
#else
  :public EdgeIteratorObserver
#endif
 {
 private:
#ifndef NDEBUG
  GraphImpl *spG;
#endif
  Iterator<unsigned int> *itId;
 public:
  xSGraphEdgeIterator(const Graph *sG);
  ~xSGraphEdgeIterator();
  edge next();
  bool hasNext();
};
//============================================================
///Out edge iterator for data sg
class xOutEdgesIterator
#ifdef NDEBUG
  :public EdgeIterator, public MemoryPool<xOutEdgesIterator>
#else
  :public EdgeIteratorObserver
#endif
 {
 private:
  GraphImpl::EdgeContainer::iterator it,itEnd;
  node n;
  edge curEdge;
  GraphImpl *spG;
  std::set<edge> loop;
 public:
  xOutEdgesIterator(const Graph *sG,const node n);
  ~xOutEdgesIterator();
  edge next();
  bool hasNext();
 protected:
  void prepareNext();
};
//============================================================
///In edge iterator for data sg
class xInEdgesIterator
#ifdef NDEBUG
  :public EdgeIterator, public MemoryPool<xInEdgesIterator>
#else
  :public EdgeIteratorObserver
#endif
 {
  GraphImpl::EdgeContainer::iterator it,itEnd;
  node n;
  edge curEdge;
  GraphImpl *spG;
  std::set<edge> loop;
 public:
  xInEdgesIterator(const Graph *sG,const node n);
  ~xInEdgesIterator();
  edge next();
  bool hasNext();
 protected:
  void prepareNext();
};
//============================================================
///In out edge iterator for data sg
class xInOutEdgesIterator
#ifdef NDEBUG
  :public EdgeIterator, public MemoryPool<xInOutEdgesIterator>
#else
  :public EdgeIteratorObserver
#endif
 {
  GraphImpl::EdgeContainer::iterator it,itEnd;
#ifndef NDEBUG
private:
  GraphImpl *spG;
#endif
public:
  xInOutEdgesIterator(const Graph *sG,const node n);
  ~xInOutEdgesIterator();
  edge next();
  bool hasNext();
};
//============================================================

}
#endif // DOXYGEN_NOTFOR_DEVEL
#endif
