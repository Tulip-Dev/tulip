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

#ifndef _PATHLENGTHMETRIC_H
#define _PATHLENGTHMETRIC_H

#include <tulip/TulipPluginHeaders.h>

class PathLengthMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Path Length", "David Auber", "15/02/2001",
                    "Assigns to each node the number of paths that goes through it.<br/><b>The "
                    "graph must be acyclic</b>.",
                    "1.0", "Hierarchical")
  PathLengthMetric(const tlp::PluginContext *context);
  bool run() override;
  bool check(std::string &) override;

private:
  double getNodeValue(const tlp::node n, tlp::DoubleProperty *leafMetric);
};

#endif
