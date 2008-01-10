//-*-c++-*-
/*
 Author: Auber David and Morgan Mathiaut
 Email : david.auber@tulip-software.org and mathiaut@labri.fr
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef GLPOLYGON_H
#define GLPOLYGON_H

#include <vector>

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {
  class TLP_GL_SCOPE GlPolygon : public GlSimpleEntity {
  public:
    GlPolygon(const bool filled = true,
	      const bool outlined= true);
    GlPolygon(const std::vector<Coord> &points, 
	      const std::vector<Color> &fillColors,
	      const std::vector<Color> &outlineColors,
		 const bool filled,
		 const bool outlined);
    GlPolygon(const unsigned int nbPoints = 3u, 
		 const unsigned int nbFillColors = 1u, 
		 const unsigned int nbOutlineColors = 1u, 
		 const bool filled = true,
		 const bool outlined = true);
    virtual ~GlPolygon();

    void draw(float lod,Camera *camera);
    
    virtual void resizePoints(const unsigned int nbPoints);
    virtual void resizeColors(const unsigned int nbColors);
    
    virtual const Coord& point(const unsigned int i) const;
    virtual Coord& point(const unsigned int i);
    virtual void addPoint(const Coord& point, const Color& fcolor,const Color& ocolor);
    
    //return the ith color used for filling the polygon
    virtual const Color& fcolor(const unsigned int i) const;
    virtual Color& fcolor(const unsigned int i);

    //return the ith color used for outlining the polygon
    virtual const Color& ocolor(const unsigned int i) const;
    virtual Color& ocolor(const unsigned int i);
    
    void setFillMode(const bool);
    void setOutlineMode(const bool);

  protected:
    std::vector<Coord> _points;
    std::vector<Color> _fillColors;
    std::vector<Color> _outlineColors;
    bool _filled;
    bool _outlined;
  };
}
#endif
