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
#ifndef _STRENGTHMETRIC_H
#define _STRENGTHMETRIC_H
#include <unordered_set>
#include <tulip/TulipPluginHeaders.h>

/** \addtogroup metric */

/** This plugin computes the Strength metric described in :
 *
 *  Y. Chiricota. F.Jourdan, and G.Melancon\n
 *  "Software component capture using graph clustering", \n
 *  "IWPC", \n
 *   2002.
 *
 */
class StrengthMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Strength", "David Auber", "26/02/2003",
      "Computes the Strength metric as described in<br/><b>Software component capture "
      "using graph clustering</b>,<br/>Y. Chiricota. F.Jourdan, an G.Melancon, IWPC "
      "(2002).<br>"
      "doi: <a href=\"https://doi.org/10.1109/WPC.2003.1199205\">10.1109/WPC.2003.1199205</a>",
      "1.0", "Graph")
  StrengthMetric(const tlp::PluginContext *context);
  ~StrengthMetric() override;
  double getNodeValue(const tlp::node n);
  double getEdgeValue(const tlp::edge n);
  bool run() override;

private:
  double e(const std::unordered_set<tlp::node> &U);
  double e(std::unordered_set<tlp::node> &U, std::unordered_set<tlp::node> &V);
  double s(std::unordered_set<tlp::node> &U, std::unordered_set<tlp::node> &V);
  double s(const std::unordered_set<tlp::node> &U);
};

#endif
