//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 22/05/2003
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_SUPERGRAPH_H
#define Tulip_SUPERGRAPH_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <vector>

#include <climits>
#include "tulip/tulipconf.h"
#include "tulip/Reflect.h"
#include "tulip/PluginContext.h"
#include "tulip/PluginProgress.h"
#include "tulip/Node.h"
#include "tulip/Edge.h"
#include "tulip/Observable.h"
#include "tulip/ObservableGraph.h"

namespace tlp {

class BooleanProperty;
template<class C>class Iterator;

  enum ElementType {NODE=0, EDGE};
  /**
   * Load a graph in the tlp format
   * Warning : this function use "tlp" import plugin (must be laoded)
   */
  TLP_SCOPE Graph * loadGraph(const std::string &filename);
  /**
   * Save a graph in tlp format
   * Warning : this function use "tlp" export plugin (must be laoded)
   */
  TLP_SCOPE bool saveGraph(Graph *, const std::string &filename);
  TLP_SCOPE Graph * importGraph(const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress=0,Graph *newGraph=0);
  TLP_SCOPE bool exportGraph(Graph *graph,std::ostream  &os,const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress=0);
  TLP_SCOPE bool applyAlgorithm(Graph *graph,std::string &errorMsg, DataSet *dataSet =0,const std::string &alg="any", PluginProgress *plugProgress=0);
  /**
   * Returns a new graph
   */
  TLP_SCOPE Graph* newGraph();
  /**
   *  Return an empty subgraph
   */
  TLP_SCOPE Graph *newSubGraph(Graph *, std::string name = "unnamed");
  /**
   *  Return a subgraph equal to the graph given in parameter (a clone subgraph)
   */
  TLP_SCOPE Graph *newCloneSubGraph(Graph *, std::string name = "unnamed");
  /**
   *  find the first node of degree 0, if no node exist return false else true
   */
  TLP_SCOPE bool getSource(Graph *, node &n);
  /**
   * Returns an istream from a gzipped file (uses gzstream lib)
   * the stream has to be deleted after use.
   * \warning Don't forget to check the stream with ios::bad()!
   */
  /**
   * Append the selected part of the graph inG (properties, nodes & edges) into the graph outG.
   * If no selection is done (inSel=NULL), the whole inG graph is appended.
   * The output selection is used to select the appended nodes & edges
   * \warning The input selection is extended to all selected edge ends.
   */
  TLP_SCOPE void copyToGraph( Graph *outG, Graph *inG, BooleanProperty* inSelection=0, BooleanProperty* outSelection=0 );
  /**
   * Remove the selected part of the graph ioG (properties, nodes & edges).
   * If no selection is done (inSel=NULL), the whole graph is reseted to default value.
   * \warning The selection is extended to all selected edge ends.
   */
  TLP_SCOPE void removeFromGraph(Graph * ioG, BooleanProperty* inSelection = 0 );

/**
 * \defgroup graphs
 */
/*@{*/
/// Interface for a graph
/**
 * The class Graph is the interface of a Graph in the Tulip Library.
 */
  class TLP_SIMPLE_SCOPE Graph : public Observable, public ObservableGraph {

