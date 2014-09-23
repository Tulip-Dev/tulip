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

#ifndef REVERSEEDGES_H
#define REVERSEEDGES_H

#include <tulip/Algorithm.h>

class ReverseEdges:public tlp::Algorithm {
public:
  PLUGININFORMATION("Reverse edges", "Ludwig Fiolka", "10/10/2012", "Reverse selected/all edges on the current graph", "0.1", "Topology Update")

  ReverseEdges(tlp::PluginContext* context);

  bool run();
};

#endif // REVERSEEDGES_H



