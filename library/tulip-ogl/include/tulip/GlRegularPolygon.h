#ifndef GLREGULARPOLYGON_H
#define GLREGULARPOLYGON_H

#include "tulip/GlAbstractPolygon.h"

namespace tlp {
  /**
   * class to create a regular polygon
   */
  class TLP_GL_SCOPE GlRegularPolygon : public GlAbstractPolygon {
  public:
    /**
     * Constructor
     */
    GlRegularPolygon(const Coord &position,
                     const Size &size,
                     unsigned int numberOfSides,
                     const Color &outlineColor = Color(255, 0 , 0 , 255),
                     const Color &fillColor = Color (0, 0, 255,255),
                     bool filled = true,
                     bool outlined =true,
                     const std::string &textureName="",
                     float outlineSize=1.);
    /**
     * Default empty destructor
     */
    virtual ~GlRegularPolygon();

  protected :

    unsigned int numberOfSides;

  };
}
#endif
