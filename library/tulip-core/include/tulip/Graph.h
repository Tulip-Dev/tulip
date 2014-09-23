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

#ifndef Tulip_SUPERGRAPH_H
#define Tulip_SUPERGRAPH_H

#include <iostream>
#include <string>
#include <vector>

#include <climits>
#include <tulip/tulipconf.h>
#include <tulip/DataSet.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/Observable.h>

namespace tlp {

class PropertyInterface;
class BooleanProperty;
class PluginProgress;
template<class C>struct Iterator;

/**
 * @enum This Enum describes the possible types of an element of the graph.
 *
 * It is used in functions that can return an edge or a node, to distinguish between the two cases.
 **/
enum ElementType {
  /** This element describes a node **/
  NODE = 0,
  /** This element describes an edge **/
  EDGE = 1
};

/**
 * @ingroup Graph
 * @brief Loads a graph in the tlp format from a file (extension can be .tlp or .tlp.gz).
 * This function uses the "TLP Import" import plugin, and will fail if it is not loaded (By default this plugin is linked into the library and should be loaded).
 *
 * If the import fails (no such file, parse error, ...) NULL is returned.
 *
 * @param filename The file in tlp format to parse.
 * @return :Graph* The imported Graph, NULL if the import failed.
 **/
TLP_SCOPE Graph * loadGraph(const std::string &filename, tlp::PluginProgress* progress = NULL);

/**
 * @ingroup Graph
 * @brief Saves the corresponding graph and all its subgraphs to a file using the tlp format. Extension of the file can be either .tlp (human-readable text file) or .tlp.gz (gzipped text file).
 *
 * This function checks the file name for the '.gz' extension and uses a compressed output if found.
 *
 * This function uses the "TLP Export" export plugin, and will fail if it is not loaded (by default this plugin is linked into the library and should be loaded).
 *
 * @param graph the graph to save.
 * @param filename the file to save the graph to.
 * @return bool Whether the export was successfull or not.
 **/
TLP_SCOPE bool saveGraph(Graph* graph, const std::string &filename, tlp::PluginProgress* progress = NULL);

/**
 * @ingroup Graph
 * @brief Exports a graph using the specified export plugin with parameters stored in the DataSet.
 *
 * You determine the destination, whether by using a fstream, or by saving the contents of the stream to the destination of your choice.
 *
 * @param graph The graph to export.
 * @param outputStream The stream to export to. Can be a standard ostream, an ofstream, or even a gzipped ostream.
 * @param format The format to use to export the Graph.
 * @param dataSet The parameters to pass to the export plugin (e.g. additional data, options for the format)
 * @param progress A PluginProgress to report the progress of the operation, as well as final state. Defaults to NULL.
 * @return bool Whether the export was successfull or not.
 **/
TLP_SCOPE bool exportGraph(Graph *graph, std::ostream &outputStream, const std::string &format, DataSet &dataSet, PluginProgress *progress=NULL);

/**
 * @ingroup Graph
 * @brief Imports a graph using the specified import plugin with the parameters stored in the DataSet.
 *
 * If no graph is passed, then a new graph will be created. You can pass a graph in order to import data into it.
 * Returns the graph with imported data, or NULL if the import failed. In this case, the Pluginprogress should have an error that can be displayed.
 *
 * @param format The format to use to import the graph.
 * @param dataSet The parameters to pass to the import plugin (file to read, ...)
 * @param progress A PluginProgress to report the progress of the operation, as well as final state. Defaults to NULL.
 * @param newGraph The graph to import the data into. This can be usefull to import data into a subgraph. Defaults to NULL.
 * @return :Graph* The graph containing the imported data, or NULL in case of failure.
 **/
TLP_SCOPE Graph* importGraph(const std::string &format, DataSet &dataSet, PluginProgress *progress=NULL,Graph *newGraph=NULL);

/**
 * @ingroup Graph
 * @brief Creates a new, empty graph.
 *
 * This is a simple method factory to create a Graph implementation (remember, Graph is only an interface).
 *
 * This is the recommended way to create a new Graph.
 *
 * @return :Graph* A new, empty graph.
 **/
TLP_SCOPE Graph* newGraph();

/**
 * @ingroup Graph
 * Appends the selected part of the graph inG (properties, nodes and edges) into the graph outG.
 * If no selection is done (inSel=NULL), the whole inG graph is appended.
 * The output selection is used to select the appended nodes & edges
 * \warning The input selection is extended to all selected edge ends.
 */
TLP_SCOPE void copyToGraph(Graph *outG, const Graph *inG, BooleanProperty* inSelection=NULL, BooleanProperty* outSelection=NULL );

/**
 * @ingroup Graph
 * Removes the selected part of the graph ioG (properties values, nodes and edges).
 * If no selection is done (inSel=NULL), the whole graph is reseted to default value.
 * \warning The selection is extended to all selected edge ends.
 */
TLP_SCOPE void removeFromGraph(Graph * ioG, BooleanProperty* inSelection=NULL);


/**
 * @ingroup Graph
 * Gets an iterator over the root graphs. That is all the currently existing graphs which have been created using the tlp::newGraph, tlp::loadGraph or tlp::importGraph functions and are the root graphs of an existing graph hierarchy.
 * @return An iterator over all the root graphs. The caller of this function is responsible of the deletion of the returned iterator.
 */
TLP_SCOPE Iterator<Graph*>* getRootGraphs();


/**
 * @ingroup Graph
 * The class Graph is the interface of a Graph in the Tulip Library.
 *
 * There are a few principles to know when working with a Tulip Graph.
 *
 * @chapter Directed
 * Every edge is directed in a Tulip Graph.
 * You can choose to ignore this, but every edge has a source and destination.
 *
 * @chapter Inheritance
 *
 * Subgraphs inherit from their parent graph.
 * This is true of nodes and edges; every node and edge in a subgraph also exists in each of its parent graphs.
 * This is also true of properties; every property in a graph exist in all of its subgraphs, except if it has been replaced
 * by a local property.
 *
 * For instance, ig you have the following graph hierarchy:
 * root
 *  / \
 * A   B
 *
 * Every node in A is in root, and every node in B is in root.
 * Nodes can be in A and root but not B; or in B and root but not A.
 *
 * For instance, imagine a graph. You want to compare it to its Delaunay triangulation.
 * You need to create a subgraph that is a clone of the original (say this is A) to keep the original graph,
 * and another copy (say this one is B) on which you will perform the delaunay triangulation.
 *
 * B will have none of the original edges, and A will have only the original edges.
 *
 * As for properties; let's imagine the same graph hierarchy.
 * You want to compare two different layouts on the same graph.
 * You need to create two clone subgraphs, on each you make the 'viewLayout' property local.
 * This results in A and B having different values for the layout, but everything else in common.
 * You then can apply two different algorithms on A and B (e.g. Bubble Tree and Tree Radial).
 *
 * @chapter Meta Nodes
 * A meta node is a node representing a subgraph of the current graph.
 *
 * @chapter Undo Redo
 * The Tulip Graph object supports for undo and redo of modifications.
 *The operations affect the whole graph hierarchy, and cannot be limited to a subgraph.
 *
 */
class TLP_SCOPE Graph : public Observable {

  friend class GraphAbstract;
  friend class GraphUpdatesRecorder;
  friend class GraphDecorator;
  friend class PropertyManager;
  friend class PropertyInterface;

public:
  Graph():id(0) {}
  virtual ~Graph() {}

