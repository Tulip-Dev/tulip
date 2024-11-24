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
#include "StrongComponent.h"
#include <queue>

PLUGIN(StrongComponent)

using namespace std;
using namespace tlp;

struct dfsFindSCCStruct {
  node curN;
  unsigned oldTS, res;
  Iterator<node> *outNodes;

  dfsFindSCCStruct(node n, Iterator<node> *nodes, unsigned ts)
      : curN(n), oldTS(ts), res(ts), outNodes(nodes) {}
};

// dfs from n and its out neighbours
// to find strongly connected components (used in directed case)
void StrongComponent::findSCC(tlp::node n, tlp_hash_map<tlp::node, bool> &finished,
                              tlp_hash_map<tlp::node, unsigned> &minTS,
                              std::stack<tlp::node> &renum) {
  minTS[n] = curTS;
  renum.push(n);

  // dfs loop on all out neighbours of n
  std::stack<dfsFindSCCStruct> dfsStack;
  dfsStack.emplace(n, graph->getOutNodes(n), curTS++);

  while (!dfsStack.empty()) {
    auto &topInfos = dfsStack.top();
    while (topInfos.outNodes->hasNext()) {
      // get next out neighbour
      n = topInfos.outNodes->next();
      if (!finished[n]) {
        // check if it has not been visited
        if (minTS[n] == 0) {
          minTS[n] = curTS;
          renum.push(n);
          // push neighbour infos on top of dfsStack
          dfsStack.emplace(n, graph->getOutNodes(n), curTS++);
          // to go deeper
          break;
        }
        if (topInfos.res > minTS[n])
          topInfos.res = minTS[n];
      }
    }
    if (&topInfos != &dfsStack.top())
      // neighbour infos has been pushed on stack
      continue;

    // cleanup
    delete topInfos.outNodes;

    minTS[topInfos.curN] = topInfos.res;

    if (topInfos.res == topInfos.oldTS) {
      n = renum.top();
      while (n != topInfos.curN) {
        renum.pop();
        finished[n] = true;
        minTS[n] = topInfos.res;
        result->setNodeValue(n, curComponent);
        n = renum.top();
      }

      finished[topInfos.curN] = true;
      result->setNodeValue(topInfos.curN, curComponent++);
      renum.pop();
    }

    // pop stack
    dfsStack.pop();

    if (dfsStack.empty())
      return;

    // propagate res of curN to its parent
    auto &parentInfos = dfsStack.top();
    if (parentInfos.res > topInfos.res)
      parentInfos.res = topInfos.res;
  }
}

struct dfsFindDEStruct {
  node curN, parent;
  edge edgeToParent;
  Iterator<edge> *inOutEdges;

  dfsFindDEStruct(node n, Iterator<edge> *edges, node nParent = node(), edge e = edge())
      : curN(n), parent(nParent), edgeToParent(e), inOutEdges(edges) {}
};

// dfs from n and its inout neighbours
// to find the disconnecting edges (used in undirected case)
void StrongComponent::findDE(tlp::node n, std::vector<tlp::edge> &de,
                             tlp_hash_map<tlp::node, unsigned int> &visitedTS,
                             tlp_hash_map<tlp::node, unsigned int> &minVisitedTS) {
  // initialize n visitedTS and minVisitedTS
  // with the current time stamp
  visitedTS[n] = minVisitedTS[n] = curTS++;

  // dfs loop on all neighbours of n
  std::stack<dfsFindDEStruct> dfsStack;
  dfsStack.emplace(n, graph->getInOutEdges(n));

  while (!dfsStack.empty()) {
    auto &topInfos = dfsStack.top();
    while (topInfos.inOutEdges->hasNext()) {
      auto e = topInfos.inOutEdges->next();
      // get next inout neighbour
      n = graph->opposite(e, topInfos.curN);
      // check if n has been visited
      if (visitedTS[n] == 0) {
        // initialize visitedTS and minVisitedTS of neighbour
        // with the current time stamp
        visitedTS[n] = minVisitedTS[n] = curTS++;
        // push neighbour infos on top of dfsStack
        dfsStack.emplace(n, graph->getInOutEdges(n), topInfos.curN, e);
        // to go deeper
        break;
      } else if (n != topInfos.parent)
        // Update minVisitedTS of curN for parent comparison
        minVisitedTS[topInfos.curN] = std::min(minVisitedTS[topInfos.curN], visitedTS[n]);
    }
    if (&topInfos != &dfsStack.top())
      // neighbour infos has been pushed on stack
      continue;

    // cleanup
    delete topInfos.inOutEdges;

    // pop stack
    dfsStack.pop();

    if (dfsStack.empty())
      return;

    // if the minVisitedTS reachable from curN subgraph
    // is before the visitedTS of its parent in DFS graph,
    // then the edge between curN and its parent is a bridge
    if (minVisitedTS[topInfos.curN] > visitedTS[topInfos.parent])
      de.push_back(topInfos.edgeToParent);
    // propagate minVisitedTS of curN to its parent
    minVisitedTS[topInfos.parent] =
        std::min(minVisitedTS[topInfos.parent], minVisitedTS[topInfos.curN]);
  }
}

