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
#ifndef _BUBBLE_CIRCULARLAYOUT_H
#define _BUBBLE_CIRCULARLAYOUT_H

#include <tulip/TulipPluginHeaders.h>

/** \addtogroup layout */

/**
 * This plugin is an implementation of a circular layout that takes
 * node size into account.  It manages size of nodes and use a standard dfs
 * for ordering nodes or search the maximum length cycle.
 *
 * HISTORY:
 *
 * 25/11/2004 version 0.0.2: size and order
 * 01/12/1999 Version 0.0.1: Initial release
 *
 *
 * NOTES:
 *
 * This work on general graphs.
 * Let n be the number of nodes, the algorithm complexity is in O(n). If the search_cycle is enabled,
 * the complexity is exponential (NP-Complete problem !).
 *
 * AUTHORS:
 *
 * David Auber and Romain Bourqui University Bordeaux I France: Email:auber@tulip-software.org
 * Daniel Archambault The University of British Columbia:
 * Email:  archam@cs.ubc.ca
 */
class Circular:public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("Circular","David Auber/ Daniel Archambault","25/11/2004","Ok","1.1","Basic")
  Circular(const tlp::PluginContext* context);
  bool run();
};

#endif
