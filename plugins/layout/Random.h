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
#ifndef _RANDOMLAYOUT_H
#define _RANDOMLAYOUT_H

#include <tulip/TulipPluginHeaders.h>

/// Random.h - An implementation of a random 3D layout.
/** This plugin is an implementation of a random 3D layout
 *
 *  <b>HISTORY</b>
 *
 *  01/12/99 Verson 0.0.1: Initial release
 *
 *
 *  \note This algorithm works on general graphs. \n
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *
 *  \author David Auber University Bordeaux I France, Email : auber@tulip-software.com
 */
class Random:public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("Random layout","David Auber","01/12/1999","Ok","1.1","Basic")
  Random(const tlp::PluginContext* context);

  bool run();
};

#endif

