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
#ifndef SHAPE_H
#define SHAPE_H

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>

#include <QXmlStreamWriter>
#include <QString>

struct ExtremityShape {

  static void Arrow(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Circle(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Cross(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Diamond(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Hexagon(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Pentagon(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Ring(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Square(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Star(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Cube(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void CubeOutlinedTransparent(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Cone(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Cylinder(QXmlStreamWriter &res, const QString &color, bool tgt);
  static void Sphere(QXmlStreamWriter &res, const tlp::Color &color, bool tgt,
                     const unsigned id_src_gradient);
  static void GlowSphere(QXmlStreamWriter &res, const tlp::Color &color, bool tgt,
                         const unsigned id_gradient);
  static void Icon(QXmlStreamWriter &res, const QString &color, const std::string &iconName,
                   bool tgt);
};

#endif // SHAPE_H
