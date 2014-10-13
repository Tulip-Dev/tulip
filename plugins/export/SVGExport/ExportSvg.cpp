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

#include <tulip/TlpQtTools.h>
#include <tulip/BoundingBox.h>

#include <QStringBuilder>

using namespace std;
using namespace tlp;

/*TODO
 * - export label correctly (font used and overlapping if possible)
 * - export label position (bottom, top, right and left)
 * - handle viewTexture
 */

namespace {
QString tlpColor2SvgColor(const Color &color) {
  //converting color to SVG
  return QString("rgb(")+QString::number(static_cast<int>(color.getR()))+","+QString::number(static_cast<int>(color.getG()))+","+QString::number(static_cast<int>(color.getB()))+")";
}

QString tlpAlphaColor2Opacity(const Color &color) {
  //converting color to SVG
  return QString::number(color.getA()/255.f);
}
}

void ExportSvg::getData(std::ostream& os) const {
  os << _outs.toStdString();
}

ExportSvg::ExportSvg():_res(&_outs) {
  _res.setAutoFormatting(true);
}

void ExportSvg::writeHeader(BoundingBox &bb) {
  _res.writeStartDocument();
  _res.writeStartElement("svg");
  _res.writeAttribute("width", QString::number(bb.width() + 1));
  _res.writeAttribute("height", QString::number(bb.height() + 1));
  _res.writeAttribute("xmlns", "http://www.w3.org/2000/svg" );
  _res.writeAttribute("xmlns:xlink","http://www.w3.org/1999/xlink");
  _res.writeAttribute("version", "1.1");
}

void ExportSvg::writeGraph( BoundingBox &bb) {
  _res.writeStartElement("g");
  _res.writeAttribute("desc", "Graph");
  _res.writeAttribute("transform", "translate("+QString::number(-bb.center().getX()+bb.width()/2.f)+","+QString::number(bb.center().getY()+bb.height()/2.f)+") scale(1,-1)"); // Translation for having a cartesian landmark in the middle of the graph

  //Background color
  _res.writeStartElement("rect");
  _res.writeAttribute("width", QString::number(bb.width() + 1));
  _res.writeAttribute("height", QString::number(bb.height() + 1));
  _res.writeAttribute("fill", "white");
  _res.writeEndElement();//rect
}


void ExportSvg::writeMetaGraph(const int transform_X, const int transform_Y, float scale) {
  _res.writeStartElement("g");
  _res.writeAttribute("desc", "Meta-Graph");
  _res.writeAttribute("transform", "translate("+QString::number(transform_X)+","+QString::number(transform_Y)+") scale(" + QString::number(scale) + "," + QString::number(-scale) + ")"); // Translation for having a cartesian landmark in the middle of the graph
}


void ExportSvg::writeEndGraph() {
  _res.writeEndElement();
}

void ExportSvg::writeEnd() {
  _res.writeEndDocument();
}

void ExportSvg::groupNode() {
  _res.writeStartElement("g");
  _res.writeAttribute("id", "Nodes");
  _res.writeAttribute("desc","This is the group of nodes");
}

void ExportSvg::groupEdge() {
  _res.writeStartElement("g");
  _res.writeAttribute("id", "Edges");
  _res.writeAttribute("desc","This is the group of edges");
}

void ExportSvg::endGroupNode() {
  _res.writeEndElement(); //g
}

void ExportSvg::endGroupEdge() {
  _res.writeEndElement(); //g
}

void ExportSvg::startNode(const unsigned id) {
  _res.writeStartElement("g");
  _res.writeAttribute("id", QString::number(id));
}

void ExportSvg::endNode() {
  _res.writeEndElement(); // g
}

void ExportSvg::startEdge(const unsigned id) {
  _res.writeStartElement("g");
  _res.writeAttribute("id", QString::number(id));
}

void ExportSvg::endEdge() {
  _res.writeEndElement(); // g
}

void ExportSvg::addLabel(const string &type, const string &label, const Color &labelcolor, const Coord &coord, const Size &size) {
  if(!label.empty()) {
    _res.writeStartElement("text");
    _res.writeAttribute("x", QString::number(coord.getX()));

    if (type == "node")
      // empirically adjust to font-size/3 to ensure centering
      _res.writeAttribute("y", QString::number(-(coord.getY() -
                          (size.getW() * 1.2)/ (3 * label.length()))));
    else
      _res.writeAttribute("y", QString::number(-coord.getY()));

    if (type == "node")
      _res.writeAttribute("font-size", QString::number((size.getW() * 1.2) /label.length()));
    else if (type == "edge")
      _res.writeAttribute("font-size", QString::number(((size.getW()+size.getH())/label.length())+8));

    _res.writeAttribute("text-anchor", "middle");
    _res.writeAttribute("transform", "scale(1,-1)"); // Translation for not having the text upside down
    _res.writeAttribute("stroke-width", "0");
    addColor(labelcolor);
    _res.writeCharacters(tlp::tlpStringToQString(label));
    _res.writeEndElement(); //text
  }
}

void ExportSvg::addColor(const tlp::Color &color) {
  _res.writeAttribute("fill", tlpColor2SvgColor(color));
  _res.writeAttribute("fill-opacity", tlpAlphaColor2Opacity(color));
}

