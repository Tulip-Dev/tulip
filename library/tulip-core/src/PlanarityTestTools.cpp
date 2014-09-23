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

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include <tulip/PlanarityTestImpl.h>
#include <tulip/StableIterator.h>
#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

/*std::ostream& operator <<(std::ostream &os , node n) {
  os << n.id;
  return os;
}
std::ostream& operator <<(std::ostream &os , edge e) {
  os << e.id;
  return os;
  }*/

// //=================================================================
edge PlanarityTestImpl::edgeReversal( edge e) {
  return reversalEdge[e];
}
//=================================================================

static int preCount;
static int postCount;

void dfsAux(Graph *sG, node n,
            MutableContainer<int>& dfsPre,
            MutableContainer<int>& dfsPos,
            list<edge>& dfsEdges) {
  dfsPre.set(n.id, preCount++);
  StableIterator<edge> it(sG->getOutEdges(n));

  while (it.hasNext()) {
    edge e = it.next();
    node target = sG->target(e);

    if (dfsPre.get(target.id) == 0) {
      dfsEdges.push_back(e);
      dfsAux(sG, target, dfsPre, dfsPos, dfsEdges);
    }
  }

  dfsPos.set(n.id, postCount++);
}
//=================================================================
list<edge> posDFS(Graph *sG,
                  MutableContainer<int> &dfsPos) {
  list<edge> dfsEdges;
  MutableContainer<int> dfsPre;
  dfsPre.setAll(0);
  preCount = postCount = 1;
  StableIterator<node> it(sG->getNodes());

  while (it.hasNext()) {
    node n = it.next();

    if (dfsPre.get(n.id) == 0)
      dfsAux(sG, n, dfsPre, dfsPos, dfsEdges);
  }

  return dfsEdges;
}
//=================================================================
void PlanarityTestImpl::makeBidirected(Graph *sG) {
  StableIterator<edge> stIte(sG->getEdges());

  while(stIte.hasNext()) {
    edge e = stIte.next();
    pair<node, node> eEnds = sG->ends(e);
    edge newEdge = sG->addEdge(eEnds.second, eEnds.first);
    bidirectedEdges[newEdge] = e;
    reversalEdge[newEdge] = e;
    reversalEdge[e] = newEdge;
  }
}
//=================================================================
/*
 * Returns true for all edges e in T_0,
 *         false otherwise.
 */
bool PlanarityTestImpl::isT0Edge(Graph *g, edge e) {
  pair<node, node> eEnds = g->ends(e);
  edge e1 = T0EdgeIn.get(eEnds.second.id);

  //test ą revoir je pense qu'en testant juste e == e1 ēa suffit !
  if (e1 != NULL_EDGE) {
    pair<node, node> e1Ends = g->ends(e1);

    if (e1Ends.first == eEnds.first && e1Ends.second == eEnds.second)
      return true;
  }

  e1 = T0EdgeIn.get(eEnds.first.id);

  if (e1 == NULL_EDGE)
    return false;

  pair<node, node> e1Ends = g->ends(e1);
  return (e1Ends.second == eEnds.first && e1Ends.first == eEnds.second);
}
//=================================================================
/*
 * Returns true for all edges e in g - T_0,
 *         false otherwise.
 */
bool PlanarityTestImpl::isBackEdge(Graph *g, edge e) {
  if (e == NULL_EDGE) return false;

  return (!isT0Edge(g, e));
}
//=================================================================
/*
 * Returns in sortedNodes the nodes n of g sorted in increasing
 * order by value[n].
 * Precondition:
 * - value[n] <= numberOfNodes for all nodes n of g, where n is
 * the number of
 * nodes in g
 */
