//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLSPHERE_H
#define Tulip_GLSPHERE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {

  class TLP_GL_SCOPE GlSphere : public GlSimpleEntity {
  
  public:
     
    GlSphere() {}
    GlSphere(const Coord &position,float radius,const Color &color=Color(0,0,0,255),float rotX=0,float rotY=0,float rotZ=0);
    GlSphere(const Coord &position,float radius,const std::string& textureFile,int alpha=255,float rotX=0,float rotY=0,float rotZ=0);

    virtual void draw(float lod,Camera *camera);

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
    
  private:
    
    Coord position;
    float radius;
    Color color;
    std::string textureFile;
    Coord rot;

  };
  
}

#endif // Tulip_GLSCENE_H