  /**
   * @brief Applies an algorithm plugin, identified by its name.
   * Algorithm plugins are subclasses of the tlp::Algorithm interface.
   * Parameters are transmitted to the algorithm trough the DataSet.
   * To determine a plugin's parameters, you can either:
   *
   * * refer to its documentation
   *
   * * use buildDefaultDataSet on the plugin object if you have an instance of it
   *
   * * call getPluginParameters() with the name of the plugin on the PluginLister
   *
   *
   * If an error occurs, a message describing the error should be stored in errorMessage.
   *
   * @param algorithm The algorithm to apply.
   * @param errorMessage A string that will be modified to contain an error message should an error occur.
   * @param dataSet The parameters to the algorithm. Defaults to NULL.
   * @param progress A PluginProgress to report the progress of the operation, as well as final state. Defaults to NULL.
   * @return bool Whether the algorithm was successfully applied.
   **/
  bool applyAlgorithm(const std::string &algorithm, std::string &errorMessage, DataSet *dataSet=NULL, PluginProgress *progress=NULL);

  //=========================================================================
  // Graph hierarchy access and building
  //=========================================================================

  /**
   * @brief Removes all nodes, edges and sub-graphs from this graph.
   *
   * Contrarily to creating a new Graph, this keeps attributes and properties.
   *
   * @return void
   **/
  virtual  void clear()=0;

  /**
    * @brief Creates and returns a new sub-graph of this graph.
    *
    * If a BooleanProperty is provided, all the nodes and edges for which it is true will be added to the subgraph.
    * If none is provided, then the subgraph will be empty.
    *
    * The id parameter should only be provided if you know exactly what you are doing; as Tulip will manage the subgraphs IDs when left to 0.
    * It is only used by the Graph loading as subgraphs ids are preserved when saving/loading a Graph.
    *
    * @param selection The elements to add to the new subgraph. Defaults to 0.
    * @param name The name of the newly created subgraph. Defaults to "unnamed".
    * @return :Graph* The newly created subgraph.
    **/
  virtual Graph *addSubGraph(BooleanProperty *selection=NULL,
                             const std::string& name = "unnamed")=0;

  /**
   * @brief Creates and returns a new named sub-graph of this graph.
   *
   * @param name The name of the newly created subgraph.
   * @return :Graph* The newly created subgraph.
   **/
  Graph *addSubGraph(const std::string& name);

  /**
   * @brief Creates and returns a subgraph that contains all the elements of this graph.
   *
   * @param name The name of the newly created subgraph. Defaults to "unnamed".
   * @param addSibling if true the clone subgraph will be a sibling of this graph, if false (the default) it will be a subgraph of this graph
   * @return :Graph* The newly created clone subgraph. NULL will be returned if addSibling is set to true and this graph is a root graph.
   **/
  virtual Graph* addCloneSubGraph(const std::string& name = "unnamed", bool addSibling = false);

  /**
   * @brief Creates and returns a new sub-graph of the graph induced by a set of nodes.
   * Every node contained in the given set of nodes is added to the subgraph.
   * Every edge connecting any two nodes in the set of given nodes is also added.
   * @param nodeSet The nodes to add to the subgraph. All the edges between these nodes are added too.
   * @param parentSubGraph If provided, is used as parent graph for the newly created subgraph instead of the graph this method is called on.
   * @return The newly created subgraph.
   */
  Graph *inducedSubGraph(const std::set<node>& nodeSet,
                         Graph* parentSubGraph = NULL);

  /**
   * @brief Deletes a sub-graph of this graph.
   * All subgraphs of the removed graph are re-parented to this graph.
   * For instance, with a graph hierarchy as follows :
   * root
   * / \
   * A  B
   *    /|\
   *   C D E
   *
   * @code root->delSubGraph(B);
   * would result in the following hierarchy:
   *  root
   * / | \\
   * A C D E
   *
   * @param graph The subgraph to delete.
   *
   * @see delAllSubGraphs() if you want to remove all descendants of a graph.
   */
  virtual void delSubGraph(Graph *graph)=0;

  /**
   * @brief Deletes a sub-graph of this graph and all of its sub-graphs.
   ** For instance, with a graph hierarchy as follows :
   * root
   * / \
   * A  B
   *    /|\
   *   C D E
   *
   * @codeline root->delSubGraph(B); @endcode
   * would result in the following hierarchy:
   *  root
   *   |
   *   A
   *
   * @param graph The subgraph to delete.
   * @see delSubGraph() if you want to keep the descendants of the subgraph to remove.
   */
  virtual void delAllSubGraphs(Graph *graph)=0;

  /**
   * @brief Returns the parent of the graph. If called on the root graph, it returns itself.
   * @return The parent of this graph (or itself if it is the root graph).
   * @see getRoot() to directly retrieve the root graph.
   */
  virtual Graph* getSuperGraph()const =0;

  /**
   * @brief Gets the root graph of the graph hierarchy.
   * @return The root graph of the graph hierarchy.
   */
  virtual Graph* getRoot() const =0;

  /**
   * @cond DOXYGEN_HIDDEN
   * @brief Sets the parent of a graph.
   * @warning ONLY USE IF YOU KNOW EXACTLY WHAT YOU ARE DOING.
   * @endcond
   */
  virtual void setSuperGraph(Graph *)=0;

  /**
   * @brief Gets an iterator over all the sub-graphs of the graph.
   * For instance, in the following graph hierarchy:
   ** root
   * / \
   * A  B
   *    /|\
   *   C D E
   *
   * @codeline root->getSubGraphs(); @endcode
   * Will return an iterator over A and B, but not C, D and E.
   * @return An iterator over this graph's direct subgraphs.
   */
  virtual Iterator<Graph *> * getSubGraphs() const=0;

  /**
   * @brief This method returns the nth subgraph.
   * Since subgraphs order cannot be ensured in every implementation, this method should be equivalent to:
   * @code
    int i=0;
    Iterator<Graph *> *it = g->getSubGraphs();
    while (it->hasNext()) {
      Graph *result = it->next();
      if (i++ == n) {
        delete it;
        return result;
      }
    }
    delete it;
    return NULL;
   * @endcode
   * @param n the index of the subgraph to retrieve.
   * @return The n-th subgraph.
   */
  virtual Graph *getNthSubGraph(unsigned int n) const;

  /**
   * @brief Return the number of direct sub-graphs.
   * For instance, in the following graph hierarchy:
   * root
   * / \
   * A  B
   *    /|\
   *   C D E
   *
   * @codeline root->numberOfSubGraphs(); @endcode
   * Will return 2.
   * @return The number of direct subgraphs.
   * @see numberOfDescendantGraphs() to count in the whole hierarchy.
   */
  virtual unsigned int numberOfSubGraphs() const=0;

  /**
   * @brief Return the number of descendant sub-graphs.
   * For instance, in the following graph hierarchy:
   * root
   * / \
   * A  B
   *    /|\
   *   C D E
   *
   * @codeline root->numberOfSubGraphs(); @endcode
   * Will return 5.
   * @return The number of descendants subgraphs.
   * @see numberOfSubGraphs() to count only direct subgraphs.
   */
  virtual unsigned int numberOfDescendantGraphs() const=0;

  /**
   * @brief Indicates if the graph argument is a direct sub-graph.
   * @param subGraph The graph to check is a subgraph of this graph.
   * @return Whether subGraph is a direct subgraph of this graph.
   * @see isDescendantGraph() to search in the whole hierarchy.
   */
  virtual bool isSubGraph(const Graph* subGraph) const=0;

  /**
   * @brief Indicates if the graph argument is a descendant of this graph.
   * @param subGraph The graph to check is a descendant of this graph.
   * @return Whether subGraph is a descendant of this graph.
   * @see isSubGraph to search only in direct subgraphs.
   */
  virtual bool isDescendantGraph(const Graph* subGraph) const=0;

  /**
   * @brief Returns a pointer on the sub-graph with the corresponding id
   * or NULL if there is no sub-graph with that id.
   * @param id The id of the subgraph to retrieve.
   * @return A subgraph of the given id, or null if no such subgraph exists on this graph.
   * @see getDescendantGraph(unsigned int) to search in the whole hierarchy.
   */
  virtual Graph* getSubGraph(unsigned int id) const=0;

