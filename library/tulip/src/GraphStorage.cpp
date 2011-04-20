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
#include <tulip/GraphStorage.h>
#include <tulip/memorypool.h>

using namespace tlp;

//=======================================================
void GraphStorage::clear() {
  nbNodes=0;
  nbEdges=0;
  outDegree.setAll(0);
}
//=======================================================
GraphStorage::GraphStorage() {
  clear();
}
//=======================================================
GraphStorage::~GraphStorage() {
  for (Nodes::iterator i=nodes.begin();i!=nodes.end();++i) {
    i->deallocateAll();
  }
}
//=======================================================
/**
 * @brief Return true if n belongs to the graph
 */
bool GraphStorage::isElement(const node n) const  {
  return !nodeIds.is_free(n.id);
}
//=======================================================
/**
 * @brief Return true if e belongs to the graph
 */
bool GraphStorage::isElement(const edge e) const {
  return !edgeIds.is_free(e.id);
}
//=======================================================
  /**
   * @brief Enables to reserve memory for nbNodes
   */
void GraphStorage::reserveNodes(size_t nb) {
  if (nb > nbNodes)
    nodes.reserve(nb);
}
//=======================================================
/**
 * @brief Enables to reserve memory for nbEdges
 */
void GraphStorage::reserveEdges(size_t nb) {
  if (nb > nbEdges)
    edges.reserve(nb);
}
//=======================================================
/**
 * @brief Enables to reserve memory for adjacency nodes
 */
void GraphStorage::reserveAdj(node n, size_t nb) {
  EdgeContainer& nEdges = nodes[n.id];
  if (nEdges.size() < nb)
    nEdges.resize(nb);
}
//=======================================================
/**
 * @brief Enables to reserve memory for adjacency nodes
 */
void GraphStorage::reserveAdj(size_t nb) {
  for (unsigned int i = 0; i < nodes.size(); ++i) {
    reserveAdj(node(i), nb);
  }
}
//=======================================================
/**
 * @brief restore adjacency edges of a given node
 */
void GraphStorage::restoreAdj(node n, std::vector<edge>& edges) {
  EdgeContainer& container = nodes[n.id];
  container.clear();
  for(unsigned int i = 0; i < edges.size(); ++i)
    container.push_back(edges[i]);
}
//=======================================================
/**
 * @brief Return the first node of graph
 */
node GraphStorage::getOneNode() const {
  for (unsigned int i = 0; i < nodes.size(); ++i) {
    if (isElement(node(i)))
      return node(i);
  }
  return node();
}
//=======================================================
/**
 * @brief Return a Tulip Iterator on nodes of the graph
 */
Iterator<node>* GraphStorage::getNodes() const {
  return nodeIds.getIds<node>();
}
//=======================================================
// define a simple class to encapsulate the memento of ids
struct IdsMemento :public GraphStorageIdsMemento {
  IdManagerState nodeIds;
  IdManagerState edgeIds;  
  ~IdsMemento() {}
};
//=======================================================
/**
 * @brief Return the current state of the ids management
 * must be deleted by the caller
 */
const GraphStorageIdsMemento* GraphStorage::getIdsMemento() {
  IdsMemento* memento = new IdsMemento();
  memento->nodeIds = nodeIds.getState();
  memento->edgeIds = edgeIds.getState();

  return memento;
}
//=======================================================
/**
 * @brief restore a state of the ids management
 */
void GraphStorage::restoreIdsMemento(const GraphStorageIdsMemento *memento) {
  nodeIds.restoreState(((IdsMemento *)memento)->nodeIds);
  edgeIds.restoreState(((IdsMemento *)memento)->edgeIds);
}
//=======================================================
/**
 * @brief Return a Tulip Iterator on edges of the graph
 */
Iterator<edge>* GraphStorage::getEdges() const {
  return edgeIds.getIds<edge>();
}
//=======================================================
// specific iterator classes used to implement
// the get*Nodes & get*Edges methods

