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

#ifndef QUADTREE_H
#define QUADTREE_H
#include <map>
#include <vector>
#include <tulip/Graph.h>
#include <tulip/Vector.h>
#include <tulip/DrawingTools.h>
#include <tulip/ForEach.h>

//class tlp::LayoutProperty;
//class tlp::SizeProperty;
//class tlp::DoubleProperty;

class QuadTreeBundle {
public:
  static void compute(tlp::Graph *graph, double splitRatio, tlp::LayoutProperty *layout=NULL, tlp::SizeProperty *size=NULL);
  //=====================================
  static bool isIn(const tlp::Coord &p, const tlp::Coord &a, const tlp::Coord &b);

protected:
  void createQuadTree(tlp::Graph *graph, tlp::LayoutProperty *layout=NULL, tlp::SizeProperty *size=NULL);
private:
  double minSize;
  double nbNodesInOriginalGraph;
  std::vector<tlp::node> resultNode;
  tlp::LayoutProperty *layout;
  tlp::SizeProperty   *size;
  tlp::DoubleProperty *rot;
  tlp::DoubleProperty *ntype;
  tlp::Graph * graph;
  double splitRatio;
  typedef tlp::Vector<double, 2> Vec2D;
  struct LessPair {
    bool operator()( const Vec2D &a, const Vec2D &b) const {
      if ((a-b).norm() < 1E-6) return false;

      if (a[0] < b[0]) return true;

      if (a[0] > b[0]) return false;

      if (a[1] < b[1]) return true;

      return false;
    }
  };
  typedef std::map<Vec2D, tlp::node, LessPair> MapVecNode;
  MapVecNode mapN;
  //=====================================
  tlp::node splitEdge(tlp::node a, tlp::node b);
  //=====================================
  void elmentSplitting(const tlp::Coord &a, const tlp::Coord &b,
                       const std::vector<tlp::node> &input,
                       std::vector<tlp::node> &in,
                       std::vector<tlp::node> &out);
  //=====================================
  void recQuad(const tlp::node a,
               const tlp::node b,
               const tlp::node c,
               const tlp::node d,
               const std::vector<tlp::node> &input);
};
#endif // QUADTREE_H
