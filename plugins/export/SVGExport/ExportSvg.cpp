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
#include "ExportSvg.h"
#include "Shape.h"
#include <tulip/BoundingBox.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TlpQtTools.h>

#include <QStringBuilder>

using namespace std;
using namespace tlp;

/*TODO
 * - export label correctly (font used and overlapping if possible)
 * - export label position (bottom, top, right and left)
 * - handle viewTexture
 * - add better support for Glowsphere node (done for edge extremity)
 * - add a better shape for cone and star
 * - check how fontawesomeicon can be implemented
 */

#define MARGIN 10

namespace {
QString tlpColor2SvgColor(const Color &color) {
  // converting color to SVG
  return QString("rgb(") + QString::number(static_cast<int>(color.getR())) +
         "," + QString::number(static_cast<int>(color.getG())) + "," +
         QString::number(static_cast<int>(color.getB())) + ")";
}

QString tlpAlphaColor2Opacity(const Color &color) {
  // converting color to SVG
  return QString::number(color.getA() / 255.f);
}
}

ExportSvg::ExportSvg(PluginProgress *pp, ostream &os, bool autoformatting)
    : ExportInterface(pp, os), _res(&_out) {
  _res.setAutoFormatting(autoformatting);
  _res.setCodec("UTF-8");
}

