//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef TULIP_GGRAPHITERATOR_H
#define TULIP_GGRAPHITERATOR_H
#include "tulip/Iterator.h"
#include "tulip/Graph.h"
#include "tulip/GraphImpl.h"
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
  bool nodeIteration;
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
class SGraphNodeIterator:public FactorNodeIterator {
 private:
  Iterator<node> *it;
  node curNode;
  bool _hasnext;
  bool value;

 public:
  SGraphNodeIterator(const Graph *sG, const MutableContainer<bool>& filter,
		     bool value=true);
  ~SGraphNodeIterator();
  node next();
  bool hasNext();
};
//============================================================
///Out node iterator for GraphView
class OutNodesIterator:public FactorNodeIterator {
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
class InNodesIterator:public FactorNodeIterator {
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
class InOutNodesIterator:public FactorNodeIterator {
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
class SGraphEdgeIterator:public FactorEdgeIterator
{
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;
  bool value;

 public:
  SGraphEdgeIterator(const Graph *sG, const MutableContainer<bool>& filter,
		     bool value = true);
  ~SGraphEdgeIterator();
  edge next();
  bool hasNext();
};
//============================================================
///Out edge iterator for GraphView
class OutEdgesIterator:public FactorEdgeIterator {
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;

 public:
  OutEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n);
  ~OutEdgesIterator();
  edge next();
  bool hasNext();
};
//============================================================
///In edge iterator for GraphView
class InEdgesIterator:public FactorEdgeIterator {
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;

 public:
  InEdgesIterator(const Graph *sG,const MutableContainer<bool>& filter,node n);
  ~InEdgesIterator();
  edge next();
  bool hasNext();
};
//============================================================
///In Out edge iterator for GraphView
class InOutEdgesIterator:public FactorEdgeIterator {
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;

 public:
  InOutEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n);
  ~InOutEdgesIterator();
  edge next();
  bool hasNext();
};



//============================================================
//Iterator for the GraphImpl
//============================================================
///Node iterator for data sg
class xSGraphNodeIterator
#ifdef NDEBUG
  :public NodeIterator
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
  :public NodeIterator
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
  :public NodeIterator
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
  :public NodeIterator
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
  :public EdgeIterator
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
  :public EdgeIterator
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
};
//============================================================
///In edge iterator for data sg
class xInEdgesIterator
#ifdef NDEBUG
  :public EdgeIterator
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
};
//============================================================
///In out edge iterator for data sg
class xInOutEdgesIterator
#ifdef NDEBUG
  :public EdgeIterator
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
