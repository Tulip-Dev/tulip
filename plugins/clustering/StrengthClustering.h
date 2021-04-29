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
#ifndef _StrengthClustering_H
#define _StrengthClustering_H

#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <tulip/TulipPluginHeaders.h>

/** \addtogroup clustering */

/**
 * This plugin performs a single-linkage clustering.
 * The similarity measure used here is the Strength Metric computed on edges.
 * The best threshold is found using MQ Quality Measure.
 * See:
 *
 * Y. Chiricota, F. Jourdan and G. Melancon, G. \n
 * "Software components capture using graph clustering", \n
 * IWPC '03: Proceedings of the 11th IEEE International Workshop on Program Comprehension, 2003
 *
 */
class StrengthClustering : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Strength Clustering", "David Auber", "27/01/2003",
      "Implements a single-linkage clustering. The similarity measure used here is "
      "the Strength Metric computed on edges. The best threshold is found using MQ "
      "Quality Measure. See:<br/>"
      "<b>Software component capture using graph clustering</b>, "
      "Y. Chiricota, F.Jourdan, and G. Melancon, "
      "IWPC '03: Proceedings of the 11th IEEE International Workshop on Program Comprehension",
      "2.0", "Clustering")
  StrengthClustering(tlp::PluginContext *context);
  ~StrengthClustering() override;
  bool run() override;
  bool check(std::string &) override;

private:
  void computeNodePartition(double threshold, std::vector<std::unordered_set<tlp::node>> &,
                            tlp::DoubleProperty &);
  double computeMQValue(const std::vector<std::unordered_set<tlp::node>> &partition, tlp::Graph *);
  double findBestThreshold(int numberOfSteps, bool &stopped, tlp::DoubleProperty &values);
};

#endif