void ExportSvg::addRotation(const double rotation, const Coord &center) {
  _res.writeAttribute("transform", "rotate("+QString::number(rotation)+","+QString::number(center.getX())+","+QString::number(center.getY())+")");
}

void ExportSvg::addBorder(const Color &borderColor, const double borderwidth) {
  _res.writeAttribute("stroke", tlpColor2SvgColor(borderColor));
  _res.writeAttribute("stroke-opacity", tlpAlphaColor2Opacity(borderColor));
  _res.writeAttribute("stroke-width", QString::number(borderwidth));
}

void ExportSvg::addShape(const tlp::NodeShape::NodeShapes &type, const Coord &coord, const Size &size) {

  QList<NodeShape::NodeShapes> list_ellipse;
  list_ellipse << NodeShape::Circle << NodeShape::Cone << NodeShape::Cylinder << NodeShape::HalfCylinder << NodeShape::Sphere;

  QList<NodeShape::NodeShapes> list_polygone;
  list_polygone << NodeShape::Billboard << NodeShape::CubeOutlined << NodeShape::CubeOutlinedTransparent << NodeShape::Triangle << NodeShape::Pentagon << NodeShape::Hexagon << NodeShape::Diamond << NodeShape::Window;

  //node coord
  float x = coord.getX();
  float y = coord.getY();
  //node size
  float w = size.getW()/2.f;  //why /2 ????
  float h = size.getH()/2.f;

  if (list_ellipse.contains(type)) { // If the shape is an ellipse
    _res.writeStartElement("ellipse");
    _res.writeAttribute("cx", QString::number(x));
    _res.writeAttribute("cy", QString::number(y));
    _res.writeAttribute("rx", QString::number(w));
    _res.writeAttribute("ry", QString::number(h));
    _res.writeEndElement();
  }

  else if (list_polygone.contains(type)) { // Else if the shape is a polygon
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

    //Rectangle points
    float xa=x-w;
    float ya=y+h;
    QString xastr = QString::number(xa);
    QString yastr = QString::number(ya);

    float xb=x+w;
    float yb=y+h;
    QString xbstr = QString::number(xb);
    QString ybstr = QString::number(yb);

    float xc=x+w;
    float yc=y-h;
    QString xcstr = QString::number(xc);
    QString ycstr = QString::number(yc);

    float xd=x-w;
    float yd=y-h;
    QString xdstr = QString::number(xd);
    QString ydstr = QString::number(yd);

    //Extra triangle

    float xe=x;
    float ye=y+h;
    QString xestr = QString::number(xe);
    QString yestr = QString::number(ye);

    //Extra hexagon

    float xf=x;
    float yf=y-h;
    QString xfstr = QString::number(xf);
    QString yfstr = QString::number(yf);

    float xg=x-w;
    float yg=y+(h*0.5);
    QString xgstr = QString::number(xg);
    QString ygstr = QString::number(yg);

    float xi=x+w;
    float yi=y+(h*0.5);
    QString xistr = QString::number(xi);
    QString yistr = QString::number(yi);

    float xj=x-w;
    float yj=y-(h*0.5);
    QString xjstr = QString::number(xj);
    QString yjstr = QString::number(yj);

    float xk=x+w;
    float yk=y-(h*0.5);
    QString xkstr = QString::number(xk);
    QString ykstr = QString::number(yk);

    //Extra pentagon

    float xl=x-(w*0.5);
    float yl=y-h;
    QString xlstr = QString::number(xl);
    QString ylstr = QString::number(yl);

    float xm=x+(w*0.5);
    float ym=y-h;
    QString xmstr = QString::number(xm);
    QString ymstr = QString::number(ym);

    float xn=x-w;
    float yn=y+(h*0.3);
    QString xnstr = QString::number(xn);
    QString ynstr = QString::number(yn);

    float xo=x+w;
    float yo=y+(h*0.3);
    QString xostr = QString::number(xo);
    QString yostr = QString::number(yo);

    //Extra diamond

    float xp=x-w;
    float yp=y;
    QString xpstr = QString::number(xp);
    QString ypstr = QString::number(yp);

    float xq=x+w;
    float yq=y;
    QString xqstr = QString::number(xq);
    QString yqstr = QString::number(yq);


    // Now we group up points to get the shape

    QString list_points_rect=xastr+","+yastr+" "+xbstr+","+ybstr+" "+xcstr+","+ycstr+" "+xdstr+","+ydstr;
    QString list_points_triangle=xestr+","+yestr+" "+xcstr+","+ycstr+" "+xdstr+","+ydstr;
    QString list_points_hexagone=xestr+","+yestr+" "+xistr+","+yistr+" "+xkstr+","+ykstr+" "+xfstr+","+yfstr+" "+xjstr+","+yjstr+" "+xgstr+","+ygstr;
    QString list_points_pentagone=xestr+","+yestr+" "+xostr+","+yostr+" "+xmstr+","+ymstr+" "+xlstr+","+ylstr+" "+xnstr+","+ynstr;
    QString list_points_diamond=xestr+","+yestr+" "+xqstr+","+yqstr+" "+xfstr+","+yfstr+" "+xpstr+","+ypstr;

    _res.writeStartElement("polygon");

    if (type == NodeShape::Triangle)
      _res.writeAttribute("points", list_points_triangle);
    else if (type==NodeShape::Hexagon)
      _res.writeAttribute("points", list_points_hexagone);
    else if (type==NodeShape::Pentagon)
      _res.writeAttribute("points", list_points_pentagone);
    else if (type==NodeShape::Diamond)
      _res.writeAttribute("points", list_points_diamond);
    else if (type==NodeShape::CubeOutlinedTransparent) {
      _res.writeAttribute("points", list_points_rect);
      _res.writeEndElement();
      _res.writeStartElement("rect");
      _res.writeAttribute("x", QString::number((x-size.getX()/2)+w/10));
      _res.writeAttribute("y", QString::number((y-size.getY()/2)+h/10));
      _res.writeAttribute("width",QString::number(w*1.8));
      _res.writeAttribute("height",QString::number(h*1.8));
      _res.writeAttribute("fill", "white");
      _res.writeAttribute("fill-opacity", "1");
    }
    else
      _res.writeAttribute("points", list_points_rect);

    _res.writeEndElement();
  }

  else if((type==NodeShape::RoundedBox)||(type==NodeShape::Cube)||(type==NodeShape::Square)) {
    _res.writeStartElement("rect");
    _res.writeAttribute("x", QString::number(x-size.getX()/2));
    _res.writeAttribute("y", QString::number(y-size.getY()/2));
    _res.writeAttribute("width",QString::number(w*2));
    _res.writeAttribute("height",QString::number(h*2));

    if(type==NodeShape::RoundedBox) {
      _res.writeAttribute("rx", QString::number(w/10.f));
      _res.writeAttribute("ry", QString::number(h/10.f));
    }

    _res.writeEndElement();
  }

  else if (type==NodeShape::ChristmasTree) { // If the shape is a christmas tree : composed by a circle, a triangle and a rectangle
    //circle
    float yc=y+(0.9*h);
    float rx=0.1*w;
    float ry=0.1*h;

    _res.writeStartElement("ellipse");
    _res.writeAttribute("cx", QString::number(x));
    _res.writeAttribute("cy", QString::number(yc));
    _res.writeAttribute("rx", QString::number(rx));
    _res.writeAttribute("ry", QString::number(ry));
    _res.writeEndElement(); // ellipse

    //triangle
    float yt1=y+(0.8*h);
    float xt2=x+(0.8*w);
    float yt2=y-(0.5*h);
    float xt3=x-(0.8*w);
    QString list_pt_tr=QString::number(x)+","+QString::number(yt1)+" "+QString::number(xt2)+","+QString::number(yt2)+" "+QString::number(xt3)+","+QString::number(yt2);

    _res.writeStartElement("polygon");
    _res.writeAttribute("points", list_pt_tr);
    _res.writeAttribute("fill", "#1A7900");
    _res.writeEndElement(); // triangle

    //rectangle
    float xr1=x-(0.25*w);
    float xr2=x+(0.25*w);
    float yr3=y-h;

    QString list_pt_rect=QString::number(xr1)+","+QString::number(yt2)+" "+QString::number(xr2)+","+QString::number(yt2)+" "+QString::number(xr2)+","+QString::number(yr3)+" "+QString::number(xr1)+","+QString::number(yr3);

    _res.writeStartElement("polygon");
    _res.writeAttribute("points", list_pt_rect);
    _res.writeAttribute("fill", "#7D7900");
    _res.writeEndElement(); // rectangle
  }

  else if (type==NodeShape::Ring) { // If the shape is a ring, we have to draw the white circle in the middle
    _res.writeStartElement("ellipse");
    _res.writeAttribute("cx", QString::number(x));
    _res.writeAttribute("cy", QString::number(y));
    _res.writeAttribute("rx", QString::number(w));
    _res.writeAttribute("ry", QString::number(h));
    _res.writeEndElement(); // ellipse

    _res.writeStartElement("ellipse");
    _res.writeAttribute("cx", QString::number(x));
    _res.writeAttribute("cy", QString::number(y));
    _res.writeAttribute("rx",  QString::number(w/3));
    _res.writeAttribute("ry", QString::number(h/3));
    _res.writeAttribute("fill", "white");
    _res.writeAttribute("fill-opacity", "1");
    _res.writeEndElement(); // ring
  }

  else if (type==NodeShape::Cross) { // If the shape is a cross, represented by two neat rectangles and perpendicular
    //polygone 1
    float xr=x-(w*0.25);
    float yr=y+h;
    float xs=x+(w*0.25);
    float ys=y+h;
    float xm=x+(w*0.25);
    float ym=y-h;
    float xl=x-(w*0.25);
    float yl=y-h;

    //polygone 2
    float xg=x-w;
    float yg=y+(h*0.25);
    float xi=x+w;
    float yi=y+(h*0.25);
    float xk=x+w;
    float yk=y-(h*0.25);
    float xj=x-w;
    float yj=y-(h*0.25);

    _res.writeStartElement("polygon");
    _res.writeAttribute("points", QString::number(xr)+","+QString::number(yr)+" "+QString::number(xs)+","+QString::number(ys)+" "+QString::number(xm)+","+QString::number(ym)+" "+QString::number(xl)+","+QString::number(yl));
    _res.writeEndElement();

    _res.writeStartElement("polygon");
    _res.writeAttribute("points", QString::number(xg)+","+QString::number(yg)+" "+QString::number(xi)+","+QString::number(yi)+" "+QString::number(xk)+","+QString::number(yk)+" "+QString::number(xj)+","+QString::number(yj));
    _res.writeEndElement();
  }

  else { // If the shape is undefined, we draw it like a circle
    _res.writeStartElement("circle");
    _res.writeAttribute("cx", QString::number(x));
    _res.writeAttribute("cy", QString::number(y));
    _res.writeAttribute("r", QString::number(w));
    _res.writeEndElement();
  }
}


