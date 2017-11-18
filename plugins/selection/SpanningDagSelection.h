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
#ifndef Tulip_SpanningDagSelection_H
#define Tulip_SpanningDagSelection_H

#include <tulip/BooleanProperty.h>
#include "../utils/PluginNames.h"

/** \addtogroup selection */

/**
 * This selection plugin enables to find a subgraph of G that is acyclic.
 *
 *  \author David Auber, LaBRI University of Bordeaux, France:
 *   auber@labri.fr
 */
class SpanningDagSelection : public tlp::BooleanAlgorithm {

public:
  PLUGININFORMATION(tlp::SelectionAlgorithm::SpanningDagSelection, "David Auber", "01/12/1999",
                    "Selects an acyclic subgraph of a graph.", "1.0", "Selection")
  SpanningDagSelection(const tlp::PluginContext *context);
  bool run();
};

#endif
