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
  /**
   * Create a rectangle with a texture
   */
  class TLP_GL_SCOPE GlRectTextured : public GlSimpleEntity {

  public:
    GlRectTextured() {}

    /**
     * Constructor : with top/bottom/left/right coords, textureName and if inPercent is true coords are in percent
     */
    GlRectTextured(float top,float bottom,float left, float right,
        const std::string& textureName,
        bool inPercent=false);

    /**
     * Constructor : with bottom/left height,width coords, textureName and if xInv/yInv is true coords viewport - coord
     */
    GlRectTextured(float bottom,float left,float height, float width,
           const std::string& textureName,
           bool xInv, bool yInv);

    virtual ~GlRectTextured() {}

    /**
     * Reload data of the rectangle
     */
    void reloadData();

    /**
     * Return the bounding box
     */
    virtual BoundingBox getBoundingBox();

    /**
     * Draw the rectangle
     */
    virtual void draw(float lod,Camera *camera);

    /**
     * Translate entity
     */
    virtual void translate(const Coord& mouvement);

    /**
     * Set color of the rectangle
     */
    virtual void setColor(const Color &color){
      this->color=color;
    }

    /**
     * Set texture of the GlRectTextured
     */
    virtual void setTexture(const std::string &name);

    /**
     * Get texture of the GlRectTextured
     */
    virtual std::string getTexture();

    /**
     * Set coordinates of rectangle
     */
    virtual void setCoordinates(float bottom, float left, float width, float height);

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
    Color color;
    bool inPercent;
    std::string textureName;
    bool xInv;
    bool yInv;

  };
}
#endif
