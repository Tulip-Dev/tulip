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


#ifndef TULIP_PLANARITYIMPL_H
#define TULIP_PLANARITYIMPL_H

#ifndef DOXYGEN_NOTFOR_USER

#include <list>
#include <vector>

#include <tulip/Edge.h>
#include <tulip/MutableContainer.h>
#include <tulip/BmdList.h>
#include <tulip/tulipconf.h>
#include <tulip/Node.h>

namespace tlp {
class Graph;
enum { NOT_VISITED, VISITED, TERMINAL, VISITED_IN_RBC };
static const node NULL_NODE = node();
static const edge NULL_EDGE = edge();

class TLP_SCOPE PlanarityTestImpl {

public:
  PlanarityTestImpl(Graph *sg);
  bool isPlanar(bool embedsg = false);
  static bool isPlanarEmbedding(const Graph *sG);
  std::list<edge> getObstructions();

private:
  bool compute(Graph *);
  void init();
  void restore();
  edge edgeReversal( edge e);
  void makeBidirected(Graph *sG);
  void swapNode(node &n1, node &n2);
  void findTerminalNodes(Graph *sG, node n, std::list<node>& listOfComponents,
                         std::map<node, std::list<node> > &terminalNodes);
  bool findObstruction(Graph *sG, node n, std::list<node>& terminalNodes);
  void setInfoForNewCNode(Graph *sG, node n, node newCNode,
                          std::list<node>& terminalNodes);
  node findActiveCNode(node, node, std::list<node>&);
  void preProcessing(Graph *);
  tlp::BmdLink<node>* searchRBC(int,  tlp::BmdLink<node>*, node, std::list<node>&);
  bool isT0Edge(Graph *, edge);
  bool isBackEdge(Graph *, edge);
  bool isCNode(node);
  void sortNodesIncreasingOrder(Graph *, MutableContainer<int>&, std::vector<node>&);
  node activeCNodeOf(bool, node);
  void addOldCNodeRBCToNewRBC(node, node, node, node, node, BmdList<node>&);
  void updateLabelB(node);
  void calcNewRBCFromTerminalNode(node, node, node, node, BmdList<node>&);
  node lastPNode(node, node);
  node lcaBetween(node, node,  const MutableContainer<node>&);
  node lcaBetweenTermNodes(node, node);
  void calculateNewRBC(Graph *, node, node, std::list<node>&);
  node findNodeWithLabelBGreaterThanDfsN(bool, Graph *, node, node);
  void setPossibleK33Obstruction(node, node, node, node);
  bool testCNodeCounter(Graph *, node, node, node, node, node&, node&);
  bool testObstructionFromTerminalNode(Graph *, node, node, node);

  //functions PlanarityTestObstr.cpp
  bool listEdgesUpwardT0(node n1, node n2);
  void extractBoundaryCycle(Graph *sG, node cNode, std::list<edge>& listEdges);
  //  edge findEdge(Graph *sG, node n1, node n2);
  void obstrEdgesTerminal(Graph* G, node w, node t, node u);
  void addPartOfBc(Graph *sG, node cNode, node n1, node n2, node n3);
  void sortByLabelB(node &n1, node &n2, node &n3);
  void obstrEdgesPNode(Graph *sG, node p, node u);
  void calcInfo3Terminals(node &t1, node &t2, node &t3, int &countMin, int &countF, node &cNode, node &q);
  void obstructionEdgesT0(Graph *sG, node w, node t1, node t2, node t3, node v);
  void obstructionEdgesCountMin1(Graph *sG, node n, node cNode, node t1, node t2, node t3);
  void obstructionEdgesCountMin23(Graph *sG, node n, node cNode, node t1, node t2, node t3, node q, node v);
  //   void obstrEdgesTermCNode(Graph *sG, node w, node t);
  void obstructionEdgesK5(Graph *sG, node w, node cNode, node t1, node t2, node t3);
  void obstructionEdgesPossibleObstrConfirmed(Graph *sG, node w, node t, node v);
  void obstructionEdgesCNodeCounter(Graph *sG, node cNode, node w, node jl, node jr, node t1, node t2);

  // functions PlanarityTestEmbed.cpp
  void embedRoot(Graph *sG, int n);
  void calculatePartialEmbedding(Graph *sG, node w, node newCNode, std::list<edge>& listBackEdges, std::list<node>& terminalNodes);
  void markPathInT(node t, node w, std::map<node, node>& backEdgeRepresentant, std::list<node>& traversedNodes);
  std::map< node, std::list<edge> > groupBackEdgesByRepr(Graph *sG, std::list<edge>& listBackEdges,
      std::map<node, node>& backEdgeRepresentant,
      std::list<node>& traversedNodes,
      std::list<node>& listRepresentants);
  std::list<node> embedUpwardT(bool embBackEdgesOutW, node t1, node t2, Graph *sG, node w,
                               std::map< node, std::list<edge> > &bEdgesRepres,
                               std::list<node>& traversedNodes,
                               BmdList<edge>& embList);
  void addOldCNodeToEmbedding(bool embBackEdgesOutW, Graph *sG, node w, node oldCNode, node u,
                              std::map<node,std::list<edge> >& bEdgesRepres,
                              std::list<node>& traversedNodes,
                              std::list<node>& toEmbedLater,
                              BmdList<edge>& embList);
  void embedBackEdges(bool embBackEdgesOutW, Graph *sG, node repr,
                      std::list<node>& traversedNodes,
                      std::list<edge>& listBackEdges,
                      BmdList<edge>& embList);
  int sortBackEdgesByDfs(Graph *sG, node w, node repr,
                         std::list<edge>& listBackEdges,
                         std::vector<edge>& backEdge);

//   void cleanPtrItem (node n,  tlp::BmdLink<node>* item);