void PlanarityTestImpl::sortNodesIncreasingOrder(Graph *g, MutableContainer<int> &value,
    vector<node> &sortedNodes) {

  // Counting sort;
  int numberOfNodes = g->numberOfNodes();
  //array<int, numberOfNodes + 1> c;
  vector<int> c(numberOfNodes + 1);

  for (int i = 1 ; i <= numberOfNodes ; ++i)
    c[i] = 0;

  //array<node, numberOfNodes + 1> a;
  vector<node> a(numberOfNodes + 1);
  int j = 0;
  node n;
  // forall_nodes(n, g)
  Iterator<node> *it = g->getNodes();

  while (it->hasNext()) {
    //a[++j] = n;
    a[++j] = it->next();
  }

  delete it;

  for (int i = 1 ; i <= numberOfNodes ; ++i) {
    unsigned int tmp = value.get(a[i].id);
    ++c[tmp];
  }

  for (int i = 2 ; i <= numberOfNodes ; ++i)
    c[i] += c[i-1];

  for (int i = numberOfNodes ; i > 0 ; i--) {
    sortedNodes[c[value.get(a[i].id)]] = a[i];
    c[value.get(a[i].id)]--;
  }
}
//=================================================================
/*
 * Calculates:
 * - DFS tree T_0 (initial T) of g represented as parent[n] for
 * all nodes n in g;
 * - dfspos_num[n] for all nodes n in T_0;
 * - node_with_dfspos[i] = n if dfspos_num[n] == i, for i = 1, ..,
 * numberOfNodes, where numberOfNodes is the number of nodes in g;
 * - largest_neighbor[n] for all nodes n in T_0;
 * - label_b[u] for all nodes n in T_0 (see PlanarityTest.h for
 * more details);
 * - nodeLabelB[n] for all nodes n in T_0 (see PlanarityTest.h for
 * more details);
 * - childrenInT0[u] for each node n in T_0 (children of n in T_0
 * in decreasing
 *   order by labelB -- needed by updateLabelB);
 */
