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
GraphView::GraphView(Graph *supergraph, BooleanProperty *filter,
                     unsigned int sgId):
  GraphAbstract(supergraph, sgId) {
  _nodeData.setAll(NULL);
  edgeAdaptativeFilter.setAll(false);

  if (filter == NULL) return;

  Iterator<unsigned int> *it = NULL;
  it = filter->nodeProperties.findAll(true);

  Iterator<node> *iteN;

  if (it == NULL) {
    Graph *graphToFilter = filter->getGraph();

    if (graphToFilter == NULL)
      graphToFilter = supergraph;

    iteN =graphToFilter->getNodes();
  }
  else
    iteN = new UINTIterator<node>(it);

  while (iteN->hasNext()) {
    node n=iteN->next();

    if (filter->getNodeValue(n)) addNode(n);
  }

  delete iteN;

  it = filter->edgeProperties.findAll(true);

  Iterator<edge> *iteE;

  if (it == NULL) {
    Graph *graphToFilter = filter->getGraph();

    if (graphToFilter == NULL)
      graphToFilter = supergraph;

    iteE = graphToFilter->getEdges();
  }
  else
    iteE = new UINTIterator<edge>(it);

  while (iteE->hasNext()) {
    edge e = iteE->next();

    if (filter->getEdgeValue(e)) addEdge(e);
  }

  delete iteE;
}
//----------------------------------------------------------------
GraphView::~GraphView() {
  // notify destruction
  observableDeleted();
}
//----------------------------------------------------------------
edge GraphView::existEdge(const node src, const node tgt,
                          bool directed) const {
  if (!isElement(src) || !isElement(tgt))
    return edge();

  std::vector<edge> ee;

  return ((GraphImpl *)getRoot())->getEdges(src, tgt, directed,
         ee, this, true)
         ? ee[0] : edge();
}
//----------------------------------------------------------------
void GraphView::reverseInternal(const edge e, const node src, const node tgt) {
  if (isElement(e)) {
    SGraphNodeData* srcData = _nodeData.get(src.id);
    SGraphNodeData* tgtData = _nodeData.get(tgt.id);
    srcData->outDegreeAdd(-1);
    srcData->inDegreeAdd(1);
    tgtData->inDegreeAdd(-1);
    tgtData->outDegreeAdd(1);

    notifyReverseEdge(e);

    // propagate edge reversal on subgraphs
    Graph* sg;
    forEach(sg, getSubGraphs()) {
      ((GraphView*) sg)->reverseInternal(e, src, tgt);
    }
  }
}
//----------------------------------------------------------------
void GraphView::setEndsInternal(const edge e, node src, node tgt,
                                const node newSrc, const node newTgt) {
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
      Graph* sg;
      forEach(sg, getSubGraphs()) {
        ((GraphView*) sg)->setEndsInternal(e, src, tgt, newSrc, newTgt);
      }
    }
    else {
      // delete e if its new ends do no belong to the graph
      // propagate edge ends update on subgraphs
      Graph* sg;
      forEach(sg, getSubGraphs()) {
        ((GraphView*) sg)->setEndsInternal(e, src, tgt, newSrc, newTgt);
      }
      notifyDelEdge(e);

      edgeAdaptativeFilter.set(e.id, false);
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
void GraphView::addNodes(unsigned int nb, std::vector<node>& addedNodes) {
  getSuperGraph()->addNodes(nb, addedNodes);
  addNodesInternal(addedNodes);
}
//----------------------------------------------------------------
void GraphView::restoreNode(node n) {
  _nodeData.set(n.id, new SGraphNodeData());
  notifyAddNode(n);
}
//----------------------------------------------------------------
void GraphView::addNodesInternal(const std::vector<node>& nodes) {
  std::vector<node>::const_iterator it = nodes.begin();
  std::vector<node>::const_iterator ite = nodes.end();

  for (; it != ite; ++it) {
    assert(getRoot()->isElement(*it));
    _nodeData.set(*it, new SGraphNodeData());
  }

  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODES, nodes));
}
//----------------------------------------------------------------
void GraphView::addNode(const node n) {
  assert(getRoot()->isElement(n));

  if (!isElement(n)) {
    if (!getSuperGraph()->isElement(n)) getSuperGraph()->addNode(n);

    restoreNode(n);
  }
}
//----------------------------------------------------------------
void GraphView::addNodes(Iterator<node>* addedNodes) {
  std::vector<node> nodes;
  std::vector<node> superNodes;
  Graph* super = getSuperGraph();
  bool superIsRoot = (super == getRoot());

  while(addedNodes->hasNext()) {
    node n = addedNodes->next();

    if (!isElement(n)) {
      nodes.push_back(n);

      if (!superIsRoot && !super->isElement(n))
        superNodes.push_back(n);
    }
  }

  if (!superNodes.empty()) {
    StlIterator<node, std::vector<node>::iterator> it(superNodes.begin(),
        superNodes.end());
    super->addNodes(&it);
  }

  if (!nodes.empty())
    addNodesInternal(nodes);
}
//----------------------------------------------------------------
edge GraphView::addEdgeInternal(edge e) {
  edgeAdaptativeFilter.set(e.id,true);
  const std::pair<node, node>& eEnds = ends(e);
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
void GraphView::addEdgesInternal(const std::vector<edge>& ee,
                                 const std::vector<std::pair<node, node> >&ends) {
  bool hasEnds = !ends.empty();
  unsigned int i = 0;
  std::vector<edge>::const_iterator it = ee.begin();
  std::vector<edge>::const_iterator ite = ee.end();

  for (; it != ite; ++it, ++i) {
    edge e = *it;
    assert(getRoot()->isElement(e));
    edgeAdaptativeFilter.set(e, true);
    std::pair<node, node> eEnds(hasEnds ? ends[i] : this->ends(e));
    node src = eEnds.first;
    node tgt = eEnds.second;
    _nodeData.get(src.id)->outDegreeAdd(1);
    _nodeData.get(tgt.id)->inDegreeAdd(1);
  }

  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, ee));
}
//----------------------------------------------------------------
edge GraphView::addEdge(const node n1, const node n2) {
  assert(isElement(n1));
  assert(isElement(n2));
  return addEdgeInternal(getSuperGraph()->addEdge(n1,n2));
}
//----------------------------------------------------------------
void GraphView::addEdge(const edge e) {
  assert(getRoot()->isElement(e));
  assert(isElement(source(e)));
  assert(isElement(target(e)));

  if (!isElement(e)) {
    if (!getSuperGraph()->isElement(e))
      getSuperGraph()->addEdge(e);

    addEdgeInternal(e);
  }
}
//----------------------------------------------------------------
void GraphView::addEdges(const std::vector<std::pair<node, node> >& ends,
                         std::vector<edge>& addedEdges) {
  getSuperGraph()->addEdges(ends, addedEdges);
  addEdgesInternal(addedEdges, ends);
}
//----------------------------------------------------------------
void GraphView::addEdges(Iterator<edge>* addedEdges) {
  std::vector<edge> ee;
  std::vector<edge> superEdges;
  Graph* super = getSuperGraph();
  bool superIsRoot = (super == getRoot());

  while(addedEdges->hasNext()) {
    edge e = addedEdges->next();
    assert(getRoot()->isElement(e));
    assert(isElement(source(e)));
    assert(isElement(target(e)));

    if (!isElement(e)) {
      ee.push_back(e);

      if (!superIsRoot && !super->isElement(e))
        superEdges.push_back(e);
    }
  }

  if (!superEdges.empty()) {
    StlIterator<edge, std::vector<edge>::iterator> it(superEdges.begin(),
        superEdges.end());
    super->addEdges(&it);
  }

  if (!ee.empty())
    addEdgesInternal(ee, std::vector<pair<node, node> >());
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n) {
  assert(isElement(n));
  notifyDelNode(n);
  _nodeData.set(n.id, NULL);
  propertyContainer->erase(n);
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n, const std::vector<edge>& ee) {
  removeEdges(ee);
  removeNode(n);
}
//----------------------------------------------------------------
void GraphView::delNode(const node n, bool deleteInAllGraphs) {
  if(deleteInAllGraphs) {
    getRoot()->delNode(n, true);
  }
  else {
    assert(isElement(n));

    // get edges vector
    std::vector<edge> ee(((GraphImpl *)getRoot())->adj(n));

    // use a stack for a dfs subgraphs propagation
    std::stack<Graph*> sgq;
    Iterator<Graph*>* sgs = getSubGraphs();

    while (sgs->hasNext()) {
      Graph* sg = sgs->next();

      if (sg->isElement(n))
        sgq.push(sg);
    }

    delete sgs;

    // subgraphs loop
    while(!sgq.empty()) {
      Graph* sg = sgq.top();

      sgs = sg->getSubGraphs();

      while (sgs->hasNext()) {
        Graph* ssg = sgs->next();

        if (ssg->isElement(n))
          sgq.push(ssg);
      }

      delete sgs;

      if (sg == sgq.top()) {
        ((GraphView *) sg)->removeNode(n, ee);
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

  edgeAdaptativeFilter.set(e.id, false);
  propertyContainer->erase(e);
  const std::pair<node, node>& eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  _nodeData.get(src.id)->outDegreeAdd(-1);
  _nodeData.get(tgt.id)->inDegreeAdd(-1);
}
//----------------------------------------------------------------
void GraphView::removeEdges(const std::vector<edge>& ee) {
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
  if(deleteInAllGraphs) {
    getRoot()->delEdge(e, true);
  }
  else {
    assert(isElement(e));
    // propagate to subgraphs
    Iterator<Graph *>*itS=getSubGraphs();

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
void GraphView::setEdgeOrder(const node n,const std::vector<edge> &v ) {
  getRoot()->setEdgeOrder(n,v);
}
//----------------------------------------------------------------
void GraphView::swapEdgeOrder(const node n,const edge e1 , const edge e2) {
  getRoot()->swapEdgeOrder(n,e1,e2);
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getNodes() const {
  Iterator<unsigned int> *it=0;
  it = _nodeData.findAll(NULL, false);

  return (new UINTIterator<node>(it));
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getInNodes(const node n) const {
  return (new InNodesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getOutNodes(const node n) const {
  return (new OutNodesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getInOutNodes(const node n) const {
  return (new InOutNodesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getEdges() const {
  Iterator<unsigned int> *it=0;
  it = edgeAdaptativeFilter.findAll(true);
  return (new UINTIterator<edge>(it));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getInEdges(const node n) const {
  return (new InEdgesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getOutEdges(const node n) const {
  return (new OutEdgesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getInOutEdges(const node n) const {
  return (new InOutEdgesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
std::vector<edge> GraphView::getEdges(const node src, const node tgt,
                                      bool directed) const {
  std::vector<edge> ee;

  if (isElement(src) && isElement(tgt))
    ((GraphImpl *)getRoot())->getEdges(src, tgt, directed, ee, this);

  return ee;
}
//----------------------------------------------------------------
void GraphView::reserveNodes(unsigned int) {
  tlp::warning() << "Warning: "  << __PRETTY_FUNCTION__ << " ... Impossible operation on a Sub Graph" << std::endl;
}
//----------------------------------------------------------------
void GraphView::reserveEdges(unsigned int) {
  tlp::warning() << "Warning: "  << __PRETTY_FUNCTION__ << " ... Impossible operation on a Sub Graph" << std::endl;
}
//----------------------------------------------------------------
bool GraphView::canPop() {
  return getRoot()->canPop();
}
//----------------------------------------------------------------
bool GraphView::canUnpop() {
  return getRoot()->canUnpop();
}
//----------------------------------------------------------------
bool GraphView::canPopThenUnpop() {
  return getRoot()->canPopThenUnpop();
}
//----------------------------------------------------------------
void GraphView::push(bool unpopAllowed,
                     std::vector<PropertyInterface*>* propertiesToPreserveOnPop) {
  getRoot()->push(unpopAllowed, propertiesToPreserveOnPop);
}
//----------------------------------------------------------------
void GraphView::pop(bool unpopAllowed) {
  getRoot()->pop(unpopAllowed);
}
//----------------------------------------------------------------
void GraphView::unpop() {
  getRoot()->unpop();
}
}
