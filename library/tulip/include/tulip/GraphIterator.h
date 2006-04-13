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
#include "Iterator.h"
#include "SuperGraph.h"
#include "SuperGraphImpl.h"
#include <set>
#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include "tulipconf.h"


class Selection;
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
  SuperGraph *_parentGraph;
  const MutableContainer<bool>& _filter;
 public:
  FactorIterator(const SuperGraph *sG,const MutableContainer<bool>& filter):
    _parentGraph(sG->getFather()),
    _filter(filter)
  {}
};
//============================================================
///Node iterator for SuperGraphView
class SGraphNodeIterator:public FactorIterator<node> {
 private:
  Iterator<node> *it;
  node curNode;
  bool _hasnext;

 public:
  SGraphNodeIterator(const SuperGraph *sG, const MutableContainer<bool>& filter);
  ~SGraphNodeIterator();
  node next();
  bool hasNext();
};
//============================================================
///Out node iterator for SuperGraphView
class OutNodesIterator:public FactorIterator<node> {
 private:
  Iterator<edge> *it;
  #ifndef NDEBUG
  const SuperGraph *graph;
  #endif
 public:
  OutNodesIterator(const SuperGraph *sG, const MutableContainer<bool>& filter, node n);
  ~OutNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In node iterator for SuperGraphView
class InNodesIterator:public FactorIterator<node> {
 private:
  Iterator<edge> *it;
  #ifndef NDEBUG
  const SuperGraph *graph;
  #endif
 public:
  InNodesIterator(const SuperGraph *sG, const MutableContainer<bool>& filter ,node n);
  ~InNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In Out node iterator for SuperGraphView
class InOutNodesIterator:public FactorIterator<node> {
 private:
  Iterator<edge> *it;
  node n;
  #ifndef NDEBUG
  const SuperGraph *graph;
  #endif
 public:
  InOutNodesIterator(const SuperGraph *sG,const MutableContainer<bool>& filter,node n);
  ~InOutNodesIterator();
  node next();
  bool hasNext();
};
//=============================================================
///Edge iterator for SuperGraphView
class SGraphEdgeIterator:public FactorIterator<edge>
{
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;

 public:
  SGraphEdgeIterator(const SuperGraph *sG, const MutableContainer<bool>& filter);
  ~SGraphEdgeIterator();
  edge next();
  bool hasNext();
};
//============================================================
///Out edge iterator for SuperGraphView
class OutEdgesIterator:public FactorIterator<edge> {
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;

 public:
  OutEdgesIterator(const SuperGraph *sG, const MutableContainer<bool>& filter, node n);
  ~OutEdgesIterator();
  edge next();
  bool hasNext();
};
//============================================================
///In edge iterator for SuperGraphView
class InEdgesIterator:public FactorIterator<edge> {
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;

 public:
  InEdgesIterator(const SuperGraph *sG,const MutableContainer<bool>& filter,node n);
  ~InEdgesIterator();
  edge next();
  bool hasNext();
};
//============================================================
///In Out edge iterator for SuperGraphView
class InOutEdgesIterator:public FactorIterator<edge> {
 private:
  Iterator<edge> *it;
  edge curEdge;
  bool _hasnext;

 public:
  InOutEdgesIterator(const SuperGraph *sG, const MutableContainer<bool>& filter, node n);
  ~InOutEdgesIterator();
  edge next();
  bool hasNext();
};



//============================================================
//Iterator for the SuperGraphImpl
//============================================================
///Node iterator for data graph
class xSGraphNodeIterator:public Iterator<node> {
 private:
  Iterator<unsigned int> *itId;
 public:
  xSGraphNodeIterator(const SuperGraph *sG);
  ~xSGraphNodeIterator();
  node next();
  bool hasNext();
};
//============================================================
///Out Node iterator for data graph
class xOutNodesIterator:public Iterator<node> {
 private:
  Iterator<edge> *it;
  SuperGraphImpl *spG;
 public:
  xOutNodesIterator(const SuperGraph *sG,const node n);
  ~xOutNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In Node iterator for data graph
class xInNodesIterator:public Iterator<node> {
 private:
  Iterator<edge> *it;
  const SuperGraphImpl *spG;
 public:
  xInNodesIterator(const SuperGraph *sG,const node n);
  ~xInNodesIterator();
  node next();
  bool hasNext();
};
//============================================================
///In Out Node iterator for data graph
class xInOutNodesIterator:public Iterator<node> {
  SuperGraphImpl::EdgeContainer::iterator it,itEnd;
  node n;
  const SuperGraphImpl *spG;
 public:
  xInOutNodesIterator(const SuperGraph *sG,const node n);
  ~xInOutNodesIterator();
  node next();
  bool hasNext();
};
//=============================================================
///Edge iterator for data graph
class xSGraphEdgeIterator:public Iterator<edge> {
 private:
  Iterator<unsigned int> *itId;
 public:
  xSGraphEdgeIterator(const SuperGraph *sG);
  ~xSGraphEdgeIterator();
  edge next();
  bool hasNext();
};
//============================================================
///Out edge iterator for data graph
class xOutEdgesIterator:public Iterator<edge> {
 private:
  SuperGraphImpl::EdgeContainer::iterator it,itEnd;
  node n;
  edge curEdge;
  SuperGraphImpl *spG;
  std::set<edge> loop;
 public:
  xOutEdgesIterator(const SuperGraph *sG,const node n);
  ~xOutEdgesIterator();
  edge next();
  bool hasNext();
};
//============================================================
///In edge iterator for data graph
class xInEdgesIterator:public Iterator<edge> {
  SuperGraphImpl::EdgeContainer::iterator it,itEnd;
  node n;
  edge curEdge;
  SuperGraphImpl *spG;
  std::set<edge> loop;
 public:
  xInEdgesIterator(const SuperGraph *sG,const node n);
  ~xInEdgesIterator();
  edge next();
  bool hasNext();
};
//============================================================
///In out edge iterator for data graph
class xInOutEdgesIterator:public Iterator<edge> {
  SuperGraphImpl::EdgeContainer::iterator it,itEnd;
public:
  xInOutEdgesIterator(const SuperGraph *sG,const node n);
  ~xInOutEdgesIterator();
  edge next();
  bool hasNext();
};
//=============================================================
#endif // DOXYGEN_NOTFOR_DEVEL
#endif