void PlanarityTestImpl::preProcessing(Graph *g) {
  int numberOfNodes = g->numberOfNodes();
  list<edge> edgeInT0; // list of edges in T_0;
  edgeInT0 = posDFS(g, dfsPosNum);

  //forall(e, edgeInT0) {
  //  tlp::warning() << "Edge in T0: " ;
  for (list<edge>::iterator it = edgeInT0.begin(); it != edgeInT0.end(); ++it) {
    edge e = *it;
    //    tlp::warning() << e.id << ",";
    node n = g->source(e);
    node v = g->target(e);
    parent.set(v.id, n);
    T0EdgeIn.set(v.id, e);
  }

  //  tlp::warning() << endl;

#ifndef NDEBUG

  for (list<edge>::iterator it = edgeInT0.begin(); it != edgeInT0.end(); ++it) {
    edge e = *it;
    assert (!(isBackEdge(g, e) || isBackEdge(g, edgeReversal(e))));
  }

#endif


  //  tlp::warning() << "DFS order :" ;
  //node n;
  //forall_nodes(n, g) {
  Iterator<node> *it1 = g->getNodes();

  while (it1->hasNext()) {
    node n = it1->next();
    int dfsPos = dfsPosNum.get(n.id);
    //    tlp::warning() << "(" << n.id << "," << dfsPos << ")";
    nodeWithDfsPos.set(dfsPos, n);
    largestNeighbor.set(n.id, dfsPos);
    labelB.set(n.id, dfsPos);

    if ((parent.get(n.id)) != NULL_NODE) // if u is not the root of T_0;
      largestNeighbor.set(n.id, dfsPosNum.get((parent.get(n.id)).id));

    if (embed)
      p0.set(n.id, parent.get(n.id));

    // saves T_0 for obstruction edges if g is not planar;
  }

  delete it1;
  //  tlp::warning() << endl;

  for (int i = 1 ; i <= numberOfNodes ; ++i) {
    node n = nodeWithDfsPos.get(i);
    //forall_out_edges (e, n) {
    Iterator<edge> *it = g->getOutEdges(n);

    while (it->hasNext()) {
      edge e = it->next();
      node v = g->target(e);

      if (labelB.get(n.id) < labelB.get(v.id))
        labelB.set(n.id, labelB.get(v.id));

      if (largestNeighbor.get(n.id) < dfsPosNum.get(v.id))
        largestNeighbor.set(n.id, dfsPosNum.get(v.id));//dfsPosNum[v];
    }

    delete it;
  }

  /// DEBUG {
  // tlp::debug() << "** Labels (dfs[u], h[u], b[u]): " << endl;
  //   for (int i = 1 ; i <= numberOfNodes ; i++) {
  //     node u = nodeWithDfsPos.get(i);
  //     tlp::debug() << "(" << dfsPosNum.get(u.id) << "," << largestNeighbor.get(u.id)
  //   << "," << labelB.get(u.id) << ") " << endl;
  //   }
  //// }

  //array<node, numberOfNodes+1> sortedNodes;
  vector<node> sortedNodes(numberOfNodes + 1);
  sortedNodes[0]=NULL_NODE;
  sortNodesIncreasingOrder(g, labelB, sortedNodes);

  /*
    tlp::warning() << "Sorted Nodes : ";
  for (int i=0; i<=numberOfNodes; ++i)
    tlp::warning() << "[" << sortedNodes[i].id << ","  ;
  tlp::warning() << endl;
  */
  for (int i = numberOfNodes ; i >= 1 ; i--) {
    node n = sortedNodes[i];
    node v = parent.get(n.id);

    //tlp::warning() << "[" << n.id << "," << v.id << "]" << endl;
    if (v != NULL_NODE)
      childrenInT0[v].push_back(n);
  }

  /*
    tlp::debug() << "** List of children in T:\n";
    Iterator<node> *it = g->getNodes();
    while (it->hasNext()) {
    node u = it->next();
    //tlp::warning() << "  " << dfsPosNum.get(u.id) << ": " ;
    node v;
    for (list<node>::iterator i = childrenInT0[u].begin(); i != childrenInT0[u].end(); i++) {
    node v = *i;
    tlp::debug() << dfsPosNum.get(v.id) << "(" << labelB.get(v.id) << "), ";
    tlp::debug() << "\n";
    }
    } delete it;
  */
  if (embed) // needed to calculate obstruction edges;
    for (int i = 1 ; i <= numberOfNodes ; ++i) {
      node n = nodeWithDfsPos.get(i);

      if (labelB.get(n.id) == largestNeighbor.get(n.id))
        nodeLabelB.set(n.id, n);
      else {
        node v = childrenInT0[n].front();
        nodeLabelB.set(n.id, nodeLabelB.get(v.id));
      }
    }
}
//=================================================================
void PlanarityTestImpl::swapNode(node &n1, node &n2) {
  node tmp = n1;
  n1 = n2;
  n2 = tmp;
}
//=================================================================
bool PlanarityTestImpl::isCNode(node n) {
  if (n == NULL_NODE) return false;

  return (dfsPosNum.get(n.id) < 0);
}
//=================================================================
node PlanarityTestImpl::activeCNodeOf(bool calculatingObstruction, node n) {
  node cNode = n;

  if (!isCNode(n))
    cNode = parent.get(n.id);

  if (!isCNode(cNode))
    return NULL_NODE; //zero

  //debug
  if (calculatingObstruction)
    while (isCNode(parent.get(cNode.id)))
      cNode = parent.get(cNode.id);
  else {
    assert(!isCNode(parent.get(cNode.id)));
  }

  return cNode;
}
//=================================================================
void PlanarityTestImpl::addOldCNodeRBCToNewRBC(node oldCNode,
    node,
    node n,
    node n1,
    node n2,
    tlp::BmdList<node>& nodeList) {
  // compress RBC[oldCNode]:
  // removes all nodes v in RBC[oldCNode] s.t. label_b[v] < dfspos_num[n]
  // or v = parent[oldCNode].

  BmdLink<node>* firstItem = RBC[oldCNode].firstItem();
  BmdLink<node>* predItem  = RBC[oldCNode].cyclicPred(firstItem, 0);
  BmdLink<node>* succItem  = RBC[oldCNode].cyclicSucc(firstItem, 0);

  node predNode = predItem->getData();
  node succNode = succItem->getData();
  node ul = NULL_NODE;
  node ur = NULL_NODE;

  // goes to the left;
  while (labelB.get(predNode.id) == dfsPosNum.get(n.id) && predNode != n1 && predNode != n2) {
    if (ul.id == UINT_MAX)
      ul = predNode;

    BmdLink<node>* tmp = predItem;
    predItem = RBC[oldCNode].cyclicPred(predItem, firstItem);
    //tlp::debug() << "tulip predItem : " << predItem->getData().id << endl;
    predNode = predItem->getData();
    //tlp::debug() << "tulip predNode : " << predNode.id << endl;
    //tlp::debug() << "tulip tmp : " << tmp->getData().id << endl;
    RBC[oldCNode].delItem(tmp);
  }


  // goes to right;
  while (labelB.get(succNode.id) == dfsPosNum.get(n.id) && succNode != n1 && succNode != n2) {
    if (ur.id == UINT_MAX)
      ur = succNode;

    BmdLink<node>* tmp = succItem;
    succItem = RBC[oldCNode].cyclicSucc(succItem, firstItem);
    succNode = succItem->getData();
    RBC[oldCNode].delItem(tmp);
  }

  RBC[oldCNode].delItem(RBC[oldCNode].firstItem());

  // endpoint to correctly concatentates with RBC[new_cnode];
  node first = n1;

  if (n1.id == UINT_MAX) {
    if (ul!= NULL_NODE)
      first = predNode;
    else
      first = succNode;
  }

  // note that oldCNode may have flipped;
  if (RBC[oldCNode].lastItem()->getData() == first)
    RBC[oldCNode].reverse();

  // reoves n1 and n2 from RBC[oldCNode];
  if (n1!= NULL_NODE) {
    RBC[oldCNode].delItem(RBC[oldCNode].firstItem());
  }

  if (n2!= NULL_NODE) {
    RBC[oldCNode].delItem(RBC[oldCNode].lastItem());
  }

  nodeList.conc(RBC[oldCNode]);
}
//=================================================================
void PlanarityTestImpl::updateLabelB(node n) {
  if (n.id == UINT_MAX)
    return;

  labelB.set(n.id, largestNeighbor.get(n.id));

  if (embed)
    nodeLabelB.set(n.id, n); // needed to calculate obstruction edges if G is not planar;

  node v;

  if (!childrenInT0[n].empty())
    v = childrenInT0[n].front();
  else
    return;

  while (v!= NULL_NODE) {
    // or v is not a descendant of n in T,
    // or v is a child of n in T,
    // or v is a child of a c-node whose parent is n;
    if ((parent.get(v.id))!= NULL_NODE && isCNode(parent.get(v.id)) && parent.get(parent.get(v.id).id) == n) {
      v = parent.get(v.id); // active c-node;
      break;
    }

    if (parent.get(v.id) == n)
      break;

    // v became sibling of n;
    childrenInT0[n].pop_front();

    if (!childrenInT0[n].empty())
      v = childrenInT0[n].front();
    else v = NULL_NODE;
  }

  if (v!= NULL_NODE && labelB.get(n.id) < labelB.get(v.id)) {
    labelB.set(n.id, labelB.get(v.id));

    if (embed)
      nodeLabelB.set(n.id, nodeLabelB.get(v.id)); // needed to calculate obstruction edges;
  }
}
//=================================================================
void PlanarityTestImpl::calcNewRBCFromTerminalNode(node newCNode,
    node n,
    node n1,
    node n2,
    tlp::BmdList<node>& nodeList) {
  node t = n1;
  node predT = NULL_NODE;
  node parentT = NULL_NODE;

  while (t != n2) {
    parentT = parent.get(t.id);

    //    tlp::debug() << "Tulip t =  " << t.id << "  : ";
    if (isCNode(t)) {
      //      tlp::debug() << "Is CNode" << endl;
      t = activeCNodeOf(false, t);
      addOldCNodeRBCToNewRBC(t, newCNode, n, predT, NULL_NODE, nodeList);
      parentT = parent.get(t.id);
      parent.set(t.id, newCNode);

      if (labelB.get(t.id) > labelB.get(newCNode.id)) {
        labelB.set(newCNode.id, labelB.get(t.id));

        if (embed) // needed to calculate obstruction edges;
          nodeLabelB.set(newCNode.id, nodeLabelB.get(t.id));
      }
    }
    else {
      //      tlp::debug() << "Not Is CNode :" << t << endl;
      parent.set(t.id, newCNode);
      updateLabelB(t);

      if (labelB.get(t.id) > dfsPosNum.get(n.id)) {
        BmdLink<node>* item = nodeList.append(t);
        ptrItem.set(t.id, item) ;//nodeList.append(t));
      }

      if (labelB.get(t.id) > labelB.get(newCNode.id)) {
        labelB.set(newCNode.id, labelB.get(t.id));

        if (embed) // needed to calculate obstruction edges;
          nodeLabelB.set(newCNode.id, nodeLabelB.get(t.id));
      }
    }

    if (!isCNode(t))
      predT = t; // predT is never a c-node;

    t = parentT;
  }
}
//=================================================================
node PlanarityTestImpl::lastPNode(node n1, node n2) {
  //  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  if (n1 == n2) {
    if (!isCNode(n1))
      return n1;
    else
      return NULL_NODE;
  }

  list<node> S;
  node n = n1;

  while (n!= NULL_NODE && n != n2) {
    S.push_front(n);
    n = parent.get(n.id);
  }

  if (n == NULL_NODE)
    return NULL_NODE;

  n = n2; //inutile

