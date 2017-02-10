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


#ifndef GRAPHSTORAGE_H
#define GRAPHSTORAGE_H
#include <cassert>
#include <vector>

#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/IdManager.h>
#include <tulip/MutableContainer.h>

namespace tlp {

class Graph;

//===========================================
/**
  * @class GraphStorageIdsMemento
  * @brief that class provides a simple interface
  * to save the state of the ids manage by the GraphStorage class
  */
class GraphStorageIdsMemento {
public:
  virtual ~GraphStorageIdsMemento() {}
};
//===========================================
/**
  * @class GraphStorage
  * @brief That class provide a simple implementation
  * for the storage of graph  elts (nodes edges)
  */
class GraphStorage {
public:
  //=======================================================
  void clear();
  //=======================================================
  GraphStorage();
  //=======================================================
  /**
   * @brief Return true if n belongs to the graph
   */
  inline bool isElement(const node n) const {
    return (n.id < nodeExist.size()) && nodeExist[n.id];
  }
  //=======================================================
  /**
   * @brief Return true if e belongs to the graph
   */
  inline bool isElement(const edge e) const {
    return (e.id < edgeExist.size()) && edgeExist[e.id];
  }
  //=======================================================
  /**
   * @brief Enables to reserve memory for nbNodes
   * Reserving memory before to addNode enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   */
  void reserveNodes(const size_t nb);
  //=======================================================
  /**
   * @brief Enables to reserve memory for nbEdges
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
    */
  void reserveEdges(const size_t nb);
  //=======================================================
  /**
   * @brief Enables to reserve memory for adjacency nodes
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   */
  void reserveAdj(const node n, const size_t nb);
  //=======================================================
  /**
   * @brief Enables to reserve memory for adjacency nodes
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   */
  void reserveAdj(const size_t nb);
  //=======================================================
  /**
   * @brief restore adjacency edges of a given node
   */
  void restoreAdj(const node n, const std::vector<edge>& edges);
  //=======================================================
  /**
   * @brief return the adjacency edges of a given node
   */
  inline const std::vector<edge>& adj(const node n) const {
    assert(isElement(n));
    return nodes[n.id].edges;
  }
  //=======================================================
  /**
   * @brief Return the first node of graph
   */
  node getOneNode() const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on nodes of the graph
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity: o(1)
   */
  inline Iterator<node>* getNodes() const {
    return nodeIds.getIds<node>();
  }
  //=======================================================
  /**
   * @brief Return the current state of the ids management
   * must be deleted by the caller
   * this can be used for push/pop
   */
  const GraphStorageIdsMemento* getIdsMemento();
  //=======================================================
  /**
   * @brief  restore a state of the ids management
   */
  void restoreIdsMemento(const GraphStorageIdsMemento*);
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on edges of the graph
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   */
  inline Iterator<edge>* getEdges() const {
    return edgeIds.getIds<edge>();
  }
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on adjacent edges of the node n
   * @warning: be careful that loops appear twice
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   */
  Iterator<edge>* getInOutEdges(const node n) const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on out edges of the node n
   * @warning: The returned iterator must be deleted by the caller to prevent memory leaks
   */
  Iterator<edge>* getOutEdges(const node n) const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on in edges of the node n
   * @warning: The returned iterator must be deleted by the caller to prevent memory leaks
   */
  Iterator<edge>* getInEdges(const node n) const;
  //=======================================================
  /**
   * @brief Returns if edges exist between two nodes
   * @param src The source of the hypothetical edges.
   * @param tgt The target of the hypothetical edges.
   * @param directed When set to false edges from target to source are also considered
   * @param edges The vector of edges to fill up with the edges found
   * @param the subgraph owning the edges
   * @param onlyFirst If true only the first edge found will be returned
   * @return true if an edge has been bound
   */
  bool getEdges(const node src, const node tgt, bool directed,
                std::vector<edge>& edges, const Graph* sg = NULL,
                bool onlyFirst = false) const;

