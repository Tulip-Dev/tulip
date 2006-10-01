//-*-c++-*-
/*
 Author: Auber David
 Email : david.auber@tulip-software.org
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef GL_HUD_POLYGON_H
#define GL_HUD_POLYGON_H

#include <tulip/Color.h>
#include <tulip/Coord.h>

#include "GlHud.h"

namespace tlp {
  class TLP_GL_SCOPE GlHudPolygon : public GlHud {
  public:
    GlHudPolygon(const vector<tlp::Coord> &points, 
		 const vector<tlp::Color> &fillColors,
		 const vector<tlp::Color> &outlineColors,
		 const bool filled,
		 const bool outlined);
    GlHudPolygon(const unsigned int nbPoints = 3u, 
		 const unsigned int nbFillColors = 1u, 
		 const unsigned int nbOutlineColors = 1u, 
		 const bool filled = true,
		 const bool outlined = true);
    virtual ~GlHudPolygon();

    void draw(tlp::GlGraph *);
    
    virtual void resizePoints(const unsigned int nbPoints);
    virtual void resizeColors(const unsigned int nbColors);
    
    virtual const tlp::Coord& point(const unsigned int i) const;
    virtual tlp::Coord& point(const unsigned int i);
    
    //return the ith color used for filling the polygon
    virtual const tlp::Color& fcolor(const unsigned int i) const;
    virtual tlp::Color& fcolor(const unsigned int i);

    //return the ith color used for outlining the polygon
    virtual const tlp::Color& ocolor(const unsigned int i) const;
    virtual tlp::Color& ocolor(const unsigned int i);
    
    void setFillMode(const bool);
    void setOutlineMode(const bool);

  protected:
    std::vector<tlp::Coord> _points;
    std::vector<tlp::Color> _fillColors;
    std::vector<tlp::Color> _outlineColors;
    bool _filled;
    bool _outlined;
  };
}
#endif
