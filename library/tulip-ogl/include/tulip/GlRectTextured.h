//-*-c++-*-
/*
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_GLRECTTEXTURED_H
#define Tulip_GLRECTTEXTURED_H

#include <string>

#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {
  class TLP_GL_SCOPE GlRectTextured : public GlSimpleEntity {

  public:
    GlRectTextured() {}

    GlRectTextured(float top,float bottom,float left, float right,
		   const std::string& textureName,
		   bool inPercent=false);
    
    virtual ~GlRectTextured() {}

    void reloadData();

    virtual BoundingBox getBoundingBox();

    virtual void draw(float lod,Camera *camera);

    /**
     * Function to export data in XML
     */
    virtual void getXML(xmlNodePtr rootNode);

    /**
     * Function to set data with XML
     */
    virtual void setWithXML(xmlNodePtr rootNode);

  protected:
    
    float top;
    float bottom;
    float left;
    float right;
    bool inPercent;
    std::string textureName;

  };
}
#endif
