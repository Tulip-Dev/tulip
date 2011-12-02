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
#ifndef Tulip_GLGRAPHINPUTDATA_H
#define Tulip_GLGRAPHINPUTDATA_H

#include <tulip/tulipconf.h>

#include <tulip/MutableContainer.h>

#include <tulip/GlMetaNodeRenderer.h>
#include <tulip/GlGraphRenderingParameters.h>

namespace tlp {

class LayoutProperty;
class DoubleProperty;
class StringProperty;
class IntegerProperty;
class BooleanProperty;
class SizeProperty;
class ColorProperty;
class PropertyManager;
class Graph;
class Glyph;
class EdgeExtremityGlyph;
class GlVertexArrayManager;

/**
 * Class use to store inputData of the graph
 */
class TLP_GL_SCOPE GlGraphInputData {

public:

  /**
   * Create the inputData with Graph : graph and GlGraphRenderingParameters : parameters
   */
  GlGraphInputData(Graph* graph, GlGraphRenderingParameters* parameters,GlMetaNodeRenderer *renderer = NULL);

  ~GlGraphInputData();

  /**
   * Return the graph of this inputData
   */
  Graph* getGraph() const {
    return graph;
  }

  /**
   * Reload layoutProperty of the graph
   */
  void reloadLayoutProperty();

  /**
   * Return the layout of this inputData
   */
  LayoutProperty* getLayoutProperty() {
    reloadLayoutProperty();
    return elementLayout;
  }

  /**
   * Reload viewLabel Property of the graph
   */
  void reloadAllProperties();

  /**
   * Set metaNode renderer
   * If deleteOldMetaNodeRenderer==true : this function delete old meta node renderer
   */
  void setMetaNodeRenderer(GlMetaNodeRenderer *renderer,bool deleteOldMetaNodeRenderer=true) {
    if(deleteOldMetaNodeRenderer)
      delete metaNodeRenderer;

    metaNodeRenderer = renderer;
  }

  /**
   * Set if the meta node renderer must be deleted at destructor
   */
  void setDeleteMetaNodeRendererAtDestructor(bool deleteAtDestructor) {
    deleteMetaNodeRendererAtDestructor=deleteAtDestructor;
  }

  /**
   * Return metaNode renderer
   */
  GlMetaNodeRenderer *getMetaNodeRenderer() const {
    return metaNodeRenderer;
  }

  /**
   * Return glEdgeDisplayManager
   */
  GlVertexArrayManager *getGlVertexArrayManager() const {
    return glVertexArrayManager;
  }

  /**
   * Set glEdgeDisplayManager
   */
  void setGlVertexArrayManager(GlVertexArrayManager * manager) {
    glVertexArrayManager=manager;
  }

  /**
   * Set if GlVertexArrayManager must be deleted in destructor
   */
  void deleteGlVertexArrayManagerInDestructor(bool del) {
    deleteGlVertexArrayManager=del;
  }

