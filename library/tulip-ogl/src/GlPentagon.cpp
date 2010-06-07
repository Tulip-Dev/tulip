#include "tulip/GlPentagon.h"

using namespace std;

namespace tlp {

  GlPentagon::GlPentagon(const Coord &position,
                         const Size &size,
                         const Color &fillColor,
                         const Color &outlineColor,
                         bool filled,
                         bool outlined,
                         const string &textureName,
                         float outlineSize):
  GlRegularPolygon(position,size,5,fillColor,outlineColor,filled,outlined,textureName,outlineSize)
  {
  }
  //=====================================================
  GlPentagon::~GlPentagon() {
  }
}
