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
#ifndef Tulip_LoopSelection_H
#define Tulip_LoopSelection_H

#include <tulip/BooleanProperty.h>
#include "../utils/PluginNames.h"

/** \addtogroup selection */

/**
 * This selection plugins enables to find loops in a graph
 * A loop is an edge that has the same source and target.
 *
 *  \author David Auber, LaBRI University Bordeaux I France:
 *   auber@labri.fr
 */
class LoopSelection : public tlp::BooleanAlgorithm {
public:
  PLUGININFORMATION(
      tlp::SelectionAlgorithm::LoopSelection, "David Auber", "20/01/2003",
      "Selects loops in a graph.<br/>A loop is an edge that has the same source and target.", "1.0",
      "Selection")
  LoopSelection(const tlp::PluginContext *context);
  bool run();
};

#endif
