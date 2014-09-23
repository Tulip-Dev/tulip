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

#include "Dijkstra.h"
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>

using namespace tlp;
using namespace std;

NodeProperty<node> Dijkstra::ndik2tlp;
EdgeProperty<edge> Dijkstra::edik2tlp;
MutableContainer<node> Dijkstra::ntlp2dik;
MutableContainer<edge> Dijkstra::etlp2dik;
VectorGraph Dijkstra::graph;
bool Dijkstra::_initB = Dijkstra::initG();

//============================================================
void Dijkstra::initDijkstra(
  const tlp::Graph * const forbidden,
  tlp::node srcTlp,
  const tlp::MutableContainer<double> &weights,
  const set<node> &fous
) {

  assert(srcTlp.isValid());
  src = ntlp2dik.get(srcTlp);
  node n;

  forbiddenNodes.setAll(false);

  if (forbidden) {
    forEach(n, forbidden->getNodes()) {
      node ndik = ntlp2dik.get(n);
      forbiddenNodes[ndik] = true;
    }
  }

  usedEdges.setAll(false);

  set<DijkstraElement *, LessDijkstraElement> dikjstraTable;
  set<DijkstraElement *, LessDijkstraElement> focusTable;

  mapDik.setAll(0);
  vector<bool> focus(graph.numberOfNodes(), false);
  set<node>::const_iterator it = fous.begin();

  for (; it!= fous.end(); ++it)
    focus[ntlp2dik.get(*it)] = true;


  const vector<node> &bNodes = graph.nodes();

  for (size_t i = 0 ; i < bNodes.size() ; ++i) {
    n = bNodes[i];

    if (n != src) { //init all nodes to +inf
      DijkstraElement *tmp = new DijkstraElement(DBL_MAX / 2. + 10., node(), n);
      dikjstraTable.insert(tmp);

      if (focus[n])
        focusTable.insert(tmp);

      mapDik[n] = tmp;
    }
    else { //init starting node to 0
      DijkstraElement * tmp = new DijkstraElement(0, n, n);
      dikjstraTable.insert(tmp);
      mapDik[n] = tmp;
    }
  }

  nodeDistance.setAll(DBL_MAX);
  nodeDistance[src] = 0;

  while (!dikjstraTable.empty()) {
    //select the first element in the list the one with min value
    set<DijkstraElement *, LessDijkstraElement>::iterator it = dikjstraTable.begin();
    DijkstraElement &u = *(*it);
    dikjstraTable.erase(it);

    if (!focusTable.empty()) {
      set<DijkstraElement *, LessDijkstraElement>::reverse_iterator it = focusTable.rbegin();
      double maxDist = (*it)->dist;

      if (u.dist > maxDist) {
        break;
      }
    }


    if (forbiddenNodes[u.n] && u.n != src)
      continue;


    edge e;
    const vector<edge> &adjEdges = graph.star(u.n);

    for (size_t i = 0 ; i < adjEdges.size() ; ++i) {
      e = adjEdges[i];
      node v = graph.opposite(e, u.n);
      DijkstraElement & dEle = *mapDik[v];
      //assert(weights.get(edik2tlp[e]) > 0);



      if ( fabs((u.dist + weights.get(edik2tlp[e])) - dEle.dist) < 1E-9) //path of the same length
        dEle.usedEdge.push_back(e);
      else

        //we find a node closer with that path
        if ( (u.dist + weights.get(edik2tlp[e])) < dEle.dist) {
          dEle.usedEdge.clear();
          //**********************************************
          dikjstraTable.erase(&dEle);

          if (focus[dEle.n]) {
            focusTable.erase(&dEle);
          }

          dEle.dist = u.dist + weights.get(edik2tlp[e]);
          dEle.previous = u.n;
          dEle.usedEdge.push_back(e);
          dikjstraTable.insert(&dEle);

          if (focus[dEle.n]) {
            focusTable.insert(&dEle);
          }
        }
    }
  }


  node tmpN;

  for (size_t i = 0 ; i < bNodes.size() ; ++i) {
    tmpN = bNodes[i];
    DijkstraElement *dEle = mapDik[tmpN];
    nodeDistance[tmpN.id] = dEle->dist;

    for (size_t i=0; i < dEle->usedEdge.size(); ++i) {
      usedEdges[dEle->usedEdge[i]] = true;
    }

    delete dEle;
  }


  resultNodes.setAll(false);
  resultEdges.setAll(false);

}
//=======================================================================
void Dijkstra::searchPaths(node ntlp, DoubleProperty *depth) {

  node n = ntlp2dik.get(ntlp);

  if (resultNodes[n]) return;

  resultNodes[n] = true;
  edge e;
  const vector<edge> &adjEdges = graph.star(n);

  for (size_t i = 0 ; i < adjEdges.size() ; ++i) {
    e = adjEdges[i];

    if (!usedEdges[e]) continue;

    if (resultEdges[e]) continue;

    node tgt = graph.opposite(e, n);

    if (nodeDistance[tgt] >= nodeDistance[n]) continue;

    resultEdges[e] = true;
    double dep = depth->getEdgeValue(edik2tlp[e]) + 1.;
#ifdef _OPENMP
    #pragma omp critical(DEPTH)
#endif
    depth->setEdgeValue(edik2tlp[e], dep);
    searchPaths(ndik2tlp[tgt], depth);
  }
}
//=============================================================================
void Dijkstra::searchPath(node ntlp, vector<node> &vNodes) {

  node n = ntlp2dik.get(ntlp);
  node tgte(ntlp);
  resultNodes.setAll(false);
  resultEdges.setAll(false);
  bool ok = true;

  while (ok) {

    resultNodes[n] = true;
    vNodes.push_back(ndik2tlp[n]);
    ok = false;
    edge edik;
    edge e;
    bool findEdge = false;
    const vector<edge> &adjEdges = graph.star(n);

    for (size_t i = 0 ; i < adjEdges.size() ; ++i) {
      edik = adjEdges[i];
      e = edik2tlp[edik];

      if (!usedEdges[edik])  continue; //that edge do not belongs to the shortest path edges

      if (resultEdges[edik]) continue; //that edge has already been treated

      node tgt = graph.opposite(edik, n);

      if (nodeDistance[tgt] >= nodeDistance[n]) continue;

      findEdge = true;
      break;
    }

    if (findEdge) {
      ok = true;
      n = graph.opposite(edik, n);//validEdge.begin()->first;
      resultEdges[edik] =  true;
    }
  }

  if (n != src) cout << "Path do not exist between node "<< src.id << " and node " << tgte.id << " !" << endl;
}