#ifndef NDEBUG
  int count = 0;
#endif

  while (isCNode(n)) {
    assert(++count<=2);

    if (S.empty())
      return NULL_NODE;

    n = S.front();
    S.pop_front();
  }

  return n;
}
//=================================================================
node PlanarityTestImpl::lcaBetween(node n1,
                                   node n2,
                                   const MutableContainer<node> &p) {
  if (isCNode(n1)) {
    node n = activeCNodeOf(false, n1);
    n1 = p.get(n.id);
  }

  if (isCNode(n2)) {
    node n = activeCNodeOf(false, n2);
    n2 = p.get(n.id);
  }

  if (dfsPosNum.get(n1.id) > dfsPosNum.get(n2.id))
    swapNode(n1, n2);

  list<node> nl;

  while (dfsPosNum.get(n1.id) < dfsPosNum.get(n2.id)) {
    nl.push_front(n1);
    n1 = p.get(n1.id);
  }

  node u = NULL_NODE;

  if (!nl.empty()) {
    u = nl.front();
    nl.pop_front();
  }

  while (n2 != u && n2 != n1 && dfsPosNum.get(n2.id) < dfsPosNum.get(n1.id)) {
    nl.push_front(n2);
    n2 = p.get(n2.id);
  }

  if (n2 == u || n2 == n1)
    return n2;

  return nl.front();
}
//=================================================================
node PlanarityTestImpl::lcaBetweenTermNodes(node n1,
    node n2) {
  node lca = lastVisited.get(n1.id);

  if (dfsPosNum.get((lastVisited.get(n2.id)).id) < dfsPosNum.get(lca.id))
    lca = lastVisited.get(n2.id);

  //==
  assert(lca == lcaBetween(n1,n2, parent));
  //==
  return lca;
}
//=================================================================
void PlanarityTestImpl::calculateNewRBC(Graph*,
                                        node newCNode,
                                        node n,
                                        list<node>& terminalNodes) {
  switch(terminalNodes.size()) {
  case 1: {
    node n1 = terminalNodes.front();
    terminalNodes.pop_front();
    ////
    calcNewRBCFromTerminalNode(newCNode, n, n1, n, RBC[newCNode]);
  }
  break;

  case 2: {
    node m = NULL_NODE;
    node t1 = terminalNodes.front();
    terminalNodes.pop_front();
    node t2 = terminalNodes.front();
    terminalNodes.pop_front();
    node aux1 = t1;
    node aux2 = t2;

    // makes t1 with lower dfspos_num than t2;
    if (isCNode(aux1)) {
      aux1 = activeCNodeOf(false, aux1);
      aux1 = parent.get(aux1.id);
    }

    if (isCNode(aux2)) {
      aux2 = activeCNodeOf(false, aux2);
      aux2 = parent.get(aux2.id);
    }

    if (dfsPosNum.get(aux1.id) > dfsPosNum.get(aux2.id))
      swapNode(t1, t2);

    m = lcaBetweenTermNodes(t1, t2);
    node t12 = lastPNode(t1, m), t22 = lastPNode(t2, m);
    BmdList<node> auxNodeList;
    calcNewRBCFromTerminalNode(newCNode, n, t1, t12, auxNodeList);

    calcNewRBCFromTerminalNode(newCNode, n, t2, t22, RBC[newCNode]);

    // adds m to RBC[newCNode];
    if (isCNode(m)) {
      m = activeCNodeOf(false, m);
      parent.set(t12.id, newCNode);
      parent.set(t22.id, newCNode);
      updateLabelB(t12);

      if (labelB.get(t12.id) > dfsPosNum.get(n.id))
        ptrItem.set(t12.id, auxNodeList.append(t12));

      updateLabelB(t22);

      if (labelB.get(t22.id) > dfsPosNum.get(n.id))
        ptrItem.set(t22.id, RBC[newCNode].append(t22));

      addOldCNodeRBCToNewRBC(m, newCNode, n, t22, t12, RBC[newCNode]);
      parent.set(m.id, newCNode);
    }
    else {
      parent.set(m.id, newCNode);
      updateLabelB(m);

      if (labelB.get(m.id) > dfsPosNum.get(n.id))
        ptrItem.set(m.id, RBC[newCNode].append(m));
    }

    if (labelB.get(m.id) > labelB.get(newCNode.id)) {
      labelB.set(newCNode.id, labelB.get(m.id));

      if (embed) // needed to calculate obstruction edges;
        nodeLabelB.set(newCNode.id, nodeLabelB.get(m.id));
    }

    auxNodeList.reverse();
    RBC[newCNode].conc(auxNodeList);
  }
  break;

  default:
    assert(false);
  }
}
//=================================================================
node PlanarityTestImpl::findNodeWithLabelBGreaterThanDfsN(bool saveLastNodeTraversed,
    Graph*,
    node n,
    node t) {
  node result = NULL_NODE;
  node vPred = t;
  node v = parent.get(t.id);

  list<node> nl, nl2; // to restore parent, label_b and node_label_b;
  map<node, node> p, nb;
  map<node, int> b;

  p[t] = parent.get(t.id); // saves info;
  nl.push_back(t);
  parent.set(t.id, NULL_NODE);

  while (v != n) {
    if (isCNode(v)) {
      v = activeCNodeOf(false, v);
      BmdListIt<node> it(RBC[v]);

      while (it.hasNext()) {
        node u = it.next();

        if (labelB.get(u.id) > dfsPosNum.get(n.id) && u != t && u != parent.get(v.id)) {
          result = u;
          break;
        }
      }
    }
    else {
      nl2.push_back(v);
      b[v] = labelB.get(v.id); // saves infos;

      if (embed) // needed to calculate obstruction edges;
        nb[v] = nodeLabelB.get(v.id);

      updateLabelB(v);

      if (labelB.get(v.id) > dfsPosNum.get(n.id)) {
        result = v;
        break;
      }
    }

    vPred = v;
    v = parent.get(v.id);
    p[vPred] = parent.get(vPred.id);
    nl.push_back(vPred);
    parent.set(vPred.id, NULL_NODE);
  }

  // restores parent;
  //forall(v, nl)
  for (list<node>::iterator it1 = nl.begin(); it1 != nl.end(); ++it1)
    parent.set((*it1).id, p[*it1]);


  if (result!= NULL_NODE)
    return result;

  // restores info if result is nil;
  //forall(v, nl2) {
  for (list<node>::iterator it2 = nl2.begin(); it2 != nl2.end(); ++it2) {
    labelB.set((*it2).id, b[*it2]);

    if (embed) // needed to calculate obstruction edges;
      nodeLabelB.set((*it2).id, nb[*it2]);
  }

  // to update label_b of n correctly in future iteration
  // (vPred is the child of n in T that belongs to Q_linha);
  if (saveLastNodeTraversed && vPred != t)
    lastNodeInQLinha = vPred; // see sets_info_for_new_cnode;
  else
    lastNodeInQLinha = NULL_NODE;

  return NULL_NODE;
}
//=================================================================
void PlanarityTestImpl::setPossibleK33Obstruction(node cNode,
    node n,
    node nl,
    node nr) {
  // K_33: X = {w, parent[cnode], u}
  //       Y = {ul, ur, ?};
  obstructionNodes.clear();
  obstructionNodes.push_back(n);
  obstructionNodes.push_back(nl);
  obstructionNodes.push_back(nr);
  cNodeOfPossibleK33Obstruction = cNode;
}
//=================================================================
bool PlanarityTestImpl::testCNodeCounter(Graph*,
    node cNode,
    node n,
    node n1,
    node n2,
    node& jl,
    node& jr) {
  assert (!(parent.get(cNode.id) != RBC[cNode].firstItem()->getData() ));

  // no chance for cNode to be an obstruction;
  /// code ajoute
  //  if (RBC.get(cNode.id) == 0)
  //    RBC.set(cNode.id, new BmdList<node>());
  ////
  if (RBC[cNode].size() <= 2)
    return false;

  BmdLink<node>* it1 = RBC[cNode].firstItem();
  BmdLink<node>* it1l = RBC[cNode].cyclicPred(it1, 0);
  BmdLink<node>* it1r = RBC[cNode].cyclicSucc(it1, 0);

  jl = it1l->getData();
  jr = it1r->getData();
  node ul = NULL_NODE;
  node ur = NULL_NODE;
  int count = 0;

  // goes to the left;
  BmdLink<node> *a=NULL, *s = it1;

  while (labelB.get(jl.id) <= dfsPosNum.get(n.id)) {
    if (labelB.get(jl.id) == dfsPosNum.get(n.id)) {
      ul = jl;
      ++count;
    }

    a = it1l;
    it1l = RBC[cNode].cyclicPred(it1l, s);
    s = a;
    jl = it1l->getData();
  }

  // goes to right;
  BmdLink<node>*  p = it1;

  while (labelB.get(jr.id) <= dfsPosNum.get(n.id)) {
    if (labelB.get(jr.id) == dfsPosNum.get(n.id)) {
      ur = jr;
      ++count;
    }

    a = it1r;
    it1r = RBC[cNode].cyclicSucc(it1r, p);
    p = a;
    jr = it1r->getData();
  }

  if (jl == n1 || jl == n2)
    ++count;

  if (jr != jl && (jr == n1 || jr == n2))
    ++count;

  if (count != counter.get(cNode.id))
    return true;

  node u = NULL_NODE;

  if (ul!= NULL_NODE && ur!= NULL_NODE)
    u = jl; // or u = jr;
  else if (ul!= NULL_NODE && n1!= NULL_NODE && n1 != jl) {
    u = jl;
    ur = n1;
  }
  else if (ur!= NULL_NODE && n1!= NULL_NODE && n1 != jr) {
    u = jr;
    ul = n1;
  }

  if (cNodeOfPossibleK33Obstruction == NULL_NODE && u != NULL_NODE)
    setPossibleK33Obstruction(cNode, u, ul, ur);

  return false;
}
//=================================================================
bool PlanarityTestImpl::testObstructionFromTerminalNode(Graph *sG,
    node w,
    node terminal,
    node u) {
  //  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  node v = terminal, predV = NULL_NODE;

  while (v != u) {
    if (isCNode(v)) {
      v = activeCNodeOf(false, v);
      node jl, jr;

      if (testCNodeCounter(sG, v, w, predV, NULL_NODE, jl, jr)) {
        if (embed)
          obstructionEdgesCNodeCounter(sG, v, w, jl, jr, terminal, NULL_NODE);

        return true;
      }

      if (cNodeOfPossibleK33Obstruction != NULL_NODE)
        break;

      v = parent.get(v.id);

      if (v == u)
        break;
    }

    predV = v; // never a c-node;
    v = parent.get(v.id);
  }

  return false;
}

