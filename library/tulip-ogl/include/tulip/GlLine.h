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

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <vector>

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {
  /** \brief This is a general class to render an OpenGL laine
   * This class is use to render an OpenGl line
   */
  class TLP_GL_SCOPE GlLine : public GlSimpleEntity {
  public:
    /**
     * Enpty constructor
     */
    GlLine():width(1.0),factor(1),pattern(0) {}
    /**
     * Constructor with a list of coord and a list of color
     */
    GlLine(const std::vector<Coord> &points,
	   const std::vector<Color> &colors);

    virtual ~GlLine();

    /**
     * Render the line
     */
    void draw(float lod,Camera *camera);

    /**
     * Change number of points
     */
    virtual void resizePoints(const unsigned int nbPoints);
    /**
     * Change number of colors
     */
    virtual void resizeColors(const unsigned int nbColors);

    virtual const Coord& point(const unsigned int i) const;
    /**
     * Return the coord of point with given id
     */
    virtual Coord& point(const unsigned int i);
    /**
     * Add a new point with given coord and given color
     */
    virtual void addPoint(const Coord& point, const Color& color);

    /**
     * return the ith color used for draw the line
     */
    virtual const Color& color(const unsigned int i) const;
    /**
     * return the ith color used for draw the line
     */
    virtual Color& color(const unsigned int i);
    /**
     * Set line width
     */
    virtual void setLineWidth(float width);
    /**
     * Set the style of the line (documentation can be found in OpenGL manuel of fuction glLineStipple)
     */
    virtual void setLineStipple(unsigned char factor,unsigned int pattern);

    /**
     * Translate entity
     */
    virtual void translate(const Coord& mouvement);

    /**
     * Function to export data in XML
     */
    virtual void getXML(xmlNodePtr rootNode);

    /**
     * Function to set data with XML
     */
    virtual void setWithXML(xmlNodePtr rootNode);

  protected:
    std::vector<Coord> _points;
    std::vector<Color> _colors;
    float width;
    unsigned char factor;
    unsigned int pattern;
  };
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
