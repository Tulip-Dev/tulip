/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#include <vector>
#include <tulip/TulipPlugin.h>

class ConnectedComponentPacking: public tlp::LayoutAlgorithm {
public:
    ConnectedComponentPacking(const tlp::PropertyContext&);
    bool run();

private: 
    tlp::Graph *workingGraph;
};

#endif



