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
#include <tulip/Graph.h>
#include <tulip/GraphImpl.h>
#include <tulip/GraphIterator.h>

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
 :FactorNodeIterator(sG,filter), sg(sG), value(val) {
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
  // anticipate first iteration
  prepareNext();
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
void SGraphNodeIterator::prepareNext() {
  while(it->hasNext()) {
    curNode = it->next();
    if (_filter.get(curNode) == value &&
	sg->isElement(curNode))
      return;
  }
  // set curNode as invalid
  curNode = node();
}
node SGraphNodeIterator::next() {
  assert(curNode.isValid());
  // we are already pointing to the next
  node tmp = curNode;
  // anticipate next iteration
  prepareNext();
  return tmp;
}
bool SGraphNodeIterator::hasNext(){ 
  return (curNode.isValid());
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
SGraphEdgeIterator::SGraphEdgeIterator(const Graph *sG, const MutableContainer<bool>& filter, bool val):FactorEdgeIterator(sG,filter), sg(sG), value(val) {
  it=_parentGraph->getEdges();
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  // anticipate first iteration
  prepareNext();
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
void SGraphEdgeIterator::prepareNext() {
  while(it->hasNext()) {
    curEdge=it->next();
    if (_filter.get(curEdge.id) == value &&
	sg->isElement(curEdge))
      return;
  }
  // set curEdge as invalid
  curEdge = edge();
}
edge SGraphEdgeIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  prepareNext();
  return tmp;
}
bool SGraphEdgeIterator::hasNext() {
  return (curEdge.isValid());
}
//===================================================================
OutEdgesIterator::OutEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter,node n):FactorEdgeIterator(sG,filter) {
  assert(sG->isElement(n));
  it=_parentGraph->getOutEdges(n);
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  // anticipate first iteration
  prepareNext();
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
void OutEdgesIterator::prepareNext() {
  while(it->hasNext()) {
    curEdge=it->next();
    if (_filter.get(curEdge.id))
      return;
  }
  // set curEdge as invalid
  curEdge = edge();
}
edge OutEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  prepareNext();
  return tmp;
}
bool OutEdgesIterator::hasNext() {
  return (curEdge.isValid());
}
//===================================================================
  InEdgesIterator::InEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n):FactorEdgeIterator(sG,filter) {
  assert(sG->isElement(n));
  it=_parentGraph->getInEdges(n);
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  // anticipate first iteration
  prepareNext();
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
void InEdgesIterator::prepareNext() {
  while (it->hasNext()) {
    curEdge=it->next();
    if (_filter.get(curEdge.id))
      return;
  }
  // set curEdge as invalid
  curEdge = edge();
}
edge InEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  prepareNext();
  return tmp;
}
bool InEdgesIterator::hasNext() {
  return (curEdge.isValid());
}
//===================================================================
  InOutEdgesIterator::InOutEdgesIterator(const Graph *sG,const MutableContainer<bool>& filter, node n):FactorEdgeIterator(sG,filter) {
  assert(sG->isElement(n));
  it=_parentGraph->getInOutEdges(n);
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  _parentGraph->addGraphObserver(this);
#endif
  // anticipate first iteration
  prepareNext();
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
void InOutEdgesIterator::prepareNext() {
  while(it->hasNext()) {
    curEdge=it->next();
    if (_filter.get(curEdge.id))
      return;
  }
  // set curEdge as invalid
  curEdge = edge();
}
edge InOutEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp=curEdge;
  // anticipating the next iteration
  prepareNext();
  return tmp;
}
bool InOutEdgesIterator::hasNext() {
  return (curEdge.isValid());
}
//============================================================
//************************************************************
//************************************************************
//============================================================
  GraphImplNodeIterator::GraphImplNodeIterator(const Graph*
#ifndef NDEBUG
					       g
#endif
					       , Iterator<node>* it):
  itId(it) {
#ifndef NDEBUG
  graph = (GraphImpl *) g;
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  graph->addGraphObserver(this);
#endif
}
GraphImplNodeIterator::~GraphImplNodeIterator(){
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  graph->removeGraphObserver(this);
#endif
  delete itId;
}
node GraphImplNodeIterator::next() {
  assert(itId->hasNext());
  return itId->next();
}
bool GraphImplNodeIterator::hasNext() {
  return (itId->hasNext());
}
//===============================================================
GraphImplEdgeIterator::GraphImplEdgeIterator(const Graph*
#ifndef NDEBUG
					       g
#endif
					     , Iterator<edge>* it):
  itId(it) {
#ifndef NDEBUG
  graph = (GraphImpl *) g;
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  graph->addGraphObserver(this);
#endif
}
GraphImplEdgeIterator::~GraphImplEdgeIterator(){
#ifndef NDEBUG
#ifdef _OPENMP
  // see explanation above
  #pragma omp critical(addRemoveObserver)
#endif
  graph->removeGraphObserver(this);
#endif
  delete itId;
}
edge GraphImplEdgeIterator::next() {
  assert(itId->hasNext());
  return itId->next();
}
bool GraphImplEdgeIterator::hasNext() {
  return itId->hasNext();
}
//===================================================================
} // namespace tpl
