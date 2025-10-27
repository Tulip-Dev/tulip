/**
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
#include <cassert>

#include <tulip/Graph.h>
#include <tulip/GraphView.h>
#include <tulip/GraphIterators.h>
#include <tulip/Observable.h>

namespace tlp {

#if !defined(NDEBUG) && !defined(_OPENMP)
static int NumIterators = 0;

void incrNumIterators() {
  NumIterators++;
}

void decrNumIterators() {
  NumIterators--;
}

struct TLP_SCOPE NodeIteratorObserver : public Observable {
  NodeIterator *itn;
  NodeIteratorObserver(NodeIterator *it) : itn(it) {}
  // Observable interface
  void treatEvent(const Event &);
};

struct TLP_SCOPE EdgeIteratorObserver : public Observable {
  EdgeIterator *ite;
  EdgeIteratorObserver(EdgeIterator *it) : ite(it) {}
  // Observable interface
  void treatEvent(const Event &);
};

void NodeIteratorObserver::treatEvent(const Event &evt) {
  switch (static_cast<const GraphEvent *>(&evt)->getType()) {
  case GraphEvent::TLP_ADD_NODE:
  case GraphEvent::TLP_AFTER_DEL_NODE:

    if (itn->hasNext())
      tlp::warning() << "Warning: node added or deleted while iterating!!!" << std::endl;

  default:
    break;
  }
}

void EdgeIteratorObserver::treatEvent(const Event &evt) {
  switch (static_cast<const GraphEvent *>(&evt)->getType()) {
  case GraphEvent::TLP_ADD_EDGE:
  case GraphEvent::TLP_AFTER_DEL_EDGE:

    if (ite->hasNext())
      tlp::warning() << "Warning: edge added or deleted while iterating!!!" << std::endl;

  default:
    break;
  }
}

int getNumIterators() {
  return NumIterators;
}

#else

void incrNumIterators() {}

void decrNumIterators() {}

int getNumIterators() {
  return 0;
}

#endif

//===================================================================
void FactorNodeIterator::enableListening(const Graph *sg) {
#if !defined(NDEBUG) && !defined(_OPENMP)
  auto itno = new NodeIteratorObserver(this);
  sg->addListener(itno);
  ito = itno;
#else
  (void)sg;
#endif
}

void FactorNodeIterator::disableListening(const Graph *sg) {
#if !defined(NDEBUG) && !defined(_OPENMP)
  auto itno = static_cast<NodeIteratorObserver *>(ito);
  sg->removeListener(itno);
  delete itno;
#else
  (void)sg;
#endif
}
//===================================================================
OutNodesIterator::OutNodesIterator(const GraphView *sG, node n) : FactorNodeIterator(sG) {
  _parentGraph = _parentGraph->getRoot();
  it = new OutEdgesIterator(sG, n);
#if !defined(NDEBUG) && !defined(_OPENMP)
  sg = sG;
  enableListening(_parentGraph);
#endif
}
OutNodesIterator::~OutNodesIterator() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  disableListening(_parentGraph);
#endif
  delete it;
}
node OutNodesIterator::next() {
#if !defined(NDEBUG) && !defined(_OPENMP)
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
InNodesIterator::InNodesIterator(const GraphView *sG, node n)
    : FactorNodeIterator(sG), it(new InEdgesIterator(sG, n)) {
  _parentGraph = _parentGraph->getRoot();
#if !defined(NDEBUG) && !defined(_OPENMP)
  sg = sG;
  enableListening(_parentGraph);
#endif
}
InNodesIterator::~InNodesIterator() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  disableListening(_parentGraph);
#endif
  delete it;
}
node InNodesIterator::next() {
#if !defined(NDEBUG) && !defined(_OPENMP)
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
InOutNodesIterator::InOutNodesIterator(const GraphView *sG, node n)
    : FactorNodeIterator(sG), it(new InOutEdgesIterator(sG, n)), n(n) {
  _parentGraph = _parentGraph->getRoot();
#if !defined(NDEBUG) && !defined(_OPENMP)
  sg = sG;
  enableListening(_parentGraph);
#endif
}
InOutNodesIterator::~InOutNodesIterator() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  disableListening(_parentGraph);
#endif
  delete it;
}
node InOutNodesIterator::next() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  assert(it->hasNext());
  node tmp = _parentGraph->opposite(it->next(), n);
  assert(sg->isElement(tmp));
  return tmp;
#else
  return _parentGraph->opposite(it->next(), n);
#endif
}
bool InOutNodesIterator::hasNext() {
  return (it->hasNext());
}
//===================================================================
void FactorEdgeIterator::enableListening(const Graph *sg) {
#if !defined(NDEBUG) && !defined(_OPENMP)
  auto iteo = new EdgeIteratorObserver(this);
  sg->addListener(iteo);
  ito = iteo;
#else
  (void)sg;
#endif
}

void FactorEdgeIterator::disableListening(const Graph *sg) {
#if !defined(NDEBUG) && !defined(_OPENMP)
  auto iteo = static_cast<EdgeIteratorObserver *>(ito);
  sg->removeListener(iteo);
  delete iteo;
#else
  (void)sg;
#endif
}
//===================================================================
OutEdgesIterator::OutEdgesIterator(const GraphView *sG, node n) : FactorEdgeIterator(sG), sg(sG) {
  assert(sG->isElement(n));
  _parentGraph = _parentGraph->getRoot();
  it = _parentGraph->getOutEdges(n);
#if !defined(NDEBUG) && !defined(_OPENMP)
  enableListening(_parentGraph);
#endif
  // anticipate first iteration
  prepareNext();
}
OutEdgesIterator::~OutEdgesIterator() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  disableListening(_parentGraph);
#endif
  delete it;
}
void OutEdgesIterator::prepareNext() {
  while (it->hasNext()) {
    curEdge = it->next();

    if (sg->isElement(curEdge))
      return;
  }

  // set curEdge as invalid
  curEdge = edge();
}
edge OutEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp = curEdge;
  // anticipating the next iteration
  prepareNext();
  return tmp;
}
bool OutEdgesIterator::hasNext() {
  return (curEdge.isValid());
}
//===================================================================
InEdgesIterator::InEdgesIterator(const GraphView *sG, node n) : FactorEdgeIterator(sG), sg(sG) {
  assert(sG->isElement(n));
  _parentGraph = _parentGraph->getRoot();
  it = _parentGraph->getInEdges(n);
#if !defined(NDEBUG) && !defined(_OPENMP)
  enableListening(_parentGraph);
#endif
  // anticipate first iteration
  prepareNext();
}
InEdgesIterator::~InEdgesIterator() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  disableListening(_parentGraph);
#endif
  delete it;
}
void InEdgesIterator::prepareNext() {
  while (it->hasNext()) {
    curEdge = it->next();

    if (sg->isElement(curEdge))
      return;
  }

  // set curEdge as invalid
  curEdge = edge();
}
edge InEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp = curEdge;
  // anticipating the next iteration
  prepareNext();
  return tmp;
}
bool InEdgesIterator::hasNext() {
  return (curEdge.isValid());
}
//===================================================================
InOutEdgesIterator::InOutEdgesIterator(const GraphView *sG, node n)
    : FactorEdgeIterator(sG), sg(sG) {
  assert(sG->isElement(n));
  _parentGraph = _parentGraph->getRoot();
  it = _parentGraph->getInOutEdges(n);
#if !defined(NDEBUG) && !defined(_OPENMP)
  enableListening(_parentGraph);
#endif
  // anticipate first iteration
  prepareNext();
}
InOutEdgesIterator::~InOutEdgesIterator() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  disableListening(_parentGraph);
#endif
  delete it;
}
void InOutEdgesIterator::prepareNext() {
  while (it->hasNext()) {
    curEdge = it->next();

    if (sg->isElement(curEdge))
      return;
  }

  // set curEdge as invalid
  curEdge = edge();
}
edge InOutEdgesIterator::next() {
  assert(curEdge.isValid());
  // we are already pointing to the next
  edge tmp = curEdge;
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
GraphNodeIterator::GraphNodeIterator(const Graph *
#if !defined(NDEBUG) && !defined(_OPENMP)
                                         g
#endif
                                     ,
                                     Iterator<node> *it)
    : itId(it) {
#if !defined(NDEBUG) && !defined(_OPENMP)
  graph = g;
  auto itno = new NodeIteratorObserver(this);
  graph->addListener(itno);
  ito = itno;
#endif
}
GraphNodeIterator::~GraphNodeIterator() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  auto itno = static_cast<NodeIteratorObserver *>(ito);
  graph->removeListener(itno);
  delete itno;
#endif
  delete itId;
}
node GraphNodeIterator::next() {
  assert(itId->hasNext());
  return itId->next();
}
bool GraphNodeIterator::hasNext() {
  return (itId->hasNext());
}
//===============================================================
GraphEdgeIterator::GraphEdgeIterator(const Graph *
#if !defined(NDEBUG) && !defined(_OPENMP)
                                         g
#endif
                                     ,
                                     Iterator<edge> *it)
    : itId(it) {
#if !defined(NDEBUG) && !defined(_OPENMP)
  graph = g;
  auto iteo = new EdgeIteratorObserver(this);
  graph->addListener(iteo);
  ito = iteo;
#endif
}
GraphEdgeIterator::~GraphEdgeIterator() {
#if !defined(NDEBUG) && !defined(_OPENMP)
  auto iteo = static_cast<EdgeIteratorObserver *>(ito);
  graph->removeListener(iteo);
  delete iteo;
#endif
  delete itId;
}
edge GraphEdgeIterator::next() {
  assert(itId->hasNext());
  return itId->next();
}
bool GraphEdgeIterator::hasNext() {
  return itId->hasNext();
}
//===================================================================
} // namespace tlp
