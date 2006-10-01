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

#include <tulip/GlHudPolygon.h>

namespace tlp {
  /** \brief This is a general class for the rendering of 2D circles.
   *
   * This class is used as a 2D HUD for the rendering of 2D circles.
   */
  class GlHudCircle : public GlHudPolygon {
  public:
    GlHudCircle(const Coord &center = Coord(0,0,0), 
		float radius = 1., 
		const Color &outlineColor = Color(255, 0 , 0 , 255),
		const Color &fillColor = Color (0, 0, 255,255),
		bool filled = false, 
		bool outlined = true,
		float startAngle = 0.0,
		unsigned int segments = 10);
    void set(const Coord &center, float radius, float startAngle);
  };
}

#endif
