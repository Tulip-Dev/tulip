/**
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
#include <tulip/GraphStorage.h>
#include <tulip/Graph.h>
#include <tulip/memorypool.h>
#include <tulip/MutableContainer.h>

using namespace tlp;

#define VECT_SET_SIZE(v, t, sz) ((t **) v)[1] = ((t **) v)[0] + sz
#define VECT_INC_SIZE(v, t, sz) (*v).reserve(sz); VECT_SET_SIZE(v, t, sz)

//=======================================================
void GraphStorage::clear() {
  nodeData.clear();
  nodeIds.clear();
  edgeIds.clear();
  edgeEnds.clear();
}
//=======================================================
/**
 * @brief reserve memory for nb nodes
 */
void GraphStorage::reserveNodes(const size_t nb) {
  if (nb > nodeData.capacity()) {
    nodeData.reserve(nb);
    nodeIds.reserve(nb);
  }
}
//=======================================================
/**
 * @brief reserve memory for nb edges
 */
void GraphStorage::reserveEdges(const size_t nb) {
  if (nb > edgeEnds.capacity()) {
    edgeEnds.reserve(nb);
    edgeIds.reserve(nb);
  }
}
//=======================================================
/**
 * @brief Enables to reserve memory for adjacency nodes
 */
void GraphStorage::reserveAdj(const node n, const size_t nb) {
  std::vector<edge>& nEdges = nodeData[n.id].edges;

  if (nEdges.size() < nb)
    nEdges.resize(nb);
}
//=======================================================
/**
 * @brief Enables to reserve memory for adjacency nodes
 */
void GraphStorage::reserveAdj(const size_t nb) {
  for (unsigned int i = 0; i < nodeData.size(); ++i) {
    reserveAdj(node(i), nb);
  }
}
//=======================================================
/**
 * @brief restore adjacency edges of a given node
 */
void GraphStorage::restoreAdj(const node n, const std::vector<edge>& edges) {
  std::vector<edge>& nEdges = nodeData[n.id].edges;
  VECT_INC_SIZE(&nEdges, edge, edges.size());
  memcpy(nEdges.data(), edges.data(), edges.size() * sizeof(edge));
}
//=======================================================
// define a simple class to encapsulate the memento of ids
struct IdsMemento :public GraphStorageIdsMemento {
  IdContainer<node> nodeIds;
  IdContainer<edge> edgeIds;
  ~IdsMemento() {}
};
//=======================================================
/**
 * @brief Return the current state of the ids management
 * must be deleted by the caller
 */
const GraphStorageIdsMemento* GraphStorage::getIdsMemento() const {
  IdsMemento* memento = new IdsMemento();
  nodeIds.copyTo(memento->nodeIds);
  edgeIds.copyTo(memento->edgeIds);

  return memento;
}
//=======================================================
/**
 * @brief restore a state of the ids management
 */
void GraphStorage::restoreIdsMemento(const GraphStorageIdsMemento *memento) {
  static_cast<const IdsMemento*>(memento)->nodeIds.copyTo(nodeIds);
  static_cast<const IdsMemento*>(memento)->edgeIds.copyTo(edgeIds);
}
//=======================================================
// specific iterator classes used to implement
// the get*Nodes & get*Edges methods

