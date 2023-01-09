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
#include <stack>

#include <tulip/BiconnectedTest.h>
#include <tulip/ConnectedTest.h>
#include <tulip/ConnectedTestListener.h>
#include <tulip/Graph.h>
#include <tulip/MutableContainer.h>
#include <tulip/StableIterator.h>

using namespace std;
using namespace tlp;

//=================================================================
static ConnectedTestListener instance;
//=================================================================
// structure below is used to implement dfs loop
struct dfsBiconnectStruct {
  node from, u, first;
  unsigned int depth;
  Iterator<node> *inOutNodes;

  dfsBiconnectStruct(Graph *graph, node n, unsigned int d = 0, node u = node(), node first = node())
      : from(n), u(u), first(first), depth(d),
        inOutNodes(new StableIterator<node>(graph->getInOutNodes(from))) {}
};

static void makeBiconnectedDFS(Graph *graph, vector<edge> &addedEdges) {
  // the graph is already connected
  // so get any node to begin
  node from = graph->getOneNode();

  if (!from.isValid())
    return;

  MutableContainer<int> low;
  MutableContainer<int> depth;
  depth.setAll(-1);
  MutableContainer<node> supergraph;
  supergraph.setAll(node());

  // dfs loop
  stack<dfsBiconnectStruct> dfsLevels;
  dfsBiconnectStruct dfsParams(graph, from);
  dfsLevels.push(dfsParams);
  depth.set(from.id, 0);
  low.set(from.id, 0);

  while (!dfsLevels.empty()) {
    dfsParams = dfsLevels.top();
    from = dfsParams.from;
    node u = dfsParams.first;

    // for every node connected to from
    Iterator<node> *itN = dfsParams.inOutNodes;

    while (itN->hasNext()) {
      node to = itN->next();

      // if there is a loop, ignore it
      if (from == to) {
        continue;
      }

      if (!u.isValid()) {
        dfsLevels.top().first = u = to;
      }

      // if the destination node has not been visited, visit it
      if (depth.get(to.id) == -1) {
        supergraph.set(to.id, from);
        dfsParams.from = to;
        dfsParams.first = node();
        dfsParams.u = u;
        unsigned int currentDepth = dfsParams.depth + 1;
        dfsParams.depth = currentDepth;
        depth.set(to.id, currentDepth);
        low.set(to.id, currentDepth);
        dfsParams.inOutNodes = new StableIterator<node>(graph->getInOutNodes(to));
        break;
      } else {
        low.set(from.id, std::min(low.get(from.id), depth.get(to.id)));
      }
    }

    if (from != dfsParams.from) {
      dfsLevels.push(dfsParams);
      continue;
    }

    delete itN;

    // pop the current dfsParams
    node to = dfsParams.from;
    from = supergraph.get(to.id);

    if (from.isValid()) {
      u = dfsParams.u;

      if (low.get(to.id) == depth.get(from.id)) {
        if (to == u && supergraph.get(from.id).isValid())
          addedEdges.push_back(graph->addEdge(to, supergraph.get(from.id)));

        if (to != u)
          addedEdges.push_back(graph->addEdge(u, to));
      }

      low.set(from.id, std::min(low.get(from.id), low.get(to.id)));
    }

    dfsLevels.pop();
  }
}

//=================================================================
bool biconnectedTest(const Graph *graph, node v, MutableContainer<unsigned int> &low,
                     MutableContainer<unsigned int> &dfsNumber, MutableContainer<node> &supergraph,
                     unsigned int &count) {
  unsigned int vDfs = count++;
  dfsNumber.set(v.id, vDfs);
  low.set(v.id, vDfs);

  for (auto w : graph->getInOutNodes(v)) {

    if (dfsNumber.get(w.id) == UINT_MAX) {
      if (vDfs == 1) {
        if (count != 2) {
          return false;
        }
      }

      supergraph.set(w.id, v);

      if (!biconnectedTest(graph, w, low, dfsNumber, supergraph, count)) {
        return false;
      }

      if (vDfs != 1) {
        if (low.get(w.id) >= dfsNumber.get(v.id)) {
          return false;
        } else
          low.set(v.id, std::min(low.get(v.id), low.get(w.id)));
      }
    } else if (supergraph.get(v.id) != w) {
      low.set(v.id, std::min(low.get(v.id), dfsNumber.get(w.id)));
    }
  }

  return true;
}
//=================================================================
static bool biconnectedTest(const Graph *graph) {
  MutableContainer<unsigned int> low;
  MutableContainer<unsigned int> dfsNumber;
  dfsNumber.setAll(UINT_MAX);
  MutableContainer<node> supergraph;
  unsigned int count = 1;
  return (biconnectedTest(graph, graph->nodes()[0], low, dfsNumber, supergraph, count) &&
          (count == graph->numberOfNodes() + 1));
}
//=================================================================
bool BiconnectedTest::isBiconnected(const tlp::Graph *graph) {
  if (graph->isEmpty()) {
    return true;
  }

  auto it = instance.resultsBuffer.find(graph);
  if (it != instance.resultsBuffer.end())
    return it->second;

  graph->addListener(instance);
  return instance.resultsBuffer[graph] = biconnectedTest(graph);
}
//=================================================================
void BiconnectedTest::makeBiconnected(Graph *graph, vector<edge> &addedEdges) {
  graph->removeListener(instance);
  instance.resultsBuffer.erase(graph);
  ConnectedTest::makeConnected(graph, addedEdges);
  makeBiconnectedDFS(graph, addedEdges);
  assert(BiconnectedTest::isBiconnected(graph));
}
