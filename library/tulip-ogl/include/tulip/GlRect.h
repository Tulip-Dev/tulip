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


#ifndef GL_RECT_H
#define GL_RECT_H

#include <tulip/GlPolygon.h>
#include <tulip/Size.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * \brief This is a general class for the rendering of 2D rectangles.
 *
 * This class is used as a 2D HUD for the rendering of 2D rectangles.
 */

class TLP_GL_SCOPE GlRect : public GlPolygon {
protected:

public:
  /**
   * Constructor : with topLeft/bottomRight coords, topLeft/bottomRight colors and if GlRect is filled/outlined
   */
  GlRect(const Coord &topLeftPos, const Coord &bottomRightPos, const Color &topLeftCol, const Color &bottomRightCol, bool filled=true, bool outlined=false);

  /**
   * Constructor : with center coords and size, fill color and outline color
   */
  GlRect(const Coord &center, const float width, const float height, const Color &fillColor, const Color &outlineColor);


  /**
   * Constructor : with GlRect is filled/outlined
   */
  GlRect(bool filled=true, bool outlined=false);

  /**
   * Destructor
   */
  virtual ~GlRect();

  /**
   * Accessor in reading to the Top Left Corner of the rectangle
   */
  virtual Coord getCenter();

  /**
   * Set the center and the size of the GlRect
   */
  void setCenterAndSize(const Coord &center, const Size &size);

  /**
   * Accessor in reading to the Top Left Corner of the rectangle
   */
  virtual Coord getTopLeftPos();

  /**
   * Accessor in reading to the Bottom Right Corner of the rectangle
   */
  virtual Coord getBottomRightPos();

  /**
   * Accessor in reading to the Top Left Corner Color of the rectangle
   */
  virtual Color getTopLeftColor();

  /**
   * Accessor in reading to the Bottom Right Corner Color of the Rectangle
   */
  virtual Color getBottomRightColor();

  /**
   * Accessor in writing to the Top Left Corner of the rectangle
   */
  virtual void setTopLeftPos(const Coord &topLeftPos);

  /**
   * Accessor in writing to the Bottom Right Corner Color of the rectangle
   */
  virtual void setBottomRightPos(const Coord &bottomRightPos);

  /**
   * Accessor in writing to the Top Left Corner of the rectangle
   */
  virtual void setTopLeftColor(const Color &topLeftCol);

  /**
   * Accessor in writing to the Bottom Right Corner Color of the rectangle
   */
  virtual void setBottomRightColor(const Color &bottomRightCol);

  /**
   * Is the point in the rectangle ?
   */
  bool inRect(double x, double y);

  virtual void draw(float lod,Camera *camera);

};

}
#endif
