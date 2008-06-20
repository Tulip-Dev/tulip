//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef GL_CYLINDER_H
#define GL_CYLINDER_H

#include <tulip/tulipconf.h>

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include <tulip/GlSimpleEntity.h>

namespace tlp {

  class TLP_GL_SCOPE GlCylinder : public GlSimpleEntity {
    
  public :

    GlCylinder() {}
    GlCylinder(const Coord &start, const Coord &end, float startRadius, float endRadius, const Color &color=Color(0,0,0,255), const bool withCaps=true);
    GlCylinder(const Coord &start, const Coord &end, float startRadius, float endRadius, const std::string& textureFile, const Color &color=Color(255,255,255, 255), const bool withCaps=true);

    void draw(float lod,Camera *camera);

    /**
     * Function to export data in XML
     */
    void getXML(xmlNodePtr rootNode) ;
    
    /**
     * Function to set data with XML
     */
    void setWithXML(xmlNodePtr rootNode) ;
    
  private:
    
    void computeVectors();
    
    Coord startPoint;
    Coord endPoint;
    
    float startRadius;
    float endRadius;
    
    float cylinderLength;

    // orientation vector
    float vx;
    float vy;
    float vz;

    // rotation vector
    float rx;
    float ry;
    float rz;

    bool displayCaps;

    Color color;
    std::string textureFile;
    

  };
  
}

#endif // GL_CYLINDER_H
