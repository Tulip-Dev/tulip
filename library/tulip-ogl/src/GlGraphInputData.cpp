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
#include "tulip/GlGraphInputData.h"

#include <tulip/Graph.h>
#include <tulip/DrawingTools.h>

#include "tulip/GlyphManager.h"
#include <tulip/EdgeExtremityGlyphManager.h>
#include "tulip/GlGraphRenderingParameters.h"
#include "tulip/GlVertexArrayManager.h"

namespace tlp {
  GlGraphInputData::GlGraphInputData(Graph* graph,GlGraphRenderingParameters* parameters,GlMetaNodeRenderer *renderer):
    deleteGlVertexArrayManager(true),
    elementAnimationFrame(new IntegerProperty(graph,"viewAnimationFrame")),
    graph(graph),
    parameters(parameters),
    elementColorPropName("viewColor"), elementLabelColorPropName("viewLabelColor"), elementSizePropName("viewSize"),
    elementLabelPositionPropName("viewLabelPosition"),elementShapePropName("viewShape"), elementRotationPropName("viewRotation"),
    elementSelectedPropName("viewSelection"),elementFontPropName("viewFont"),elementFontSizePropName("viewFontSize"),
    elementLabelPropName("viewLabel"), elementTexturePropName("viewTexture"),
    elementBorderColorPropName("viewBorderColor"), elementBorderWidthPropName("viewBorderWidth"), elementLayoutPropName(""),
    elementSrcAnchorShapePropName("viewSrcAnchorShape"),elementSrcAnchorSizePropName("viewSrcAnchorSize"),
    elementTgtAnchorShapePropName("viewTgtAnchorShape"),elementTgtAnchorSizePropName("viewTgtAnchorSize")
    {

    reloadAllProperties();
    GlyphManager::getInst().initGlyphList(&this->graph, this, glyphs);

    EdgeExtremityGlyphManager::getInst().initGlyphList(&this->graph, this,
                                                       extremityGlyphs);
    if(renderer)
      metaNodeRenderer=renderer;
    else
      metaNodeRenderer=new GlMetaNodeRenderer();

    glVertexArrayManager=new GlVertexArrayManager(this);
  }

GlGraphInputData::~GlGraphInputData() {
	if(deleteGlVertexArrayManager)
		delete glVertexArrayManager;
	GlyphManager::getInst().clearGlyphList(&this->graph, this, glyphs);
	EdgeExtremityGlyphManager::getInst().clearGlyphList(&this->graph, this,
			extremityGlyphs);
}

void GlGraphInputData::reloadLayoutProperty() {
    if(!graph->attributeExist("viewLayout")){
      if(elementLayoutPropName==""){
	elementLayout = graph->getProperty<LayoutProperty>("viewLayout");
      }else{
	elementLayout = graph->getProperty<LayoutProperty>(elementLayoutPropName);
      }
    }else {
      graph->getAttribute("viewLayout",elementLayout);      
    }
}

void GlGraphInputData::reloadAllProperties() {
	reloadLayoutProperty();

	elementRotation = graph->getProperty<DoubleProperty> (
			elementRotationPropName);
    elementFont = graph->getProperty<StringProperty>(elementFontPropName);
    elementFontSize = graph->getProperty<IntegerProperty>(elementFontSizePropName);
	elementSelected = graph->getProperty<BooleanProperty> (
			elementSelectedPropName);
	elementLabel = graph->getProperty<StringProperty> (elementLabelPropName);
    	elementLabelColor = graph->getProperty<ColorProperty> (
			elementLabelColorPropName);
	elementLabelPosition = graph->getProperty<IntegerProperty> (
			elementLabelPositionPropName);
	elementColor = graph->getProperty<ColorProperty> (elementColorPropName);
	elementShape = graph->getProperty<IntegerProperty> (elementShapePropName);
	elementSize = graph->getProperty<SizeProperty> (elementSizePropName);
	elementTexture
			= graph->getProperty<StringProperty> (elementTexturePropName);
	elementBorderColor = graph->getProperty<ColorProperty> (
			elementBorderColorPropName);
	elementBorderWidth = graph->getProperty<DoubleProperty> (
			elementBorderWidthPropName);

	elementSrcAnchorShape = graph->getProperty<IntegerProperty> (
			elementSrcAnchorShapePropName);
	elementSrcAnchorSize = graph->getProperty<SizeProperty> (
			elementSrcAnchorSizePropName);
	elementTgtAnchorShape = graph->getProperty<IntegerProperty> (
			elementTgtAnchorShapePropName);
	elementTgtAnchorSize = graph->getProperty<SizeProperty> (
			elementTgtAnchorSizePropName);
}

}
