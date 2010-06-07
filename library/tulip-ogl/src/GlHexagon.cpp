#include "tulip/GlHexagon.h"

using namespace std;

namespace tlp {

  GlHexagon::GlHexagon(const Coord &position,
                         const Size &size,
                         const Color &fillColor,
                         const Color &outlineColor,
                         bool filled,
                         bool outlined,
                         const string &textureName,
                         float outlineSize):
  GlRegularPolygon(position,size,6,fillColor,outlineColor,filled,outlined,textureName,outlineSize)
  {
  }
  //=====================================================
  GlHexagon::~GlHexagon() {
  }
}