  friend class GraphAbstract;
  friend class GraphUpdatesRecorder;
  friend class GraphDecorator;
  friend class PropertyManager;

public:
  Graph():id(0) {}
  virtual ~Graph() {}
  //=========================================================================
  // Graph hierarchy acces and building
  //=========================================================================
  /**
   *  Remove all nodes, edges and subgraphs of the supergraph
   */
  virtual  void clear()=0;
  /**
   * Creates and returns a new SubGraph of the graph
   * The elements of the new SubGraph are those selected in the selection
   * if there is no selection an empty SubGraph is return.
   */
  virtual Graph *addSubGraph(BooleanProperty *selection=0,
			     unsigned int id = 0)=0;
  /**
   *  Creates and returns the subgraph induced by a set of nodes
   */
  Graph *inducedSubGraph(const std::set<node>& nodeSet);
  /**
   * Delete a SubGraph of this graph.
   * The SubGraph's SubGraphs become SubGraphs of the graph.
   */
  virtual void delSubGraph(Graph *)=0;
  /**
   * Del the SubGraph of the graph and all its SubGraphs.
   */
  virtual void delAllSubGraphs(Graph *)=0;
  /**
   * Returns the parent of the graph,
   *  if it has no parent (is the root graph), it returns itself.
   */
  virtual Graph* getSuperGraph()const =0;
  /**
   * Deprecated function, use getSuperGraph() instead.
   */
  Graph* getFather()const {
    std::cerr << __PRETTY_FUNCTION__ << " is deprecated, use getSuperGraph() instead." << std::endl;
    return getSuperGraph();
  }
  /**
   * Returns the root graph of the graph hierarchy
   */
  virtual Graph* getRoot() const =0;
  /**
   * Set the parent of a graph (use very carefully)
   * Standard user should never use this function.
   */
  virtual void setSuperGraph(Graph *)=0;
  /**
   * Deprecated function, use setSuperGraph() instead.
   */
  void setFather(Graph *sg) {
    std::cerr << __PRETTY_FUNCTION__ << " is deprecated, use setSuperGraph() instead." << std::endl;
    setFather(sg);
  }
  /**
   * Returns an iterator on all the SubGraphs of the graph
   */
  virtual Iterator<Graph *> * getSubGraphs() const=0;
  /**
   * indicates if the graph argument is a direct subgraph
   */
  virtual bool isSubGraph(Graph* sg) const=0;
  /**
   * indicates if the graph argument is a descendant of this graph
   */
  virtual bool isDescendantGraph(Graph* sg) const=0;
  /**
   * Returns a pointer on the subgraph with the corresponding id
   * or NULL if there is no subgraph with that id
   */
  virtual Graph* getSubGraph(unsigned int id) const=0;
  /**
   * Returns a pointer of the descendant with the corresponding id
   * or NULL if there is no descendant  with that id
   */
  virtual Graph* getDescendantGraph(unsigned int id) const=0;
  //==============================================================================
  // Modification of the graph structure
  //==============================================================================
  /**
   * Add a new node in the graph and return it. This node is also added in all
   * the graph ancestors to maintain the sub_graph relation between graphs.
   */
  virtual node addNode()=0;
  /**
   * Add an existing node in the graph. this node is also added in all
   * the graph ancestors to maintain the sub_graph relation between graphs.
   * Warning, the node must be element of the graph hierarchy, thus it must be
   * element of the root graph.
   */
  virtual void addNode(const node)=0;
  /**
   * Delete a node in the graph. this node is also removed in all
   * the sub-graph of the graph to maintain the sub-graph relation between graphs.
   * Warning : One can't add an existing node to the root graph
   */
  virtual void delNode(const node)=0;
  /**
   * Delete a node in all the hierarchy of graphs.
   */
  virtual void delAllNode(const node)=0;
  /**
   * Add a new edge in the graph and return it. This edge is also added in all
   * the super-graph of the graph to maintain the sub-graph relation between graphs.
   * If the second parameter is true the ordering of edges will be preserved.
   */
  virtual edge addEdge(const node, const node )=0;
  /**
   * Add an existing edge in the graph. this edge is also added in all
   * the super-graph of the graph to maintain the sub-graph relation between graphs.
   * Warning, the edge must be element of the graph hierarchy, thus it must be
   * element of the root graph.
   * Warning : One can't add an existing edge to the root graph
   */
  virtual void addEdge(const edge )=0;
  /**
   * Delete an edge in the graph. this edge is also removed in all
   * the sub-graph of the graph to maintain the sub-graph relation between graphs.
   * The ordering of edges is preserved.
   */
  virtual  void delEdge(const edge )=0;
  /**
   * Delete an edge in all the hierarchy of graphs.
   * The ordering of edges around the node
   * is preserved.
   */
  virtual  void delAllEdge(const edge)=0;
  /**
   * Set the order of the edges around a node. This operation
   * ensure that adjacent edges of a node will
   * be odered and consecutive as they are in the vector given in parameter.
   */
  virtual void setEdgeOrder(const node,const std::vector<edge> & )=0;
  /**
   * Swap two edges in the adjacent list of a node.
   */
  virtual void swapEdgeOrder(const node,const edge , const edge )=0;
  /**
   *  Reverse the direction of an edge, the source becomes the target and the target.
   *  becomes the source.
   *  Warning : The ordering is global to the entire graph hierarchy. Thus, by changing
   *  The ordering of a graph you change the ordering of the hierarchy.
   */
  virtual void reverse(const edge)=0;
  //================================================================================
  //Iterators on the graph structure.
  //================================================================================
  /// Return an existing node of the graph
  virtual node getOneNode() const =0;
  ///Return an iterator on the nodes.
  virtual Iterator<node>* getNodes() const =0;
  /// Return the ith successor of a node
  virtual node getInNode(const node,unsigned int )const =0;
  /// Return an iterator on the predecessors of a node.
  virtual Iterator<node>* getInNodes(const node) const =0;
  /// Return the ith predecessor of a node
  virtual node getOutNode(const node,unsigned int) const =0;
  /// Return an iterator on the successors of a node.
  virtual Iterator<node>* getOutNodes(const node) const =0;
  /// Return an iterator on the neighbours of a node.
  virtual Iterator<node>* getInOutNodes(const node) const =0;
  /// Returns the underlying graph of a meta node
  virtual Graph* getNodeMetaInfo(const node) const = 0;
  /// Return an iterator on the edges.
  virtual Iterator<edge>* getEdges() const =0;
  /// Return an existing edge of the graph
  virtual edge getOneEdge() const =0;
  /// Return an iterator on the out-edges of a node.
  virtual Iterator<edge>* getOutEdges(const node) const =0;
  /// Return an iterator on the in-out-edges of a node.
  virtual Iterator<edge>* getInOutEdges(const node) const =0;
  /// Return an iterator on the in--edges of a node.
  virtual Iterator<edge>* getInEdges(const node) const =0;
  /// Return an iterator on the underlying edges of a meta edge
  virtual Iterator<edge>* getEdgeMetaInfo(const edge) const =0;
  //================================================================================
  // Graph, nodes and edges informations about the graph stucture
  //================================================================================
  /// Return the graph's id, this id is unique.
  unsigned int getId() const {return id;}
  /// Return the number of nodes in the graph
  virtual unsigned int numberOfNodes()const =0;
  /// Return the number of edges in the graph
  virtual unsigned int numberOfEdges()const =0;
  /// Return degree of a node.
  virtual unsigned int deg(const node)const =0;
  /// Return indegree of a node.
  virtual unsigned int indeg(const node)const =0;
  /// Return outdegree of a node.
  virtual unsigned int outdeg(const node)const =0;
  /// Return the source of the edge.
  virtual node source(const edge)const =0;
  /// Return the target of the edge.
  virtual node target(const edge)const =0;
  /// Return the source and target of the edge
  /// as the first and second of the returned pair
  virtual const std::pair<node, node>& ends(const edge)const=0;
  /// Return the opposite node for s in the edge e
  virtual node opposite(const edge, const node)const =0;
  /// Return true if the node is element of the graph.
  virtual bool isElement(const node ) const =0;
  /// Return true if the node is a meta node.
  virtual bool isMetaNode(const node ) const =0;
  /// Return true if the edge is element of the graph.
  virtual bool isElement(const edge ) const =0;
  /// Return true if the edge is a meta edge.
  virtual bool isMetaEdge(const edge ) const =0;
  /** Returns the edge if it exists an edge between two nodes
   *  the 'directed' flag indicates if the direction of the edge
   *  (from source to target) must be taken in to account).
   *  If no edge is found return an invalid edge.
   */
  virtual edge existEdge(const node source, const node target,
			 bool directed = true) const =0;
  //================================================================================
  // Access to the graph attributes and to the node/edge property.
  //================================================================================
  ///Return graph attributes
  const DataSet & getAttributes() const {
    return (const_cast<Graph *>(this))->getNonConstAttributes();
  }
  ///Get an attribute of the graph; returns true if a value was found
  ///false if not
  template<typename ATTRIBUTETYPE>
  bool getAttribute(const std::string &name, ATTRIBUTETYPE& value) const;
  ///deprecated version of the previous method
  template<typename ATTRIBUTETYPE>
  ATTRIBUTETYPE getAttribute(const std::string &name) const;
  /// Untyped accessor returning a copy
  DataType* getAttribute(const std::string& name) const;
  ///Set an attribute of the graph
  template<typename ATTRIBUTETYPE>
  void setAttribute(const std::string &name,const ATTRIBUTETYPE &value);
  /// set attritute from an untyped value
  void setAttribute(const std::string &name, const DataType* value);
  /// remove an existing attribute
  void removeAttribute(const std::string &name) {
    notifyRemoveAttribute(this, name);
    getNonConstAttributes().remove(name);
  }
  /// return if the attribute exist
  bool attributeExist(const std::string &name) {
    return getAttributes().exist(name);
  }
  /**
   *  add a property to the graph
   *  Be aware that the PropertyInterface will now belong to the graph object;
   *  and so it will be deleted automatically. Using of delete on that property
   *  will cause a segmentation violation (use delLocalProperty instead).
   */
  virtual  void addLocalProperty(const std::string &name, PropertyInterface *prop)=0;
  /**
   * Returns a pointer to a PropertyInterface which is in the pool.
   * The real type of the PropertyInterface is tested with the template parameter.
   * If the PropertyInterface is not in the pool, a new one is created and returned.
   * Using of delete on that property will cause a segmentation violation
   * (use delLocalProperty instead).
   */
  template<typename PropertyType>
  PropertyType* getLocalProperty(const std::string &name);
  /**
   * Computes a property on the current graph, if it is not empty,
   * using an external named algorithm (plug-in).
   * The result is stored in result. Warning: all information in result will be deleted
   * If the function returns false error message are stored in msg.
   * One can give a PluginProgress to the algortihm in order to have feed back or to stop
   * the algorithm during its computation. One can give parameter to the algorithm
   * using the DataSet. In some cases algorithms can use this DataSet in order
   * to give as result external information (not stored in result).
   */
  template<typename PropertyType>
  bool computeProperty(const std::string &algorithm, PropertyType* result, std::string &msg,
		       PluginProgress *progress=0, DataSet *data=0);
  /**
   * Returns a pointer to a PropertyInterface which is in the pool or in the pool of an ascendant
   * The real type of the PropertyInterface is tested with the template parameter.
   * If the PropertyInterface is not the pool it creates a new one and return it.
   * Using of delete on that property will cause a segmentation violation
   * (use delLocalProperty instead).
   */
  template<typename PropertyType>
  PropertyType* getProperty(const std::string &name);
  /**
   * Returns a pointer on an existing property. If the property does not
   * exist return NULL.
   * In DEBUG the existence of a property is checked using an assertion.
   */
  virtual PropertyInterface* getProperty(const std::string& name)=0;
  /**
   *  Return true if a property of that name exists
   *  in the graph or in an ancestor
   */
  virtual  bool existProperty(const std::string& name)=0;
  /**
   * Returns true if a property of that name exists is in the graph
   */
  virtual  bool existLocalProperty(const std::string& name)=0;
  /**
   * Remove a property from the graph
   */
  virtual  void delLocalProperty(const std::string& name)=0;
  /**
   * Returns an iterator on the names of local properties
   */
  virtual Iterator<std::string>* getLocalProperties()=0;
  /**
   * Returns an iterator on the names of the inherited properties
   */
  virtual Iterator<std::string>* getInheritedProperties()=0;
  /**
   * Returns an iterator on the name of all the properties
   */
  virtual Iterator<std::string>* getProperties()=0;
  /**
   * Returns an iterator on the local properties
   */
  virtual Iterator<PropertyInterface*>* getLocalObjectProperties()=0;
  /**
   * Returns an iterator on the inherited properties
   */
  virtual Iterator<PropertyInterface*>* getInheritedObjectProperties()=0;
  /**
   * Returns an iterator on all the properties
   */
  virtual Iterator<PropertyInterface*>* getObjectProperties()=0;

