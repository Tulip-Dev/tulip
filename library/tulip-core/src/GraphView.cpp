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
#include <queue>

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
  GraphAbstract(supergraph, sgId),
  nNodes(0),
  nEdges(0) {
  nodeAdaptativeFilter.setAll(false);
  edgeAdaptativeFilter.setAll(false);
  inDegree.setAll(0);
  outDegree.setAll(0);

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
bool GraphView::isElement(const node n) const {
  return nodeAdaptativeFilter.get(n.id);
}
//----------------------------------------------------------------
bool GraphView::isElement(const edge e) const {
  return edgeAdaptativeFilter.get(e.id);
}
//----------------------------------------------------------------
edge GraphView::existEdge(const node src, const node tgt,
                          bool directed) const {
  if (!nodeAdaptativeFilter.get(src.id) ||
      !nodeAdaptativeFilter.get(tgt.id))
    return edge();

  std::vector<edge> edges;

  if (((GraphImpl *)getRoot())->getEdges(src, tgt, directed, edges)) {
    for(std::vector<edge>::iterator ite = edges.begin();
        ite != edges.end(); ++ite) {
      if(edgeAdaptativeFilter.get(ite->id))
        return *ite;
    }
  }

  return edge();
}
//----------------------------------------------------------------
unsigned int GraphView::deg(const node n) const {
  return inDegree.get(n.id)+outDegree.get(n.id);
}
//----------------------------------------------------------------
unsigned int GraphView::indeg(const node n) const {
  return inDegree.get(n.id);
}
//----------------------------------------------------------------
unsigned int GraphView::outdeg(const node n) const {
  return outDegree.get(n.id);
}
//----------------------------------------------------------------
void GraphView::reverseInternal(const edge e, const node src, const node tgt) {
  if (isElement(e)) {
    outDegree.add(src.id, -1);
    inDegree.add(tgt.id, -1);
    inDegree.add(src.id, 1);
    outDegree.add(tgt.id, 1);

    notifyReverseEdge(e);

    // propagate edge reversal on subgraphs
    Graph* sg;
    forEach(sg, getSubGraphs()) {
      ((GraphView*) sg)->reverseInternal(e, src, tgt);
    }
  }
}
//----------------------------------------------------------------
void GraphView::setEndsInternal(const edge e, const node src, const node tgt,
                                const node newSrc, const node newTgt) {
  if (isElement(e)) {
    if (isElement(newSrc) && isElement(newTgt)) {
      notifyBeforeSetEnds(e);

      if (src != newSrc) {
        outDegree.add(src.id, -1);
        outDegree.add(newSrc.id, 1);
      }

      if (tgt != newTgt) {
        inDegree.add(tgt.id, -1);
        inDegree.add(newTgt.id, 1);
      }

      // notification
      notifyAfterSetEnds(e);

      // propagate edge ends update on subgraphs
      Graph* sg;
      forEach(sg, getSubGraphs()) {
        ((GraphView*) sg)->setEndsInternal(e, src, tgt, newSrc, newTgt);
      }
    }
    else
      // delete edge if new edge ends do no belong to the graph
      delEdge(e);
  }
}
//----------------------------------------------------------------
node GraphView::addNode() {
  node tmp = getSuperGraph()->addNode();
  return restoreNode(tmp);
}
//----------------------------------------------------------------
void GraphView::addNodes(unsigned int nb, std::vector<node>& addedNodes) {
  getSuperGraph()->addNodes(nb, addedNodes);
  restoreNodes(addedNodes);
}
//----------------------------------------------------------------
node GraphView::restoreNode(node n) {
  nodeAdaptativeFilter.set(n.id, true);
  ++nNodes;
  notifyAddNode(n);
  return n;
}
//----------------------------------------------------------------
void GraphView::restoreNodes(const std::vector<node>& nodes) {
  std::vector<node>::const_iterator it = nodes.begin();
  std::vector<node>::const_iterator ite = nodes.end();

  for (; it != ite; ++it) {
    assert(getRoot()->isElement(*it));
    nodeAdaptativeFilter.set(*it, true);
  }

  nNodes += nodes.size();

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
    restoreNodes(nodes);
}
//----------------------------------------------------------------
edge GraphView::addEdgeInternal(edge e) {
  edgeAdaptativeFilter.set(e.id,true);
  ++nEdges;
  const std::pair<node, node>& eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  outDegree.add(src.id, 1);
  inDegree.add(tgt.id, 1);
  notifyAddEdge(e);
  return e;
}
//----------------------------------------------------------------
edge GraphView::restoreEdge(edge e, const node, const node) {
  return addEdgeInternal(e);
}
//----------------------------------------------------------------
void GraphView::restoreEdges(const std::vector<edge>& edges,
                             const std::vector<std::pair<node, node> >&ends) {
  bool hasEnds = !ends.empty();
  unsigned int i = 0;
  std::vector<edge>::const_iterator it = edges.begin();
  std::vector<edge>::const_iterator ite = edges.end();

  for (; it != ite; ++it, ++i) {
    edge e = *it;
    assert(getRoot()->isElement(e));
    edgeAdaptativeFilter.set(e, true);
    std::pair<node, node> eEnds(hasEnds ? ends[i] : this->ends(e));
    node src = eEnds.first;
    node tgt = eEnds.second;
    outDegree.add(src.id, 1);
    inDegree.add(tgt.id, 1);
  }

  nEdges += edges.size();

  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGES, edges));
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

  if (!edgeAdaptativeFilter.get(e.id)) {
    if (!getSuperGraph()->isElement(e))
      getSuperGraph()->addEdge(e);

    addEdgeInternal(e);
  }
}
//----------------------------------------------------------------
void GraphView::addEdges(const std::vector<std::pair<node, node> >& ends,
                         std::vector<edge>& addedEdges) {
  assert(ends.size() == addedEdges.size());
  getSuperGraph()->addEdges(ends, addedEdges);
  restoreEdges(addedEdges, ends);
}
//----------------------------------------------------------------
void GraphView::addEdges(Iterator<edge>* addedEdges) {
  std::vector<edge> edges;
  std::vector<edge> superEdges;
  Graph* super = getSuperGraph();
  bool superIsRoot = (super == getRoot());

  while(addedEdges->hasNext()) {
    edge e = addedEdges->next();
    assert(getRoot()->isElement(e));
    assert(isElement(source(e)));
    assert(isElement(target(e)));

    if (!edgeAdaptativeFilter.get(e.id)) {
      edges.push_back(e);

      if (!superIsRoot && !super->isElement(e))
        superEdges.push_back(e);
    }
  }

  if (!superEdges.empty()) {
    StlIterator<edge, std::vector<edge>::iterator> it(superEdges.begin(),
        superEdges.end());
    super->addEdges(&it);
  }

  if (!edges.empty())
    restoreEdges(edges, std::vector<pair<node, node> >());
}
//----------------------------------------------------------------
void GraphView::delNodeInternal(const node n) {
  nodeAdaptativeFilter.set(n.id, false);
  propertyContainer->erase(n);
  --nNodes;
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n) {
  notifyDelNode(n);
  delNodeInternal(n);
}
//----------------------------------------------------------------
void GraphView::removeNode(const node n, const std::vector<edge>& edges) {
  assert(isElement(n));
  notifyDelNode(n);
  removeEdges(edges);
  delNodeInternal(n);
}
//----------------------------------------------------------------
void GraphView::delNode(const node n, bool deleteInAllGraphs) {
  if(deleteInAllGraphs) {
    getRoot()->delNode(n, true);
  }
  else {
    assert (isElement(n));
    notifyDelNode(n);

    // get edges vector with loops appearing only once
    std::vector<edge> edges;
    ((GraphImpl *)getRoot())->getInOutEdges(n, edges, true);

    // use a queue for a dfs subgraphs propagation
    std::queue<Graph*> sgq;
    Iterator<Graph*>* sgs = getSubGraphs();

    while (sgs->hasNext()) {
      Graph* sg = sgs->next();

      if (sg->isElement(n))
        sgq.push(sg);
    }

    delete sgs;

    // subgraphs loop
    while(!sgq.empty()) {
      Graph* sg = sgq.front();

      sgs = sg->getSubGraphs();

      while (sgs->hasNext()) {
        Graph* ssg = sgs->next();

        if (ssg->isElement(n))
          sgq.push(ssg);
      }

      delete sgs;

      if (sg == sgq.front()) {
        ((GraphView *) sg)->removeNode(n, edges);
        sgq.pop();
      }
    }

    removeEdges(edges);

    delNodeInternal(n);
  }
}
//----------------------------------------------------------------
void GraphView::delEdgeInternal(const edge e) {
  edgeAdaptativeFilter.set(e.id,false);
  propertyContainer->erase(e);
  --nEdges;
  const std::pair<node, node>& eEnds = ends(e);
  node src = eEnds.first;
  node tgt = eEnds.second;
  outDegree.add(src.id, -1);
  inDegree.add(tgt.id, -1);
}
//----------------------------------------------------------------
void GraphView::removeEdge(const edge e) {
  assert(isElement(e));
  notifyDelEdge(e);
  delEdgeInternal(e);
}
//----------------------------------------------------------------
void GraphView::removeEdges(const std::vector<edge>& edges) {
  std::vector<edge>::const_iterator ite = edges.begin();

  while (ite != edges.end()) {
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
    notifyDelEdge(e);
    // propagate to subgraphs
    Iterator<Graph *>*itS=getSubGraphs();

    while (itS->hasNext()) {
      Graph *subGraph = itS->next();

      if (subGraph->isElement(e))
        subGraph->delEdge(e);
    }

    delete itS;
    delEdgeInternal(e);
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
  it = nodeAdaptativeFilter.findAll(true);

  if (it==0)
    return new SGraphNodeIterator(this, nodeAdaptativeFilter);
  else
    return (new UINTIterator<node>(it));
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getInNodes(const node n) const {
  return (new InNodesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getOutNodes(const node n)const {
  return (new OutNodesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<node>* GraphView::getInOutNodes(const node n)const {
  return (new InOutNodesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getEdges()const {
  Iterator<unsigned int> *it=0;
  it = edgeAdaptativeFilter.findAll(true);

  if (it==0)
    return new SGraphEdgeIterator(this, edgeAdaptativeFilter);
  else
    return (new UINTIterator<edge>(it));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getInEdges(const node n)const {
  return (new InEdgesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getOutEdges(const node n)const {
  return (new OutEdgesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
Iterator<edge>* GraphView::getInOutEdges(const node n)const {
  return (new InOutEdgesIterator(this, edgeAdaptativeFilter,n));
}
//----------------------------------------------------------------
std::vector<edge> GraphView::getEdges(const node src, const node tgt,
                                      bool directed) const {
  std::vector<edge> edges;

  if (nodeAdaptativeFilter.get(src.id) && nodeAdaptativeFilter.get(tgt.id) &&
      ((GraphImpl *)getRoot())->getEdges(src, tgt, directed, edges)) {
    for(std::vector<edge>::iterator ite = edges.begin(); ite != edges.end(); ++ite) {
      if(!edgeAdaptativeFilter.get(ite->id)) {
        ite = edges.erase(ite); //erase returns an iterator to the next element in the vector
        //going back in the vector because the for loop will do a ++ite before testing the end of the vector
        --ite;
      }
    }
  }

  return edges;
}
//----------------------------------------------------------------
unsigned int GraphView::numberOfNodes()const {
  return nNodes;
}
//----------------------------------------------------------------
void GraphView::reserveNodes(unsigned int) {
  tlp::warning() << "Warning: "  << __PRETTY_FUNCTION__ << " ... Impossible operation on a Sub Graph" << std::endl;
}
//----------------------------------------------------------------
unsigned int GraphView::numberOfEdges() const {
  return nEdges;
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