  /**
   * @brief Returns a pointer on the sub-graph with the corresponding name
   * or NULL if there is no sub-graph with that name.
   * @param name The name of the subgraph to retrieve.
   * @return A Graph named name, or NULL if no such subgraph exists on this graph.
   * @see getDescendantGraph(const std::string &) to search in the whole hierarchy.
   */
  virtual Graph* getSubGraph(const std::string &name) const=0;

  /**
   * @brief Returns a pointer on the descendant with the corresponding id
   * or NULL if there is no descendant  with that id.
   * @param id The id of the descendant graph to retrieve.
   * @return A graph with the given id, or NULL if no such graph exists in this graph's descendants.
   * @see getSubGraph(unsigned int) to search only in direct subgraphs.
   */
  virtual Graph* getDescendantGraph(unsigned int id) const=0;

  /**
   * @brief Returns a pointer on the first descendant graph with the corresponding name
   * or NULL if there is no descendant graph with that name.
   * @param name The name of the descendant graph to look for.
   * @return A graph named name, or NULL if there is no such graph in this graph's descendants.
   * @see getSubGraph(const std::string &) to search only in direct subgraphs.
   */
  virtual Graph* getDescendantGraph(const std::string &name) const=0;

  /**
   * @brief Gets an iterator over all the descendant sub-graphs of the graph.
   * For instance, in the following graph hierarchy:
   ** root
   * / \
   * A  B
   *    /|\
   *   C D E
   *
   * @codeline root->getSubGraphs(); @endcode
   * Will return an iterator over A B, C, D and E.
   * @return An iterator over this graph's descendant subgraphs.
   */
  Iterator<Graph *> * getDescendantGraphs() const;

  //==============================================================================
  // Modification of the graph structure
  //==============================================================================
  /**
   * @brief Adds a new node in the graph and returns it. This node is also added in all
   * the ancestor graphs.
   * @return The newly added node.
   * @see addNodes() if you want to add more than one node.
   */
  virtual node addNode()=0;

  /**
   * @brief Adds new nodes in the graph and returns them in the addedNodes vector.
   * The new nodes are also added in all the ancestor graphs.
   *
   * @param nbNodes The number of nodes to add.
   * @param addedNodes The newly added nodes. This vector is cleared before being filled.
   * @see addNode() to add a single node.
   */
  virtual void addNodes(unsigned int nbNodes, std::vector<node>& addedNodes)=0;

  /**
   * @brief Adds an existing node in the graph. This node is also added in all the ancestor graphs.
   * This node must exists in the graph hierarchy (which means it must exist in the root graph).
   * You cannot add a node to the root graph this way (as it must already be an element of the root graph).
   * @warning Using this method on the root graph will display a warning on the console.
   *
   * @param n The node to add to a subgraph. This node must exist in the root graph.
   * @see addNode() to add a new node to a graph.
   */
  virtual void addNode(const node n)=0;

  /**
   * @brief Adds existing nodes in the graph. The nodes are also added in all the ancestor graphs.
   * as with addNode(const tlp::node), the nodes must exist in the graph hierarchy and thus exist in the root graph,
   * and node cannot be added this way to the root graph.

   * @warning Using this method on the root graph will display a warning on the console.
   * @warning The graph does not take ownership of the Iterator.
   *
   * @param nodes An iterator over nodes to add to this subgraph. The graph does not takes ownership of this iterator.
   */
  virtual void addNodes(Iterator<node>* nodes)=0;

  /**
  * @brief Adds existing nodes in the graph. The nodes are also added in all the ancestor graphs.
  * as with addNode(const tlp::node), the nodes must exist in the graph hierarchy and thus exist in the root graph,
  * and nodes cannot be added this way to the root graph.

  * @warning Using this method on the root graph will display a warning on the console.
  *
  * @param nodes a vector of nodes to add to this subgraph.
  */
  void addNodes(const std::vector<node>& nodes);

  /**
   * @brief Deletes a node in the graph.
   * This node is also removed in the sub-graphs hierarchy of the current graph.
   * @param n The node to delete.
   * @param deleteInAllGraphs Whether to delete in all its parent graphs or only in this graph. By default only removes in the current graph.
   * @see delNodes() to remove multiple nodes.
   */
  virtual void delNode(const node n, bool deleteInAllGraphs = false)=0;

  /**
   * @brief Deletes nodes in the graph.
   * These nodes are also removed in the sub-graphs hierarchy of the current graph.
   * @warning the graph does not take ownership of the Iterator.
   * @param it The nodes to delete.
   * @param deleteInAllGraphs Whether to delete in all its parent graphs or only in this graph. By default only removes in the current graph.
   * @see delNode() to remove a single node.
   */
  virtual void delNodes(Iterator<node>* it, bool deleteInAllGraphs = false)=0;

  /**
   * @brief Deletes nodes in the graph.
   * These nodes are also removed in the sub-graphs hierarchy of the current graph.
   * @warning the graph does not take ownership of the Iterator.
   * @param nodes a vector of the nodes to delete.
   * @param deleteInAllGraphs Whether to delete in all its parent graphs or only in this graph. By default only removes in the current graph.
   * @see delNode() to remove a single node.
   */
  void delNodes(const std::vector<node>& nodes, bool deleteInAllGraphs = false);

  /**
   * @brief Adds a new edge in the graph
   * This edge is also added in all the super-graph of the graph.
   * @param source The source of the edge.
   * @param target The target of the edge.
   * @return The newly added edge.
   * @see addEdges() to add multiple edges at once.
   */
  virtual edge addEdge(const node source, const node target)=0;

  /**
   * @brief Adds new edges in the graph and returns them the addedEdges vector.
   * The new edges are also added in all the graph ancestors.
   *
   * @warning If the edges vector contains a node that does not belong to this graph,
   * undefined behavior will ensue.
   * @param edges A vector describing between which nodes to add edges.
   * The first element of the pair is the source, the second is the destination.
   * @param addedEdges The newly added edges. This vector is cleared before being filled.
   *
   */
  virtual void addEdges(const std::vector<std::pair<node, node> >& edges,
                        std::vector<edge>& addedEdges)=0;

  /**
   * @brief Adds an existing edge in the graph. This edge is also added in all
   * the ancestor graphs.
   * The edge must be an element of the graph hierarchy, thus it must be
   * an element of the root graph.
   * @warning Using this method on the root graph will display a warning on the console.
   * @param e The edge to add to this subgraph.
   * @see addEgdes() to add more than one edge at once.
   * @see addNode() to add nodes.
   */
  virtual void addEdge(const edge e)=0;

  /**
   * @brief Adds existing edges in the graph. The edges are also added in all
   * the ancestor graphs.
   * The added edges must be elements of the graph hierarchy,
   * thus they must be elements of the root graph.
   * @warning Using this method on the root graph will display a warning on the console.
   * @warning The graph does not take ownership of the iterator.
   * @param edges The edges to add on this subgraph.
   */
  virtual void addEdges(Iterator<edge>* edges)=0;

  /**
   * @brief Adds existing edges in the graph. The edges are also added in all
   * the ancestor graphs.
   * The added edges must be elements of the graph hierarchy,
   * thus they must be elements of the root graph.
   * @warning Using this method on the root graph will display a warning on the console.
   * @param edges a vector of the edges to add on this subgraph.
   */
  void addEdges(const std::vector<edge>& edges);

  /**
   * @brief Deletes an edge in the graph. The edge is also removed in
   * the sub-graphs hierarchy.
   * The ordering of remaining edges is preserved.
   * @param e The edge to delete.
   * @param deleteInAllGraphs Whether to delete in all its parent graphs or only in this graph. By default only removes in the current graph.
   */
  virtual void delEdge(const edge e, bool deleteInAllGraphs = false)=0;

