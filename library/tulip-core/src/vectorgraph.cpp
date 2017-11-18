/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
  bInOutNodeIterator(const std::vector<node> &nodes, const std::vector<bool> &sens,
                     unsigned int elem)
      : itn(nodes.begin()), itn_end(nodes.end()), its(sens.begin()), its_end(sens.end()),
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

    while ((its != its_end) && ((*its) != SENS)) {
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
  bInOutEdgeIterator(const std::vector<edge> &edges, const std::vector<bool> &sens,
                     unsigned int elem)
      : ite(edges.begin()), ite_end(edges.end()), its(sens.begin()), its_end(sens.end()),
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

    while ((its != its_end) && ((*its) != SENS)) {
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
VectorGraph::VectorGraph() {}
//=======================================================
VectorGraph::~VectorGraph() {
  set<ValArrayInterface *>::const_iterator it;

  for (it = _nodeArrays.begin(); it != _nodeArrays.end(); ++it)
    delete (*it);

  for (it = _edgeArrays.begin(); it != _edgeArrays.end(); ++it)
    delete (*it);
}
//=======================================================
void VectorGraph::clear() {
  delAllNodes();
  set<ValArrayInterface *>::const_iterator it;

  for (it = _nodeArrays.begin(); it != _nodeArrays.end(); ++it)
    delete (*it);

  for (it = _edgeArrays.begin(); it != _edgeArrays.end(); ++it)
    delete (*it);

  _nodeArrays.clear();
  _edgeArrays.clear();
}
//=======================================================
edge VectorGraph::existEdge(const node src, const node tgt, bool directed) const {
  const _iNodes &srcData = _nData[src];
  const _iNodes &tgtData = _nData[tgt];
  unsigned int srcDeg = srcData._adje.size();
  unsigned int tgtDeg = tgtData._adje.size();

  if (tgtDeg < srcDeg) {
    const std::vector<bool> &adjt = tgtData._adjt;
    const std::vector<node> &adjn = tgtData._adjn;

    for (size_t i = 0; i < tgtDeg; ++i) {
      if (directed && !adjt[i] && adjn[i] == src)
        return tgtData._adje[i];

      if (!directed && adjn[i] == src)
        return tgtData._adje[i];
    }
  } else {
    const std::vector<bool> &adjt = srcData._adjt;
    const std::vector<node> &adjn = srcData._adjn;

    for (size_t i = 0; i < srcDeg; ++i) {
      if (directed && adjt[i] && adjn[i] == tgt)
        return srcData._adje[i];

      if (!directed && adjn[i] == tgt)
        return srcData._adje[i];
    }
  }

  return edge();
}
//=======================================================
void VectorGraph::setEdgeOrder(const node n, const std::vector<edge> &v) {
  //@TODO add assert that test edge element of v are correct
  for (size_t i = 0; i < v.size() - 1; ++i) {
    swapEdgeOrder(n, _nData[n]._adje[i], v[i]);
  }

  //        integrityTest();
}
//=======================================================
void VectorGraph::swapEdgeOrder(const node n, const edge e1, const edge e2) {
  if (e1 == e2)
    return; // we can't swap loops in our model

  unsigned int e1i, e2i;

  if (source(e1) == n)
    e1i = _eData[e1]._endsPos.first;
  else
    e1i = _eData[e1]._endsPos.second;

  if (source(e2) == n)
    e2i = _eData[e2]._endsPos.first;
  else
    e2i = _eData[e2]._endsPos.second;

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
    _eData[e1]._endsPos.first = e2i;
  else
    _eData[e1]._endsPos.second = e2i;

  if (source(e2) == n)
    _eData[e2]._endsPos.first = e1i;
  else
    _eData[e2]._endsPos.second = e1i;
}
//=======================================================
void VectorGraph::reserveNodes(const size_t nbNodes) {
  _nodes.reserve(nbNodes);
  _nData.reserve(nbNodes);
  set<ValArrayInterface *>::const_iterator it;

  for (it = _nodeArrays.begin(); it != _nodeArrays.end(); ++it)
    (*it)->reserve(nbNodes);
}
//=======================================================
void VectorGraph::reserveEdges(const size_t nbEdges) {
  _edges.reserve(nbEdges);
  _eData.reserve(nbEdges);
  set<ValArrayInterface *>::const_iterator it;

  for (it = _edgeArrays.begin(); it != _edgeArrays.end(); ++it)
    (*it)->reserve(nbEdges);
}
//=======================================================
void VectorGraph::reserveAdj(const size_t nbEdges) {
  for (unsigned int i = 0; i < _nodes.size(); ++i) {
    reserveAdj(_nodes[i], nbEdges);
  }
}
//=======================================================
void VectorGraph::reserveAdj(const node n, const size_t nbEdges) {
  _nData[n]._adjt.reserve(nbEdges);
  _nData[n]._adje.reserve(nbEdges);
  _nData[n]._adjn.reserve(nbEdges);
}
//=======================================================
Iterator<node> *VectorGraph::getNodes() const {
  return new MPStlIterator<node, vector<node>::const_iterator>(_nodes.begin(), _nodes.end());
}
//=======================================================
Iterator<edge> *VectorGraph::getEdges() const {
  return new MPStlIterator<edge, vector<edge>::const_iterator>(_edges.begin(), _edges.end());
}
//=======================================================
Iterator<edge> *VectorGraph::getInOutEdges(const node n) const {
  return new MPStlIterator<edge, vector<edge>::const_iterator>(_nData[n]._adje.begin(),
                                                               _nData[n]._adje.end());
}
//=======================================================
Iterator<edge> *VectorGraph::getOutEdges(const node n) const {
  return new bInOutEdgeIterator<true>(_nData[n]._adje, _nData[n]._adjt, outdeg(n));
}
//=======================================================
Iterator<edge> *VectorGraph::getInEdges(const node n) const {
  return new bInOutEdgeIterator<false>(_nData[n]._adje, _nData[n]._adjt, indeg(n));
}
//=======================================================
Iterator<node> *VectorGraph::getInOutNodes(const node n) const {
  return new MPStlIterator<node, vector<node>::const_iterator>(_nData[n]._adjn.begin(),
                                                               _nData[n]._adjn.end());
}
//=======================================================
Iterator<node> *VectorGraph::getInNodes(const node n) const {
  return new bInOutNodeIterator<false>(_nData[n]._adjn, _nData[n]._adjt, indeg(n));
}
//=======================================================
Iterator<node> *VectorGraph::getOutNodes(const node n) const {
  return new bInOutNodeIterator<true>(_nData[n]._adjn, _nData[n]._adjt, outdeg(n));
}
//=======================================================
node VectorGraph::addNode() {
  node newNode(_nodes.get());

  if (newNode.id == _nData.size()) {
    _nData.push_back(_iNodes());
    addNodeToArray(newNode);
  } else
    _nData[newNode].clear();

  return newNode;
}
//=======================================================
void VectorGraph::addNodes(unsigned int nb, std::vector<node> *addedNodes) {
  if (addedNodes) {
    addedNodes->clear();
    addedNodes->reserve(nb);
  }

  unsigned int first = _nodes.getFirstOfRange(nb);

  if (addedNodes) {
    addedNodes->resize(nb);
    memcpy(addedNodes->data(), &_nodes[first], nb * sizeof(node));
  }

  unsigned int sz = _nData.size();

  if (sz < _nodes.size()) {
    _nData.resize(_nodes.size());
    addNodeToArray(node(_nodes.size() - 1));
    // get the number of recycled nodes
    // that need to be cleared
    nb -= _nodes.size() - sz;
  }

  for (unsigned int i = 0; i < nb; ++i)
    _nData[_nodes[first + i]].clear();
}
//=======================================================
void VectorGraph::delNode(const node n) {
  assert(isElement(n));
  delEdges(n);
  _nData[n].clear();
  _nodes.free(n);

  if (_nodes.empty())
    _nData.resize(0);

  // integrityTest();
}
//=======================================================
void VectorGraph::addEdgeInternal(const edge newEdge, const node src, const node tgt) {
  _iEdges &eData = _eData[newEdge];
  eData._ends = std::make_pair(src, tgt);
  _iNodes &srcData = _nData[src];
  eData._endsPos.first = srcData._adje.size();

  _iNodes &tgtData = _nData[tgt];

  if (src != tgt)
    eData._endsPos.second = tgtData._adje.size();
  else
    eData._endsPos.second = srcData._adje.size() + 1;

  srcData.addEdge(true, tgt, newEdge);
  tgtData.addEdge(false, src, newEdge);

  srcData._outdeg += 1;
}
//=======================================================
edge VectorGraph::addEdge(const node src, const node tgt) {
  assert(isElement(src));
  assert(isElement(tgt));
  edge newEdge(_edges.get());

  if (newEdge.id == _eData.size()) {
    _eData.resize(newEdge.id + 1);
    addEdgeToArray(newEdge);
  }

  addEdgeInternal(newEdge, src, tgt);

  // integrityTest();
  return newEdge;
}
//=======================================================
void VectorGraph::addEdges(const std::vector<std::pair<node, node> > &ends,
                           std::vector<edge> *addedEdges) {
  unsigned int nb = ends.size();

  if (nb == 0)
    return;

  if (addedEdges) {
    addedEdges->clear();
    addedEdges->reserve(nb);
  }

  unsigned int first = _edges.getFirstOfRange(nb);

  if (addedEdges) {
    addedEdges->resize(nb);
    memcpy(addedEdges->data(), &_edges[first], nb * sizeof(edge));
  }

  unsigned int sz = _eData.size();

  if (sz < _edges.size()) {
    _eData.resize(_edges.size());
    addEdgeToArray(edge(_edges.size() - 1));
  }

  for (unsigned int i = 0; i < nb; ++i)
    addEdgeInternal(_edges[first + i], ends[i].first, ends[i].second);
}

//=======================================================
void VectorGraph::delEdge(const edge e) {
  assert(isElement(e));
  node psrc = _eData[e]._ends.first;
  _nData[psrc]._outdeg -= 1;
  node ptgt = _eData[e]._ends.second;
  partialDelEdge(psrc, e);

  if (psrc != ptgt)
    partialDelEdge(ptgt, e);

  // remove the edge in the edge vector
  removeEdge(e);
  // integrityTest();
}
//=======================================================
void VectorGraph::delEdges(const node n) {
  assert(isElement(n));
  _iNodes &ndata = _nData[n];
  const vector<edge> &adje = ndata._adje;
  const vector<node> &adjn = ndata._adjn;
  const vector<bool> &adjt = ndata._adjt;

  for (unsigned int i = 0; i < adje.size(); ++i) {
    edge e = adje[i];

    if (isElement(e)) {

      node opp = adjn[i];

      if (opp != n) { // not a loop
        partialDelEdge(opp, e);

        if (adjt[i] == false)
          _nData[opp]._outdeg -= 1;
      }

      removeEdge(e);
    }
  }

  ndata.clear();
}
//=======================================================
void VectorGraph::delAllEdges() {
  _edges.clear();
  _eData.clear();

  for (size_t i = 0; i < _nodes.size(); ++i) {
    _nData[_nodes[i]].clear();
  }
}
//=======================================================
void VectorGraph::delAllNodes() {
  _edges.clear();
  _eData.clear();
  _nodes.clear();
  _nData.clear();
}
//=======================================================
node VectorGraph::opposite(const edge e, const node n) const {
  assert(isElement(e));
  assert(isElement(n));

  const pair<node, node> &endsE = _eData[e]._ends;

  if (endsE.first == n)
    return endsE.second;
  else {
    assert(endsE.second == n);
    return endsE.first;
  }
}
//=======================================================
void VectorGraph::reverse(const edge e) {
  assert(isElement(e));

  _iEdges &eData = _eData[e];
  node src = eData._ends.first;
  _iNodes &srcData = _nData[src];
  srcData._outdeg -= 1;
  node tgt = eData._ends.second;
  _iNodes &tgtData = _nData[tgt];
  tgtData._outdeg += 1;
  eData._ends.first = tgt;
  eData._ends.second = src;
  std::pair<unsigned int, unsigned int> &endsPos = eData._endsPos;
  unsigned int srcPos = endsPos.first;
  unsigned int tgtPos = endsPos.second;
  srcData._adjt[srcPos] = false;
  tgtData._adjt[tgtPos] = true;
  endsPos.first = tgtPos;
  endsPos.second = srcPos;
  // integrityTest();
}
//=======================================================
void VectorGraph::setEnds(const edge e, const node src, const node tgt) {
  assert(isElement(e));
  assert(isElement(src));
  assert(isElement(tgt));

  _iEdges &eData = _eData[e];
  node psrc = eData._ends.first;
  node ptgt = eData._ends.second;

  _nData[psrc]._outdeg -= 1;
  _iNodes &srcData = _nData[src];
  srcData._outdeg += 1;

  partialDelEdge(psrc, e);

  if (psrc != ptgt)
    partialDelEdge(ptgt, e);

  eData._ends = pair<node, node>(src, tgt);
  eData._endsPos.first = srcData._adje.size();
  _iNodes &tgtData = _nData[tgt];

  if (src != tgt)
    eData._endsPos.second = tgtData._adje.size();
  else // loop
    eData._endsPos.second = srcData._adje.size() + 1;

  srcData.addEdge(true, tgt, e);
  tgtData.addEdge(false, src, e);

  // integrityTest();
}
//=======================================================
void VectorGraph::shuffleNodes() {
  random_shuffle(_nodes.begin(), _nodes.end());

  // recompute indices of nodes
  _nodes.reIndex();
}
//=======================================================
void VectorGraph::shuffleEdges() {
  random_shuffle(_edges.begin(), _edges.end());

  // recompute indices of edges
  _edges.reIndex();
}
//=======================================================
void VectorGraph::swap(const node a, const node b) {
  _nodes.swap(a, b);
}
//=======================================================
void VectorGraph::swap(const edge a, const edge b) {
  _edges.swap(a, b);
}
//=======================================================
void VectorGraph::dump() const {
  tlp::debug() << "nodes : ";
  node n;
  forEach(n, getNodes()) tlp::debug() << n.id << " ";
  tlp::debug() << endl;
  tlp::debug() << "edges: ";
  edge e;
  forEach(e, getEdges()) tlp::debug() << "e_" << e.id << "(" << source(e).id << "," << target(e).id
                                      << ") ";
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

  for (unsigned int i = 0; i < numberOfNodes(); ++i)
    testCond("nodesId in array :", _nodes.getPos(_nodes[i]) == i);

  for (unsigned int i = 0; i < numberOfEdges(); ++i)
    testCond("edgesId in array :", _edges.getPos(_edges[i]) == i);

  set<edge> edgeFound;
  set<node> nodeFound;

  for (unsigned int i = 0; i < numberOfNodes(); ++i) {
    testCond("edge adjn == node adje",
             _nData[_nodes[i]]._adjn.size() == _nData[_nodes[i]]._adje.size());
    testCond("edge adjn == node adjt",
             _nData[_nodes[i]]._adjn.size() == _nData[_nodes[i]]._adjt.size());
    unsigned int _indeg = 0;
    unsigned int _outdeg = 0;
    node n = _nodes[i];
    nodeFound.insert(n);

    for (unsigned int j = 0; j < _nData[n]._adjn.size(); ++j) {
      testCond("opposite", opposite(_nData[n]._adje[j], _nodes[i]) == _nData[n]._adjn[j]);

      if (!_nData[n]._adjt[j])
        ++_indeg;
      else
        ++_outdeg;

      edgeFound.insert(_nData[n]._adje[j]);
      nodeFound.insert(_nData[n]._adjn[j]);
    }

    testCond("_adjt in", _indeg == indeg(n));
    testCond("_adjt out", _outdeg == outdeg(n));

    sumDeg += _nData[_nodes[i]]._adjn.size();
    testCond("deg/in/out", _nData[n]._adjn.size() == outdeg(n) + indeg(n));
  }

  testCond("edges found", edgeFound.size() == _edges.size());
  testCond("nodes found", nodeFound.size() == _nodes.size());

  // edge extremities pos test
  for (unsigned int i = 0; i < numberOfEdges(); ++i) {
    edge e = _edges[i];
    node src = source(e);
    node tgt = target(e);
    unsigned int srcp = _eData[e]._endsPos.first;
    unsigned int tgtp = _eData[e]._endsPos.second;
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

  for (; it != _nodeArrays.end(); ++it)
    (*it)->addElement(n.id);
}
//=======================================================
void VectorGraph::addEdgeToArray(edge e) {
  set<ValArrayInterface *>::const_iterator it = _edgeArrays.begin();

  for (; it != _edgeArrays.end(); ++it)
    (*it)->addElement(e.id);
}
//=======================================================
void VectorGraph::removeEdge(edge e) {
  if (_edges.isElement(e)) {
    _edges.free(e);

    if (_edges.empty())
      _eData.resize(0);
  }
}
//=======================================================
void VectorGraph::moveEdge(node n, unsigned int a, unsigned int b) {
  if (a == b)
    return;

  edge moved = _nData[n]._adje[a];

  if (_nData[n]._adjt[a]) // if true in edges -> target
    _eData[moved]._endsPos.first = b;
  else
    _eData[moved]._endsPos.second = b;

  _nData[n]._adje[b] = _nData[n]._adje[a];
  _nData[n]._adjn[b] = _nData[n]._adjn[a];
  _nData[n]._adjt[b] = _nData[n]._adjt[a];
}
//=======================================================
void VectorGraph::partialDelEdge(node n, edge e) {
  // e1 e2 e3 e4 e1 e6 e7 e7
  unsigned int endP = _nData[n]._adje.size() - 1;

  if (endP > 0) {
    bool loop = _eData[e]._ends.first == _eData[e]._ends.second;

    if (loop) {
      unsigned int i1 = std::max(_eData[e]._endsPos.first, _eData[e]._endsPos.second);
      unsigned int i2 = std::min(_eData[e]._endsPos.first, _eData[e]._endsPos.second);
      moveEdge(n, endP, i1);
      --endP;
      moveEdge(n, endP, i2);
    } else {
      unsigned int i;

      if (_eData[e]._ends.first == n)
        i = _eData[e]._endsPos.first;
      else
        i = _eData[e]._endsPos.second;

      moveEdge(n, endP, i);
    }
  }

  _nData[n]._adje.resize(endP);
  _nData[n]._adjn.resize(endP);
  _nData[n]._adjt.resize(endP);
}
//=======================================================
}
