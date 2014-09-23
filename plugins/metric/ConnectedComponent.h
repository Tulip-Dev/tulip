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
#ifndef _ConnectedComponent_H
#define _ConnectedComponent_H

#include <tulip/TulipPluginHeaders.h>

/** \addtogroup metric */

/** This plugin is an implementation of the connected component decompostion algorithm. each
 *  node and edge that belongs to the same component receive the same value.
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the same
 *  connected component they have the same value else they have a different value.
 *
 */
class ConnectedComponent:public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Connected Component","David Auber","01/07/2002","Implements the connected component decompostion.","1.0","Component")
  ConnectedComponent(const tlp::PluginContext* context);
  ~ConnectedComponent();
  bool run();
};

#endif
