#ifndef GLHEXAGON_H
#define GLHEXAGON_H

#include "tulip/GlRegularPolygon.h"

namespace tlp {
  /**
   * class to create an hexagon
   */
  class TLP_GL_SCOPE GlHexagon : public GlRegularPolygon {
  public:
    /**
     * Constructor
     */
    GlHexagon(const Coord &position,
               const Size &size,
               const Color &outlineColor = Color(255, 0 , 0 , 255),
               const Color &fillColor = Color (0, 0, 255,255),
               bool filled = true,
               bool outlined =true,
               const std::string &textureName="",
               float outlineSize=1.);
    /**
     * Default empty destructor
     */
    virtual ~GlHexagon();

  };
}
#endif
