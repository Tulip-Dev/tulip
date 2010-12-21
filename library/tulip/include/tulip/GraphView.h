/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_SUPERGRAPHVIEW_H
#define Tulip_SUPERGRAPHVIEW_H

#ifndef DOXYGEN_NOTFOR_USER

#include "tulip/GraphAbstract.h"
#include "tulip/GraphImpl.h"
#include "tulip/MutableContainer.h"

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
  edge addEdge(const node n1,const node n2);
  void addNode(const node);
  void addEdge(const edge);
  void delNode(const node);
  void delEdge(const edge);
  void delAllNode(const node);
  void delAllEdge(const edge);
  void setEdgeOrder(const node,const std::vector<edge> & );
  void swapEdgeOrder(const node,const edge , const edge );
  //=========================================================================
  bool isElement(const node ) const;
  bool isElement(const edge ) const;
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
  //=========================================================================
  // only implemented on a root graph
  virtual void reserveNodes(unsigned int nbNodes);
  virtual void reserveEdges(unsigned int nbEdges);
  //=========================================================================
  // updates management
  virtual void push(bool unpopAllowed = true);
  virtual void pop(bool unpopAllowed = true);
  virtual bool nextPopKeepPropertyUpdates(PropertyInterface* prop);
  virtual void unpop();
  virtual bool canPop();
  virtual bool canUnpop();

protected:
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  virtual node restoreNode(node);
  virtual edge restoreEdge(edge, node source, node target);
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node);
  virtual void removeEdge(const edge, const node= node());

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
  void reverse(const edge, const node src, const node tgt);
  void setEnds(const edge, const node src, const node tgt,
	       const node newSrc, const node newTgt);
};

}
#endif

#endif



