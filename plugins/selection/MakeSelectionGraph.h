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

#ifndef MAKESELECTIONGRAPH_H
#define MAKESELECTIONGRAPH_H

#include <tulip/BooleanProperty.h>
#include <tulip/GraphTest.h>
#include "../utils/PluginNames.h"

/** \addtogroup selection */

/**
 * This selection plugin extends the current selection to a graph.
 * All selected edges will also have their extremities selected (no dangling edges).
 *
 *  \author Bruno Pinaud, LaBRI University of Bordeaux France:
 */

class MakeSelectionGraph : public tlp::BooleanAlgorithm {
public:
  MakeSelectionGraph(const tlp::PluginContext *context);
  PLUGININFORMATION(tlp::SelectionAlgorithm::MakeSelectionGraph, "Bruno Pinaud", "28/11/2016",
                    "Extends the selection to have a graph.<br/>"
                    "All selected edges of the current graph will have their extremities selected "
                    "(no dangling edges).",
                    "1.0", "Selection")
  bool run() override;
};

class isGraphTest : public tlp::GraphTest {
  PLUGININFORMATION("Graph", "Bruno Pinaud", "29/11/2016", "Tests whether the set of the selected "
                                                           "elements of the current graph is a "
                                                           "graph or not (no dangling edges).",
                    "1.0", "Topological Test")
public:
  isGraphTest(const tlp::PluginContext *context);
  bool test() override;
};

#endif // MAKESELECTIONGRAPH_H
