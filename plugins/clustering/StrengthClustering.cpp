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

#include "StrengthClustering.h"

#include <tulip/StableIterator.h>

using namespace std;
using namespace tlp;

PLUGIN(StrengthClustering)

//================================================================================
StrengthClustering::~StrengthClustering() {}
//==============================================================================
double StrengthClustering::computeMQValue(const vector<unordered_set<node>> &partition, Graph *sg) {

  vector<unsigned int> nbIntraEdges(partition.size(), 0);

  map<pair<unsigned int, unsigned int>, unsigned int> nbExtraEdges;

  MutableContainer<unsigned int> clusterId;
  auto itPart = partition.begin();

  for (unsigned int i = 0; itPart != partition.end(); ++itPart, ++i) {
    auto itSet = itPart->begin();

    for (; itSet != itPart->end(); ++itSet) {
      clusterId.set(itSet->id, i);
    }
  }

  for (auto e : sg->edges()) {
    auto eEnds = sg->ends(e);
    node n1 = eEnds.first;
    node n2 = eEnds.second;

    if (n1.id >= n2.id) {
      n1 = n2;
      n2 = eEnds.first;
    }

    unsigned int n1ClustId = clusterId.get(n1.id);
    unsigned int n2ClustId = clusterId.get(n2.id);

    if (n1ClustId == n2ClustId)
      nbIntraEdges[n1ClustId] += 1;
    else {
      pair<unsigned int, unsigned int> pp(n1ClustId, n2ClustId);

      if (nbExtraEdges.find(pp) != nbExtraEdges.end()) {
        nbExtraEdges[pp] += 1;
      } else {
        nbExtraEdges[pp] = 1;
      }
    }
  }

  double positive = 0;

  for (unsigned int i = 0; i < partition.size(); ++i) {
    if (partition[i].size() > 1)
      positive +=
          2.0 * double(nbIntraEdges[i]) / double(partition[i].size() * (partition[i].size() - 1));
  }

  positive /= double(partition.size());

  double negative = 0;
  map<pair<unsigned int, unsigned int>, unsigned int>::const_iterator itMap = nbExtraEdges.begin();

  for (; itMap != nbExtraEdges.end(); ++itMap) {
    const pair<unsigned int, unsigned int> &pp = itMap->first;
    unsigned int val = itMap->second;

    if (!partition[pp.first].empty() && !partition[pp.second].empty())
      negative += double(val) / double(partition[pp.first].size() * partition[pp.second].size());
  }

  if (partition.size() > 1)
    negative /= double(partition.size() * (partition.size() - 1)) / 2.0;

  double result = positive - negative;
  return result;
}

