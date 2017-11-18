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
#ifndef Tulip_SPANNINGTREESELECTION_H
#define Tulip_SPANNINGTREESELECTION_H

#include <tulip/BooleanProperty.h>

/** \addtogroup selection */

/**
 * This selection plugins enables to find a subgraph of G that is a forest (a set of trees).
 *
 *  \author David Auber, LaBRI University Bordeaux I France:
 *   auber@labri.fr
 */
class SpanningTreeSelection : public tlp::BooleanAlgorithm {
public:
  PLUGININFORMATION("Spanning Forest", "David Auber", "01/12/1999",
                    "Selects a subgraph of a graph that is a forest (a set of trees).", "1.0",
                    "Selection")
  SpanningTreeSelection(const tlp::PluginContext *context);
  ~SpanningTreeSelection();
  bool run();
};

#endif
