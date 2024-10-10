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

#include <tulip/StringProperty.h>
#include <tulip/StringCollection.h>

#include "StrahlerMetric.h"

PLUGIN(StrahlerMetric)

using namespace std;
using namespace tlp;

namespace std {
struct couple {
  int p, r;
  bool operator==(const couple d) {
    if ((p == d.p) && (r == d.r))
      return true;

    return false;
  }
};

template <>
struct equal_to<couple> {
  bool operator()(const couple c, const couple d) {
    if ((c.r == d.r) && (c.p == d.p))
      return true;

    return false;
  }
};

template <>
struct less<couple> {
  bool operator()(const couple c, const couple d) {
    return (c.r < d.r) || ((c.r == d.r) && (c.p < d.p));
  }
};
} // namespace std

struct StackEval {
  StackEval(int f, int u) : freeS(f), usedS(u) {}
  int freeS, usedS;
};

struct GreaterStackEval {
  bool operator()(const StackEval e1, const StackEval e2) {
    return (e1.freeS > e2.freeS);
  }
};

Strahler StrahlerMetric::topSortStrahler(tlp::node n, SortInfos &nInfos, int &curPref,
                                         std::unordered_map<node, SortInfos> &sortInfos,
                                         std::unordered_map<node, Strahler> &cachedValues) {
  Strahler result;
  nInfos.visited = true;
  nInfos.prefix = curPref++;

  if (graph->outdeg(n) == 0) {
    nInfos.finished = true;
    return (result);
  }

  list<int> strahlerResult;
  list<StackEval> tmpEval;
  // Construction des ensembles pour evaluer le strahler

  for (auto tmpN : graph->getOutNodes(n)) {
    SortInfos &tmpInfos = sortInfos[tmpN];
    if (!tmpInfos.visited) {
      // Arc Normal
      nInfos.toFree = 0;
      Strahler tmpValue = topSortStrahler(tmpN, tmpInfos, curPref, sortInfos, cachedValues);
      // Data for strahler evaluation on the spanning Dag.
      strahlerResult.push_front(tmpValue.strahler);
      // Counting current used stacks.
      tmpEval.emplace_back(tmpValue.stacks - tmpValue.usedStack + nInfos.toFree,
                           tmpValue.usedStack - nInfos.toFree);
    } else {
      if (tmpInfos.finished) {
        if (tmpInfos.prefix < nInfos.prefix) {
          // Cross Edge
          Strahler tmpValue = cachedValues[tmpN];
          // Data for strahler evaluation on the spanning Dag.
          strahlerResult.push_front(tmpValue.strahler);
          // Looking if we need more stacks to evaluate this node.
          tmpEval.emplace_back(tmpValue.stacks, 0);
        } else {
          // Arc descent
          Strahler tmpValue = cachedValues[tmpN];
          // Data for strahler evaluation on the spanning Dag.
          strahlerResult.push_front(tmpValue.strahler);
        }
      } else {
        if (tmpN == n) {
          tmpEval.emplace_back(1, 0);
        } else {
          // New nested cycle.
          ++tmpInfos.toFree;
          tmpEval.emplace_back(0, 1);
        }

        // Return edge
        // Register needed tp store the result of the recursive call
        strahlerResult.push_front(1);
      }
    }
  }

  // compute the minimal nested cycles
  GreaterStackEval gSE;
  tmpEval.sort(gSE);
  result.stacks = 0;
  result.usedStack = 0;

  for (auto se : tmpEval) {
    result.usedStack += se.usedS;
    result.stacks = std::max(result.stacks, se.freeS + se.usedS);
    result.stacks -= se.usedS;
  }

  result.stacks = result.stacks + result.usedStack;
  // evaluation of tree strahler
  int additional = 0;
  int available = 0;
  strahlerResult.sort();

  while (!strahlerResult.empty()) {
    int tmpDbl = strahlerResult.back();
    strahlerResult.pop_back();

    if (tmpDbl > available) {
      additional += tmpDbl - available;
      available = tmpDbl - 1;
    } else
      available -= 1;
  }

  result.strahler = additional;
  strahlerResult.clear();
  nInfos.finished = true;
  cachedValues[n] = result;
  return result;
}

static const char *paramHelp[] = {
    // All nodes
    "If true, for each node the Strahler number is computed from a spanning tree having that node "
    "as root: complexity o(n^2). If false the Strahler number is computed from a spanning tree "
    "having the heuristicly estimated graph center as root.",

    // Type
    "Sets the type of computation."};

#define COMPUTATION_TYPE "type"
#define COMPUTATION_TYPES "all;ramification;nested cycles;"
#define ALL 0
#define REGISTERS 1
#define STACKS 2
//==============================================================================
StrahlerMetric::StrahlerMetric(const tlp::PluginContext *context)
    : DoubleAlgorithm(context), allNodes(false) {
  addInParameter<bool>("all nodes", paramHelp[0], "false");
  addInParameter<StringCollection>("type", paramHelp[1], COMPUTATION_TYPES, true,
                                   "all<br/>ramification<br/>nested cycles");
}
//==============================================================================
bool StrahlerMetric::run() {
  allNodes = false;
  StringCollection computationTypes(COMPUTATION_TYPES);
  computationTypes.setCurrent(0);

  if (dataSet != nullptr) {
    dataSet->get("all nodes", allNodes);
    dataSet->get("type", computationTypes);
  }

  std::unordered_map<node, SortInfos> sortInfos;
  std::unordered_map<node, Strahler> cachedValues;
  int curPref = 0;

  unsigned int i = 0;

  if (pluginProgress)
    pluginProgress->showPreview(false);

  for (auto n : graph->nodes()) {
    SortInfos &nInfos = sortInfos[n];
    nInfos.toFree = 0;

    if (!nInfos.finished) {
      topSortStrahler(n, nInfos, curPref, sortInfos, cachedValues);
    }

    if (allNodes) {
      if (pluginProgress && ((++i % 100) == 0) &&
          (pluginProgress->progress(i, graph->numberOfNodes()) != TLP_CONTINUE))
        break;

      switch (computationTypes.getCurrent()) {
      case ALL:
        result->setNodeValue(
            n, sqrt(double(cachedValues[n].strahler) * double(cachedValues[n].strahler) +
                    double(cachedValues[n].stacks) * double(cachedValues[n].stacks)));
        break;

      case REGISTERS:
        result->setNodeValue(n, cachedValues[n].strahler);
        break;

      case STACKS:
        result->setNodeValue(n, cachedValues[n].stacks);
      }

      sortInfos.clear();
      cachedValues.clear();
      curPref = 0;
    }
  }

  if (pluginProgress->state() != TLP_CONTINUE)
    return pluginProgress->state() != TLP_CANCEL;

  if (!allNodes) {

    for (auto n : graph->nodes()) {

      switch (computationTypes.getCurrent()) {
      case ALL:
        result->setNodeValue(
            n, sqrt(double(cachedValues[n].strahler) * double(cachedValues[n].strahler) +
                    double(cachedValues[n].stacks) * double(cachedValues[n].stacks)));
        break;

      case REGISTERS:
        result->setNodeValue(n, cachedValues[n].strahler);
        break;

      case STACKS:
        result->setNodeValue(n, cachedValues[n].stacks);
      }
    }
  }

  return pluginProgress->state() != TLP_CANCEL;
}
//==============================================================================
