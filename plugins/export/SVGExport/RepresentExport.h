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
#ifndef REPRESENTEXPORT_H
#define REPRESENTEXPORT_H

#include <vector>

#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/ParametricCurves.h>

class QString;

namespace tlp {
class Color;
struct BoundingBox;
}

// Builder
class RepresentExport {

public:

  virtual ~RepresentExport() {}

  // This method write the header of the exported file
  virtual void writeHeader(tlp::BoundingBox &bb)=0;

  // This method begin a graph
  virtual void writeGraph(tlp::BoundingBox &bb)=0;

  // This method begin a meta-graph
  virtual void writeMetaGraph(const int transform_X, const int transform_Y, float scale)=0;



  // This method end a graph or a meta-graph
  virtual void writeEndGraph() = 0;


  // This method end the file
  virtual void writeEnd()=0;

  // This method regroup nodes if we want to modify them later
  virtual void groupNode()=0;

  // This method regroup edges if we want to modify them later
  virtual void groupEdge()=0;

  // This method end the group of node
  virtual void endGroupNode()=0;

  // This method end the group of edge
  virtual void endGroupEdge()=0;

  // This method represent nodes according to incoming parameters
  virtual void startNode(const unsigned id)=0;
  virtual void endNode()=0;

  // This method define the shape of a node
  virtual void addShape(const tlp::NodeShape::NodeShapes &, const tlp::Coord &coord, const tlp::Size &size)=0;

  // This method represent edges according to incoming parameters
  // No edge color interpolation
  virtual void exportEdge(const tlp::EdgeShape::EdgeShapes &, const std::vector<tlp::Coord> &bends, const tlp::Color &edgeColor,const double width, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const unsigned id_src_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const unsigned id_tgt_shape, const std::vector<tlp::Coord>& edgeVertice)=0;
  // eEge color interpolation between edgeColor1 and edgeColor2
  virtual void exportEdge(const unsigned id, const tlp::EdgeShape::EdgeShapes &, const std::vector<tlp::Coord> &bends, const tlp::Color &edgeColor1, const tlp::Color &edgeColor2, const double width, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const unsigned id_src_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const unsigned id_tgt_shape, const std::vector<tlp::Coord>& edgeVertice)=0;

  // This method define extremities of edges
  virtual void exportEdgeExtremity(const unsigned id_src_shape, const unsigned id_tgt_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const tlp::Color &color, unsigned int& id_src_gradient, unsigned int& id_tgt_gradient, const tlp::Coord &coord_edge_extremity_source, const tlp::Coord &coord_edge_extremity_target, const tlp::Size &size_node_src, const tlp::Size &size_node_tgt)=0;

  // This method represent edges according to incoming parameters
  virtual void startEdge(const unsigned id)=0;
  virtual void endEdge()=0;


  // Elements properties
  virtual void addLabel(const std::string &type, const std::string &label, const tlp::Color &labelcolor, const tlp::Coord &coord, const tlp::Size &size)=0;
  virtual void addColor(const tlp::Color &nodeColor)=0;
  virtual void addRotation(const double rotation, const tlp::Coord &center)=0;
  virtual void addBorder(const tlp::Color &borderColor, const double borderwidth)=0;

  virtual void getData(std::ostream& os) const=0;

};

#endif // REPRESENTEXPORT_H
