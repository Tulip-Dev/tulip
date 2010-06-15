//-*-c++-*-
/*
 Author: Delorme Maxime and Mathiaut Morgan
 Email : Maxime.Delorme@gmail.com and mathiaut@labri.fr

 Last modification : 05/11/2007

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef GL_RECT_H
#define GL_RECT_H

#include <tulip/GlPolygon.h>

#define N_RECT_POINTS 4

namespace tlp {
/** \brief This is a general class for the rendering of 2D rectangles.
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

  /**
   * Function to export data in XML
   */
  virtual void getXML(xmlNodePtr rootNode);

  /**
   * Function to set data with XML
   */
  virtual void setWithXML(xmlNodePtr rootNode);

  /**
   * Draw the GlRect
   */
  virtual void draw(float lod,Camera *camera);

};
}
#endif
