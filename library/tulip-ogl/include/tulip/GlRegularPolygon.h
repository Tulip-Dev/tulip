#ifndef GLREGULARPOLYGON_H
#define GLREGULARPOLYGON_H

#include "tulip/GlPolygon.h"

namespace tlp {
  /**
   * class to create a regular polygon
   */
  class TLP_GL_SCOPE GlRegularPolygon : public GlPolygon {
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

    /**
     * Set the start angle
     */
    void setStartAngle(float angle);

  protected :

    void computePolygon(const Coord &position,const Size &size);


    unsigned int numberOfSides;
    float startAngle;

  };
}
#endif
