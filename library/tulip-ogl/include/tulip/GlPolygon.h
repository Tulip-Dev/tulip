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
  /**
   * class to create a polygon
   */
  class TLP_GL_SCOPE GlPolygon : public GlSimpleEntity {
  public:
    /**
     * Constructor where specify if the polygon is filled, is outlines and the outline size
     */
    GlPolygon(const bool filled = true,
	      const bool outlined= true,
	      const int outlineSize = 1);
    /**
     * Constructor with a vector of point, a vector of fill color, a vector of outline color and if the polygon is filled, is outlined and the outline size
     */
    GlPolygon(const std::vector<Coord> &points,
	      const std::vector<Color> &fillColors,
	      const std::vector<Color> &outlineColors,
	      const bool filled,
	      const bool outlined,
	      const int outlineSize = 1);
    /**
     * Constructor with a number of point, a number of fill color, a number of outline color and if the polygon is filled, outlined and the ouline size
     */
    GlPolygon(const unsigned int nbPoints,
	      const unsigned int nbFillColors,
	      const unsigned int nbOutlineColors,
	      const bool filled = true,
	      const bool outlined = true,
	      const int outlineSize = 1);
    virtual ~GlPolygon();

    /**
     * Draw the polygon
     */
    void draw(float lod,Camera *camera);

    /**
     * Change number of point of the polygon
     */
    virtual void resizePoints(const unsigned int nbPoints);
    /**
     * Change number of colors of the polygon
     */
    virtual void resizeColors(const unsigned int nbColors);

    /**
     * return the ith point
     */
    virtual const Coord& point(const unsigned int i) const;
    /**
     * return the ith point
     */
    virtual Coord& point(const unsigned int i);
    /**
     * Add a new point with a Coord, a fill color and an ouline color
     */
    virtual void addPoint(const Coord& point, const Color& fcolor,const Color& ocolor);

    /**
     * return the ith color used for filling the polygon
     */
    virtual const Color& fcolor(const unsigned int i) const;
    /**
     * return the ith color used for filling the polygon
     */
    virtual Color& fcolor(const unsigned int i);
    /**
     * Set the ith color used for filling the polygon
     */
    virtual void setFColor(const unsigned int i,const Color &color);

    /**
     * return the ith color used for outlining the polygon
     */
    virtual const Color& ocolor(const unsigned int i) const;
    /**
     * return the ith color used for outlining the polygon
     */
    virtual Color& ocolor(const unsigned int i);
    /**
     * Set the ith color used for outlining the polygon
     */
    virtual void setOColor(const unsigned int i,const Color &color);

    /**
     * Set if the polygon is filled or not
     */
    void setFillMode(const bool);
    /**
     * Set if the polygon is outlined or not
     */
    void setOutlineMode(const bool);

    /**
     * Translate entity
     */
    virtual void translate(const Coord& mouvement);

    /**
     * Function to export data and type in XML
     */
    virtual void getXML(xmlNodePtr rootNode);

    /**
     * Function to export data in XML
     */
    virtual void getXMLOnlyData(xmlNodePtr rootNode);

    /**
     * Function to set data with XML
     */
    virtual void setWithXML(xmlNodePtr rootNode);

  protected:
    std::vector<Coord> points;
    std::vector<Color> fillColors;
    std::vector<Color> outlineColors;
    bool filled;
    bool outlined;
    int outlineSize;
  };
}
#endif