//=================================================================
BmdLink<node>*  PlanarityTestImpl::searchRBC(int dir, BmdLink<node>* it, node n,
    list<node>& traversedNodesInRBC) {
  if (it != NULL && (it->prev() == NULL || it->succ() == NULL)) // 1st or last item in RBC;
    return it;

  BmdLink<node>* prev = it;
  BmdLink<node>* aux1 = it->prev();
  BmdLink<node>* aux2 = it->succ();

  if (dir == 1) {
    //leda_swap(aux1, aux2);
    BmdLink<node>*  tmp = aux1;
    aux1 = aux2;
    aux2 = tmp;
  }

  it = aux1;
  node u = it->getData();
  int b = labelB.get(u.id);

  while (it != NULL && (b <= dfsPosNum.get(n.id) || dir != 1) && state.get(u.id) == NOT_VISITED) {
    aux1 = it->prev();

    if (aux1 == prev)
      aux1 = it->succ();

    prev = it;
    it = aux1;
    u = prev->getData();
    state.set(u.id, VISITED_IN_RBC);
    traversedNodesInRBC.push_back(u);

    if (it != NULL) {
      u = it->getData();
      b = labelB.get(u.id);
    }
  }

  if (it == NULL)
    return prev;
  else if (state.get(u.id) != NOT_VISITED || (it->prev() == NULL || it->succ() == NULL))
    return it;
  else
    return NULL;
}
//=================================================================
/*
 * Returns the active c-node of RBC list containing "u".
 * Traverses only nodes with label_b <= dfspos_num[w].
 * Let z be the first element of RBC.
 * All traversed nodes, but z, are inserted in "nl".
 * Pre-conditions:
 * - "u" is not the 1st element in RBC list;
 * - ptritem[u] is the pointer to the item containing "u";
 * - "u" is a node that is an ancestor of a neighbor of w;
 * - state[v] == NOT_VISITED for each node v in RBC list that was not
 *   traversed.
 */