  // updates management
  /*
   * Marks the state of the current root graph hierarchy.
   * The next updates will be recorded in order to be undone at the
   * next call of the pop method. Be careful that all the updates are undone
   * except those who affect the ordering of edges.
   * If the argument unpopAllowed is set to false, the next updates
   * could not be replayed after undone. If some previously undone
   * updates exist they could no longer be replayed.
   */
  virtual void push(bool unpopAllowed = true)=0;
  /*
   * Restores a previously marked state of the current root graph
   * hierarchy. The restored state does not remain marked.
   * If the argument unpopAllowed is set to false then
   * the undone updates could not be replayed.
   */
  virtual void pop(bool unpopAllowed = true)=0;
  /*
   * Indicates that next updates of the property given in argument
   * will be preserved during the next call of the pop method.
   * Returns false if updates of that property are already recorded
   * since the last call of the push method.
   */
  virtual bool nextPopKeepPropertyUpdates(PropertyInterface* prop)=0;
  /*
   * Marks again the current state of the root graph hierarchy
   * and replays the last updates previously undone.
   */
  virtual void unpop()=0;
  /*
   * Returns true if a previously marked state can be restored.
   */
  virtual bool canPop()=0;
  /*
   * Returns true if some previously undone updates can be replayed.
   */
  virtual bool canUnpop()=0;

