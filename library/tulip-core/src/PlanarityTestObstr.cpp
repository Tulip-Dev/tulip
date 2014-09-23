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

#include <tulip/PlanarityTestImpl.h>
#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

//=================================================================
/*
 * Adds to "el" the edges in path from w1 to w2, upward in T0.
 */
bool PlanarityTestImpl::listEdgesUpwardT0(node n1, node n2) {
  if (n1 == n2)
    return true;

  node u = n1;

  while (u != NULL_NODE && u != n2) {
    edge e = T0EdgeIn.get(u.id);
    obstructionEdges.push_back(e);
    u = p0.get(u.id);
  }

  return (u == n2);
}
//=================================================================
/*
 * Adds boundary cycle of "cnode" to "list_edges".
 * Precondition:
 * - embed_list[cnode] is an embedding of the 2-connected component represented
 *   by "cnode" with "RBC[cnode] contained in the boundary".
 */
void PlanarityTestImpl::extractBoundaryCycle(Graph *sG, node cNode,
    list<edge>& listEdges) {
  assert(embedList[cNode].size()!=0);
  map<node, list<edge> > el;
  BmdListIt<edge> it(embedList[cNode]);

  while (it.hasNext()) {
    edge e = it.next();
    node u = sG->source(e);
    el[u].push_back(e);
  }

  edge e;
  node u;
  u = parent.get(cNode.id);
  //  e = el[u].contents(el[u].first());
  e = el[u].front();
  listEdges.push_back(e);
  u = sG->target(e);
#ifndef NDEBUG
  unsigned int count = 0;
#endif

  while (u != parent.get(cNode.id)) {
    assert(++count <= numberOfNodesInG);
    e = el[u].front();
    listEdges.push_back(e);
    u = sG->target(e);
  }
}
//=================================================================
void PlanarityTestImpl::obstrEdgesTerminal(Graph* G, node w, node t, node u) {
  (void)u; //fixes unused parameter warning in release builds
  node mm = lcaBetween(nodeLabelB.get(t.id), neighborWTerminal.get(t.id), p0);
  (void)mm;
  assert((listEdgesUpwardT0(nodeLabelB.get(t.id), mm)));
  assert(listEdgesUpwardT0(neighborWTerminal.get(t.id), mm));
  assert(listEdgesUpwardT0(mm, u));
  edge e = G->existEdge(nodeLabelB.get(t.id), nodeWithDfsPos.get(labelB.get(t.id)));
  assert(e.isValid());
  obstructionEdges.push_back(e);
  e = G->existEdge(neighborWTerminal.get(t.id), w);
  assert(e.isValid());
  obstructionEdges.push_back(e);
}
//=================================================================
/*
 * Adds part of the boundary cycle of "cnode" that contains w1 and doesn't contain
 * path between w2 and w3 to "el".
 */