StrongComponent::StrongComponent(const tlp::PluginContext *context) : DoubleAlgorithm(context) {
  addInParameter<bool>("directed",
                       "Indicates if the graph should be considered as directed or not.", "true");
  addOutParameter<unsigned>("#strongly connected components",
                            "Number of strongly components found");
}

StrongComponent::~StrongComponent() {}

bool StrongComponent::run() {
  // ensure metric value of
  // disconnecting edges will be zero
  curComponent = 1;
  // ensure time stamp of non visited nodes
  // will be zero
  curTS = 1;
  bool directed = true;

  if (dataSet)
    dataSet->get("directed", directed);

  if (directed) {
    // Tarjan algorithm
    // indicates if the dfs from a node has been done
    tlp_hash_map<node, bool> finished(graph->numberOfNodes());
    // indicates the minimum time stamp encountered along the
    // dfs from a node
    tlp_hash_map<node, unsigned> minTS(graph->numberOfNodes());
    // record the nodes encountered along the dfs
    stack<node> renum;

    for (auto n : graph->nodes()) {
      if (minTS[n] == 0)
        findSCC(n, finished, minTS, renum);
    }
  } else {
    // looking for the disconnecting edges
    std::vector<edge> de;
    {
      // records the time stamp when a node is first visited
      tlp_hash_map<tlp::node, unsigned int> visitedTS(graph->numberOfNodes());
      // indicates the minimum time stamp encountered along the
      // dfs from a node
      tlp_hash_map<tlp::node, unsigned int> minVisitedTS(graph->numberOfNodes());
      for (auto n : graph->nodes()) {
        if (visitedTS[n] == 0)
          findDE(n, de, visitedTS, minVisitedTS);
      }
    }
    if (!de.empty()) {
      tlp_hash_map<tlp::edge, bool> visited(graph->numberOfEdges());
      curComponent = 1;
      // mark disconnecting edges
      // as already visited
      for (auto e : de)
        visited[e] = true;
      // then compute scc
      for (auto n : graph->nodes()) {
        if (!result->getNodeValue(n)) {
          result->setNodeValue(n, curComponent);
          std::queue<node> nodesQueue;
          nodesQueue.push(n);
          // BFS loop from current node
          // to extract the connected component owning it
          while (!nodesQueue.empty()) {
            node n = nodesQueue.front();
            nodesQueue.pop();
            // loop on all neighbours
            for (auto e : graph->getInOutEdges(n)) {
              // check if e has been visited
              if (!visited[e]) {
                // mark e as already visited
                visited[e] = true;
                auto nn = graph->opposite(e, n);
                if (!result->getNodeValue(nn)) {
                  result->setNodeValue(nn, curComponent);
                  // add in queue for further deeper exploration
                  nodesQueue.push(nn);
                }
              }
            }
          }
          // increase for next scc
          ++curComponent;
        }
      }
    }
  }
  for (auto ite : graph->edges()) {
    auto eEnds = graph->ends(ite);

    if (result->getNodeValue(eEnds.first) == result->getNodeValue(eEnds.second))
      result->setEdgeValue(ite, result->getNodeValue(eEnds.first));
    else
      result->setEdgeValue(ite, 0);
  }

  if (dataSet != nullptr)
    dataSet->set<unsigned>("#strongly connected components", --curComponent);

  return true;
}