node PlanarityTestImpl::findActiveCNode(node u, node w, list<node>& nl) {
  //  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  list<node> traversedNodesInRBC;
  assert(isCNode(parent.get(u.id)));

  if (state.get(u.id) != NOT_VISITED) {
    assert(!isCNode(parent.get(parent.get(u.id).id)));
    return parent.get(u.id);
  }

  BmdLink<node>*  it1 = ptrItem.get(u.id);//ptrItem[u];
  assert(it1 != NULL);

  state.set(u.id, VISITED_IN_RBC);
  traversedNodesInRBC.push_back(u);
  BmdLink<node>*  it = searchRBC(1, it1, w, traversedNodesInRBC);

  if (it == NULL)  it = searchRBC(0, it1, w, traversedNodesInRBC);

  assert(it != NULL);

  node v = it->getData();
  node cNode;

  if (it->prev() != 0 && it->succ() != 0)
    cNode = parent.get(v.id); // path compressed;
  else
    cNode = activeCNode[it];

  assert(cNode != NULL_NODE);

  node first = RBC[cNode].firstItem()->getData();

  // path compression;
  //forall(v, traversedNodesInRBC)
  for (list<node>::iterator it = traversedNodesInRBC.begin(); it != traversedNodesInRBC.end(); ++it) {
    if (*it != first) {
      if (*it != u)
        nl.push_back(v);

      parent.set((*it).id, cNode);
    }
    else state.set((*it).id, NOT_VISITED);
  }

  return cNode;
}
//=================================================================
