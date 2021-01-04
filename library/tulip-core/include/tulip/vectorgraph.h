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

#ifndef VECTORGRAPH_H
#define VECTORGRAPH_H
#include <vector>
#include <algorithm>

#include <set>
#include <cassert>

#include <tulip/tulipconf.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/IdManager.h>
#include <tulip/vectorgraphproperty.h>

namespace tlp {

template <class itType>
struct Iterator;
//===========================================
/**
 * @class VectorGraph
 *
 * @brief That class provide a simple implementation of graph structure (without subgraphs,
 * observer, metagraph)
 * it enables to obtain very efficient access/modification time.
 *
 * User can both use tulip iterators or direct vector to access to the graph structure for better
 * performance.
 * To have maximum speedup, that Graph implementation use only vectors, almost all operations
 * are done in constant time (even modification), however since the class use vectors, modification
 * of adjacency can change the ordering of edges around nodes. If you use it only for standard
 * graph operations there is no problem. However if you want to manipulate maps, be aware that
 * a modification can change the graph embedding. EdgeOrdering function can be used to reorder
 * correctly elements when necessary.
 *
 * @warning the class is not thread safe
 * @warning modifications of the graph structure invalidate iterations.
 *
 * @warning Use that class only if you need performance.
 * @todo split the file in .h .cpp
 */
class TLP_SCOPE VectorGraph {

public:
  //=======================================================
  VectorGraph();
  //=======================================================
  ~VectorGraph();
  //=======================================================
  /**
   * @brief delete all nodes/edges and Properties of the graph
   * @remark o(N + E + NbProp)
   */
  void clear();
  //=======================================================
  /**
   * @brief Test if an edge exist between two nodes, in directed mode the orientation
   * is taken into account.
   * @return the found edge, else an invalid edge.
   * @remark o(min(deg(src), deg(tgt))
   * @todo test
   */
  edge existEdge(const node src, const node tgt, const bool directed = true) const;
  //=======================================================
  /**
   * @brief Return true if n belongs to the graph
   * @remark o(1)
   */
  bool isElement(const node n) const {
    assert(n.id < _nData.size());
    return _nodes.isElement(n);
  }
  //=======================================================
  /**
   * @brief Return true if e belongs to the graph
   * @remark o(1)
   */
  bool isElement(const edge e) const {
    assert(e.id < _eData.size());
    return _edges.isElement(e);
  }
  //=======================================================
  /**
   * \brief Set the ordering of edges around n according to their order in v.
   * \warning there is no test here, all edge in v must be element of star(e)
   * @remark o(v.size())
   */
  void setEdgeOrder(const node n, const std::vector<edge> &v);
  //=======================================================
  /**
   * \brief swap to edge in the ordered adjacency vector
   * \warning the two edges must be element of star(v)
   * @remark o(1)
   */
  void swapEdgeOrder(const node n, const edge e1, const edge e2);
  //=======================================================
  /**
   * @brief Enables to reserve memory for nbNodes
   * Reserving memory before to addNode enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   * @remark o(N + nbNodes)
   */
  void reserveNodes(const size_t nbNodes);
  //=======================================================
  /**
   * @brief Enables to reserve memory for nbEdges
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   * @remark o(N + nbNodes)
   */
  void reserveEdges(const size_t nbEdges);
  //=======================================================
  /**
   * @brief Enables to reserve memory for adjacency nodes
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   * @remark o(E + nbEdges)
   */
  void reserveAdj(const size_t nbEdges);
  //=======================================================
  /**
   * @brief Enables to reserve memory for adjacency nodes
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   * @remark o(E + nbEdges)
   */
  void reserveAdj(const node n, const size_t nbEdges);
  //=======================================================
  /**
   * @brief Return the node at position id in the array of nodes
   * @remark o(1)
   */
  node operator[](const unsigned int id) const {
    assert(id < _nodes.size());
    return _nodes[id];
  }
  //=======================================================
  /**
   * @brief Return the edge at position id in the array of nodes
   * @remark o(1)
   */
  edge operator()(const unsigned int id) const {
    assert(id < _edges.size());
    return _edges[id];
  }
  //=======================================================
  /**
   * @brief Return the first node of graph (ie first node in the array of nodes)
   * @remark o(1)
   */
  node getOneNode() const {
    assert(!isEmpty());
    return _nodes[0];
  }
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on nodes of the graph
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @remark: o(1)
   */
  Iterator<node> *getNodes() const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on edges of the graph
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @remark: o(1)
   */
  Iterator<edge> *getEdges() const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on adjacent edges of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @remark: o(1)
   */
  Iterator<edge> *getInOutEdges(const node n) const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on out edges of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @remark: o(1)
   */
  Iterator<edge> *getOutEdges(const node n) const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on in edges of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @remark: o(1)
   */
  Iterator<edge> *getInEdges(const node n) const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on adjacent nodes of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @remark: o(1)
   */
  Iterator<node> *getInOutNodes(const node n) const;
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on in nodes of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @remark o(1)
   */
  //=======================================================
  Iterator<node> *getInNodes(const node n) const;
  /**
   * @brief Return a Tulip Iterator on out nodes of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @remark o(1)
   */
  //=======================================================
  Iterator<node> *getOutNodes(const node n) const;
  //=======================================================
  /**
   * @brief Return the degree of a node
   * @remark o(1)
   */
  unsigned int deg(const node n) const {
    assert(isElement(n));
    return _nData[n]._adjn.size();
  }
  //=======================================================
  /**
   * @brief Return the out degree of a node
   * @remark o(1)
   */
  unsigned int outdeg(const node n) const {
    assert(isElement(n));
    return _nData[n]._outdeg;
  }
  //=======================================================
  /**
   * @brief Return the in degree of a node
   * @remark o(1)
   */
  unsigned int indeg(const node n) const {
    return deg(n) - _nData[n]._outdeg;
  }
  //=======================================================
  /**
   * @brief Return the number of edges in the graph
   * @remark: o(1)
   */
  unsigned int numberOfEdges() const {
    return _edges.size();
  }
  //=======================================================
  /**
   * @brief Return the number of nodes in the graph
   * @remark: o(1)
   */
  unsigned int numberOfNodes() const {
    return _nodes.size();
  }
  //=======================================================
  /**
   * @brief Return whether the graph has nodes or not
   * @remark: o(1)
   */
  bool isEmpty() const {
    return _nodes.empty();
  }
  //=======================================================
  /**
   * @brief Add a new node in the graph and return it
   * @warning: That operation modifies the array of nodes
   * and thus invalidates all iterators on it.
   * @remark: o(1)
   */
  node addNode();
  //=======================================================
  /**
   * @brief Add nb new nodes in the structure and returns them
   * in the addedNodes vector
   * @warning: That operation modifies the array of nodes
   * and thus invalidates all iterators on it. The addedNodes vector
   * is cleared before adding nodes
   * @complexity: o(1)
   */
  void addNodes(unsigned int nb, std::vector<node> *addedNodes = nullptr);
  //=======================================================
  /**
   * @brief Delete a node and all its adjacent edges in the graph
   * @warning That operation modifies the array of nodes and the array of edges
   * and thus invalidates all iterators on it.
   * @warning That operation modifies the array of neighboors of extrmities of edges, thus
   * it invalidates iterators on adjacency for the nodes at the extremities od the deleted
   * edges.
   * @warning Orders of edges in the extremities of the deleted edges are affected
   * @remark: o(1)
   */
  void delNode(const node n);
  //=======================================================
  /**
   * @brief Add a new edge between src and tgt and return it
   * @warning That operation modifies the array of neighboors of extrmities of edges, thus
   * it invalidates iterators on adjacency for the nodes at the extremities od the deleted
   * edges.
   * @remark o(1)
   */
  edge addEdge(const node src, const node tgt);
  //=======================================================
  /**
   * @brief Add edges in the structure and returns them
   * in the addedEdges vector
   * @warning: That operation modifies the array of edges and
   * the adjacency edges of its ends thus any iterators existing for
   * these structures will be invalidated.
   */
  void addEdges(const std::vector<std::pair<node, node>> &edges,
                std::vector<edge> *addedEdges = nullptr);
  //=======================================================
  /**
   * @brief Delete an edge in the graph
   * @warning: That operation modifies the array of edges
   * and thus invalidates all iterators on it.
   * @warning That operation modifies the array of neighboors of extremities of the edge e, thus
   * it invalidates iterators on adjacency for the nodes at the extremities od the deleted
   * edge.
   * @warning Orders of edges in the extremities of the deleted edge are affected
   * @remark o(1)
   */
  void delEdge(const edge e);
  //=======================================================
  /**
   * @brief Delete all adjacent edges (in/out) of a node
   * @warning: That operation modifies the array of edges
   * and thus invalidates all iterators on it.
   * @warning That operation modifies the array of neighboors of extremities of the edge e, thus
   * it invalidates iterators on adjacency for the nodes at the extremities od the deleted
   * edge.
   * @warning Orders of edges in the extremities of the deleted edge are affected
   * @remark o(deg(V))
   * @todo test
   */
  void delEdges(const node n);
  //=======================================================
  /**
   * @brief Delete all edges in the graph
   * @warning: That operation modifies the array of edges and all arrays of nodes
   * and thus invalidates all iterators, only graph nodes iterators are not affected.
   * @remark o(E + V)
   */
  void delAllEdges();
  //=======================================================
  /**
   * @brief Delete all nodes in the graph
   * @warning: That operation modifies the array of edges and all arrays of nodes
   * and thus invalidates all iterators.
   * @remark o(E + V)
   */
  void delAllNodes();
  //=======================================================
  /**
   * @brief return the first extremity (considered as source if the graph is directed) of an edge
   * @remark o(1)
   */
  node source(const edge e) const {
    assert(isElement(e));
    return _eData[e]._ends.first;
  }
  //=======================================================
  /**
   * @brief return the second extremity (considered as target if the graph is directed) of an
   * edge
   * @remark o(1)
   */
  node target(const edge e) const {
    assert(isElement(e));
    return _eData[e]._ends.second;
  }
  //=======================================================
  /**
   * @brief return the opposite node of n through edge e
   * @remark o(1)
   */
  node opposite(const edge e, const node n) const;
  //=======================================================
  /**
   * @brief Reverse an edge e, source become target and target become soure
   * @remark o(1)
   */
  void reverse(const edge e);
  //=======================================================
  /**
   * @brief change the source of an edge
   * @warning That operation modifies the array of neighboors of extrmities of edges, thus
   * it invalidates iterators on adjacency for the nodes at the extremities of the modified
   * edges and nodes.
   * @remark o(1)
   * \see setEnds
   */
  void setSource(const edge e, const node n) {
    assert(isElement(e));
    assert(isElement(n));
    setEnds(e, n, target(e));
  }
  //=======================================================
  /**
   * @brief change the target of an edge
   * @warning That operation modifies the array of neighboors of extrmities of edges, thus
   * it invalidates iterators on adjacency for the nodes at the extremities of the modified
   * edges and nodes.
   * @remark o(1)
   * \see setEnds
   */
  void setTarget(const edge e, const node n) {
    assert(isElement(e));
    assert(isElement(n));
    setEnds(e, source(e), n);
  }
  //=======================================================
  /**
   * @brief Return the extremities of an edge (src, target)
   * @remark o(1)
   */
  const std::pair<node, node> &ends(const edge e) const {
    assert(isElement(e));
    return _eData[e]._ends;
  }
  //=======================================================
  /**
   * @brief Reconnect the edge e to have the new given extremities
   * @warning That operation modifies the array of neighboors of extrmities of edges, thus
   * it invalidates iterators on adjacency for the nodes at the extremities of the modified
   * edges and nodes.
   * @remark o(1)
   */
  void setEnds(const edge e, const node src, const node tgt);
  //=======================================================
  /**
   * @brief Shuffle the array of graph nodes
   * @remark dependent of stl::shuffle algorithm (should be o(N))
   */
  void shuffleNodes();
  //=======================================================
  /**
   * @brief Shuffle the array of graph edges
   * @remark dependent of stl::shuffle algorithm (should be o(E))
   */
  void shuffleEdges();
  //=======================================================
  /**
   * @brief Sort all edges according to comparison functor given in parameter
   * if stable is true a stable sort algorithm is applied
   * Comparison should be an instance of a class wihch implements operator():
   * @remark dependent of stl::sort and stl::stable_sort algorithm (should be o(E log (E)))
   * @code
   *  class Compare {
   *  //return true if a < b
   *  bool operator()(const edge a, const edge b);
   *  };
   * @endcode
   * \warning that function is not compatible with the Tulip Graph API
   */
  template <typename Compare>
  void sortEdges(Compare cmp, bool stable = false) {
    if (stable)
      stable_sort(_edges.begin(), _edges.end(), cmp);
    else
      sort(_edges.begin(), _edges.end(), cmp);

    // recompute indices of edges
    _edges.reIndex();
  }
  //=======================================================
  /**
   * @brief Sort all nodes according to comparison functor given in parameter
   * if stable is true a stable sort algorithm is applied
   * Comparison should be an instance of a class wihch implements operator():
   * @code
   *  class Compare {
   *  //return true if a < b
   *  bool operator()(const node a, const node b);
   *  };
   * @endcode
   * @remark dependent of stl::sort and stl::stable_sort algorithm (should be o(N log (N)))
   * \warning that function is not compatible with the Tulip Graph API
   */
  template <typename Compare>
  void sortNodes(Compare cmp, bool stable = false) {
    if (stable)
      stable_sort(_nodes.begin(), _nodes.end(), cmp);
    else
      sort(_nodes.begin(), _nodes.end(), cmp);

    // recompute indices of nodes
    _nodes.reIndex();
  }
  //=======================================================
  /**
   * @brief return the position of an edge in the array of edges.
   * \warning that function is not compatible with the Tulip Graph API
   * @remark  o(1)
   */
  unsigned int edgePos(const edge e) const {
    assert(isElement(e));
    return _edges.getPos(e);
  }
  //=======================================================
  /**
   * @brief return the position of a node in the array of nodes.
   * \warning that function is not compatible with the Tulip Graph API
   * @remark  o(1)
   */
  unsigned int nodePos(const node n) const {
    assert(isElement(n));
    return _nodes.getPos(n);
  }
  //=======================================================
  /**
   * @brief Swap two nodes in the array of graph nodes
   * @remark  o(1)
   * \warning that function is not compatible with the Tulip Graph API
   */
  void swap(const node a, const node b);
  //=======================================================
  /**
   * @brief Swap two edges in the array of graph edge
   * @remark  o(1)
   * \warning that function is not compatible with the Tulip Graph API
   */
  void swap(const edge a, const edge b);
  //=======================================================
  /**
   * @brief Create a new node array of type TYPE
   * NodesAttr can be copied in constant time it is just a pointer
   * NodesAttr is not a smart pointer it must be deleted with freeNodesAttribute
   * @remark  o(log(number of arrays) + new of a vector<TYPE> of size N)
   * \warning that function is not compatible with the Tulip Graph API
   */
  template <typename TYPE>
  void alloc(NodeProperty<TYPE> &prop) {
    auto values = new typename VectorGraphProperty<TYPE>::ValuesImpl(
        _nodes.size() + _nodes.numberOfFree(), _nodes.capacity());
    _nodeValues.insert(values);
    prop = NodeProperty<TYPE>(values, this);
  }
  //=======================================================
  /**
   * @brief Delete an Array from the set of node values
   * @warning all copy of the VectorGraphProperty::ValuesImpl are no more valid (serious bug if they
   * are used after)
   * @remark  o(log(number of arrays) + free of a vector<TYPE> of size N)
   * \warning that function is not compatible with the Tulip Graph API
   */
  template <typename TYPE>
  void free(NodeProperty<TYPE> &prop) {
    assert(_nodeValues.find(prop._values) != _nodeValues.end());
    delete prop._values;
    _nodeValues.erase(prop._values);
  }
  //=======================================================
  /**
   * @brief Create a new edge array of type TYPE
   * EdgesAttr can be copied in constant time it is just a pointer
   * EdgesAttr is not a smart pointer it must be deleted with freeEdgesAttribute
   * @remark  o(log(number of node arrays) + new of a vector<TYPE> of size E)
   * \warning that function is not compatible with the Tulip Graph API
   */
  template <typename TYPE>
  void alloc(EdgeProperty<TYPE> &prop) {
    auto values = new typename VectorGraphProperty<TYPE>::ValuesImpl(
        _edges.size() + _edges.numberOfFree(), _edges.capacity());
    _edgeValues.insert(values);
    prop = EdgeProperty<TYPE>(values, this);
  }
  //=======================================================
  /**
   * @brief Delete an Array from the set of edge values
   * @warning all copy of the VectorGraphProperty::ValuesImpl are no more valid (serious bug if they
   * are used after)
   * @remark  o(log(number of edge values) + free of a vector<TYPE> of size E)
   * \warning that function is not compatible with the Tulip Graph API
   */
  template <typename TYPE>
  void free(EdgeProperty<TYPE> &prop) {
    assert(_edgeValues.find(prop._values) != _edgeValues.end());
    delete prop._values;
    _edgeValues.erase(prop._values);
  }
  //=======================================================
  /**
   * @brief Return a const reference on the vector of adjacent nodes of n
   *
   * It is the fastest way to access to node adjacency, Iterators are 25% slower.
   * \warning code that use that function won't be compatible with Tulip Graph API
   *
   * @remark o(1)
   * \see getInOutNodes
   * \see getInNodes
   * \see getOutNodes
   */
  const std::vector<node> &adj(const node n) const {
    assert(isElement(n));
    return _nData[n]._adjn;
  }
  //=======================================================
  /**
   * @brief Return a const reference on the vector of adjacent edges of n
   *
   * It is the fastest way to access to edge adjacency, Iterators are 25% slower.
   * \warning code that use that function won't be compatible with Tulip Graph API
   *
   * @remark o(1)
   * \see getInOutEdges
   * \see getInEdges
   * \see getOutEdges
   */
  const std::vector<edge> &star(const node n) const {
    assert(isElement(n));
    return _nData[n]._adje;
  }
  //=======================================================
  /**
   * @brief Return a const reference on the vector of nodes of the graph
   * It is the fastest way to access to edge adjacency, Iterators are 25% slower.
   * \warning code that use that function won't be compatible with Tulip Graph API
   * @remark o(1)
   */
  const std::vector<node> &nodes() const {
    return _nodes;
  }
  //=======================================================
  /**
   * @brief Return a const reference on the vector of edges of the graph
   * It is the fastest way to access to edge adjacency, Iterators are 25% slower.
   * \warning code that use that function won't be compatible with Tulip Graph API
   * @remark o(1)
   */
  const std::vector<edge> &edges() const {
    return _edges;
  }
//=======================================================
#ifndef NDEBUG
  /**
   * these two functions are used internally to insure that property has been allocated in debug
   * mode
   * @warning never used these function directly even in debug mode !!!
   */
  bool isNodeAttr(VectorGraphValues *values) {
    return (_nodeValues.find(values) != _nodeValues.end());
  }

