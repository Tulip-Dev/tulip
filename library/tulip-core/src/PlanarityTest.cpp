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
#include <tulip/Graph.h>
#include <tulip/PlanarityTest.h>
#include <tulip/PlanarityTestImpl.h>
#include <tulip/BiconnectedTest.h>

using namespace std;
using namespace tlp;
//=================================================================
PlanarityTest * PlanarityTest::instance=NULL;
//=================================================================
bool PlanarityTest::isPlanar(Graph* graph) {
  if(instance==NULL)
    instance = new PlanarityTest();

  Observable::holdObservers();
  bool result = instance->compute(graph);
  Observable::unholdObservers();
  return result;
}
bool PlanarityTest::isPlanarEmbedding(const tlp::Graph* graph) {
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

  for (; it!=addedEdges.end(); ++it)
    graph->delEdge(*it, true);

  Observable::unholdObservers();
  return true;
}
//=================================================================
list<edge> PlanarityTest::getObstructionsEdges(Graph* graph) {
  if (PlanarityTest::isPlanar(graph))
    return list<edge>();

  vector<edge> addedEdges;
  Observable::holdObservers();
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  planarTest.isPlanar(true);
  list<edge> tmpList = planarTest.getObstructions();
  {
    vector<edge>::const_iterator it = addedEdges.begin();

    for (; it!=addedEdges.end(); ++it)
      graph->delEdge(*it, true);
  }
  Observable::unholdObservers();
  set<edge> tmpAdded(addedEdges.begin(), addedEdges.end());
  list<edge> result;

  for (list<edge>::iterator it = tmpList.begin(); it != tmpList.end(); ++it) {
    if (tmpAdded.find(*it) == tmpAdded.end())
      result.push_back(*it);
  }

  return result;
}
//=================================================================
bool PlanarityTest::compute(Graph* graph) {

  if (resultsBuffer.find(graph)!=resultsBuffer.end())
    return resultsBuffer[graph];

  unsigned int nbOfNodes = graph->numberOfNodes();

  if (nbOfNodes==0) {
    resultsBuffer[graph] = true;
    return true;
  }

  // quick test
  if ((nbOfNodes >= 3) && (graph->numberOfEdges() > ((3 * nbOfNodes) - 6))) {
    graph->addListener(this);
    return resultsBuffer[graph] = false;
  }

  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  PlanarityTestImpl planarTest(graph);
  resultsBuffer[graph] = planarTest.isPlanar(true);
  vector<edge>::const_iterator it = addedEdges.begin();

  for (; it!=addedEdges.end(); ++it)
    graph->delEdge(*it, true);

  graph->addListener(this);
  return resultsBuffer[graph];
}
//=================================================================
void PlanarityTest::treatEvent(const Event& evt) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

  if (gEvt) {
    Graph* graph = gEvt->getGraph();

    switch(gEvt->getType()) {
    case GraphEvent::TLP_DEL_NODE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (!resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_EDGE:

      if (resultsBuffer.find(graph)!=resultsBuffer.end())
        if (resultsBuffer[graph]) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      break;
    }
  }
  else {
    // From my point of view the use of dynamic_cast should be correct
    // but it fails, so I use reinterpret_cast (pm)
    Graph* graph = reinterpret_cast<Graph *>(evt.sender());

    if (graph && evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
