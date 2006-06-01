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

#ifndef GL_HUD_H
#define GL_HUD_H

#include <tulip/GlAugmentedDisplay.h>

namespace tlp {

/** \brief General class for the rendering of 2D augmented displays
 *
 */
class GlHud : public GlAugmentedDisplay {
  
public:
  
  /**
   * Constructor
   */
  GlHud();

  /**
   * Destructor
   */
  virtual ~GlHud();

  /**
   * Function used to draw ... This is a pure virtual function
   * \attention Must absolutely be called with unsetup in every derivated class
   * \attention You call setup(), you render, you call unsetup()
   * \attention This must not be called withing glBegin/glEnd statements.
   */
  virtual void draw(GlGraph*);

  /**
   * Function used to setup the display
   */
  virtual void setup();

  /**
   * Function used to unsetup the display
   * \attention Must absolutely be called with setup in every derivated class
   * \attention You call setup(), you render, you call unsetup()
   * \attention This must not be called withing glBegin/glEnd statements.
   */
  virtual void unsetup();

  /**
   * Function used to transform the coordinates of a vertice, so that the user can define screen coordinates.
   */
  virtual Coord transformCoordinates(const Coord &position);
};

}


#endif
