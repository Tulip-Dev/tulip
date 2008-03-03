#include "tulip/GlGraphStaticData.h"

#include <iostream>

using namespace std;

namespace tlp {

  const int GlGraphStaticData::edgeShapesCount = 3;
  int GlGraphStaticData::edgeShapeIds[edgeShapesCount] = {BEZIERSHAPE, POLYLINESHAPE, SPLINESHAPE};
  string GlGraphStaticData::labelPositionNames[] = { string("Center"), string("Top"), string("Bottom"),string("Left"), string("Right") };

  string GlGraphStaticData::edgeShapeName(int id) {
    switch(id) {
    case POLYLINESHAPE:
      return string("Polyline");
    case BEZIERSHAPE:
      return string("Bezier Curve");
    case SPLINESHAPE:
      return string("Spline Curve");
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
