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

#ifndef QUADTREE_H
#define QUADTREE_H
#include <tulip/tuliphash.h>
#include <vector>
#include <tulip/Graph.h>
#include <tulip/Vector.h>
#include <tulip/DrawingTools.h>

class QuadTreeBundle {
public:
  static void compute(tlp::Graph *graph, double splitRatio, tlp::LayoutProperty *layout = nullptr,
                      tlp::SizeProperty *size = nullptr);
  //=====================================
  static bool isIn(const tlp::Coord &p, const tlp::Coord &a, const tlp::Coord &b);

protected:
  void createQuadTree(tlp::Graph *graph, tlp::LayoutProperty *layout = nullptr,
                      tlp::SizeProperty *size = nullptr);

private:
  double minSize;
  double nbNodesInOriginalGraph;
  std::vector<tlp::node> resultNode;
  tlp::LayoutProperty *layout;
  tlp::SizeProperty *size;
  tlp::DoubleProperty *rot;
  tlp::Graph *graph;
  double splitRatio;
  typedef tlp::Vec2d Vec2D;
  typedef tlp_hash_map<Vec2D, tlp::node> MapVecNode;
  MapVecNode mapN;
  //=====================================
  tlp::node splitEdge(tlp::node a, tlp::node b);
  //=====================================
  void elmentSplitting(const tlp::Coord &a, const tlp::Coord &b,
                       const std::vector<tlp::node> &input, std::vector<tlp::node> &in,
                       std::vector<tlp::node> &out);
  //=====================================
  void recQuad(const tlp::node a, const tlp::node b, const tlp::node c, const tlp::node d,
               const std::vector<tlp::node> &input);
};
#endif // QUADTREE_H
