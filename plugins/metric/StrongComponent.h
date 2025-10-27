/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef _STRONGCOMPONENT_H
#define _STRONGCOMPONENT_H

#include <stack>
#include <tulip/tuliphash.h>
#include <tulip/DoubleProperty.h>

/** This plugin is an implementation of a strongly connected components decomposition.
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the
 * same
 *  strongly connected component they have the same value else they have a
 *  different value.
 *
 */
class StrongComponent : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Strongly Connected Components", "David Auber", "12/06/2001",
                    "Implements a strongly connected components decomposition.<br/> \
                    If two nodes are in the same strongly connected component, they have the same value; if not, they have a different value.<br/> \
                    Edges between nodes in the same component have the same value as nodes.<br/> \
                    Those between nodes of different components have a null value.<br/> \
                    In the undirected case, these later ones are the disconnecting edges (also called bridges) of the graph.",
                    "1.1", "Component")
  StrongComponent(const tlp::PluginContext *context);
  ~StrongComponent() override;
  bool run() override;

private:
  unsigned curComponent, curTS;
  void findSCC(tlp::node, tlp_hash_map<tlp::node, bool> &, tlp_hash_map<tlp::node, unsigned> &,
               std::stack<tlp::node> &);
  void findDE(tlp::node, std::vector<tlp::edge> &, tlp_hash_map<tlp::node, unsigned int> &,
              tlp_hash_map<tlp::node, unsigned int> &);
};

#endif
