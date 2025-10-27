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
#ifndef _CLUSTERMETRIC_H
#define _CLUSTERMETRIC_H

#include <tulip/DoubleProperty.h>

class ClusterMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Cluster", "Bruno Pinaud", "03/10/2023",
      "This plugin computes the local clustering coefficient and its average value for the whole graph as described in: "
      "Watts, D., Strogatz, S. Collective dynamics of ‘small-world’ networks."
      " Nature 393, 440–442 (1998).<br> <a href=\"https://doi.org/10.1038/30918\">https://doi.org/10.1038/30918</a><br> "
      "(note: these algorithms work on general simple graphs).",
      "2.0", "Graph")
  ClusterMetric(const tlp::PluginContext *context);
  bool run() override;
  bool check(std::string &err) override;
};

#endif
