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

#include "tulip/tulipconf.h"

#include <tulip/MutableContainer.h>

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
	/**
	 * Get property name for elementSourceAnchorShape
	 */
	std::string getElementSourceAnchorShapePropName() {
		return elementSrcAnchorShapePropName;
	}
	/**
	 * Set property name for elementSourceAnchorShape
	 */
	void setElementSourceAnchorShapePropName(const std::string &name) {
		elementSrcAnchorShapePropName = name;
	}
	/**
	 * Get property name for elementSourceAnchorSize
	 */
	std::string getElementSourceAnchorSizePropName() {
		return elementSrcAnchorSizePropName;
	}
	/**
	 * Set property name for elementSourceAnchorSize
	 */
	void setElementSourceAnchorSizePropName(const std::string& name) {
		elementSrcAnchorSizePropName = name;
	}
	/**
	 * Get property name for elementTargetAnchorShape
	 */
	std::string getElementTargetAnchorShapePropName() {
		return elementTgtAnchorShapePropName;
	}
	/**
	 * Set property name for elementTargetAnchorShape
	 */
	void setElementTargetAnchorShapePropName(const std::string &name) {
		elementTgtAnchorShapePropName = name;
	}
	/**
	 * Get property name for elementTargetAnchorSize
	 */
	std::string getElementTargetAnchorSizePropName() {
		return elementTgtAnchorSizePropName;
	}
	/**
	 * Set property name for elementTargetAnchorSize
	 */
	void setElementTargetAnchorSizePropName(const std::string& name) {
		elementTgtAnchorSizePropName = name;
	}

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
	void setDeleteMetaNodeRendererAtDestructor(bool deleteAtDestructor){
		deleteMetaNodeRendererAtDestructor=deleteAtDestructor;
	}

	/**
	 * Return metaNode renderer
	 */
  GlMetaNodeRenderer *getMetaNodeRenderer() {
		return metaNodeRenderer;
	}

  /**
   * Return glEdgeDisplayManager
   */
  GlVertexArrayManager *getGlVertexArrayManager(){
    return glVertexArrayManager;
  }

  /**
   * Set glEdgeDisplayManager
   */
  void setGlVertexArrayManager(GlVertexArrayManager * manager){
    glVertexArrayManager=manager;
  }

  /**
   * Set if GlVertexArrayManager must be deleted in destructor
   */
  void deleteGlVertexArrayManagerInDestructor(bool del){
    deleteGlVertexArrayManager=del;
  }

  /**
   * Return a pointer on the property used to elementColor
   */
  ColorProperty * getElementColor() {
    return elementColor;
  }
  /**
   * Set the pointer on the property used to elementColor
   */
  void setElementColor(ColorProperty *property) {
     elementColor=property;
  }
  /**
   * Return a pointer on the property used to elementLabelColor
   */
  ColorProperty *getElementLabelColor() {
    return elementLabelColor;
  }
  /**
   * Set the pointer on the property used to elementLabelColor
   */
  void setElementLabelColor(ColorProperty *property) {
    elementLabelColor=property;
  }
  /**
   * Return a pointer on the property used to elementSize
   */
  SizeProperty *getElementSize() {
    return elementSize;
  }
  /**
   * Set the pointer on the property used to elementSize
   */
  void setElementSize(SizeProperty *property) {
    elementSize=property;
  }
  /**
   * Return a pointer on the property used to elementLabelPosition
   */
  IntegerProperty *getElementLabelPosition() {
    return elementLabelPosition;
  }
  /**
   * Set the pointer on the property used to elementLabelPosition
   */
  void setElementLabelPosition(IntegerProperty *property) {
    elementLabelPosition=property;
  }
  /**
   * Return a pointer on the property used to elementShape
   */
  IntegerProperty *getElementShape() {
    return elementShape;
  }
  /**
   * Set the pointer on the property used to elementShape
   */
  void setElementShape(IntegerProperty *property) {
    elementShape=property;
  }
  /**
   * Return a pointer on the property used to elementRotation
   */
  DoubleProperty *getElementRotation() {
    return elementRotation;
  }
  /**
   * Set the pointer on the property used to elementRotation
   */
  void setElementRotation(DoubleProperty *property) {
    elementRotation=property;
  }
  /**
   * Return a pointer on the property used to elementSelected
   */
  BooleanProperty *getElementSelected() {
    return elementSelected;
  }
  /**
   * Set the pointer on the property used to elementSelected
   */
  void setElementSelected(BooleanProperty *property) {
    elementSelected=property;
  }
  /**
   * Return a pointer on the property used to elementFont
   */
  StringProperty   *getElementFont() {
    return elementFont;
  }
  /**
   * Set the pointer on the property used to elementFont
   */
  void setElementFont(StringProperty *property) {
    elementFont=property;
  }
  /**
   * Return a pointer on the property used to elementFontSize
   */
  IntegerProperty  *getElementFontSize() {
    return elementFontSize;
  }
  /**
   * Set the pointer on the property used to elementFontSize
   */
  void setElementFontSize(IntegerProperty *property) {
    elementFontSize=property;
  }
  /**
   * Return a pointer on the property used to elementLabel
   */
  StringProperty *getElementLabel() {
    return elementLabel;
  }
  /**
   * Set the pointer on the property used to elementLabel
   */
  void setElementLabel(StringProperty *property) {
    elementLabel=property;
  }
  /**
   * Return a pointer on the property used to elementLayout
   */
  LayoutProperty *getElementLayout() {
    return elementLayout;
  }
  /**
   * Set the pointer on the property used to elementLayout
   */
  void setElementLayout(LayoutProperty *property) {
    elementLayout=property;
  }
  /**
   * Return a pointer on the property used to elementTexture
   */
  StringProperty *getElementTexture() {
    return elementTexture;
  }
  /**
   * Set the pointer on the property used to elementTexture
   */
  void setElementTexture(StringProperty *property) {
    elementTexture=property;
  }
  /**
   * Return a pointer on the property used to elementBorderColor
   */
  ColorProperty *getElementBorderColor() {
    return elementBorderColor;
  }
  /**
   * Set the pointer on the property used to elementBorderColor
   */
  void setElementBorderColor(ColorProperty *property) {
    elementBorderColor=property;
  }
  /**
   * Return a pointer on the property used to elementBorderWidth
   */
  DoubleProperty *getElementBorderWidth() {
    return elementBorderWidth;
  }
  /**
   * Set the pointer on the property used to elementBorderWidth
   */
  void setElementBorderWidth(DoubleProperty *property) {
    elementBorderWidth=property;
  }
  /**
   * Return a pointer on the property used to elementSrcAnchorShape
   */
  IntegerProperty *getElementSrcAnchorShape() {
    return elementSrcAnchorShape;
  }
  /**
   * Set the pointer on the property used to elementSrcAnchorShape
   */
  void setElementSrcAnchorShape(IntegerProperty *property) {
    elementSrcAnchorShape=property;
  }
  /**
   * Return a pointer on the property used to elementSrcAnchorSize
   */
  SizeProperty *getElementSrcAnchorSize() {
    return elementSrcAnchorSize;
  }
  /**
   * Set the pointer on the property used to elementSrcAnchorSize
   */
  void setElementSrcAnchorSize(SizeProperty *property) {
    elementSrcAnchorSize=property;
  }
  /**
   * Return a pointer on the property used to elementTgtAnchorShape
   */
  IntegerProperty *getElementTgtAnchorShape() {
    return elementTgtAnchorShape;
  }
  /**
   * Set the pointer on the property used to elementTgtAnchorShape
   */
  void setElementTgtAnchorShape(IntegerProperty *property) {
    elementTgtAnchorShape=property;
  }
  /**
   * Return a pointer on the property used to elementTgtAnchorSize
   */
  SizeProperty *getElementTgtAnchorSize() {
    return elementTgtAnchorSize;
  }
  /**
   * Set the property name for elementSourceAnchorSize
   */
  void setElementTgtAnchorSize(SizeProperty *property) {
    elementTgtAnchorSize=property;
  }
  /**
   * Return a pointer on the property used to elementAnimationFrame
   */
  IntegerProperty *getElementAnimationFrame() {
    return elementAnimationFrame;
  }
  /**
   * Set the pointer on the property used to elementAnimationFrame
   */
  void setElementAnimationFrame(IntegerProperty *property) {
    elementAnimationFrame=property;
  }




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


public :

  Graph* graph;

  GlGraphRenderingParameters* parameters;

  MutableContainer<Glyph *> glyphs;
  MutableContainer<EdgeExtremityGlyph *> extremityGlyphs;

protected:

  bool deleteGlVertexArrayManager;

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
