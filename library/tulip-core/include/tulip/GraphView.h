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
  ~GraphView();
  //========================================================================
  node addNode();
  void addNodes(unsigned int nb);
  void addNodes(unsigned int nb, std::vector<node> &addedNodes);
  void addNode(const node);
  void addNodes(Iterator<node> *nodes);
  edge addEdge(const node n1, const node n2);
  void addEdges(const std::vector<std::pair<node, node> > &edges, std::vector<edge> &addedEdges);
  void addEdges(const std::vector<std::pair<node, node> > &edges);
  void addEdge(const edge);
  void addEdges(Iterator<edge> *edges);
  void delNode(const tlp::node n, bool deleteInAllGraphs = false);
  void delEdge(const tlp::edge e, bool deleteInAllGraphs = false);
  void setEdgeOrder(const node n, const std::vector<edge> &v) {
    getRootImpl()->setEdgeOrder(n, v);
  }
  void swapEdgeOrder(const node n, const edge e1, const edge e2) {
    getRootImpl()->swapEdgeOrder(n, e1, e2);
  }
  //=========================================================================
  inline bool isElement(const node n) const {
    return _nodeData.get(n.id) != NULL;
  }
  inline bool isElement(const edge e) const {
    return _edges.isElement(e);
  }
  edge existEdge(const node source, const node target, bool directed) const;
  inline unsigned int numberOfNodes() const {
    return _nodes.size();
  }
  inline unsigned int numberOfEdges() const {
    return _edges.size();
  }
  //=========================================================================
  inline unsigned int deg(const node n) const {
    assert(isElement(n));
    SGraphNodeData *nData = _nodeData.get(n.id);
    return nData->inDegree + nData->outDegree;
  }
  inline unsigned int indeg(const node n) const {
    assert(isElement(n));
    return _nodeData.get(n.id)->inDegree;
  }
  inline unsigned int outdeg(const node n) const {
    assert(isElement(n));
    return _nodeData.get(n.id)->outDegree;
  }
  //=========================================================================
  inline node source(const edge e) const {
    return getRootImpl()->source(e);
  }
  inline void setSource(const edge e, const node newSrc) {
    assert(isElement(e));
    getRootImpl()->setEnds(e, newSrc, node());
  }
  inline node target(const edge e) const {
    return getRootImpl()->target(e);
  }
  inline void setTarget(const edge e, const node newTgt) {
    assert(isElement(e));
    getRootImpl()->setEnds(e, node(), newTgt);
  }
  inline const std::pair<node, node> &ends(const edge e) const {
    return getRootImpl()->ends(e);
  }
  inline void setEnds(const edge e, const node newSrc, const node newTgt) {
    assert(isElement(e));
    getRootImpl()->setEnds(e, newSrc, newTgt);
  }
  inline node opposite(const edge e, const node n) const {
    return getRootImpl()->opposite(e, n);
  }
  inline void reverse(const edge e) {
    assert(isElement(e));
    getRootImpl()->reverse(e);
  }
  //=========================================================================
  inline const std::vector<node> &nodes() const {
    return _nodes;
  }
  inline unsigned int nodePos(const node n) const {
    return _nodes.getPos(n);
  }
  Iterator<node> *getNodes() const;
  Iterator<node> *getInNodes(const node) const;
  Iterator<node> *getOutNodes(const node) const;
  Iterator<node> *getInOutNodes(const node) const;
  inline const std::vector<edge> &edges() const {
    return _edges;
  }
  inline unsigned int edgePos(const edge e) const {
    return _edges.getPos(e);
  }
  Iterator<edge> *getEdges() const;
  Iterator<edge> *getInEdges(const node) const;
  Iterator<edge> *getOutEdges(const node) const;
  Iterator<edge> *getInOutEdges(const node) const;
  std::vector<edge> getEdges(const node source, const node target, bool directed = true) const;
  inline const std::vector<edge> &allEdges(const node n) const {
    return getRootImpl()->allEdges(n);
  }
  inline void sortElts() {
    _nodes.sort();
    _edges.sort();
  }
  inline Graph *getRoot() const {
    // handle root destruction (see GraphAbstract destructor)
    return id == 0 ? const_cast<GraphView *>(this) : GraphAbstract::getRoot();
  }
  //=========================================================================
  // only implemented on a root graph
  virtual void reserveNodes(unsigned int nbNodes);
  virtual void reserveEdges(unsigned int nbEdges);
  //=========================================================================
  // updates management
  virtual void push(bool unpopAllowed = true,
                    std::vector<PropertyInterface *> *propertiesToPreserveOnPop = NULL);
  virtual void pop(bool unpopAllowed = true);
  virtual void popIfNoUpdates();
  virtual void unpop();
  virtual bool canPop();
  virtual bool canUnpop();
  virtual bool canPopThenUnpop();

protected:
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  virtual void restoreNode(node);
  virtual void restoreEdge(edge, node source, node target);
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node);
  virtual void removeEdge(const edge);
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
                        const std::vector<std::pair<node, node> > &ends);
};
}
#endif

///@endcond
