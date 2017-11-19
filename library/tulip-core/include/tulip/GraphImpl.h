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

#ifndef Tulip_SUPERGRAPHIMPL_H
#define Tulip_SUPERGRAPHIMPL_H

#include <set>

#include <vector>
#include <list>
#include <tulip/GraphAbstract.h>
#include <tulip/GraphStorage.h>
#include <tulip/IdManager.h>

namespace tlp {
template <class itType>
struct Iterator;
class GraphView;
class GraphUpdatesRecorder;

/// Implementation of the graph support of the Graph.
class TLP_SCOPE GraphImpl : public GraphAbstract {

  friend class GraphUpdatesRecorder;
  friend class TLPExport;

public:
  GraphImpl();
  ~GraphImpl() override;
  void clear() override;
  //=========================================================================
  inline bool isElement(const node n) const override {
    return storage.isElement(n);
  }
  bool isElement(const edge e) const override {
    return storage.isElement(e);
  }
  edge existEdge(const node source, const node target, bool directed = true) const override;
  node addNode() override;
  void addNodes(unsigned int nb) override;
  void addNodes(unsigned int nb, std::vector<node> &addedNodes) override;
  void addNode(const node) override;
  void addNodes(Iterator<node> *nodes) override;
  edge addEdge(const node, const node) override;
  void addEdges(const std::vector<std::pair<node, node>> &edges) override;
  void addEdges(const std::vector<std::pair<node, node>> &edges,
                std::vector<edge> &addedEdges) override;
  void addEdge(const edge) override;
  void addEdges(Iterator<edge> *edges) override;
  void delNode(const tlp::node n, bool deleteInAllGraphs = false) override;
  void delEdge(const tlp::edge e, bool deleteInAllGraphs = false) override;
  inline void setEdgeOrder(const node n, const std::vector<edge> &v) override {
    storage.setEdgeOrder(n, v);
  }
  inline void swapEdgeOrder(const node n, const edge e1, const edge e2) override {
    storage.swapEdgeOrder(n, e1, e2);
  }
  //=========================================================================
  inline const std::vector<node> &nodes() const override {
    return storage.nodes();
  }
  inline unsigned int nodePos(const node n) const override {
    return storage.nodePos(n);
  }
  Iterator<node> *getNodes() const override;
  Iterator<node> *getInNodes(const node) const override;
  Iterator<node> *getOutNodes(const node) const override;
  Iterator<node> *getInOutNodes(const node) const override;
  inline const std::vector<edge> &edges() const override {
    return storage.edges();
  }
  inline unsigned int edgePos(const edge e) const override {
    return storage.edgePos(e);
  }
  Iterator<edge> *getEdges() const override;
  Iterator<edge> *getInEdges(const node) const override;
  Iterator<edge> *getOutEdges(const node) const override;
  Iterator<edge> *getInOutEdges(const node) const override;
  std::vector<edge> getEdges(const node source, const node target,
                             bool directed = true) const override;
  bool getEdges(const node source, const node target, bool directed, std::vector<edge> &edges,
                const Graph *sg = nullptr, bool onlyFirst = false) const {
    return storage.getEdges(source, target, directed, edges, sg, onlyFirst);
  }
  inline const std::vector<edge> &allEdges(const node n) const override {
    return storage.adj(n);
  }
  //========================================================================
  inline unsigned int deg(const node n) const override {
    assert(isElement(n));
    return storage.deg(n);
  }
  inline unsigned int indeg(const node n) const override {
    assert(isElement(n));
    return storage.indeg(n);
  }
  inline unsigned int outdeg(const node n) const override {
    assert(isElement(n));
    return storage.outdeg(n);
  }
  //========================================================================
  inline node source(const edge e) const override {
    assert(isElement(e));
    return storage.source(e);
  }
  inline node target(const edge e) const override {
    assert(isElement(e));
    return storage.target(e);
  }
  inline node opposite(const edge e, const node n) const override {
    assert(isElement(e));
    return storage.opposite(e, n);
  }
  inline const std::pair<node, node> &ends(const edge e) const override {
    return storage.ends(e);
  }
  inline void setSource(const edge e, const node newSrc) override {
    assert(isElement(e));
    this->setEnds(e, newSrc, node());
  }
  inline void setTarget(const edge e, const node newTgt) override {
    assert(isElement(e));
    this->setEnds(e, node(), newTgt);
  }
  void setEnds(const edge, const node, const node) override;
  void reverse(const edge) override;
  //=======================================================================
  inline unsigned int numberOfEdges() const override {
    return storage.numberOfEdges();
  }
  inline unsigned int numberOfNodes() const override {
    return storage.numberOfNodes();
  }
  inline void sortElts() override {
    storage.sortElts();
  }
  //=======================================================================
  // updates management
  void push(bool unpopAllowed = true,
            std::vector<PropertyInterface *> *propertiesToPreserveOnPop = nullptr) override;
  void pop(bool unpopAllowed = true) override;
  void popIfNoUpdates() override;
  void unpop() override;
  bool canPop() override;
  bool canUnpop() override;
  bool canPopThenUnpop() override;

  // observer interface
  void treatEvents(const std::vector<Event> &) override;

  // for subgraph id management
  unsigned int getSubGraphId(unsigned int id);
  void freeSubGraphId(unsigned int id);

  // to improve memory allocation
  // attempt to reserve enough space to store nodes/edges
  void reserveNodes(unsigned int nbNodes) override;
  void reserveEdges(unsigned int nbEdges) override;

protected:
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  void restoreNode(node) override;
  void restoreEdge(edge, node source, node target) override;
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  void removeNode(const node) override;
  void removeEdge(const edge) override;
  // used by PropertyManager
  bool canDeleteProperty(Graph *g, PropertyInterface *prop) override;

private:
  GraphStorage storage;
  IdManager graphIds;
  std::list<GraphUpdatesRecorder *> previousRecorders;
  std::list<Graph *> observedGraphs;
  std::list<PropertyInterface *> observedProps;
  std::list<GraphUpdatesRecorder *> recorders;

  void observeUpdates(Graph *);
  void unobserveUpdates();
  void delPreviousRecorders();
};
}
#endif

///@endcond