bool ExportSvg::writeHeader(const BoundingBox &bb) {
  _res.writeStartDocument();
  _res.writeStartElement("svg");
  double width = bb.width() + 1;
  double height = bb.height() + 1;
  _res.writeAttribute("width", QString::number(width));
  _res.writeAttribute("height", QString::number(height));
  _res.writeAttribute("xmlns", "http://www.w3.org/2000/svg");
  _res.writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
  _res.writeAttribute("viewbox", "0 0 " + QString::number(width + MARGIN) +
                                     " " + QString::number(height + MARGIN));
  _res.writeAttribute("version", "1.1");
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::writeGraph(const BoundingBox &bb, const Color &background) {
  // Background color
  _res.writeStartElement("rect");
  _res.writeAttribute("width", QString::number(bb.width() + 1));
  _res.writeAttribute("height", QString::number(bb.height() + 1));
  _res.writeAttribute("fill", tlpColor2SvgColor(background));
  _res.writeEndElement(); // rect
  // start to add graph. First translate from Tulip coordinates to SVG
  // coordinates
  _res.writeStartElement("g");
  _res.writeAttribute("desc", "Graph");
  _res.writeAttribute(
      "transform",
      "translate(" + QString::number(-bb.center().getX() + bb.width() / 2.f) +
          "," + QString::number(bb.center().getY() + bb.height() / 2.f) +
          ") scale(1,-1)"); // Translation for having a cartesian landmark in
                            // the middle of the graph
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::writeMetaGraph(const int transform_X, const int transform_Y,
                               float scale) {
  _res.writeStartElement("g");
  _res.writeAttribute("desc", "Meta-Graph");
  _res.writeAttribute("transform", "translate(" + QString::number(transform_X) +
                                       "," + QString::number(transform_Y) +
                                       ") scale(" + QString::number(scale) +
                                       "," + QString::number(-scale) +
                                       ")"); // Translation for having a
                                             // cartesian landmark in the middle
                                             // of the graph
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::writeEndGraph() {
  _res.writeEndElement();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::writeEnd() {
  _res.writeEndDocument();
  _os << _out.constData();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::groupNode() {
  _res.writeStartElement("g");
  _res.writeAttribute("id", "Nodes");
  _res.writeAttribute("desc", "This is the group of nodes");
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::groupEdge() {
  _res.writeStartElement("g");
  _res.writeAttribute("id", "Edges");
  _res.writeAttribute("desc", "This is the group of edges");
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::endGroupNode() {
  _res.writeEndElement();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::endGroupEdge() {
  _res.writeEndElement();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::startNode(const unsigned id) {
  _res.writeStartElement("g");
  _res.writeAttribute("id", QString::number(id));
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::endNode() {
  _res.writeEndElement();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::startEdge(const unsigned id) {
  _res.writeStartElement("g");
  _res.writeAttribute("id", QString::number(id));
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::endEdge() {
  _res.writeEndElement();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::addLabel(const string &type, const string &label,
                         const Color &labelcolor, const Coord &coord,
                         const unsigned &fsize, const Size &size) {
  if (!label.empty()) {
    _res.writeStartElement("text");
    _res.writeAttribute("x", QString::number(coord.getX()));

    if (type == "node") {
      // empirically adjust to font-size/3 to ensure centering
      _res.writeAttribute(
          "y", QString::number(-(coord.getY() -
                                 (size.getW() * 1.2) / (3 * label.length()))));
      _res.writeAttribute("font-size",
                          QString::number(size.getW() * 1.2 / label.length()));
    } else {
      _res.writeAttribute("y", QString::number(-coord.getY()));
      _res.writeAttribute("font-size", QString::number(fsize / 18));
    }
    _res.writeAttribute("text-anchor", "middle");
    _res.writeAttribute(
        "transform",
        "scale(1,-1)"); // Translation for not having the text upside down
    _res.writeAttribute("stroke-width", "0");
    addColor(labelcolor);
    _res.writeCharacters(tlp::tlpStringToQString(label));
    _res.writeEndElement(); // text
  }
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::addColor(const tlp::Color &color) {
  _res.writeAttribute("fill", tlpColor2SvgColor(color));
  _res.writeAttribute("fill-opacity", tlpAlphaColor2Opacity(color));
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::addRotation(const double rotation, const Coord &center) {
  _res.writeAttribute("transform", "rotate(" + QString::number(rotation) + "," +
                                       QString::number(center.getX()) + "," +
                                       QString::number(center.getY()) + ")");
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::addBorder(const Color &borderColor, const double borderwidth) {
  _res.writeAttribute("stroke", tlpColor2SvgColor(borderColor));
  _res.writeAttribute("stroke-opacity", tlpAlphaColor2Opacity(borderColor));
  _res.writeAttribute("stroke-width", QString::number(borderwidth / 10));
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
  return !_res.hasError();
#else
  return true;
#endif
}

bool ExportSvg::addShape(const tlp::NodeShape::NodeShapes &type,
                         const Coord &coord, const Size &size,
                         const Color &bordercolor, const double borderwidth,
                         const Color &color) {
  // node coord
  float x = coord.getX();
  float y = coord.getY();
  // node size
  float w = size.getW() / 2.f; // why /2 ????
  float h = size.getH() / 2.f;

  /*
    This is the scheme of coordinates of each point used

    a * * r * * e * * s * * b
    *           *           *
    *           *           *
    g           *           i
    *           *           *
    n           *           o
    p * * * * * o * * * * * q
    *           *           *
    *           *           *
    j           *           k
    *           *           *
    *           *           *
    d * * l * * f * * m * * c
  */

  // Rectangle points
  float xa = x - w;
  float ya = y + h;
  float xb = x + w;
  float yb = y + h;
  float xc = x + w;
  float yc = y - h;
  QString xcstr = QString::number(xc);
  QString ycstr = QString::number(yc);

  float xd = x - w;
  float yd = y - h;
  QString xdstr = QString::number(xd);
  QString ydstr = QString::number(yd);

  // Extra triangle
  float xe = x;
  float ye = y + h;
  QString xestr = QString::number(xe);
  QString yestr = QString::number(ye);

  // Extra hexagon
  float yf = y - h;
  QString yfstr = QString::number(yf);
  QString xfstr = QString::number(x);
  float xg = x - w;
  float yg = y + (h * 0.5);
  float xi = x + w;
  float yi = y + (h * 0.5);
  float xj = x - w;
  float yj = y - (h * 0.5);
  float xk = x + w;
  float yk = y - (h * 0.5);

  // Extra pentagon
  float xl = x - (w * 0.5);
  float yl = y - h;
  float xm = x + (w * 0.5);
  float ym = y - h;
  float xn = x - w;
  float yn = y + (h * 0.3);
  float xo = x + w;
  float yo = y + (h * 0.3);

  // Extra diamond
  float xp = x - w;
  float xq = x + w;

  // Now we group up points to get the shape
  QString list_points_rect = QString::number(xa) + "," + QString::number(ya) +
                             " " + QString::number(xb) + "," +
                             QString::number(yb) + " " + xcstr + "," + ycstr +
                             " " + xdstr + "," + ydstr;

  switch (type) {
  case NodeShape::Circle:
  case NodeShape::Cylinder:
  case NodeShape::HalfCylinder:
  case NodeShape::Sphere:
  case NodeShape::GlowSphere:
    _res.writeStartElement("ellipse");
    _res.writeAttribute("cx", QString::number(x));
    _res.writeAttribute("cy", QString::number(y));
    _res.writeAttribute("rx", QString::number(w));
    _res.writeAttribute("ry", QString::number(h));
    addColor(color);
    if (borderwidth > 0)
      addBorder(bordercolor, borderwidth);
    break;

  case NodeShape::CubeOutlinedTransparent:
    _res.writeStartElement("polygon");
    _res.writeAttribute("points", list_points_rect);
    _res.writeAttribute("fill", "none");
    if (borderwidth == 0)
      addBorder(tlp::Color::Black, 0.02);
    else
      addBorder(bordercolor, borderwidth);
    _res.writeEndElement();
    break;
  case NodeShape::Triangle:
    _res.writeStartElement("polygon");
    _res.writeAttribute("points", xestr + "," + yestr + " " + xcstr + "," +
                                      ycstr + " " + xdstr + "," + ydstr);
    addColor(color);
    if (borderwidth > 0)
      addBorder(bordercolor, borderwidth);
    break;

  case NodeShape::Hexagon:
    _res.writeStartElement("polygon");
    _res.writeAttribute(
        "points", xestr + "," + yestr + " " + QString::number(xi) + "," +
                      QString::number(yi) + " " + QString::number(xk) + "," +
                      QString::number(yk) + " " + xfstr + "," + yfstr + " " +
                      QString::number(xj) + "," + QString::number(yj) + " " +
                      QString::number(xg) + "," + QString::number(yg));
    addColor(color);
    if (borderwidth > 0)
      addBorder(bordercolor, borderwidth);
    break;
  case NodeShape::Pentagon:
    _res.writeStartElement("polygon");
    _res.writeAttribute(
        "points", xestr + "," + yestr + " " + QString::number(xo) + "," +
                      QString::number(yo) + " " + QString::number(xm) + "," +
                      QString::number(ym) + " " + QString::number(xl) + "," +
                      QString::number(yl) + " " + QString::number(xn) + "," +
                      QString::number(yn));
    addColor(color);
    if (borderwidth > 0)
      addBorder(bordercolor, borderwidth);
    break;
  case NodeShape::Diamond:
    _res.writeStartElement("polygon");
    _res.writeAttribute(
        "points", xestr + "," + yestr + " " + QString::number(xq) + "," +
                      QString::number(y) + " " + xfstr + "," + yfstr + " " +
                      QString::number(xp) + "," + QString::number(y));
    addColor(color);
    if (borderwidth > 0)
      addBorder(bordercolor, borderwidth);
    break;
  case NodeShape::Billboard:
  case NodeShape::CubeOutlined:
  case NodeShape::Window:
    _res.writeStartElement("polygon");
    _res.writeAttribute("points", list_points_rect);
    addColor(color);
    if (borderwidth > 0)
      addBorder(bordercolor, borderwidth);
    break;

  case NodeShape::Cube:
  case NodeShape::Square:
  case NodeShape::RoundedBox:
    _res.writeStartElement("rect");
    _res.writeAttribute("x", QString::number(x - size.getX() / 2));
    _res.writeAttribute("y", QString::number(y - size.getY() / 2));
    _res.writeAttribute("width", QString::number(w * 2));
    _res.writeAttribute("height", QString::number(h * 2));
    if (type == NodeShape::RoundedBox) {
      _res.writeAttribute("rx", QString::number(w / 10.f));
      _res.writeAttribute("ry", QString::number(h / 10.f));
    }
    addColor(color);
    if (borderwidth > 0)
      addBorder(bordercolor, borderwidth);
    break;
  case NodeShape::ChristmasTree: // If the shape is a christmas tree: composed
                                 // by a circle, a triangle and a rectangle
    {
      // circle
      float yc = y + (0.9 * h);
      float rx = 0.1 * w;
      float ry = 0.1 * h;

    case NodeShape::ChristmasTree: { // If the shape is a christmas tree:
                                     // composed by a circle, a triangle and a
                                     // rectangle
      // circle
      float yc = y + (0.9 * h);
      float rx = 0.1 * w;
      float ry = 0.1 * h;
      addColor(color);
      if (borderwidth > 0)
        addBorder(bordercolor, borderwidth);
      _res.writeStartElement("ellipse");
      _res.writeAttribute("cx", QString::number(x));
      _res.writeAttribute("cy", QString::number(yc));
      _res.writeAttribute("rx", QString::number(rx));
      _res.writeAttribute("ry", QString::number(ry));
      _res.writeEndElement(); // ellipse

      // triangle
      float yt1 = y + (0.8 * h);
      float xt2 = x + (0.8 * w);
      float yt2 = y - (0.5 * h);
      float xt3 = x - (0.8 * w);
      QString list_pt_tr = QString::number(x) + "," + QString::number(yt1) +
                           " " + QString::number(xt2) + "," +
                           QString::number(yt2) + " " + QString::number(xt3) +
                           "," + QString::number(yt2);

      _res.writeStartElement("polygon");
      _res.writeAttribute("points", list_pt_tr);
      _res.writeAttribute("fill", "#1A7900");
      _res.writeEndElement(); // triangle

      _res.writeStartElement("polygon");
      _res.writeAttribute("points", list_pt_tr);
      _res.writeAttribute("fill", "#1A7900");
      _res.writeEndElement(); // triangle

      // rectangle
      float xr1 = x - (0.25 * w);
      float xr2 = x + (0.25 * w);
      float yr3 = y - h;

      QString list_pt_rect = QString::number(xr1) + "," + QString::number(yt2) +
                             " " + QString::number(xr2) + "," +
                             QString::number(yt2) + " " + QString::number(xr2) +
                             "," + QString::number(yr3) + " " +
                             QString::number(xr1) + "," + QString::number(yr3);

    case NodeShape::Ring: // If the shape is a ring, we have to draw the white
                          // circle in the middle
      _res.writeStartElement("ellipse");
      _res.writeAttribute("cx", QString::number(x));
      _res.writeAttribute("cy", QString::number(y));
      _res.writeAttribute("rx", QString::number(w));
      _res.writeAttribute("ry", QString::number(h));
      _res.writeAttribute("fill", "none");
      _res.writeAttribute("fill-opacity", "1");
      _res.writeAttribute("stroke", tlpColor2SvgColor(color));
      _res.writeAttribute("stroke-width", QString::number(w / 2));
      _res.writeEndElement(); // ellipse
      if (borderwidth > 0) {
        _res.writeStartElement("ellipse");
        _res.writeAttribute("cx", QString::number(x));
        _res.writeAttribute("cy", QString::number(y));
        _res.writeAttribute("rx", QString::number(w + w * 0.1));
        _res.writeAttribute("ry", QString::number(h + h * 0.1));
        _res.writeAttribute("fill", "none");
        addBorder(bordercolor, borderwidth);
      }
      break;
    case NodeShape::Cross: {
      // polygone 1
      float xr = x - (w * 0.25);
      float yr = y + h;
      float xs = x + (w * 0.25);
      float ys = y + h;
      float xm = x + (w * 0.25);
      float ym = y - h;
      float xl = x - (w * 0.25);
      float yl = y - h;

      // polygone 2
      float xg = x - w;
      float yg = y + (h * 0.25);
      float xi = x + w;
      float yi = y + (h * 0.25);
      float xk = x + w;
      float yk = y - (h * 0.25);
      float xj = x - w;
      float yj = y - (h * 0.25);

      _res.writeStartElement("polygon");
      _res.writeAttribute(
          "points", QString::number(xr) + "," + QString::number(yr) + " " +
                        QString::number(xs) + "," + QString::number(ys) + " " +
                        QString::number(xm) + "," + QString::number(ym) + " " +
                        QString::number(xl) + "," + QString::number(yl));
      _res.writeEndElement();

      _res.writeStartElement("polygon");
      _res.writeAttribute(
          "points", QString::number(xg) + "," + QString::number(yg) + " " +
                        QString::number(xi) + "," + QString::number(yi) + " " +
                        QString::number(xk) + "," + QString::number(yk) + " " +
                        QString::number(xj) + "," + QString::number(yj));
      addColor(color);
      if (borderwidth > 0)
        addBorder(bordercolor, borderwidth);
    } break;

    // TODO!!!! Right now, just draw a circle
    case NodeShape::Cone:
    case NodeShape::Star:
    case NodeShape::FontAwesomeIcon:
    default:
      _res.writeStartElement("ellipse");
      _res.writeAttribute("cx", QString::number(x));
      _res.writeAttribute("cy", QString::number(y));
      _res.writeAttribute("rx", QString::number(w));
      _res.writeAttribute("ry", QString::number(h));
      addColor(color);
      if (borderwidth > 0)
        addBorder(bordercolor, borderwidth);
      break;
    }
      _res.writeEndElement();
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
      return !_res.hasError();
#else
      return true;
#endif
    }

    bool ExportSvg::exportEdge(
        const unsigned id, const tlp::EdgeShape::EdgeShapes &type,
        const std::vector<tlp::Coord> &bends, const tlp::Color &color1,
        const tlp::Color &color2, const double width,
        const EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type,
        const unsigned id_src_shape,
        const EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type,
        const unsigned id_tgt_shape, const std::vector<Coord> &edgeVertice) {
      // Color gradient
      QString name("gradient_edge_" + QString::number(id));
      _res.writeStartElement("defs");
      _res.writeStartElement("linearGradient");

      _res.writeAttribute("id", name);
      _res.writeAttribute("gradientUnits", "objectBoundingBox");

      _res.writeStartElement("stop");
      _res.writeAttribute("offset", "0%");
      _res.writeAttribute("stop-color", tlpColor2SvgColor(color2));
      _res.writeAttribute("stop-opacity", tlpAlphaColor2Opacity(color2));
      _res.writeEndElement();

      _res.writeStartElement("stop");
      _res.writeAttribute("offset", "100%");
      _res.writeAttribute("stop-color", tlpColor2SvgColor(color1));
      _res.writeAttribute("stop-opacity", tlpAlphaColor2Opacity(color1));
      _res.writeEndElement();
      _res.writeEndElement();
      _res.writeEndElement();

      return createEdge(type, bends, "url(#" + name + ")", "1", width,
                        src_anchor_shape_type, id_src_shape,
                        tgt_anchor_shape_type, id_tgt_shape, edgeVertice);
    }

    bool ExportSvg::exportEdge(
        const tlp::EdgeShape::EdgeShapes &type, const vector<Coord> &bends,
        const Color &color, const double width,
        const tlp::EdgeExtremityShape::EdgeExtremityShapes
            src_anchor_shape_type,
        const unsigned id_src_shape,
        const tlp::EdgeExtremityShape::EdgeExtremityShapes
            tgt_anchor_shape_type,
        const unsigned id_tgt_shape, const std::vector<Coord> &edgeVertice) {
      return createEdge(type, bends, tlpColor2SvgColor(color),
                        tlpAlphaColor2Opacity(color), width,
                        src_anchor_shape_type, id_src_shape,
                        tgt_anchor_shape_type, id_tgt_shape, edgeVertice);
    }

    bool ExportSvg::createEdge(
        const tlp::EdgeShape::EdgeShapes &type, const vector<Coord> &bends,
        const QString &color, const QString &qcolorA, const double width,
        const tlp::EdgeExtremityShape::EdgeExtremityShapes
            src_anchor_shape_type,
        const unsigned id_src_shape,
        const tlp::EdgeExtremityShape::EdgeExtremityShapes
            tgt_anchor_shape_type,
        const unsigned id_tgt_shape, const std::vector<Coord> &edgeVertice) {
      QString node_source_X(QString::number(edgeVertice[0].getX()));
      QString node_source_Y(QString::number(edgeVertice[0].getY()));
      QString node_target_X(
          QString::number(edgeVertice[edgeVertice.size() - 1].getX()));
      QString node_target_Y(
          QString::number(edgeVertice[edgeVertice.size() - 1].getY()));

      _res.writeStartElement("path");
      QString points("M " + node_source_X + "," + node_source_Y);

      if (bends.empty()) { // If there is no checkpoints, it will be a simple
                           // line
        points += " L";
      } else { // Otherwise, we draw it according to its type
        vector<Coord> controlPoints;
        controlPoints.push_back(edgeVertice[0]);
        controlPoints.insert(controlPoints.end(), bends.begin(), bends.end());
        controlPoints.push_back(edgeVertice[edgeVertice.size() - 1]);
        vector<Coord> curvePoints;
        switch (type) {
        case EdgeShape::Polyline: {
          points += " L";
          for (vector<Coord>::const_iterator it = bends.begin();
               it != bends.end(); ++it) {
            points += " " + QString::number(it->getX()) + "," +
                      QString::number(it->getY());
          }
          break;
        }

        case EdgeShape::BezierCurve: {
          if (bends.size() == 1) {
            points += " Q " + QString::number(bends.begin()->getX()) + "," +
                      QString::number(bends.begin()->getY());
            break;
          } else {
            computeBezierPoints(controlPoints, curvePoints);
            points += " S";
            for (vector<Coord>::const_iterator it = curvePoints.begin();
                 it != curvePoints.end(); ++it) {
              points += " " + QString::number(it->getX()) + "," +
                        QString::number(it->getY());
            }
          }
          break;
        }

        case EdgeShape::CatmullRomCurve: {
          computeCatmullRomPoints(controlPoints, curvePoints);
          points += " S";
          for (vector<Coord>::const_iterator it = curvePoints.begin();
               it != curvePoints.end(); ++it) {
            points += " " + QString::number(it->getX()) + "," +
                      QString::number(it->getY());
          }
          break;
        }

        case EdgeShape::CubicBSplineCurve: {
          computeOpenUniformBsplinePoints(controlPoints, curvePoints);
          points += " S";
          for (vector<Coord>::const_iterator it = curvePoints.begin();
               it != curvePoints.end(); ++it) {
            points += " " + QString::number(it->getX()) + "," +
                      QString::number(it->getY());
          }
          break;
        }
        }
      }

      _res.writeAttribute("d",
                          points + " " + node_target_X + "," + node_target_Y);
      _res.writeAttribute("fill", "none");
      _res.writeAttribute("stroke-width", QString::number(width));
      _res.writeAttribute("stroke-opacity", qcolorA);
      _res.writeAttribute("stroke", color);

      if (src_anchor_shape_type != EdgeExtremityShape::None)
        _res.writeAttribute("marker-start",
                            "url(#Msrc" + QString::number(id_src_shape) + ")");

      if (tgt_anchor_shape_type != EdgeExtremityShape::None)
        _res.writeAttribute("marker-end",
                            "url(#Mtgt" + QString::number(id_tgt_shape) + ")");

      _res.writeEndElement(); // path
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
      return !_res.hasError();
#else
      return true;
#endif
    }

    bool ExportSvg::exportEdgeExtremity(
        const unsigned id_src_shape, const unsigned id_tgt_shape,
        const tlp::EdgeExtremityShape::EdgeExtremityShapes
            src_anchor_shape_type,
        const tlp::EdgeExtremityShape::EdgeExtremityShapes
            tgt_anchor_shape_type,
        const tlp::Color &color, const unsigned int id_src_gradient,
        const unsigned int id_tgt_gradient,
        const Coord &coord_edge_extremity_source,
        const Coord &coord_edge_extremity_target, const Size &size_node_src,
        const Size &size_node_tgt) {
      if (src_anchor_shape_type != EdgeExtremityShape::None) {
        // Writing the context
        _res.writeStartElement("defs");
        _res.writeStartElement("marker");
        _res.writeAttribute("id", "Msrc" % QString::number(id_src_shape));
        _res.writeAttribute("markerUnits", "strokeWidth");
        _res.writeAttribute("orient", "auto");
        _res.writeAttribute("markerWidth", "4");
        _res.writeAttribute("markerHeight", "3");

        switch (src_anchor_shape_type) {
        case EdgeExtremityShape::None:
          break;

        case EdgeExtremityShape::FontAwesomeIcon: // add an arrow to replace
                                                  // FontAwesomeIcon
        case EdgeExtremityShape::Arrow:
          ExtremityShape::Arrow(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Circle:
          ExtremityShape::Circle(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Cross:
          ExtremityShape::Cross(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Diamond:
          ExtremityShape::Diamond(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Hexagon:
          ExtremityShape::Hexagon(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Pentagon:
          ExtremityShape::Pentagon(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Ring:
          ExtremityShape::Ring(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Square:
          ExtremityShape::Square(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Star:
          ExtremityShape::Star(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Cube:
          ExtremityShape::Cube(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::CubeOutlinedTransparent:
          ExtremityShape::CubeOutlinedTransparent(
              _res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Cone:
          ExtremityShape::Cone(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Cylinder:
          ExtremityShape::Cylinder(_res, tlpColor2SvgColor(color), false);
          break;

        case EdgeExtremityShape::Sphere:
          ExtremityShape::Sphere(_res, color, false, id_src_gradient);
          break;

        case EdgeExtremityShape::GlowSphere:
          ExtremityShape::GlowSphere(_res, color, false, id_src_gradient,
                                     coord_edge_extremity_source,
                                     size_node_src);
          break;
        }
        _res.writeEndElement();

        if (src_anchor_shape_type != EdgeExtremityShape::GlowSphere) {
          _res.writeEndElement(); // End context "marker"
          _res.writeEndElement(); // End context "def"
        }
      }

      if (tgt_anchor_shape_type != EdgeExtremityShape::None) {
        // Writing the context
        _res.writeStartElement("defs");
        _res.writeStartElement("marker");

        _res.writeAttribute("id", "Mtgt" % QString::number(id_tgt_shape));
        _res.writeAttribute("markerUnits", "strokeWidth");
        _res.writeAttribute("orient", "auto");
        _res.writeAttribute("markerWidth", "4");
        _res.writeAttribute("markerHeight", "3");

        switch (tgt_anchor_shape_type) {
        case EdgeExtremityShape::None:
          break;

        case EdgeExtremityShape::FontAwesomeIcon: // add an arrow to replace
                                                  // FontAwesomeIcon
        case EdgeExtremityShape::Arrow:
          ExtremityShape::Arrow(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Circle:
          ExtremityShape::Circle(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Cross:
          ExtremityShape::Cross(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Diamond:
          ExtremityShape::Diamond(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Hexagon:
          ExtremityShape::Hexagon(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Pentagon:
          ExtremityShape::Pentagon(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Ring:
          ExtremityShape::Ring(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Square:
          ExtremityShape::Square(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Star:
          ExtremityShape::Star(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Cube:
          ExtremityShape::Cube(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::CubeOutlinedTransparent:
          ExtremityShape::CubeOutlinedTransparent(
              _res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Cone:
          ExtremityShape::Cone(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Cylinder:
          ExtremityShape::Cone(_res, tlpColor2SvgColor(color), true);
          break;

        case EdgeExtremityShape::Sphere:
          ExtremityShape::Sphere(_res, color, true, id_tgt_gradient);
          break;

        case EdgeExtremityShape::GlowSphere:
          ExtremityShape::GlowSphere(_res, color, true, id_tgt_gradient,
                                     coord_edge_extremity_target,
                                     size_node_tgt);
          break;
        }
        _res.writeEndElement();
        if (tgt_anchor_shape_type != EdgeExtremityShape::GlowSphere) {
          _res.writeEndElement(); // End context "marker"
          _res.writeEndElement(); // End context "def"
        }
      }
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
      return !_res.hasError();
#else
      return true;
#endif
    }
