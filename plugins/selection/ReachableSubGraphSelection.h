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
#ifndef Tulip_REACHABLESUBGRAPHELECTION_H
#define Tulip_REACHABLESUBGRAPHELECTION_H

#include <tulip/BooleanProperty.h>
#include "../utils/PluginNames.h"

/** \addtogroup selection */

/**
 * This selection plugin enables to find all nodes and edges at a given distance of one of the node
 * of a given set of nodes.
 * It takes three parameters :
 * - <b>int</b> distance
 * - <b>StringCollection</b> edge direction : output edges , input edges , all edges
 * - <b>BooleanProperty *</b> startingnodes : the selected nodes of this BooleanProperty
 * (those whose associated value is true) will be used as starting nodes.
 *
 *  \author David Auber, LaBRI University Bordeaux I France:
 *   auber@labri.fr
 */
class ReachableSubGraphSelection : public tlp::BooleanAlgorithm {
public:
  PLUGININFORMATION(tlp::SelectionAlgorithm::ReachableSubGraphSelection, "David Auber",
                    "01/12/1999", "Selects all nodes and edges at a maximum distance of 1 of the "
                                  "node of a given set of selected nodes.",
                    "1.1", "Selection")

  ReachableSubGraphSelection(const tlp::PluginContext *context);
  ~ReachableSubGraphSelection();
  bool run();
};

#endif