  /**
   * @brief Deletes edges in the graph. These edges are also removed in the sub-graphs hierarchy.
   * The ordering of remaining edges is preserved.
   * @warning The graph does not take ownership of the Iterator.
   * @param itE
   * @param deleteInAllGraphs  Whether to delete in all its parent graphs or only in this graph. By default only removes in the current graph.
   */
  virtual void delEdges(Iterator<edge>* itE, bool deleteInAllGraphs = false)=0;

  /**
   * @brief Deletes edges in the graph. These edges are also removed in the sub-graphs hierarchy.
   * The ordering of remaining edges is preserved.
   * @warning The graph does not take ownership of the Iterator.
   * @param edges a vector of the edges to delete
   * @param deleteInAllGraphs  Whether to delete in all its parent graphs or only in this graph. By default only removes in the current graph.
   */
  void delEdges(const std::vector<edge>& edges, bool deleteInAllGraphs = false);

  /**
   * @brief Sets the order of the edges around a node.
   * This operation ensures that adjacent edges of a node will
   * be ordered as they are in the vector of edges given in parameter.
   *
   * This can be useful if you want to make sure you retrieve the edges in a specific order when iterating upon them.
   * @param n The node whose edges to order.
   * @param edges The edges, in the order you want them.
   */
  virtual void setEdgeOrder(const node n,const std::vector<edge> &edges)=0;

  /**
   * @brief Swaps two edges in the adjacency list of a node.
   * @param n The node on whoch to swap the edges order.
   * @param e1 The first edge, that will take the second edge's position.
   * @param e2 The second edge, that will take the first edge's position.
   */
  virtual void swapEdgeOrder(const node n,const edge e1, const edge e2)=0;

  /**
   * @brief Sets the source of an edge to be the given node.
   * @param e The edge to change the source of.
   * @param source The new source of the edge.
   */
  virtual void setSource(const edge e, const node source) = 0;

  /**
   * @brief Sets the target of an edge to be the given node.
   * @param e The edge to change the target of.
   * @param target The new target of the edge.
   */
  virtual void setTarget(const edge e, const node target) = 0;

  /**
   * @brief Sets both the source and the target of an edge.
   * @param e The edge to set the source and target of.
   * @param source The new source of the edge.
   * @param target The new target of the edge.
   */
  virtual void setEnds(const edge e, const node source, const node target) = 0;

  /**
   * @brief Reverses the direction of an edge, the source becomes the target and the target
   *  becomes the source.
   * @warning The ordering is global to the entire graph hierarchy. Thus, by changing
   *  the ordering of a graph you change the ordering of the hierarchy.
   * @param e The edge top reverse.
   */
  virtual void reverse(const edge e)=0;
  // Attempts to reserve enough space to store nodes.
  // Only defined on root graph.
  virtual void reserveNodes(unsigned int nbNodes) = 0;
  // Attempts to reserve enough space to store edges.
  // Only defined on root graph.
  virtual void reserveEdges(unsigned int nbEdges) = 0;
  //================================================================================
  //Iterators on the graph structure.
  //================================================================================
  /**
   * @brief Finds the first node whose input degree equals 0.
   *
   * @return tlp::node The first encountered node with input degree of 0, or an invalid node if none was found.
   **/
  virtual tlp::node getSource() const;

  /**
   * @brief Gets a node on the graph.
   * @return Any node of the graph.
   */
  virtual node getOneNode() const =0;

  /**
   * @brief Gets an iterator over this graph's nodes.
   * @return An iterator over all the nodes of this graph.
   * @see getInNodes()
   * @see getOutNodes()
   * @see getInOutNodes()
   * @see getEdges()
   */
  virtual Iterator<node>* getNodes() const =0;

  /**
   * @brief Gets the i-th node in the input nodes of a given node.
   * An input node 'in' of a node 'N' is the source of an edge going from
   * 'in' to 'N'. e.g.
   * @code
   * node in = graph->addNode();
   * node N = graph->addNode();
   * graph->addEdge(in, N);
   * //in == graph->getInNode(N, 1);
   * @endcode
   *
   * If you have 5 input nodes on a node N, then
   * @codeline graph->getInNode(2); @endcode
   * will return the second of those nodes.
   * It will ignore the output nodes of this node.
   * @param n The node to get an input node of.
   * @param i The index of the input node to get.
   * @return The i-th input node of the given node.
   * @see getInNodes()
   * @see getInEdges()
   */
  virtual node getInNode(const node n,unsigned int i)const =0;

  /**
   * @brief Gets an iterator over the input nodes of a node.
   * @param n The node to get the input nodes of.
   * @return An iterator over the input nodes of a node.
   * @see getInNode()
   * @see getInOutNodes()
   * @see getInEdges()
   */
  virtual Iterator<node>* getInNodes(const node n) const =0;

  /**
   * @brief Gets the i-th node in the output nodes of a given node.
   * An output node 'out' of a node 'N' is the target of an edge going from
   * 'N' to 'out'. e.g.
   * @code
   * node N = graph->addNode();
   * node out = graph->addNode();
   * graph->addEdge(N, out);
   * //out == graph->getOutNode(N, 1);
   * @endcode
   *
   * If you have 5 output nodes on a node N, then
   * @codeline graph->getOutNode(2); @endcode
   * will return the second of those nodes.
   * It will ignore the input nodes of this node.
   * @param n The node to get an output node of.
   * @param i The index of the output node to get.
   * @return The i-th output node of the given node.
   * @see getOutNodes()
   * @see getOutEdges()
   */
  virtual node getOutNode(const node n,unsigned int i) const =0;

  /**
   * @brief Gets an iterator over the output nodes of a node.
   * @param n The node to get the output nodes of.
   * @return An iterator over the output nodes of a node.
   * @see getOutNode()
   * @see getInOutNodes()
   * @see getOutEdges()
   */
  virtual Iterator<node>* getOutNodes(const node n) const =0;

  /**
   * @brief Gets an iterator over the neighbors of a given node.
   * @param n The node to retrieve the neighbors of.
   * @return An iterator over the node's neighbors.
   */
  virtual Iterator<node>* getInOutNodes(const node n) const =0;

  /**
   * @brief Gets an iterator performing a breadth-first search on the graph.
   * @param root The node from whom to start the BFS. If not provided, the root
   * node will be assigned to a source node in the graph (node with input degree equals to 0).
   * If there is no source node in the graph, a random node will be picked.
   * @return A stable iterator over the graph nodes in the BFS order.
   */
  virtual Iterator<node>* bfs(const node root = node()) const = 0;

  /**
   * @brief Gets an iterator performing a depth-first search on the graph.
   * @param root The node from whom to start the DFS. If not provided, the root
   * node will be assigned to a source node in the graph (node with input degree equals to 0).
   * If there is no source node in the graph, a random node will be picked.
   * @return A stable iterator over the graph nodes in the DFS order.
   */
  virtual Iterator<node>* dfs(const node root = node()) const = 0;

  /**
   * @brief Gets the underlying graph of a meta node.
   * @param metaNode The metanode.
   * @return The Graph pointed to by the metanode.
   * @see getEdgeMetaInfo()
   */
  virtual Graph* getNodeMetaInfo(const node metaNode) const = 0;

  /**
   * @brief Get an iterator over all the graph's edges.
   * @return An iterator over all the graph's edges.
   * @see getInEdges()
   * @see getOutEdges()
   * @see getInOutEdges()
   */
  virtual Iterator<edge>* getEdges() const =0;

  /**
   * @brief Gets an edge of the graph.
   * @return Any one of the graph's edges.
   */
  virtual edge getOneEdge() const =0;

