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

#ifndef GRAPHSTORAGE_H
#define GRAPHSTORAGE_H
#include <cassert>
#include <vector>

#include <tulip/Node.h>
#include <tulip/Edge.h>
#include "tulip/IdManager.h"
#include "tulip/SimpleVector.h"
#include "tulip/MutableContainer.h"

using namespace std;
using namespace tlp;

namespace tlp {

//===========================================
/**
  * @class GraphStorage
  * @brief That class provide a simple implementation
  * for the storage of graph  elts (nodes edges)
  */
class GraphStorage {

#ifndef DOXYGEN_NOTFOR_DEVEL
 private :
  // specific types
  typedef SimpleVector<edge> EdgeContainer;
  typedef std::vector<EdgeContainer> Nodes;
  typedef std::vector<std::pair< node , node > > Edges;
  
  // data members
  MutableContainer<unsigned int> outDegree;
  mutable Edges edges;
  mutable Nodes nodes;
  IdManager nodeIds;
  IdManager edgeIds;
  unsigned int nbNodes;
  unsigned int nbEdges;

  // specific iterator classes
  struct EdgeContainerIterator :public Iterator<edge> {
    EdgeContainer::iterator it, itEnd;
    EdgeContainerIterator(EdgeContainer& v) :it(v.begin()), itEnd(v.end()) {
    }
    ~EdgeContainerIterator() {
    }
    bool hasNext() {
      return (it != itEnd);
    }
    edge next() {
      assert(hasNext());
      edge tmp=(*it);
      ++it;
      return tmp;
    }
  };

  // define some values for further template specializations
  enum IO_TYPE {IN = 0, OUT = 1, INOUT = 2};

  // define a template class to iterate on in or out edges of a given node
  template<IO_TYPE io_type>
  struct IOEdgeContainerIterator :public Iterator<edge> {
    node n;
    edge curEdge;
    std::set<edge> loop;
    const Edges& edges;
    EdgeContainer::iterator it, itEnd;

    void prepareNext() {
      for (;it != itEnd; ++it) {
	curEdge = *it;
	node curNode =
	  io_type ? edges[curEdge.id].first : edges[curEdge.id].second;
	if (curNode != n)
	  continue;
	curNode = io_type ? edges[curEdge.id].second : edges[curEdge.id].first;
	if (curNode == n) {
	  if (loop.find(curEdge)==loop.end()) {
	    loop.insert(curEdge);
	    ++it;
	    return;
	  }
	} else {
	  ++it;
	  return;
	}
      }
      // set curEdge as invalid
      curEdge = edge();
    }

    IOEdgeContainerIterator(node n, EdgeContainer& v, Edges& edges)
    :n(n),edges(edges),it(v.begin()),itEnd(v.end()) {
      prepareNext();
    }

    ~IOEdgeContainerIterator() {}

    edge next() {
      // check hasNext()
      assert(curEdge.isValid());
      // we are already pointing to the next
      edge tmp=curEdge;
      // anticipating the next iteration
      prepareNext();
      return tmp;
    }

    bool hasNext() {
      return (curEdge.isValid());
    }
  };

  // define a class to iterate on in or out nodes of a given node
  template<IO_TYPE io_type>
  struct IONodesIterator :public Iterator<node> {
    node n;
    const Edges& edges;
    Iterator<edge>* it;

    IONodesIterator(node n, EdgeContainer& nEdges, Edges& edges):n(n), edges(edges) {
      if (io_type == INOUT)
	it = new EdgeContainerIterator(nEdges);
      else
	it = new IOEdgeContainerIterator<io_type>(n, nEdges, edges);
    }

    ~IONodesIterator() {
      delete it;
    }

    bool hasNext() {
      return (it->hasNext());
    }

    node next() {
      // check hasNext()
      assert(it->hasNext());
      const pair<node, node>& ends = edges[it->next()];
      if (io_type == OUT)
	return ends.second;
      if (io_type == IN)
	return ends.first;
      return (ends.first == n) ? ends.second : ends.first;
    }

  };

