#include "tulip/GlTriangle.h"



using namespace std;

namespace tlp {

  GlTriangle::GlTriangle(const Coord &position,
                         const Size &size,
                         const Color &fillColor,
                         const Color &outlineColor,
                         bool filled,
                         bool outlined,
                         const string &textureName,
                         float outlineSize):
  GlRegularPolygon(position,size,3,fillColor,outlineColor,filled,outlined,textureName,outlineSize)
  {
  }
  //=====================================================
  GlTriangle::~GlTriangle() {
  }
}
