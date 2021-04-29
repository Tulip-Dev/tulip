#ifndef EXPORTSVG_H
#define EXPORTSVG_H

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
#include "ExportInterface.h"
#include <set>

#include <QXmlStreamWriter>
#include <QByteArray>
#include <QString>

class ExportSvg : public ExportInterface {

  QXmlStreamWriter _res;
  QByteArray _out;
  std::set<std::string> _base64fontAdded;
  bool _woff2;
  bool _gloweffectAdded;
  bool createEdge(const tlp::EdgeShape::EdgeShapes &type, const std::vector<tlp::Coord> &bends,
                  const QString &color, const QString &qcolorA, const double width,
                  const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type,
                  const unsigned id_src_shape,
                  const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type,
                  const unsigned id_tgt_shape, const std::vector<tlp::Coord> &edgeVertice);
  void addWebFontFromIconName(const std::string &iconName);
  void addGlowEffect();

public:
  ExportSvg(tlp::PluginProgress *pp, std::ostream &os, const bool autoformatting, const bool woff2);
  bool writeHeader(const tlp::BoundingBox &bb) override;
  bool writeGraph(const tlp::BoundingBox &bb, const tlp::Color &background,
                  bool noBackground) override;
  bool writeMetaGraph(const int transform_X, const int transform_Y, float scale) override;
  bool writeEndGraph() override;
  bool groupNode() override;
  bool addShape(const tlp::NodeShape::NodeShapes &type, const tlp::Coord &coord,
                const tlp::Size &size, const tlp::Color &bordercolor, const double borderwidth,
                const tlp::Color &color, std::string iconName) override;
  bool addRotation(const double rotation, const tlp::Coord &center) override;
  bool endGroupNode() override;
  bool groupEdge() override;
  bool exportEdge(const tlp::EdgeShape::EdgeShapes &type, const std::vector<tlp::Coord> &bends,
                  const tlp::Color &color, const double width,
                  const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type,
                  const unsigned id_src_shape,
                  tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type,
                  const unsigned id_tgt_shape, const std::vector<tlp::Coord> &edgeVertice) override;
  bool exportEdge(const unsigned id, const tlp::EdgeShape::EdgeShapes &type,
                  const std::vector<tlp::Coord> &bends, const tlp::Color &color1,
                  const tlp::Color &color2, const double width,
                  const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type,
                  const unsigned id_src_shape,
                  const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type,
                  const unsigned id_tgt_shape, const std::vector<tlp::Coord> &edgeVertice) override;
  bool exportEdgeExtremity(const unsigned id_src_shape, const unsigned id_tgt_shape,
                           const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type,
                           const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type,
                           const tlp::Color &color, const unsigned int id_src_gradient,
                           const unsigned int id_tgt_gradient,
                           const std::string &iconName) override;
  bool addLabel(const std::string &type, const std::string &label, const tlp::Color &labelcolor,
                const tlp::Coord &coord, const unsigned &fsize, const tlp::Size &size) override;
  bool addBorder(const tlp::Color &borderColor, const double borderwidth) override;
  bool startNode(const unsigned id) override;
  bool endNode() override;
  bool addColor(const tlp::Color &color) override;
  bool startEdge(const unsigned id) override;
  bool endEdge() override;
  bool endGroupEdge() override;
  bool writeEnd() override;
};

#endif // EXPORTSVG_H
