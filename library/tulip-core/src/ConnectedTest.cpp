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
#include <list>
#include <tulip/ConnectedTest.h>
#include <tulip/ConnectedTestListener.h>
#include <tulip/Graph.h>
#include <tulip/MutableContainer.h>
#include <tulip/StaticProperty.h>

using namespace std;
using namespace tlp;
//=================================================================
static ConnectedTestListener instance;
//=================================================================
static unsigned int connectedTest(const Graph *const graph, node n,
                                  NodeStaticProperty<bool> &visited) {
  list<node> nodesToVisit;
  visited[n] = true;
  nodesToVisit.push_front(n);
  unsigned int count = 1;

  while (!nodesToVisit.empty()) {
    node r = nodesToVisit.front();
    nodesToVisit.pop_front();
    // loop on all neighbours
    for (auto neighbour : graph->getInOutNodes(r)) {
      unsigned int neighPos = graph->nodePos(neighbour);
      // check if neighbour has been visited
      if (!visited[neighPos]) {
        // mark neighbour as already visited
        visited[neighPos] = true;
        // push it for further deeper exploration
        nodesToVisit.push_back(neighbour);
        ++count;
      }
    }
  }

  return count;
}
//=================================================================
#ifndef NDEBUG
static bool connectedTest(const Graph *const graph) {
  if (graph->isEmpty())
    return true;

  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);
  unsigned int count = connectedTest(graph, graph->getOneNode(), visited);
  return (count == graph->numberOfNodes());
}
#endif
//=================================================================
bool ConnectedTest::isConnected(const tlp::Graph *const graph) {
  if (instance.resultsBuffer.find(graph) != instance.resultsBuffer.end())
    return instance.resultsBuffer[graph];

  if (graph->isEmpty())
    return true;

  // trees are the minimally connected graphs
  if (graph->numberOfEdges() < graph->numberOfNodes() - 1)
    return false;

  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);
  unsigned int count = connectedTest(graph, graph->getOneNode(), visited);
  bool result = (count == graph->numberOfNodes());
  graph->addListener(instance);
  return instance.resultsBuffer[graph] = result;
}
//=================================================================
void ConnectedTest::makeConnected(Graph *graph, vector<edge> &addedEdges) {
  graph->removeListener(instance);
  instance.resultsBuffer.erase(graph);
  vector<node> toLink;
  connect(graph, toLink);

  for (unsigned int i = 1; i < toLink.size(); ++i)
    addedEdges.push_back(graph->addEdge(toLink[i - 1], toLink[i]));

  assert(connectedTest(graph));
}
//=================================================================
unsigned int ConnectedTest::numberOfConnectedComponents(const tlp::Graph *const graph) {
  if (graph->isEmpty())
    return 0u;

  // trees are the minimally connected graphs
  if (graph->numberOfNodes() == graph->numberOfEdges() + 1)
    return 1u;

  graph->removeListener(instance);
  vector<node> toLink;
  connect(graph, toLink);
  unsigned int result;

  if (!toLink.empty())
    result = toLink.size();
  else
    result = 1u;

  instance.resultsBuffer[graph] = (result == 1u);
  graph->addListener(instance);
  return result;
}
//======================================================================
void ConnectedTest::computeConnectedComponents(const tlp::Graph *graph,
                                               vector<vector<node>> &components) {
  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);
  // do a bfs traversal for each node
  TLP_MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
    // check if curNode has been already visited
    if (!visited[i]) {
      // add a new component
      components.emplace_back();
      std::vector<node> &component = components.back();
      // and initialize it with current node
      component.push_back(n);
      // do a bfs traversal this node
      list<node> nodesToVisit;
      visited[i] = true;
      nodesToVisit.push_front(n);

      while (!nodesToVisit.empty()) {
        n = nodesToVisit.front();
        nodesToVisit.pop_front();

        // loop on all neighbours
        for (auto neighbour : graph->getInOutNodes(n)) {
          unsigned int neighPos = graph->nodePos(neighbour);
          // check if neighbour has been visited
          if (!visited[neighPos]) {
            // mark neighbour as already visited
            visited[neighPos] = true;
            // insert it in current component
            component.push_back(neighbour);
            // push it for further deeper exploration
            nodesToVisit.push_back(neighbour);
          }
        }
      }
    }
  });
}
//=================================================================
void ConnectedTest::connect(const tlp::Graph *const graph, vector<node> &toLink) {
  auto it = instance.resultsBuffer.find(graph);
  if (it != instance.resultsBuffer.end()) {
    if (it->second)
      return;
  }

  if (graph->isEmpty())
    return;

  NodeStaticProperty<bool> visited(graph);
  visited.setAll(false);

  TLP_MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
    if (!visited[i]) {
      toLink.push_back(n);
      connectedTest(graph, n, visited);
    }
  });
}