 private:
  // member functions below do not belong to the public API
  // they are just needed by the current implementation
  //=======================================================
  /**
   * @brief remove an edge from an EdgeContainer
   * @warning That operation modify the EdgeContainer
   * and thus devalidate all iterators on it.
   * @complexity: o(1)
   */
  static void removeFromEdgeContainer(EdgeContainer &c, const edge e) {
    bool copy = false;
    EdgeContainer::iterator previous = c.begin();
    for (EdgeContainer::iterator i=previous; i!=c.end(); ++i) {
      edge e1 = *i;
      if (copy)
	*previous = e1;
      previous = i;
      if (e1 == e)  
	copy = true;
    }
    if (copy)
      c.pop_back();
  }
  //=======================================================
  /**
   * @brief remove an edge from the edges structure
   * and from the EdgeContainer of its ends
   * except for the end node in argument if it is valid
   * @warning That operation modify the array of edges
   * and thus devalidate all iterators on it.
   * @complexity: o(1)
   */
  void removeFromEdges(const edge e, node end = node()) {
    edgeIds.free(e.id);
    nbEdges--;
    pair<node, node>& eEnds = edges[e.id];
    // remove from source's edges
    node n = eEnds.first;
    if (n != end)
      removeFromEdgeContainer(nodes[n.id], e);
    // remove from target's edges
    n = eEnds.second;
    if (n != end)
      removeFromEdgeContainer(nodes[n.id], e);
  }

#endif // DOXYGEN_NOTFOR_DEVEL

 public:
  //=======================================================
  void clear() {
    nbNodes=0;
    nbEdges=0;
    outDegree.setAll(0);
  }
  //=======================================================
  GraphStorage() {
    clear();
  }
    
  //=======================================================
  ~GraphStorage() {
    for (Nodes::iterator i=nodes.begin();i!=nodes.end();++i) {
      i->deallocateAll();
    }
  }

  //=======================================================
  /**
   * @brief Return true if n belongs to the graph
   * @complexity o(1)
   */
  bool isElement(const node n) const  {
    return !nodeIds.is_free(n.id);
  }
  //=======================================================
  /**
   * @brief Return true if e belongs to the graph
   * @complexity o(1)
   */
  bool isElement(const edge e) const {
    return !edgeIds.is_free(e.id);
  }
  //=======================================================
  /**
   * @brief Enables to reserve memory for nbNodes
   * Reserving memory before to addNode enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   */
  void reserveNodes(size_t nb) {
    if (nb > nbNodes)
      nodes.reserve(nb);
  }
  //=======================================================
  /**
   * @brief Enables to reserve memory for nbEdges
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
    */
  void reserveEdges(size_t nb) {
    if (nb > nbEdges)
      edges.reserve(nb);
  }
  //=======================================================
  /**
   * @brief Enables to reserve memory for adjacency nodes
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   */
  void reserveAdj(node n, size_t nb) {
    EdgeContainer& nEdges = nodes[n.id];
    if (nEdges.size() < nb)
      nEdges.resize(nb);
  }
  //=======================================================
  /**
   * @brief Enables to reserve memory for adjacency nodes
   * Reserving memory before to addEdge enable to reduce the number of vector resizing and then
   * to speed up significantly construction of graphs.
   */
  void reserveAdj(size_t nb) {
    for (unsigned int i = 0; i < nodes.size(); ++i) {
      reserveAdj(node(i), nb);
    }
  }
  //=======================================================
  /**
   * @brief restore adjacency edges of a given node
   */
  void restoreAdj(node n, vector<edge>& edges) {
    EdgeContainer& container = nodes[n.id];
    container.clear();
    for(unsigned int i = 0; i < edges.size(); ++i)
      container.push_back(edges[i]);
  }
  //=======================================================
  /**
   * @brief Return the first node of graph
   */
  node getOneNode() const {
    for (unsigned int i = 0; i < nodes.size(); ++i) {
      if (isElement(node(i)))
	return node(i);
    }
    return node();
  }
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on nodes of the graph
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity: o(1)
   */
  Iterator<node>* getNodes() const {
    return nodeIds.getIds<node>();
  }
  //=======================================================
  /**
   * @brief Return the current state of the IdManager dedicated to nodes
   * this can be used for push/pop
   * @complexity: o(1)
   */
  IdManagerInfos getNodeIdManagerInfos() {
    return nodeIds;
  }
  //=======================================================
  /**
   * @brief restore infos for IdManager dedicated to nodes
   * @complexity: o(1)
   */
  void restoreNodeIdManagerInfos(const IdManagerInfos& infos) {
    return nodeIds.restoreInfos(infos);
  }
  //=======================================================
  /**
   * @brief indicates if allocated node ids are contiguous or not
   * and thus enables file storage optimization
   * @complexity: o(1)
   */
  bool hasFragmentedNodeIds() const {
    return nodeIds.hasFreeIds();
  }
  //=======================================================
  /**
   * @brief return the id of the first allocated node
   * and thus enables file storage optimization
   * @complexity: o(1)
   */
  unsigned int getFirstNodeId() const {
    return nodeIds.getFirstId();
  }
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on edges of the graph
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity: o(1)
   */
  Iterator<edge>* getEdges() const {
    return edgeIds.getIds<edge>();
  }
  //=======================================================
  /**
   * @brief Return the current state of the IdManager dedicated to edges
   * this can be used for push/pop
   * @complexity: o(1)
   */
  IdManagerInfos getEdgeIdManagerInfos() {
    return edgeIds;
  }
  //=======================================================
  /**
   * @brief restore infos for IdManager dedicated to edges
   * @complexity: o(1)
   */
  void restoreEdgeIdManagerInfos(const IdManagerInfos& infos) {
    return edgeIds.restoreInfos(infos);
  }
  //=======================================================
  /**
   * @brief indicates if allocated edge ids are contiguous or not
   * and thus enables file storage optimization
   * 
   * @complexity: o(1)
   */
  bool hasFragmentedEdgeIds() const {
    return edgeIds.hasFreeIds();
  }
  //=======================================================
  /**
   * @brief return the id of the first allocated edge
   * and thus enables file storage optimization
   * @complexity: o(1)
   */
  unsigned int getFirstEdgeId() const {
    return edgeIds.getFirstId();
  }
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on adjacent edges of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity: o(1)
   */
   Iterator<edge>* getInOutEdges(node n) const {
     return new EdgeContainerIterator(nodes[n.id]);
   }
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on out edges of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity: o(1)
   */
   Iterator<edge>* getOutEdges(node n) const {
     return new IOEdgeContainerIterator<OUT>(n, nodes[n.id], edges);
   }
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on in edges of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity: o(1)
   */
   Iterator<edge>* getInEdges(node n) const {
     return new IOEdgeContainerIterator<IN>(n, nodes[n.id], edges);
   }
     
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on adjacent nodes of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity: o(1)
   */
  Iterator<node>* getInOutNodes(node n) const {
    return new IONodesIterator<INOUT>(n, nodes[n.id], edges);
  }
  //=======================================================
  /**
   * @brief Return a Tulip Iterator on in nodes of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity o(1)
   */
  //=======================================================
  Iterator<node>* getInNodes(node n) const {
    return new IONodesIterator<IN>(n, nodes[n.id], edges);
  }
    