  /**
   * @brief Gets an iterator over the output edges of a node.
   * @param n The node to get the output edges from.
   * @return An iterator over the node's output edges.
   * @see getEdges()
   * @see getOutEdges()
   * @see getInOutEdges()
   */
  virtual Iterator<edge>* getOutEdges(const node n) const =0;

  /**
   * @brief Gets an iterator over the edges of a node.
   * @param n The node to get the edges from.
   * @return An iterator over the node's edges.
   * @see getEdges()
   * @see getOutEdges()
   * @see getInEdges()
   */
  virtual Iterator<edge>* getInOutEdges(const node n) const =0;

  /**
   * @brief Gets an iterator over the input edges of a node.
   * @param n The node to get the input edges from.
   * @return An iterator over the node's input edges.
   * @see getEdges()
   * @see getOutEdges()
   * @see getInOutEdges()
   */
  virtual Iterator<edge>* getInEdges(const node n) const =0;

  /**
   * @brief Gets an iterator over the edges composing a meta edge.
   * @param metaEdge The metaEdge to get the real edges of.
   * @return An Iterator over the edges composing the metaEdge.
   * @see getNodeMetaInfo()
   */
  virtual Iterator<edge>* getEdgeMetaInfo(const edge metaEdge) const =0;

  //================================================================================
  // Graph, nodes and edges information about the graph stucture
  //================================================================================
  /**
   * @brief Gets the unique identifier of the graph.
   * @return The unique identifier of this graph.
   */
  unsigned int getId() const {
    return id;
  }

  /**
   * @brief Gets the number of nodes in this graph.
   * @return The number of nodes in this graph.
   * @see numberOfEdges()
   */
  virtual unsigned int numberOfNodes()const =0;

  /**
   * @brief Gets the number of edges in this graph.
   * @return The number of edges in this graph.
   * @see numberOfNodes()
   */
  virtual unsigned int numberOfEdges()const =0;

  /**
   * @param n The node to get the degree of.
   * @return The degree of the given node.
   */
  virtual unsigned int deg(const node n)const =0;

  /**
   * @brief Get the input degree of a node.
   * @param n The node to get the input degree of.
   * @return The input degree of the given node.
   */
  virtual unsigned int indeg(const node n)const =0;

  /**
   * @brief Get the output degree of a node.
   * @param n The node to get the output degree of.
   * @return The output degree of the given node.
   */
  virtual unsigned int outdeg(const node n)const =0;

  /**
   * @brief Gets the source of an edge.
   * @param e The edge to get the source of.
   * @return The source of the given edge.
   */
  virtual node source(const edge e)const =0;

  /**
   * @brief Gets the target of an edge.
   * @param e The edge to get the target of.
   * @return The target of the given edge.
   */
  virtual node target(const edge e)const =0;

  /**
   * @brief Gets the source and the target of an edge.
   * @param e The edge to get the ends of.
   * @return A pair whose first element is the source, and second is the target.
   */
  virtual const std::pair<node, node>& ends(const edge e)const=0;

  /**
   * @brief Gets the opposite node  of n through e.
   * @param e The edge linking the two nodes.
   * @param n The node at one end of e.
   * @return The node at the other end of e.
   */
  virtual node opposite(const edge e, const node n)const =0;

  /**
   * @brief Checks if an element belongs to this graph.
   * @param n The node to check if it is an element of the graph.
   * @return Whether or not the element belongs to the graph.
   */
  virtual bool isElement(const node n) const =0;

  /**
   * @brief Checks if a node is a meta node.
   * @param n The node to check if it is a meta node.
   * @return Whether or not the node is a meta node.
   */
  virtual bool isMetaNode(const node n) const =0;

  /**
   * @brief Checks if an element belongs to this graph.
   * @param e The edge to check if it is an element of the graph.
   * @return Whether or not the element belongs to the graph.
   */
  virtual bool isElement(const edge e) const =0;

  /**
   * @brief Checks if an edge is a meta edge.
   * @param e The edge to check if it is a meta edge.
   * @return Whether or not the edge is a meta edge.
   */
  virtual bool isMetaEdge(const edge e) const =0;

  /**
   * @brief Checks if an edge exists between two given nodes.
   * @param source The source of the hypothetical edge.
   * @param target The target of the hypothetical edge.
   * @param directed When set to false edges from target to source are also considered
   * @return true if such an edge exists
   */
  virtual bool hasEdge(const node source, const node target,
                       bool directed = true) const {
    return existEdge(source, target, directed).isValid();
  }


  /**
   * @brief Returns all the edges between two nodes.
   * @param source The source of the hypothetical edges.
   * @param target The target of the hypothetical edges.
   * @param directed When set to false edges from target to source are also considered
   * @return a vector of existing edges
   */
  virtual std::vector<edge> getEdges(const node source, const node target,
                                     bool directed = true) const=0;

  /**
   * @brief Returns the first edge found between the two given nodes.
   * @warning This function always returns an edge,
   * you need to check if this edge is valid with edge::isValid().
   * @param source The source of the hypothetical edge.
   * @param target The target of the hypothetical edge.
   * @param directed When set to false
   * an edge from target to source may also be returned
   * @return An edge that is only valid if it exists.
   */
  virtual edge existEdge(const node source, const node target,
                         bool directed = true) const=0;

  //================================================================================
  // Access to the graph attributes and to the node/edge property.
  //================================================================================
  /**
   * @brief Sets the name of the graph.
   * The name does not have to be unique, it is used for convenience.
   * @param name The new name of the graph.
   */
  virtual void setName(const std::string &name) = 0;

  /**
   * @brief Retrieves the name of the graph.
   * @return The name of the graph.
   */
  virtual std::string getName() const = 0;

  /**
   * @brief Gets the attributes of the graph.
   *
   * The attributes contains the name and any user-defined value.
   * @return The attributes of the graph.
   */
  const DataSet & getAttributes() const {
    return (const_cast<Graph *>(this))->getNonConstAttributes();
  }

  /**
   * @brief Gets an attribute on the graph.
   * @param name The name of the attribute to set.
   * @param value The value to set.
   * @return Whether the setting of the attribute was sucessful.
   */
  template<typename ATTRIBUTETYPE>
  bool getAttribute(const std::string &name, ATTRIBUTETYPE& value) const;

  /**
   * @brief Gets a copy of the attribute.
   * @param name The name of the attribute to retrieve.
   * @return A copy of the attribute to retrieve.
   */
  DataType* getAttribute(const std::string& name) const;

  /**
   * @brief Sets an attribute on the graph.
   * @param name The name of the attribute to set.
   * @param value The value to set on this attribute.
   */
  template<typename ATTRIBUTETYPE>
  void setAttribute(const std::string &name,const ATTRIBUTETYPE &value);

  /**
   * @brief Sets an attribute on the graph.
   * @param name The name of the attribute to set.
   * @param value The value to set.
   */
  void setAttribute(const std::string &name, const DataType* value);

  /**
   * @brief Removes an attribute on the graph.
   * @param name The name of the attribute to remove.
   */
  void removeAttribute(const std::string &name) {
    notifyRemoveAttribute(name);
    getNonConstAttributes().remove(name);
  }

  /**
   * @brief Checks if an attribute exists.
   * @param name The name of the attribute to check for.
   * @return Whether the attribute exists.
   */
  bool attributeExist(const std::string &name) {
    return getAttributes().exist(name);
  }

  /**
   * @brief Adds a property to the graph.
   * The graph takes ownership of the property. If you want to delete it, use
   * Graph::delLocalProperty().
   * @param name The unique identifier of the property.
   * @param prop The property to add.
   */
  virtual  void addLocalProperty(const std::string &name, PropertyInterface *prop)=0;

