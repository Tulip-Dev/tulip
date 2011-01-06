/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

DOUBLEPLUGINOFGROUP(EccentricityMetric,"Eccentricity","Auber/Munzner","18/06/2004","Alpha","1.0","Graph");

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
    "If true the eccentricity will be computed using all path between a node to the other (not only the maximum one, ie radiality)"\
    HTML_HELP_CLOSE(),
  };
}

EccentricityMetric::EccentricityMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) {
  addParameter<bool>("closeness centrality",paramHelp[0],"false");
  addParameter<bool>("norm",paramHelp[0],"true");
  addParameter<bool>("directed",paramHelp[0],"false");  
}
//====================================================================
EccentricityMetric::~EccentricityMetric() {
}
//====================================================================
bool EccentricityMetric::check(string &err) {
    return true;
}
//====================================================================
double EccentricityMetric::compute(node n) {
//    cout << "t_" << omp_get_thread_num() << "[" << n.id << "] start" << flush << endl;
    MutableContainer<unsigned int> distance;
    distance.setAll(0);
    double val;
    double nbAcc = 0.;    
    if (directed)
      val = tlp::maxDistance(graph, n, distance, DIRECTED);
    else
      val = tlp::maxDistance(graph, n, distance, UNDIRECTED);

    node n2;
    forEach(n2, graph->getNodes()) {
      if (distance.get(n2) < graph->numberOfNodes())
	nbAcc += 1.;
    }
    
    if (allPaths) {
      val = 0.;
      node n2;
      forEach(n2, graph->getNodes()) {
	if (distance.get(n2) < graph->numberOfNodes() && n2 != n)
	  val += double( distance.get(n2.id) ) / (nbAcc - 1 ) ;
      }
    }
  if (norm) val /= nbAcc;
//    cout << "t_" << omp_get_thread_num() << "[" << n.id << "] end" << flush << endl;

  return val;
}
//====================================================================
bool EccentricityMetric::run() {
  allPaths = false;
  norm     = true;
  directed = false;
    if (dataSet!=0) {
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
  unsigned int nbThreads = 1;
#ifdef _OPENMP
  nbThreads = omp_get_num_procs();
#endif

//  double t1 = omp_get_wtime();
   bool stopfor = false;
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (size_t ni = 0; ni < nbElem; ++ni) {
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
      }
      res[ni] = compute(vecNodes[ni]);
  }
  for (size_t ni = 0; ni < nbElem; ++ni) {
    doubleResult->setNodeValue(vecNodes[ni], res[ni]);
  }
/*
  double t2 = omp_get_wtime();
  for (size_t ni = 0; ni < nbElem; ++ni) {
    double val = compute(vecNodes[ni]);
    doubleResult->setNodeValue(vecNodes[ni], val);    
  }
  double t3 = omp_get_wtime();
  cout << "omp : " << t2 - t1 << "s" << endl << flush;
  cout << "sng : " << t3 - t2 << "s" << endl << flush;
*/
  /*
  Iterator<node> *itN = graph->getNodes();
  for (unsigned int i=0; itN->hasNext(); ++i) {
    if (pluginProgress->progress(i, graph->numberOfNodes())!=TLP_CONTINUE) break;
    node n = itN->next();
    doubleResult->setNodeValue(n, compute(n));
  }
  */
  return pluginProgress->state()!=TLP_CANCEL;
}
