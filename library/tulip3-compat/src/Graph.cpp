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
#include <tulip3/Graph.h>
#include <tulip/Graph.h>

bool getSource(tlp::Graph *g, tlp::node &n) {
  n = g->getSource();
  return n.isValid();
}

bool applyAlgorithm(tlp::Graph *graph, std::string &errorMessage, tlp::DataSet *dataSet,const std::string &algorithm, tlp::PluginProgress *progress) {
  return graph->applyAlgorithm(errorMessage, dataSet, algorithm, progress);
}
