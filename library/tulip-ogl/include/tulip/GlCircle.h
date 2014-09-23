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


#ifndef GLCIRCLE_H
#define GLCIRCLE_H

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/GlRegularPolygon.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * @brief Class used to render circles as GlEntity.
 *
 * If you want a circle : use this class
 */
class TLP_GL_SCOPE GlCircle : public GlRegularPolygon {
public:
  /**
   * @brief Constructor
   *
   * @param startAngle if start angle is equal to 0, the circle construction begin with the top center point
   * @param segment number of triangle used to render the circle
   */
  GlCircle(const Coord &center = Coord(0,0,0),
           float radius = 1.,
           const Color &outlineColor = Color(255, 0 , 0 , 255),
           const Color &fillColor = Color (0, 0, 255,255),
           bool filled = false,
           bool outlined = true,
           float startAngle = 0.0,
           unsigned int segments = 10);

  /**
   * @brief Write-acess Accessor to the center, the radius and the startAngle of the circle.
   */
  void set(const Coord &center, float radius, float startAngle);

  /**
   * @brief Function to export data in XML
   */
  virtual void getXML(std::string &outString);
};

}

#endif
