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

#include <vector>
#include <tulip/Graph.h>
#include <tulip/GraphTools.h>
#include <tulip/SimpleTest.h>
#include <tulip/MutableContainer.h>

using namespace std;
using namespace tlp;
//=================================================================
SimpleTest *SimpleTest::undirInstance = nullptr;
SimpleTest *SimpleTest::dirInstance = nullptr;

//=================================================================
SimpleTest::SimpleTest() {}
//=================================================================
bool SimpleTest::isSimple(const tlp::Graph *graph, const bool directed) {
  SimpleTest *instance = nullptr;
  if (directed) {
    if (!dirInstance) {
      dirInstance = new SimpleTest();
    }
    instance = dirInstance;
  } else {
    if (!undirInstance) {
      undirInstance = new SimpleTest();
    }
    instance = undirInstance;
  }

  if (instance->resultsBuffer.find(graph) == instance->resultsBuffer.end()) {
    instance->resultsBuffer[graph] = simpleTest(graph, nullptr, nullptr, directed);
    graph->addListener(instance);
  }

  return instance->resultsBuffer[graph];
}
//**********************************************************************
void SimpleTest::makeSimple(Graph *graph, vector<edge> &removed, const bool directed) {
  if (SimpleTest::isSimple(graph, directed))
    return;

  SimpleTest::simpleTest(graph, &removed, &removed, directed);

  edge prev;
  for (edge e : removed) {
    // multiple edges and loops were pushed back into the removed vector
    // but an edge can be a loop and a multiple one at the same time
    // so it may have been added twice in the vector.
    // As the edges are only processed once (see static function below)
    // the two occurences are necessarily consecutives
    if (e != prev)
      graph->delEdge(e);
    prev = e;
  }

  assert(SimpleTest::isSimple(graph, directed));
}
//=================================================================
bool SimpleTest::simpleTest(const tlp::Graph *graph, vector<edge> *multipleEdges,
                            vector<edge> *loops, const bool directed) {
  bool result = true;
  bool computeAll = (loops != nullptr) || (multipleEdges != nullptr);
  MutableContainer<bool> visited;
  visited.setAll(false);

  auto getEdges = getEdgesIterator(directed ? DIRECTED : UNDIRECTED);

  for (auto current : graph->nodes()) {
    // Search for multiple edges and loops
    MutableContainer<bool> targeted;
    targeted.setAll(false);

    for (auto e : getEdges(graph, current)) {

      // check if edge has already been visited
      // Take care that in makeSimple (see above) we assume that edges
      // are only processed once
      if (visited.get(e.id))
        continue;

      // mark edge as already visited
      visited.set(e.id, true);
      node target = graph->opposite(e, current);

      if (target == current) { // loop
        if (!computeAll) {
          result = false;
          break;
        }

        if (loops != nullptr) {
          loops->push_back(e);
          result = false;
        }
      }

      if (targeted.get(target.id) == true) {
        if (!computeAll) {
          result = false;
          break;
        }

        if (multipleEdges != nullptr) {
          multipleEdges->push_back(e);
          result = false;
        }
      } else
        targeted.set(target.id, true);
    }

    if (!computeAll && !result)
      break;
  }

  return result;
}
//=================================================================
void SimpleTest::deleteResult(Graph *graph) {
  resultsBuffer.erase(graph);
  graph->removeListener(this);
}
//=================================================================
void SimpleTest::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer[graph])
        deleteResult(graph);

      break;

    case GraphEvent::TLP_DEL_EDGE:

      if (!resultsBuffer[graph])
        deleteResult(graph);

      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE)
      deleteResult(graph);
  }
}