void ExportSvg::exportEdge(const unsigned id, const tlp::EdgeShape::EdgeShapes &type, const std::vector<tlp::Coord> &bends, const tlp::Color &color1, const tlp::Color &color2, const double width, const EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const unsigned id_src_shape, const EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const unsigned id_tgt_shape, const std::vector<Coord>& edgeVertice) {
  //Color gradient
  QString name("gradient_edge_"+QString::number(id));
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

  createEdge(type, bends, "url(#"+name+")", "1", width, src_anchor_shape_type, id_src_shape, tgt_anchor_shape_type, id_tgt_shape, edgeVertice);

}


void ExportSvg::exportEdge(const tlp::EdgeShape::EdgeShapes &type, const vector<Coord> &bends, const Color &color, const double width, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const unsigned id_src_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const unsigned id_tgt_shape, const std::vector<Coord>& edgeVertice) {
  createEdge(type, bends, tlpColor2SvgColor(color), tlpAlphaColor2Opacity(color), width, src_anchor_shape_type, id_src_shape, tgt_anchor_shape_type, id_tgt_shape, edgeVertice);
}


void ExportSvg::createEdge(const tlp::EdgeShape::EdgeShapes &type, const vector<Coord> &bends, const QString &color, const QString &qcolorA, const double width, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const unsigned id_src_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const unsigned id_tgt_shape, const std::vector<Coord>& edgeVertice) {

  QString node_source_X;
  QString node_source_Y;
  QString node_target_X;
  QString node_target_Y;

  node_source_X = QString::number(edgeVertice[0].getX());
  node_source_Y = QString::number(edgeVertice[0].getY());
  node_target_X = QString::number(edgeVertice[edgeVertice.size() - 1].getX());
  node_target_Y = QString::number(edgeVertice[edgeVertice.size() - 1].getY());

  _res.writeStartElement("path");
  QString points = QString("M")+ " " + node_source_X + "," + node_source_Y;

  if (bends.empty()) { // If there is no checkpoints, it will be a simple line
    points += " L";
  }
  else { // Otherwise, we draw it according to its type
    vector<Coord> controlPoints;
    controlPoints.push_back(edgeVertice[0]);
    controlPoints.insert(controlPoints.end(), bends.begin(), bends.end());
    controlPoints.push_back(edgeVertice[edgeVertice.size() - 1]);
    vector<Coord> curvePoints;

    switch(type) {
    case EdgeShape::Polyline: {
      points += " L";

      for(vector<Coord>::const_iterator it=bends.begin(); it!=bends.end(); ++it) {
        points += " " + QString::number(it->getX()) + "," + QString::number(it->getY());
      }

      break;
    }

    case EdgeShape::BezierCurve: {
      if(bends.size() == 1) {
        points += " Q " + QString::number(bends.begin()->getX()) + "," + QString::number(bends.begin()->getY());
        break;
      }
      else {
        computeBezierPoints(controlPoints, curvePoints);
        points += " S";

        for(vector<Coord>::const_iterator it = curvePoints.begin(); it != curvePoints.end(); ++it) {
          points += " " + QString::number(it->getX()) + "," + QString::number(it->getY());
        }
      }

      break;
    }

    case EdgeShape::CatmullRomCurve : {
      computeCatmullRomPoints(controlPoints, curvePoints);
      points += " S";

      for(vector<Coord>::const_iterator it = curvePoints.begin(); it != curvePoints.end(); ++it) {
        points += " " + QString::number(it->getX()) + "," + QString::number(it->getY());
      }

      break;
    }

    case EdgeShape::CubicBSplineCurve : {
      computeOpenUniformBsplinePoints(controlPoints, curvePoints);
      points += " S";

      for(vector<Coord>::const_iterator it = curvePoints.begin(); it != curvePoints.end(); ++it) {
        points += " " + QString::number(it->getX()) + "," + QString::number(it->getY());
      }

      break;
    }

    default : {
      points += " S";

      for(vector<Coord>::const_iterator it = edgeVertice.begin(); it != edgeVertice.end(); ++it) {
        points += " " + QString::number(it->getX()) + "," + QString::number(it->getY());
      }

      break;
    }
    }
  }

  _res.writeAttribute("d", points+ " "+ node_target_X + "," + node_target_Y);
  _res.writeAttribute("fill", "none");
  _res.writeAttribute("stroke-width", QString::number(width));
  _res.writeAttribute("stroke-opacity", qcolorA);
  _res.writeAttribute("stroke", color);

  if (src_anchor_shape_type != EdgeExtremityShape::None)
    _res.writeAttribute("marker-start","url(#Msrc" + QString::number(id_src_shape) + ")");

  if (tgt_anchor_shape_type != EdgeExtremityShape::None)
    _res.writeAttribute("marker-end","url(#Mtgt" + QString::number(id_tgt_shape) + ")");

  _res.writeEndElement(); // path
}

