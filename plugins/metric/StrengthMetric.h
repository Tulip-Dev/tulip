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
#ifndef _STRENGTHMETRIC_H
#define _STRENGTHMETRIC_H
#include <tulip/tuliphash.h>
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
class StrengthMetric:public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Strength","David Auber","26/02/2003","Computes the Strength metric described in\nY. Chiricota. F.Jourdan, an G.Melancon\n'Software component capture using graph clustering',\nIWPC 2002","1.0","Graph")
  StrengthMetric(const tlp::PluginContext* context);
  ~StrengthMetric();
  double getNodeValue(const tlp::node n);
  double getEdgeValue(const tlp::edge n);
  bool run();
private:
  double e(const TLP_HASH_SET<tlp::node> &U);
  double e(TLP_HASH_SET<tlp::node> &U, TLP_HASH_SET<tlp::node> &V);
  double s(TLP_HASH_SET<tlp::node> &U, TLP_HASH_SET<tlp::node> &V);
  double s(const TLP_HASH_SET<tlp::node> &U);
};

#endif