void PlanarityTestImpl::addPartOfBc(Graph *sG, node cNode,
                                    node n1, node n2, node n3) {
  list<edge> bc, el1, el2;
  extractBoundaryCycle(sG, cNode, bc);
  edge e;
  bool flag = false;
  int side = 0;

  for (list<edge>::const_iterator it = bc.begin() ; it != bc.end() ; ++it) {
    node source = sG->source(*it);

    if (source == n1) {
      if (flag)
        side = 1;
      else
        side = 2;
    }

    if (source == n2 || source == n3)
      flag = !flag;

    if (flag)
      el1.push_back(e);
    else
      el2.push_back(e);
  }

  assert(side!=0);

  if (side == 1) {
    el1.splice(el1.begin(), obstructionEdges);
    obstructionEdges = el1;
  }
  else {
    el2.splice(el2.begin(), obstructionEdges);
    obstructionEdges = el2;
  }
}
//=================================================================
void PlanarityTestImpl::sortByLabelB(node &n1, node &n2, node &n3) {
  if (labelB.get(n2.id) < labelB.get(n1.id))
    swapNode(n1, n2);

  if (labelB.get(n3.id) < labelB.get(n1.id))
    swapNode(n1, n3);

  if (labelB.get(n3.id) < labelB.get(n2.id))
    swapNode(n2, n3);
}
//=================================================================
void PlanarityTestImpl::obstrEdgesPNode(Graph *sG, node p, node u) {
  (void)u;//fixes unused aprameter in release builds
  assert(listEdgesUpwardT0(nodeLabelB.get(p.id), u));
  edge e = sG->existEdge(nodeLabelB.get(p.id), nodeWithDfsPos.get(labelB.get(p.id)));
  assert(e.isValid());
  obstructionEdges.push_back(e);
}
//=================================================================
void PlanarityTestImpl::calcInfo3Terminals(node &t1, node &t2, node &t3,
    int &countMin, int &countF,
    node &cNode, node &q) {
  countMin = countF = 0;
  int min = labelB.get(t1.id);
  min = std::min(min, labelB.get(t2.id));
  min = std::min(min, labelB.get(t3.id));

  if (min == labelB.get(t1.id)) countMin++;

  if (min == labelB.get(t2.id)) countMin++;

  if (min == labelB.get(t3.id)) countMin++;

  cNode = q = NULL_NODE;
  node w1 = t1, w2 = t2, w3 = t3;

  if (isCNode(t1))
    w1 = parent.get(t1.id);

  if (isCNode(t2))
    w2 = parent.get(t2.id);

  if (isCNode(t3))
    w3 = parent.get(t3.id);

  assert(!(isCNode(w1) || isCNode(w2) || isCNode(w3)));

  node m1 = lcaBetween(w1, w2, parent);
  node m2 = lcaBetween(w1, w3, parent);
  node m3 = lcaBetween(w2, w3, parent);

  if (isCNode(m1)) m1 = activeCNodeOf(true, m1);

  if (isCNode(m2)) m2 = activeCNodeOf(true, m2);

  if (isCNode(m3)) m3 = activeCNodeOf(true, m3);

  if (isCNode(m1) && m1 == m2 && m2 == m3) {
    cNode = m1;
    node u1 = lastPNode(w1, cNode);
    node u2 = lastPNode(w2, cNode);
    node u3 = lastPNode(w3, cNode);

    if (u1 == t1) countF++;

    if (u2 == t2) countF++;

    if (u3 == t3) countF++;
  }

  if (countF == 3)
    return;
  else
    cNode = NULL_NODE;

  node q1 = lcaBetween(w1, w2, p0);
  node q2 = lcaBetween(w1, w3, p0);
  node q3 = lcaBetween(w2, w3, p0);
  node k1 = t1, k2 = t2, k3 = t3;
  min = dfsPosNum.get(q1.id);

  if (dfsPosNum.get(q2.id) < min) {
    swapNode(k2, k3);
    min = dfsPosNum.get(q2.id);
  }

  if (dfsPosNum.get(q3.id) < min) {
    k1 = t3;
    k2 = t2;
    k3 = t1;
    min = dfsPosNum.get(q3.id);
  }

  // is min a terminal node?
  if (min == dfsPosNum.get(t1.id) || min == dfsPosNum.get(t2.id) || min == dfsPosNum.get(t3.id)) {
    if (dfsPosNum.get(k2.id) == min) swapNode(k1, k2);

    if (dfsPosNum.get(k3.id) == min) swapNode(k1, k3);

    assert(dfsPosNum.get(k1.id)==min);
    cNode = activeCNodeOf(true, k1); // dfspos_num[k1] == min;
    int max = dfsPosNum.get(q1.id);
    max = std::max(max, dfsPosNum.get(q2.id));
    max = std::max(max, dfsPosNum.get(q3.id));

    if (max >= dfsPosNum.get(parent.get(cNode.id).id))
      q = parent.get(cNode.id);
    else if (activeCNodeOf(true, k2) != cNode) {
      q = lastPNode(k2, cNode);
      swapNode(k2, k3);
    }
    else
      q = lastPNode(k3, cNode);

    t1 = k1;
    t2 = k2;
    t3 = k3;
    assert(cNode != NULL_NODE);
    return;
  }
}
//=================================================================
/*
 * K33 -- T0;
 * Pre-conditions:
 * - t1, t2 and t3 are terminal nodes;
 * - if parent_cnode != nil then t3 == nil.
 */
