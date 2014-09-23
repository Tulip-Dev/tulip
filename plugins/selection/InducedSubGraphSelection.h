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
#ifndef Tulip_InducedSubGraphSelection_H
#define Tulip_InducedSubGraphSelection_H

#include <tulip/TulipPluginHeaders.h>

/** \addtogroup selection */

/// This selection plugins enables to find a subgraph induced by a set of nodes.
/**
 * Let V' a subset of nodes, G' is an induced subgraph of G if all edges
 * (and only these ones) element of G that link
 * two nodes (only one in case of loop) of V' are in G'.
 *
 *  \author David Auber, LaBRI University Bordeaux I France:
 *   auber@tulip-software.org
 */
class InducedSubGraphSelection:public tlp::BooleanAlgorithm {
public:
  PLUGININFORMATION("Induced Sub-Graph","David Auber","08/08/2001","Alpha","1.0", "Selection")
  InducedSubGraphSelection(const tlp::PluginContext* context);
  bool run();
};

#endif




