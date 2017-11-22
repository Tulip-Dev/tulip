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
#include <stack>

#include <tulip/StableIterator.h>
#include <tulip/ForEach.h>
#include <tulip/BooleanProperty.h>
#include <tulip/Graph.h>
#include <tulip/GraphIterator.h>
#include <tulip/GraphView.h>
#include <tulip/PropertyManager.h>

using namespace std;
namespace tlp {
//----------------------------------------------------------------
GraphView::GraphView(Graph *supergraph, BooleanProperty *filter, unsigned int sgId)
    : GraphAbstract(supergraph, sgId) {
  _nodeData.setAll(nullptr);

  if (filter == nullptr)
    return;

  if ((filter->getGraph() == supergraph) && (filter->getNodeDefaultValue() == true) &&
      (filter->numberOfNonDefaultValuatedNodes() == 0)) {
    // clone all supergraph nodes
    _nodes.clone(supergraph->nodes());
    unsigned int nbNodes = _nodes.size();

    for (unsigned int i = 0; i < nbNodes; ++i)
      _nodeData.set(_nodes[i], new SGraphNodeData());
  } else {
    Iterator<unsigned int> *it = nullptr;
    it = filter->nodeProperties.findAll(true);

    Iterator<node> *iteN;

    if (it == nullptr) {
      Graph *graphToFilter = filter->getGraph();

      if (graphToFilter == nullptr)
        graphToFilter = supergraph;

      iteN = graphToFilter->getNodes();
    } else
      iteN = new UINTIterator<node>(it);

    while (iteN->hasNext()) {
      node n = iteN->next();

      if (filter->getNodeValue(n))
        addNode(n);
    }

    delete iteN;
  }

  if ((filter->getGraph() == supergraph) && (filter->getEdgeDefaultValue() == true) &&
      (filter->numberOfNonDefaultValuatedEdges() == 0)) {
    // clone all supergraph edges
    _edges.clone(supergraph->edges());
    // and degrees of nodes
    unsigned int nbNodes = _nodes.size();

    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = _nodes[i];
      SGraphNodeData *nData = _nodeData.get(n.id);
      nData->outDegree = supergraph->outdeg(n);
      nData->inDegree = supergraph->indeg(n);
    }
  } else {
    Iterator<unsigned int> *it = nullptr;
    it = filter->edgeProperties.findAll(true);

    Iterator<edge> *iteE;

    if (it == nullptr) {
      Graph *graphToFilter = filter->getGraph();

      if (graphToFilter == nullptr)
        graphToFilter = supergraph;

      iteE = graphToFilter->getEdges();
    } else
      iteE = new UINTIterator<edge>(it);

    while (iteE->hasNext()) {
      edge e = iteE->next();

      if (filter->getEdgeValue(e))
        addEdge(e);
    }

    delete iteE;
  }
}
//----------------------------------------------------------------
GraphView::~GraphView() {
  // notify destruction
  observableDeleted();
}
//----------------------------------------------------------------
edge GraphView::existEdge(const node src, const node tgt, bool directed) const {
  if (!isElement(src) || !isElement(tgt))
    return edge();

  std::vector<edge> ee;

  return getRootImpl()->getEdges(src, tgt, directed, ee, this, true) ? ee[0] : edge();
}
//----------------------------------------------------------------
void GraphView::reverseInternal(const edge e, const node src, const node tgt) {
  if (isElement(e)) {
    SGraphNodeData *srcData = _nodeData.get(src.id);
    SGraphNodeData *tgtData = _nodeData.get(tgt.id);
    srcData->outDegreeAdd(-1);
    srcData->inDegreeAdd(1);
    tgtData->inDegreeAdd(-1);
    tgtData->outDegreeAdd(1);

    notifyReverseEdge(e);

    // propagate edge reversal on subgraphs
    Graph *sg;
    forEach(sg, getSubGraphs()) {
      static_cast<GraphView *>(sg)->reverseInternal(e, src, tgt);
    }
  }
}
//----------------------------------------------------------------
void GraphView::setEndsInternal(const edge e, node src, node tgt, const node newSrc,
                                const node newTgt) {
  if (isElement(e)) {
    if (isElement(newSrc) && isElement(newTgt)) {
      notifyBeforeSetEnds(e);

      if (src != newSrc) {
        _nodeData.get(newSrc.id)->outDegreeAdd(1);

        if (src.isValid() && isElement(src))
          _nodeData.get(src.id)->outDegreeAdd(-1);
        else
          // as src may no longer exist (pop case)
          // set src as invalid for subgraphs loop
          src = node();
      }

      if (tgt != newTgt) {
        _nodeData.get(newTgt.id)->inDegreeAdd(1);

        if (tgt.isValid() && isElement(tgt))
          _nodeData.get(tgt.id)->inDegreeAdd(-1);
        else
          // as tgt may no longer exist (pop case)
          // set tgt as invalid for subgraphs loop
          tgt = node();
      }

      // notification
      notifyAfterSetEnds(e);

      // propagate edge ends update on subgraphs
      Graph *sg;
      forEach(sg, getSubGraphs()) {
        static_cast<GraphView *>(sg)->setEndsInternal(e, src, tgt, newSrc, newTgt);
      }
    } else {
      // delete e if its new ends do no belong to the graph
      // propagate edge ends update on subgraphs
      Graph *sg;
      forEach(sg, getSubGraphs()) {
        static_cast<GraphView *>(sg)->setEndsInternal(e, src, tgt, newSrc, newTgt);
      }
      notifyDelEdge(e);

      _edges.remove(e);
      propertyContainer->erase(e);
      _nodeData.get(src.id)->outDegreeAdd(-1);
      _nodeData.get(tgt.id)->inDegreeAdd(-1);
    }
  }
}
//----------------------------------------------------------------
node GraphView::addNode() {
  node tmp = getSuperGraph()->addNode();
  restoreNode(tmp);
  return tmp;
}
//----------------------------------------------------------------
void GraphView::addNodes(unsigned int nb) {
  getSuperGraph()->addNodes(nb);
  addNodesInternal(nb, nullptr);
}
//----------------------------------------------------------------
void GraphView::addNodes(unsigned int nb, std::vector<node> &addedNodes) {
  getSuperGraph()->addNodes(nb, addedNodes);
  addNodesInternal(nb, &addedNodes);
}
//----------------------------------------------------------------
void GraphView::restoreNode(node n) {
  _nodeData.set(n.id, new SGraphNodeData());
  _nodes.add(n);
  notifyAddNode(n);
}
//----------------------------------------------------------------
void GraphView::addNodesInternal(unsigned int nbAdded, const std::vector<node> *nodes) {
  _nodes.reserve(_nodes.size() + nbAdded);

  std::vector<node>::const_iterator it;

  if (nodes)
    it = nodes->begin();
  else {
    nodes = &getSuperGraph()->nodes();
    it = nodes->begin() + nodes->size() - nbAdded;
  }

  std::vector<node>::const_iterator ite = nodes->end();

  for (; it != ite; ++it) {
    node n(*it);
    assert(getRootImpl()->isElement(n));
    _nodeData.set(n.id, new SGraphNodeData());
    _nodes.add(n);
  }

  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, nbAdded));
}
//----------------------------------------------------------------
void GraphView::addNode(const node n) {
  assert(getRoot()->isElement(n));

  if (!isElement(n)) {
    if (!getSuperGraph()->isElement(n))
      getSuperGraph()->addNode(n);

    restoreNode(n);
  }
}
//----------------------------------------------------------------
void GraphView::addNodes(Iterator<node> *addedNodes) {
  std::vector<node> nodes;
  std::vector<node> superNodes;
  Graph *super = getSuperGraph();
  bool superIsRoot = (super == getRoot());

  while (addedNodes->hasNext()) {
    node n = addedNodes->next();

    if (!isElement(n)) {
      nodes.push_back(n);

      if (!superIsRoot && !super->isElement(n))
        superNodes.push_back(n);
    }
  }

  if (!superNodes.empty()) {
    StlIterator<node, std::vector<node>::iterator> it(superNodes.begin(), superNodes.end());
    super->addNodes(&it);
  }

  if (!nodes.empty())
    addNodesInternal(nodes.size(), &nodes);
}
//----------------------------------------------------------------
edge GraphView::addEdgeInternal(edge e) {
  _edges.add(e);
  const std::pair<node, node> &eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  _nodeData.get(src.id)->outDegreeAdd(1);
  _nodeData.get(tgt.id)->inDegreeAdd(1);
  notifyAddEdge(e);
  return e;
}
//----------------------------------------------------------------
void GraphView::restoreEdge(edge e, const node, const node) {
  addEdgeInternal(e);
}
//----------------------------------------------------------------
void GraphView::addEdgesInternal(unsigned int nbAdded, const std::vector<edge> *ee,
                                 const std::vector<std::pair<node, node>> &ends) {
  _edges.reserve(_edges.size() + nbAdded);

  bool hasEnds = !ends.empty();

  unsigned int i = 0;
  std::vector<edge>::const_iterator it;

  if (ee)
    it = ee->begin();
  else {
    ee = &getSuperGraph()->edges();
    it = ee->begin() + ee->size() - nbAdded;
  }

  std::vector<edge>::const_iterator ite = ee->end();

  for (; it != ite; ++it, ++i) {
    edge e = *it;
    assert(getRootImpl()->isElement(e));
    _edges.add(e);
    std::pair<node, node> eEnds(hasEnds ? ends[i] : this->ends(e));
    node src = eEnds.first;
    node tgt = eEnds.second;
    _nodeData.get(src.id)->outDegreeAdd(1);
    _nodeData.get(tgt.id)->inDegreeAdd(1);
  }

  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, nbAdded));
}
//----------------------------------------------------------------
edge GraphView::addEdge(const node n1, const node n2) {
  assert(isElement(n1));
  assert(isElement(n2));
  return addEdgeInternal(getSuperGraph()->addEdge(n1, n2));
}
//----------------------------------------------------------------
void GraphView::addEdge(const edge e) {
  assert(getRootImpl()->isElement(e));
  assert(isElement(source(e)));
  assert(isElement(target(e)));

  if (!isElement(e)) {
    if (!getSuperGraph()->isElement(e))
      getSuperGraph()->addEdge(e);

    addEdgeInternal(e);
  }
}
//----------------------------------------------------------------
void GraphView::addEdges(const std::vector<std::pair<node, node>> &ends) {
  getSuperGraph()->addEdges(ends);
  addEdgesInternal(ends.size(), nullptr, ends);
}
//----------------------------------------------------------------
void GraphView::addEdges(const std::vector<std::pair<node, node>> &ends,
                         std::vector<edge> &addedEdges) {
  getSuperGraph()->addEdges(ends, addedEdges);
  addEdgesInternal(ends.size(), &addedEdges, ends);
}
//----------------------------------------------------------------
void GraphView::addEdges(Iterator<edge> *addedEdges) {
  std::vector<edge> ee;
  std::vector<edge> superEdges;
  Graph *super = getSuperGraph();
  bool superIsRoot = (super == getRoot());

  while (addedEdges->hasNext()) {
    edge e = addedEdges->next();
    assert(getRootImpl()->isElement(e));
    assert(isElement(source(e)));
    assert(isElement(target(e)));

    if (!isElement(e)) {
      ee.push_back(e);

      if (!superIsRoot && !super->isElement(e))
        superEdges.push_back(e);
    }
  }

  if (!superEdges.empty()) {
    StlIterator<edge, std::vector<edge>::iterator> it(superEdges.begin(), superEdges.end());
    super->addEdges(&it);
  }

  if (!ee.empty())
    addEdgesInternal(ee.size(), &ee, std::vector<pair<node, node>>());
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n) {
  assert(isElement(n));
  notifyDelNode(n);
  _nodeData.set(n.id, nullptr);
  _nodes.remove(n);
  propertyContainer->erase(n);
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n, const std::vector<edge> &ee) {
  removeEdges(ee);
  removeNode(n);
}
//----------------------------------------------------------------
void GraphView::delNode(const node n, bool deleteInAllGraphs) {
  if (deleteInAllGraphs) {
    getRootImpl()->delNode(n, true);
  } else {
    assert(isElement(n));

    // get edges vector
    std::vector<edge> ee(this->allEdges(n));

    // use a stack for a dfs subgraphs propagation
    std::stack<Graph *> sgq;
    Iterator<Graph *> *sgs = getSubGraphs();

    while (sgs->hasNext()) {
      Graph *sg = sgs->next();

      if (sg->isElement(n))
        sgq.push(sg);
    }

    delete sgs;

    // subgraphs loop
    while (!sgq.empty()) {
      Graph *sg = sgq.top();

      sgs = sg->getSubGraphs();

      while (sgs->hasNext()) {
        Graph *ssg = sgs->next();

        if (ssg->isElement(n))
          sgq.push(ssg);
      }

      delete sgs;

      if (sg == sgq.top()) {
        static_cast<GraphView *>(sg)->removeNode(n, ee);
        sgq.pop();
      }
    }

    removeNode(n, ee);
  }
}
//----------------------------------------------------------------
void GraphView::removeEdge(const edge e) {
  assert(isElement(e));
  notifyDelEdge(e);

  _edges.remove(e);
  propertyContainer->erase(e);
  const std::pair<node, node> &eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  _nodeData.get(src.id)->outDegreeAdd(-1);
  _nodeData.get(tgt.id)->inDegreeAdd(-1);
}
//----------------------------------------------------------------
void GraphView::removeEdges(const std::vector<edge> &ee) {
  std::vector<edge>::const_iterator ite = ee.begin();

  while (ite != ee.end()) {
    edge e = (*ite);

    if (isElement(e))
      removeEdge(e);

    ++ite;
  }
}
//----------------------------------------------------------------
void GraphView::delEdge(const edge e, bool deleteInAllGraphs) {
  if (deleteInAllGraphs) {
    getRootImpl()->delEdge(e, true);
  } else {
    assert(isElement(e));
    // propagate to subgraphs
    Iterator<Graph *> *itS = getSubGraphs();

    while (itS->hasNext()) {
      Graph *subGraph = itS->next();

      if (subGraph->isElement(e))
        subGraph->delEdge(e);
    }

    delete itS;
    removeEdge(e);
  }
}
//----------------------------------------------------------------
Iterator<node> *GraphView::getNodes() const {
  return new GraphNodeIterator(
      this, new StlIterator<node, std::vector<node>::const_iterator>(_nodes.begin(), _nodes.end()));
}
//----------------------------------------------------------------
Iterator<node> *GraphView::getInNodes(const node n) const {
  return (new InNodesIterator(this, n));
}
//----------------------------------------------------------------
Iterator<node> *GraphView::getOutNodes(const node n) const {
  return (new OutNodesIterator(this, n));
}
//----------------------------------------------------------------
Iterator<node> *GraphView::getInOutNodes(const node n) const {
  return (new InOutNodesIterator(this, n));
}
//----------------------------------------------------------------
Iterator<edge> *GraphView::getEdges() const {
  return new GraphEdgeIterator(
      this, new StlIterator<edge, std::vector<edge>::const_iterator>(_edges.begin(), _edges.end()));
}
//----------------------------------------------------------------
Iterator<edge> *GraphView::getInEdges(const node n) const {
  return (new InEdgesIterator(this, n));
}
//----------------------------------------------------------------
Iterator<edge> *GraphView::getOutEdges(const node n) const {
  return (new OutEdgesIterator(this, n));
}
//----------------------------------------------------------------
Iterator<edge> *GraphView::getInOutEdges(const node n) const {
  return (new InOutEdgesIterator(this, n));
}
//----------------------------------------------------------------
std::vector<edge> GraphView::getEdges(const node src, const node tgt, bool directed) const {
  std::vector<edge> ee;

  if (isElement(src) && isElement(tgt))
    getRootImpl()->getEdges(src, tgt, directed, ee, this);

  return ee;
}
//----------------------------------------------------------------
void GraphView::reserveNodes(unsigned int) {
#ifndef NDEBUG
  tlp::warning() << "Warning: " << __PRETTY_FUNCTION__ << " ... Impossible operation on a subgraph"
                 << std::endl;
#endif
}
//----------------------------------------------------------------
void GraphView::reserveEdges(unsigned int) {
#ifndef NDEBUG
  tlp::warning() << "Warning: " << __PRETTY_FUNCTION__ << " ... Impossible operation on a subgraph"
                 << std::endl;
#endif
}
//----------------------------------------------------------------
bool GraphView::canPop() {
  return getRootImpl()->canPop();
}
//----------------------------------------------------------------
bool GraphView::canUnpop() {
  return getRootImpl()->canUnpop();
}
//----------------------------------------------------------------
bool GraphView::canPopThenUnpop() {
  return getRootImpl()->canPopThenUnpop();
}
//----------------------------------------------------------------
void GraphView::push(bool unpopAllowed,
                     std::vector<PropertyInterface *> *propertiesToPreserveOnPop) {
  getRootImpl()->push(unpopAllowed, propertiesToPreserveOnPop);
}
//----------------------------------------------------------------
void GraphView::pop(bool unpopAllowed) {
  getRootImpl()->pop(unpopAllowed);
}
//----------------------------------------------------------------
void GraphView::popIfNoUpdates() {
  getRootImpl()->popIfNoUpdates();
}
//----------------------------------------------------------------
void GraphView::unpop() {
  getRootImpl()->unpop();
}
}