  /**
   * @brief Return a Tulip Iterator on out nodes of the node n
   * @warning: The returned iterator should be deleted by the caller to prevent memory leaks
   * @complexity o(1)
   */
  //=======================================================
  Iterator<node>* getOutNodes(node n) const  {
    return new IONodesIterator<OUT>(n, nodes[n.id], edges);
  }
  //=======================================================
  /**
   * @brief Return the degree of a node
   * @complexity o(1)
   */
  unsigned int deg(node n) const {
    assert(isElement(n));
    return nodes[n.id].size();
  }
  //=======================================================
  /**
   * @brief Return the out degree of a node
   * @complexity o(1)
   */
  unsigned int outdeg(node n) const {
    assert(isElement(n));
    return outDegree.get(n.id);
  }

  //=======================================================
  /**
   * @brief Return the in degree of a node
   * @complexity o(1)
   */
  unsigned int indeg(node n) const {
    assert(isElement(n));
    return nodes[n.id].size()- outDegree.get(n.id);
  }
  //=======================================================
  /**
   * @brief Return the number of edges in the graph
   * @complexity: o(1)
   */
  unsigned int numberOfEdges() const {
    return nbEdges;
  }
  //=======================================================
  /**
   * @brief Return the number of nodes in the graph
   * @complexity: o(1)
   */
  unsigned int numberOfNodes() const {
    return nbNodes;
  }
  //=======================================================
  /**
   * @brief Return the extremities of an edge (src, target)
   * @complexity o(1)
   */
  const std::pair<node, node>& ends(const edge e) const {
    assert(isElement(e));
    return edges[e.id];
  }
  //=======================================================
  /**
   * @brief return the first extremity (considered as source if the graph is directed) of an edge
   * @complexity o(1)
   */
  node source(edge e) const {
    return edges[e.id].first;
  }
  //=======================================================
  /**
   * @brief return the second extremity (considered as target if the graph is directed) of an edge
   * @complexity o(1)
   */
  node target(edge e) const {
    return edges[e.id].second;
  }
  //=======================================================
  /**
   * @brief return the opposite node of n through edge e
   * @complexity o(1)
   */
  node opposite(edge e, node n) const {
    const std::pair<node, node>& eEnds = edges[e.id];
    assert((eEnds.first == n) || (eEnds.second == n));
    return (eEnds.first == n) ? eEnds.second : eEnds.first;
  }

