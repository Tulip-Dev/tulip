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
#include "StrongComponent.h"

PLUGIN(StrongComponent)

using namespace std;
using namespace tlp;

unsigned StrongComponent::attachNumerotation(tlp::node n,
                                             std::unordered_map<tlp::node, bool> &visited,
                                             std::unordered_map<tlp::node, bool> &finished,
                                             std::unordered_map<tlp::node, unsigned> &minAttach,
                                             unsigned &id, std::stack<tlp::node> &renum,
                                             unsigned &curComponent) {
  if (visited[n])
    return minAttach[n];

  visited[n] = true;
  minAttach[n] = id;
  renum.push(n);
  auto oldId = id;
  unsigned res = id++;

  for (auto nn : graph->getOutNodes(n)) {

    if (!finished[nn]) {
      unsigned tmp = attachNumerotation(nn, visited, finished, minAttach, id, renum, curComponent);

      if (res > tmp)
        res = tmp;
    }
  }

  minAttach[n] = res;

  if (res == oldId) {
    node tmp = renum.top();
    while (tmp != n) {
      renum.pop();
      finished[tmp] = true;
      minAttach[tmp] = res;
      result->setNodeValue(tmp, curComponent);
      tmp = renum.top();
    }

    finished[n] = true;
    result->setNodeValue(n, curComponent++);
    renum.pop();
  }

  return res;
}

StrongComponent::StrongComponent(const tlp::PluginContext *context) : DoubleAlgorithm(context) {
  addOutParameter<unsigned>("#strongly connected components",
                            "Number of strongly components found");
  // old name
  declareDeprecatedName("Strongly Connected Component");
}

StrongComponent::~StrongComponent() {}

bool StrongComponent::run() {
  std::unordered_map<node, bool> visited(graph->numberOfNodes());
  std::unordered_map<node, bool> finished(graph->numberOfNodes());
  stack<node> renum;
  std::unordered_map<node, unsigned> cachedValues(graph->numberOfNodes());
  unsigned id = 1;
  unsigned curComponent = 0;

  for (auto itn : graph->nodes()) {
    if (!visited[itn]) {
      attachNumerotation(itn, visited, finished, cachedValues, id, renum, curComponent);
    }
  }

  for (auto ite : graph->edges()) {
    auto eEnds = graph->ends(ite);

    if (result->getNodeValue(eEnds.first) == result->getNodeValue(eEnds.second))
      result->setEdgeValue(ite, result->getNodeValue(eEnds.first));
    else
      result->setEdgeValue(ite, curComponent);
  }

  if (dataSet != nullptr)
    dataSet->set<unsigned>("#strongly connected components", curComponent);

  return true;
}
