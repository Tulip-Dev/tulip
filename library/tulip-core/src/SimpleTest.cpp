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

#include <tulip/Graph.h>
#include <tulip/GraphTools.h>
#include <tulip/SimpleTest.h>
#include <tulip/MutableContainer.h>

using namespace std;
using namespace tlp;
//=================================================================
class SimpleTestListener : public Observable {
public:
  // override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  void treatEvent(const Event &) override;

  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the map.
   **/
  tlp_hash_map<const Graph *, bool> resultsBuffer;

  inline void deleteResult(Graph *graph) {
    resultsBuffer.erase(graph);
    graph->removeListener(this);
  }
};

//=================================================================
void SimpleTestListener::treatEvent(const Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer[graph])
        deleteResult(graph);

      break;

    case GraphEvent::TLP_AFTER_DEL_EDGE:

      if (!resultsBuffer[graph])
        deleteResult(graph);

      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      deleteResult(graph);
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
//=================================================================
static SimpleTestListener undirInstance;
static SimpleTestListener dirInstance;
//=================================================================
bool SimpleTest::isSimple(const tlp::Graph *graph, const bool directed) {
  SimpleTestListener *instance = nullptr;
  if (directed) {
    instance = &dirInstance;
  } else {
    instance = &undirInstance;
  }

  auto it = instance->resultsBuffer.find(graph);
  if (it != instance->resultsBuffer.end())
    return it->second;

  graph->addListener(instance);
  return instance->resultsBuffer[graph] = simpleTest(graph, nullptr, nullptr, directed);
}

bool SimpleTest::hasLoops(const Graph *graph) {
  vector<edge> removed;
  SimpleTest::simpleTest(graph, nullptr, &removed);
  return !removed.empty();
}

bool SimpleTest::hasParallelEdges(const Graph *graph, const bool directed) {
  vector<edge> parallelEdges;
  SimpleTest::simpleTest(graph, &parallelEdges, nullptr, directed);
  return !parallelEdges.empty();
}

//**********************************************************************
void SimpleTest::makeSimple(Graph *graph, vector<edge> &removed, const bool directed) {
  if (SimpleTest::isSimple(graph, directed))
    return;

  SimpleTest::simpleTest(graph, &removed, &removed, directed);

  for (edge e : removed) {
    graph->delEdge(e);
  }

  assert(SimpleTest::isSimple(graph, directed));
}
//=================================================================
bool SimpleTest::simpleTest(const tlp::Graph *graph, vector<edge> *multipleEdges,
                            vector<edge> *loops, const bool directed) {
  bool result = true;
  const bool computeAll = (loops != nullptr) || (multipleEdges != nullptr);
  const bool vDiff = loops != multipleEdges;
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
      bool loopFound = false;

      if (target == current) { // loop
        if (!computeAll) {
          result = false;
          break;
        }

        if (loops != nullptr) {
          loopFound = true;
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
          // e is not added in multipleEdges
          // if it is already a loop and loops == multipleEdges
          if (vDiff || !loopFound)
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
