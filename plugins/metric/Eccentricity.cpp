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

EccentricityMetric::~EccentricityMetric() {
}
bool EccentricityMetric::check(string &err) {
    return true;
}
bool EccentricityMetric::run() {
  bool allPaths = false;
  bool norm = true;
  bool directed = false;
  
  if (dataSet!=0) {
    dataSet->get("closeness centrality", allPaths);
    dataSet->get("norm", norm);
    dataSet->get("directed", directed);    
  }
  
  MutableContainer<double> nbAccessible;
  nbAccessible.setAll(0);
  
  Iterator<node> *itN= graph->getNodes();
  for (unsigned int i=0; itN->hasNext(); ++i) {
    if (pluginProgress->progress(i,graph->numberOfNodes())!=TLP_CONTINUE) break;
    node n = itN->next();
    MutableContainer<unsigned int> distance;
    distance.setAll(0);
    double val;
    if (directed)
      val = tlp::maxDistance(graph, n, distance, DIRECTED);
    else
      val = tlp::maxDistance(graph, n, distance, UNDIRECTED);

    node n2;
    forEach(n2, graph->getNodes()) {
      if (distance.get(n2) < graph->numberOfNodes())
	nbAccessible.set(n, nbAccessible.get(n) + 1); 
    }
    
    if (allPaths) {
      val = 0;
      node n2;
      forEach(n2, graph->getNodes()) {
	if (distance.get(n2) < graph->numberOfNodes() && n2 != n)
	  val += double( distance.get(n2.id)) / (nbAccessible.get(n) - 1 ) ;
      }
    }
    
    doubleResult->setNodeValue(n, val);
  } delete itN;

  if (norm) {
    node n;
    forEach(n, graph->getNodes()) {
      double val = doubleResult->getNodeValue(n);
      doubleResult->setNodeValue(n, val / (nbAccessible.get(n) - 1 ));
    }
  }
  return pluginProgress->state()!=TLP_CANCEL;
}