  /**
   * @brief Gets an existing property.
   * In DEBUG mode an assertion checks the existence of the property.
   *
   * The graph keeps ownership of the property, if you wish to remove it from the graph use
   * Graph::delLocalProperty().
   *
   * @param name The unique identifier of the property.
   * @return An existing property, or NULL if no property with the given name exists.
   */
  virtual PropertyInterface* getProperty(const std::string& name) const = 0;

  /**
   * @brief Gets a property on this graph.
   * The name of a property indentifies it uniquely.
   * Either there already exists a property with the given name, in which case it is returned.
   * Either no such porperty exists and it is created.
   *
   * The graph keeps ownership of the property, if you wish to remove it fgrom the graph use
   * Graph::delLocalProperty().
   * @warning using the wrong template parameter will cause a segmentation fault.
   * @param The unique identifier of the property.
   * @return The property of given name.
   */
  template<typename PropertyType>
  PropertyType* getLocalProperty(const std::string &name);

  /**
   * @brief Gets a property on this graph or one of its ancestors.
   * If the property already exists on the graph or in one of its ancestors, it is returned.
   * Otherwise a new property is created on this graph.
   *
   * The graph keeps ownership of the property, if you wish to remove it from the graph use
   * Graph::delLocalProperty().
   *
   * @warning using the wrong propertyType will result in a segmentation fault. Using an invalid property type will always return NULL.
   * @param name The unique identifier of the property.
   * @return An existing property, or a new one if none exists with the given name.
   */
  template<typename PropertyType>
  PropertyType* getProperty(const std::string &name);

  /**
   * @brief Gets a property on this graph, and this graph only.
   * This forwards the call to the template version of getLocalProperty(), with the correct template parameter deduced from the propertyType parameter.
   *
   * The graph keeps ownership of the property, if you wish to remove it from the graph use
   * Graph::delLocalProperty().
   *
   * @warning using the wrong propertyType will result in a segmentation fault. Using an invalid property type will always return NULL.
   * @param propertyName The unique identifier of the property.
   * @param propertyType A string describing the type of the property.
   * @return The property of given name.
   * @see getLocalProperty().
   */
  PropertyInterface *getLocalProperty(const std::string& propertyName, const std::string& propertyType);

  /**
   * @brief Gets a property on this graph or one of its ancestors.
   * This forwards the call to the template version of getProperty(), with the correct template parameter deduced from the propertyType parameter.
   *
   * The graph keeps ownership of the property, if you wish to remove it from the graph use
   * Graph::delLocalProperty().
   *
   * @warning using the wrong propertyType will result in a segmentation fault. Using an invalid property type will always return NULL.
   * @param propertyName The unique identifier of the property.
   * @param propertyType A string describing the type of the property.
   * @return The property of given name.
   * @see getProperty().
   */
  PropertyInterface *getProperty(const std::string& propertyName, const std::string& propertyType);

  /**
   * @brief Checks if a property exists in this graph or one of its ancestors.
   * @param The unique identifier of the property.
   * @return Whether a property with the given name exists.
   */
  virtual  bool existProperty(const std::string& name) const = 0;

  /**
   * @brief Checks if a property exists in this graph.
   * @param The unique identifier of the property.
   * @return Whether a property with the given name exists.
   */
  virtual  bool existLocalProperty(const std::string& name) const = 0;

  /**
   * @brief Removes and deletes a property from this graph.
   * The property is removed from the graph's property pool, meaning its name can now be used by another property.
   * The object is deleted and the memory freed.
   * @param name The unique identifier of the property.
   */
  virtual  void delLocalProperty(const std::string& name)=0;

  /**
   * @brief Gets an iterator over the names of the local properties of this graph.
   * @return An iterator over this graph's properties names.
   */
  virtual Iterator<std::string>* getLocalProperties() const=0;

  /**
   * @brief Gets an iterator over the local properties of this graph.
   * @return An iterator over this graph's properties.
   */
  virtual Iterator<PropertyInterface*>* getLocalObjectProperties() const=0;

  /**
   * @brief Gets an iterator over the names of the properties inherited from this graph's ancestors,
   * excluding this graph's local properties.
   * @return An iterator over the names of the properties this graph inherited.
   */
  virtual Iterator<std::string>* getInheritedProperties() const=0;

  /**
   * @brief Gets an iterator over the properties inherited from this graph's ancestors,
   * excluding this graph's local properties.
   * @return An iterator over the properties this graph inherited.
   */
  virtual Iterator<PropertyInterface*>* getInheritedObjectProperties() const=0;

  /**
   * @brief Gets an iterator over the names of all the properties attached to this graph,
   * whether they are local or inherited.
   * @return An iterator over the names of all the properties attached to this graph.
   */
  virtual Iterator<std::string>* getProperties() const=0;

  /**
   * @brief Gets an iterator over the properties attached to this graph,
   * whether they are local or inherited.
   * @return An iterator over all of the properties attached to this graph.
   */
  virtual Iterator<PropertyInterface*>* getObjectProperties() const=0;

  /**
   * @brief Runs a plugin on the graph, whose result is a property.
   *
   * @param algorithm The name of the plugin to run.
   * @param result The property in which to put the results. All previous values will be erased.
   * @param errorMessage Stores the error message if the plugin fails.
   * @param progress A means of feedback during the plugin execution. Some plugins support being stopped or cancelled through this.
   * @param parameters The parameters of the algorithm. Some algorithms use this DataSet to output some additional information.
   * @return Whether the plugin executed successfully or not. If not, check the error message.
   *
   * @see PluginLister::getPluginParameters() to retrieve the list of default parameters for the pligin.
   */
  bool applyPropertyAlgorithm(const std::string &algorithm,
                              PropertyInterface* result,
                              std::string &errorMessage,
                              PluginProgress *progress=NULL,
                              DataSet *parameters=NULL);

  // updates management
  /**
   * @brief Saves the current state of the whole graph hierarchy and allows to revert to this state later on, using pop().
   * All modifications except those altering the ordering of the edges will be undone.
   *
   * This allows to undo/redo modifications on a graph.
   * This is mostly useful from a user interface point of view, but some algorithms use this mechanism to clean up before finishing.
   * For instance:
   * @code
   * Graph* graph = tlp::newGraph();
   * DoubleProperty* prop = graph->getProperty<DoubleProperty>("metric");
   * string errorMessage;
   *
   * //our super metric stuff we want to kee
   * DoubleProperty* superProperty = graph->getProperty<DoubleProperty>("superStuff");
   * vector<PropertyInterface*> propertiesToKeep;
   * propertiesToKeep.push_back(superProperty);
   *
   *
   * //apply some metric
   * graph->applyPropertyAlgorithm("Degree", prop, errorMessage);
   *
   * // save this state to be able to revert to it later
   * //however we do not want to allow to unpop(), which would go forward again to the state where prop contains 'Depth'.
   * //this saves some memory.
   * //Also we always want to keep the value of our super property, so we pass it in the collection of properties to leave unaffected by the pop().
   * graph->push(false, propertiesToKeep);
   *
   * //compute the quality of this metric, or whatever makes sense
   * int degreeQuality = prop->getMax();
   *
   * //compute another metric
   * graph->applyPropertyAlgorithm("Depth", prop, errorMessage);
   *
   * //compute our secret metric, that depends on depth
   * graph->applyPropertyAlgorithm("MySuperSecretAlgorithm", superProperty, errorMessage);
   *
   * //compute its quality
   * int depthQuality = prop->getMax();
   *
   * //if the degree was better, revert back to the state where its contents were in prop.
   * if(degreeQuality > depthQuality) {
   *    //this does not affect superProperty, as we told the system not to consider it when recording modifications to potentially revert.
   *    graph->pop();
   * }
   *
   * //do some stuff using our high quality metric
   * ColorProperty* color = graph->getProperty("viewColor");
   * graph->applyPropertyAlgorithm("Color Mapping", color, errorMessage);
   *
   * @endcode
   *
   * @param unpopAllowed Whether or not to allow to re-do the modifications once they are undone.
   * @param propertiesToPreserveOnPop A collection of properties whose state to preserve when using pop().
   * @see pop()
   * @see unpop()
   * @see canPop()
   * @see canUnPop()
   * @see canPopThenUnPop()
   */
  virtual void push(bool unpopAllowed = true,
                    std::vector<PropertyInterface*>* propertiesToPreserveOnPop= NULL)=0;

