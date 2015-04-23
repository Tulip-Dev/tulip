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


#ifndef Tulip_SUPERGRAPHVIEW_H
#define Tulip_SUPERGRAPHVIEW_H

#ifndef DOXYGEN_NOTFOR_USER

#include <tulip/GraphAbstract.h>
#include <tulip/GraphImpl.h>
#include <tulip/MutableContainer.h>

namespace tlp {

/**
 * This class is one the implementation of the Graph Interface
 * It only filters the elements of its parents.
 */
class GraphView:public GraphAbstract {

  friend class GraphImpl;
public:
  GraphView(Graph *supergraph, BooleanProperty *filter, unsigned int id = 0);
  ~GraphView();
  //========================================================================
  node addNode();
  void addNodes(unsigned int nb, std::vector<node>& addedNodes);
  void addNode(const node);
  void addNodes(Iterator<node>* nodes);
  edge addEdge(const node n1,const node n2);
  void addEdges(const std::vector<std::pair<node, node> >& edges,
                std::vector<edge>& addedEdges);
  void addEdge(const edge);
  void addEdges(Iterator<edge>* edges);
  void delNode(const tlp::node n, bool deleteInAllGraphs = false);
  void delEdge(const tlp::edge e, bool deleteInAllGraphs = false);
  void setEdgeOrder(const node,const std::vector<edge> & );
  void swapEdgeOrder(const node,const edge , const edge );
  //=========================================================================
  bool isElement(const node ) const;
  bool isElement(const edge ) const;
  edge existEdge(const node source, const node target,
                 bool directed) const;
  unsigned int numberOfNodes() const;
  unsigned int numberOfEdges() const;
  //=========================================================================
  unsigned int deg(const node) const;
  unsigned int indeg(const node) const;
  unsigned int outdeg(const node) const;
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
  //=========================================================================
  // only implemented on a root graph
  virtual void reserveNodes(unsigned int nbNodes);
  virtual void reserveEdges(unsigned int nbEdges);
  //=========================================================================
  // updates management
  virtual void push(bool unpopAllowed = true,
                    std::vector<PropertyInterface*>* propertiesToPreserveOnPop= NULL);
  virtual void pop(bool unpopAllowed = true);
  virtual void unpop();
  virtual bool canPop();
  virtual bool canUnpop();
  virtual bool canPopThenUnpop();

protected:
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  virtual node restoreNode(node);
  virtual void restoreNodes(const std::vector<node>&);
  virtual edge restoreEdge(edge, node source, node target);
  virtual void restoreEdges(const std::vector<edge>& edges,
                            const std::vector<std::pair<node, node> >& ends);
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node);
  virtual void removeEdge(const edge);
  void removeNode(const node n, const std::vector<edge>& edges);
  void removeEdges(const std::vector<edge>& edges);

private:
  MutableContainer<bool> nodeAdaptativeFilter;
  MutableContainer<bool> edgeAdaptativeFilter;
  MutableContainer<unsigned int> outDegree;
  MutableContainer<unsigned int> inDegree;
  mutable int nNodes;
  mutable int nEdges;
  edge addEdgeInternal(edge);
  void delEdgeInternal(const edge);
  void delNodeInternal(const node);
  void reverseInternal(const edge, const node src, const node tgt);
  void setEndsInternal(const edge, const node src, const node tgt,
                       const node newSrc, const node newTgt);
};

}
#endif

#endif



///@endcond
