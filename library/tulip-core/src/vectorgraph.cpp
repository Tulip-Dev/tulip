/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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


#include <vector>
#include <set>
#include <cassert>
#include <iostream>
#include <string>

#include <tulip/vectorgraph.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/ForEach.h>
#include <tulip/StlIterator.h>

using namespace std;
using namespace tlp;

namespace tlp {
template class TLP_SCOPE ValArray<double>;
template class TLP_SCOPE ValArray<node>;
template class TLP_SCOPE NodeProperty<node>;
template class TLP_SCOPE NodeProperty<double>;
template class TLP_SCOPE EdgeProperty<node>;
template class TLP_SCOPE EdgeProperty<double>;
}

namespace {
//==================================================================================
/**
  * \class bInOutNodeIterator
  * \brief internal class to iterate on adj+/adj- of element of a BAsicGraph
  * \see VectorGraph
  */
template <bool SENS>
class bInOutNodeIterator : public Iterator<node>, public MemoryPool<bInOutNodeIterator<SENS> > {
public:
  bInOutNodeIterator(const std::vector<node> &nodes, const std::vector<bool> &sens, unsigned int elem):
    itn(nodes.begin()), itn_end(nodes.end()),
    its(sens.begin()) , its_end(sens.end() ),
    _elem(elem) {
    update();
  }
  bool hasNext() {
    return its != its_end;
  }
  node next() {
    node n = *itn;
    ++its;
    ++itn;
    --_elem;
    update();
    return n;
  }

private:
  void update() {
    if (_elem == 0) {
      its = its_end;
      return;
    }

    while((its != its_end) && ((*its) != SENS)) {
      ++its;
      ++itn;
    }
  }

  std::vector<node>::const_iterator itn, itn_end;
  std::vector<bool>::const_iterator its, its_end;
  unsigned int _elem;
};

//==================================================================================
/**
  * \class bInOutEdgeIterator
  * \brief internal class to iterate on star+/star- of element of a BAsicGraph
  * \see VectorGraph
  */
template <bool SENS>
class bInOutEdgeIterator : public Iterator<edge>, public MemoryPool<bInOutEdgeIterator<SENS> > {
public:
  bInOutEdgeIterator(const std::vector<edge> &edges, const std::vector<bool> &sens, unsigned int elem ):
    ite(edges.begin()), ite_end(edges.end()),
    its(sens.begin()) , its_end(sens.end() ),
    _elem(elem) {
    update();
  }
  bool hasNext() {
    return its != its_end;
  }
  edge next() {
    edge e = *ite;
    ++its;
    ++ite;
    --_elem;
    update();
    return e;
  }

private:
  void update() {
    if (_elem == 0) {
      its = its_end;
      return;
    }

    while((its != its_end) && ((*its) != SENS)) {
      ++its;
      ++ite;
    }
  }

  std::vector<edge>::const_iterator ite, ite_end;
  std::vector<bool>::const_iterator its, its_end;
  unsigned int _elem;
};
}

