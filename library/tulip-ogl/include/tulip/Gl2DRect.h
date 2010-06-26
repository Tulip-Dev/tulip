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
#ifndef Tulip_GL2DRECT_H
#define Tulip_GL2DRECT_H

#include <string>

#include "tulip/GlRect.h"

namespace tlp {
  /**
   * Create a 2D rectangle
   * This rect add screen percentage system (for example you can create a rectangle that start on (10%,10%) and finish on (90%90%)
   * If you whan a simple 2D rectangle with normal coordinates use GlRect class
   */
  class TLP_GL_SCOPE Gl2DRect : public GlRect {

  public:
    Gl2DRect();

    /**
     * Constructor : with top/bottom/left/right coords, textureName and if inPercent is true coords are in percent
     */
    Gl2DRect(float top,float bottom,float left, float right,
             const std::string& textureName,
             bool inPercent=false);

    /**
     * Constructor : with bottom/left height,width coords, textureName and if xInv/yInv is true coords viewport - coord
     */
    Gl2DRect(float bottom,float left,float height, float width,
           const std::string& textureName,
           bool xInv, bool yInv);

    virtual ~Gl2DRect() {}

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
    bool inPercent;
    bool xInv;
    bool yInv;

  };
}
#endif
