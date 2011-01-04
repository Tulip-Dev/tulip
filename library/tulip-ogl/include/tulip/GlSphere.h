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

  /** \addtogroup GlEntities
   * Class to create a sphere with GlEntity system
   */
  class TLP_GL_SCOPE GlSphere : public GlSimpleEntity {

  public:

    GlSphere() {}
    /**
     * Create a sphere with a position, a radius a fill color and multiple rotation (if you want)
     */
    GlSphere(const Coord &position,float radius,const Color &color=Color(0,0,0,255),float rotX=0,float rotY=0,float rotZ=0);
    /**
        * Create a sphere with a position, a radius, a texture, an alphe and multiple rotation (if you want)
        */
    GlSphere(const Coord &position,float radius,const std::string& textureFile,int alpha=255,float rotX=0,float rotY=0,float rotZ=0);

    /**
     * Draw the sphere
     */
    virtual void draw(float lod,Camera *camera);

    /**
     * Translate entity
     */
    virtual void translate(const Coord& mouvement);

    /**
     * Set the texture name
     */
    virtual void setTexture(const std::string &texture) {
      textureFile=texture;
    }

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
