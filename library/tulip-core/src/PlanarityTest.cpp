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
#include <set>

#include <tulip/Graph.h>
#include <tulip/PlanarityTest.h>
#include <tulip/PlanarityTestImpl.h>
#include <tulip/BiconnectedTest.h>

using namespace std;
using namespace tlp;
//=================================================================
class PlanarityTestListener : public Observable {
public:
  // override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  void treatEvent(const Event &) override;

  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the map.
   **/
  tlp_hash_map<const Graph *, bool> resultsBuffer;
};

void PlanarityTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    auto it = resultsBuffer.find(graph);
    switch (gEvt->getType()) {
    case GraphEvent::TLP_AFTER_DEL_EDGE:
    case GraphEvent::TLP_AFTER_DEL_NODE:

      if (it != resultsBuffer.end())
        if (it->second)
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_ADD_EDGE:

      if (it != resultsBuffer.end())
        if (it->second == false)
          return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
//=================================================================
static PlanarityTestListener instance;
//=================================================================
bool PlanarityTest::isPlanar(Graph *graph) {
  auto itr = instance.resultsBuffer.find(graph);
  if (itr != instance.resultsBuffer.end())
    return itr->second;

  unsigned int nbOfNodes = graph->numberOfNodes();

  if (nbOfNodes == 0)
    return instance.resultsBuffer[graph] = true;

  // quick test
  if ((nbOfNodes >= 3) && (graph->numberOfEdges() > ((3 * nbOfNodes) - 6))) {
    graph->addListener(instance);
    return instance.resultsBuffer[graph] = false;
  }

  Observable::holdObservers();
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  instance.resultsBuffer[graph] = planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();

  for (; it != addedEdges.end(); ++it)
    graph->delEdge(*it, true);

  Observable::unholdObservers();
  graph->addListener(instance);
  return instance.resultsBuffer[graph];
}
bool PlanarityTest::isPlanarEmbedding(const tlp::Graph *graph) {
  return PlanarityTestImpl::isPlanarEmbedding(graph);
}
//=================================================================
bool PlanarityTest::planarEmbedding(Graph *graph) {
  if (!PlanarityTest::isPlanar(graph))
    return false;

  Observable::holdObservers();
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();

  for (; it != addedEdges.end(); ++it)
    graph->delEdge(*it, true);

  Observable::unholdObservers();
  return true;
}
//=================================================================
list<edge> PlanarityTest::getObstructionsEdges(Graph *graph) {
  if (PlanarityTest::isPlanar(graph))
    return list<edge>();

  vector<edge> addedEdges;
  Observable::holdObservers();
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);
  list<edge> &&result = planarTest.getObstructions();
  set<edge> tmpAdded;
  for (auto e : addedEdges) {
    graph->delEdge(e, true);
    tmpAdded.insert(e);
  }
  Observable::unholdObservers();
  for (auto ite = result.begin(); ite != result.end();) {
    if (tmpAdded.find(*ite) != tmpAdded.end())
      ite = result.erase(ite);
    else
      ++ite;
  }

  return std::move(result);
}