  Graph *sg;
  int totalCNodes;
  bool embed, biconnected;
  node lastNodeInQLinha;
  std::map<edge, edge> bidirectedEdges;
  std::map<edge, edge> reversalEdge;

//   // auxiliary variable to help detecting obstruction;
  node cNodeOfPossibleK33Obstruction;

//   // for each node u in T, children is the list of u's children
//   // ordered in decreasing order by label_b
//   // (it helps to update label_b's in constant time);
//   //node_array<list<node>> childrenInT0;
//   //std::map<node, std::list<node>* > childrenInT0;
  std::map<node, std::list<node> > childrenInT0;

//   // for each 2-connected component represented by r,
//   // list_back_edges[r] is the list of all back-edges in component r
//   // (it helps to calculate an embedding of G, if G is planar);
//   //node_array<list<edge> > listBackEdges;
//   //std::map<node, std::list<edge>* > listBackEdges;
  std::map<node, std::list<edge> > listBackEdges;

//   // the Representative Boundary Cycle for each c-node;
//   //std::map<node, BmdList<node> > RBC;
  std::map<node, BmdList<node> > RBC;

//   // for each node u in G, the algorithm calculates the
//   // clockwise ordering of edges with source u around u, such that
//   // G.sort_edges(embed_list) is a plane map, if it exists
  std::map<node, BmdList<edge> > embedList;

//   // to avoid path compression of c-nodes;
  std::map<tlp::BmdLink<node>*, node> activeCNode;

//   // (it helps to calculate an embedding of G, if G is planar, in
//   // case of 2 terminal nodes);
  BmdList<edge> listBackEdgesOutW;

//   // list of nodes in an obstruction found in G if G is not planar
//   // (it helps to calculate "obstruction_edges");
  std::list<node> obstructionNodes;

//   // list of edges in an obstruction found int G if G is not planar;
  std::list<edge> obstructionEdges;


//   //node_array<edge> backEdgeOut; NON UTILISE

//   //node_map<BmdListItem> ptrItem;
  MutableContainer< tlp::BmdLink<node>*> ptrItem;

//   //node_map<int> dfsPosNum;
  MutableContainer<int> dfsPosNum;

//   //array<node> nodeWithDfsPos;
  MutableContainer<node> nodeWithDfsPos;

//   // to help calculate an embedding or an obstruction;
//   //node_array<edge> T0EdgeIn;
  MutableContainer<edge> T0EdgeIn;

//   //node_map<node>
//   //p0 saves initial DFS tree T_0 of G;
  MutableContainer<node> parent;
  MutableContainer<node> p0;

//   // for each node u in T,
//   // largest_neighbor[u] = max{dfspos_num[v] : v is a neighbor of u in G};
//   //node_map<int> largestNeighbor;
  MutableContainer<int> largestNeighbor;

//   // for each node u in T,
//   // label_b[u] = max{largest_neighbor[v] : v is a descendat of u in T_u}
//   // where T_u is the subtree of T rooted at u;
//   //node_map<int> labelB;
  MutableContainer<int> labelB;

//   // for each node u in T, node_label_b[u] = v
//   // where v is a descendant of u in T and largest_neighbor[v] == label_b[u]
//   // (it helps to find an obstruction in G, if G is not planar);
//   //node_map<node> nodeLabelB;
  MutableContainer<node> nodeLabelB;

//   // to help find the lca between two terminal nodes;
//   //node_map<node> lastVisited;
  MutableContainer<node> lastVisited;

//   // given w, for each terminal node u of w, neighbor_w_terminal[u] is
//   // a descendant of u that is a neighbor of w in G;
//   //node_map<node> neighborWTerminal;
  MutableContainer<node> neighborWTerminal;

//   // to help search for terminal nodes and calculate an embedding of G if G is
//   // planar (states: VISITED, NOT_VISITED, TERMINAL);
//   //node_map<int> state;
  MutableContainer<int> state;

//   // for each (active) c-node d, counter[d] is the number of children of d
//   // with a descendant that are neighbor of w in G;
  MutableContainer<int> counter;

//   // (it helps to calculate an embedding of G, if G is planar);
//   //node_array<bool> hasBackEdge;
  MutableContainer<bool> hasBackEdge;
  unsigned int numberOfNodesInG;
};

}

//std::ostream& operator <<(std::ostream &os , node n);
//std::ostream& operator <<(std::ostream &os , edge e);
std::list<tlp::edge> posDFS(tlp::Graph *sG, tlp::MutableContainer<int> &dfsPos);

#endif
#endif
///@endcond
