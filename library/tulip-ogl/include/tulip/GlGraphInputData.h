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
class EdgeExtremityGlyph;

/**
 * Class use to store inputData of the graph
 */
class TLP_GL_SCOPE GlGraphInputData {

public:

	/**
	 * Create the inputData with Graph : graph and GlGraphRenderingParameters : parameters
	 */
	GlGraphInputData(Graph* graph, GlGraphRenderingParameters* parameters,
			GlMetaNodeRenderer *renderer = NULL);

	~GlGraphInputData();

	/**
	 * Return the graph of this inputData
	 */
	Graph* getGraph() {
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
	 * Set property name for elementColor
	 */
	void setElementColorPropName(const std::string &name) {
		elementColorPropName = name;
		elementColor = graph->getProperty<ColorProperty> (elementColorPropName);
	}
	/**
	 * Get property name for elementColor
	 */
	std::string getElementColorPropName() {
		return elementColorPropName;
	}
	/**
	 * Set property name for elementLabelColor
	 */
	void setElementLabelColorPropName(const std::string &name) {
		elementLabelColorPropName = name;
		elementLabelColor = graph->getProperty<ColorProperty> (elementLabelColorPropName);
	}
	/**
	 * Get property name for elementLabelColor
	 */
	std::string getElementLabelColorPropName() {
		return elementLabelColorPropName;
	}
	/**
	 * Set property name for elementSize
	 */
	void setElementSizePropName(const std::string &name) {
		elementSizePropName = name;
		elementSize = graph->getProperty<SizeProperty> (elementSizePropName);
	}
	/**
	 * Get property name for elementSize
	 */
	std::string getElementSizePropName() {
		return elementSizePropName;
	}
	/**
	 * Set property name for elementLabelPosition
	 */
	void setElementLabelPositionPropName(const std::string &name) {
		elementLabelPositionPropName = name;
		elementLabelPosition = graph->getProperty<IntegerProperty> (elementLabelPositionPropName);
	}
	/**
	 * Get property name for elementLabelPosition
	 */
	std::string getElementLabelPositionPropName() {
		return elementLabelPositionPropName;
	}
	/**
	 * Set property name for elementShape
	 */
	void setElementShapePropName(const std::string &name) {
		elementShapePropName = name;
		elementShape = graph->getProperty<IntegerProperty> (elementShapePropName);
	}
	/**
	 * Get property name for elementShape
	 */
	std::string getElementShapePropName() {
		return elementShapePropName;
	}
	/**
	 * Set property name for elementRotation
	 */
	void setElementRotationPropName(const std::string &name) {
		elementRotationPropName = name;
		elementLabelColor = graph->getProperty<ColorProperty> (elementLabelColorPropName);
	}
	/**
	 * Get property name for elementRotation
	 */
	std::string getElementRotationPropName() {
		return elementRotationPropName;
	}
	/**
	 * Set property name for elementSelected
	 */
	void setElementSelectedPropName(const std::string &name) {
		elementSelectedPropName = name;
		elementSelected = graph->getProperty<BooleanProperty> (elementSelectedPropName);
	}
	/**
	 * Get property name for elementSelected
	 */
	std::string getElementSelectedPropName() {
		return elementSelectedPropName;
	}
	/**
     * Set property name for elementFont
     */
    void setElementFontPropName(const std::string &name){
      elementFontPropName=name;
      elementFont = graph->getProperty<StringProperty>(elementFontPropName);
    }
    /**
     * Get property name for elementFont
     */
    std::string getElementFontPropName(){
      return elementFontPropName;
    }
    /**
     * Set property name for elementFontSize
     */
    void setElementFontSizePropName(const std::string &name){
      elementFontSizePropName=name;
      elementFontSize = graph->getProperty<IntegerProperty>(elementFontSizePropName);
    }
    /**
     * Get property name for elementFontSize
     */
    std::string getElementFontSizePropName(){
      return elementFontSizePropName;
    }
    /**
	 * Set property name for elementLabel
	 */
	void setElementLabelPropName(const std::string &name) {
		elementLabelPropName = name;
		elementLabel = graph->getProperty<StringProperty> (elementLabelPropName);
	}
	/**
	 * Get property name for elementLabel
	 */
	std::string getElementLabelPropName() {
		return elementLabelPropName;
	}
	/**
	 * Set property name for elementTexture
	 */
	void setElementTexturePropName(const std::string &name) {
		elementTexturePropName = name;
		elementTexture = graph->getProperty<StringProperty> (elementTexturePropName);
	}
	/**
	 * Get property name for elementTexture
	 */
	std::string getElementTexturePropName() {
		return elementTexturePropName;
	}
	/**
	 * Set property name for elementBorderColor
	 */
	void setElementBorderColorPropName(const std::string &name) {
		elementBorderColorPropName = name;
		elementBorderColor = graph->getProperty<ColorProperty> (elementBorderColorPropName);
	}
	/**
	 * Get property name for elementBorderColor
	 */
	std::string getElementBorderColorPropName() {
		return elementBorderColorPropName;
	}
	/**
	 * Set property name for elementBorderWidth
	 */
	void setElementBorderWidthPropName(const std::string &name) {
		elementBorderWidthPropName = name;
		elementBorderWidth = graph->getProperty<DoubleProperty> (elementBorderWidthPropName);
	}
	/**
	 * Get property name for elementBorderWidth
	 */
	std::string getElementBorderWidthPropName() {
		return elementBorderWidthPropName;
	}
	/**
	 * Set property name for elementLayout
	 */
	void setElementLayoutPropName(const std::string &name) {
		elementLayoutPropName = name;
		reloadLayoutProperty();
	}
	/**
	 * Get property name for elementLayout
	 */
	std::string getElementLayoutPropName() {
		if (elementLayoutPropName == "")
			return "viewLayout";
		else
			return elementLayoutPropName;
	}

	std::string getElementSourceAnchorShapePropName() {
		return elementSrcAnchorShapePropName;
	}

	void setElementSourceAnchorShapePropName(const std::string &name) {
		elementSrcAnchorShapePropName = name;
	}

	std::string getElementSourceAnchorSizePropName() {
		return elementSrcAnchorSizePropName;
	}

	void setElementSourceAnchorSizePropName(const std::string& name) {
		elementSrcAnchorSizePropName = name;
	}

	std::string getElementTargetAnchorShapePropName() {
		return elementTgtAnchorShapePropName;
	}

	void setElementTargetAnchorShapePropName(const std::string &name) {
		elementTgtAnchorShapePropName = name;
	}

	std::string getElementTargetAnchorSizePropName() {
		return elementTgtAnchorSizePropName;
	}

	void setElementTargetAnchorSizePropName(const std::string& name) {
		elementTgtAnchorSizePropName = name;
	}

	/**
	 * Set metaNode renderer
	 */
	void setMetaNodeRenderer(GlMetaNodeRenderer *renderer) {
		metaNodeRenderer = renderer;
	}

	/**
	 * Return metaNode renderer
	 */
	GlMetaNodeRenderer *getMetaNodeRenderer() {
		return metaNodeRenderer;
	}

protected:

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
	GlMetaNodeRenderer *metaNodeRenderer;

public:

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

	Graph* graph;

	GlGraphRenderingParameters* parameters;

	MutableContainer<Glyph *> glyphs;
	MutableContainer<EdgeExtremityGlyph *> extremityGlyphs;
};
}

#endif
