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
#ifndef _StrengthClustering_H
#define _StrengthClustering_H

#include <string>
#include <set>
#include <vector>
#include <map>
#include <tulip/TulipPluginHeaders.h>

/** \addtogroup clustering */

/**
* This plugin performs a single-linkage clustering.
* The similarity measure used here is the Strength Metric computed on edges.
* The best threshold is found using MQ Quality Measure.
* See :
*
* Y. Chiricota, F. Jourdan and G. Melancon, G. \n
* "Software components capture using graph clustering", \n
* IEEE Computer Society, \n
* 2003.
*
*/
class StrengthClustering:public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Strength Clustering", "David Auber", "27/01/2003", "Alpha", "2.0","Clustering")
  StrengthClustering(tlp::PluginContext* context);
  ~StrengthClustering();
  bool run();
  bool check(std::string &);
private:
  void computeNodePartition(double threshold,
                            std::vector< std::set<tlp::node> >&);
  double computeMQValue(const std::vector< std::set<tlp::node> >& partition,
                        tlp::Graph *);
  double findBestThreshold(int numberOfSteps, bool& stopped);
  tlp::DoubleProperty* values;
};

#endif
