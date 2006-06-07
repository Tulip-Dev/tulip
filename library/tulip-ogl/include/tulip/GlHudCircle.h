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

#ifndef GL_HUD_CIRCLE_H
#define GL_HUD_CIRCLE_H

#include <tulip/GlHud.h>
#include <vector>

namespace tlp {
/** \brief This is a general class for the rendering of 2D circles.
 *
 * This class is used as a 2D HUD for the rendering of 2D circles.
 */
class GlHudCircle : public GlHud {

 protected:
  Coord center; /**< The center position of the circle */
  Color color; /**< The color of the circle */
  int radius; /**< The radius of the circle */
  int segments; /**< The number of segments of the circle. Higher values imply a smoother circle */

  vector<Coord> points; /**< The control points of the circle*/

  /**
   * Default constructor : forbidden
   */
  GlHudCircle();

  /**
   * Function computing the control points of the circle from the center, the radius and the number of segments.
   */
  void computeCircle();

 public:

  /**
   * Constructor
   */
  GlHudCircle(const Coord &center, const Color &color, int radius, int segments);

  /**
   * Destructor
   */
  virtual ~GlHudCircle();

  /**
   * Function drawing the circle
   */
  virtual void draw(GlGraph*);

  /**
   * Accessor in reading to the center of the circle
   */
  virtual Coord getCenter();

  /**
   * Accessor in reading to the color of the circle
   */
  virtual Color getColor();

  /**
   * Accessor in reading to the radius of the circle
   */
  virtual int getRadius();

  /**
   * Accessor in reading to the segment of the circle
   */
  virtual int getSegments();

  /**
   * Accessor in writing to the center of the circle
   */
  virtual void setCenter(const Coord &center);

  /**
   * Accessor in writing to the color of the circle
   */
  virtual void setColor(const Color &color);

  /**
   * Accessor in writing to the radius of the circle
   */
  virtual void setRadius(int radius);

  /**
   * Accessor in writing to the number of segments
   */
  virtual void setSegments(int segments);
};
}

#endif
