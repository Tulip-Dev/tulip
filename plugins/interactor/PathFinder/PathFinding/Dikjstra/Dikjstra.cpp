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

#include "Dikjstra.h"
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ForEach.h>

using namespace tlp;
using namespace std;

//============================================================
void Dikjstra::initDikjstra(const tlp::Graph * const graph,
                            const tlp::Graph * const forbiddenNodes,
                            tlp::node src,
                            EdgeOrientation directed,
                            const tlp::MutableContainer<double> &weights,
                            double,
                            const set<node> &focus
                           ) {
  assert(src.isValid());
  this->graph = graph;
  this->forbiddenNodes = forbiddenNodes;
  usedEdges.setAll(false);
  this->src = src;
  set<DikjstraElement *, LessDikjstraElement> dikjstraTable;
  set<DikjstraElement *, LessDikjstraElement> focusTable;
  MutableContainer<DikjstraElement *> mapDik;
  mapDik.setAll(0);

  node n;
  forEach (n, graph->getNodes()) {
    if (n != src) { //init all nodes to +inf
      DikjstraElement *tmp = new DikjstraElement(DBL_MAX / 2. + 10., node(), n);
      dikjstraTable.insert(tmp);

      if (focus.find(n) != focus.end())
        focusTable.insert(tmp);

      mapDik.set(n.id, tmp);
    }
    else { //init starting node to 0
      DikjstraElement * tmp = new DikjstraElement(0, n, n);
      dikjstraTable.insert(tmp);
      mapDik.set(n.id, tmp);
    }
  }

  nodeDistance.setAll(DBL_MAX);
  nodeDistance.set(src.id, 0);

  while (!dikjstraTable.empty()) {
    //select the first element in the list the one with min value
    set<DikjstraElement *, LessDikjstraElement>::iterator it = dikjstraTable.begin();
    DikjstraElement &u = *(*it);
    dikjstraTable.erase(it);

    if (!focusTable.empty()) {
      set<DikjstraElement *, LessDikjstraElement>::reverse_iterator it = focusTable.rbegin();
      //set<DikjstraElement *>::iterator it2 = focusTable.begin();
      double maxDist = (*it)->dist;

      if (u.dist > maxDist) break;

      /*
       if (fabs(u.dist - maxDist) > 1E-9) {
          if (u.dist > maxDist) {
              assert(maxDist < (DBL_MAX/2.));
              break;
          }
      }
      */
      //if ((u.dist > maxDist) break;
    }

    if (forbiddenNodes != 0)
      if (forbiddenNodes->isElement(u.n) && u.n!=this->src)
        continue;

    edge e;

    Iterator<edge> *iter = 0;

    switch (directed) {
    case Oriented:
      iter = graph->getOutEdges(u.n);
      break;

    case NonOriented:
      iter = graph->getInOutEdges(u.n);
      break;

    case Reversed:
      iter = graph->getInEdges(u.n);
      break;
    }

    forEach(e, iter) {
      node v = graph->opposite(e, u.n);
      DikjstraElement & dEle = *mapDik.get(v.id);
      assert(weights.get(e.id) > 0);

      /*
                  if (DBL_MAX - u.dist <  weights.get(e.id))
              cerr << __PRETTY_FUNCTION__ << "at line : " << __LINE__ << " : Double overflow" << endl;
            if (DBL_MAX - fabs((u.dist + weights.get(e.id))) < dEle.dist)
              cerr << __PRETTY_FUNCTION__ << "at line : " << __LINE__ << " : Double overflow" << endl;
                    */
      if ( fabs((u.dist + weights.get(e.id)) - dEle.dist) < 1E-9) //path of the same length
        dEle.usedEdge.push_back(e);
      else

        //we find a node closer with that path
        if ( (u.dist + weights.get(e.id)) < dEle.dist) {
          dEle.usedEdge.clear();
          //**********************************************
          dikjstraTable.erase(&dEle);

          if (focus.find(dEle.n)!=focus.end()) {
            focusTable.erase(&dEle);
          }

          dEle.dist = u.dist + weights.get(e.id);
          /*
          if (DBL_MAX - u.dist <  weights.get(e.id))
          cerr << __PRETTY_FUNCTION__ << "at line : " << __LINE__ << " : Double overflow" << endl;
          */
          dEle.previous = u.n;
          dEle.usedEdge.push_back(e);
          dikjstraTable.insert(&dEle);

          if (focus.find(dEle.n)!=focus.end()) {
            focusTable.insert(&dEle);
          }
        }
    }
  }

  usedEdges.setAll(false);
  node tmpN;
  forEach(tmpN, graph->getNodes()) {
    DikjstraElement *dEle = mapDik.get(tmpN.id);
    nodeDistance.set(tmpN.id, dEle->dist);

    for (unsigned int i=0; i < dEle->usedEdge.size(); ++i) {
      usedEdges.set(dEle->usedEdge[i].id, true);
    }

    delete dEle;
  }
}
//=======================================================================
void Dikjstra::internalSearchPaths(node n, BooleanProperty *result, DoubleProperty *depth) {
  if (result->getNodeValue(n)) return;

  result->setNodeValue(n, true);
  edge e;
  forEach(e, graph->getInOutEdges(n)) {
    if (!usedEdges.get(e.id)) continue;

    if (result->getEdgeValue(e)) continue;

    node tgt = graph->opposite(e, n);

    if (nodeDistance.get(tgt.id) >= nodeDistance.get(n.id)) continue;

    result->setEdgeValue(e, true);
    double dep = depth->getEdgeValue(e) + 1.;
    depth->setEdgeValue(e, dep);
    searchPaths(tgt, result, depth);
  }
}
//=============================================================================
bool Dikjstra::searchPath(node n, BooleanProperty *result, vector<node> &vNodes, DoubleProperty *preference) {
  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);
  bool ok = true;

  while (ok) {
    result->setNodeValue(n, true);
    vNodes.push_back(n);
    ok = false;
    //set<edge> validEdge;
    map<double, edge> validEdge;
    edge e;
    forEach(e, graph->getInOutEdges(n)) {
      if (!usedEdges.get(e.id)) continue; //that edge do not belongs to the shortest path edges

      if (result->getEdgeValue(e)) continue; //that edge has already been treated

      node tgt = graph->opposite(e, n);

      if (nodeDistance.get(tgt.id) >= nodeDistance.get(n.id)) continue;

      validEdge[preference->getNodeValue(tgt)] = e ;
    }

    if (!validEdge.empty()) {
      ok = true;
      e = validEdge.rbegin()->second;
      n = graph->opposite(e, n);//validEdge.begin()->first;
      result->setEdgeValue(e, true);
    }
  }

  if (n != src) {
#ifndef NDEBUG
    cout << "Path do not exist !" << endl;
#endif /* NDEBUG */
    return false;
  }

  return true;
}
//========================================
bool Dikjstra::searchPaths(tlp::node n, tlp::BooleanProperty *result, tlp::DoubleProperty *depth) {
  internalSearchPaths(n,result,depth);
  return result->getNodeValue(src);
}