  /**
   * @brief Undoes modifications and reverts the whole graph hierarchy back to a previous state.
   *
   * @param unpopAllowed Whether or not it is possible to redo what will be undoe by this call.
   */
  virtual void pop(bool unpopAllowed = true)=0;

  /**
   * @brief Re-perform actions that were undone using pop().
   *
   * For instance:
   * @code
   * DoubleProperty* prop = graph->getProperty<DoubleProperty>("metric");
   * string errorMessage;
   *
   * //apply some metric
   * graph->applyPropertyAlgorithm("Degree", prop, errorMessage);
   *
   * // save this state to be able to revert to it later
   * graph->push();
   *
   * //compute the quality of this metric, or whatever makes sense
   * int degreeQuality = prop->getMax();
   *
   * //compute another metric
   * graph->applyPropertyAlgorithm("Depth", prop, errorMessage);
   *
   * //compute its quality
   * int depthQuality = prop->getMax();
   *
   * //if the degree was better, revert back to the state where its contents were in prop.
   * if(degreeQuality > depthQuality) {
   *    graph->pop();
   * }
   *
   * ...
   *
   * //revert back to the depth for some reason.
   * graph->unpop();
   * @endcode
   */
  virtual void unpop()=0;

  /**
   * @brief Checks if there is a state to revert to.
   * @return Whether there was a previous call to push() that was not yet pop()'ed.
   */
  virtual bool canPop()=0;

  /**
   * @brief Checks if the last undone modifications can be redone.
   * @return Whether it is possible to re-do modifications that have been undone by pop().
   */
  virtual bool canUnpop()=0;

  /**
   * @brief Checks if it is possible to call pop() and then unPop(), to undo then re-do modifications.
   * @return Whether it is possible to undo and then redo.
   */
  virtual bool canPopThenUnpop()=0;

  // meta nodes management
  /**
   * @brief Creates a meta-node from a set of nodes.
   * Every edges from any node in the set to another node of the graph will be replaced with meta edges
   * from the meta node to the other nodes.
   * @warning This method will fail when called on the root graph.
   *
   * @param nodeSet The nodes to put into the meta node.
   * @param multiEdges Whether a meta edge should be created for each underlying edge.
   * @param delAllEdge Whether the underlying edges will be removed from the whole hierarchy.
   * @return The newly created meta node.
   */
  node createMetaNode(const std::set<node> &nodeSet, bool multiEdges = true, bool delAllEdge = true);

  /**
   *  @brief Populates a quotient graph with one meta node
   * for each iterated graph.
   *
   * @param itS a Graph iterator, (typically a subgraph iterator)
   * @param quotientGraph the graph that will contain the meta nodes
   * @param metaNodes will contains all the added meta nodes after the call
   *
   */
  void createMetaNodes(Iterator<Graph *> *itS, Graph *quotientGraph,
                       std::vector<node>& metaNodes);
  /**
   * @brief Closes an existing subgraph into a metanode.  Edges from nodes
   * in the subgraph to nodes outside the subgraph are replaced with
   * edges from the metanode to the nodes outside the subgraph.
   * @warning this method will fail when called on the root graph.
   *
   * @param subGraph an existing subgraph
   * @param multiEdges indicates if a meta edge will be created for each underlying edge
   * @param delAllEdge indicates if the underlying edges will be removed from the entire hierarchy
   */
  node createMetaNode(Graph* subGraph, bool multiEdges = true, bool delAllEdge = true);

  /**
   * @brief Opens a metanode and replaces all edges between that
   * meta node and other nodes in the graph.
   *
   * @warning this method will fail when called on the root graph.
   *
   * @param n The meta node to open.
   * @param updateProperties If set to true, open meta node will update inner nodes layout, color, size, etc
   */
  void openMetaNode(node n, bool updateProperties=true);

protected:
  virtual DataSet &getNonConstAttributes() = 0;
  // designed to reassign an id to a previously deleted elt
  // used by GraphUpdatesRecorder
  virtual node restoreNode(node)=0;
  virtual void restoreNodes(const std::vector<node>& nodes)=0;
  virtual edge restoreEdge(edge, node source, node target)=0;
  virtual void restoreEdges(const std::vector<edge>& edges,
                            const std::vector<std::pair<node, node> >& ends)=0;
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node)=0;
  virtual void removeEdge(const edge)=0;

  // to check if a property can be deleted
  // used by PropertyManager
  virtual bool canDeleteProperty(Graph* g, PropertyInterface *prop) {
    return getRoot()->canDeleteProperty(g, prop);
  }

  // local property renaming
  // can failed if a property with the same name already exists
  virtual bool renameLocalProperty(PropertyInterface* prop,
                                   const std::string& newName)=0;

  // internally used to deal with sub graph deletion
  virtual void removeSubGraph(Graph*)=0;
  virtual void clearSubGraphs()=0;
  virtual void restoreSubGraph(Graph*)=0;
  virtual void setSubGraphToKeep(Graph*)=0;

  // for notification of GraphObserver
  void notifyAddNode(const node n);
  void notifyAddNode(Graph*, const node n) {
    notifyAddNode(n);
  }
  void notifyAddEdge(const edge e);
  void notifyAddEdge(Graph*, const edge e) {
    notifyAddEdge(e);
  }
  void notifyBeforeSetEnds(const edge e);
  void notifyBeforeSetEnds(Graph*, const edge e) {
    notifyBeforeSetEnds(e);
  }
  void notifyAfterSetEnds(const edge e);
  void notifyAfterSetEnds(Graph*, const edge e) {
    notifyAfterSetEnds(e);
  }
  void notifyDelNode(const node n);
  void notifyDelNode(Graph*, const node n) {
    notifyDelNode(n);
  }
  void notifyDelEdge(const edge e);
  void notifyDelEdge(Graph*, const edge e) {
    notifyDelEdge(e);
  }
  void notifyReverseEdge(const edge e);
  void notifyReverseEdge(Graph*, const edge e) {
    notifyReverseEdge(e);
  }
  void notifyBeforeAddSubGraph(const Graph*);
  void notifyAfterAddSubGraph(const Graph*);
  void notifyBeforeAddSubGraph(Graph*, const Graph* sg) {
    notifyBeforeAddSubGraph(sg);
  }
  void notifyAfterAddSubGraph(Graph*, const Graph* sg) {
    notifyAfterAddSubGraph(sg);
  }
  void notifyBeforeDelSubGraph(const Graph*);
  void notifyAfterDelSubGraph(const Graph*);
  void notifyBeforeDelSubGraph(Graph*, const Graph* sg) {
    notifyBeforeDelSubGraph(sg);
  }
  void notifyAfterDelSubGraph(Graph*, const Graph* sg) {
    notifyAfterDelSubGraph(sg);
  }

  void notifyBeforeAddDescendantGraph(const Graph*);
  void notifyAfterAddDescendantGraph(const Graph*);
  void notifyBeforeDelDescendantGraph(const Graph*);
  void notifyAfterDelDescendantGraph(const Graph*);