void PlanarityTestImpl::obstructionEdgesT0(Graph *sG, node w, node t1,
    node t2, node t3, node v) {
  if (t3 == NULL_NODE) t3 = v;

  node w1 = t1, w2 = t2, w3 = t3;
  sortByLabelB(w1, w2, w3);
  assert (listEdgesUpwardT0(nodeWithDfsPos.get(labelB.get(w1.id)), nodeWithDfsPos.get(labelB.get(w3.id))));

  w1 = t1;
  w2 = t2;
  w3 = t3;

  if (isCNode(w1)) w1 = parent.get(w1.id);

  if (isCNode(w2)) w2 = parent.get(w2.id);

  if (isCNode(w3)) w3 = parent.get(w3.id);

  node m1 = lcaBetween(w1, w2, p0);
  node m2 = lcaBetween(w1, w3, p0);
  node m3 = lcaBetween(w2, w3, p0);
  int min = dfsPosNum.get(m1.id), max = dfsPosNum.get(m1.id);

  if (dfsPosNum.get(m2.id) < min) {
    swapNode(t2, t3);
    min = dfsPosNum.get(m2.id);
  }

  if (dfsPosNum.get(m3.id) < min) {
    node k1 = t2, k2 = t3, k3 = t1;
    t1 = k1;
    t2 = k2;
    t3 = k3;
    min = dfsPosNum.get(m3.id);
  }

  //  if (dfsPosNum.get(m2.id) > max) max = dfsPosNum.get(m2.id);
  //  if (dfsPosNum.get(m3.id) > max) max = dfsPosNum.get(m3.id);

  max = std::max(max, dfsPosNum.get(m2.id));
  max = std::max(max, dfsPosNum.get(m3.id));

  obstrEdgesTerminal(sG, w, t1, nodeWithDfsPos.get(max));

  if (t3 != v)
    obstrEdgesTerminal(sG, w, t3, nodeWithDfsPos.get(max));
  else {
    node u = w;

    if (lcaBetween(v, nodeWithDfsPos.get(min), p0) == nodeWithDfsPos.get(min)) {
      // v is in the same c-node of node_with_dfspos[min];
      node cNode = activeCNodeOf(true, v);
      addPartOfBc(sG, cNode, parent.get(cNode.id), v, nodeWithDfsPos.get(min));
      u = v; //last_pnode(node_label_b[v], cnode);
      assert(listEdgesUpwardT0(parent.get(cNode.id), w));
    }

    obstrEdgesPNode(sG, v, u);
  }

  obstrEdgesTerminal(sG, w, t2, nodeWithDfsPos.get(min));
}
// //=================================================================
// /*
//  * Pre-conditions:
//  * - t1, t2 and t3 are terminal nodes;
//  * - ti is p-node whose parent is cnode;
//  * - if t3 == nil, then parent[cnode] has a descendant v s.t.
//  *   label_b[v] > dfspos_num[w] and v is not a descendant of cnode;
//  * - cnode is part of a K_{3,3} obstruction.
//  */
void PlanarityTestImpl::obstructionEdgesCountMin1(Graph *sG, node n, node cNode,
    node t1, node t2, node t3) {
  if (t3 == NULL_NODE) t3 = parent.get(cNode.id);

  sortByLabelB(t1, t2, t3);
  assert(listEdgesUpwardT0(n, nodeWithDfsPos.get(labelB.get(t3.id))));
  addPartOfBc(sG, cNode, t1, t2, t3);
  assert(listEdgesUpwardT0(nodeLabelB.get(t1.id), t1));
  edge e = sG->existEdge(nodeLabelB.get(t1.id), nodeWithDfsPos.get(labelB.get(t1.id)));
  assert(e.isValid());
  obstructionEdges.push_back(e);

  if (t2 != parent.get(cNode.id))
    obstrEdgesTerminal(sG, n, t2, t2);
  else
    obstrEdgesPNode(sG, parent.get(cNode.id), n);

  if (t3 != parent.get(cNode.id))
    obstrEdgesTerminal(sG, n, t3, t3);
  else
    obstrEdgesPNode(sG, parent.get(cNode.id), n);
}
// //=================================================================
// /*
//  * Pre-conditions:
//  * - t1, t2 and t3 are terminal nodes;
//  * - t1 is terminal p-node,
//  *   t1 = min{lca_in_T0(t1,t2,),lca_in_T0(t1,t3),lca_in_T0(t2,t3)}
//  *   and t1 is a "child" of cnode;
//  * - if t3 == nil, then v is a descendant in T of a node in path from
//  *   parent[parent[cnode]] to a child of w s.t. label_b[v] > dfspos_num[w];
//  *   label_b[v] > dfspos_num[w] and v is not a descendant of cnode;
//  * - cnode is part of a K_{3,3} obstruction.
//  */
void PlanarityTestImpl::obstructionEdgesCountMin23(Graph *sG,
    node n,
    node cNode,
    node t1,
    node t2,
    node t3,
    node q,
    node v) {
  node n1 = t1, n2 = t2, n3 = t3;

  if (t3 == NULL_NODE)
    n3 = v;

  sortByLabelB(n1, n2, n3);

  assert(listEdgesUpwardT0(nodeWithDfsPos.get(labelB.get(n1.id)), nodeWithDfsPos.get(labelB.get(n3.id))));

  n2 = lastPNode(t2, cNode);

  if (n2 != q)
    addPartOfBc(sG, cNode, q, t1, n2);
  else
    addPartOfBc(sG, cNode, q, t1, parent.get(cNode.id));

  obstrEdgesTerminal(sG, n, t1, t1);
  obstrEdgesTerminal(sG, n, t2, n2);

  if (t3 != NULL_NODE) {
    node u = lcaBetween(nodeLabelB.get(t3.id), q, p0);
    // "u" can be an ancestor of q when t3 is not descendant of cNode;
    obstrEdgesTerminal(sG, n, t3, u);
    assert(listEdgesUpwardT0(q, u));
  }
  else {
    obstrEdgesPNode(sG, v, n);
    node mm = lcaBetween(v, parent.get(cNode.id), p0);
    (void)mm;
    assert(listEdgesUpwardT0(parent.get(cNode.id), mm));
  }
}
// //=================================================================
// void PlanarityTestImpl::obstrEdgesTermCNode(Graph *sG, node w, node t) {
//   node u = lastPNode(nodeLabelB.get(t.id), t);
//   node uu = lastPNode(neighborWTerminal.get(t.id), t);
//   assert(listEdgesUpwardT0(nodeLabelB.get(t.id), u));
//   assert(listEdgesUpwardT0(neighborWTerminal.get(t.id), uu));