  //=======================================================
  /**
   * @brief Return a Tulip Iterator on adjacent nodes of the node n
   * @warning: The returned iterator must be deleted by the caller to prevent memory leaks
   */
  Iterator<node>* getInOutNodes(const node n) const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on in nodes of the node n
   * @warning: The returned iterator must be deleted by the caller to prevent memory leaks
   */
  Iterator<node>* getInNodes(const node n) const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on out nodes of the node n
   * @warning: The returned iterator must be deleted by the caller to prevent memory leaks
   */
  Iterator<node>* getOutNodes(const node n) const;
  //=======================================================
  /**
   * @brief Return the degree of a node
   */
  inline unsigned int deg(const node n) const {
    assert(isElement(n));
    return nodes[n.id].edges.size();
  }
  //=======================================================
  /**
   * @brief Return the out degree of a node
   */
  inline unsigned int outdeg(const node n) const {
    assert(isElement(n));
    return nodes[n.id].outDegree;
  }
  //=======================================================
  /**
   * @brief Return the in degree of a node
   */
  inline unsigned int indeg(const node n) const {
    assert(isElement(n));
    const EdgeContainer& ctnr = nodes[n.id];
    return ctnr.edges.size() - ctnr.outDegree;
  }
  //=======================================================
  /**
   * @brief Return the number of edges in the graph
   */
  inline unsigned int numberOfEdges() const {
    return nbEdges;
  }
  //=======================================================
  /**
   * @brief Return the number of nodes in the graph
   */
  inline unsigned int numberOfNodes() const {
    return nbNodes;
  }
  //=======================================================
  /**
   * @brief Return the extremities of an edge (src, target)
   */
  inline const std::pair<node, node>& ends(const edge e) const {
    assert(isElement(e));
    return edges[e.id];
  }
  //=======================================================
  /**
   * @brief return the first extremity (considered as source if the graph is directed) of an edge
   */
  inline node source(const edge e) const {
    assert(isElement(e));
    return edges[e.id].first;
  }
  //=======================================================
  /**
   * @brief return the second extremity (considered as target if the graph is directed) of an edge
   */
  inline node target(const edge e) const {
    assert(isElement(e));
    return edges[e.id].second;
  }
  //=======================================================
  /**
   * @brief return the opposite node of n through edge e
   */
  inline node opposite(const edge e, const node n) const {
    assert(isElement(e));
    const std::pair<node, node>& eEnds = edges[e.id];
    assert((eEnds.first == n) || (eEnds.second == n));
    return (eEnds.first == n) ? eEnds.second : eEnds.first;
  }

