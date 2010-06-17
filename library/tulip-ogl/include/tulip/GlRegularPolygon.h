/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

    /**
     * Get the number of Sides
     */
    unsigned int getNumberOfSides();

    /**
     * Set the number of sides (use setNumberOfSides)
     */
    void resizePoints(const unsigned int number);

    /**
     * Set the number of sides
     */
    void setNumberOfSides(unsigned int number);

  protected :

    void computePolygon();


    Coord position;
    Size size;
    unsigned int numberOfSides;
    float startAngle;

  };
}
#endif