//   addPartOfBc(sG, t, parent.get(t.id), u, uu);
//   edge e = sG->existEdge(nodeLabelB.get(t.id), nodeWithDfsPos.get(labelB.get(t.id)));
//   assert(e.isValid());
//   obstructionEdges.push_back(e);
//   e = sG->existEdge(neighborWTerminal.get(t.id), w);
//   assert(e.isValid());
//   obstructionEdges.push_back(e);
// }
// //=================================================================
// /*
//  * Pre-conditions:
//  * - t1, t2 and t3 are (p-nodes) terminals in the same c-node;
//  * - cnode is part of a K_5 obstruction.
// */
void PlanarityTestImpl::obstructionEdgesK5(Graph *sG,
    node w,
    node cNode,
    node t1,
    node t2,
    node t3) {
  if (t3 == NULL_NODE)
    t3 = parent.get(cNode.id);

  int max = labelB.get(t1.id);
  max = std::max(max, labelB.get(t2.id));
  max = std::max(max, labelB.get(t3.id));
  //  if (max < labelB.get(t2.id))
  //    max = labelB.get(t2.id);
  //  if (max < labelB.get(t3.id))
  //    max = labelB.get(t3.id);
  assert(listEdgesUpwardT0(w, nodeWithDfsPos.get(max)));
  extractBoundaryCycle(sG, cNode, obstructionEdges);
  obstrEdgesTerminal(sG, w, t1, t1);
  obstrEdgesTerminal(sG, w, t2, t2);

  if (t3 != parent.get(cNode.id))
    obstrEdgesTerminal(sG, w, t3, t3);
  else
    obstrEdgesPNode(sG, parent.get(cNode.id), w);
}
// //=================================================================
void PlanarityTestImpl::obstructionEdgesPossibleObstrConfirmed(Graph *sG,
    node w,
    node t,
    node v) {
  node cNode = cNodeOfPossibleK33Obstruction;
  node f = obstructionNodes.front();
  obstructionNodes.pop_front();
  node jl = obstructionNodes.front();
  obstructionNodes.pop_front();
  node jr = obstructionNodes.front();
  obstructionNodes.pop_front();

  if (labelB.get(jl.id) > dfsPosNum.get(w.id))
    nodeLabelB.set(jl.id, neighborWTerminal.get(t.id));

  if (labelB.get(jr.id) > dfsPosNum.get(w.id))
    nodeLabelB.set(jr.id, neighborWTerminal.get(t.id));

  node w1 = nodeWithDfsPos.get(labelB.get(v.id));
  node w2 = nodeWithDfsPos.get(labelB.get(f.id));

  if (dfsPosNum.get(w2.id) < dfsPosNum.get(w1.id))
    swapNode(w1, w2);

  assert(listEdgesUpwardT0(w, w2));
  assert(listEdgesUpwardT0(nodeLabelB.get(f.id), f));
  assert(listEdgesUpwardT0(nodeLabelB.get(jl.id), jl));
  assert(listEdgesUpwardT0(nodeLabelB.get(jr.id), jr));

  node m = lcaBetween(parent.get(cNode.id), v, p0);
  (void)m;

  assert(listEdgesUpwardT0(parent.get(cNode.id), m));
  assert(listEdgesUpwardT0(nodeLabelB.get(v.id), m));

  edge e = sG->existEdge( nodeLabelB.get(v.id), nodeWithDfsPos.get(labelB.get(v.id)));
  assert( e.isValid());

  obstructionEdges.push_back(e);
  e = sG->existEdge(nodeLabelB.get(f.id), nodeWithDfsPos.get(labelB.get(f.id)));
  assert( e.isValid());

  obstructionEdges.push_back(e);
  e = sG->existEdge(nodeLabelB.get(jl.id), w);
  assert(e.isValid());

  obstructionEdges.push_back(e);
  e = sG->existEdge(nodeLabelB.get(jr.id), w);
  assert( e.isValid());

  obstructionEdges.push_back(e);

  extractBoundaryCycle(sG, cNode, obstructionEdges);
}
//=================================================================
/*
 * K33
 * Pre-conditions:
 * - for i = 1, 2, if t_i is not nil, then it is a terminal node that is descendant
 * of cnode.
 */
