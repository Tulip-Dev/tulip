/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/Camera.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/TulipViewSettings.h>

class FTGLPolygonFont;
class FTOutlineFont;

namespace tlp {

struct OcclusionTest;

#define MIN_LABEL_POSITION LabelPosition::Center
#define MAX_LABEL_POSITION LabelPosition::Right

/**
 * @ingroup OpenGL
 * @brief Create a label into Tulip 3D engine
 *
 * The shortes way to create a label is :
 * @code
 * GlLabel *label=new GlLabel(Coord(0,0,0), Size (1,1,1), Color(1,1,1));
 * label->setText("example");
 * @endcode
 */
class TLP_GL_SCOPE GlLabel : public GlSimpleEntity {
public :

  /**
   * @brief Constructor
   * @warning Don't use this constructor : see other constructor
   */
  GlLabel();

  /**
   * @brief Contructor
   * @param centerPosition position of the label
   * @param size size of the label
   * @param fontColor color of the label
   * @param leftAlign true if you want a left align label
   */
  GlLabel(Coord centerPosition,Size size,Color fontColor,bool leftAlign=false);

  /**
   * @brief Destructor
   */
  ~GlLabel();

  /**
   * @brief Set default parameters of GlLabel
   *
   * This function is call by constructor, so you don't have to call it
   */
  void init();

  /**
   * @brief Set the text of the label
   */
  void setText(const std::string& text);

  /**
   * @brief getText gets the text of this label.
   * @return The text of this label.
   */
  const std::string& getText() const;

  /**
   * @brief Set the position used to render the label
   */
  void setPosition(const Coord &position);

  /**
   * @brief Return the position of the label
   */
  Coord getPosition();

  ///@cond DOXYGEN_HIDDEN

  /**
   * @brief Set the translation used after rotation of the label
   */
  virtual void setTranslationAfterRotation(Coord translation) {
    translationAfterRotation=translation;
  }

  /**
   * @brief return the translation used after rotation of the label
   */
  virtual Coord getTranslationAfterRotation() {
    return translationAfterRotation;
  }

  ///@endcond

  /**
   * @brief Set the alignment of the label : LabelPosition::Center, LabelPosition::Top, LabelPosition::Bottom, LabelPosition::Left, LabelPosition::Right
   * This function is usefull when you have an entity : you spesify the size of the position of this entity and you tell that you want a label outside this entity
   * @see LabelPosition
   * @see setSizeOfOutAlign
   */
  virtual void setAlignment(int alignment) {
    this->alignment=alignment;
  }

  /**
   * @brief Return the bounding box of the label
   */
  virtual BoundingBox getBoundingBox();

  /**
   * @brief Return the bounding box of the text of the label after transformations
   */
  virtual BoundingBox getTextBoundingBox();

  /**
   * @brief Set the size of the label
   */
  virtual void setSize(const Size &size);

  /**
   * @brief return the size of the text
   */
  virtual Size getSize();

  /**
   * @brief Set the size for alignment outside (left/right/top/bottom)
   *
   * You can have a specific size when you want a label outside
   * @see setAlignment
   *
   * @Warning : this size is reinit when you call setSize
   */
  virtual void setSizeForOutAlign(const Size &size);

  /**
   * @brief return the size for alignment outside (left/right/top/bottom)
   * @see setAlignment
   */
  virtual Size getSizeForOutAlign();

  /**
   * @brief Set color of label
   */
  virtual void setColor(const Color &color) {
    this->color=color;
  }

  /**
   * @brief Get color use to render the label
   */
  virtual Color getColor() {
    return color;
  }

  /**
   * @brief Enable/disable the OpenGL depth test for the label (default depth test is enable)
   */
  virtual void enableDepthTest(bool state) {
    depthTestEnabled=state;
  }

  /**
   * @brief Enable/disable if label is scaled to size
   */
  virtual void setScaleToSize(bool state) {
    scaleToSize=state;
  }

  /**
   * @brief Set the stencil and draw the Label, this function is usefull when we directly call draw without tulip engine
   */
  void drawWithStencil(float lod,Camera *camera=NULL);

  /**
   * @brief Return the height of the label after scaling in  size box
   */
  float getHeightAfterScale();

  /**
   * @brief Draw the Label
   */
  virtual void draw(float, Camera *camera=NULL);

  /**
   * @brief Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * @brief Rotate Label
   */
  virtual void rotate(float xRot, float yRot, float zRot);

  /**
   * @brief Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * @brief Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

  /**
   * @brief Switch to bold font
   */
  virtual void setBoldFont();

  /**
   * @brief Switch to plain font
   */
  virtual void setPlainFont();

  /**
   * @brief Change font name
   */
  virtual void setFontName(const std::string &name);

  /**
   * @brief Change font name, size and color of the text
   */
  virtual void setFontNameSizeAndColor(const std::string &name, const int &size, const Color &color);

