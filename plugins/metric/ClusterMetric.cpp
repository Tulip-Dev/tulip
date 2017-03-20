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
#include "ClusterMetric.h"

PLUGIN(ClusterMetric)

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // depth
    "Maximal depth of a computed cluster."};
//=================================================
ClusterMetric::ClusterMetric(const tlp::PluginContext *context) : DoubleAlgorithm(context) {
  addInParameter<unsigned int>("depth", paramHelp[0], "1");
}
//=================================================
static double clusterGetEdgeValue(Graph *graph, tlp::NodeStaticProperty<double> &clusters, const edge e) {
  pair<node, node> eEnds = graph->ends(e);
  const double v1 = clusters.getNodeValue(eEnds.first);
  const double v2 = clusters.getNodeValue(eEnds.second);

  double sum = v1 * v1 + v2 * v2;
  if (sum)
    return 1. - fabs(v1 - v2) / sqrt(sum);

  return 0.;
}
//=================================================
bool ClusterMetric::run() {
  //  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  unsigned int maxDepth = 1;

  if (dataSet != nullptr)
    dataSet->get("depth", maxDepth);

  tlp::NodeStaticProperty<double> clusters(graph);
  clusteringCoefficient(graph, clusters, maxDepth, pluginProgress);
  const std::vector<node> &nodes = graph->nodes();
  for (unsigned int i = 0; i < nodes.size(); ++i)
    result->setNodeValue(nodes[i], clusters[i]);

  const std::vector<edge> &edges = graph->edges();
  for (unsigned int i = 0; i < edges.size(); ++i) {
    edge e = edges[i];
    result->setEdgeValue(e, clusterGetEdgeValue(graph, clusters, e));
  }
  return true;
}
