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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/Graph.h"
#include "tulip/GraphImpl.h"
#include "tulip/GraphView.h"
#include "tulip/LayoutProperty.h"
#include "tulip/GraphIterator.h"
#include "tulip/StableIterator.h"

using namespace std;
using namespace tlp;

/* 
 * function to test if an edge e exist in the adjacency of a node
 */
static bool existEdgeE(Graph *g, const node n1, const node n2, edge e) {
  Iterator<edge> *it = g->getOutEdges(n1);
  while (it->hasNext()) {
    edge e1(it->next());
    if (e == e1) {
      delete it;
      return true;
    }
  } delete it;
  return false;
}
/* 
 * function to test the integrity of the graph structure
 */
/* no needed for the moment
static bool integrityTest(Graph *graph) {
  Iterator<edge> *itE = graph->getEdges();
  set<edge> edgesTest;
  while(itE->hasNext()) {
    edge e = itE->next();
    edgesTest.insert(e);
    if (!existEdgeE(graph, graph->source(e), graph->target(e), e)) {
      cerr << "edge do not exist in neighbood" << endl;
      delete itE;
      return false;
    }
  }
  delete itE;
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()) {
    node n(itN->next());
    unsigned int degree = 0;
    Iterator<edge> *it = graph->getInOutEdges(n);
    while (it->hasNext()) {
      edge e = it->next();
      bool found = edgesTest.find(e)!=edgesTest.end();
      if (graph->isElement(e)!=found) {
	cerr << "isElment function not valid" << endl;
	delete it;
	delete itN;
	return false;
      }
      if (!graph->isElement(e)) {
	cerr << "Adjacency edges are not valid" << endl;
	delete it;
	delete itN;
	return false;
      }
      degree++;
    } delete it;
    if (graph->deg(n) != degree) {
      cerr << "degree failed" << endl;
      return false;
    }
  }delete itN;
  return true;
}
*/
//----------------------------------------------------------------
GraphImpl::GraphImpl():GraphAbstract(this) {
  nbNodes=0;
  nbEdges=0;
  outDegree.setAll(0);
}
//----------------------------------------------------------------
GraphImpl::~GraphImpl() {
  notifyDestroy(this);
  StableIterator<Graph *> itS(getSubGraphs());
  while(itS.hasNext())
    delAllSubGraphs(itS.next());
  delete propertyContainer; //must be done here because Property proxy needs to access to the graph structure
  removeObservers();
  for (Nodes::iterator i=nodes.begin();i!=nodes.end();++i) {
    i->deallocateAll();
  }
}
//----------------------------------------------------------------
void GraphImpl::clear() {
  GraphAbstract::clear();
  nbNodes=0;
  nbEdges=0;
  outDegree.setAll(0);
}
//----------------------------------------------------------------
bool GraphImpl::isElement(const node n) const {
  return !nodeIds.is_free(n.id);
}
//----------------------------------------------------------------
bool GraphImpl::isElement(const edge e) const {
  return !edgeIds.is_free(e.id);
}
//----------------------------------------------------------------
node GraphImpl::addNode() {
  node newNode(nodeIds.get());
  outDegree.set(newNode.id, 0);
  while (nodes.size() <= newNode.id){
    nodes.push_back(EdgeContainer());
  }
  assert(nodes[newNode.id].empty());
  nbNodes++;
  notifyAddNode(this,newNode);
  return newNode;
}
//----------------------------------------------------------------
void GraphImpl::addNode(const node n) {
  cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << endl;
}
//----------------------------------------------------------------
edge GraphImpl::addEdge(const node s,const node t) {
  assert(isElement(s) && isElement(t));
  pair< node , node > tmp(s,t);
  outDegree.set(s.id, outDegree.get(s.id) + 1);
  edge newEdge(edgeIds.get());
  while (edges.size()<=newEdge.id){
    edges.push_back(tmp);
  }
  edges[newEdge.id] = tmp;
  nodes[s.id].push_back(newEdge);
  nodes[t.id].push_back(newEdge);
  nbEdges++;
  notifyAddEdge(this,newEdge);
  return newEdge;
}
//----------------------------------------------------------------
void GraphImpl::addEdge(const edge e) {
  cerr << "{Warning ] : "  << __PRETTY_FUNCTION__ << " ... Impossible operation on Root Graph" << endl;
  cerr << "\t Trying to add edge " << e.id << " (" << source(e).id << "," << target(e).id << ")" << endl;
}
//----------------------------------------------------------------
void GraphImpl::delNode(const node n) {
  assert (isElement(n));
  notifyDelNode(this, n);
  externRemove(n);
  set<edge> loops;
  bool haveLoops = false;
  unsigned int toRemove = 0;
  for(EdgeContainer::iterator i=nodes[n.id].begin(); i!=nodes[n.id].end(); ++i) {
    node s = opposite(*i, n);
    if (s!=n) {
      notifyDelEdge(this, *i);
      if (source(*i) == s)
	outDegree.set(s.id, outDegree.get(s.id) - 1);
      removeEdge(nodes[s.id], *i);
      getPropertyManager()->erase(*i);
      edgeIds.free(i->id);
      ++toRemove;
    }
    else {
      loops.insert(*i);
      haveLoops = true;
    }
  }
  if (haveLoops) {
    set<edge>::const_iterator it;
    for ( it = loops.begin(); it!=loops.end(); ++it) {
      notifyDelEdge(this, *it);
      getPropertyManager()->erase(*it);
      edgeIds.free(it->id);
      ++toRemove;
    }
  }
  nbEdges -= toRemove;
  nodes[n.id].clear();
}
//----------------------------------------------------------------
void GraphImpl::delEdge(const edge e) {
  notifyDelEdge(this,e);
  assert(existEdgeE(this, source(e),target(e), e));
  if (!isElement(e)) {
    return;
  }
  //Warning, the current implementation doesn't manage the updating of 
  //properties for upper_subgraph in the case of Super Graph Implementation
  node s = source(e);
  node t = target(e);
  outDegree.set(s.id, outDegree.get(s.id)-1);
  externRemove(e);
  removeEdge(nodes[s.id], e);
  removeEdge(nodes[t.id], e);
}
//----------------------------------------------------------------
void GraphImpl::delAllNode(const node n){delNode(n);}
//----------------------------------------------------------------
void GraphImpl::delAllEdge(const edge e){delEdge(e);}
//----------------------------------------------------------------
void GraphImpl::setEdgeOrder(const node n,const vector<edge> &v ) {
  //  cerr << __PRETTY_FUNCTION__ << "not tested function" << endl;
  if (v.size()==0) return;
  MutableContainer<int> isEle;
  isEle.setAll(0); 
  for (vector<edge>::const_iterator it=v.begin();it!=v.end();++it) {
    isEle.set(it->id, isEle.get(it->id)+1);
  }
  vector<edge>::const_iterator it2=v.begin();
  EdgeContainer currentOrder = nodes[n.id];
  for (unsigned int i=0; i<currentOrder.size(); ++i) {
    if ( isEle.get(currentOrder[i].id)>0 ) {
      isEle.set(currentOrder[i].id, isEle.get(currentOrder[i].id) -1);
      currentOrder[i] = *it2;
      ++it2;
    }
  }
}
//----------------------------------------------------------------
void GraphImpl::swapEdgeOrder(const node n,const edge e1 , const edge e2) {
  //  cerr << __PRETTY_FUNCTION__ << " not tested function" << endl;
  if (e1==e2) return;
  EdgeContainer adjacency=nodes[n.id];
  unsigned int e1Pos=UINT_MAX,e2Pos=UINT_MAX;
  for (unsigned int i=0;i<deg(n);++i) {
    if (adjacency[i]==e1) e1Pos=i;
    if (adjacency[i]==e2) e2Pos=i;
    if (e1Pos!=UINT_MAX && e2Pos!=UINT_MAX) break;
  }
  assert(e1Pos!=UINT_MAX && e2Pos!=UINT_MAX);
  adjacency[e1Pos]=e2;
  adjacency[e2Pos]=e1;
}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getNodes()const
{return (new xSGraphNodeIterator(this));}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getInNodes(const node n)const
{return (new xInNodesIterator(this,n));}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getOutNodes(const node n)const
{return (new xOutNodesIterator(this,n));}
//----------------------------------------------------------------
Iterator<node>* GraphImpl::getInOutNodes(const node n)const
{return (new xInOutNodesIterator(this,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getEdges()const
{return (new xSGraphEdgeIterator(this));}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getInEdges(const node n)const
{return (new xInEdgesIterator(this,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getOutEdges(const node n)const
{return (new xOutEdgesIterator(this,n));}
//----------------------------------------------------------------
Iterator<edge>* GraphImpl::getInOutEdges(const node n)const
{return (new xInOutEdgesIterator(this,n));}
//----------------------------------------------------------------
unsigned int GraphImpl::deg(const node n) const {
  assert(isElement(n));
  return nodes[n.id].size();
}
//----------------------------------------------------------------
unsigned int GraphImpl::indeg(const node n) const {
  assert(isElement(n));
  return nodes[n.id].size()-outDegree.get(n.id);
}
//----------------------------------------------------------------
unsigned int GraphImpl::outdeg(const node n) const {
  assert(isElement(n));
  return outDegree.get(n.id);
}
//----------------------------------------------------------------
node GraphImpl::source(const edge e)const{
  assert(isElement(e));
  return edges[e.id].first;
}
//----------------------------------------------------------------
node GraphImpl::target(const edge e)const{
  assert(isElement(e));
  return edges[e.id].second;
}
//----------------------------------------------------------------
const string layoutProperty = "viewLayout";
void GraphImpl::reverse(const edge e) {
  assert(isElement(e));
  node src = edges[e.id].first;
  node tgt = edges[e.id].second;
  edges[e.id].first  = tgt;
  edges[e.id].second = src;
  outDegree.set(src.id, outDegree.get(src.id) - 1);
  outDegree.set(tgt.id, outDegree.get(tgt.id) + 1);
  // reverse bends if needed
  if (existProperty(layoutProperty)) {
    LayoutProperty *graphLayout = (LayoutProperty *) getProperty(layoutProperty);
    std::vector<Coord> bends = graphLayout->getEdgeValue(e);
    if (bends.size() > 0) {
      unsigned int halfSize = bends.size()/2;
      for (unsigned int i = 0, j = bends.size() - 1; i < halfSize; ++i, j--) {
	Coord tmp = bends[i];
	bends[i] = bends[j];
	bends[j] = tmp;
      }
      graphLayout->setEdgeValue(e, bends);
    }
  }
  notifyReverseEdge(this,e);
}
//----------------------------------------------------------------
unsigned int GraphImpl::numberOfEdges()const{return nbEdges;}
//----------------------------------------------------------------
unsigned int GraphImpl::numberOfNodes()const{return nbNodes;}
//----------------------------------------------------------------
void GraphImpl::removeEdge(EdgeContainer &c, const edge e) {
  bool copy = false;
  EdgeContainer::iterator previous;
  for (EdgeContainer::iterator i=c.begin(); i!=c.end(); ++i) {
    edge e1 = *i;
    if (copy)
      *previous = e1;
    previous = i;
    if (e1 == e)  
      copy = true;
  }
  c.pop_back();
}
//----------------------------------------------------------------
void GraphImpl::externRemove(const edge e) {
  assert(isElement(e));
  Iterator<Graph *>*itS=getSubGraphs();
  while (itS->hasNext()) {
    Graph *subgraph = itS->next();
    assert(subgraph != this);
    if (subgraph->isElement(e))
      subgraph->delEdge(e);
  } delete itS;
  getPropertyManager()->erase(e);
  edgeIds.free(e.id);
  nbEdges--;
}
//----------------------------------------------------------------
void GraphImpl::externRemove(const node n) {
  assert(isElement(n));
  Iterator<Graph *>*itS=getSubGraphs();
  while (itS->hasNext()) {
    Graph *subgraph = itS->next();
    assert(subgraph != this);
    if (subgraph->isElement(n))
      subgraph->delNode(n);
  } delete itS;
  getPropertyManager()->erase(n);
  nodeIds.free(n.id);
  nbNodes--;
}
//----------------------------------------------------------------
