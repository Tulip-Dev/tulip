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
#ifndef _NODEMETRIC_H
#define _NODEMETRIC_H

#include <tulip/TulipPluginHeaders.h>




/** \addtogroup metric */

/** This plugin computes the number of nodes in the subtree induced by each node.
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the same
 *  connected component they have the same value else they have a
 *  different value.
 *
 */
class NodeMetric:public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Node","David Auber","20/12/1999","Computes the number of nodes in the subtree\ninduced by each node of an acyclic graph.","1.0","Hierarchical")
  NodeMetric(const tlp::PluginContext* context);
  bool run();
  bool check(std::string &);
private:
  double getNodeValue(const tlp::node n);
};

#endif

