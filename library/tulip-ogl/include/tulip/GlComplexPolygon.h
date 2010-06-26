/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#ifndef GLCOMPLEXPOLYGON_H
#define GLCOMPLEXPOLYGON_H

#include <vector>

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {
  /**
   * class to create a complex polygon (concave polygon or polygon with hole)
   */
  class TLP_GL_SCOPE GlComplexPolygon : public GlSimpleEntity {
  public:
    /**
     * Default constructor
     */
    GlComplexPolygon() {}
    /**
     * Constructor with a vector of coords, a fill color, a number of decomposition in bezier mode (if bezier==0 the polygon is render without bezier mode)
     * and a textureName if you want
     */
    GlComplexPolygon(const std::vector<Coord> &coords,Color fcolor,int bezier=0,const std::string &textureName = "");
    /**
     *  Constructor with a vector of coords, a fill color, an outline color, a number of decomposition in bezier mode (if bezier==0 the polygon is render without bezier mode)
     * and a textureName if you want
     */
    GlComplexPolygon(const std::vector<Coord> &coords,Color fcolor,Color ocolor,int bezier=0,const std::string &textureName = "");
    /**
     * Constructor with a vector of vector of coords (the first vector of coord is the polygon and others vectors are holes in polygon), a fill color,
     * a number of decomposition in bezier mode (if bezier==0 the polygon is render without bezier mode) and a textureName if you want
     */
    GlComplexPolygon(const std::vector<std::vector<Coord> >&coords,Color fcolor,int bezier=0,const std::string &textureName = "");
    /**
     * Constructor with a vector of vector of coords (the first vector of coord is the polygon and others vectors are holes in polygon), a fill color, an outline color
     * a number of decomposition in bezier mode (if bezier==0 the polygon is render without bezier mode) and a textureName if you want
     */
    GlComplexPolygon(const std::vector<std::vector<Coord> >&coords,Color fcolor,Color ocolor,int bezier=0,const std::string &textureName = "");
    virtual ~GlComplexPolygon();

    /**
     * Draw the complex polygon
     */
    virtual void draw(float lod,Camera *camera);

    /**
     * Add a new point in polygon
     */
    virtual void addPoint(const Coord& point);
    /**
     * Begin a new hole in the polygon
     */
    virtual void beginNewHole();

    /**
     * Set if the polygon is outlined or not
     */
    void setOutlineMode(const bool);

    /**
     * Set size of outline
     */
    void setOutlineSize(double size);

    /**
     * Get fill color of GlComplexPolygon
     */
    Color getFillColor() {return fillColor;}

    /**
     * Set fill color of GlComplexPolygon
     */
    void setFillColor(const Color &color){fillColor=color;}

    /**
     * Get outline color of GlComplexPolygon
     */
    Color getOutlineColor() {return outlineColor;}

    /**
     * Set outline color of GlComplexPolygon
     */
    void setOutlineColor(const Color &color){outlineColor=color;}

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
    void createPolygon(const std::vector<Coord> &coords,int bezier);

    std::vector<std::vector<Coord> > points;
    int currentVector;
    bool outlined;
    Color fillColor;
    Color outlineColor;
    double outlineSize;
    std::string textureName;
  };
}
#endif
