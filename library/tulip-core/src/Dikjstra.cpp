/**
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

#include <tulip/Dikjstra.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/GraphTools.h>

using namespace tlp;
using namespace std;

//============================================================
Dikjstra::Dikjstra(const Graph *const graph, node src, const EdgeStaticProperty<double> &weights,
                   NodeStaticProperty<double> &nd, std::function<Iterator<edge> *(node)> &getEdges)
    : nodeDistance(nd) {
  assert(src.isValid());
  this->graph = graph;
  usedEdges.setAll(false);
  this->src = src;
  set<DikjstraElement *, LessDikjstraElement> dikjstraTable;
  NodeStaticProperty<DikjstraElement *> mapDik(graph);
  mapDik.setAll(nullptr);

  unsigned int i = 0;
  for (auto n : graph->nodes()) {
    DikjstraElement *tmp;
    if (n != src)
      // init all nodes to +inf
      tmp = new DikjstraElement(DBL_MAX / 2. + 10., node(), n);
    else
      // init starting node to 0
      tmp = new DikjstraElement(0, n, n);

    dikjstraTable.insert(tmp);

    mapDik[i++] = tmp;
  }

  while (!dikjstraTable.empty()) {
    // select the first element in the list the one with min value
    set<DikjstraElement *, LessDikjstraElement>::iterator it = dikjstraTable.begin();
    DikjstraElement &u = *(*it);
    dikjstraTable.erase(it);

    edge e;

    Iterator<edge> *iter = getEdges(u.n);

    while (iter->hasNext()) {
      e = iter->next();
      node v = graph->opposite(e, u.n);
      auto dEle = mapDik[v];
      double eWeight = weights.getEdgeValue(e);
      assert(eWeight > 0);

      if (fabs((u.dist + eWeight) - dEle->dist) < 1E-9) // path of the same length
        dEle->usedEdge.push_back(e);
      else if ((u.dist + eWeight) < dEle->dist) {
        // we find a node closer with that path
        dEle->usedEdge.clear();
        //**********************************************
        dikjstraTable.erase(dEle);

        dEle->dist = u.dist + eWeight;
        dEle->previous = u.n;
        dEle->usedEdge.push_back(e);
        dikjstraTable.insert(dEle);
      }
    }
    delete iter;
  }

  usedEdges.setAll(false);
  i = 0;
  for (auto n : graph->nodes()) {
    DikjstraElement *dEle = mapDik[i++];
    nodeDistance[n] = dEle->dist;

    for (auto e : dEle->usedEdge) {
      usedEdges.set(e.id, true);
    }

    delete dEle;
  }
}
//=============================================================================
bool Dikjstra::searchPath(node n, BooleanProperty *result) {
  bool ok = true;

  while (ok) {
    result->setNodeValue(n, true);
    ok = false;
    Iterator<edge> *it = graph->getInOutEdges(n);
    while (it->hasNext()) {
      edge e = it->next();
      if (!usedEdges.get(e.id))
        continue; // edge does not belong to the shortest path

      if (result->getEdgeValue(e))
        continue; // edge already treated

      node tgt = graph->opposite(e, n);

      if (nodeDistance[tgt] >= nodeDistance[n])
        continue;

      n = tgt;
      result->setEdgeValue(e, true);
      ok = true;
      break;
    }
    delete it;
  }

  if (n != src) {
#ifndef NDEBUG
    cout << "Path does not exist !" << endl;
#endif /* NDEBUG */
    return false;
  }

  return true;
}
//=======================================================================
void Dikjstra::internalSearchPaths(node n, BooleanProperty *result) {
  result->setNodeValue(n, true);
  for (auto e : graph->getInOutEdges(n)) {
    if (!usedEdges.get(e.id))
      continue;

    if (result->getEdgeValue(e))
      continue;

    node tgt = graph->opposite(e, n);

    if (nodeDistance[tgt] >= nodeDistance[n])
      continue;

    result->setEdgeValue(e, true);
    if (!result->getNodeValue(tgt))
      internalSearchPaths(tgt, result);
  }
}
//========================================
bool Dikjstra::searchPaths(node n, BooleanProperty *result) {
  internalSearchPaths(n, result);
  return result->getNodeValue(src);
}

//========================================
#define SMALLEST_WEIGHT 1.E-6

bool selectShortestPaths(const Graph *const graph, node src, node tgt, ShortestPathType pathType,
                         const DoubleProperty *const weights, BooleanProperty *result) {
  std::function<Iterator<edge> *(node)> getOutEdges = [&](node un) {
    return graph->getOutEdges(un);
  };
  std::function<Iterator<edge> *(node)> getInOutEdges = [&](node un) {
    return graph->getInOutEdges(un);
  };
  std::function<Iterator<edge> *(node)> getInEdges = [&](node un) { return graph->getInEdges(un); };

  std::function<Iterator<edge> *(node)> getEdges;
  switch (pathType) {
  case ShortestPathType::OnePath:
  case ShortestPathType::AllPaths:
    getEdges = getInOutEdges;
    break;
  case ShortestPathType::OneDirectedPath:
  case ShortestPathType::AllDirectedPaths:
    getEdges = getOutEdges;
    break;
  case ShortestPathType::OneReversedPath:
  case ShortestPathType::AllReversedPaths:
    getEdges = getInEdges;
    break;
  }

  EdgeStaticProperty<double> eWeights(graph);
  if (!weights) {
    eWeights.setAll(SMALLEST_WEIGHT);
  } else {
    auto fn = [&](edge e, unsigned int i) {
      double val(weights->getEdgeValue(e));

      eWeights[i] = val ? val : SMALLEST_WEIGHT;
    };
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, fn);
  }

  NodeStaticProperty<double> nodeDistance(graph);
  Dikjstra dikjstra(graph, src, eWeights, nodeDistance, getEdges);

  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);

  if (uint(pathType) < ShortestPathType::AllPaths)
    return dikjstra.searchPath(tgt, result);
  return dikjstra.searchPaths(tgt, result);
}
