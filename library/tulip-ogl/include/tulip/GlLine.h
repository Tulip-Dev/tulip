//-*-c++-*-
/*
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef GLLINE_H
#define GLLINE_H

#include <vector>

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {
  class TLP_GL_SCOPE GlLine : public GlSimpleEntity {
  public:
    GlLine() {}
    GlLine(const std::vector<Coord> &points, 
	   const std::vector<Color> &colors);

    virtual ~GlLine();

    void draw(float lod);
    
    virtual void resizePoints(const unsigned int nbPoints);
    virtual void resizeColors(const unsigned int nbColors);
    
    virtual const Coord& point(const unsigned int i) const;
    virtual Coord& point(const unsigned int i);
    virtual void addPoint(const Coord& point, const Color& color);
    
    //return the ith color used for draw the line
    virtual const Color& color(const unsigned int i) const;
    virtual Color& color(const unsigned int i);

  protected:
    std::vector<Coord> _points;
    std::vector<Color> _colors;
  };
}
#endif
