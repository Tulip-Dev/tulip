/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include "Dijkstra.h"
#include <tulip/LayoutProperty.h>
#include <tulip/ParallelTools.h>

using namespace tlp;
using namespace std;

NodeProperty<node> Dijkstra::ndik2tlp;
EdgeProperty<edge> Dijkstra::edik2tlp;
MutableContainer<node> Dijkstra::ntlp2dik;
MutableContainer<edge> Dijkstra::etlp2dik;
VectorGraph Dijkstra::graph;
bool Dijkstra::_initB = Dijkstra::initG();

// define the lock needed to protect Dijkstra()/~Dijkstra()
TLP_DEFINE_GLOBAL_LOCK(DijkstraProps);

//============================================================
void Dijkstra::initDijkstra(const tlp::Graph *const forbidden, tlp::node srcTlp,
                            const EdgeStaticProperty<double> &weights, const set<node> &fous) {

  assert(srcTlp.isValid());
  src = ntlp2dik.get(srcTlp);

  forbiddenNodes.setAll(false);

  if (forbidden) {
    for (auto n : forbidden->nodes()) {
      node ndik = ntlp2dik.get(n);
      forbiddenNodes[ndik] = true;
    }
  }

  usedEdges.setAll(false);

  set<DijkstraElement *, LessDijkstraElement> dijkstraTable;
  set<DijkstraElement *, LessDijkstraElement> focusTable;

  mapDik.setAll(0);
  vector<bool> focus(graph.numberOfNodes(), false);

  for (auto n : fous)
    focus[ntlp2dik.get(n)] = true;

  for (auto n : graph.nodes()) {
    if (n != src) { // init all nodes to +inf
      DijkstraElement *tmp = new DijkstraElement(DBL_MAX / 2. + 10., node(), n);
      dijkstraTable.insert(tmp);

      if (focus[n])
        focusTable.insert(tmp);

      mapDik[n] = tmp;
    } else { // init starting node to 0
      DijkstraElement *tmp = new DijkstraElement(0, n, n);
      dijkstraTable.insert(tmp);
      mapDik[n] = tmp;
    }
  }

  nodeDistance.setAll(DBL_MAX);
  nodeDistance[src] = 0;

  while (!dijkstraTable.empty()) {
    // select the first element in the list the one with min value
    set<DijkstraElement *, LessDijkstraElement>::iterator it = dijkstraTable.begin();
    DijkstraElement &u = *(*it);
    dijkstraTable.erase(it);

    if (!focusTable.empty()) {
      set<DijkstraElement *, LessDijkstraElement>::reverse_iterator it = focusTable.rbegin();
      double maxDist = (*it)->dist;

      if (u.dist > maxDist) {
        break;
      }
    }

    node n = u.n;
    if (forbiddenNodes[n] && n != src)
      continue;

    for (auto &adj : graph.adj(n)) {
      edge e = adj.link();
      DijkstraElement &dEle = *mapDik[adj.opposite()];

      auto eWeight = weights.getEdgeValue(edik2tlp[e]);
      if (fabs((u.dist + eWeight) - dEle.dist) < 1E-9) // path of the same length
        dEle.usedEdge.push_back(e);
      else if ((u.dist + eWeight) < dEle.dist) {
        // we find a node closer with that path
        dEle.usedEdge.clear();
        //**********************************************
        dijkstraTable.erase(&dEle);

        if (focus[dEle.n]) {
          focusTable.erase(&dEle);
        }

        dEle.dist = u.dist + eWeight;
        dEle.previous = n;
        dEle.usedEdge.push_back(e);
        dijkstraTable.insert(&dEle);

        if (focus[dEle.n]) {
          focusTable.insert(&dEle);
        }
      }
    }
  }

  for (auto n : graph.nodes()) {
    DijkstraElement *dEle = mapDik[n];
    nodeDistance[n] = dEle->dist;

    for (size_t i = 0; i < dEle->usedEdge.size(); ++i) {
      usedEdges[dEle->usedEdge[i]] = true;
    }

    delete dEle;
  }

  resultNodes.setAll(false);
  resultEdges.setAll(false);
}
//=======================================================================
void Dijkstra::searchPaths(node ntlp, EdgeStaticProperty<unsigned int> &depth) {

  node n = ntlp2dik.get(ntlp);

  if (resultNodes[n])
    return;

  resultNodes[n] = true;

  for (auto &adj : graph.adj(n)) {
    edge e = adj.link();
    if (!usedEdges[e] || resultEdges[e])
      continue;

    node tgt = adj.opposite();

    if (nodeDistance[tgt] >= nodeDistance[n])
      continue;

    resultEdges[e] = true;
    auto ePos = depth.getGraph()->edgePos(edik2tlp[e]);
    depth[ePos] += 1;

    if (!resultNodes[tgt])
      searchPaths(ndik2tlp[tgt], depth);
  }
}
//=============================================================================
void Dijkstra::searchPath(node ntlp, vector<node> &vNodes) {

  node n = ntlp2dik.get(ntlp);
  node tgte(ntlp);
  resultEdges.setAll(false);
  bool ok = true;

  while (ok) {
    vNodes.push_back(ndik2tlp[n]);
    ok = false;

    for (auto &adj : graph.adj(n)) {
      edge e = adj.link();

      // check if that edge does not belong to the shortest path edges
      // or if it has already been treated
      if (!usedEdges[e] || resultEdges[e])
        continue;

      node tgt = adj.opposite();

      if (nodeDistance[tgt] >= nodeDistance[n])
        continue;

      n = tgt;
      resultEdges[e] = ok = true;
      break;
    }
  }

  if (n != src)
    cout << "A path does not exist between node " << src.id << " and node " << tgte.id << "!"
         << endl;
}