  // meta nodes management
  /**
   * method to close a set of existing nodes into a metanode.
   * Edges from nodes in the set to other nodes are replaced with
   * edges from the metanode to the other nodes.
   * Warning: this method will failed when called on the root graph.
   *
   * \param nodeSet: a set of existing nodes
   * \param multiEdges: indicates if a meta edge will be created for each underlying edge
   * \param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
   */
  node createMetaNode(const std::set<node> &nodeSet, bool multiEdges = true, bool delAllEdge = true);
  /**
   * method to populate a quotient graph with one meta node
   * for each iterated graph
   *
   * \param itS: an Graph iterator, (typically a subgraph iterator)
   * \param quotientGraph: the graph containing the meta nodes
   * \param metaNodes: will contains all the added meta nodes after the call
   *
   */
  void createMetaNodes(Iterator<Graph *> *itS, Graph *quotientGraph,
		       std::vector<node>& metaNodes);
  /**
   * Method to close an existing subgraph into a metanode.  Edges from nodes
   * in the subgraph to nodes outside the subgraph are replaced with
   * edges from the metanode to the nodes outside the subgraph.
   * Warning: this method will failed when called on the root graph.
   *
   * \param subGraph: an existing subgraph
   * \param multiEdges: indicates if a meta edge will be created for each underlying edge
   * \param delAllEdge: indicates if the underlying edges will be removed from the entire hierarchy
   */
  node createMetaNode(Graph* subGraph, bool multiEdges = true, bool delAllEdge = true);
  /**
   * Method to open a metanode and replace all edges between that
   * meta node and other nodes in the graph.
   * Warning: this method will failed when called on the root graph.
   */
  void openMetaNode(node n);

protected:
  virtual DataSet &getNonConstAttributes() = 0;
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  virtual node restoreNode(node)=0;
  virtual edge restoreEdge(edge, node source, node target)=0;
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node)=0;
  virtual void removeEdge(const edge, const node=node())=0;

