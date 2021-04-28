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

#ifndef OCTREEBUNDLE_H
#define OCTREEBUNDLE_H
#include <tulip/DrawingTools.h>
#include <tulip/Vector.h>

class OctreeBundle {
public:
  static void compute(tlp::Graph *graph, double splitRatio,
                      tlp::LayoutProperty *layout = nullptr,
                      tlp::SizeProperty *size = nullptr);
  //=====================================
  static bool isIn(const tlp::Coord &p, const tlp::Coord &a,
                   const tlp::Coord &b, const tlp::Coord &c,
                   const tlp::Coord &d);

protected:
  void createOctree(tlp::Graph *graph, tlp::LayoutProperty *layout = nullptr,
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
  typedef std::unordered_map<tlp::Coord, tlp::node> MapVecNode;
  MapVecNode mapN;
  //=====================================
  tlp::node splitEdge(tlp::node a, tlp::node b);
  //=====================================
  void elmentSplitting(const tlp::Coord &a, const tlp::Coord &b,
                       const tlp::Coord &c, const tlp::Coord &d,
                       const std::vector<tlp::node> &input,
                       std::vector<tlp::node> &in, std::vector<tlp::node> &out);
  //=====================================
  void recQuad(const tlp::Coord fr[4], const tlp::Coord ba[4],
               const std::vector<tlp::node> &input);
  tlp::node addNode(const tlp::Coord &pos);
  std::vector<std::pair<tlp::node, tlp::node>> unvalidEdges;
};

#endif // OCTREEBUNDLE_H