  bool isEdgeAttr(VectorGraphValues *values) {
    return (_edgeValues.find(values) != _edgeValues.end());
  }
#endif
  //=============================================================
  /**
   * output the graph in a very simple way for debugging
   */
  void dump() const;
  //=============================================================
  /**
   * internal function to test the integrity of the graph
   */
  void integrityTest();

private:
  struct _iNodes {
    _iNodes() : _outdeg(0) {}

    void clear() {
      _outdeg = 0;
      _adjt.clear();
      _adjn.clear();
      _adje.clear();
    }

    void addEdge(bool t, node n, edge e) {
      _adjt.push_back(t);
      _adjn.push_back(n);
      _adje.push_back(e);
    }

    unsigned int _outdeg;    /** out degree of nodes */
    std::vector<bool> _adjt; /** orientation of the edge, used to separate in and out edges/nodes */
    std::vector<node> _adjn; /** inout nodes*/
    std::vector<edge> _adje; /** inout edges*/
  };

  struct _iEdges {
    std::pair<node, node> _ends;                    /** source and target of an edge */
    std::pair<unsigned int, unsigned int> _endsPos; /** edge pos in the ends adjacencies */
  };

  std::vector<_iNodes> _nData; /** internal storage of nodes */
  std::vector<_iEdges> _eData; /** internal storage of edges */