void PlanarityTestImpl::obstructionEdgesCNodeCounter(Graph *sG, node cNode, node w,
    node jl, node jr, node t1, node t2) {
  //  tlp::debug() << __PRETTY_FUNCTION__ << endl;
  // seachs for a node f in RBC[cNode] between jl and jr s.t.
  // f has a descendant that is a neighbor of w in G;
  bool flag = false;
  node f = NULL_NODE, t12 = NULL_NODE, t22 = NULL_NODE;

  if (t1 != NULL_NODE && t1 != cNode)
    t12 = lastPNode(t1, cNode);

  if (t2 != NULL_NODE && t2 != cNode)
    t22 = lastPNode(t2, cNode);

  BmdListIt<node> it(RBC[cNode]);

  while (it.hasNext()) {
    node v = it.next();

    if (v == jl || v == jr)
      flag = true;
    else if (flag && (v == t12 || v == t22 || labelB.get(v.id) == dfsPosNum.get(w.id))) {
      f = v;
      break;
    }
  }

  if (f == t12)
    nodeLabelB.set(f.id, neighborWTerminal.get(t1.id));
  else if (f == t22)
    nodeLabelB.set(f.id, neighborWTerminal.get(t2.id));

  //#ifdef _DEBUG_MODE_
  // if (f == NULL_NODE) {
  //     tlp::debug() << "-> Program terminated! (obstruction_edges_cnode_counter-01)";
  //     tlp::debug()<<"obstruction_edges_cnode_counter:\n";
  //     tlp::debug()<<"  cnode="<<dfsPosNum.get(cNode.id)<<",f="<<dfsPosNum.get(f.id);
  //     tlp::debug()<<"\n  jl="<<dfsPosNum.get(jl.id)<<",b="<<labelB.get(jl.id)<<",nl="<<dfsPosNum.get(nodeLabelB.get(jl.id).id);
  //     tlp::debug()<<"\n  jr="<<dfsPosNum.get(jr.id)<<",b="<<labelB.get(jr.id)<<",nl="<<dfsPosNum.get(nodeLabelB.get(jr.id).id);
  //     tlp::debug()<<"\n";
  //   }
  //#endif

  node w1 = nodeWithDfsPos.get(labelB.get(jl.id));
  node w2 = nodeWithDfsPos.get(labelB.get(jr.id));

  if (dfsPosNum.get(w2.id) < dfsPosNum.get(w1.id))
    swapNode(w1, w2);

  assert(listEdgesUpwardT0(parent.get(cNode.id), w2));
  assert(listEdgesUpwardT0(nodeLabelB.get(f.id), f));
  assert(listEdgesUpwardT0(nodeLabelB.get(jl.id), jl));
  assert(listEdgesUpwardT0(nodeLabelB.get(jr.id), jr));
  edge e = sG->existEdge(nodeLabelB.get(f.id), w);
  assert(e.isValid());
  obstructionEdges.push_back(e);
  e = sG->existEdge(nodeLabelB.get(jl.id), nodeWithDfsPos.get(labelB.get(jl.id)));
  assert(e.isValid());
  obstructionEdges.push_back(e);
  e = sG->existEdge(nodeLabelB.get(jr.id), nodeWithDfsPos.get(labelB.get(jr.id)));
  assert(e.isValid());
  obstructionEdges.push_back(e);
  extractBoundaryCycle(sG, cNode, obstructionEdges);
}
//=================================================================
