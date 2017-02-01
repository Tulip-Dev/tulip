/*
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
template<class itType >
struct Iterator;
class GraphView;
class GraphUpdatesRecorder;

///Implementation of the graph support of the Graph.
class TLP_SCOPE GraphImpl:public GraphAbstract {

  friend class GraphUpdatesRecorder;
  friend class TLPExport;

public:
  GraphImpl();
  ~GraphImpl();
  void clear();
  //=========================================================================
  bool isElement(const node) const;
  bool isElement(const edge) const;
  edge existEdge(const node source, const node target,
                 bool directed = true) const;
  node addNode();
  void addNodes(unsigned int nb);
  void addNodes(unsigned int nb, std::vector<node>& addedNodes);
  void addNode(const node);
  void addNodes(Iterator<node>* nodes);
  edge addEdge(const node ,const node);
  void addEdges(const std::vector<std::pair<node, node> >& edges);
  void addEdges(const std::vector<std::pair<node, node> >& edges,
                std::vector<edge>& addedEdges);
  void addEdge(const edge);
  void addEdges(Iterator<edge>* edges);
  void delNode(const tlp::node n, bool deleteInAllGraphs = false);
  void delEdge(const tlp::edge e, bool deleteInAllGraphs = false);
  void setEdgeOrder(const node,const std::vector<edge> &);
  void swapEdgeOrder(const node,const edge , const edge);
  //=========================================================================
  Iterator<node>* getNodes() const;
  Iterator<node>* getInNodes(const node) const;
  Iterator<node>* getOutNodes(const node) const;
  Iterator<node>* getInOutNodes(const node) const;
  Iterator<edge>* getEdges() const;
  Iterator<edge>* getInEdges(const node) const;
  Iterator<edge>* getOutEdges(const node) const;
  Iterator<edge>* getInOutEdges(const node) const;
  std::vector<edge> getEdges(const node source, const node target,
                             bool directed = true) const;
  bool getEdges(const node source, const node target, bool directed,
                std::vector<edge>& edges) const {
    return storage.getEdges(source, target, directed, edges);
  }
  // to ensure that loops appear only once
  void getInOutEdges(const node n, std::vector<edge>& edges) const {
    storage.getInOutEdges(n, edges);
  }

  //========================================================================
  unsigned int deg(const node n) const {
    assert(isElement(n));
    return storage.deg(n);
  }
  unsigned int indeg(const node n) const {
    assert(isElement(n));
    return storage.indeg(n);
  }
  unsigned int outdeg(const node n) const {
    assert(isElement(n));
    return storage.outdeg(n);
  }
  //========================================================================
  virtual node source(const edge e) const {
    assert(isElement(e));
    return storage.source(e);
  }
  virtual node target(const edge e) const {
    assert(isElement(e));
    return storage.target(e);
  }
  virtual node opposite(const edge e, const node n) const {
    assert(isElement(e));
    return storage.opposite(e, n);
  }
  virtual const std::pair<node, node>& ends(const edge e) const  {
    return storage.ends(e);
  }
  virtual void setEnds(const edge, const node, const node);
  virtual void reverse(const edge);
  //=======================================================================
  unsigned int numberOfEdges() const {
    return storage.numberOfEdges();
  }
  unsigned int numberOfNodes() const {
    return storage.numberOfNodes();
  }
  //=======================================================================
  // updates management
  virtual void push(bool unpopAllowed = true,
                    std::vector<PropertyInterface*>* propertiesToPreserveOnPop= NULL);
  virtual void pop(bool unpopAllowed = true);
  virtual void unpop();
  virtual bool canPop();
  virtual bool canUnpop();
  virtual bool canPopThenUnpop();

  // observer interface
  void treatEvents(const std::vector<Event> &);

  // for subgraph id management
  unsigned int getSubGraphId(unsigned int id);
  void freeSubGraphId(unsigned int id);

  // to improve memory allocation
  // attempt to reserve enough space to store nodes/edges
  virtual void reserveNodes(unsigned int nbNodes);
  virtual void reserveEdges(unsigned int nbEdges);

protected:
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  virtual node restoreNode(node);
  virtual edge restoreEdge(edge, node source, node target);
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node);
  virtual void removeEdge(const edge);
  // used by PropertyManager
  virtual bool canDeleteProperty(Graph* g, PropertyInterface *prop);

private :
  GraphStorage storage;
  IdManager graphIds;
  std::list<GraphUpdatesRecorder*> previousRecorders;
  std::list<Graph *> observedGraphs;
  std::list<PropertyInterface*> observedProps;
  std::list<GraphUpdatesRecorder*> recorders;

  void observeUpdates(Graph*);
  void unobserveUpdates();
  void delPreviousRecorders();
};

}
#endif

///@endcond