  IdContainer<node> _nodes; /** vector of nodes element of the graph */
  IdContainer<edge> _edges; /** vector of edges element of the graph */

  std::set<VectorGraphValues *>
      _nodeValues; /** set of all node properties allocated on that graph */
  std::set<VectorGraphValues *>
      _edgeValues; /** set of all edge properties allocated on that graph */

  //=======================================================
  /**
   * internal function to break the program and output debug information during
   * integrity test.
   * @TODO hide that function in the .cpp file
   */
  void testCond(std::string str, bool b);
  //=======================================================
  /**
   * internal function to adjust size of node properties when graph is modified
   */
  void addNodeToValues(node n);
  //=======================================================
  /**
   * internal function to adjust size of edge properties when graph is modified
   */
  void addEdgeToValues(edge e);
  //=======================================================
  /**
   * internal function to configure a new edge and its ends
   */
  void addEdgeInternal(edge e, node src, node tgt);
  //=======================================================
  /**
   * internal function to remove an edge
   */
  void removeEdge(edge e);
  //=======================================================
  /**
   * Internal function to remove the edge e in the adjacency list of n
   */
  void moveEdge(node n, unsigned int a, unsigned int b);
  /**
   * Internal function tp remove the edge e in the adjacency list of n
   */
  void partialDelEdge(node n, edge e);
  //=======================================================
};

#ifndef NDEBUG // these two functions are used to insure that property has been allocated in debug
               // mode
template <typename TYPE>
bool NodeProperty<TYPE>::isValid() const {
  if (this->_graph == 0)
    return false;

  if (this->_values == 0)
    return false;

  return this->_graph->isNodeAttr(this->_values);
}

template <typename TYPE>
bool EdgeProperty<TYPE>::isValid() const {
  if (this->_graph == 0)
    return false;

  if (this->_values == 0)
    return false;

  return this->_graph->isEdgeAttr(this->_values);
}
#endif
} // namespace tlp
#endif // VECTORGRAPH_H
///@endcond
