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

#include <vector>
#include <set>
#include <cassert>
#include <string>
#include <random>

#include <tulip/vectorgraph.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/StlIterator.h>

using namespace std;
using namespace tlp;

namespace tlp {
template struct TLP_SCOPE VectorGraphProperty<double>::ValuesImpl;
template struct TLP_SCOPE VectorGraphProperty<node>::ValuesImpl;
template class TLP_SCOPE NodeProperty<node>;
template class TLP_SCOPE NodeProperty<double>;
template class TLP_SCOPE EdgeProperty<node>;
template class TLP_SCOPE EdgeProperty<double>;
} // namespace tlp

namespace {
//==================================================================================
/**
 * \class inOutNodeIterator
 * \brief internal class to iterate on adj+/adj- of a VectorGraph node
 */
template <bool OUT_DIR>
class inOutNodeIterator : public Iterator<node>, public MemoryPool<inOutNodeIterator<OUT_DIR>> {
public:
  inOutNodeIterator(const std::vector<VectorGraph::adjData> &adj, unsigned int nb)
      : ita(adj.begin()), ita_end(adj.end()), _nb(nb) {
    goNext();
  }
  bool hasNext() override {
    return ita != ita_end;
  }
  node next() override {
    node n = ita->_n;
    ++ita;
    --_nb;
    goNext();

    return n;
  }

private:
  void goNext() {
    if (_nb == 0) {
      ita = ita_end;
      return;
    }

    while ((ita != ita_end) && (ita->_out != OUT_DIR)) {
      ++ita;
    }
  }

  std::vector<VectorGraph::adjData>::const_iterator ita, ita_end;
  unsigned int _nb;
};

//==================================================================================
/**
 * \class inOutEdgeIterator
 * \brief internal class to iterate on star+/star- of a VectorGraph node
 */
template <bool OUT_DIR>
class inOutEdgeIterator : public Iterator<edge>, public MemoryPool<inOutEdgeIterator<OUT_DIR>> {
public:
  inOutEdgeIterator(const std::vector<VectorGraph::adjData> &adj, unsigned int nb)
      : ita(adj.begin()), ita_end(adj.end()), _nb(nb) {
    goNext();
  }
  bool hasNext() override {
    return ita != ita_end;
  }
  edge next() override {
    edge e = ita->link();
    ++ita;
    --_nb;
    goNext();

    return e;
  }

private:
  void goNext() {
    if (_nb == 0) {
      ita = ita_end;
      return;
    }

    while ((ita != ita_end) && (ita->_out != OUT_DIR)) {
      ++ita;
    }
  }
  std::vector<VectorGraph::adjData>::const_iterator ita, ita_end;
  unsigned int _nb;
};

//==================================================================================
/**
 * \class allNodeIterator
 * \brief internal class to iterate on adjacent nodes of a VectorGraph node
 */
class allNodeIterator : public Iterator<node>, public MemoryPool<allNodeIterator> {
public:
  allNodeIterator(const std::vector<VectorGraph::adjData> &adj)
      : ita(adj.begin()), ita_end(adj.end()) {}
  bool hasNext() override {
    return ita != ita_end;
  }
  node next() override {
    node n = ita->_n;
    ++ita;

    return n;
  }

private:
  std::vector<VectorGraph::adjData>::const_iterator ita, ita_end;
};

//==================================================================================
/**
 * \class allEdgeIterator
 * \brief internal class to iterate on star of a VectorGraph node
 */
class allEdgeIterator : public Iterator<edge>, public MemoryPool<allEdgeIterator> {
public:
  allEdgeIterator(const std::vector<VectorGraph::adjData> &adj)
      : ita(adj.begin()), ita_end(adj.end()) {}
  bool hasNext() override {
    return ita != ita_end;
  }
  edge next() override {
    edge e = ita->link();
    ++ita;

    return e;
  }

private:
  std::vector<VectorGraph::adjData>::const_iterator ita, ita_end;
};
} // namespace

