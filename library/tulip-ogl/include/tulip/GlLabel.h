//-*-c++-*-
/*
 Author: Mathiaut Morgan
 Email : mathiaut@labri.fr

 Last modification : 22/11/07

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLLABEL_H
#define Tulip_GLLABEL_H

#include <string>

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {

  class Camera;
  class TextRenderer;

  /**
   * Create a entity Label
   */
  class TLP_GL_SCOPE GlLabel : public GlSimpleEntity
  {
  public :

    static const int TEXTURE_MODE = 0;
    static const int POLYGON_MODE = 1;

    GlLabel();

    /**
     * Contructor with fontPath, centerPosition, size, fontColor and alignment
     */
    GlLabel(Coord centerPosition,Coord size,Color fontColor,bool leftAlign=false);

    /**
     * Contructor with fontPath, centerPosition, size, fontColor and alignment
     * \deprecate this constructor will be remove on Tulip 4 version
     */
    GlLabel(const std::string &fontPath,Coord centerPosition,Coord size,Color fontColor,bool leftAlign=false);

    ~GlLabel();

    /**
     * Set the text of the label
     */
    void setText(const std::string& text);

    /**
     * Return the bounding box of the label
     */
    virtual BoundingBox getBoundingBox();

    /**
     * return the size of the text
     */
    virtual Coord getSize();

    /**
     * Set color of label
     */
    virtual void setColor(const Color &color) {
      this->color=color;
    }

    /**
     * Get color use to render the label
     */
    virtual Color getColor() {
      return color;
    }

    /**
     * Draw the Label
     */
    virtual void draw(float lod, Camera *camera);

    /**
     * Translate entity
     */
    virtual void translate(const Coord& mouvement);

    /**
     * Rotate Label
     */
    virtual void rotate(float xRot, float yRot, float zRot);

    /**
     * Function to export data in XML
     */
    virtual void getXML(xmlNodePtr rootNode);

    /**
     * Function to set data with XML
     */
    virtual void setWithXML(xmlNodePtr rootNode);

    /**
     * Switch to bold font
     */
    virtual void setBoldFont();

    /**
     * Switch to plain font
     */
    virtual void setPlainFont();

    /**
     * Switch rendering mode to polygon- or texture-based rendering
     */
    virtual void setRenderingMode(int mode);


  private :

    std::string text;
    static TextRenderer *renderer;
    Coord centerPosition;
    Coord size;
    Color color;
    bool leftAlign;
    float xRot;
    float yRot;
    float zRot;
  };
}
#endif
