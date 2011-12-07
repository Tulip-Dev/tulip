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
#include <tulip/GlGraphInputData.h>

#include <tulip/Graph.h>

#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/GlVertexArrayManager.h>

namespace tlp {
GlGraphInputData::GlGraphInputData(Graph* graph,GlGraphRenderingParameters* parameters,GlMetaNodeRenderer *renderer):
  graph(graph),
  parameters(parameters),
  deleteGlVertexArrayManager(true),
  deleteMetaNodeRendererAtDestructor(true) {

  propertiesMap[VIEW_COLOR]=graph->getProperty<ColorProperty>("viewColor");
  _properties.insert(propertiesMap[VIEW_COLOR]);
  propertiesMap[VIEW_LABELCOLOR]=graph->getProperty<ColorProperty>("viewLabelColor");
  _properties.insert(propertiesMap[VIEW_LABELCOLOR]);
  propertiesMap[VIEW_SIZE]=graph->getProperty<SizeProperty>("viewSize");
  _properties.insert(propertiesMap[VIEW_SIZE]);
  propertiesMap[VIEW_LABEL]=graph->getProperty<StringProperty>("viewLabel");
  _properties.insert(propertiesMap[VIEW_LABEL]);
  propertiesMap[VIEW_LABELPOSITION]=graph->getProperty<IntegerProperty>("viewLabelPosition");
  _properties.insert(propertiesMap[VIEW_LABELPOSITION]);
  propertiesMap[VIEW_SHAPE]=graph->getProperty<IntegerProperty>("viewShape");
  _properties.insert(propertiesMap[VIEW_SHAPE]);
  propertiesMap[VIEW_ROTATION]=graph->getProperty<DoubleProperty>("viewRotation");
  _properties.insert(propertiesMap[VIEW_ROTATION]);
  propertiesMap[VIEW_SELECTED]=graph->getProperty<BooleanProperty>("viewSelection");
  _properties.insert(propertiesMap[VIEW_SELECTED]);
  propertiesMap[VIEW_FONT]=graph->getProperty<StringProperty>("viewFont");
  _properties.insert(propertiesMap[VIEW_FONT]);
  propertiesMap[VIEW_FONTSIZE]=graph->getProperty<IntegerProperty>("viewFontSize");
  _properties.insert(propertiesMap[VIEW_FONTSIZE]);
  propertiesMap[VIEW_TEXTURE]=graph->getProperty<StringProperty>("viewTexture");
  _properties.insert(propertiesMap[VIEW_TEXTURE]);
  propertiesMap[VIEW_BORDERCOLOR]=graph->getProperty<ColorProperty>("viewBorderColor");
  _properties.insert(propertiesMap[VIEW_BORDERCOLOR]);
  propertiesMap[VIEW_BORDERWIDTH]=graph->getProperty<DoubleProperty>("viewBorderWidth");
  _properties.insert(propertiesMap[VIEW_BORDERWIDTH]);
  propertiesMap[VIEW_LAYOUT]=graph->getProperty<LayoutProperty>("viewLayout");
  _properties.insert(propertiesMap[VIEW_LAYOUT]);
  propertiesMap[VIEW_SRCANCHORSHAPE]=graph->getProperty<IntegerProperty>("viewSrcAnchorShape");
  _properties.insert(propertiesMap[VIEW_SRCANCHORSHAPE]);
  propertiesMap[VIEW_SRCANCHORSIZE]=graph->getProperty<SizeProperty>("viewSrcAnchorSize");
  _properties.insert(propertiesMap[VIEW_SRCANCHORSIZE]);
  propertiesMap[VIEW_TGTANCHORSHAPE]=graph->getProperty<IntegerProperty>("viewTgtAnchorShape");
  _properties.insert(propertiesMap[VIEW_TGTANCHORSHAPE]);
  propertiesMap[VIEW_TGTANCHORSIZE]=graph->getProperty<SizeProperty>("viewTgtAnchorSize");
  _properties.insert(propertiesMap[VIEW_TGTANCHORSIZE]);
  propertiesMap[VIEW_ANIMATIONFRAME]=new IntegerProperty(graph,"viewAnimationFrame");
  _properties.insert(propertiesMap[VIEW_ANIMATIONFRAME]);

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

  if(deleteMetaNodeRendererAtDestructor)
    delete metaNodeRenderer;
}
}
