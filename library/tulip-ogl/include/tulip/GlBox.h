//-*-c++-*-
/*
 Author: Delorme Maxime and Mathiaut Morgan
 Email : Maxime.Delorme@gmail.com and mathiaut@labri.fr

 Last modification : 05/11/07

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLBOX_H
#define Tulip_GLBOX_H

#include "tulip/GlSimpleEntity.h"
#include "tulip/GlPolygon.h"

#include "tulip/Coord.h"
#include "tulip/Color.h"
#include "tulip/Size.h"

#define N_BOX_POINTS 8
#define N_BOX_FACES  6
#define N_QUAD_POINTS 4

namespace tlp {
/** \brief General class used to render boxes as GlEntity.
 *
 * This class is a generic class to render boxes as GlEntity.
 */
class TLP_GL_SCOPE GlBox : public GlSimpleEntity
{
 protected:
  Coord* position; /**< The position of the center of the box*/
  Color* color; /**< The color of the box */
  Size* size; /**< size is the "radius" of the box */
  
  Coord* points[N_BOX_POINTS]; /**< The coordinates of each of the 8 points of the box. 
				  \attention points[0] = front top left
				  \attention points[1] = front top right
				  \attention points[2] = front bottom right
				  \attention points[3] = front bottom left
				  \attention points[4] = back top left
				  \attention points[5] = back top right
				  \attention points[6] = back bottom right
				  \attention points[7] = back bottom left */

  GlPolygon* faces[N_BOX_FACES]; /**< Stores a GlPolygon per face */

  /**
   * The default constructor
   *
   * \attention It's usage is forbidden, so that's why it has been put in protected members.
   */
  GlBox();

  /**
   * Function used to compute the points of the box from a center and a size.
   */
  void computePoints();

  /**
   * Function used to compute the GlADQuad from the points of the box.
   */
  void computeFaces();

 public:  

  /**
   * Constructor from size
   *
   * \param position The center of the box.
   * \param size The length of each dimension of the box.
   * \param color The color of the box.
   */
  GlBox(const Coord& position, const Size &size, const Color& color);

  /**
   * Constructor from points
   *
   * \param points Each point of the box. c.f. the variable "points" to know the placement.
   * \param color The color of the box.
   */
  GlBox(Coord points[8], const Color& color);

  /**
   * Constructor from bounding box
   *
   * \param frontTopLeft The position of the point at the front-top-left of the box (points[0]).
   * \param backbottomRight The position of the point at the back-bottom-right of the box (points[6]).
   * \param color The color of the box.
   */
  GlBox(const Coord& frontTopLeft, const Coord& backBottomRight, const Color& color);

  /**
   * Destructor.
   */
  virtual ~GlBox();

  /**
   * Virtual function used to draw the box.
   */
  virtual void draw(float lod);

  /**
   * Accessor in writing to the size.
   */
  void setSize(const Size& size);

  /**
   * Accessor in writing to the position.
   */
  void setPosition(const Coord& position);

  /**
   * Accessor in reading to the size.
   */
  Size* getSize() const;
};
}
#endif
