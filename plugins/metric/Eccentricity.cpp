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
#include <atomic>

#include <tulip/ConnectedTest.h>
#include <tulip/StaticProperty.h>
#include <tulip/GraphMeasure.h>
#include <tulip/GraphParallelTools.h>

#include "Eccentricity.h"

using namespace std;
using namespace tlp;

PLUGIN(EccentricityMetric)

static const char *paramHelp[] = {
    // closeness centrality
    "If true, the closeness centrality is computed (i.e. the average distance from a node to all "
    "others).",

    // norm
    "If true, the returned values are normalized. "
    "For the closeness centrality, the reciprocal of the sum of distances is returned. "
    "The eccentricity values are divided by the graph diameter. "
    "<b> Warning: </b> The normalized eccentricity values should be computed on a (strongly) "
    "connected graph.",

    // directed
    "If true, the graph is considered directed.",

    // weight
    "An existing edge weight metric property."};

EccentricityMetric::EccentricityMetric(const tlp::PluginContext *context)
    : DoubleAlgorithm(context), allPaths(false), norm(true), directed(false) {
  addInParameter<bool>("closeness centrality", paramHelp[0], "false");
  addInParameter<bool>("norm", paramHelp[1], "true");
  addInParameter<bool>("directed", paramHelp[2], "false");
  addInParameter<NumericProperty *>("weight", paramHelp[3], "", false);
  addOutParameter<double>("graph diameter", "The computed diameter (-1 if not computed)", "-1");
}
//====================================================================
EccentricityMetric::~EccentricityMetric() {}
//====================================================================
double EccentricityMetric::compute(unsigned int nPos) {

  NodeStaticProperty<double> distance(graph);
  distance.setAll(0);
  double val = tlp::maxDistance(graph, nPos, distance, weight, directed ? DIRECTED : UNDIRECTED);

  if (!allPaths)
    return val;

  double nbAcc = 0.;
  val = 0.;
  unsigned int nbNodes = graph->numberOfNodes();
  double max_d_acc = nbNodes + 0.;
  if (weight)
    max_d_acc = nbNodes * weight->getEdgeDoubleMax();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    double d = distance[i];

    if (d < max_d_acc) {
      nbAcc += 1.;

      if (i != nPos)
        val += d;
    }
  }

  if (nbAcc < 2.0)
    return 0.0;

  if (norm)
    val = 1.0 / val;
  else
    val /= (nbAcc - 1.0);

  return val;
}
//====================================================================
bool EccentricityMetric::run() {
  allPaths = false;
  norm = true;
  directed = false;
  weight = nullptr;

  if (dataSet != nullptr) {
    dataSet->get("closeness centrality", allPaths);
    dataSet->get("norm", norm);
    dataSet->get("directed", directed);
    dataSet->get("weight", weight);
  }

  // Edges weights should be positive
  if (weight && weight->getEdgeDoubleMin() <= 0) {
    pluginProgress->setError("Edges weights should be positive.");
    return false;
  }

  NodeStaticProperty<double> res(graph);
  unsigned int nbNodes = graph->numberOfNodes();

  double diameter = 1.0;
  std::atomic<bool> stopfor(false);
  TLP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) {
    if (stopfor.load())
      return;

    if (ThreadManager::getThreadNumber() == 0) {
      if (pluginProgress->progress(i, nbNodes / ThreadManager::getNumberOfThreads()) !=
          TLP_CONTINUE) {
        stopfor = true;
      }
    }

    res[i] = compute(i);

    if (!allPaths && norm) {
      TLP_LOCK_SECTION(DIAMETER) {
        if (diameter < res[i])
          diameter = res[i];
      }
      TLP_UNLOCK_SECTION(DIAMETER);
    }
  });

  if (pluginProgress->state() != TLP_CONTINUE)
    return pluginProgress->state() != TLP_CANCEL;

  TLP_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
    if (!allPaths && norm)
      result->setNodeValue(n, res[i] / diameter);
    else
      result->setNodeValue(n, res[i]);
  });

  if (dataSet != nullptr)
    dataSet->set("graph diameter", (!allPaths && norm) ? diameter : double(-1));

  return pluginProgress->state() != TLP_CANCEL;
}
