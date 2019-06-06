/*
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

#ifndef TULIP_TRICONNECTEDTEST_H
#define TULIP_TRICONNECTEDTEST_H

#include <tulip/tulipconf.h>

namespace tlp {

class Graph;

/**
 * @ingroup Checks
 * @brief Provides functions to test if a graph is triconnected.
 **/
class TLP_SCOPE TriconnectedTest {
public:
  /**
   * @brief Checks if the graph is triconnected.
   * Creates a clone sugraph in which to operate, then iterates over the nodes, and deletes them.
   * Once the node is deleted, checks if the graph is biconnected.
   * If it is not, then the graph is not triconnected.
   * If it is, adds back the node and its edges.
   *
   * @param graph The graph to check is triconnected.
   * @return bool True if the graph is triconnected, false otherwise.
   **/
  static bool isTriconnected(Graph *graph);
};
} // namespace tlp
#endif
