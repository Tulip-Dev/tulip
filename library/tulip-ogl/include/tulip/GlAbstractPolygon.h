//-*-c++-*-
/*
 Author: Auber David and Morgan Mathiaut
 Email : david.auber@tulip-software.org and mathiaut@labri.fr

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef GLABSTRACTPOLYGON_H
#define GLABSTRACTPOLYGON_H

#include <vector>

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

#include "tulip/GlSimpleEntity.h"
#include "tulip/GlTools.h"

namespace tlp {
  /**
   * class to create a abstract polygon
   */
  class TLP_GL_SCOPE GlAbstractPolygon : public GlSimpleEntity {
  public:
    /**
     * Constructor
     */
    GlAbstractPolygon();
    /**
     * Default empty destructor
     */
    virtual ~GlAbstractPolygon();

    /**
     * Get if the polygon is filled or not
     */
    bool getFillMode();

    /**
     * Set if the polygon is filled or not
     */
    void setFillMode(const bool);

    /**
     * Get if the polygon is outlined or not
     */
    bool getOutlineMode();

    /**
     * Set if the polygon is outlined or not
     */
    void setOutlineMode(const bool);

    /**
     * Get the ith color used to filling the polygon
     */
    Color getFillColor(unsigned int i);

    /**
     * Set the ith color used to filling the polygon
     */
    void setFillColor(unsigned int i, const Color &color);

    /**
     * Set color used to filling the whole polygon
     */
    void setFillColor(const Color &color);

    /**
     * Get the ith color used to outlining the polygon
     */
    Color getOutlineColor(unsigned int i);

    /**
     * Set the ith color used to outlining the polygon
     */
    void setOutlineColor(unsigned int i, const Color &color);

    /**
     * Set the color used to outlining the whole polygon
     */
    void setOutlineColor(const Color &color);

    /**
     * Get the textureName
     */
    std::string getTextureName();

    /**
     * Set the textureName
     */
    void setTextureName(const std::string &name);

    /**
     * Get the outline size
     */
    float getOutlineSize();

    /**
     * Set the outline size
     */
    void setOutlineSize(float size);

    /**
     * Draw the polygon
     */
    virtual void draw(float lod,Camera *camera);

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

    /**
     * set Coords of the polygon
     */
    virtual void setPoints(const std::vector<Coord> &points);

    /**
     * set fill colors of the polygon
     */
    virtual void setFillColors(const std::vector<Color> &colors);

    /**
     * set outline colors of the polygon
     */
    virtual void setOutlineColors(const std::vector<Color> &colors);

    virtual void clearGenerated();

    std::vector<Coord> points;
    std::vector<Color> fillColors;
    std::vector<Color> outlineColors;
    bool filled;
    bool outlined;
    std::string textureName;
    float outlineSize;

    std::vector<Coord> normalArray;
    GLubyte *indices;
    GLfloat *texArray;

    bool generated;
    GLuint buffers[6];

  public :

    //=====================================================
    // Deprecated
    //=====================================================

    /**
     * return the ith color used for filling the polygon
     * @deprecated use getFillColor
     */
    const Color& fcolor(const unsigned int i) const;

    /**
     * return the ith color used for filling the polygon
     * @deprecated use getFillColor
     */
    Color& fcolor(const unsigned int i);

    /**
     * Set the ith color used for filling the polygon
     * @deprecated use setFillColor
     */
    void setFColor(const unsigned int i,const Color &color);

    /**
     * return the ith color used for outlining the polygon
     * @deprecated use getOutlineColor
     */
    const Color& ocolor(const unsigned int i) const;

    /**
     * return the ith color used for outlining the polygon
     * @deprecated use getOutlineColor
     */
    virtual Color& ocolor(const unsigned int i);

    /**
     * Set the ith color used for outlining the polygon
     * @deprecated use setOutlineColor
     */
    virtual void setOColor(const unsigned int i,const Color &color);
  };
}
#endif
