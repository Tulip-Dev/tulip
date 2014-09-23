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
#include <tulip/GlGraphStaticData.h>
#include <tulip/TulipViewSettings.h>

#include <iostream>

using namespace std;

namespace tlp {

const int GlGraphStaticData::edgeShapesCount = 4;
int GlGraphStaticData::edgeShapeIds[edgeShapesCount] = {EdgeShape::Polyline, EdgeShape::BezierCurve, EdgeShape::CatmullRomCurve, EdgeShape::CubicBSplineCurve};
string GlGraphStaticData::labelPositionNames[] = { string("Center"), string("Top"), string("Bottom"),string("Left"), string("Right") };

string GlGraphStaticData::edgeShapeName(int id) {
  switch(id) {
  case EdgeShape::Polyline:
    return string("Polyline");

  case EdgeShape::BezierCurve:
    return string("Bezier Curve");

  case EdgeShape::CatmullRomCurve:
    return string("Catmull-Rom Spline");

  case EdgeShape::CubicBSplineCurve:
    return string("Cubic B-Spline");

  default:
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid edge shape id" << endl;
    return string("invalid shape id");
  }
}

int GlGraphStaticData::edgeShapeId(const string& name) {
  if (name == edgeShapeName(EdgeShape::Polyline))
    return EdgeShape::Polyline;

  if (name == edgeShapeName(EdgeShape::BezierCurve))
    return EdgeShape::BezierCurve;

  if (name == edgeShapeName(EdgeShape::CatmullRomCurve))
    return EdgeShape::CatmullRomCurve;

  if (name == edgeShapeName(EdgeShape::CubicBSplineCurve))
    return EdgeShape::CubicBSplineCurve;

  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  tlp::warning() << "Invalid edge shape name" << endl;
  return -1;
}


string GlGraphStaticData::labelPositionName(int id) {
  if (id > -1 && id < 5)
    return labelPositionNames[id];

  return string("invalid label position id");
}

int GlGraphStaticData::labelPositionId(const string& name) {
  for (int i = 0; i < 5; i++) {
    if (name == labelPositionNames[i])
      return i;
  }

  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  tlp::warning() << "Invalid label position name" << endl;
  return -1;
}

}
