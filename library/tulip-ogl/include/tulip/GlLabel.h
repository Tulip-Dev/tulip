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
#ifndef Tulip_GLLABEL_H
#define Tulip_GLLABEL_H

#include <string>

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {

  class Camera;
  class TextRenderer;
  struct OcclusionTest;

  /**
   * \addtogroup GlEntities
   */
  /*@{*/
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
     * Set default parameters of GlLabel
     */
    void init();

    /**
     * Set the text of the label
     */
    void setText(const std::string& text);

    /**
     * Set the position used to render the label
     */
    void setPosition(const Coord &position);

    /**
     * Return the position of the label
     */
    Coord getPosition();

    /**
     * Set the translation used after rotation of the label
     */
    virtual void setTranslationAfterRotation(Coord translation){
      translationAfterRotation=translation;
    }

    /**
     * return the translation used after rotation of the label
     */
    virtual Coord getTranslationAfterRotation(){
      return translationAfterRotation;
    }

    /**
     * Set the alignment of the label : ON_CENTER, ON_TOP, ON_BOTTOM, ON_LEFT, ON_RIGHT
     */
    virtual void setAlignment(int alignment){
      this->alignment=alignment;
    }

    /**
     * Return the bounding box of the label
     */
    virtual BoundingBox getBoundingBox();

    /**
     * Set the size of the label
     */
    virtual void setSize(const Coord &size);

    /**
     * return the size of the text
     */
    virtual Coord getSize();

    /**
     * Set the size for alignment outside (left/right/top/bottom)
     *  Warning : this size is reinit when you call setSize
     */
    virtual void setSizeForOutAlign(const Coord &size);

    /**
     * return the size for alignment outside (left/right/top/bottom)
     */
    virtual Coord getSizeForOutAlign();

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
     * Enable/disable the depth test for the label (default depth test is enable)
     */
    virtual void enableDepthTest(bool state){
      depthTestEnabled=state;
    }

    /**
     * Enable/disable if label is scaled to size
     */
    virtual void setScaleToSize(bool state){
      scaleToSize=state;
    }

    /**
     * Set the stencil and draw the Label, this function is usefull when we directly call draw without tulip engine
     */
    void drawWithStencil(float lod, Camera *camera=NULL);

    /**
     * Draw the Label
     */
    virtual void draw(float lod, Camera *camera=NULL);

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
     * Change font name
     */
    virtual void setFontName(const std::string &name);

    /**
     * Change font name, size and color of the text
     */
    virtual void setFontNameSizeAndColor(const std::string &name, const int &size, const Color &color);

    /**
     * Switch rendering mode to polygon- or texture-based rendering
     */
    virtual void setRenderingMode(int mode);

    /**
     * Set the occlusion tester
     *  If occlusionTester is NULL : deactivate occlusion test
     */
    virtual void setOcclusionTester(OcclusionTest *tester){
      occlusionTester=tester;
    }

    /**
     * Set if the label is otimized with the lod
     */
    virtual void setUseLODOptimisation(bool state){
      useLOD=state;
    }

    /**
     * Return label border for occlusion test
     */
    virtual bool getUseLODOptimisation(){
      return useLOD;
    }

    /**
     * Set label border for occlusion test
     */
    virtual void setLabelOcclusionBorder(unsigned int size){
      occlusionBorderSize=size;
    }

    /**
     * Return label border for occlusion test
     */
    virtual unsigned int getLabelOcclusionBorder(){
      return occlusionBorderSize;
    }


  private :

    std::string text;
    std::string fontName;
    int renderingMode;
    static TextRenderer *renderer;
    Coord centerPosition;
    Coord translationAfterRotation;
    Coord size;
    Coord sizeForOutAlign;
    Color color;
    int alignment;
    bool scaleToSize;
    bool depthTestEnabled;
    bool leftAlign;
    float xRot;
    float yRot;
    float zRot;
    bool useLOD;
    unsigned int occlusionBorderSize;
    OcclusionTest *occlusionTester;
  };
  /*@}*/
}
#endif
