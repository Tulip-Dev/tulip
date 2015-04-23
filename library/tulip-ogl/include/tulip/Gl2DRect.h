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

#ifndef Tulip_GL2DRECT_H
#define Tulip_GL2DRECT_H

#include <string>

#include <tulip/GlRect.h>

namespace tlp {
/**
 * @ingroup OpenGL
 *
 * @brief Create a 2D rectangle
 *
 * This rect add screen percentage system (for example you can create a rectangle that start on (10%,10%) and finish on (90%90%)
 * If you whan a simple 2D rectangle with normal coordinates use GlRect class
 * @see GlRect
 */
class TLP_GL_SCOPE Gl2DRect : public GlRect {

public:
  /**
   * @brief Don't use this constructor
   */
  Gl2DRect();

  /**
   * @brief Constructor
   *
   * @param top coordinate
   * @param bottom coordinate
   * @param left coordinate
   * @param right coordinate
   * @param textureName path to a texture
   * @param inPercent if true : top/bottom/left/right is used as in percent parameters
   */
  Gl2DRect(float top,float bottom,float left, float right,
           const std::string& textureName,
           bool inPercent=false);

  /**
   * @brief Constructor
   *
   * @param top coordinate
   * @param bottom coordinate
   * @param left coordinate
   * @param right coordinate
   * @param textureName path to a texture
   * @param xInv if true : use viewportX - left and viewportX - right
   * @param yInv if true : use viewportY - top and viewportY - bottom
   */
  Gl2DRect(float bottom,float left,float height, float width,
           const std::string& textureName,
           bool xInv, bool yInv);

  /**
   * @brief Destructor
   */
  virtual ~Gl2DRect() {}

  virtual BoundingBox getBoundingBox();

  virtual void draw(float lod,Camera *camera);

  virtual void translate(const Coord& mouvement);

  /**
   * @brief Set texture
   */
  virtual void setTexture(const std::string &name);

  /**
   * @brief Get texture
   */
  virtual std::string getTexture();

  /**
   * @brief Set coordinates of rectangle
   */
  virtual void setCoordinates(float bottom, float left, float width, float height);

  virtual void getXML(std::string &outString);

  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

protected:

  float top;
  float bottom;
  float left;
  float right;
  bool inPercent;
  bool xInv;
  bool yInv;

};

}
#endif
