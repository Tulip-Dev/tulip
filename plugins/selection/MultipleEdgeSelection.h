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
#ifndef Tulip_ParaSelection_H
#define Tulip_ParaSelection_H

#include <tulip/TulipPluginHeaders.h>
/** \addtogroup selection */


/**
 * This selection plugins enables to find the multiple-edges and parallel-edges in a graph.
 * Let e1, e2 two edges, e1 and e2 are parallel edges if source(e1)=source(e2) and
 * target(e1) = target(e2).
 * If it exists n edges between two nodes, only n-1 edges will be selected.
 *
 *  \author David Auber, LaBRI University Bordeaux I France:
 *   auber@tulip-software.org
 */
class MultipleEdgeSelection:public tlp::BooleanAlgorithm {
public:
  PLUGININFORMATION("Multiple Edge","David Auber","20/01/2003","Alpha","1.0", "Selection")
  MultipleEdgeSelection(const tlp::PluginContext* context);
  bool run();
};

#endif