  //=======================================================
  /**
   * @brief Reconnect the edge e to have the new given ends
   * @warning That operation modify the array of neighboors of extrmities of edges, thus
   * it devalidates iterators on adjacency for the nodes at the extremities of the modified edges and nodes.
   */
  void setEnds(const edge e, const node newSrc, const node newTgt);
  //=======================================================
  /**
   * @brief change the source of an edge
   * @warning That operation modify the array of neighboors of extrmities of edges, thus
   * it devalidates iterators on adjacency for the nodes at the extremities of the modified edges and nodes.
   * \see setEnds
   */
  inline void setSource(const edge e, const node n) {
    setEnds(e, n, node());
  }
  //=======================================================
  /**
   * @brief change the target of an edge
   * @warning That operation modify the array of neighboors of extrmities of edges, thus
   * it devalidates iterators on adjacency for the nodes at the extremities of the modified edges and nodes.
   * \see setEnds
   */
  inline void setTarget(const edge e, const node n) {
    setEnds(e, node(), n);
  }
  //=======================================================
  /**
   * @brief Reverse an edge e, source become target and target become soure
   */
  void reverse(const edge e);
  //=======================================================
  /**
   * \brief Set the ordering of edges around n according to their order in v.
   */
  void setEdgeOrder(const node n, const std::vector<edge> &v );
  //=======================================================
  /**
   * \brief swap to edge in the ordered adjacency vector
   * \warning the two edges must be element of star(v)
   */
  void swapEdgeOrder(const node n, const edge e1, const edge e2);
  //=======================================================
  /**
   * @brief Add the given node in the structure and return it
   * @warning: That operation modify the array of nodes
   * and thus devalidate all iterators on it.
   * @complexity: o(1)
   */
  node restoreNode(const node n);
  //=======================================================
  /**
   * @brief Add a new node in the structure and return it
   * @warning: That operation modify the array of nodes
   * and thus devalidate all iterators on it.
   * @complexity: o(1)
   */
  inline node addNode() {
    return restoreNode(node(nodeIds.get()));
  }
  //=======================================================
  /**
   * @brief Add nb new nodes in the structure and returns them
   * in the addedNodes vector
   * @warning: That operation modify the array of nodes
   * and thus devalidate all iterators on it. The addedNodes vector
   * is cleared before adding nodes
   * @complexity: o(1)
   */
  void addNodes(unsigned int nb, std::vector<node>* addedNodes = NULL);
  //=======================================================
  /**
   * @brief remove a node from the nodes structure only
   * @warning That operation modify the array of nodes
   * and thus devalidate all iterators on it.
   * @complexity: o(1)
   */
  void removeFromNodes(const node n);
  //=======================================================
  /**
   * @brief Delete a node and all its adjacent edges in the graph
   * @warning That operation modify the array of nodes and the array of edges
   * and thus devalidate all iterators on it.
   * @warning That operation modify the array of neighboors of extrmities of edges, thus
   * it devalidates iterators on adjacency for the nodes at the extremities od the deleted edges.
   * @warning Orders of edges in the extremities of the deleted edges are affected
   * @complexity: o(1)
   */
  void delNode(const node n);
  //=======================================================
  /**
   * @brief restore the given edge between src and tgt and return it
   * the last argument indicates if the edge has to be added
   * in the adjacency edges of its two ends
   * @warning That operation modify the array of edges and
   * the adjacency edges of its ends thus any iterators existing for
   * these structures will be devalidated.
   */
  void restoreEdge(const node src, const node tgt, const edge e);
  //=======================================================
  /**
   * @brief Add a new edge between src and tgt and return it
   * @warning That operation modify the array of edges and
   * the adjacency edges of its ends thus any iterators existing for
   * these structures will be devalidated.
   */
  edge addEdge(const node src, const node tgt);
  //=======================================================
  /**
   * @brief Add edges in the structure and returns them
   * in the addedEdges vector
   * @warning: That operation modify the array of edges and
   * the adjacency edges of its ends thus any iterators existing for
   * these structures will be devalidated.
   */
  void addEdges(const std::vector<std::pair<node, node> >& edges,
                std::vector<edge>* addedEdges = NULL);
//=======================================================
  /**
   * @brief Delete an edge in the graph
   * @warning: That operation modify the array of edges
   * and thus devalidate all iterators on it.
   * @warning That operation modify the array of neighboors of extremities of the edge e, thus
   * it devalidates iterators on adjacency for the nodes at the extremities od the deleted edge.
   * @warning Orders of edges in the extremities of the deleted edge are affected
   */
  void delEdge(const edge e);
  //=======================================================
  /**
   * @brief Delete all edges in the graph
   * @warning: That operation modify the array of edges and all arrays of nodes
   * and thus devalidate all iterators, only graph nodes iterators are not affected.
   */
  void delAllEdges();
  //=======================================================
  /**
   * @brief Delete all nodes in the graph
   * @warning: That operation modify the array of edges and all arrays of nodes
   * and thus devalidate all iterators.
   */
  void delAllNodes();
  //=======================================================
private :
  // specific types
  typedef std::vector<edge> EdgeVector;

  struct EdgeContainer {
    EdgeVector edges;
    unsigned int outDegree;

    EdgeContainer(): outDegree(0) {}
  };
  typedef std::vector<EdgeContainer> Nodes;
  typedef std::vector<std::pair< node , node > > Edges;

  // data members
  mutable Edges edges;
  mutable Nodes nodes;
  std::vector<bool> edgeExist;
  std::vector<bool> nodeExist;
  IdManager nodeIds;
  IdManager edgeIds;
  unsigned int nbNodes;
  unsigned int nbEdges;

  // member functions below do not belong to the public API
  // they are just needed by the current implementation
  //=======================================================
  /**
   * @brief remove an edge from an EdgeContainer
   * @warning That operation modify the EdgeContainer
   * and thus devalidate all iterators on it.
   */
  static void removeFromEdgeContainer(EdgeContainer &c, const edge e);
  //=======================================================
  /**
   * @brief remove an edge from the edges structure
   * and from the EdgeContainer of its ends
   * except for the end node in argument if it is valid
   * @warning That operation modify the array of edges
   * and thus devalidate all iterators on it.
   */
  void removeFromEdges(const edge e, node end = node());
};
}
#endif // GRAPHSTORAGE_H
///@endcond
