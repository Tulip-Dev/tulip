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

#ifndef DFS_H_
#define DFS_H_

#include <float.h>
#include <vector>

#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/MutableContainer.h>

#include "../PathAlgorithm.h"

namespace tlp {
class BooleanProperty;
class DoubleProperty;
class Graph;


/**
 * @brief An implementation of the DFS algorithm to find all the existing paths between two nodes.
 */
class DFS: public PathAlgorithm {
public:
  /**
   * Initiate the DFS algorithm parameters.
   * @param result The boolean property where to store the results.
   * @param dists A working double property where distances between nodes and the target node will be stored.
   * @param tgt the target node.
   * @param weights the edges weights.
   * @param edgesOrientation The edge orientation policy.
   * @param maxDist The maximal distance the algorithm can go from the source node before dropping the search (DBL_MAX by default)
   */
  DFS(tlp::Graph *graph, tlp::BooleanProperty *result, tlp::DoubleProperty *dists, tlp::node tgt, tlp::MutableContainer<double> &weights, EdgeOrientation edgesOrientation, double maxDist=DBL_MAX);

  ~DFS();

  /**
   * Compute the path between the source node and the target node. Caution ! This method is recursive and could cause a stack overflow on big graphs.
   * @param src The source node.
   * @return true if a path has been found.
   */
  bool searchPaths(tlp::node src);

private:
  tlp::Graph *graph;
  tlp::BooleanProperty *result;
  tlp::DoubleProperty *dists;
  tlp::BooleanProperty *visitable;
  tlp::node tgt;
  tlp::MutableContainer<double> *weights;
  std::vector<tlp::edge> path;
  double currentDist;
  EdgeOrientation edgesOrientation;
  double maxDist;
};
}
#endif /* DFS_H_ */
