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
#include "ClusterMetric.h"

#include <tulip/StaticProperty.h>
#include <tulip/GraphMeasure.h>
#include <tulip/SimpleTest.h>

PLUGIN(ClusterMetric)

using namespace std;
using namespace tlp;

ClusterMetric::ClusterMetric(const tlp::PluginContext *context) : DoubleAlgorithm(context) {
  addOutParameter<double>(
      "average clustering coefficient",
      "Average value of the local clustering coefficient associated to the nodes");
}

bool ClusterMetric::check(string &err) {
  if (!SimpleTest::isSimple(graph, false)) {
    err = "The graph is not simple.";
    return false;
  }
  return true;
}

//=================================================
bool ClusterMetric::run() {
  tlp::NodeStaticProperty<double> clusters(graph);
  clusteringCoefficient(graph, clusters);

  clusters.copyToProperty(result);

  // compute average
  double sum = 0;
  for (auto v : clusters) {
    sum += v;
  }
  dataSet->set("average clustering coefficient", sum / graph->numberOfNodes());

  return true;
}
