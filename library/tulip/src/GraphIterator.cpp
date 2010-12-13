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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include "tulip/GraphIterator.h"
#include "tulip/BooleanProperty.h"

namespace tlp {

#ifndef NDEBUG
static int NumIterators = 0;

void incrNumIterators() {
  NumIterators++;
}

void decrNumIterators() {
  NumIterators--;
}

void NodeIteratorObserver::addNode(Graph*, node) {
  if (hasNext())
    std::cerr << "Warning: node added while iterating !!!" << std::endl;
}

void NodeIteratorObserver::delNode(Graph*, node) {
  if (hasNext())
    std::cerr << "Warning: node deleted while iterating !!!" << std::endl;
}
   
void EdgeIteratorObserver::addEdge(Graph*, edge) {
  if (hasNext())
    std::cerr << "Warning: edge added while iterating !!!" << std::endl;
}

void EdgeIteratorObserver::delEdge(Graph*, edge) {
  if (hasNext())
    std::cerr << "Warning: edge deleted while iterating !!!" << std::endl;
}

int getNumIterators() {
  return NumIterators;
}

#else

void incrNumIterators() {
}

void decrNumIterators() {
}

int getNumIterators() {
  return 0;
}

#endif


//============================================================
SGraphNodeIterator::SGraphNodeIterator(const Graph *sG, const MutableContainer<bool>& filter, bool val)
 :FactorNodeIterator(sG,filter), sg(sG), _hasnext(true), value(val) {
  it=_parentGraph->getNodes();
#ifndef NDEBUG
#ifdef _OPENMP
  // iterator creation is set as critical because they may be used
  // in parallel loops and there is an implicit GraphObserver addition
  // which results in concurrent updates of the GraphObserver::observers
  // data member which are not thread safe
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  next();
}
SGraphNodeIterator::~SGraphNodeIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // same as iterator creation
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
node SGraphNodeIterator::next() {
  assert(_hasnext);
  // we are already pointing to the next
  node tmp=curNode;
  // anticipate next iteration
  if ((_hasnext = it->hasNext())) {
    curNode=it->next();
    while (!(_hasnext = !(_filter.get(curNode.id)!= value ||
			  !sg->isElement(curNode)))) {
      if (!it->hasNext()) break;
      curNode=it->next();
    }
  }
  return tmp;
}
bool SGraphNodeIterator::hasNext(){ 
  return (_hasnext);
}
//===================================================================
OutNodesIterator::OutNodesIterator(const Graph *sG, const MutableContainer<bool>& filter,node n):FactorNodeIterator(sG,filter) {
  it = new OutEdgesIterator(sG,filter,n);
#ifndef NDEBUG
  sg = sG;
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
}
OutNodesIterator::~OutNodesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
node OutNodesIterator::next() {
#ifndef NDEBUG
  assert(it->hasNext());
  node tmp = _parentGraph->target(it->next());
  assert(sg->isElement(tmp));
  return tmp;
#endif
  return _parentGraph->target(it->next());
}
bool OutNodesIterator::hasNext() {
  return (it->hasNext());
}
//===================================================================
InNodesIterator::InNodesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n):FactorNodeIterator(sG,filter),
										   it(new InEdgesIterator(sG,filter,n)) {
#ifndef NDEBUG
  sg = sG;
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
}
InNodesIterator::~InNodesIterator() { 
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->removeGraphObserver(this);
#endif
  delete it; 
}
node InNodesIterator:: next() {
#ifndef NDEBUG
  assert(it->hasNext());
  node tmp = _parentGraph->source(it->next());
  assert(sg->isElement(tmp));
  return tmp;
#endif
  return _parentGraph->source(it->next());
}
bool InNodesIterator::hasNext() {
  return (it->hasNext());
}
//===================================================================
InOutNodesIterator::InOutNodesIterator(const Graph *sG, const MutableContainer<bool>& filter,node n):FactorNodeIterator(sG,filter),
											 it(new InOutEdgesIterator(sG,filter,n)),
											 n(n) {
#ifndef NDEBUG
  sg = sG;
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
}
InOutNodesIterator::~InOutNodesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
node InOutNodesIterator::next() {
#ifndef NDEBUG
  assert(it->hasNext());
  node tmp = _parentGraph->opposite(it->next(),n);
  assert(sg->isElement(tmp));
  return tmp;
#else
  return _parentGraph->opposite(it->next(),n);
#endif
}
bool InOutNodesIterator::hasNext() {
  return (it->hasNext());
}
//===============================================================
SGraphEdgeIterator::SGraphEdgeIterator(const Graph *sG, const MutableContainer<bool>& filter, bool val):FactorEdgeIterator(sG,filter), sg(sG), _hasnext(true), value(val) {
  it=_parentGraph->getEdges();
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  // _hasnext is initially set to true to avoid failure when asserting
  // in the first call of next() below
  next();
}
SGraphEdgeIterator::~SGraphEdgeIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
edge SGraphEdgeIterator::next() {
  assert(_hasnext);
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  if ((_hasnext = it->hasNext())){
    curEdge=it->next();
    while (!(_hasnext = !(_filter.get(curEdge.id)!= value ||
			  !sg->isElement(curEdge))) &&
	   it->hasNext()) {
      curEdge=it->next();
    }
  }
  return tmp;
}
bool SGraphEdgeIterator::hasNext() {
  return (_hasnext);
}
//===================================================================
OutEdgesIterator::OutEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter,node n):FactorEdgeIterator(sG,filter), _hasnext(true) {
  assert(sG->isElement(n));
  it=_parentGraph->getOutEdges(n);
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  // _hasnext is initially set to true to avoid failure when asserting
  // in the first call of next() below
  next();
}
OutEdgesIterator::~OutEdgesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
edge OutEdgesIterator::next() {
  assert(_hasnext);
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  if ((_hasnext = it->hasNext())) {
      curEdge=it->next();
      while (!(_hasnext = _filter.get(curEdge.id)) &&
	     it->hasNext()) {
	  if (!it->hasNext()) break;
	  curEdge=it->next();
	}
    }
  return tmp;
}
bool OutEdgesIterator::hasNext() {
  return (_hasnext);
}
//===================================================================
  InEdgesIterator::InEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n):FactorEdgeIterator(sG,filter), _hasnext(true) {
  assert(sG->isElement(n));
  it=_parentGraph->getInEdges(n);
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  // _hasnext is initially set to true to avoid failure when asserting
  // in the first call of next() below
  next();
}
InEdgesIterator::~InEdgesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
edge InEdgesIterator::next() {
  assert(_hasnext);
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  if ((_hasnext = it->hasNext())) {
    curEdge=it->next();
    while (!(_hasnext = _filter.get(curEdge.id)) &&
	   it->hasNext()) {
      curEdge=it->next();
    }
  }  
  return tmp;
}
bool InEdgesIterator::hasNext() {
  return (_hasnext);
}
//===================================================================
  InOutEdgesIterator::InOutEdgesIterator(const Graph *sG,const MutableContainer<bool>& filter, node n):FactorEdgeIterator(sG,filter), _hasnext(true) {
  assert(sG->isElement(n));
  it=_parentGraph->getInOutEdges(n);
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  // _hasnext is initially set to true to avoid failure when asserting
  // in the first call of next() below
  next();
}
InOutEdgesIterator::~InOutEdgesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
edge InOutEdgesIterator::next() {
  assert(_hasnext);
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  if ((_hasnext = it->hasNext())) {
    curEdge=it->next();
    while (!(_hasnext = _filter.get(curEdge.id)) &&
	   it->hasNext()) {
      curEdge=it->next();
    }
  }
  return tmp;
}
bool InOutEdgesIterator::hasNext() {
  return (_hasnext);
}
//============================================================
//************************************************************
//************************************************************
//============================================================
xSGraphNodeIterator::xSGraphNodeIterator(const Graph *sG):
  itId(((GraphImpl *)sG)->nodeIds.getIds()) {
#ifndef NDEBUG
  spG = (GraphImpl *)sG;
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->addGraphObserver(this);
#endif
}
xSGraphNodeIterator::~xSGraphNodeIterator(){
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->removeGraphObserver(this);
#endif
  delete itId;
}
node xSGraphNodeIterator::next() {
  assert(itId->hasNext());
  return node(itId->next());
}
bool xSGraphNodeIterator::hasNext() {
  return (itId->hasNext());
}
//===================================================================
xOutNodesIterator::xOutNodesIterator(const Graph *sG,const node n):
  it(new xOutEdgesIterator((GraphImpl *)sG,n)), spG((GraphImpl *)sG) {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->addGraphObserver(this);
#endif
}
xOutNodesIterator::~xOutNodesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->removeGraphObserver(this);
#endif
  delete it;
}
node xOutNodesIterator::next() {
  assert(it->hasNext());
  return spG->target(it->next());
}
bool xOutNodesIterator::hasNext() {
  return (it->hasNext());
}
//===================================================================
xInNodesIterator::xInNodesIterator(const Graph *sG,const node n): 
  it(new xInEdgesIterator(sG,n)), spG((GraphImpl *)sG) {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->addGraphObserver(this);
#endif
}
xInNodesIterator::~xInNodesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->removeGraphObserver(this);
#endif
  delete it;
}
node xInNodesIterator:: next() {
  assert(it->hasNext());
  return spG->source(it->next());
}
bool xInNodesIterator::hasNext() {
  return (it->hasNext());
}
//===================================================================
xInOutNodesIterator::xInOutNodesIterator(const Graph *sG,const node n):
  it(((GraphImpl *)sG)->nodes[n.id].begin()),
  itEnd(((GraphImpl *)sG)->nodes[n.id].end()),
  n(n), spG((GraphImpl *)sG) {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->addGraphObserver(this);
#endif
}
xInOutNodesIterator::~xInOutNodesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->removeGraphObserver(this);
#endif
}
node xInOutNodesIterator::next() {
  assert(it!=itEnd);
  edge tmp=(*it);
  it++;
  return spG->opposite(tmp,n);
}
bool xInOutNodesIterator::hasNext() {
  return (it!=itEnd);
}
//===============================================================
xSGraphEdgeIterator::xSGraphEdgeIterator(const Graph *sG):
  itId(((GraphImpl *)sG)->edgeIds.getIds()) {
#ifndef NDEBUG
  spG = (GraphImpl *)sG;
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->addGraphObserver(this);
#endif
}
xSGraphEdgeIterator::~xSGraphEdgeIterator(){
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->removeGraphObserver(this);
#endif
  delete itId;
}
edge xSGraphEdgeIterator::next() {
  assert(itId->hasNext());
  return edge(itId->next());
}
bool xSGraphEdgeIterator::hasNext() {
  return itId->hasNext();
}
//===================================================================
xOutEdgesIterator::xOutEdgesIterator(const Graph *sG,const node n):
  it(((GraphImpl *)sG)->nodes[n.id].begin()),
  itEnd(((GraphImpl *)sG)->nodes[n.id].end()),
  // curEdge(0) is needed to avoid failure when asserting
  // in the first call of next() below
  n(n), curEdge(0), spG((GraphImpl *)sG) {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->addGraphObserver(this);
#endif
  next();
}
xOutEdgesIterator::~xOutEdgesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->removeGraphObserver(this);
#endif
}
edge xOutEdgesIterator::next() {
  // check hasNext()
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  if (it!=itEnd) {
    curEdge=*it;
    while(1) {
      while (spG->edges[curEdge.id].first!=n) {
	++it;
	if (it==itEnd) {
	  // curEdge no longer valid => hasNext == false
	  curEdge = edge();
	  return tmp;
	}
	curEdge=*it;
      }
      if (spG->edges[curEdge.id].second == n) {
	if (loop.find(curEdge)==loop.end()) {
	  loop.insert(curEdge);
	  break;
	}
	else {
	  ++it;
	  if (it==itEnd) {
	    // curEdge no longer valid => hasNext == false
	    curEdge = edge();
	    return tmp;
	  }
	  curEdge=*it;
	}
      }
      else break;
    }
    ++it;
  } else
    // curEdge no longer valid => hasNext == false
    curEdge = edge();
  return tmp;
}
bool xOutEdgesIterator::hasNext() {
  return (curEdge.isValid());
}
//===================================================================
xInEdgesIterator::xInEdgesIterator(const Graph *sG,const node n):
  it(((GraphImpl *)sG)->nodes[n.id].begin()),
  itEnd(((GraphImpl *)sG)->nodes[n.id].end()),
  // curEdge(0) is needed to avoid failure when asserting
  // in the first call of next() below
  n(n), curEdge(0), spG((GraphImpl *)sG) {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->addGraphObserver(this);
#endif
  next();
}
xInEdgesIterator::~xInEdgesIterator() {
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->removeGraphObserver(this);
#endif
}
edge xInEdgesIterator::next() {
  // check hasNext()
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  if (it!=itEnd) {
    curEdge=*it;
    while(1) {
      while (spG->edges[curEdge.id].second!=n) {
	++it;
	if (it==itEnd) {
	  // curEdge no longer valid => hasNext == false
	  curEdge = edge();
	  return tmp;
	}
	curEdge=*it;
      }
      if (spG->edges[curEdge.id].first == n) {
	if (loop.find(curEdge)==loop.end()) {
	  loop.insert(curEdge);
	  ++it;
	  if (it==itEnd) {
	    // curEdge no longer valid => hasNext == false
	    curEdge = edge();
	    return tmp;
	  }
	  curEdge=*it;
	}
	else {
	  break;
	}
      }
      else break;
    }
    ++it;
  } else
    // curEdge no longer valid => hasNext == false
    curEdge = edge();
  return tmp;
}
bool xInEdgesIterator::hasNext() {
  return (curEdge.isValid());
}
//===================================================================
xInOutEdgesIterator::xInOutEdgesIterator(const Graph *sG, const node n): 
  it(((GraphImpl *)sG)->nodes[n.id].begin()),
  itEnd(((GraphImpl *)sG)->nodes[n.id].end()){
#ifndef NDEBUG
  spG = (GraphImpl *)sG;
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->addGraphObserver(this);
#endif
}
xInOutEdgesIterator::~xInOutEdgesIterator(){
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  spG->removeGraphObserver(this);
#endif
}
edge xInOutEdgesIterator::next() {
  // check hasNext()
  assert(it != itEnd);
  edge tmp=(*it);
  ++it;
  return tmp;
}
bool xInOutEdgesIterator::hasNext() {
  return (it!=itEnd);
}
//===================================================================
} // namespace tpl