// define a class to iterate on graph storage edges
struct EdgeContainerIterator :public Iterator<edge>,
			      public MemoryPool<EdgeContainerIterator> {
  SimpleVector<edge>::iterator it, itEnd;
  EdgeContainerIterator(SimpleVector<edge>& v)
    :it(v.begin()), itEnd(v.end()) {
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
// IO_IN => in nodes/edges
// IO_OUT => out nodes/edges
// IO_INOUT => inout nodes/edges
// IO_ prefix is needed on windows platform
// to avoid compilation failure
enum IO_TYPE {IO_IN = 0, IO_OUT = 1, IO_INOUT = 2};

// define a template class to iterate on in or out edges of a given node
template<IO_TYPE io_type>
struct IOEdgeContainerIterator :public Iterator<edge>,
				public MemoryPool<IOEdgeContainerIterator<io_type> > {
  node n;
  edge curEdge;
  std::set<edge> loop;
  const std::vector<std::pair< node , node > >& edges;
  SimpleVector<edge>::iterator it, itEnd;

  void prepareNext() {
    for (;it != itEnd; ++it) {
      curEdge = *it;
      // note that io_type value may only be IO_IN which is null
      // or IO_OUT which is define to 1
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

  IOEdgeContainerIterator(node n, SimpleVector<edge>& v,
			  std::vector<std::pair< node , node > >& edges)
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
struct IONodesIterator :public Iterator<node>,
			public MemoryPool<IONodesIterator<io_type> >{
  node n;
  const std::vector<std::pair< node , node > >& edges;
  Iterator<edge>* it;

  IONodesIterator(node n, SimpleVector<edge>& nEdges,
		  std::vector<std::pair< node , node > >& edges):n(n),
								 edges(edges) {
    if (io_type == IO_INOUT)
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
    const std::pair<node, node>& ends = edges[it->next()];
    // out nodes
    if (io_type == IO_OUT)
      return ends.second;
    // in nodes
    if (io_type == IO_IN)
      return ends.first;
    // inout nodes
    return (ends.first == n) ? ends.second : ends.first;
  }

};
//=======================================================
Iterator<edge>* GraphStorage::getInOutEdges(node n) const {
  return new EdgeContainerIterator(nodes[n.id]);
}
//=======================================================
Iterator<edge>* GraphStorage::getOutEdges(node n) const {
  return new IOEdgeContainerIterator<IO_OUT>(n, nodes[n.id], edges);
}
//=======================================================
Iterator<edge>* GraphStorage::getInEdges(node n) const {
  return new IOEdgeContainerIterator<IO_IN>(n, nodes[n.id], edges);
}
//=======================================================
Iterator<node>* GraphStorage::getInOutNodes(node n) const {
  return new IONodesIterator<IO_INOUT>(n, nodes[n.id], edges);
}
//=======================================================
Iterator<node>* GraphStorage::getInNodes(node n) const {
  return new IONodesIterator<IO_IN>(n, nodes[n.id], edges);
}
//=======================================================
Iterator<node>* GraphStorage::getOutNodes(node n) const  {
  return new IONodesIterator<IO_OUT>(n, nodes[n.id], edges);
}
//=======================================================
/**
 * @brief Return the degree of a node
 */
unsigned int GraphStorage::deg(node n) const {
  assert(isElement(n));
  return nodes[n.id].size();
}

//=======================================================
/**
 * @brief Return the out degree of a node
 */
unsigned int GraphStorage::outdeg(node n) const {
  assert(isElement(n));
  return outDegree.get(n.id);
}

//=======================================================
/**
 * @brief Return the in degree of a node
 */
unsigned int GraphStorage::indeg(node n) const {
  assert(isElement(n));
  return nodes[n.id].size()- outDegree.get(n.id);
}
//=======================================================
/**
 * @brief Return the number of edges in the graph
 */
unsigned int GraphStorage::numberOfEdges() const {
  return nbEdges;
}
//=======================================================
/**
 * @brief Return the number of nodes in the graph
 */
unsigned int GraphStorage::numberOfNodes() const {
  return nbNodes;
}
//=======================================================
/**
 * @brief Return the extremities of an edge (src, target)
 */
const std::pair<node, node>& GraphStorage::ends(const edge e) const {
  assert(isElement(e));
  return edges[e.id];
}
//=======================================================
/**
 * @brief return the first extremity (considered as source if the graph is directed) of an edge
 */
node GraphStorage::source(edge e) const {
  return edges[e.id].first;
}
//=======================================================
/**
 * @brief return the second extremity (considered as target if the graph is directed) of an edge
 * @complexity o(1)
 */
node GraphStorage::target(edge e) const {
  return edges[e.id].second;
}
//=======================================================
/**
 * @brief return the opposite node of n through edge e
 */
node GraphStorage::opposite(edge e, node n) const {
  const std::pair<node, node>& eEnds = edges[e.id];
  assert((eEnds.first == n) || (eEnds.second == n));
  return (eEnds.first == n) ? eEnds.second : eEnds.first;
}

//=======================================================
/**
 * @brief Reconnect the edge e to have the new given ends
 */
void GraphStorage::setEnds(const edge e, const node newSrc, const node newTgt) {
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
 */
void GraphStorage::setSource(const edge e, const node n) {
  setEnds(e, n, node());
}
//=======================================================
/**
 * @brief change the target of an edge
 */
void GraphStorage::setTarget(const edge e, const node n) {
  setEnds(e, node(), n);
}
//=======================================================
/**
 * @brief Reverse an edge e, source become target and target become soure
 */
void GraphStorage::reverse(const edge e) {
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
 *  àbrief Set the ordering of edges around n according to their order in v.
 */
void GraphStorage::setEdgeOrder(const node n, const std::vector<edge> &v ) {
  if (v.size()==0) return;
  MutableContainer<int> isEle;
  isEle.setAll(0); 
  for (std::vector<edge>::const_iterator it=v.begin();it!=v.end();++it) {
    isEle.set(it->id, isEle.get(it->id)+1);
  }
  std::vector<edge>::const_iterator it2=v.begin();
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
void GraphStorage::swapEdgeOrder(const node n, const edge e1, const edge e2) {
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
 */
node GraphStorage::addNode(node n) {
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
 */
node GraphStorage::addNode() {
  return addNode(node(this->nodeIds.get()));
}
//=======================================================
/**
 * @brief remove a node from the nodes structure only
 */
void GraphStorage::removeFromNodes(const node n) {
  nodes[n.id].clear();
  nodeIds.free(n.id);
  nbNodes--;
}
//=======================================================
/**
 * @brief Delete a node and all its adjacent edges in the graph
 */
void GraphStorage::delNode(node n) {
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
    std::set<edge>::const_iterator it;
    for ( it = loops.begin(); it!=loops.end(); ++it) {
      removeFromEdges(*it, n);
    }
  }
  removeFromNodes(n);
}
//=======================================================
/**
 * @brief Add the given edge between src and tgt and return it
 */
edge GraphStorage::addEdge(const node src, const node tgt,
			   const edge e, bool updateEndsEdges) {
  std::pair< node , node > tmp(src, tgt);
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
 */
edge GraphStorage::addEdge(node src, node tgt) {
  return addEdge(src, tgt, edge(edgeIds.get()), true);
}
//=======================================================
/**
 * @brief Delete an edge in the graph
 */
void GraphStorage::delEdge(edge e) {
  unsigned srcId = source(e).id;
  outDegree.set(srcId, outDegree.get(srcId) - 1);
  removeFromEdges(e);
}
//=======================================================
/**
 * @brief Delete all edges in the graph
 */
void GraphStorage::delAllEdges() {
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
 */
void GraphStorage::delAllNodes() {
  delAllEdges();
  nbNodes = 0;
  nodes.clear();
}

// member functions below do not belong to the public API
// they are just needed by the current implementation
//=======================================================
/**
 * @brief remove an edge from an EdgeContainer
 */
 void GraphStorage::removeFromEdgeContainer(EdgeContainer &c, const edge e) {
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
 */
void GraphStorage::removeFromEdges(const edge e, node end) {
  edgeIds.free(e.id);
  nbEdges--;
  std::pair<node, node>& eEnds = edges[e.id];
  // remove from source's edges
  node n = eEnds.first;
  if (n != end)
    removeFromEdgeContainer(nodes[n.id], e);
  // remove from target's edges
  n = eEnds.second;
  if (n != end)
    removeFromEdgeContainer(nodes[n.id], e);
}

