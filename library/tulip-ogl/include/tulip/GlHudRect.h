//-*-c++-*-
/*
 Author: Delorme Maxime
 Email : Maxime.Delorme@gmail.com

 Last modification : 05/08/2005 (fr) 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef GL_HUD_RECT_H
#define GL_HUD_RECT_H

#include <tulip/GlHud.h>

#define N_RECT_POINTS 4

/** \brief This is a general class for the rendering of 2D rectangles.
 *
 * This class is used as a 2D HUD for the rendering of 2D rectangles.
 */

class TLP_GL_SCOPE GlHudRect : public GlHud {
 protected:

  Coord topLeftPos; /**< Top Left coordinate of the rectangle */
  Coord bottomRightPos; /**< Bottom Right coordinate of the rectangle */

  Color topLeftCol; /**< Top Left color of the rectangle */
  Color bottomRightCol; /**< Bottom Right color of the rectangle */



 public:

  /**
   * Default constructor 
   */
  GlHudRect();
  /**
   * Constructor
   */
  GlHudRect(const Coord &topLeftPos, const Coord &bottomRightPos, const Color &topLeftCol, const Color &bottomRightCol);

  /**
   * Destructor
   */
  virtual ~GlHudRect();

  /**
   * Virtual function used to draw the rectangle
   */
  virtual void draw(GlGraph*);

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
};

#endif
