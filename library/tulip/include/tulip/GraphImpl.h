//-*-c++-*-
#ifndef Tulip_SUPERGRAPHIMPL_H
#define Tulip_SUPERGRAPHIMPL_H

#ifndef DOXYGEN_NOTFOR_USER

#include <set>

#if (__GNUC__ < 3)
#include <hash_set>
#else
#include <ext/hash_set>
#endif

#include <vector>
#include "GraphAbstract.h"
#include "IdManager.h"
#include "SimpleVector.h"
#include "MutableContainer.h"

namespace tlp {

class GraphView;
template<class C>class Iterator;
class Int;

///Implementation of the graph support of the Graph.
class GraphImpl:public GraphAbstract {

  friend class xSGraphNodeIterator;
  friend class xSGraphEdgeIterator;
  friend class xInOutEdgesIterator;
  friend class xOutEdgesIterator;
  friend class xInEdgesIterator;
  friend class xInOutNodesIterator;

public:
  GraphImpl();
  ~GraphImpl();
  void clear();
  //=========================================================================
  bool isElement(const node ) const;
  bool isElement(const edge ) const;
  node addNode();
  void addNode(const node);
  edge addEdge(const node ,const node);
  void addEdge(const edge);
  void delNode(const node);
  void delEdge(const edge );
  void delAllNode(const node );
  void delAllEdge(const edge );
  void setEdgeOrder(const node,const std::vector<edge> & );
  void swapEdgeOrder(const node,const edge , const edge );
  //=========================================================================
  Iterator<node>* getNodes() const;
  Iterator<node>* getInNodes(const node )const;
  Iterator<node>* getOutNodes(const node )const;
  Iterator<node>* getInOutNodes(const node )const;
  Iterator<edge>* getEdges()const;
  Iterator<edge>* getInEdges(const node )const;
  Iterator<edge>* getOutEdges(const node )const;
  Iterator<edge>* getInOutEdges(const node )const;
  //================================================================================
  unsigned int deg(const node ) const;
  unsigned int indeg(const node) const;
  unsigned int outdeg(const node) const;
  //================================================================================
  virtual node source(const edge) const;
  virtual node target(const edge) const;
  virtual void reverse(const edge);
  //================================================================================
  unsigned int numberOfEdges()const;
  unsigned int numberOfNodes()const;
  //================================================================================

private :
  typedef SimpleVector<edge> EdgeContainer;
  typedef std::vector<EdgeContainer> Nodes;
  typedef std::vector<std::pair< node , node > > Edges;
  MutableContainer<unsigned int> outDegree;
  mutable Edges edges;
  mutable Nodes nodes;
  IdManager nodeIds;
  IdManager edgeIds;
  unsigned int nbNodes;
  unsigned int nbEdges;

  static void removeEdge(EdgeContainer &, const edge);
  void externRemove(const edge);
  void externRemove(const node);
};

}
#endif

#endif