  //=======================================================
  /**
   * @brief Reconnect the edge e to have the new given ends
   * @warning That operation modify the array of neighboors of extrmities of edges, thus
   * it devalidates iterators on adjacency for the nodes at the extremities of the modified edges and nodes.
   * @complexity o(1)
   */
  void setEnds(const edge e, const node newSrc, const node newTgt) {
    assert(isElement(e));
    std::pair<node, node>& eEnds = edges[e.id];
    node src = eEnds.first;
    node tgt = eEnds.second;
    // nothing to do if same ends
    if (src == newSrc && tgt == newTgt)
      return;
    node nSrc = newSrc;
    node nTgt = newTgt;

    if (newSrc.isValid() && src != newSrc) {
      assert(isElement(newSrc));
      eEnds.first  = newSrc;
      outDegree.set(src.id, outDegree.get(src.id) - 1);
      outDegree.set(newSrc.id, outDegree.get(newSrc.id) + 1);
      nodes[newSrc.id].push_back(e);
      removeFromEdgeContainer(nodes[src.id], e);
    } else
      nSrc = src;
    if (newTgt.isValid() && tgt != newTgt) {
      assert(isElement(newTgt));
      eEnds.second = newTgt;
      nodes[newTgt.id].push_back(e);
      removeFromEdgeContainer(nodes[tgt.id], e);
    } else
      nTgt = tgt;
  }
  //=======================================================
  /**
   * @brief change the source of an edge
   * @warning That operation modify the array of neighboors of extrmities of edges, thus
   * it devalidates iterators on adjacency for the nodes at the extremities of the modified edges and nodes.
   * @complexity o(1)
   * \see setEnds
   */
  void setSource(const edge e, const node n) {
    setEnds(e, n, node());
  }
  //=======================================================
  /**
   * @brief change the target of an edge
   * @warning That operation modify the array of neighboors of extrmities of edges, thus
   * it devalidates iterators on adjacency for the nodes at the extremities of the modified edges and nodes.
   * @complexity o(1)
   * \see setEnds
   */
  void setTarget(const edge e, const node n) {
    setEnds(e, node(), n);
  }
  //=======================================================
  /**
   * @brief Reverse an edge e, source become target and target become soure
   * @complexity o(1)
   */
  void reverse(const edge e) {
    assert(isElement(e));
    std::pair<node, node>& eEnds = edges[e.id];
    node src = eEnds.first;
    node tgt = eEnds.second;
    eEnds.first  = tgt;
    eEnds.second = src;
    outDegree.set(src.id, outDegree.get(src.id) - 1);
    outDegree.set(tgt.id, outDegree.get(tgt.id) + 1);
  }
  //=======================================================
  /**
   * \brief Set the ordering of edges around n according to their order in v.
   */
  void setEdgeOrder(const node n, const std::vector<edge> &v ) {
    if (v.size()==0) return;
    MutableContainer<int> isEle;
    isEle.setAll(0); 
    for (vector<edge>::const_iterator it=v.begin();it!=v.end();++it) {
      isEle.set(it->id, isEle.get(it->id)+1);
    }
    vector<edge>::const_iterator it2=v.begin();
    EdgeContainer& currentOrder = nodes[n.id];
    for (unsigned int i=0; i<currentOrder.size(); ++i) {
      if ( isEle.get(currentOrder[i].id)>0 ) {
	isEle.set(currentOrder[i].id, isEle.get(currentOrder[i].id) -1);
	currentOrder[i] = *it2;
	++it2;
      }
    }
  }
  //=======================================================
  /**
   * \brief swap to edge in the ordered adjacency vector
   * \warning the two edges must be element of star(v)
   * @complexity o(1)
   */
  void swapEdgeOrder(const node n, const edge e1, const edge e2) {
    //  cerr << __PRETTY_FUNCTION__ << " not tested function" << endl;
    if (e1==e2) return;
    EdgeContainer& adjacency = nodes[n.id];
    unsigned int e1Pos=UINT_MAX,e2Pos=UINT_MAX;
    for (unsigned int i=0;i<deg(n);++i) {
      if (adjacency[i]==e1) e1Pos=i;
      if (adjacency[i]==e2) e2Pos=i;
      if (e1Pos!=UINT_MAX && e2Pos!=UINT_MAX) break;
    }
    assert(e1Pos!=UINT_MAX && e2Pos!=UINT_MAX);
    adjacency[e1Pos]=e2;
    adjacency[e2Pos]=e1;
  }
  //=======================================================
  /**
   * @brief Add the given node in the structure and return it
   * @warning: That operation modify the array of nodes
   * and thus devalidate all iterators on it.
   * @complexity: o(1)
   */
  node addNode(node n) {
    outDegree.set(n.id, 0);
    while (nodes.size() <= n.id){
      nodes.push_back(EdgeContainer());
    }
    nodes[n.id].clear();
    nbNodes++;

    return n;
  }
  //=======================================================
  /**
   * @brief Add a new node in the structure and return it
   * @warning: That operation modify the array of nodes
   * and thus devalidate all iterators on it.
   * @complexity: o(1)
   */
  node addNode() {
    return addNode(node(this->nodeIds.get()));
  }
  //=======================================================
  /**
   * @brief remove a node from the nodes structure only
   * @warning That operation modify the array of nodes
   * and thus devalidate all iterators on it.
   * @complexity: o(1)
   */
  void removeFromNodes(const node n) {
    nodes[n.id].clear();
    nodeIds.free(n.id);
    nbNodes--;
  }
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
  void delNode(node n) {
    assert(isElement(n));
    std::set<edge> loops;
    bool haveLoops = false;
    for(EdgeContainer::iterator i=nodes[n.id].begin(); i!=nodes[n.id].end();
	++i) {
      node s = opposite(*i, n);
      if (s!=n) {
	if (source(*i) == s)
	  outDegree.set(s.id, outDegree.get(s.id) - 1);
	removeFromEdges(*i, n);
      }
      else {
	loops.insert(*i);
	haveLoops = true;
      }
    }
    if (haveLoops) {
      set<edge>::const_iterator it;
      for ( it = loops.begin(); it!=loops.end(); ++it) {
	removeFromEdges(*it, n);
      }
    }
    removeFromNodes(n);
  }
  //=======================================================
  /**
   * @brief Add the given edge between src and tgt and return it
   * the last argument indicates if the edge has to be added
   * in the adjacency edges of its two ends
   * @warning That operation modify the array of edges and
   * the adjacency edges of its ends thus any iterators existing for
   * these structures will be devalidated.
   * @complexity o(1)
   */
  edge addEdge(const node src, const node tgt,
	       const edge e, bool updateEndsEdges) {
    pair< node , node > tmp(src, tgt);
    outDegree.set(src.id, outDegree.get(src.id) + 1);
    while (edges.size()<= e.id){
      edges.push_back(tmp);
    }
    edges[e.id] = tmp;
    if (updateEndsEdges) {
      nodes[src.id].push_back(e);
      nodes[tgt.id].push_back(e);
    }
    nbEdges++;

    return e;
  }
  //=======================================================
  /**
   * @brief Add a new edge between src and tgt and return it
   * @warning That operation modify the array of edges and
   * the adjacency edges of its ends thus any iterators existing for
   * these structures will be devalidated.
   * @complexity o(1)
   */
  edge addEdge(node src, node tgt) {
    return addEdge(src, tgt, edge(edgeIds.get()), true);
  }
  //=======================================================
  /**
   * @brief Delete an edge in the graph
   * @warning: That operation modify the array of edges
   * and thus devalidate all iterators on it.
   * @warning That operation modify the array of neighboors of extremities of the edge e, thus
   * it devalidates iterators on adjacency for the nodes at the extremities od the deleted edge.
   * @warning Orders of edges in the extremities of the deleted edge are affected
   * @complexity o(1)
   */
   void delEdge(edge e) {
     unsigned srcId = source(e).id;
     outDegree.set(srcId, outDegree.get(srcId) - 1);
     removeFromEdges(e);
   }
  //=======================================================
  /**
   * @brief Delete all edges in the graph
   * @warning: That operation modify the array of edges and all arrays of nodes
   * and thus devalidate all iterators, only graph nodes iterators are not affected.
   * @complexity o(E + V)
   */
   void delAllEdges() {
     nbEdges = 0;
     outDegree.setAll(0);
     edges.clear();
     // loop on nodes to clear adjacency edges
     std::vector<EdgeContainer>::iterator it = nodes.begin();
     for (std::vector<EdgeContainer>::iterator it = nodes.begin();
	  it != nodes.end(); ++it) {
       (*it).clear();
     }
   }
  //=======================================================
  /**
   * @brief Delete all nodes in the graph
   * @warning: That operation modify the array of edges and all arrays of nodes
   * and thus devalidate all iterators.
   * @complexity o(E + V)
   */
   void delAllNodes() {
     delAllEdges();
     nbNodes = 0;
     nodes.clear();
   }
  //=======================================================
};
}
#endif // GRAPHSTORAGE_H