//==============================================================================
void StrengthClustering::computeNodePartition(double threshold, vector<unordered_set<node>> &result,
                                              tlp::DoubleProperty &values) {
  Graph *tmpGraph = graph->addCloneSubGraph();

  for (auto e : graph->edges()) {
    if (values.getEdgeValue(e) < threshold) {
      auto eEnds = graph->ends(e);

      if (graph->deg(eEnds.first) > 1 && graph->deg(eEnds.second) > 1)
        tmpGraph->delEdge(e);
    }
  }

  // Select SubGraph singleton in graph
  unordered_set<node> singleton;

  for (auto n : tmpGraph->nodes()) {
    if (tmpGraph->deg(n) == 0)
      singleton.insert(n);
  }

  // restore edges to reconnect singleton by computing induced subgraph
  for (auto e : graph->edges()) {
    auto eEnds = graph->ends(e);

    if (singleton.find(eEnds.first) != singleton.end() &&
        singleton.find(eEnds.second) != singleton.end()) {
      tmpGraph->addEdge(e);
    }
  }

  // Extract connected components
  DoubleProperty connected(tmpGraph);
  string errMsg;
  tmpGraph->applyPropertyAlgorithm("Connected Components", &connected, errMsg);

  // Compute the node partition
  int index = 0;
  tlp_hash_map<double, int> resultIndex;

  for (auto n : tmpGraph->nodes()) {
    double val = connected.getNodeValue(n);

    if (resultIndex.find(val) != resultIndex.end())
      result[resultIndex[val]].insert(n);
    else {
      result.emplace_back(unordered_set<node>());
      resultIndex[val] = index;
      result[index].insert(n);
      ++index;
    }
  }

  graph->delAllSubGraphs(tmpGraph);
}
//==============================================================================
double StrengthClustering::findBestThreshold(int numberOfSteps, bool &stopped,
                                             tlp::DoubleProperty &values) {
  double maxMQ = -2;
  double threshold = values.getEdgeMin(graph);
  double deltaThreshold =
      (values.getEdgeMax(graph) - values.getEdgeMin(graph)) / double(numberOfSteps);
  int steps = 0;

  for (double i = values.getEdgeMin(graph); i < values.getEdgeMax(graph); i += deltaThreshold) {
    vector<unordered_set<node>> tmp;
    computeNodePartition(i, tmp, values);

    if (pluginProgress && ((++steps % (numberOfSteps / 10)) == 0)) {
      pluginProgress->progress(steps, numberOfSteps);

      if ((stopped = (pluginProgress->state() != TLP_CONTINUE))) {
        return threshold;
      }
    }

    double mq = computeMQValue(tmp, graph);

    if (mq > maxMQ) {
      threshold = i;
      maxMQ = mq;
    }
  }

  return threshold;
}
//==============================================================================
static const char *paramHelp[] = {
    // metric
    "Metric used in order to multiply strength metric computed values."
    "If one is given, the complexity is O(n log(n)), O(n) neither."};

//================================================================================
StrengthClustering::StrengthClustering(PluginContext *context) : DoubleAlgorithm(context) {
  addInParameter<NumericProperty *>("metric", paramHelp[0], "", false);
  addDependency("Strength", "1.0");
}

//==============================================================================
bool StrengthClustering::run() {
  string errMsg;
  DoubleProperty values(graph);

  if (!graph->applyPropertyAlgorithm("Strength", &values, errMsg, nullptr, pluginProgress))
    return false;

  NumericProperty *metric = nullptr;

  if (dataSet) {
    dataSet->get("metric", metric);
  }

  if (metric) {
    NumericProperty *mult = metric->copyProperty(graph);

    if (pluginProgress)
      pluginProgress->setComment("Computing Strength metric X specified metric on edges ...");

    mult->uniformQuantification(100);
    unsigned int steps = 0, maxSteps = graph->numberOfEdges();

    if (maxSteps < 10)
      maxSteps = 10;

    for (auto e : graph->edges()) {
      values.setEdgeValue(e, values.getEdgeValue(e) * (mult->getEdgeDoubleValue(e) + 1));

      if (pluginProgress && ((++steps % (maxSteps / 10) == 0))) {
        pluginProgress->progress(steps, maxSteps);

        if (pluginProgress->state() != TLP_CONTINUE)
          return pluginProgress->state() != TLP_CANCEL;
      }
    }
    delete mult;
  }

  bool stopped = false;
  const unsigned int NB_TEST = 100;

  if (pluginProgress) {
    pluginProgress->setComment("Partitioning nodes...");
    pluginProgress->progress(0, NB_TEST + 1);
  }

  double threshold = findBestThreshold(NB_TEST, stopped, values);

  if (stopped)
    return pluginProgress->state() != TLP_CANCEL;

  vector<unordered_set<node>> tmp;
  computeNodePartition(threshold, tmp, values);

  for (unsigned int i = 0; i < tmp.size(); ++i) {
    unordered_set<node>::const_iterator it;

    for (it = tmp[i].begin(); it != tmp[i].end(); ++it) {
      result->setNodeValue(*it, i);
    }
  }

  return true;
}
//================================================================================
bool StrengthClustering::check(string &erreurMsg) {
  if (!SimpleTest::isSimple(graph)) {
    erreurMsg = "The graph must be simple";
    return false;
  }
  return true;
}