  /**
   * Return a pointer on the property used to elementColor
   */
  ColorProperty * getElementColor() const {
    return elementColor;
  }
  /**
   * Set the pointer on the property used to elementColor
   */
  void setElementColor(ColorProperty *property) {
    _properties.erase(elementColor);
    _properties.insert(property);
    elementColor=property;
  }
  /**
   * Return a pointer on the property used to elementLabelColor
   */
  ColorProperty *getElementLabelColor() const {
    return elementLabelColor;
  }
  /**
   * Set the pointer on the property used to elementLabelColor
   */
  void setElementLabelColor(ColorProperty *property) {
    _properties.erase(elementLabelColor);
    _properties.insert(property);
    elementLabelColor=property;
  }
  /**
   * Return a pointer on the property used to elementSize
   */
  SizeProperty *getElementSize() const {
    return elementSize;
  }
  /**
   * Set the pointer on the property used to elementSize
   */
  void setElementSize(SizeProperty *property) {
    _properties.erase(elementSize);
    _properties.insert(property);
    elementSize=property;
  }
  /**
   * Return a pointer on the property used to elementLabelPosition
   */
  IntegerProperty *getElementLabelPosition() const {
    return elementLabelPosition;
  }
  /**
   * Set the pointer on the property used to elementLabelPosition
   */
  void setElementLabelPosition(IntegerProperty *property) {
    _properties.erase(elementLabelPosition);
    _properties.insert(property);
    elementLabelPosition=property;
  }
  /**
   * Return a pointer on the property used to elementShape
   */
  IntegerProperty *getElementShape() const {
    return elementShape;
  }
  /**
   * Set the pointer on the property used to elementShape
   */
  void setElementShape(IntegerProperty *property) {
    _properties.erase(elementShape);
    _properties.insert(property);
    elementShape=property;
  }
  /**
   * Return a pointer on the property used to elementRotation
   */
  DoubleProperty *getElementRotation() const {
    return elementRotation;
  }
  /**
   * Set the pointer on the property used to elementRotation
   */
  void setElementRotation(DoubleProperty *property) {
    _properties.erase(elementRotation);
    _properties.insert(property);
    elementRotation=property;
  }
  /**
   * Return a pointer on the property used to elementSelected
   */
  BooleanProperty *getElementSelected() const {
    return elementSelected;
  }
  /**
   * Set the pointer on the property used to elementSelected
   */
  void setElementSelected(BooleanProperty *property) {
    _properties.erase(elementSelected);
    _properties.insert(property);
    elementSelected=property;
  }
  /**
   * Return a pointer on the property used to elementFont
   */
  StringProperty   *getElementFont() const {
    return elementFont;
  }
  /**
   * Set the pointer on the property used to elementFont
   */
  void setElementFont(StringProperty *property) {
    _properties.erase(elementFont);
    _properties.insert(property);
    elementFont=property;
  }
  /**
   * Return a pointer on the property used to elementFontSize
   */
  IntegerProperty  *getElementFontSize() const {
    return elementFontSize;
  }
  /**
   * Set the pointer on the property used to elementFontSize
   */
  void setElementFontSize(IntegerProperty *property) {
    _properties.erase(elementFontSize);
    _properties.insert(property);
    elementFontSize=property;
  }
  /**
   * Return a pointer on the property used to elementLabel
   */
  StringProperty *getElementLabel() const {
    return elementLabel;
  }
  /**
   * Set the pointer on the property used to elementLabel
   */
  void setElementLabel(StringProperty *property) {
    _properties.erase(elementLabel);
    _properties.insert(property);
    elementLabel=property;
  }
  /**
   * Return a pointer on the property used to elementLayout
   */
  LayoutProperty *getElementLayout() const {
    return elementLayout;
  }
  /**
   * Set the pointer on the property used to elementLayout
   */
  void setElementLayout(LayoutProperty *property) {
    _properties.erase(elementLayout);
    _properties.insert(property);
    elementLayout=property;
  }
  /**
   * Return a pointer on the property used to elementTexture
   */
  StringProperty *getElementTexture() const {
    return elementTexture;
  }
  /**
   * Set the pointer on the property used to elementTexture
   */
  void setElementTexture(StringProperty *property) {
    _properties.erase(elementTexture);
    _properties.insert(property);
    elementTexture=property;
  }
  /**
   * Return a pointer on the property used to elementBorderColor
   */
  ColorProperty *getElementBorderColor() const {
    return elementBorderColor;
  }
  /**
   * Set the pointer on the property used to elementBorderColor
   */
  void setElementBorderColor(ColorProperty *property) {
    _properties.erase(elementBorderColor);
    _properties.insert(property);
    elementBorderColor=property;
  }
  /**
   * Return a pointer on the property used to elementBorderWidth
   */
  DoubleProperty *getElementBorderWidth() const {
    return elementBorderWidth;
  }
  /**
   * Set the pointer on the property used to elementBorderWidth
   */
  void setElementBorderWidth(DoubleProperty *property) {
    _properties.erase(elementBorderWidth);
    _properties.insert(property);
    elementBorderWidth=property;
  }
  /**
   * Return a pointer on the property used to elementSrcAnchorShape
   */
  IntegerProperty *getElementSrcAnchorShape() const {
    return elementSrcAnchorShape;
  }
  /**
   * Set the pointer on the property used to elementSrcAnchorShape
   */
  void setElementSrcAnchorShape(IntegerProperty *property) {
    _properties.erase(elementSrcAnchorShape);
    _properties.insert(property);
    elementSrcAnchorShape=property;
  }
  /**
   * Return a pointer on the property used to elementSrcAnchorSize
   */
  SizeProperty *getElementSrcAnchorSize() const {
    return elementSrcAnchorSize;
  }
  /**
   * Set the pointer on the property used to elementSrcAnchorSize
   */
  void setElementSrcAnchorSize(SizeProperty *property) {
    _properties.erase(elementSrcAnchorSize);
    _properties.insert(property);
    elementSrcAnchorSize=property;
  }
  /**
   * Return a pointer on the property used to elementTgtAnchorShape
   */
  IntegerProperty *getElementTgtAnchorShape() const {
    return elementTgtAnchorShape;
  }
  /**
   * Set the pointer on the property used to elementTgtAnchorShape
   */
  void setElementTgtAnchorShape(IntegerProperty *property) {
    _properties.erase(elementTgtAnchorShape);
    _properties.insert(property);
    elementTgtAnchorShape=property;
  }
  /**
   * Return a pointer on the property used to elementTgtAnchorSize
   */
  SizeProperty *getElementTgtAnchorSize() const {
    return elementTgtAnchorSize;
  }
  /**
   * Set the property name for elementSourceAnchorSize
   */
  void setElementTgtAnchorSize(SizeProperty *property) {
    _properties.erase(elementTgtAnchorSize);
    _properties.insert(property);
    elementTgtAnchorSize=property;
  }
  /**
   * Return a pointer on the property used to elementAnimationFrame
   */
  IntegerProperty *getElementAnimationFrame() const {
    return elementAnimationFrame;
  }
  /**
   * Set the pointer on the property used to elementAnimationFrame
   */
  void setElementAnimationFrame(IntegerProperty *property) {
    _properties.erase(elementAnimationFrame);
    _properties.insert(property);
    elementAnimationFrame=property;
  }

