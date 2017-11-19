/*
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_SUPERGRAPHVIEW_H
#define Tulip_SUPERGRAPHVIEW_H

#include <vector>
#include <tulip/GraphAbstract.h>
#include <tulip/GraphImpl.h>
#include <tulip/MutableContainer.h>
#include <tulip/IdManager.h>

namespace tlp {
// used for node management
struct SGraphNodeData {
  unsigned int outDegree;
  unsigned int inDegree;
  SGraphNodeData() : outDegree(0), inDegree(0) {}
  inline void outDegreeAdd(int i) {
    outDegree += i;
  }
  inline void inDegreeAdd(int i) {
    inDegree += i;
  }
};

typedef SGraphNodeData *SGraphNodeDataPtr;
DECL_STORED_PTR(SGraphNodeDataPtr);

/**
 * This class is one of the implementation of the Graph Interface
 * It only filters the elements of its parents.
 */
class GraphView : public GraphAbstract {

  inline GraphImpl *getRootImpl() const {
    return static_cast<GraphImpl *>(getRoot());
  }

  friend class GraphImpl;

public:
  GraphView(Graph *supergraph, BooleanProperty *filter, unsigned int id);
  ~GraphView() override;
  //========================================================================
  node addNode() override;
  void addNodes(unsigned int nb) override;
  void addNodes(unsigned int nb, std::vector<node> &addedNodes) override;
  void addNode(const node) override;
  void addNodes(Iterator<node> *nodes) override;
  edge addEdge(const node n1, const node n2) override;
  void addEdges(const std::vector<std::pair<node, node>> &edges,
                std::vector<edge> &addedEdges) override;
  void addEdges(const std::vector<std::pair<node, node>> &edges) override;
  void addEdge(const edge) override;
  void addEdges(Iterator<edge> *edges) override;
  void delNode(const tlp::node n, bool deleteInAllGraphs = false) override;
  void delEdge(const tlp::edge e, bool deleteInAllGraphs = false) override;
  void setEdgeOrder(const node n, const std::vector<edge> &v) override {
    getRootImpl()->setEdgeOrder(n, v);
  }
  void swapEdgeOrder(const node n, const edge e1, const edge e2) override {
    getRootImpl()->swapEdgeOrder(n, e1, e2);
  }
  //=========================================================================
  inline bool isElement(const node n) const override {
    return _nodeData.get(n.id) != nullptr;
  }
  inline bool isElement(const edge e) const override {
    return _edges.isElement(e);
  }
  edge existEdge(const node source, const node target, bool directed) const override;
  inline unsigned int numberOfNodes() const override {
    return _nodes.size();
  }
  inline unsigned int numberOfEdges() const override {
    return _edges.size();
  }
  //=========================================================================
  inline unsigned int deg(const node n) const override {
    assert(isElement(n));
    SGraphNodeData *nData = _nodeData.get(n.id);
    return nData->inDegree + nData->outDegree;
  }
  inline unsigned int indeg(const node n) const override {
    assert(isElement(n));
    return _nodeData.get(n.id)->inDegree;
  }
  inline unsigned int outdeg(const node n) const override {
    assert(isElement(n));
    return _nodeData.get(n.id)->outDegree;
  }
  //=========================================================================
  inline node source(const edge e) const override {
    return getRootImpl()->source(e);
  }
  inline void setSource(const edge e, const node newSrc) override {
    assert(isElement(e));
    getRootImpl()->setEnds(e, newSrc, node());
  }
  inline node target(const edge e) const override {
    return getRootImpl()->target(e);
  }
  inline void setTarget(const edge e, const node newTgt) override {
    assert(isElement(e));
    getRootImpl()->setEnds(e, node(), newTgt);
  }
  inline const std::pair<node, node> &ends(const edge e) const override {
    return getRootImpl()->ends(e);
  }
  inline void setEnds(const edge e, const node newSrc, const node newTgt) override {
    assert(isElement(e));
    getRootImpl()->setEnds(e, newSrc, newTgt);
  }
  inline node opposite(const edge e, const node n) const override {
    return getRootImpl()->opposite(e, n);
  }
  inline void reverse(const edge e) override {
    assert(isElement(e));
    getRootImpl()->reverse(e);
  }
  //=========================================================================
  inline const std::vector<node> &nodes() const override {
    return _nodes;
  }
  inline unsigned int nodePos(const node n) const override {
    return _nodes.getPos(n);
  }
  Iterator<node> *getNodes() const override;
  Iterator<node> *getInNodes(const node) const override;
  Iterator<node> *getOutNodes(const node) const override;
  Iterator<node> *getInOutNodes(const node) const override;
  inline const std::vector<edge> &edges() const override {
    return _edges;
  }
  inline unsigned int edgePos(const edge e) const override {
    return _edges.getPos(e);
  }
  Iterator<edge> *getEdges() const override;
  Iterator<edge> *getInEdges(const node) const override;
  Iterator<edge> *getOutEdges(const node) const override;
  Iterator<edge> *getInOutEdges(const node) const override;
  std::vector<edge> getEdges(const node source, const node target,
                             bool directed = true) const override;
  inline const std::vector<edge> &allEdges(const node n) const override {
    return getRootImpl()->allEdges(n);
  }
  inline void sortElts() override {
    _nodes.sort();
    _edges.sort();
  }
  inline Graph *getRoot() const override {
    // handle root destruction (see GraphAbstract destructor)
    return id == 0 ? const_cast<GraphView *>(this) : GraphAbstract::getRoot();
  }
  //=========================================================================
  // only implemented on a root graph
  void reserveNodes(unsigned int nbNodes) override;
  void reserveEdges(unsigned int nbEdges) override;
  //=========================================================================
  // updates management
  void push(bool unpopAllowed = true,
            std::vector<PropertyInterface *> *propertiesToPreserveOnPop = nullptr) override;
  void pop(bool unpopAllowed = true) override;
  void popIfNoUpdates() override;
  void unpop() override;
  bool canPop() override;
  bool canUnpop() override;
  bool canPopThenUnpop() override;

protected:
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  void restoreNode(node) override;
  void restoreEdge(edge, node source, node target) override;
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  void removeNode(const node) override;
  void removeEdge(const edge) override;
  void removeNode(const node n, const std::vector<edge> &edges);
  void removeEdges(const std::vector<edge> &edges);

private:
  MutableContainer<SGraphNodeDataPtr> _nodeData;
  SGraphIdContainer<node> _nodes;
  SGraphIdContainer<edge> _edges;
  void outDegreeAdd(node n, int i);
  void inDegreeAdd(node n, int i);
  edge addEdgeInternal(edge);
  void reverseInternal(const edge, const node src, const node tgt);
  void setEndsInternal(const edge, node src, node tgt, const node newSrc, const node newTgt);
  void addNodesInternal(unsigned int nbAdded, const std::vector<node> *nodes);
  void addEdgesInternal(unsigned int nbAdded, const std::vector<edge> *edges,
                        const std::vector<std::pair<node, node>> &ends);
};
}
#endif

///@endcond
