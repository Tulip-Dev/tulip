//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 29/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLGRAPHINPUTDATA_H
#define Tulip_GLGRAPHINPUTDATA_H

#include "tulip/tulipconf.h"

#include <tulip/MutableContainer.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>

#include "tulip/GlMetaNodeRenderer.h"
#include "tulip/GlGraphRenderingParameters.h"

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

  /**
   * Class use to store inputData of the graph
   */
  class TLP_GL_SCOPE GlGraphInputData {

  public:

    /**
     * Create the inputData with Graph : graph and GlGraphRenderingParameters : parameters
     */
    GlGraphInputData(Graph* graph,GlGraphRenderingParameters* parameters,GlMetaNodeRenderer *renderer=NULL);

    ~GlGraphInputData();

    /**
     * Return the graph of this inputData
     */
    Graph* getGraph() {return graph;}

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
     * Set property name for elementColor
     */
    void setElementColorPropName(const std::string &name){
      elementColorPropName=name;
      elementColor = graph->getProperty<ColorProperty>(elementColorPropName);
    }
    /**
     * Get property name for elementColor
     */
    std::string getElementColorPropName(){
      return elementColorPropName;
    }
    /**
     * Set property name for elementLabelColor
     */
    void setElementLabelColorPropName(const std::string &name){
      elementLabelColorPropName=name;
      elementLabelColor = graph->getProperty<ColorProperty>(elementLabelColorPropName);
    }
    /**
     * Get property name for elementLabelColor
     */
    std::string getElementLabelColorPropName(){
      return elementLabelColorPropName;
    }
    /**
     * Set property name for elementSize
     */
    void setElementSizePropName(const std::string &name){
      elementSizePropName=name;
      elementSize = graph->getProperty<SizeProperty>(elementSizePropName);
    }
    /**
     * Get property name for elementSize
     */
    std::string getElementSizePropName(){
      return elementSizePropName;
    }
    /**
     * Set property name for elementLabelPosition
     */
    void setElementLabelPositionPropName(const std::string &name){
      elementLabelPositionPropName=name;
      elementLabelPosition = graph->getProperty<IntegerProperty>(elementLabelPositionPropName);
    }
    /**
     * Get property name for elementLabelPosition
     */
    std::string getElementLabelPositionPropName(){
      return elementLabelPositionPropName;
    }
    /**
     * Set property name for elementShape
     */
    void setElementShapePropName(const std::string &name){
      elementShapePropName=name;
      elementShape = graph->getProperty<IntegerProperty>(elementShapePropName);
    }
    /**
     * Get property name for elementShape
     */
    std::string getElementShapePropName(){
      return elementShapePropName;
    }
    /**
     * Set property name for elementRotation
     */
    void setElementRotationPropName(const std::string &name){
      elementRotationPropName=name;
      elementLabelColor = graph->getProperty<ColorProperty>(elementLabelColorPropName);
    }
    /**
     * Get property name for elementRotation
     */
    std::string getElementRotationPropName(){
      return elementRotationPropName;
    }
    /**
     * Set property name for elementSelected
     */
    void setElementSelectedPropName(const std::string &name){
      elementSelectedPropName=name;
      elementSelected = graph->getProperty<BooleanProperty>(elementSelectedPropName);
    }
    /**
     * Get property name for elementSelected
     */
    std::string getElementSelectedPropName(){
      return elementSelectedPropName;
    }
    /**
     * Set property name for elementLabel
     */
    void setElementLabelPropName(const std::string &name){
      elementLabelPropName=name;
      elementLabel = graph->getProperty<StringProperty>(elementLabelPropName);
    }
    /**
     * Get property name for elementLabel
     */
    std::string getElementLabelPropName(){
      return elementLabelPropName;
    }
    /**
     * Set property name for elementTexture
     */
    void setElementTexturePropName(const std::string &name){
      elementTexturePropName=name;
      elementTexture = graph->getProperty<StringProperty>(elementTexturePropName);
    }
    /**
     * Get property name for elementTexture
     */
    std::string getElementTexturePropName(){
      return elementTexturePropName;
    }
    /**
     * Set property name for elementBorderColor
     */
    void setElementBorderColorPropName(const std::string &name){
      elementBorderColorPropName=name;
      elementBorderColor = graph->getProperty<ColorProperty>(elementBorderColorPropName);
    }
    /**
     * Get property name for elementBorderColor
     */
    std::string getElementBorderColorPropName(){
      return elementBorderColorPropName;
    }
    /**
     * Set property name for elementBorderWidth
     */
    void setElementBorderWidthPropName(const std::string &name){
      elementBorderWidthPropName=name;
      elementBorderWidth = graph->getProperty<DoubleProperty>(elementBorderWidthPropName);
    }
    /**
     * Get property name for elementBorderWidth
     */
    std::string getElementBorderWidthPropName(){
      return elementBorderWidthPropName;
    }
    /**
     * Set property name for elementLayout
     */
    void setElementLayoutPropName(const std::string &name){
      elementLayoutPropName=name;
      reloadLayoutProperty();
    }
    /**
     * Get property name for elementLayout
     */
    std::string getElementLayoutPropName(){
      if(elementLayoutPropName=="")
        return "viewLayout";
      else
        return elementLayoutPropName;
    }

    /**
     * Set metaNode renderer
     */
    void setMetaNodeRenderer(GlMetaNodeRenderer *renderer){
      metaNodeRenderer=renderer;
    }

    /**
     * Return metaNode renderer
     */
    GlMetaNodeRenderer *getMetaNodeRenderer() {
      return metaNodeRenderer;
    }

  protected :

    std::string elementColorPropName;
    std::string elementLabelColorPropName;
    std::string elementSizePropName;
    std::string elementLabelPositionPropName;
    std::string elementShapePropName;
    std::string elementRotationPropName;
    std::string elementSelectedPropName;
    std::string elementLabelPropName;
    std::string elementTexturePropName;
    std::string elementBorderColorPropName;
    std::string elementBorderWidthPropName;
    std::string elementLayoutPropName;

    GlMetaNodeRenderer *metaNodeRenderer;

  public:

    ColorProperty    *elementColor;
    ColorProperty    *elementLabelColor;
    SizeProperty     *elementSize;
    IntegerProperty  *elementLabelPosition;
    IntegerProperty  *elementShape;
    DoubleProperty   *elementRotation;
    BooleanProperty  *elementSelected;
    StringProperty   *elementLabel;
    LayoutProperty   *elementLayout;
    StringProperty   *elementTexture;
    ColorProperty    *elementBorderColor;
    DoubleProperty   *elementBorderWidth;

    Graph* graph;

    GlGraphRenderingParameters* parameters;

    MutableContainer<Glyph *> glyphs;

  };
}

#endif