void ExportSvg::exportEdgeExtremity(const unsigned id_src_shape, const unsigned id_tgt_shape, const tlp::EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type, const tlp::EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type, const tlp::Color &color, unsigned int& id_src_gradient, unsigned int& id_tgt_gradient, const Coord &coord_edge_extremity_source, const Coord &coord_edge_extremity_target, const Size &size_node_src, const Size &size_node_tgt) {
  if (src_anchor_shape_type != EdgeExtremityShape::None) {
    // Writing the context
    _res.writeStartElement("defs");
    _res.writeStartElement("marker");

    _res.writeAttribute("id","Msrc" % QString::number(id_src_shape));
    _res.writeAttribute("markerUnits","strokeWidth");
    _res.writeAttribute("orient","auto");
    _res.writeAttribute("markerWidth","4");
    _res.writeAttribute("markerHeight","3");

    switch(src_anchor_shape_type) {
    case EdgeExtremityShape::Arrow: {
      _res.writeAttribute("viewBox","-10 0 10 10");
      _res.writeAttribute("refX","-6.7");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polyline");
      _res.writeAttribute("points","0,0 -10,5 0,10 -1,5");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Circle: {
      _res.writeAttribute("viewBox","1 1 10 10");
      _res.writeAttribute("refX","1.5");
      _res.writeAttribute("refY","6");

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Cross: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","4");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("path");
      _res.writeAttribute("d","M 0 0 L 10 10 M 0 10 L 10 0");
      _res.writeAttribute("stroke",tlpColor2SvgColor(color));
      _res.writeAttribute("stroke-width","2");
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Diamond: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","1.7");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","5,0 0,5 5,10 10,5");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Hexagon: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","0");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","5,0 0,3 0,7 5,10 10,7 10,3");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Pentagon: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","0");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","6,0 0,2 0,8 6,10 10,5");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Ring: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","1");
      _res.writeAttribute("refY","5");
      _res.writeAttribute("stroke",tlpColor2SvgColor(color));
      _res.writeAttribute("stroke-width","0.55");

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","5");
      _res.writeAttribute("cy","5");
      _res.writeAttribute("r","4.5");
      _res.writeAttribute("fill","white");
      _res.writeEndElement();

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","5");
      _res.writeAttribute("cy","5");
      _res.writeAttribute("r","3.2");
      _res.writeAttribute("fill","white");
      _res.writeAttribute("stroke-width","2.5");
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Square: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","0");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("rect");
      _res.writeAttribute("x","0");
      _res.writeAttribute("y","0");
      _res.writeAttribute("width","10");
      _res.writeAttribute("height","10");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Star: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","2");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","6,0 3.75,3 0,2 2.25,5 0,8 3.75,6.9 6,10 6,6.25 10,5 6,3.75");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Cube: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","0");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("rect");
      _res.writeAttribute("x","0");
      _res.writeAttribute("y","0");
      _res.writeAttribute("width","10");
      _res.writeAttribute("height","10");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::CubeOutlinedTransparent: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","0");
      _res.writeAttribute("refY","5");
      _res.writeAttribute("stroke",tlpColor2SvgColor(color));

      _res.writeStartElement("rect");
      _res.writeAttribute("x","0");
      _res.writeAttribute("y","0");
      _res.writeAttribute("width","10");
      _res.writeAttribute("height","10");
      _res.writeAttribute("fill","white");
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Cone: {
      _res.writeAttribute("viewBox","0 0 11 10");
      _res.writeAttribute("refX","1");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","0,5 0.94,8 3.44,10 6.94,10 9.88,8.15 10.94,5 9.88,2 6.94,0 3.44,0 0.94,2");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Cylinder: {
      _res.writeAttribute("viewBox","0 0 11 10");
      _res.writeAttribute("refX","1");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","0,5 0.94,8 3.44,10 6.94,10 9.88,8.15 10.94,5 9.88,2 6.94,0 3.44,0 0.94,2");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Sphere: {
      QString debut_gradient_sphere = "url(#";
      QString milieu_gradient_sphere = "RadGradientSrc" % QString::number(id_src_gradient);
      QString fin_gradient_sphere = ")";
      QString gradient_sphere = debut_gradient_sphere % milieu_gradient_sphere % fin_gradient_sphere;

      QString edgeColor = tlpColor2SvgColor(color);
      QStringList RGBList = edgeColor.split(",");
      QStringList RList = RGBList[0].split("(");
      QStringList BList = RGBList[2].split(")");
      QString Rcolor;
      QString Gcolor;
      QString Bcolor;

      _res.writeAttribute("viewBox","1 1 10 10");
      _res.writeAttribute("refX","1.5");
      _res.writeAttribute("refY","6");

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");
      _res.writeAttribute("fill",gradient_sphere);
      _res.writeEndElement();

      _res.writeStartElement("radialGradient");
      _res.writeAttribute("id",milieu_gradient_sphere);
      _res.writeAttribute("gradientUnits","userSpaceOnUse");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","0%");
      _res.writeAttribute("stop-color",tlpColor2SvgColor(color));
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/8);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/8);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/8);
      QString couleur33 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","33.3%");
      _res.writeAttribute("stop-color",couleur33);
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/5);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/5);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/5);
      QString couleur66 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","66.6%");
      _res.writeAttribute("stop-color",couleur66);
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/3);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/3);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/3);
      QString couleur100 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","100%");
      _res.writeAttribute("stop-color",couleur100);
      _res.writeEndElement();

      _res.writeEndElement(); // End element "radialGradient"
      break;
    }

    case EdgeExtremityShape::GlowSphere: {
      QString debut_gradient_sphere = "url(#";
      QString milieu_gradient_sphere = "RadGradientSrc" % QString::number(id_src_gradient);
      QString fin_gradient_sphere = ")";
      QString gradient_sphere = debut_gradient_sphere % milieu_gradient_sphere % fin_gradient_sphere;

      QString edgeColor = tlpColor2SvgColor(color);
      QStringList RGBList = edgeColor.split(",");
      QStringList RList = RGBList[0].split("(");
      QStringList BList = RGBList[2].split(")");
      QString Rcolor;
      QString Gcolor;
      QString Bcolor;

      _res.writeAttribute("viewBox","1 1 10 10");
      _res.writeAttribute("refX","1.5");
      _res.writeAttribute("refY","6");

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");
      _res.writeAttribute("fill",gradient_sphere);
      _res.writeEndElement();

      _res.writeStartElement("radialGradient");
      _res.writeAttribute("id",milieu_gradient_sphere);
      _res.writeAttribute("gradientUnits","userSpaceOnUse");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","0%");
      _res.writeAttribute("stop-color",tlpColor2SvgColor(color));
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/8);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/8);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/8);
      QString couleur33 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","33.3%");
      _res.writeAttribute("stop-color",couleur33);
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/5);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/5);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/5);
      QString couleur66 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","66.6%");
      _res.writeAttribute("stop-color",couleur66);
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/3);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/3);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/3);
      QString couleur100 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","100%");
      _res.writeAttribute("stop-color",couleur100);
      _res.writeEndElement();

      _res.writeEndElement();// End element "radialGRadient"
      _res.writeEndElement();// End context "marker"
      _res.writeEndElement();// End context "def"


      // Starting the glow effect
      _res.writeStartElement("def");

      QString debut_glow_sphere = "url(#";
      QString milieu_glow_sphere = "RadGradientSrc" % QString::number(id_src_gradient+1);
      QString fin_glow_sphere = ")";
      QString glow_sphere = debut_glow_sphere % milieu_glow_sphere % fin_glow_sphere;

      _res.writeStartElement("radialGradient");
      _res.writeAttribute("id",milieu_glow_sphere);
      _res.writeAttribute("cx","50%");
      _res.writeAttribute("cy","50%");
      _res.writeAttribute("r","50%");
      _res.writeAttribute("fx","50%");
      _res.writeAttribute("fy","50%");

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","80%");
      _res.writeAttribute("stop-color","white");
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() + RList[1].toInt()/9);
      Gcolor = QString::number(RGBList[1].toInt() + RGBList[1].toInt()/9);
      Bcolor = QString::number(BList[0].toInt() + BList[0].toInt()/9);
      QString couleur90 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","90%");
      _res.writeAttribute("stop-color",couleur90);
      _res.writeEndElement();

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","100%");
      _res.writeAttribute("stop-color","white");
      _res.writeEndElement();

      _res.writeEndElement();// End element "radialGradient"
      _res.writeEndElement();// End context "def"

      _res.writeStartElement("ellipse");
      _res.writeAttribute("cx",QString::number(coord_edge_extremity_source.getX()));
      _res.writeAttribute("cy",QString::number(coord_edge_extremity_source.getY()));
      _res.writeAttribute("rx",QString::number(size_node_src.getW()/2));
      _res.writeAttribute("ry",QString::number(size_node_src.getH()/2));
      _res.writeAttribute("fill",glow_sphere);
      _res.writeAttribute("fill-opacity","0.5");
      _res.writeEndElement();

      _res.writeStartElement("ellipse");
      _res.writeAttribute("cx",QString::number(coord_edge_extremity_source.getX()));
      _res.writeAttribute("cy",QString::number(coord_edge_extremity_source.getY()));
      _res.writeAttribute("rx",QString::number(size_node_src.getW()/2 - size_node_src.getW()/8));
      _res.writeAttribute("ry",QString::number(size_node_src.getH()/2 - size_node_src.getH()/8));
      _res.writeAttribute("fill","white");
      _res.writeAttribute("fill-opacity","0");
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::None:
      break;
    }

    if(src_anchor_shape_type != EdgeExtremityShape::GlowSphere) {
      _res.writeEndElement();// End context "marker"
      _res.writeEndElement();// End context "def"
    }
  }

  if (tgt_anchor_shape_type != EdgeExtremityShape::None) {
    // Writing the context
    _res.writeStartElement("defs");
    _res.writeStartElement("marker");

    _res.writeAttribute("id","Mtgt" % QString::number(id_tgt_shape));
    _res.writeAttribute("markerUnits","strokeWidth");
    _res.writeAttribute("orient","auto");
    _res.writeAttribute("markerWidth","4");
    _res.writeAttribute("markerHeight","3");

    switch(tgt_anchor_shape_type) {
    case EdgeExtremityShape::Arrow: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","6.7");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polyline");
      _res.writeAttribute("points","0,0 10,5 0,10 1,5");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Circle: {
      _res.writeAttribute("viewBox","1 1 10 10");
      _res.writeAttribute("refX","10.5");
      _res.writeAttribute("refY","6");

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Cross: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","6");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("path");
      _res.writeAttribute("d","M 0 0 L 10 10 M 0 10 L 10 0");
      _res.writeAttribute("stroke",tlpColor2SvgColor(color));
      _res.writeAttribute("stroke-width","2");
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Diamond: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","8.5");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","5,0 0,5 5,10 10,5");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Hexagon: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","10");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","5,0 0,3 0,7 5,10 10,7 10,3");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Pentagon: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","8.5");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","6,0 0,2 0,8 6,10 10,5");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Ring: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","9");
      _res.writeAttribute("refY","5");
      _res.writeAttribute("stroke",tlpColor2SvgColor(color));
      _res.writeAttribute("stroke-width","0.55");

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","5");
      _res.writeAttribute("cy","5");
      _res.writeAttribute("r","4.5");
      _res.writeAttribute("fill","white");
      _res.writeEndElement();

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","5");
      _res.writeAttribute("cy","5");
      _res.writeAttribute("r","3.2");
      _res.writeAttribute("fill","white");
      _res.writeAttribute("stroke-width","2.5");
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Square: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","10");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("rect");
      _res.writeAttribute("x","0");
      _res.writeAttribute("y","0");
      _res.writeAttribute("width","10");
      _res.writeAttribute("height","10");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Star: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","5");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","6,0 3.75,3 0,2 2.25,5 0,8 3.75,6.9 6,10 6,6.25 10,5 6,3.75");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Cube: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","10");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("rect");
      _res.writeAttribute("x","0");
      _res.writeAttribute("y","0");
      _res.writeAttribute("width","10");
      _res.writeAttribute("height","10");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::CubeOutlinedTransparent: {
      _res.writeAttribute("viewBox","0 0 10 10");
      _res.writeAttribute("refX","10");
      _res.writeAttribute("refY","5");
      _res.writeAttribute("stroke",tlpColor2SvgColor(color));

      _res.writeStartElement("rect");
      _res.writeAttribute("x","0");
      _res.writeAttribute("y","0");
      _res.writeAttribute("width","10");
      _res.writeAttribute("height","10");
      _res.writeAttribute("fill","white");
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Cone: {
      _res.writeAttribute("viewBox","0 0 11 10");
      _res.writeAttribute("refX","10");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","0,5 0.94,8 3.44,10 6.94,10 9.88,8.15 10.94,5 9.88,2 6.94,0 3.44,0 0.94,2");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Cylinder: {
      _res.writeAttribute("viewBox","0 0 11 10");
      _res.writeAttribute("refX","10");
      _res.writeAttribute("refY","5");

      _res.writeStartElement("polygon");
      _res.writeAttribute("points","0,5 0.94,8 3.44,10 6.94,10 9.88,8.15 10.94,5 9.88,2 6.94,0 3.44,0 0.94,2");
      _res.writeAttribute("fill",tlpColor2SvgColor(color));
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::Sphere: {
      QString debut_gradient_sphere = "url(#";
      QString milieu_gradient_sphere = "RadGradientTgt" % QString::number(id_tgt_gradient);
      QString fin_gradient_sphere = ")";
      QString gradient_sphere = debut_gradient_sphere % milieu_gradient_sphere % fin_gradient_sphere;

      QString edgeColor = tlpColor2SvgColor(color);
      QStringList RGBList = edgeColor.split(",");
      QStringList RList = RGBList[0].split("(");
      QStringList BList = RGBList[2].split(")");
      QString Rcolor;
      QString Gcolor;
      QString Bcolor;

      _res.writeAttribute("viewBox","1 1 10 10");
      _res.writeAttribute("refX","10.5");
      _res.writeAttribute("refY","6");

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");
      _res.writeAttribute("fill",gradient_sphere);
      _res.writeEndElement();

      _res.writeStartElement("radialGradient");
      _res.writeAttribute("id",milieu_gradient_sphere);
      _res.writeAttribute("gradientUnits","userSpaceOnUse");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","0%");
      _res.writeAttribute("stop-color",tlpColor2SvgColor(color));
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/8);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/8);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/8);
      QString couleur33 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","33.3%");
      _res.writeAttribute("stop-color",couleur33);
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/5);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/5);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/5);
      QString couleur66 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","66.6%");
      _res.writeAttribute("stop-color",couleur66);
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/3);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/3);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/3);
      QString couleur100 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","100%");
      _res.writeAttribute("stop-color",couleur100);
      _res.writeEndElement();

      _res.writeEndElement(); // End element "radialGradient"
      break;
    }

    case EdgeExtremityShape::GlowSphere: {
      QString debut_gradient_sphere = "url(#";
      QString milieu_gradient_sphere = "RadGradientTgt" % QString::number(id_tgt_gradient);
      QString fin_gradient_sphere = ")";
      QString gradient_sphere = debut_gradient_sphere % milieu_gradient_sphere % fin_gradient_sphere;

      QString edgeColor = tlpColor2SvgColor(color);
      QStringList RGBList = edgeColor.split(",");
      QStringList RList = RGBList[0].split("(");
      QStringList BList = RGBList[2].split(")");
      QString Rcolor;
      QString Gcolor;
      QString Bcolor;

      _res.writeAttribute("viewBox","1 1 10 10");
      _res.writeAttribute("refX","10.5");
      _res.writeAttribute("refY","6");

      _res.writeStartElement("circle");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");
      _res.writeAttribute("fill",gradient_sphere);
      _res.writeEndElement();

      _res.writeStartElement("radialGradient");
      _res.writeAttribute("id",milieu_gradient_sphere);
      _res.writeAttribute("gradientUnits","userSpaceOnUse");
      _res.writeAttribute("cx","6");
      _res.writeAttribute("cy","6");
      _res.writeAttribute("r","5");

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","0%");
      _res.writeAttribute("stop-color",tlpColor2SvgColor(color));
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/8);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/8);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/8);
      QString couleur33 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","33.3%");
      _res.writeAttribute("stop-color",couleur33);
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/5);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/5);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/5);
      QString couleur66 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","66.6%");
      _res.writeAttribute("stop-color",couleur66);
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() - RList[1].toInt()/3);
      Gcolor = QString::number(RGBList[1].toInt() - RGBList[1].toInt()/3);
      Bcolor = QString::number(BList[0].toInt() - BList[0].toInt()/3);
      QString couleur100 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","100%");
      _res.writeAttribute("stop-color",couleur100);
      _res.writeEndElement();

      _res.writeEndElement();// End element "radialGRadient"
      _res.writeEndElement();// End context "marker"
      _res.writeEndElement();// End context "def"


      // Starting the glow effect
      _res.writeStartElement("def");

      QString debut_glow_sphere = "url(#";
      QString milieu_glow_sphere = "RadGradientTgt" % QString::number(id_tgt_gradient+1);
      QString fin_glow_sphere = ")";
      QString glow_sphere = debut_glow_sphere % milieu_glow_sphere % fin_glow_sphere;

      _res.writeStartElement("radialGradient");
      _res.writeAttribute("id",milieu_glow_sphere);
      _res.writeAttribute("cx","50%");
      _res.writeAttribute("cy","50%");
      _res.writeAttribute("r","50%");
      _res.writeAttribute("fx","50%");
      _res.writeAttribute("fy","50%");

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","80%");
      _res.writeAttribute("stop-color","white");
      _res.writeEndElement();

      Rcolor = QString::number(RList[1].toInt() + RList[1].toInt()/9);
      Gcolor = QString::number(RGBList[1].toInt() + RGBList[1].toInt()/9);
      Bcolor = QString::number(BList[0].toInt() + BList[0].toInt()/9);
      QString couleur90 = "rgb(" % Rcolor % "," % Gcolor % "," % Bcolor % ")";

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","90%");
      _res.writeAttribute("stop-color",couleur90);
      _res.writeEndElement();

      _res.writeStartElement("stop");
      _res.writeAttribute("offset","100%");
      _res.writeAttribute("stop-color","white");
      _res.writeEndElement();

      _res.writeEndElement();// End element "radialGradient"
      _res.writeEndElement();// End context "def"

      _res.writeStartElement("ellipse");
      _res.writeAttribute("cx",QString::number(coord_edge_extremity_target.getX()));
      _res.writeAttribute("cy",QString::number(coord_edge_extremity_target.getY()));
      _res.writeAttribute("rx",QString::number(size_node_tgt.getW()/2));
      _res.writeAttribute("ry",QString::number(size_node_tgt.getH()/2));
      _res.writeAttribute("fill",glow_sphere);
      _res.writeAttribute("fill-opacity","0.5");
      _res.writeEndElement();

      _res.writeStartElement("ellipse");
      _res.writeAttribute("cx",QString::number(coord_edge_extremity_target.getX()));
      _res.writeAttribute("cy",QString::number(coord_edge_extremity_target.getY()));
      _res.writeAttribute("rx",QString::number(size_node_tgt.getW()/2 - size_node_tgt.getW()/8));
      _res.writeAttribute("ry",QString::number(size_node_tgt.getH()/2 - size_node_tgt.getH()/8));
      _res.writeAttribute("fill","white");
      _res.writeAttribute("fill-opacity","0");
      _res.writeEndElement();
      break;
    }

    case EdgeExtremityShape::None:
      break;
    }

    if(tgt_anchor_shape_type != EdgeExtremityShape::GlowSphere) {
      _res.writeEndElement();// End context "marker"
      _res.writeEndElement();// End context "def"
    }
  }
}
