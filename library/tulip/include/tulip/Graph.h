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

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include <climits>
#include "tulipconf.h"
#include "Reflect.h"
#include "PluginContext.h"
#include "PluginProgress.h"
#include "Node.h"
#include "Edge.h"
#include "ObservableGraph.h"

namespace tlp {

class PropertyManager;
class BooleanProperty;
template<class C>class Iterator;

  enum ElementType {NODE=0, EDGE};
  /**
   * Load a sg in the tlp format
   * Warning : this function use "tlp" import plugin (must be laoded)
   */
  TLP_SCOPE Graph * loadGraph(const std::string &filename);
  /**
   * Save a sg in tlp format
   * Warning : this function use "tlp" export plugin (must be laoded)
   */
  TLP_SCOPE bool saveGraph(Graph *, const std::string &filename);
  TLP_SCOPE Graph * importGraph(const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress=0,Graph *newGraph=0);
  TLP_SCOPE bool exportGraph(Graph *sg,std::ostream  &os,const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress=0);
  TLP_SCOPE bool clusterizeGraph(Graph *sg,std::string &errorMsg, DataSet *dataSet =0,const std::string &alg="hierarchical" , PluginProgress *plugProgress=0);
  /**
   * Return a new sg
   */
  TLP_SCOPE Graph* newGraph();
  /**
   *  Return an empty subsg 
   */
  TLP_SCOPE Graph *newSubGraph(Graph *, std::string name = "unnamed");
  /**
   *  Return a subsg equal to the sg given in parameter (a clone subsg)
   */
  TLP_SCOPE Graph *newCloneSubGraph(Graph *, std::string name = "unnamed");
  /**
   *  find the first node of degree 0, if no node exist return false else true
   */
  TLP_SCOPE bool getSource(Graph *, node &n);
  /**
   * Return an istream from a gzipped file (uses gzstream lib)
   * the stream has to be deleted after use.
   * \warning Don't forget to check the stream with ios::bad()!
   */
  /**
   * Append the selected part of the sg inG (properties, nodes & edges) into the sg outG.
   * If no selection is done (inSel=NULL), the whole inG sg is appended.
   * The output selection is used to select the appended nodes & edges
   * \warning The input selection is extended to all selected edge ends.
   */
  TLP_SCOPE void copyToGraph( Graph * outG, Graph *	inG, BooleanProperty* inSelection=0, BooleanProperty* outSelection=0 );
  /**
   * Remove the selected part of the sg ioG (properties, nodes & edges).
   * If no selection is done (inSel=NULL), the whole sg is reseted to default value.
   * \warning The selection is extended to all selected edge ends.
   */
  TLP_SCOPE void removeFromGraph(Graph * ioG, BooleanProperty* inSelection = 0 );

/**
 * \defgroup graphs
 */ 
/*@{*/
/// Interface for a sg
/**
 * The class Graph is the interface of a Graph in the Tulip Library.
 */
class TLP_SIMPLE_SCOPE Graph : public ObservableGraph {

public:  
  Graph();
  virtual ~Graph();
  //=========================================================================  
  // Graph hierarrchy acces and building
  //=========================================================================
  /**
   *  Remove all nodes, edges and subgraphs of the supergraph 
   */
  virtual  void clear()=0;
  /**
   * Create and return a new SubGraph of the sg
   * The elements of the new SubGraph is those selected in the selection
   * if there is no selection an empty SubGraph is return.
   */
  virtual Graph *addSubGraph(BooleanProperty *selection=0)=0;
  /**
   * Del a SubGraph of this sg.
   * The SubGraph's SubGraphs become SubGraphs of the sg.
   */
  virtual void delSubGraph(Graph *)=0;
  /**
   * Del the SubGraph of the sg and all its SubGraphs.
   */
  virtual void delAllSubGraphs(Graph *)=0;
  /**
   * Return the Graph(UpperGraph of the sg), if it has no Graph it 
   * returns itself
   */
  virtual Graph* getFather()const =0;
  /**
   * Return the root sg of the sg hierarchy
   */
  virtual Graph* getRoot() const =0;
  /**
   * Set the Graph of a sg (use very carefully)
   * Normal user should never use this function.
   */
  virtual void setFather(Graph *)=0;
  /**
   * Return an iterator on all the SubGraphs of the sg
   */
  virtual Iterator<Graph *> * getSubGraphs() const=0;
  //==============================================================================
  // Modification of the sg structure
  //==============================================================================
  /** 
   * Add a new node in the sg and return it. This node is also added in all 
   * the Graph of the sg to maintain the sub_graph relation between graphs.
   */
  virtual node addNode()=0;
  /** 
   * Add an existing node in the sg. this node is also added in all 
   * the Graph of the sg to maintain the sub_graph relation between graphs.
   * Warning, the node must be element of the sg hierarchy, thus it must be 
   * element of the root sg.
   */
  virtual void addNode(const node)=0;
  /**
   * Delete a node in the sg. this node is also removed in all 
   * the sub-graph of the sg to maintain the sub-graph relation between graphs.
   * Warning : One can't add an existing node to the root sg
   */
  virtual void delNode(const node)=0;
  /**
   * Delete a node in all the hierarchy of graphs.
   */
  virtual void delAllNode(const node)=0;
  /** 
   * Add a new edge in the sg and return it. This edge is also added in all 
   * the super-graph of the sg to maintain the sub-graph relation between graphs. 
   * If the second parameter is true the ordering of edges will be preserved.
   */
  virtual edge addEdge(const node, const node )=0;
  /** 
   * Add an existing edge in the sg. this edge is also added in all 
   * the super-graph of the sg to maintain the sub-graph relation between graphs.
   * Warning, the edge must be element of the sg hierarchy, thus it must be 
   * element of the root sg.
   * Warning : One can't add an existing edge to the root sg
   */
  virtual void addEdge(const edge )=0;
  /**
   * Delete an edge in the sg. this edge is also removed in all 
   * the sub-graph of the sg to maintain the sub-graph relation between graphs.
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
   *  Warning : The ordering is global to the entire sg hierarchy. Thus, by changing
   *  The ordering of a sg you change the ordering of the hierarchy.
   */
  virtual void reverse(const edge)=0;
  //================================================================================
  //Iterators on the sg structure.
  //================================================================================
  /// Return an existing node of the sg
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
  /// Return an iterator on the edges.
  virtual Iterator<edge>* getEdges() const =0;
  /// Return an existing edge of the sg
  virtual edge getOneEdge() const =0;
  /// Return an iterator on the out-edges of a node.
  virtual Iterator<edge>* getOutEdges(const node) const =0;
  /// Return an iterator on the in-out-edges of a node.
  virtual Iterator<edge>* getInOutEdges(const node) const =0;
  /// Return an iterator on the in--edges of a node.
  virtual Iterator<edge>* getInEdges(const node) const =0;
  //================================================================================
  // Graph, nodes and adges informations about the sg stucture
  //================================================================================
  /// Return the sg's id, this id is unique.
  int getId() const {return id;}
  /// Return the number of nodes in the sg
  virtual unsigned int numberOfNodes()const =0;
  /// Return the number of edges in the sg
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
  /// Return the opposite node for s in the edge e
  virtual node opposite(const edge, const node)const =0;
  /// Return true if the node is element of the sg.
  virtual bool isElement(const node ) const =0;
  /// Return true if the edge is element of the sg.
  virtual bool isElement(const edge ) const =0;
  /** Return the edge if it exists an edge between two node 
   *  sens of the edge is not taken into account)
   *  If no edge is found return an invalid edge.
   */
  virtual edge existEdge(const node , const node) const =0;
  //================================================================================
  // Access to the sg attributes and to the node/edge property.
  //================================================================================
  ///Return sg attributes
  virtual DataSet & getAttributes() =0;
  ///Get an attribute of the sg
  template<typename ATTRIBUTETYPE> 
  ATTRIBUTETYPE getAttribute(const std::string &name);
  ///Set an attribute of the sg
  template<typename ATTRIBUTETYPE> 
  void setAttribute(const std::string &name,const ATTRIBUTETYPE &value);
  /**
   * Return a pointer to a propertyProxy which is in the pool.
   * The real type of the propertyproxy is tested with the template parameter.
   * If the propertyProxy is not in the pool, a new one is created and return.
   */
  template<typename Proxytype> 
  Proxytype* getLocalProperty(const std::string &name);
  /**
   * Compute a property on this sg using an external algorithm (plug-in) 
   * The result is stored in result, Warning all information in result will be deleted
   * If the function return false error message are stored in msg.
   * One can give a PluginProgress to the algortihm in order to have feed back or to stop
   * the algorithm during its computation. One can give parameter to the algorithm 
   * using the DataSet. In some cases algorithms can use this DataSet in order 
   * to give as result external information (not stored in result).
   */
  template<typename Proxytype>
  bool computeProperty(const std::string &algorithm, Proxytype result, std::string &msg, 
		       PluginProgress *progress=0, DataSet *data=0);
  /**
   * Return a pointer to a propertyProxy which is in the pool or in the pool of an ascendant
   * The real type of the propertyproxy is tested with the template parameter.
   * If the propertyProxy is not the pool it creates a new one and return it.
   */
  template<typename Proxytype>
  Proxytype* getProperty(const std::string &name);
  /**
   * Return a pointer to a propertyProxy which is in the pool or in the pool of an ascendant.
   * The real type of the propertyproxy is tested with the template parameter.
   * If the propertyProxy is in the pool the cached value return true else false.
   * The resultBool value indicate if the check algorithm of the associated property was ok;
   * The resultStr is the error message which has been return by the Property.
   */
  /**
   * Return a pointer on an existing property. If the property does not 
   * exist return 0.
   * In DEBUG the existence of a property is checked using an assertion.
   */
  virtual PropertyInterface* getProperty(const std::string &name)=0;  
  /**
   *  Return true if the propertyProxy exists
   *  in the Graph or in a Graph
   */
  virtual  bool existProperty(const std::string&name)=0;
  /**
   * Return true if the propertyProxy is in the sg
   */
  virtual  bool existLocalProperty(const std::string&name)=0;
  /**
   * Remove a AbstractProperty from the sg
   */
  virtual  void delLocalProperty(const std::string&name)=0;
  /**
   * Return an iterator on the local properties
   */
  virtual Iterator<std::string>* getLocalProperties()=0;
  /**
   * Return an iterator on the inherited properties
   */
  virtual Iterator<std::string>* getInheritedProperties()=0;
  /**
   * Return an iterator on the properties
   */
  virtual Iterator<std::string>* getProperties()=0;

private:
  /**
   *  Return a pointer on the porperty proxy container associated to the 
   *  supergraph.
   */
  virtual  PropertyManager *getPropertyManager()=0;

  unsigned int id;
};

}

///Print the sg (only nodes and edges) in ostream, in the tulip format
TLP_SCOPE std::ostream& operator<< (std::ostream &,const tlp::Graph *);
/*@}*/
//================================================================================
// Specilization of some template class
//================================================================================
#ifndef DOXYGEN_NOTFOR_DEVEL

namespace stdext {
  template <>
  struct TLP_SCOPE hash<const tlp::Graph *> {
    size_t operator()(const tlp::Graph *s) const {return size_t(s->getId());}
  };
  template <>
  struct TLP_SCOPE hash<tlp::Graph *> {
    size_t operator()(tlp::Graph *s) const {return size_t(s->getId());}
  };
}

#endif // DOXYGEN_NOTFOR_DEVEL
//include the template code
#include "cxx/Graph.cxx"
#endif