  std::set<tlp::PropertyInterface*> properties() const { return _properties; }

public :

  Graph* graph;

  GlGraphRenderingParameters* parameters;

  MutableContainer<Glyph *> glyphs;
  MutableContainer<EdgeExtremityGlyph *> extremityGlyphs;

protected:

  std::set<tlp::PropertyInterface*> _properties;

  bool deleteGlVertexArrayManager;

  ColorProperty *elementColor;
  ColorProperty *elementLabelColor;
  SizeProperty *elementSize;
  IntegerProperty *elementLabelPosition;
  IntegerProperty *elementShape;
  DoubleProperty *elementRotation;
  BooleanProperty *elementSelected;
  StringProperty   *elementFont;
  IntegerProperty  *elementFontSize;
  StringProperty *elementLabel;
  LayoutProperty *elementLayout;
  StringProperty *elementTexture;
  ColorProperty *elementBorderColor;
  DoubleProperty *elementBorderWidth;
  IntegerProperty *elementSrcAnchorShape;
  SizeProperty *elementSrcAnchorSize;
  IntegerProperty *elementTgtAnchorShape;
  SizeProperty *elementTgtAnchorSize;
  IntegerProperty *elementAnimationFrame;

  std::string elementColorPropName;
  std::string elementLabelColorPropName;
  std::string elementSizePropName;
  std::string elementLabelPositionPropName;
  std::string elementShapePropName;
  std::string elementRotationPropName;
  std::string elementSelectedPropName;
  std::string elementFontPropName;
  std::string elementFontSizePropName;
  std::string elementLabelPropName;
  std::string elementTexturePropName;
  std::string elementBorderColorPropName;
  std::string elementBorderWidthPropName;
  std::string elementLayoutPropName;
  std::string elementSrcAnchorShapePropName;
  std::string elementSrcAnchorSizePropName;
  std::string elementTgtAnchorShapePropName;
  std::string elementTgtAnchorSizePropName;

  bool deleteMetaNodeRendererAtDestructor;
  GlMetaNodeRenderer *metaNodeRenderer;
  GlVertexArrayManager *glVertexArrayManager;


};
}

#endif
