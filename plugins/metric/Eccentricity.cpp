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
#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/ConnectedTest.h>
#include <tulip/StaticProperty.h>
#include <tulip/GraphMeasure.h>

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
    "<b> Warning : </b> The normalized eccentricity values sould be computed on a (strongly) "
    "connected graph.",

    // directed
    "If true, the graph is considered directed."};

EccentricityMetric::EccentricityMetric(const tlp::PluginContext *context)
    : DoubleAlgorithm(context), allPaths(false), norm(true), directed(false) {
  addInParameter<bool>("closeness centrality", paramHelp[0], "false");
  addInParameter<bool>("norm", paramHelp[1], "true");
  addInParameter<bool>("directed", paramHelp[2], "false");
}
//====================================================================
EccentricityMetric::~EccentricityMetric() {}
//====================================================================
double EccentricityMetric::compute(const node &n) {

  NodeStaticProperty<unsigned int> distance(graph);
  distance.setAll(0);
  double val = directed ? tlp::maxDistance(graph, n, distance, DIRECTED)
                        : tlp::maxDistance(graph, n, distance, UNDIRECTED);

  if (!allPaths)
    return val;

  double nbAcc = 0.;
  val = 0.;
  unsigned int nbNodes = graph->numberOfNodes();

  for (const node &n2 : graph->nodes()) {
    unsigned int d = distance[n];

    if (d < nbNodes) {
      nbAcc += 1.;

      if (n2 != n)
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

  if (dataSet != nullptr) {
    dataSet->get("closeness centrality", allPaths);
    dataSet->get("norm", norm);
    dataSet->get("directed", directed);
  }

  const std::vector<node> &nodes = graph->nodes();
  NodeStaticProperty<double> res(graph);
  //  omp_set_num_threads(4);

  size_t nbNodes = nodes.size();
#ifdef _OPENMP
  int nbThreads = omp_get_num_procs();
#else
  unsigned int nbThreads = 1;
#endif

  //  double t1 = omp_get_wtime();
  double diameter = 1.0;
  bool stopfor = false;
#ifdef _OPENMP
#pragma omp parallel for
#endif

  for (OMP_ITER_TYPE i = 0; i < static_cast<OMP_ITER_TYPE>(nbNodes); ++i) {
    if (stopfor)
      continue;

#ifdef _OPENMP

    if (omp_get_thread_num() == 0) {
#endif

      if (pluginProgress->progress(i, graph->numberOfNodes() / nbThreads) != TLP_CONTINUE) {
#ifdef _OPENMP
#pragma omp critical(STOPFOR)
#endif
        stopfor = true;
      }

#ifdef _OPENMP
    }

#endif
    res[nodes[i]] = compute(nodes[i]);

    if (!allPaths && norm)
#ifdef _OPENMP
#pragma omp critical(DIAMETER)
#endif
    {
      if (diameter < res[nodes[i]])
        diameter = res[nodes[i]];
    }
  }

  for (const node &n : nodes) {
    if (!allPaths && norm)
      result->setNodeValue(n, res[n] / diameter);
    else
      result->setNodeValue(n, res[n]);
  }

  if (!allPaths && norm)
    dataSet->set("Graph Diameter", diameter);

  return pluginProgress->state() != TLP_CANCEL;
}