  // to check if a property can be deleted
  // used by PropertyManager
  virtual bool canDeleteProperty(Graph* g, PropertyInterface *prop) {
    return getRoot()->canDeleteProperty(g, prop);
  }

  // to deal with sub graph deletion
  virtual void removeSubGraph(Graph*)=0;
  virtual void clearSubGraphs()=0;
  // only called by GraphUpdatesRecorder
  virtual void restoreSubGraph(Graph*, bool restoreSubGraphs = false)=0;
  virtual void setSubGraphToKeep(Graph*)=0;
  // overload of inherited methods
  // used to manage push/pop
  void notifyDestroy();
  void notifyAddSubGraph(Graph*);

protected:

  unsigned int id;
  std::set<tlp::PropertyInterface*> circularCalls;
};

}

///Print the graph (only nodes and edges) in ostream, in the tulip format
TLP_SCOPE std::ostream& operator<< (std::ostream &,const tlp::Graph *);
/*@}*/
//================================================================================
// Specilization of some template class
//================================================================================
#ifndef DOXYGEN_NOTFOR_DEVEL

TLP_BEGIN_HASH_NAMESPACE {
  template <>
  struct TLP_SCOPE hash<const tlp::Graph *> {
    size_t operator()(const tlp::Graph *s) const {return size_t(s->getId());}
  };
  template <>
  struct TLP_SCOPE hash<tlp::Graph *> {
    size_t operator()(tlp::Graph *s) const {return size_t(s->getId());}
  };
} TLP_END_HASH_NAMESPACE

#endif // DOXYGEN_NOTFOR_DEVEL
//include the template code
#include "cxx/Graph.cxx"
#endif
