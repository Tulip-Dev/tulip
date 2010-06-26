/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/GlGraphStaticData.h"

#include <iostream>

using namespace std;

namespace tlp {

  const int GlGraphStaticData::edgeShapesCount = 4;
  int GlGraphStaticData::edgeShapeIds[edgeShapesCount] = {BEZIERSHAPE, POLYLINESHAPE, SPLINESHAPE, CUBICBSPLINE};
  string GlGraphStaticData::labelPositionNames[] = { string("Center"), string("Top"), string("Bottom"),string("Left"), string("Right") };

  string GlGraphStaticData::edgeShapeName(int id) {
    switch(id) {
    case POLYLINESHAPE:
      return string("Polyline");
    case BEZIERSHAPE:
      return string("Bezier Curve");
    case SPLINESHAPE:
      return string("Catmull-Rom Spline");
    case CUBICBSPLINE:
      return string("Cubic B-Spline");
    default:
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid edge shape id" << endl;
      return string("invalid shape id");
    }
  }
  
  int GlGraphStaticData::edgeShapeId(string name) {
    if (name == edgeShapeName(POLYLINESHAPE))
      return POLYLINESHAPE;
    if (name == edgeShapeName(BEZIERSHAPE))
      return BEZIERSHAPE;
    if (name == edgeShapeName(SPLINESHAPE))
      return SPLINESHAPE;
    if (name == edgeShapeName(CUBICBSPLINE))
      return CUBICBSPLINE;
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "Invalid edge shape name" << endl;
    return -1;
  }
  
  
  string GlGraphStaticData::labelPositionName(int id) {
    if (id > -1 && id < 5)
      return labelPositionNames[id];
    return string("invalid label position id");
  }

  int GlGraphStaticData::labelPositionId(string name) {
    for (int i = 0; i < 5; i++) {
      if (name == labelPositionNames[i])
	return i;
    }
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "Invalid label position name" << endl;
    return -1;
  }
  
}
