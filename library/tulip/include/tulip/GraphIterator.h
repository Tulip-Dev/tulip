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
#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include "tulip/tulipconf.h"

namespace tlp {

//template<class C>class Iterator;
#ifndef DOXYGEN_NOTFOR_DEVEL
template<typename TYPE> class UINTIterator : public Iterator<TYPE> {
public:
  UINTIterator(Iterator<unsigned int> *it):it(it) {
  }
  ~UINTIterator(){
    delete it;
  }
  bool hasNext() {
    return it->hasNext();
  }
  TYPE next() {
    return TYPE(it->next());
  }
private:
  Iterator<unsigned int> *it;
};

//===========================================================
///Factorization of code for iterators
template<class itType> class FactorIterator:public Iterator<itType> {
 protected:
  Graph *_parentGraph;
  const MutableContainer<bool>& _filter;
 public:
  FactorIterator(const Graph *sG,const MutableContainer<bool>& filter):
    _parentGraph(sG->getSuperGraph()),
    _filter(filter)
  {}
};
//============================================================
///Node iterator for GraphView
class SGraphNodeIterator:public FactorIterator<node> {
 private:
  Iterator<node> *it;
  node curNode;
  bool _hasnext;

 public:
  SGraphNodeIterator(const Graph *sG, const MutableContainer<bool>& filter);
  ~SGraphNodeIterator();
  node next();
  bool hasNext();
};
//============================================================
///Out node iterator for GraphView
class OutNodesIterator:public FactorIterator<node> {
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
class InNodesIterator:public FactorIterator<node> {
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
class InOutNodesIterator:public FactorIterator<node> {
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
class SGraphEdgeIterator:public FactorIterator<edge>
{
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;

 public:
  SGraphEdgeIterator(const Graph *sG, const MutableContainer<bool>& filter);
  ~SGraphEdgeIterator();
  edge next();
  bool hasNext();
};
//============================================================
///Out edge iterator for GraphView
class OutEdgesIterator:public FactorIterator<edge> {
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
class InEdgesIterator:public FactorIterator<edge> {
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
class InOutEdgesIterator:public FactorIterator<edge> {
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
class xSGraphNodeIterator:public Iterator<node> {
 private:
  Iterator<unsigned int> *itId;
 public:
  xSGraphNodeIterator(const Graph *sG);
  ~xSGraphNodeIterator();
  node next();
  bool hasNext();
};
//============================================================
///Out Node iterator for data sg
class xOutNodesIterator:public Iterator<node> {
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
class xInNodesIterator:public Iterator<node> {
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
class xInOutNodesIterator:public Iterator<node> {
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
class xSGraphEdgeIterator:public Iterator<edge> {
 private:
  Iterator<unsigned int> *itId;
 public:
  xSGraphEdgeIterator(const Graph *sG);
  ~xSGraphEdgeIterator();
  edge next();
  bool hasNext();
};
//============================================================
///Out edge iterator for data sg
class xOutEdgesIterator:public Iterator<edge> {
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
class xInEdgesIterator:public Iterator<edge> {
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
class xInOutEdgesIterator:public Iterator<edge> {
  GraphImpl::EdgeContainer::iterator it,itEnd;
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