namespace tlp {
//===========================================
VectorGraph::VectorGraph() {
}
//=======================================================
VectorGraph::~VectorGraph() {
  set<ValArrayInterface *>::const_iterator it;

  for(it = _nodeArrays.begin(); it!= _nodeArrays.end(); ++it)
    delete (*it);

  for(it = _edgeArrays.begin(); it!= _edgeArrays.end(); ++it)
    delete (*it);
}
//=======================================================
void VectorGraph::clear() {
  delAllNodes();
  set<ValArrayInterface *>::const_iterator it;

  for(it = _nodeArrays.begin(); it!= _nodeArrays.end(); ++it)
    delete (*it);

  for(it = _edgeArrays.begin(); it!= _edgeArrays.end(); ++it)
    delete (*it);

  _nodeArrays.clear();
  _edgeArrays.clear();
}
//=======================================================
edge VectorGraph::existEdge(const node src, const node tgt, bool directed ) const {
  node tn;

  if (deg(tgt) < deg(src)) {
    for (size_t i = 0; i<_nData[tgt]._adjt.size(); ++i) {
      if (directed && !_nData[tgt]._adjt[i] && _nData[tgt]._adjn[i] == src)
        return _nData[tgt]._adje[i];

      if (!directed && _nData[tgt]._adjn[i] == src)
        return _nData[tgt]._adje[i];
    }
  }
  else {
    for (size_t i = 0; i<_nData[src]._adje.size(); ++i) {
      if (directed && _nData[src]._adjt[i] && _nData[src]._adjn[i] == tgt)
        return _nData[src]._adje[i];

      if (!directed && _nData[src]._adjn[i] == tgt)
        return _nData[src]._adje[i];
    }
  }

  return edge();
}
//=======================================================
bool VectorGraph::isElement(const node n) const {
  if (n.id < _nData.size() && _nData[n]._nodesId != UINT_MAX)
    return true;

  return false;
}
//=======================================================
bool VectorGraph::isElement(const edge e) const {
  if (e.id < _eData.size() && _eData[e]._edgesId != UINT_MAX)
    return true;

  return false;
}
//=======================================================
void VectorGraph::setEdgeOrder(const node n, const std::vector<edge> &v ) {
  //@TODO add assert that test edge element of v are correct
  for (size_t i=0; i < v.size() - 1; ++i) {
    swapEdgeOrder(n, _nData[n]._adje[i], v[i]);
  }

  //        integrityTest();
}
//=======================================================
void VectorGraph::swapEdgeOrder(const node n, const edge e1, const edge e2) {
  if (e1 == e2) return; //we can't swap loops in our model

  unsigned int e1i, e2i;

  if (source(e1) == n)
    e1i = _eData[e1]._edgeExtremitiesPos.first;
  else
    e1i = _eData[e1]._edgeExtremitiesPos.second;

  if (source(e2) == n)
    e2i = _eData[e2]._edgeExtremitiesPos.first;
  else
    e2i = _eData[e2]._edgeExtremitiesPos.second;

  edge tmpe = _nData[n]._adje[e1i];
  _nData[n]._adje[e1i] = _nData[n]._adje[e2i];
  _nData[n]._adje[e2i] = tmpe;
  node tmpn = _nData[n]._adjn[e1i];
  _nData[n]._adjn[e1i] = _nData[n]._adjn[e2i];
  _nData[n]._adjn[e2i] = tmpn;
  bool tmpt = _nData[n]._adjt[e1i];
  _nData[n]._adjt[e1i] = _nData[n]._adjt[e2i];
  _nData[n]._adjt[e2i] = tmpt;

  if (source(e1) == n)
    _eData[e1]._edgeExtremitiesPos.first = e2i;
  else
    _eData[e1]._edgeExtremitiesPos.second = e2i;

  if (source(e2) == n)
    _eData[e2]._edgeExtremitiesPos.first = e1i;
  else
    _eData[e2]._edgeExtremitiesPos.second = e1i;
}
//=======================================================
void VectorGraph::reserveNodes(const size_t nbNodes) {
  _nodes.reserve(nbNodes);
  _nData.reserve(nbNodes);
  set<ValArrayInterface *>::const_iterator it;

  for(it = _nodeArrays.begin(); it!= _nodeArrays.end(); ++it)
    (*it)->reserve(nbNodes);
}
//=======================================================
void VectorGraph::reserveEdges(const size_t nbEdges) {
  _edges.reserve(nbEdges);
  _eData.reserve(nbEdges);
  set<ValArrayInterface *>::const_iterator it;

  for(it = _edgeArrays.begin(); it!= _edgeArrays.end(); ++it)
    (*it)->reserve(nbEdges);
}
//=======================================================
void VectorGraph::reserveAdj(const size_t nbEdges) {
  for (unsigned int i = 0; i < _nodes.size(); ++i) {
    reserveAdj(_nodes[i], nbEdges);
  }
}
//=======================================================
void VectorGraph::reserveAdj(const node n,const  size_t nbEdges) {
  _nData[n]._adjt.reserve(nbEdges);
  _nData[n]._adje.reserve(nbEdges);
  _nData[n]._adjn.reserve(nbEdges);
}
//=======================================================
node VectorGraph::operator[](const unsigned int id) const {
  assert(id < _nodes.size());
  return _nodes[id];
}
//=======================================================
edge VectorGraph::operator()(const unsigned int id) const {
  assert(id < _edges.size());
  return _edges[id];
}
//=======================================================
node VectorGraph::getOneNode() const {
  assert(numberOfNodes()>0);
  return _nodes[0];
}
//=======================================================
Iterator<node> * VectorGraph::getNodes() const {
  return new MPStlIterator<node, vector<node>::const_iterator>(_nodes.begin(), _nodes.end());
}
//=======================================================
Iterator<edge> * VectorGraph::getEdges() const {
  return new MPStlIterator<edge, vector<edge>::const_iterator>(_edges.begin(), _edges.end());
}
//=======================================================
Iterator<edge> * VectorGraph::getInOutEdges(const node n) const {
  return new MPStlIterator<edge, vector<edge>::const_iterator>(_nData[n]._adje.begin(), _nData[n]._adje.end());
}
//=======================================================
Iterator<edge> * VectorGraph::getOutEdges(const node n) const {
  return new bInOutEdgeIterator<true>(_nData[n]._adje, _nData[n]._adjt, outdeg(n));
}
//=======================================================
Iterator<edge> * VectorGraph::getInEdges(const node n) const {
  return new bInOutEdgeIterator<false>(_nData[n]._adje, _nData[n]._adjt, indeg(n));
}
//=======================================================
Iterator<node> * VectorGraph::getInOutNodes(const node n) const {
  return new MPStlIterator<node, vector<node>::const_iterator>(_nData[n]._adjn.begin(), _nData[n]._adjn.end());
}
//=======================================================
Iterator<node> * VectorGraph::getInNodes(const node n) const {
  return new bInOutNodeIterator<false>(_nData[n]._adjn, _nData[n]._adjt, indeg(n));
}
//=======================================================
Iterator<node> * VectorGraph::getOutNodes(const node n) const {
  return new bInOutNodeIterator<true>(_nData[n]._adjn, _nData[n]._adjt, outdeg(n));
}
//=======================================================
unsigned int VectorGraph::deg(const node n) const {
  return _nData[n]._adjn.size();
}
//=======================================================
unsigned int VectorGraph::outdeg(const node n) const {
  return _nData[n]._outdeg;
}
//=======================================================
unsigned int VectorGraph::indeg(const node n) const {
  return deg(n) - _nData[n]._outdeg;
}
//=======================================================
unsigned int VectorGraph::numberOfEdges() const {
  return _edges.size();
}
//=======================================================
unsigned int VectorGraph::numberOfNodes() const {
  return _nodes.size();
}
//=======================================================
node VectorGraph::addNode() {
  node newNode;

  if (_freeNodes.empty()) {
    newNode = node(_nodes.size());
    _nData.push_back(_iNodes(_nodes.size()));
    addNodeToArray(newNode);
  }
  else {
    newNode = _freeNodes.back();
    _freeNodes.pop_back();
    _nData[newNode].clear();
    _nData[newNode]._nodesId  = _nodes.size();
    //nodesVal[newNode] = NodeData();//If needed could be done by adding a reset function on ValArray
  }

  _nodes.push_back(newNode);
  return newNode;
}
//=======================================================
void VectorGraph::delNode(const node n) {
  assert(isElement(n));
  delEdges(n);
  //remove the nodes in the node vector
  unsigned int nb_nodes = _nodes.size();
  unsigned int npos = _nData[n]._nodesId;

  if (npos != nb_nodes - 1) {
    _nodes[npos] = _nodes[nb_nodes - 1];
    _nData[_nodes[npos]]._nodesId = npos;
  }

  _nodes.resize(nb_nodes - 1);
  _freeNodes.push_back(n);
  _nData[n]._nodesId = UINT_MAX;
  //integrityTest();
}
//=======================================================
edge VectorGraph::addEdge(const node src, const node tgt) {
  assert(isElement(src));
  assert(isElement(tgt));
  edge newEdge;

  if (_freeEdges.empty()) {
    newEdge = edge(_edges.size());
    _eData.push_back(_iEdges());
    _eData[newEdge]._edgeExtremities = pair<node, node>(src, tgt);

    if (src != tgt)
      _eData[newEdge]._edgeExtremitiesPos = pair<unsigned int, unsigned int>(_nData[src]._adje.size(), _nData[tgt]._adje.size() );
    else
      _eData[newEdge]._edgeExtremitiesPos = pair<unsigned int, unsigned int>(_nData[src]._adje.size(), _nData[tgt]._adje.size() + 1);

    addEdgeToArray(newEdge);
  }
  else {
    newEdge = _freeEdges.back();
    _freeEdges.pop_back();
    _eData[newEdge]._edgeExtremities = pair<node, node>(src, tgt);

    if (src != tgt)
      _eData[newEdge]._edgeExtremitiesPos = pair<unsigned int, unsigned int>(_nData[src]._adje.size(), _nData[tgt]._adje.size() );
    else //loop
      _eData[newEdge]._edgeExtremitiesPos = pair<unsigned int, unsigned int>(_nData[src]._adje.size(), _nData[tgt]._adje.size() + 1 );

    //edgesVal[newEdge] = EdgeData(); //If needed could be done by adding a reset function on ValArray
  }

  _eData[newEdge]._edgesId  = _edges.size();
  _edges.push_back(newEdge);

  _nData[src].addEdge(true, tgt, newEdge);
  _nData[tgt].addEdge(false, src, newEdge);

  _nData[src]._outdeg += 1;
  //integrityTest();
  return newEdge;
}
//=======================================================
void VectorGraph::delEdge(const edge e) {
  assert(isElement(e));
  _nData[source(e)]._outdeg -= 1;
  node psrc = _eData[e]._edgeExtremities.first;
  node ptgt = _eData[e]._edgeExtremities.second;
  partialDelEdge(psrc, e);

  if (psrc != ptgt)
    partialDelEdge(ptgt, e);

  //remove the edge in the edge vector
  removeEdge(e);
  //integrityTest();
}
//=======================================================
void VectorGraph::delEdges(const node n) {
  assert(isElement(n));
  vector<edge> tmpEdges(_nData[n]._adje);
  vector<edge>::iterator it(tmpEdges.begin());
  vector<edge>::iterator itEnd(tmpEdges.end());

  for(; it != itEnd; ++it) {
    if (!isElement(*it)) continue;

    if (source(*it) != target(*it)) {//not a loop
      partialDelEdge(opposite(*it, n), *it);
    }

    if (source(*it) != n)
      _nData[source(*it)]._outdeg -= 1;

    removeEdge(*it);
  }

  _nData[n].clear();
}
//=======================================================
void VectorGraph::delAllEdges() {
  _freeEdges.insert(_freeEdges.end(), _edges.rbegin(), _edges.rend());

  for(size_t i=0; i<_edges.size(); ++i) {
    _eData[_edges[i]]._edgesId = UINT_MAX;
  }

  _edges.resize(0);

  for(size_t i=0; i<_nodes.size(); ++i) {
    _nData[_nodes[i]].clear();
  }
}
//=======================================================
void VectorGraph::delAllNodes() {
  _freeEdges.insert(_freeEdges.end(),_edges.rbegin(), _edges.rend());

  for(size_t i=0; i<_edges.size(); ++i) {
    _eData[_edges[i]]._edgesId = UINT_MAX;
  }

  _edges.resize(0);

  _freeNodes.insert(_freeNodes.end(),_nodes.rbegin(), _nodes.rend());

  for(size_t i=0; i<_nodes.size(); ++i) {
    _nData[_nodes[i]]._nodesId = UINT_MAX;
  }

  _nodes.resize(0);
}
//=======================================================
node VectorGraph::source(const edge e) const {
  assert(isElement(e));
  return _eData[e]._edgeExtremities.first;
}
//=======================================================
node VectorGraph::target(edge e) const {
  assert(isElement(e));
  return _eData[e]._edgeExtremities.second;
}
//=======================================================
node VectorGraph::opposite(const edge e, const node n) const {
  assert(isElement(n));
  assert(isElement(e));
  assert(source(e) == n || target(e)==n);

  const pair<node, node> &endsE = _eData[e]._edgeExtremities;

  if (endsE.first == n)
    return endsE.second;
  else
    return endsE.first;
}
//=======================================================
void VectorGraph::reverse(const edge e) {
  assert(isElement(e));

  _nData[source(e)]._outdeg -= 1;
  _nData[target(e)]._outdeg += 1;
  node psrc = _eData[e]._edgeExtremities.first;
  node ptgt = _eData[e]._edgeExtremities.second;
  _nData[psrc]._adjt[_eData[e]._edgeExtremitiesPos.first ] = false;
  _nData[ptgt]._adjt[_eData[e]._edgeExtremitiesPos.second] = true;
  node tmpN = _eData[e]._edgeExtremities.first;
  _eData[e]._edgeExtremities.first = _eData[e]._edgeExtremities.second;
  _eData[e]._edgeExtremities.second = tmpN;
  unsigned int tmpP = _eData[e]._edgeExtremitiesPos.first;
  _eData[e]._edgeExtremitiesPos.first = _eData[e]._edgeExtremitiesPos.second;
  _eData[e]._edgeExtremitiesPos.second = tmpP;
  //integrityTest();
}
//=======================================================
void VectorGraph::setSource(const edge e, const node n) {
  assert(isElement(e));
  assert(isElement(n));
  setEnds(e, n, target(e));
}
//=======================================================
void VectorGraph::setTarget(const edge e, const node n) {
  assert(isElement(e));
  assert(isElement(n));
  setEnds(e, source(e), n);
}
//=======================================================
std::pair<node, node> VectorGraph::ends(const edge e) const {
  assert(isElement(e));
  return _eData[e]._edgeExtremities;
}
//=======================================================
void VectorGraph::setEnds(const edge e,const  node src, const node tgt) {
  assert(isElement(e));
  assert(isElement(src));
  assert(isElement(tgt));

  node psrc = _eData[e]._edgeExtremities.first;
  node ptgt = _eData[e]._edgeExtremities.second;

  _nData[psrc]._outdeg -= 1;
  _nData[src]._outdeg += 1;

  partialDelEdge(psrc, e);

  if (psrc != ptgt)
    partialDelEdge(ptgt, e);

  _eData[e]._edgeExtremities = pair<node, node>(src, tgt);

  if (src != tgt)
    _eData[e]._edgeExtremitiesPos = pair<unsigned int, unsigned int>(_nData[src]._adje.size(), _nData[tgt]._adje.size() );
  else
    _eData[e]._edgeExtremitiesPos = pair<unsigned int, unsigned int>(_nData[src]._adje.size(), _nData[tgt]._adje.size() + 1);

  _nData[src].addEdge(true, tgt, e);
  _nData[tgt].addEdge(false, src, e);

  //integrityTest();
}
//=======================================================
void VectorGraph::shuffleNodes() {
  random_shuffle(_nodes.begin(), _nodes.end());

  //recompute indices of nodes
  for (size_t i = 0; i < _nodes.size(); ++i) {
    _nData[_nodes[i]]._nodesId = i;
  }
}
//=======================================================
void VectorGraph::shuffleEdges() {
  random_shuffle(_edges.begin(), _edges.end());

  //recompute indices of edges
  for (size_t i = 0; i < _edges.size(); ++i) {
    _eData[_edges[i]]._edgesId = i;
  }
}
//=======================================================
unsigned int VectorGraph::edgePos(const edge e) const {
  assert(isElement(e));
  return _eData[e]._edgesId;
}
//=======================================================
unsigned int VectorGraph::nodePos(const node n) const {
  assert(isElement(n));
  return _nData[n]._nodesId;
}
//=======================================================
void VectorGraph::swap(const node a, const node b) {
  unsigned int pa = _nData[a]._nodesId;
  unsigned int pb = _nData[b]._nodesId;
  node tmp = _nodes[pa];
  _nodes[pa] = _nodes[pb];
  _nodes[pb] = tmp;
  _nData[a]._nodesId = pb;
  _nData[b]._nodesId = pa;
}
//=======================================================
void VectorGraph::swap(const edge a, const edge b) {
  unsigned int pa  = _eData[a]._edgesId;
  unsigned int pb  = _eData[b]._edgesId;
  edge tmp   = _edges[pa];
  _edges[pa]  = _edges[pb];
  _edges[pb]  = tmp;
  _eData[a]._edgesId = pb;
  _eData[b]._edgesId = pa;
}
//=======================================================
const vector<node>& VectorGraph::adj(const node n) const {
  assert(isElement(n));
  return _nData[n]._adjn;
}
//=======================================================
const vector<edge>& VectorGraph::star(const node n) const {
  assert(isElement(n));
  return _nData[n]._adje;
}
//=======================================================
const vector<node>& VectorGraph::nodes() const {
  return _nodes;
}
//=======================================================
const vector<edge>& VectorGraph::edges() const {
  return _edges;
}
//=======================================================
void VectorGraph::dump() const {
  tlp::debug() << "nodes : ";
  node n;
  forEach(n, getNodes())
  tlp::debug() << n.id << " ";
  tlp::debug() << endl;
  tlp::debug() << "edges: ";
  edge e;
  forEach(e, getEdges())
  tlp::debug() << "e_" << e.id << "(" << source(e).id << "," << target(e).id << ") ";
  tlp::debug() << endl;

  forEach(n, getNodes()) {
    tlp::debug() << "n_" << n << "{";
    edge e;
    forEach(e, getInOutEdges(n)) {
      tlp::debug() << "e_" << e.id << " ";
    }
    tlp::debug() << "}";
    tlp::debug() << endl;
  }
}
//=============================================================
void VectorGraph::integrityTest() {
  double sumDeg = 0;

  for (unsigned int i=0; i < numberOfNodes(); ++i)
    testCond("nodesId in array :" , _nData[_nodes[i]]._nodesId == i);

  for (unsigned int i=0; i < numberOfEdges(); ++i)
    testCond("edgesId in array :" , _eData[_edges[i]]._edgesId == i);

  set<edge> edgeFound;
  set<node> nodeFound;

  for (unsigned int i=0; i < numberOfNodes(); ++i) {
    testCond("edge adjn == node adje", _nData[_nodes[i]]._adjn.size() == _nData[_nodes[i]]._adje.size());
    testCond("edge adjn == node adjt", _nData[_nodes[i]]._adjn.size() == _nData[_nodes[i]]._adjt.size());
    unsigned int _indeg  = 0;
    unsigned int _outdeg = 0;
    node n = _nodes[i];
    nodeFound.insert(n);

    for (unsigned int j=0; j<_nData[n]._adjn.size(); ++j) {
      testCond("opposite", opposite(_nData[n]._adje[j], _nodes[i]) == _nData[n]._adjn[j]);

      if (!_nData[n]._adjt[j])
        ++_indeg;
      else
        ++_outdeg;

      edgeFound.insert(_nData[n]._adje[j]);
      nodeFound.insert(_nData[n]._adjn[j]);
    }

    testCond("_adjt in",  _indeg  == indeg(n));
    testCond("_adjt out", _outdeg == outdeg(n));

    sumDeg += _nData[_nodes[i]]._adjn.size();
    testCond("deg/in/out", _nData[n]._adjn.size() == outdeg(n) + indeg(n));
  }

  testCond("edges found", edgeFound.size() == _edges.size());
  testCond("nodes found", nodeFound.size() == _nodes.size());

  //edge extremities pos test
  for (unsigned int i=0; i < numberOfEdges(); ++i) {
    edge e = _edges[i];
    node src = source(e);
    node tgt = target(e);
    unsigned int srcp = _eData[e]._edgeExtremitiesPos.first;
    unsigned int tgtp = _eData[e]._edgeExtremitiesPos.second;
    testCond("p1 :", _nData[src]._adje[srcp] == e);
    testCond("p2 :", _nData[tgt]._adje[tgtp] == e);
    testCond("p3 :", _nData[src]._adjn[srcp] == tgt);
    testCond("p4 :", _nData[tgt]._adjn[tgtp] == src);
    testCond("p5 :", _nData[src]._adjt[srcp] == true);
    testCond("p6 :", _nData[tgt]._adjt[tgtp] == false);
  }

  testCond("Nb edges", sumDeg == (numberOfEdges() * 2));
}

//=======================================================
void VectorGraph::testCond(string str, bool b) {
  if (!b) {
    tlp::debug() << str << flush << endl;
    dump();
    exit(1);
  }
}
//=======================================================
void VectorGraph::addNodeToArray(node n) {
  set<ValArrayInterface *>::const_iterator it = _nodeArrays.begin();

  for(; it!= _nodeArrays.end(); ++it)
    (*it)->addElement(n.id);
}
//=======================================================
void VectorGraph::addEdgeToArray(edge e) {
  set<ValArrayInterface *>::const_iterator it = _edgeArrays.begin();

  for(; it!= _edgeArrays.end(); ++it)
    (*it)->addElement(e.id);
}
//=======================================================
void VectorGraph::removeEdge(edge e) {
  if (_eData[e]._edgesId == UINT_MAX) return;

  unsigned int nb_edges = _edges.size();
  unsigned int epos = _eData[e]._edgesId;

  if (epos != nb_edges - 1) {
    _edges[epos] = _edges[nb_edges - 1];
    _eData[_edges[epos]]._edgesId = epos;
  }

  _edges.resize(nb_edges - 1);
  _freeEdges.push_back(e);
  _eData[e]._edgesId = UINT_MAX;
}
//=======================================================
void VectorGraph::moveEdge(node n, unsigned int a, unsigned int b) {
  if (a == b) return;

  edge moved = _nData[n]._adje[a];

  if (_nData[n]._adjt[a]) //if true in edges -> target
    _eData[moved]._edgeExtremitiesPos.first  = b;
  else
    _eData[moved]._edgeExtremitiesPos.second = b;

  _nData[n]._adje[b]  = _nData[n]._adje[a];
  _nData[n]._adjn[b]  = _nData[n]._adjn[a];
  _nData[n]._adjt[b]  = _nData[n]._adjt[a];
}
//=======================================================
void VectorGraph::partialDelEdge(node n, edge e) {
  // e1 e2 e3 e4 e1 e6 e7 e7
  unsigned int endP = _nData[n]._adje.size() - 1;

  if (endP > 0) {
    bool loop = _eData[e]._edgeExtremities.first == _eData[e]._edgeExtremities.second;

    if (loop) {
      unsigned int i1 = std::max(_eData[e]._edgeExtremitiesPos.first, _eData[e]._edgeExtremitiesPos.second);
      unsigned int i2 = std::min(_eData[e]._edgeExtremitiesPos.first, _eData[e]._edgeExtremitiesPos.second);
      moveEdge(n, endP, i1);
      --endP;
      moveEdge(n, endP, i2);
    }
    else {
      unsigned int i;

      if (_eData[e]._edgeExtremities.first == n)
        i = _eData[e]._edgeExtremitiesPos.first;
      else
        i = _eData[e]._edgeExtremitiesPos.second;

      moveEdge(n, endP, i);
    }
  }

  _nData[n]._adje.resize(endP);
  _nData[n]._adjn.resize(endP);
  _nData[n]._adjt.resize(endP);
}
//=======================================================

}