namespace tlp {
//===========================================
VectorGraph::VectorGraph() {}
//=======================================================
VectorGraph::~VectorGraph() {
  for (auto values : _nodeValues)
    delete values;

  for (auto values : _edgeValues)
    delete values;
}
//=======================================================
void VectorGraph::clear() {
  delAllNodes();
  for (auto values : _nodeValues)
    delete values;

  for (auto values : _edgeValues)
    delete values;

  _nodeValues.clear();
  _edgeValues.clear();
}
//=======================================================
edge VectorGraph::existEdge(node src, node tgt, bool directed) const {
  auto &srcData = _nData[src];
  auto &tgtData = _nData[tgt];
  unsigned int srcDeg = srcData._adj.size();
  unsigned int tgtDeg = tgtData._adj.size();

  auto data = srcData;
  if (tgtDeg < srcDeg) {
    data = tgtData;
    tgt = src;
    directed = !directed;
  }
  for (auto adj : data._adj) {
    if (adj.opposite() == tgt && (directed || !adj.isOut()))
      return adj.link();
  }

  return edge();
}
//=======================================================
void VectorGraph::setEdgeOrder(const node n, const std::vector<edge> &v) {
  //@TODO add assert that test edge element of v are correct
  for (size_t i = 0; i < v.size() - 1; ++i) {
    swapEdgeOrder(n, _nData[n]._adj[i].link(), v[i]);
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

  auto tmpa = _nData[n]._adj[e1i];
  _nData[n]._adj[e1i] = _nData[n]._adj[e2i];
  _nData[n]._adj[e2i] = tmpa;

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
  for (auto values : _nodeValues)
    values->reserve(nbNodes);
}
//=======================================================
void VectorGraph::reserveEdges(const size_t nbEdges) {
  _edges.reserve(nbEdges);
  _eData.reserve(nbEdges);
  for (auto values : _edgeValues)
    values->reserve(nbEdges);
}
//=======================================================
void VectorGraph::reserveAdj(const size_t nbEdges) {
  for (unsigned int i = 0; i < _nodes.size(); ++i) {
    reserveAdj(_nodes[i], nbEdges);
  }
}
//=======================================================
void VectorGraph::reserveAdj(const node n, const size_t nbEdges) {
  _nData[n]._adj.reserve(nbEdges);
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
  return new allEdgeIterator(_nData[n]._adj);
}
//=======================================================
Iterator<edge> *VectorGraph::getOutEdges(const node n) const {
  return new inOutEdgeIterator<true>(_nData[n]._adj, outdeg(n));
}
//=======================================================
Iterator<edge> *VectorGraph::getInEdges(const node n) const {
  return new inOutEdgeIterator<false>(_nData[n]._adj, indeg(n));
}
//=======================================================
Iterator<node> *VectorGraph::getInOutNodes(const node n) const {
  return new allNodeIterator(_nData[n]._adj);
}
//=======================================================
Iterator<node> *VectorGraph::getOutNodes(const node n) const {
  return new inOutNodeIterator<true>(_nData[n]._adj, outdeg(n));
}
//=======================================================
Iterator<node> *VectorGraph::getInNodes(const node n) const {
  return new inOutNodeIterator<false>(_nData[n]._adj, indeg(n));
}
//=======================================================
node VectorGraph::addNode() {
  node newNode(_nodes.get());

  if (newNode.id == _nData.size()) {
    _nData.emplace_back();
    addNodeToValues(newNode);
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
    addNodeToValues(node(_nodes.size() - 1));
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
  auto &eData = _eData[newEdge];
  eData._ends = std::make_pair(src, tgt);
  auto &srcData = _nData[src];
  eData._endsPos.first = srcData._adj.size();

  auto &tgtData = _nData[tgt];

  if (src != tgt)
    eData._endsPos.second = tgtData._adj.size();
  else
    eData._endsPos.second = srcData._adj.size() + 1;

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
    addEdgeToValues(newEdge);
  }

  addEdgeInternal(newEdge, src, tgt);

  // integrityTest();
  return newEdge;
}
//=======================================================
void VectorGraph::addEdges(const std::vector<std::pair<node, node>> &ends,
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
    addEdgeToValues(edge(_edges.size() - 1));
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
  auto &ndata = _nData[n];

  for (auto adj : ndata._adj) {
    edge e = adj.link();

    if (isElement(e)) {

      node opp = adj.opposite();

      if (opp != n) { // not a loop
        partialDelEdge(opp, e);

        if (adj.isOut() == false)
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

  auto &eData = _eData[e];
  node src = eData._ends.first;
  auto &srcData = _nData[src];
  srcData._outdeg -= 1;
  node tgt = eData._ends.second;
  auto &tgtData = _nData[tgt];
  tgtData._outdeg += 1;
  eData._ends.first = tgt;
  eData._ends.second = src;
  std::pair<unsigned int, unsigned int> &endsPos = eData._endsPos;
  unsigned int srcPos = endsPos.first;
  unsigned int tgtPos = endsPos.second;
  srcData._adj[srcPos]._out = false;
  tgtData._adj[tgtPos]._out = true;
  endsPos.first = tgtPos;
  endsPos.second = srcPos;
  // integrityTest();
}
//=======================================================
void VectorGraph::setEnds(const edge e, const node src, const node tgt) {
  assert(isElement(e));
  assert(isElement(src));
  assert(isElement(tgt));

  auto &eData = _eData[e];
  node psrc = eData._ends.first;
  node ptgt = eData._ends.second;

  _nData[psrc]._outdeg -= 1;
  auto &srcData = _nData[src];
  srcData._outdeg += 1;

  partialDelEdge(psrc, e);

  if (psrc != ptgt)
    partialDelEdge(ptgt, e);

  eData._ends = pair<node, node>(src, tgt);
  eData._endsPos.first = srcData._adj.size();
  auto &tgtData = _nData[tgt];

  if (src != tgt)
    eData._endsPos.second = tgtData._adj.size();
  else // loop
    eData._endsPos.second = srcData._adj.size() + 1;

  srcData.addEdge(true, tgt, e);
  tgtData.addEdge(false, src, e);

  // integrityTest();
}
//=======================================================
void VectorGraph::shuffleNodes() {
  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(_nodes.begin(), _nodes.end(), g);

  // recompute indices of nodes
  _nodes.reIndex();
}
//=======================================================
void VectorGraph::shuffleEdges() {
  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(_edges.begin(), _edges.end(), g);

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
  for (auto n : nodes())
    tlp::debug() << n.id << " ";
  tlp::debug() << endl;
  tlp::debug() << "edges: ";
  for (auto e : edges())
    tlp::debug() << "e_" << e.id << "(" << source(e).id << "," << target(e).id << ") ";
  tlp::debug() << endl;

  for (auto n : nodes()) {
    tlp::debug() << "n_" << n << "{";
    for (auto e : getInOutEdges(n)) {
      tlp::debug() << "e_" << e.id << " ";
    }
    tlp::debug() << "}";
    tlp::debug() << endl;
  }
}

//=============================================================
/**
 * internal function to break the program and output debug information during
 * integrity test.
 */
static void testCond(VectorGraph *v, const std::string &str, bool b) {
  if (!b) {
    tlp::debug() << str << flush << endl;
    v->dump();
    exit(1);
  }
}

//=============================================================
void VectorGraph::integrityTest() {
  double sumDeg = 0;

  for (unsigned int i = 0; i < numberOfNodes(); ++i)
    testCond(this, "nodesId in array :", _nodes.getPos(_nodes[i]) == i);

  for (unsigned int i = 0; i < numberOfEdges(); ++i)
    testCond(this, "edgesId in array :", _edges.getPos(_edges[i]) == i);

  set<edge> edgeFound;
  set<node> nodeFound;

  for (unsigned int i = 0; i < numberOfNodes(); ++i) {
    unsigned int _indeg = 0;
    unsigned int _outdeg = 0;
    node n = _nodes[i];
    nodeFound.insert(n);

    for (auto &adj : adj(n)) {
      testCond(this, "opposite", opposite(adj.link(), _nodes[i]) == adj.opposite());
      if (!adj.isOut())
        ++_indeg;
      else
        ++_outdeg;

      edgeFound.insert(adj.link());
      nodeFound.insert(adj.opposite());

      testCond(this, "_adj in", _indeg == indeg(n));
      testCond(this, "_adj out", _outdeg == outdeg(n));
    }

    sumDeg += _nData[_nodes[i]]._adj.size();
    testCond(this, "deg/in/out", _nData[n]._adj.size() == outdeg(n) + indeg(n));
  }

  testCond(this, "edges found", edgeFound.size() == _edges.size());
  testCond(this, "nodes found", nodeFound.size() == _nodes.size());

  // edge extremities pos test
  for (unsigned int i = 0; i < numberOfEdges(); ++i) {
    edge e = _edges[i];
    node src = source(e);
    node tgt = target(e);
    unsigned int srcp = _eData[e]._endsPos.first;
    unsigned int tgtp = _eData[e]._endsPos.second;
    testCond(this, "p1 :", _nData[src]._adj[srcp].link() == e);
    testCond(this, "p2 :", _nData[tgt]._adj[tgtp].link() == e);
    testCond(this, "p3 :", _nData[src]._adj[srcp].opposite() == tgt);
    testCond(this, "p4 :", _nData[tgt]._adj[tgtp].opposite() == src);
    testCond(this, "p5 :", _nData[src]._adj[srcp].isOut() == true);
    testCond(this, "p6 :", _nData[tgt]._adj[tgtp].isOut() == false);
  }

  testCond(this, "Nb edges", sumDeg == (numberOfEdges() * 2));
}

//=======================================================
void VectorGraph::addNodeToValues(node n) {
  for (auto values : _nodeValues)
    values->addElement(n.id);
}
//=======================================================
void VectorGraph::addEdgeToValues(edge e) {
  for (auto values : _edgeValues)
    values->addElement(e.id);
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

  auto &adj = _nData[n]._adj[a];
  edge moved = adj.link();

  if (adj.isOut()) // if true in edges -> target
    _eData[moved]._endsPos.first = b;
  else
    _eData[moved]._endsPos.second = b;

  _nData[n]._adj[b] = adj;
}
//=======================================================
void VectorGraph::partialDelEdge(node n, edge e) {
  // e1 e2 e3 e4 e1 e6 e7 e7
  unsigned int endP = _nData[n]._adj.size() - 1;

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

  _nData[n]._adj.resize(endP);
}
//=======================================================
} // namespace tlp
