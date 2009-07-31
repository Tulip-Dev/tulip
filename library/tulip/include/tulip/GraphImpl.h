//-*-c++-*-
#ifndef Tulip_SUPERGRAPHIMPL_H
#define Tulip_SUPERGRAPHIMPL_H

#ifndef DOXYGEN_NOTFOR_USER

#include <set>

#include <vector>
#include <ext/slist>
#include "tulip/GraphAbstract.h"
#include "tulip/IdManager.h"
#include "tulip/SimpleVector.h"
#include "tulip/MutableContainer.h"

namespace tlp {

class GraphView;
class GraphUpdatesRecorder;
template<class C>class Iterator;
class Int;

///Implementation of the graph support of the Graph.
class GraphImpl:public GraphAbstract, public Observer {

  friend class xSGraphNodeIterator;
  friend class xSGraphEdgeIterator;
  friend class xInOutEdgesIterator;
  friend class xOutEdgesIterator;
  friend class xInEdgesIterator;
  friend class xInOutNodesIterator;
  friend class GraphUpdatesRecorder;
  

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
  // updates management
  virtual void push(bool unpopAllowed = true);
  virtual void pop(bool unpopAllowed = true);
  virtual bool nextPopKeepPropertyUpdates(PropertyInterface* prop);
  virtual void unpop();
  virtual bool canPop();
  virtual bool canUnpop();

  // observer interface
  void update(std::set<Observable *>::iterator begin ,std::set<Observable *>::iterator end);
  void observableDestroyed(Observable*);

  // for subgraph id management
  unsigned int getSubGraphId(unsigned int id);
  void freeSubGraphId(unsigned int id);

protected:
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  virtual node restoreNode(node);
  virtual edge restoreEdge(edge, node source, node target);
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node);
  virtual void removeEdge(const edge, const node = node());

private :
  typedef SimpleVector<edge> EdgeContainer;
  typedef std::vector<EdgeContainer> Nodes;
  typedef std::vector<std::pair< node , node > > Edges;
  MutableContainer<unsigned int> outDegree;
  mutable Edges edges;
  mutable Nodes nodes;
  IdManager graphIds;
  IdManager nodeIds;
  IdManager edgeIds;
  unsigned int nbNodes;
  unsigned int nbEdges;
  stdext::slist<GraphUpdatesRecorder*> previousRecorders;
  stdext::slist<Graph *> observedGraphs;
  stdext::slist<PropertyInterface*> observedProps;
  stdext::slist<GraphUpdatesRecorder*> recorders;

  // methods used in push/pop implementation
  static void removeEdge(EdgeContainer &, const edge);
  edge addEdgeInternal(edge, node source, node target,
		       bool updateContainers);
  void delNodeInternal(const node);
  void restoreContainer(node, std::vector<edge>&);
  void observeUpdates(Graph*);
  void unobserveUpdates();
  void delPreviousRecorders();
};

}
#endif

#endif
