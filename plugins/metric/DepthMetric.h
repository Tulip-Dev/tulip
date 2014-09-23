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
#ifndef _DEPTHMETRIC_H
#define _DEPTHMETRIC_H

#include <tulip/TulipPluginHeaders.h>


/** \addtogroup metric */

/** This plugins compute for each node n, the maximum path-length between n and the other node.
 *  The graph must be acyclic.
 *
 *  \note This algorithm works on general graphs.
 *
 */
class DepthMetric :public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Depth","David Auber","15/02/2001","Compute for each node n on an acyclic graph,\nthe maximum path-length between n and the other node.","1.1","Hierarchical")
  DepthMetric(const tlp::PluginContext* context);
  bool run();
  bool check(std::string &);
private:
  double getNodeValue(const tlp::node n);
  tlp::NumericProperty* edgeWeight;
};

#endif
