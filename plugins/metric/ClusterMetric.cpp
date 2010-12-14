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
#include "ClusterMetric.h"

DOUBLEPLUGINOFGROUP(ClusterMetric,"Cluster","David Auber","26/02/2003","Beta","1.0","Graph");

using namespace std;
using namespace tlp;

namespace {
  const char * paramHelp[] = {
    // property
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "values", "[1 , |G|]" ) \
    HTML_HELP_DEF( "default", "1" ) \
    HTML_HELP_BODY() \
    "This parameter defines the maximal depth of a computed cluster." \
    HTML_HELP_CLOSE()
  };
}
//=================================================
ClusterMetric::ClusterMetric(const tlp::PropertyContext &context):DoubleAlgorithm(context) {
  addParameter<unsigned int>("depth",paramHelp[0],"1");
}
//=================================================
static double clusterGetEdgeValue(Graph *graph,
				  MutableContainer<double>& clusters,
				  const edge e ) {
  pair<node, node> eEnds = graph->ends(e);
  const double& v1 = clusters.get(eEnds.first.id);
  const double& v2 = clusters.get(eEnds.second.id);
  if (v1*v1 + v2*v2 > 0)
    return 1.- fabs(v1 - v2)/sqrt(v1*v1 + v2*v2);
  return 0.;
}
//=================================================
bool ClusterMetric::run() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  unsigned int maxDepth = 1;
  if (dataSet!=0) dataSet->get("depth", maxDepth);
  MutableContainer<double> clusters;
  clusteringCoefficient(graph, clusters, maxDepth, pluginProgress);
  node n;
  forEach(n, graph->getNodes())
    doubleResult->setNodeValue(n, clusters.get(n.id));
  edge e;
  forEach(e, graph->getEdges())
    doubleResult->setEdgeValue(e, clusterGetEdgeValue(graph, clusters, e));
  return true;
}

