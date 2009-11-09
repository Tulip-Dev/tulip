//-*-c++-*-
/*
 Author: Morgan Mathiaut
 Email : david.auber@tulip-software.org and mathiaut@labri.fr

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
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
    GlComplexPolygon(std::vector<Coord> &coords,Color fcolor,int bezier=0,const std::string &textureName = "");
    /**
     *  Constructor with a vector of coords, a fill color, an outline color, a number of decomposition in bezier mode (if bezier==0 the polygon is render without bezier mode)
     * and a textureName if you want
     */
    GlComplexPolygon(std::vector<Coord> &coords,Color fcolor,Color ocolor,int bezier=0,const std::string &textureName = "");
    /**
     * Constructor with a vector of vector of coords (the first vector of coord is the polygon and others vectors are holes in polygon), a fill color,
     * a number of decomposition in bezier mode (if bezier==0 the polygon is render without bezier mode) and a textureName if you want
     */
    GlComplexPolygon(std::vector<std::vector<Coord> >&coords,Color fcolor,int bezier=0,const std::string &textureName = "");
    /**
     * Constructor with a vector of vector of coords (the first vector of coord is the polygon and others vectors are holes in polygon), a fill color, an outline color
     * a number of decomposition in bezier mode (if bezier==0 the polygon is render without bezier mode) and a textureName if you want
     */
    GlComplexPolygon(std::vector<std::vector<Coord> >&coords,Color fcolor,Color ocolor,int bezier=0,const std::string &textureName = "");
    virtual ~GlComplexPolygon();

    /**
     * Draw the complex polygon
     */
    void draw(float lod,Camera *camera);

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
    void createPolygon(std::vector<Coord> &coords,int bezier);

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
