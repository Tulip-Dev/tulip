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
#ifndef CONNECTEDCOMPONNENTPACKING_H
#define CONNECTEDCOMPONNENTPACKING_H

#include <tulip/TulipPluginHeaders.h>

class ConnectedComponentPacking: public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("Connected Component Packing", "David Auber", "26/05/05", "beta", "1.0", "Misc")
  ConnectedComponentPacking(const tlp::PluginContext* context);
  bool run();

private:
  tlp::Graph *workingGraph;
};

#endif



