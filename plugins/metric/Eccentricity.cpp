/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
  "If true, the closeness centrality is computed (i.e. the average distance from a node to all others).",

  // norm
  "If true, the returned values are normalized. "
  "For the closeness centrality, the reciprocal of the sum of distances is returned. "
  "The eccentricity values are divided by the graph diameter. "
  "<b> Warning : </b> The normalized eccentricity values sould be computed on a (strongly) connected graph.",

  // directed
  "If true, the graph is considered directed."
};

EccentricityMetric::EccentricityMetric(const tlp::PluginContext* context):DoubleAlgorithm(context), allPaths(false), norm(true), directed(false) {
  addInParameter<bool>("closeness centrality",paramHelp[0],"false");
  addInParameter<bool>("norm",paramHelp[1],"true");
  addInParameter<bool>("directed",paramHelp[2],"false");
}
//====================================================================
EccentricityMetric::~EccentricityMetric() {
}
//====================================================================
double EccentricityMetric::compute(unsigned int nPos,
                                   const std::vector<node> &nodes) {

  NodeStaticProperty<unsigned int> distance(graph);
  distance.setAll(0);
  double val = directed ?
               tlp::maxDistance(graph, nPos, distance, DIRECTED) :
               tlp::maxDistance(graph, nPos, distance, UNDIRECTED);

  if(!allPaths)
    return val;

  double nbAcc = 0.;
  val = 0.;
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    unsigned int d = distance[i];

    if (d < nbNodes) {
      nbAcc += 1.;

      if (i!=nPos)
        val += d;
    }
  }

  if(nbAcc<2.0) return 0.0;

  if (norm) val=1.0/val;
  else val/=(nbAcc-1.0);

  return val;
}
//====================================================================
bool EccentricityMetric::run() {
  allPaths = false;
  norm     = true;
  directed = false;

  if (dataSet!=NULL) {
    dataSet->get("closeness centrality", allPaths);
    dataSet->get("norm", norm);
    dataSet->get("directed", directed);
  }

  const std::vector<node>& nodes = graph->nodes();
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

  for (int ni = 0; ni < static_cast<int>(nbNodes) ; ++ni) {
    if (stopfor) continue;

#ifdef _OPENMP

    if (omp_get_thread_num() == 0) {
#endif

      if (pluginProgress->progress(ni , graph->numberOfNodes() / nbThreads )!=TLP_CONTINUE) {
#ifdef _OPENMP
        #pragma omp critical(STOPFOR)
#endif
        stopfor = true;
      }

#ifdef _OPENMP
    }

#endif
    res[ni] = compute(ni, nodes);

    if(!allPaths && norm)
#ifdef _OPENMP
      #pragma omp critical(DIAMETER)
#endif
    {
      if(diameter<res[ni])
        diameter=res[ni];
    }
  }

  for (size_t ni = 0; ni < nbNodes; ++ni) {
    if(!allPaths && norm)
      result->setNodeValue(nodes[ni], res[ni]/diameter);
    else
      result->setNodeValue(nodes[ni], res[ni]);
  }
  if(!allPaths && norm)
      dataSet->set<double>("Graph Diameter", diameter);

  return pluginProgress->state()!=TLP_CANCEL;
}
