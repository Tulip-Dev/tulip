#ifndef EXPORTSVG_H
#define EXPORTSVG_H


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
#include "RepresentExport.h"

#include <QXmlStreamWriter>
#include <QString>

#include <tulip/TulipViewSettings.h>

// First concrete builder
class ExportSvg : public RepresentExport {

  QXmlStreamWriter _res;
  QString _outs;

  void createEdge(const tlp::EdgeShape::EdgeShapes &type, const std::vector<tlp::Coord> &bends, const QString &color, const QString &qcolorA, const double width, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const unsigned id_src_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const unsigned id_tgt_shape, const std::vector<tlp::Coord>& edgeVertice);

public:
  ExportSvg();
  void writeHeader(tlp::BoundingBox &bb);
  void writeGraph(tlp::BoundingBox &bb);
  void writeMetaGraph(const int transform_X, const int transform_Y, float scale);
  void writeEndGraph();
  void groupNode();
  void addShape(const tlp::NodeShape::NodeShapes &type, const tlp::Coord &coord, const tlp::Size &size);
  void addRotation(const double rotation, const tlp::Coord &center);
  void endGroupNode();
  void groupEdge();
  void exportEdge(const tlp::EdgeShape::EdgeShapes &type, const std::vector<tlp::Coord> &bends, const tlp::Color &color,const double width, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const unsigned id_src_shape, tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const unsigned id_tgt_shape, const std::vector<tlp::Coord>& edgeVertice);
  void exportEdge(const unsigned id, const tlp::EdgeShape::EdgeShapes &type, const std::vector<tlp::Coord> &bends, const tlp::Color &color1, const tlp::Color &color2, const double width, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const unsigned id_src_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const unsigned id_tgt_shape, const std::vector<tlp::Coord>& edgeVertice);
  void exportEdgeExtremity(const unsigned id_src_shape, const unsigned id_tgt_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const tlp::Color &color, unsigned int& id_src_gradient, unsigned int& id_tgt_gradient, const tlp::Coord &coord_edge_extremity_source, const tlp::Coord &coord_edge_extremity_target, const tlp::Size &size_node_src, const tlp::Size &size_node_tgt);
  void addLabel(const std::string &type, const std::string &label, const tlp::Color &labelcolor, const tlp::Coord &coord, const tlp::Size &size);
  void addBorder(const tlp::Color &borderColor, const double borderwidth);
  void startNode(const unsigned id);
  void endNode();
  void addColor(const tlp::Color &color);
  void startEdge(const unsigned id);
  void endEdge();
  void endGroupEdge();
  void writeEnd();
  void getData(std::ostream& os) const;
};


#endif // EXPORTSVG_H
