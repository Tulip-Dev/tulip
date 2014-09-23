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


#ifndef GLPOLYGON_H
#define GLPOLYGON_H

#include <vector>

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>
#include <tulip/GlAbstractPolygon.h>

namespace tlp {
/**
 * @ingroup OpenGL
 * @brief Class to create a polygon GlEntity
 *
 */
class TLP_GL_SCOPE GlPolygon : public GlAbstractPolygon {
public:
  /**
   * @brief Constructor where specify if the polygon is filled, is outlines the texture name and the outline size
   */
  GlPolygon(const bool filled = true,
            const bool outlined= true,
            const std::string &textureName="",
            const float outlineSize = 1);
  /**
   * @brief Constructor with a vector of point, a vector of fill color, a vector of outline color and if the polygon is filled, is outlined and the outline size
   */
  GlPolygon(const std::vector<Coord> &points,
            const std::vector<Color> &fillColors,
            const std::vector<Color> &outlineColors,
            const bool filled,
            const bool outlined,
            const std::string &textureName="",
            const float outlineSize = 1);
  /**
   * @brief Constructor with a number of point, a number of fill color, a number of outline color and if the polygon is filled, outlined and the ouline size
   */
  GlPolygon(const unsigned int nbPoints,
            const unsigned int nbFillColors,
            const unsigned int nbOutlineColors,
            const bool filled = true,
            const bool outlined = true,
            const std::string &textureName="",
            const float outlineSize = 1);
  virtual ~GlPolygon();

  /**
   * @brief Change number of point of the polygon
   */
  virtual void resizePoints(const unsigned int nbPoints);
  /**
   * @brief Change number of colors of the polygon
   */
  virtual void resizeColors(const unsigned int nbColors);

  /**
   * @brief return the ith point
   */
  virtual const Coord& point(const unsigned int i) const;
  /**
   * @brief return the ith point
   */
  virtual Coord& point(const unsigned int i);
};

}
#endif
