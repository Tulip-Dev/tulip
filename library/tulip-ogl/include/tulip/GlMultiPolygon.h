//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 11/12/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLMULTIPOLYGON_H
#define Tulip_GLMULTIPOLYGON_H

#include <vector>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>
#include <tulip/Coord.h>

#include "tulip/GlPolygon.h"
#include "tulip/GlSimpleEntity.h"

namespace tlp {

  /**
   * Entity who represent a vector of polygons
   */
  class TLP_GL_SCOPE GlMultiPolygon : public GlSimpleEntity {
  
  public:
    
    /**
     * Constructor same as GlPolygon
     */
    GlMultiPolygon(bool filled=true,bool outlined=true,const Color& fillColor=Color(0,0,0,255),const Color& outlineColor=Color(0,0,0,255));

    /**
     * Add a polygon with the Coord poly
     */
    void addPolygon(const std::vector<Coord> &poly);

    /**
     * Draw all polygons
     */
    virtual void draw(float lod,Camera *camera);

    /**
     * Translate entity
     */
    virtual void translate(const Coord& mouvement);

    /**
     * Get the data in XML form
     */
    void getXML(xmlNodePtr rootNode);
    
    /**
     * Set the data with XML
     */
    void setWithXML(xmlNodePtr rootNode);

  protected:
    
    std::vector<GlPolygon> polygons;
    
    bool filled;
    bool outlined;
    
    Color fillColor;
    Color outlineColor;

  };
  
}

#endif // Tulip_GLMULTIPOLYGON_H