// define a class to iterate on graph storage edges
struct EdgeContainerIterator :public Iterator<edge>,
  public MemoryPool<EdgeContainerIterator> {
  std::vector<edge>::iterator it, itEnd;
  EdgeContainerIterator(std::vector<edge>& v)
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
  MutableContainer<bool> loops;
  const std::vector<std::pair< node , node > >& edges;
  std::vector<edge>::iterator it, itEnd;

  void prepareNext() {
    for (; it != itEnd; ++it) {
      curEdge = *it;
      // note that io_type value may only be IO_IN which is null
      // or IO_OUT which is define to 1
      node curNode =
        io_type ? edges[curEdge.id].first : edges[curEdge.id].second;

      if (curNode != n)
        continue;

      curNode = io_type ? edges[curEdge.id].second : edges[curEdge.id].first;

      if (curNode == n) {
        if (!loops.get(curEdge.id)) {
          loops.set(curEdge.id, true);
          ++it;
          return;
        }
      }
      else {
        ++it;
        return;
      }
    }

    // set curEdge as invalid
    curEdge = edge();
  }

  IOEdgeContainerIterator(node n, std::vector<edge>& v,
                          const std::vector<std::pair<node, node> >& edges)
    :n(n),edges(edges),it(v.begin()),itEnd(v.end()) {
    loops.setAll(false);
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
  public MemoryPool<IONodesIterator<io_type> > {
  node n;
  const std::vector<std::pair< node , node > >& edges;
  Iterator<edge>* it;

  IONodesIterator(node n, std::vector<edge>& nEdges,
                  const std::vector<std::pair< node , node > >& edges):n(n),
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
Iterator<edge>* GraphStorage::getInOutEdges(const node n) const {
  return new EdgeContainerIterator(nodeData[n.id].edges);
}
//=======================================================
bool GraphStorage::getEdges(const node src, const node tgt, bool directed,
                            std::vector<edge>& vEdges, const Graph *sg,
                            bool onlyFirst) const {
  std::vector<edge>::const_iterator it = nodeData[src.id].edges.begin();
  edge previous;

  while(it != nodeData[src.id].edges.end()) {
    edge e = (*it);

    // loops appear twice
    // be aware that we assume that the second instance of the loop
    // immediatly appears after the first one
    if (e != previous) {
      const std::pair<node, node>& eEnds = edgeEnds[e.id];

      if (((eEnds.second == tgt && eEnds.first == src) ||
           (!directed && eEnds.first == tgt && eEnds.second == src)) &&
          (!sg || sg->isElement(e))) {
        vEdges.push_back(e);

        if (onlyFirst)
          return true;
      }
    }

    previous = e;
    ++it;
  }

  return !vEdges.empty();
}
//=======================================================
Iterator<edge>* GraphStorage::getOutEdges(const node n) const {
  return new IOEdgeContainerIterator<IO_OUT>(n, nodeData[n.id].edges, edgeEnds);
}
//=======================================================
Iterator<edge>* GraphStorage::getInEdges(const node n) const {
  return new IOEdgeContainerIterator<IO_IN>(n, nodeData[n.id].edges, edgeEnds);
}
//=======================================================
Iterator<node>* GraphStorage::getInOutNodes(const node n) const {
  return new IONodesIterator<IO_INOUT>(n, nodeData[n.id].edges, edgeEnds);
}
//=======================================================
Iterator<node>* GraphStorage::getInNodes(const node n) const {
  return new IONodesIterator<IO_IN>(n, nodeData[n.id].edges, edgeEnds);
}
//=======================================================
Iterator<node>* GraphStorage::getOutNodes(const node n) const  {
  return new IONodesIterator<IO_OUT>(n, nodeData[n.id].edges, edgeEnds);
}
//=======================================================
/**
 * @brief Reconnect the edge e to have the new given ends
 */
void GraphStorage::setEnds(const edge e, const node newSrc, const node newTgt) {
  assert(isElement(e));
  std::pair<node, node>& eEnds = edgeEnds[e.id];
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
    NodeData& sCtnr = nodeData[src.id];
    NodeData& nCtnr = nodeData[newSrc.id];
    sCtnr.outDegree -= 1;
    nCtnr.outDegree += 1;
    nCtnr.edges.push_back(e);
    removeFromNodeData(sCtnr, e);
  }
  else
    nSrc = src;

  if (newTgt.isValid() && tgt != newTgt) {
    assert(isElement(newTgt));
    eEnds.second = newTgt;
    nodeData[newTgt.id].edges.push_back(e);
    removeFromNodeData(nodeData[tgt.id], e);
  }
  else
    nTgt = tgt;
}
//=======================================================
/**
 * @brief Reverse an edge e, source become target and target become soure
 */
void GraphStorage::reverse(const edge e) {
  assert(isElement(e));
  std::pair<node, node>& eEnds = edgeEnds[e.id];
  node src = eEnds.first;
  node tgt = eEnds.second;
  eEnds.first  = tgt;
  eEnds.second = src;
  nodeData[src.id].outDegree -= 1;
  nodeData[tgt.id].outDegree += 1;
}
//=======================================================
/**
 *  \brief Set the ordering of edges around n according to their order in v.
 */
void GraphStorage::setEdgeOrder(const node n, const std::vector<edge> &v ) {
  if (v.empty()) return;

  MutableContainer<int> isEle;
  isEle.setAll(0);

  for (std::vector<edge>::const_iterator it=v.begin(); it!=v.end(); ++it) {
    isEle.add(it->id, 1);
  }

  std::vector<edge>::const_iterator it2=v.begin();
  std::vector<edge>& currentOrder = nodeData[n.id].edges;

  for (unsigned int i=0; i<currentOrder.size(); ++i) {
    if (isEle.get(currentOrder[i].id)>0) {
      isEle.add(currentOrder[i].id, -1);
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
  if (e1==e2) return;

  std::vector<edge>& adjacency = nodeData[n.id].edges;
  unsigned int e1Pos=UINT_MAX,e2Pos=UINT_MAX;

  for (unsigned int i=0; i<deg(n); ++i) {
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
 * @brief restore the given node in the structure and return it
 */
void GraphStorage::restoreNode(const node n) {
  if (n.id == nodeData.size())
    nodeData.resize(n.id + 1);
  NodeData& nData = nodeData[n.id];
  // clear edge infos
  nData.edges.clear();
  nData.outDegree = 0;
}
//=======================================================
/**
 * @brief Add a new node in the structure and return it
 * @warning: That operation modify the array of nodes
 * and thus devalidate all iterators on it.
 * @complexity: o(1)
 */
node GraphStorage::addNode() {
  node n(nodeIds.get());
  restoreNode(n);
  return n;
}
//=======================================================
/**
 * @brief Add nb new nodes in the structure
 * and return them in addedNodes
 */
void GraphStorage::addNodes(unsigned int nb, std::vector<node>* addedNodes) {
  if (nb == 0)
    return;

  if (addedNodes) {
    addedNodes->clear();
    addedNodes->reserve(nb);
  }

  unsigned int first = nodeIds.getFirstOfRange(nb);

  if (addedNodes) {
    VECT_INC_SIZE(addedNodes, node, nb);
    memcpy(addedNodes->data(), &nodeIds[first], nb * sizeof(node));
  }

  unsigned int sz = nodeData.size();

  if (sz < nodeIds.size()) {
    nodeData.resize(nodeIds.size());
    // get the number of recycled nodes
    // that need to be restored
    nb -= nodeIds.size() - sz;
  }

  for(unsigned int i = 0; i < nb; ++i)
    restoreNode(nodeIds[first + i]);
}
//=======================================================
/**
 * @brief remove a node from the nodes structure
 */
void GraphStorage::removeFromNodes(const node n) {
  NodeData& nData = nodeData[n.id];
  // clear edge infos
  nData.edges.clear();
  nData.outDegree = 0;
  // push in free pool
  nodeIds.free(n);

  if (nodeIds.empty())
    nodeData.resize(0);
}
//=======================================================
/**
 * @brief Delete a node and all its adjacent edges in the graph
 */
void GraphStorage::delNode(const node n) {
  assert(isElement(n));
  std::vector<edge> loops;

  const std::vector<edge>& edges = nodeData[n.id].edges;

  for(std::vector<edge>::const_iterator i=edges.begin(); i!=edges.end();
      ++i) {
    const std::pair<node, node>& iEnds = ends(*i);
    node src = iEnds.first;
    node tgt = iEnds.second;

    if (src!=tgt) {
      if (src!=n)
        nodeData[src.id].outDegree -= 1;

      removeFromEdges(*i, n);
    }
    else
      loops.push_back(*i);
  }

  for (std::vector<edge>::const_iterator it = loops.begin(); it!=loops.end();
       ++it)
    removeFromEdges(*it, n);

  removeFromNodes(n);
}
//=======================================================
/**
 * @brief Add a new edge between src and tgt and return it
 * @warning That operation modify the array of edges and
 * the adjacency edges of its ends thus any iterators existing for
 * these structures will be devalidated.
 */
void GraphStorage::restoreEdge(const node src, const node tgt, const edge e) {
  std::pair<node, node>& ends = edgeEnds[e.id];
  ends.first = src;
  ends.second = tgt;
  nodeData[src.id].outDegree += 1;
}
//=======================================================
/**
 * @brief Add a new edge between src and tgt and return it
 * @warning That operation modify the array of edges and
 * the adjacency edges of its ends thus any iterators existing for
 * these structures will be devalidated.
 */
edge GraphStorage::addEdge(const node src, const node tgt) {
  edge e(edgeIds.get());

  if (e.id == edgeEnds.size())
    edgeEnds.resize(e.id + 1);

  std::pair<node, node>& ends = edgeEnds[e.id];
  ends.first = src;
  ends.second = tgt;
  NodeData& srcData = nodeData[src.id];
  srcData.outDegree += 1;
  srcData.edges.push_back(e);
  nodeData[tgt.id].edges.push_back(e);

  return e;
}
//=======================================================
/**
 * @brief Add edges in the structure and returns them
 * in the addedEdges vector
 */
void GraphStorage::addEdges(const std::vector<std::pair<node, node> >& ends,
                            std::vector<edge>* addedEdges) {
  unsigned int nb = ends.size();

  if (nb == 0)
    return;

  if (addedEdges) {
    addedEdges->clear();
    addedEdges->reserve(nb);
  }

  unsigned int first = edgeIds.getFirstOfRange(nb);

  if (addedEdges) {
    VECT_INC_SIZE(addedEdges, edge, nb);
    memcpy(addedEdges->data(), &edgeIds[first], nb * sizeof(edge));
  }

  unsigned int sz = edgeEnds.size();

  if (sz < edgeIds.size())
    edgeEnds.resize(edgeIds.size());

  for(unsigned int i = 0; i < nb; ++i) {
    node src = ends[i].first;
    node tgt = ends[i].second;
    edge e = edgeIds[first + i];
    std::pair<node, node>& ends = edgeEnds[e.id];
    ends.first = src;
    ends.second = tgt;
    NodeData& srcData = nodeData[src.id];
    srcData.outDegree += 1;
    srcData.edges.push_back(e);
    nodeData[tgt.id].edges.push_back(e);
  }
}
//=======================================================
/**
 * @brief Delete an edge in the graph
 */
void GraphStorage::delEdge(const edge e) {
  unsigned srcId = source(e).id;
  nodeData[srcId].outDegree -= 1;
  removeFromEdges(e);
}
//=======================================================
/**
 * @brief Delete all edges in the graph
 */
void GraphStorage::delAllEdges() {
  edgeEnds.clear();
  edgeIds.clear();

  // loop on nodes to clear adjacency edges
  for (std::vector<NodeData>::iterator it = nodeData.begin();
       it != nodeData.end(); ++it) {
    (*it).edges.clear();
  }
}
//=======================================================
/**
 * @brief Delete all nodes in the graph
 */
void GraphStorage::delAllNodes() {
  clear();
}

// member functions below do not belong to the public API
// they are just needed by the current implementation
//=======================================================
/**
 * @brief remove an edge from a NodeData
 */
void GraphStorage::removeFromNodeData(NodeData &c, const edge e) {
  std::vector<edge>& edges = c.edges;
  unsigned int nbEdges = edges.size();

  for (unsigned int i = 0; i < nbEdges; ++i)
    if ((e == edges[i]) && (i != nbEdges - 1)) {
      memmove(&edges[i], &edges[i + 1], (nbEdges - i - 1) * sizeof(edge));
      break;
    }

  edges.pop_back();
}
//=======================================================
/**
 * @brief remove an edge from the edges structure
 * and from the NodeData of its ends
 * except for the end node in argument if it is valid
 */
void GraphStorage::removeFromEdges(const edge e, node end) {
  edgeIds.free(e);
  std::pair<node, node>& eEnds = edgeEnds[e.id];
  // remove from source's edges
  node n = eEnds.first;

  if (n != end)
    removeFromNodeData(nodeData[n.id], e);

  // remove from target's edges
  n = eEnds.second;

  if (n != end)
    removeFromNodeData(nodeData[n.id], e);
}