  ///@cond DOXYGEN_HIDDEN
  /**
   * @brief This parameters is not used
   */
  virtual void setRenderingMode(int mode);
  ///@endcond

  /**
   * @brief Set the occlusion tester
   * If occlusionTester is NULL : deactivate occlusion test
   */
  virtual void setOcclusionTester(OcclusionTest *tester) {
    occlusionTester=tester;
  }

  /**
   * @brief Set if the label is otimized with the lod
   */
  virtual void setUseLODOptimisation(bool state,BoundingBox bb=BoundingBox()) {
    useLOD=state;
    lodBoundingBox=bb;
  }

  /**
   * @brief Return if the label is otimized with the lod
   */
  virtual bool getUseLODOptimisation() {
    return useLOD;
  }

  /**
   * @brief Set labels density of occlusion test
   * This density must be in interval -100 100
   *
   * - If density is equal to -100 : we don't have occlusion test
   * - If density is equal to 0 : GlLabels don't overlap
   * - If density > 0 : GlLabels don't overlap and have space wetween us
   */
  virtual void setLabelsDensity(int density) {
    if(density<-100)
      labelsDensity=-100;
    else if(density>100)
      labelsDensity=100;
    else
      labelsDensity=density;
  }

  /**
   * @brief Return label density of occlusion test
   * This density will be in interval -100 100
   */
  virtual int getLabelDensity() {
    return labelsDensity;
  }

  /**
   * @brief Set min screen size (in pixel) of the label : this size is used in not scaled mode
   * @see setUseMinMaxSize
   */
  void setMinSize(int size) {
    minSize=size;
  }

  /**
   * @brief Get min screen size (in pixel) of the label : this size is used in not scaled mode
   * @see setUseMinMaxSize
   */
  int getMinSize() {
    return minSize;
  }

  /**
   * @brief Set max screen size (in pixel) of the label : this size is used in not scaled mode
   * @see setUseMinMaxSize
   */
  void setMaxSize(int size) {
    maxSize=size;
  }

  /**
   * @brief Get max screen size (in pixel) of the label : this size is used in not scaled mode
   * @see setUseMinMaxSize
   */
  int getMaxSize() {
    return maxSize;
  }

  /**
   * @brief Set if the label use min/max screen size in not scaled mode
   */
  void setUseMinMaxSize(bool state) {
    useMinMaxSize=state;
  }

  /**
   * @brief Return if the label using min/max screen size in not scaled mode
   */
  bool isUsingMinMaxSize() {
    return useMinMaxSize;
  }

  /**
   * @brief Return the font size
   */
  int getFontSize() const {
    return fontSize;
  }

  /**
   * @brief Sets the font size used when rendering the label.
   */
  void setFontSize(int size) {
    fontSize = size;
  }

  /**
   * @brief Return the outline color
   */
  Color getOutlineColor() const {
    return outlineColor;
  }

  /**
   * @brief Sets the outline color used when rendering the label.
   */
  void setOutlineColor(const Color &color) {
    outlineColor = color;
  }

  /**
   * @brief Return the outline size
   */
  float getOutlineSize() const {
    return outlineSize;
  }

  /**
   * @brief Sets the outline size used when rendering the label.
   */
  void setOutlineSize(float size) {
    outlineSize = size;
  }

  /**
   * @brief Return the texture name used to render the label
   */
  std::string getTextureName() const {
    return textureName;
  }

  /**
   * @brief Sets the texture name used when rendering the label.
   */
  void setTextureName(const std::string &name) {
    textureName=name;
  }

  /**
   * @brief Return if the label is billboarded
   */
  bool isBillboarded() {
    return billboarded;
  }

  /**
   * @brief Set if the label is billboarded
   */
  void setBillboarded(bool billboarded) {
    this->billboarded=billboarded;
  }

private :

  std::string text;
  std::string fontName;
  int fontSize;
  int renderingMode;
  FTGLPolygonFont *font;
  FTOutlineFont *borderFont;
  Coord centerPosition;
  Coord translationAfterRotation;
  Size size;
  Size sizeForOutAlign;
  Color color;
  Color outlineColor;
  float outlineSize;
  std::string textureName;
  int alignment;
  bool scaleToSize;
  bool useMinMaxSize;
  int minSize;
  int maxSize;
  bool depthTestEnabled;
  bool leftAlign;
  bool billboarded;
  float xRot;
  float yRot;
  float zRot;
  bool useLOD;
  BoundingBox lodBoundingBox;
  int labelsDensity;
  OcclusionTest *occlusionTester;

  Camera oldCamera;
  float oldLod;
  Vec4i oldViewport;

  std::vector<std::string> textVector;
  std::vector<float> textWidthVector;
  BoundingBox textBoundingBox;
};

}
#endif

