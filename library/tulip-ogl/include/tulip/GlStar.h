/*
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

#ifndef GLREGULARSTAR_H
#define GLREGULARSTAR_H

#include <tulip/Size.h>
#include <tulip/GlComplexPolygon.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * @brief class to create a star
 */
class TLP_GL_SCOPE GlStar : public GlComplexPolygon {
public:
  /**
   * @brief Constructor
   *
   * The outline is the border of the regular star
   *
   * The fill is inside the regular star
   */
  GlStar(const Coord &position,
         const Size &size,
         unsigned int numberOfStarPoints,
         const Color &fillColor = Color (0, 0, 255),
         const Color &outlineColor = Color(0, 0, 0),
         bool outlined =true,
         const std::string &textureName="",
         float outlineSize=1.);
  /**
   * @brief Destructor
   */
  virtual ~GlStar();

  /**
   * @brief Get the number of star points
   */
  unsigned int getNumberOfStarPoints();

protected :

  void computeStar();


  Coord position;
  Size size;
  unsigned int numberOfStarPoints;
};

}
#endif
