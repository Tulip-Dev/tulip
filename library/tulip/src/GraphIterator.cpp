//-*-c++-*-
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

void NodeIteratorObserver::addNode(Graph* g, node n) {
  if (hasNext())
    std::cerr << "Warning: node added while iterating !!!" << std::endl;
}

void NodeIteratorObserver::delNode(Graph* g, node n) {
  if (hasNext())
    std::cerr << "Warning: node deleted while iterating !!!" << std::endl;
}
   
void EdgeIteratorObserver::addEdge(Graph* g, edge e) {
  if (hasNext())
    std::cerr << "Warning: edge added while iterating !!!" << std::endl;
}

void EdgeIteratorObserver::delEdge(Graph* g, edge e) {
  if (hasNext())
    std::cerr << "Warning: edge deleted while iterating !!!" << std::endl;
}

#endif

int getNumIterators() {
#ifndef NDEBUG
  return NumIterators;
#else
  return 0;
#endif
}

//============================================================
SGraphNodeIterator::SGraphNodeIterator(const Graph *sG, const MutableContainer<bool>& filter):FactorNodeIterator(sG,filter) {
  it=_parentGraph->getNodes();
  _hasnext=false;
#ifndef NDEBUG
  _parentGraph->addGraphObserver(this);
#endif
  if (it->hasNext()) {
    curNode=it->next();
    while (_filter.get(curNode.id)!=true) {
      if (!it->hasNext()) break;
      curNode=it->next();
    }
    if (_filter.get(curNode.id)) _hasnext=true;
  }
}
SGraphNodeIterator::~SGraphNodeIterator() {
#ifndef NDEBUG
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
node SGraphNodeIterator::next() {
  node tmp=curNode;
  _hasnext=false;
  if (it->hasNext()) {
    curNode=it->next();
    while (_filter.get(curNode.id)!=true) {
      if (!it->hasNext()) break;
      curNode=it->next();
    }
    if (_filter.get(curNode.id)) _hasnext=true;
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
  _parentGraph->addGraphObserver(this);
#endif
}
OutNodesIterator::~OutNodesIterator() {
#ifndef NDEBUG
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
node OutNodesIterator::next() {
#ifndef NDEBUG
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
  _parentGraph->addGraphObserver(this);
#endif
}
InNodesIterator::~InNodesIterator() { 
#ifndef NDEBUG
  _parentGraph->removeGraphObserver(this);
#endif
  delete it; 
}
node InNodesIterator:: next() {
#ifndef NDEBUG
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
  _parentGraph->addGraphObserver(this);
#endif
}
InOutNodesIterator::~InOutNodesIterator() {
#ifndef NDEBUG
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
node InOutNodesIterator::next() {
#ifndef NDEBUG
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
SGraphEdgeIterator::SGraphEdgeIterator(const Graph *sG, const MutableContainer<bool>& filter):FactorEdgeIterator(sG,filter) {
  it=_parentGraph->getEdges();
  _hasnext=false;
#ifndef NDEBUG
  _parentGraph->addGraphObserver(this);
#endif
  if (it->hasNext()) {
    curEdge=it->next();
    while (_filter.get(curEdge.id)!=true) {
      if (!it->hasNext()) break;
      curEdge=it->next();
    }
    if (_filter.get(curEdge.id)) _hasnext=true;
  }
}
SGraphEdgeIterator::~SGraphEdgeIterator() {
#ifndef NDEBUG
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
edge SGraphEdgeIterator::next() {
  edge tmp=curEdge;
  _hasnext=false;
  if (it->hasNext()){
    curEdge=it->next();
    while (_filter.get(curEdge.id)!=true) {
      if (!it->hasNext()) break;
      curEdge=it->next();
    }
    if (_filter.get(curEdge.id)) _hasnext=true;
  }
  return tmp;
}
bool SGraphEdgeIterator::hasNext() {
  return (_hasnext);
}
//===================================================================
OutEdgesIterator::OutEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter,node n):FactorEdgeIterator(sG,filter) {
  assert(sG->isElement(n));
  it=_parentGraph->getOutEdges(n);
  _hasnext=false;
#ifndef NDEBUG
  _parentGraph->addGraphObserver(this);
#endif
  if (it->hasNext()){
    curEdge=it->next();
    while (_filter.get(curEdge.id)!=true) {
      if (!it->hasNext()) break;
      curEdge=it->next();
    }
    if (_filter.get(curEdge.id)) _hasnext=true;
  }
}
OutEdgesIterator::~OutEdgesIterator() {
#ifndef NDEBUG
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
edge OutEdgesIterator::next() {
  edge tmp=curEdge;
  _hasnext=false;
  if (it->hasNext()) {
      curEdge=it->next();
      while (_filter.get(curEdge.id)!=true) {
	  if (!it->hasNext()) break;
	  curEdge=it->next();
	}
      if (_filter.get(curEdge.id))
	_hasnext=true;
    }
  return tmp;
}
bool OutEdgesIterator::hasNext() {
  return (_hasnext);
}
//===================================================================
InEdgesIterator::InEdgesIterator(const Graph *sG, const MutableContainer<bool>& filter, node n):FactorEdgeIterator(sG,filter){
  assert(sG->isElement(n));
  it=_parentGraph->getInEdges(n);
  _hasnext=false;
#ifndef NDEBUG
  _parentGraph->addGraphObserver(this);
#endif
  if (it->hasNext()){
    curEdge=it->next();
    while (_filter.get(curEdge.id)!=true){
      if (!it->hasNext()) break;
      curEdge=it->next();
    }
    if (_filter.get(curEdge.id)) _hasnext=true;
  }
}
InEdgesIterator::~InEdgesIterator() {
#ifndef NDEBUG
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
edge InEdgesIterator::next() {
  edge tmp=curEdge;
  _hasnext=false;
  if (it->hasNext()) {
    curEdge=it->next();
    while (_filter.get(curEdge.id)!=true) {
      if (!it->hasNext()) break;
      curEdge=it->next();
    }
    if (_filter.get(curEdge.id)) _hasnext=true;
  }  
  return tmp;
}
bool InEdgesIterator::hasNext() {
  return (_hasnext);
}
//===================================================================
InOutEdgesIterator::InOutEdgesIterator(const Graph *sG,const MutableContainer<bool>& filter, node n):FactorEdgeIterator(sG,filter){
  assert(sG->isElement(n));
  it=_parentGraph->getInOutEdges(n);
  _hasnext=false;
#ifndef NDEBUG
  _parentGraph->addGraphObserver(this);
#endif
  if (it->hasNext()) {
    curEdge=it->next();
    while (_filter.get(curEdge.id)!=true) {
      if (!it->hasNext()) break;
      curEdge=it->next();
    }
    if (_filter.get(curEdge.id)) _hasnext=true;
  }
}
InOutEdgesIterator::~InOutEdgesIterator() {
#ifndef NDEBUG
  _parentGraph->removeGraphObserver(this);
#endif
  delete it;
}
edge InOutEdgesIterator::next() {
  edge tmp=curEdge;
  _hasnext=false;
  if (it->hasNext()) {
    curEdge=it->next();
    while (_filter.get(curEdge.id)!=true) {
      if (!it->hasNext()) break;
      curEdge=it->next();
    }
    if (_filter.get(curEdge.id)) _hasnext=true;
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
  spG->addGraphObserver(this);
#endif
}
xSGraphNodeIterator::~xSGraphNodeIterator(){
#ifndef NDEBUG
  spG->removeGraphObserver(this);
#endif
  delete itId;
}
node xSGraphNodeIterator::next() {
  return node(itId->next());
}
bool xSGraphNodeIterator::hasNext() {
  return (itId->hasNext());
}
//===================================================================
xOutNodesIterator::xOutNodesIterator(const Graph *sG,const node n):
  it(new xOutEdgesIterator((GraphImpl *)sG,n)), spG((GraphImpl *)sG) {
#ifndef NDEBUG
  spG->addGraphObserver(this);
#endif
}
xOutNodesIterator::~xOutNodesIterator() {
#ifndef NDEBUG
  spG->removeGraphObserver(this);
#endif
  delete it;
}
node xOutNodesIterator::next() {
  return spG->target(it->next());
}
bool xOutNodesIterator::hasNext() {
  return (it->hasNext());
}
//===================================================================
xInNodesIterator::xInNodesIterator(const Graph *sG,const node n): 
  it(new xInEdgesIterator(sG,n)), spG((GraphImpl *)sG) {
#ifndef NDEBUG
  spG->addGraphObserver(this);
#endif
}
xInNodesIterator::~xInNodesIterator() {
#ifndef NDEBUG
  spG->removeGraphObserver(this);
#endif
  delete it;
}
node xInNodesIterator:: next() {
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
  spG->addGraphObserver(this);
#endif
}
xInOutNodesIterator::~xInOutNodesIterator() {
#ifndef NDEBUG
  spG->removeGraphObserver(this);
#endif
}
node xInOutNodesIterator::next() {
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
  spG->addGraphObserver(this);
#endif
}
xSGraphEdgeIterator::~xSGraphEdgeIterator(){
#ifndef NDEBUG
  spG->removeGraphObserver(this);
#endif
  delete itId;
}
edge xSGraphEdgeIterator::next() {
  return edge(itId->next());
}
bool xSGraphEdgeIterator::hasNext() {
  return itId->hasNext();
}
//===================================================================
xOutEdgesIterator::xOutEdgesIterator(const Graph *sG,const node n):
  it(((GraphImpl *)sG)->nodes[n.id].begin()),
  itEnd(((GraphImpl *)sG)->nodes[n.id].end()),
  n(n), spG((GraphImpl *)sG) {
#ifndef NDEBUG
  spG->addGraphObserver(this);
#endif
  if (it!=itEnd) {
    curEdge=*it;
    while(1) {
      while (spG->edges[curEdge.id].first!=n) {
	++it;
	if (it==itEnd) break;
	curEdge=*it;
      }
      if (it==itEnd) break;
      if (spG->edges[curEdge.id].second == n) {
	if (loop.find(curEdge)==loop.end()) {
	  loop.insert(curEdge);
	  break;
	}
	else {
	  ++it;
	  if (it==itEnd) break;
	  curEdge=*it;
	}
      }
      else break;
    }
  }
}
xOutEdgesIterator::~xOutEdgesIterator() {
#ifndef NDEBUG
  spG->removeGraphObserver(this);
#endif
}
edge xOutEdgesIterator::next() {
  assert(hasNext());
  edge tmp=curEdge;
  ++it;
  if (it!=itEnd) {
    curEdge=*it;
    while(1) {
      while (spG->edges[curEdge.id].first!=n) {
	++it;
	if (it==itEnd) break;
	curEdge=*it;
      }
      if (it==itEnd) break;
      if (spG->edges[curEdge.id].second == n) {
	if (loop.find(curEdge)==loop.end()) {
	  loop.insert(curEdge);
	  break;
	}
	else {
	  ++it;
	  if (it==itEnd) break;
	  curEdge=*it;
	}
      }
      else break;
    }
  }
  return tmp;
}
bool xOutEdgesIterator::hasNext() {
  return (it!=itEnd);
}
//===================================================================
xInEdgesIterator::xInEdgesIterator(const Graph *sG,const node n):
  it(((GraphImpl *)sG)->nodes[n.id].begin()),
  itEnd(((GraphImpl *)sG)->nodes[n.id].end()),
  n(n), spG((GraphImpl *)sG) {
#ifndef NDEBUG
  spG->addGraphObserver(this);
#endif
  if (it!=itEnd) {
    curEdge=*it;
    while(1) {
      while (spG->edges[curEdge.id].second!=n) {
	++it;
	if (it==itEnd) break;
	curEdge=*it;
      }
      if (it==itEnd) break;
      if (spG->edges[curEdge.id].first == n) {
	if (loop.find(curEdge) == loop.end()) {
	  loop.insert(curEdge);
	  ++it;
	  if (it == itEnd) break;
	  curEdge = *it;
	}
	else {
	  break;
	}
      }
      else break;
    }
  }
}
xInEdgesIterator::~xInEdgesIterator() {
#ifndef NDEBUG
  spG->removeGraphObserver(this);
#endif
}
edge xInEdgesIterator::next() {
  assert(hasNext());
  edge tmp=curEdge;
  ++it;
  if (it!=itEnd) {
    curEdge=*it;
    while(1) {
      while (spG->edges[curEdge.id].second!=n) {
	++it;
	if (it==itEnd) break;
	curEdge=*it;
      }
      if (it==itEnd) break;
      if (spG->edges[curEdge.id].first == n) {
	if (loop.find(curEdge)==loop.end()) {
	  loop.insert(curEdge);
	  ++it;
	  if (it==itEnd) break;
	  curEdge=*it;
	}
	else {
	  break;
	}
      }
      else break;
    }
  }
  return tmp;
}
bool xInEdgesIterator::hasNext() {
  return (it!=itEnd);
}
//===================================================================
xInOutEdgesIterator::xInOutEdgesIterator(const Graph *sG, const node n): 
  it(((GraphImpl *)sG)->nodes[n.id].begin()),
  itEnd(((GraphImpl *)sG)->nodes[n.id].end()){
#ifndef NDEBUG
  spG = (GraphImpl *)sG;
  spG->addGraphObserver(this);
#endif
}
xInOutEdgesIterator::~xInOutEdgesIterator(){
#ifndef NDEBUG
  spG->removeGraphObserver(this);
#endif
}
edge xInOutEdgesIterator::next() {
  edge tmp=(*it);
  ++it;
  return tmp;
}
bool xInOutEdgesIterator::hasNext() {
  return (it!=itEnd);
}
//===================================================================
} // namespace tpl