  void notifyBeforeAddLocalProperty(const std::string&);
  void notifyAddLocalProperty(const std::string&);
  void notifyAddLocalProperty(Graph*, const std::string& name) {
    notifyAddLocalProperty(name);
  }
  void notifyBeforeDelLocalProperty(const std::string&);
  void notifyAfterDelLocalProperty(const std::string&);
  void notifyDelLocalProperty(Graph*, const std::string& name) {
    notifyBeforeDelLocalProperty(name);
  }
  void notifyBeforeSetAttribute(const std::string&);
  void notifyBeforeSetAttribute(Graph*, const std::string& name) {
    notifyBeforeSetAttribute(name);
  }
  void notifyAfterSetAttribute(const std::string&);
  void notifyAfterSetAttribute(Graph*, const std::string& name) {
    notifyAfterSetAttribute(name);
  }
  void notifyRemoveAttribute(const std::string&);
  void notifyRemoveAttribute(Graph*, const std::string& name) {
    notifyRemoveAttribute(name);
  }
  void notifyDestroy();
  void notifyDestroy(Graph*) {
    notifyDestroy();
  }

  unsigned int id;
  TLP_HASH_MAP<std::string, tlp::PropertyInterface*> circularCalls;
};

/**
 * @ingroup Observation
 * Event class for specific events on Graph
 **/
class TLP_SCOPE GraphEvent :public Event {
public:
  // be careful about the ordering of the constants
  // in the enum below because it is used in some assertions
  enum GraphEventType {
    TLP_ADD_NODE = 0,
    TLP_DEL_NODE = 1,
    TLP_ADD_EDGE = 2,
    TLP_DEL_EDGE = 3,
    TLP_REVERSE_EDGE = 4,
    TLP_BEFORE_SET_ENDS = 5,
    TLP_AFTER_SET_ENDS = 6,
    TLP_ADD_NODES = 7,
    TLP_ADD_EDGES = 8,
    TLP_BEFORE_ADD_DESCENDANTGRAPH = 9,
    TLP_AFTER_ADD_DESCENDANTGRAPH = 10,
    TLP_BEFORE_DEL_DESCENDANTGRAPH = 11,
    TLP_AFTER_DEL_DESCENDANTGRAPH = 12,
    TLP_BEFORE_ADD_SUBGRAPH = 13,
    TLP_AFTER_ADD_SUBGRAPH = 14,
    TLP_BEFORE_DEL_SUBGRAPH = 15,
    TLP_AFTER_DEL_SUBGRAPH = 16,
    TLP_ADD_LOCAL_PROPERTY = 17,
    TLP_BEFORE_DEL_LOCAL_PROPERTY = 18,
    TLP_AFTER_DEL_LOCAL_PROPERTY = 19,
    TLP_ADD_INHERITED_PROPERTY = 20,
    TLP_BEFORE_DEL_INHERITED_PROPERTY = 21,
    TLP_AFTER_DEL_INHERITED_PROPERTY = 22,
    TLP_BEFORE_RENAME_LOCAL_PROPERTY = 23,
    TLP_AFTER_RENAME_LOCAL_PROPERTY = 24,
    TLP_BEFORE_SET_ATTRIBUTE = 25,
    TLP_AFTER_SET_ATTRIBUTE = 26,
    TLP_REMOVE_ATTRIBUTE = 27,
    TLP_BEFORE_ADD_LOCAL_PROPERTY = 28,
    TLP_BEFORE_ADD_INHERITED_PROPERTY = 29
  };

  // constructor for node/edge events
  GraphEvent(const Graph& g, GraphEventType graphEvtType, unsigned int id,
             Event::EventType evtType = Event::TLP_MODIFICATION)
    : Event(g, evtType), evtType(graphEvtType) {
    info.eltId = id;
  }
  // constructor for nodes events
  GraphEvent(const Graph& g, GraphEventType graphEvtType,
             const std::vector<node>& nodes,
             Event::EventType evtType = Event::TLP_MODIFICATION)
    : Event(g, evtType), evtType(graphEvtType) {
    info.nodes = &nodes;
  }
  // constructor for edges events
  GraphEvent(const Graph& g, GraphEventType graphEvtType,
             const std::vector<edge>& edges,
             Event::EventType evtType = Event::TLP_MODIFICATION)
    : Event(g, evtType), evtType(graphEvtType) {
    info.edges = &edges;
  }
  // constructor for subgraph events
  GraphEvent(const Graph& g, GraphEventType graphEvtType,
             const Graph* sg)
    : Event(g, Event::TLP_MODIFICATION), evtType(graphEvtType) {
    info.subGraph = sg;
  }

  // constructor for attribute/property events
  GraphEvent(const Graph& g, GraphEventType graphEvtType,
             const std::string& str,
             Event::EventType evtType = Event::TLP_MODIFICATION)
    : Event(g, evtType), evtType(graphEvtType) {
    info.name = new std::string(str);
  }

  // constructor for rename property events
  GraphEvent(const Graph& g, GraphEventType graphEvtType,
             PropertyInterface* prop,
             const std::string& newName)
    : Event(g, Event::TLP_MODIFICATION), evtType(graphEvtType) {
    info.renamedProp =
      new std::pair<PropertyInterface*,std::string>(prop, newName);
  }

  // destructor needed to cleanup name if any
  ~GraphEvent() {
    if (evtType > TLP_AFTER_DEL_SUBGRAPH) {
      if (evtType == TLP_BEFORE_RENAME_LOCAL_PROPERTY ||
          evtType == TLP_AFTER_RENAME_LOCAL_PROPERTY)
        delete info.renamedProp;
      else
        delete info.name;
    }
  }

  Graph* getGraph() const {
    return static_cast<Graph *>(sender());
  }

  node getNode() const {
    assert(evtType < TLP_ADD_EDGE);
    return node(info.eltId);
  }

  edge getEdge() const {
    assert(evtType > TLP_DEL_NODE && evtType < TLP_ADD_NODES);
    return edge(info.eltId);
  }

  const std::vector<node>& getNodes() const {
    assert(evtType == TLP_ADD_NODES);
    return *(info.nodes);
  }

  const std::vector<edge>& getEdges() const {
    assert(evtType == TLP_ADD_EDGES);
    return *(info.edges);
  }

  const Graph* getSubGraph() const {
    assert(evtType > TLP_ADD_EDGES && evtType < TLP_ADD_LOCAL_PROPERTY);
    return info.subGraph;
  }

  const std::string& getAttributeName() const {
    assert(evtType > TLP_AFTER_DEL_INHERITED_PROPERTY);
    return *(info.name);
  }

  const std::string& getPropertyName() const;

  PropertyInterface* getProperty() const {
    assert(evtType == TLP_BEFORE_RENAME_LOCAL_PROPERTY ||
           evtType == TLP_AFTER_RENAME_LOCAL_PROPERTY);
    return info.renamedProp->first;
  }

  const std::string& getPropertyNewName() const {
    assert(evtType == TLP_BEFORE_RENAME_LOCAL_PROPERTY);
    return info.renamedProp->second;
  }

  const std::string& getPropertyOldName() const {
    assert(evtType == TLP_AFTER_RENAME_LOCAL_PROPERTY);
    return info.renamedProp->second;
  }

  GraphEventType getType() const {
    return evtType;
  }

protected:
  GraphEventType evtType;
  union {
    unsigned int eltId;
    const Graph* subGraph;
    std::string* name;
    const std::vector<node>* nodes;
    const std::vector<edge>* edges;
    std::pair<PropertyInterface*, std::string>* renamedProp;
  } info;
};

}

///Print the graph (only nodes and edges) in ostream, in the tulip format
TLP_SCOPE std::ostream& operator<< (std::ostream &,const tlp::Graph *);

//================================================================================
// these functions allow to use tlp::Graph as a key in a hash-based data structure (e.g. hashmap).
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
#include "cxx/Graph.cxx"
#endif
