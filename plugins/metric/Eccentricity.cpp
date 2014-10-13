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
#include <deque>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <tulip/ConnectedTest.h>
#include "Eccentricity.h"

PLUGIN(EccentricityMetric)

using namespace std;
using namespace tlp;

namespace {
const char * paramHelp[] = {
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "values", "[true , false]" ) \
  HTML_HELP_DEF( "default", "false" ) \
  HTML_HELP_BODY() \
  "If true, the closeness centrality is computed (i.e. the average distance from the node to all others)."\
  HTML_HELP_CLOSE(),
  // norm
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "values", "[true , false]" ) \
  HTML_HELP_DEF( "default", "false" ) \
  HTML_HELP_BODY() \
  "If true, the returned values are normalized. " \
  "For the closeness centrality, the reciprocal of the sum of distances is returned. " \
  "The eccentricity values are divided by the graph diameter. " \
  "<b> Warning : </b> The normalized eccentricity values sould be computed on a (strongly) connected graph."
  HTML_HELP_CLOSE(),
  // directed
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "values", "[true , false]" ) \
  HTML_HELP_DEF( "default", "false" ) \
  HTML_HELP_BODY() \
  "If true, the graph is considered directed."\
  HTML_HELP_CLOSE(),
};
}

EccentricityMetric::EccentricityMetric(const tlp::PluginContext* context):DoubleAlgorithm(context), allPaths(false), norm(true), directed(false) {
  addInParameter<bool>("closeness centrality",paramHelp[0],"false");
  addInParameter<bool>("norm",paramHelp[1],"true");
  addInParameter<bool>("directed",paramHelp[2],"false");
}
//====================================================================
EccentricityMetric::~EccentricityMetric() {
}
//====================================================================
double EccentricityMetric::compute(node n) {

  MutableContainer<unsigned int> distance;
  distance.setAll(0);
  double val;

  val = directed ?
        tlp::maxDistance(graph, n, distance, DIRECTED) :
        tlp::maxDistance(graph, n, distance, UNDIRECTED);

  if(!allPaths)
    return val;

  double nbAcc = 0.;
  node n2;
  val = 0.;
  forEach(n2, graph->getNodes()) {
    if (distance.get(n2.id) < graph->numberOfNodes()) {
      nbAcc += 1.;

      if(n2!=n)
        val += double(distance.get(n2.id)) ;
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

  node n;
  size_t i = 0;
  vector<node> vecNodes(graph->numberOfNodes());
  vector<double> res(graph->numberOfNodes());
  forEach(n, graph->getNodes()) {
    vecNodes[i] = n;
    ++i;
  }
//  omp_set_num_threads(4);

  size_t nbElem = vecNodes.size();
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

  for (int ni = 0; ni < static_cast<int>(nbElem) ; ++ni) {
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
    res[ni] = compute(vecNodes[ni]);

    if(!allPaths && norm)
#ifdef _OPENMP
      #pragma omp critical(DIAMETER)
#endif
    {
      if(diameter<res[ni])
        diameter=res[ni];
    }
  }

  for (size_t ni = 0; ni < nbElem; ++ni) {
    if(!allPaths && norm)
      result->setNodeValue(vecNodes[ni], res[ni]/diameter);
    else
      result->setNodeValue(vecNodes[ni], res[ni]);
  }

  /*
    double t2 = omp_get_wtime();
    for (size_t ni = 0; ni < nbElem; ++ni) {
      double val = compute(vecNodes[ni]);
      result->setNodeValue(vecNodes[ni], val);
    }
    double t3 = omp_get_wtime();
    tlp::debug() << "omp : " << t2 - t1 << "s" << endl << flush;
    tlp::debug() << "sng : " << t3 - t2 << "s" << endl << flush;
  */
  /*
  Iterator<node> *itN = graph->getNodes();
  for (unsigned int i=0; itN->hasNext(); ++i) {
    if (pluginProgress->progress(i, graph->numberOfNodes())!=TLP_CONTINUE) break;
    node n = itN->next();
    result->setNodeValue(n, compute(n));
  }
  */
  return pluginProgress->state()!=TLP_CANCEL;
}
