/**
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

#include <tulip/GlyphPreviewRenderer.h>
#include <tulip/GlyphsManager.h>
#include <tulip/Graph.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlGraph.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/Camera.h>
#include <tulip/GlLayer.h>
#include <tulip/ZoomAndPanAnimation.h>

using namespace tlp;
using namespace std;

GlyphPreviewRenderer GlyphPreviewRenderer::_instance;

GlyphPreviewRenderer::GlyphPreviewRenderer():_graph(newGraph()),_node(_graph->addNode()) {
  _graph->getColorProperty("viewColor").setAllNodeValue(Color(192,192,192));
  _graph->getDoubleProperty("viewBorderWidth").setAllNodeValue(1);
}

GlyphPreviewRenderer::~GlyphPreviewRenderer() {
  delete _graph;
}

GlyphPreviewRenderer& GlyphPreviewRenderer::instance() {
  return _instance;
}

QPixmap GlyphPreviewRenderer::render(unsigned int pluginId) {
  if(_previews.find(pluginId) == _previews.end()) {
    _graph->getIntegerProperty("viewShape").setNodeValue(_node, pluginId);
    GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
    renderer->setViewPortSize(16,16);
    renderer->clearScene();
    renderer->addGraphToScene(_graph);
    renderer->getScene()->centerScene();
    renderer->getScene()->getMainLayer()->getCamera()->setZoomFactor(0.9);
    renderer->renderScene(false, true);
    QImage preview = renderer->getImage();
    _previews[pluginId] = QPixmap::fromImage(preview);
  }

  return _previews[pluginId];
}

EdgeExtremityGlyphPreviewRenderer EdgeExtremityGlyphPreviewRenderer::_instance;

EdgeExtremityGlyphPreviewRenderer::EdgeExtremityGlyphPreviewRenderer():_graph(newGraph()) {
  //_graph->getColorProperty("viewColor").setAllNodeValue(Color(255,255,255,0));
  //_graph->getColorProperty("viewBorderColor").setAllNodeValue(Color(255,255,255,0));
  _graph->getColorProperty("viewColor").setAllEdgeValue(Color(192,192,192));
  _graph->getColorProperty("viewBorderColor").setAllEdgeValue(Color(0,0,0));
  node n1 = _graph->addNode();
  node n2= _graph->addNode();
  _edge = _graph->addEdge(n1,n2);
  _graph->getLayoutProperty("viewLayout").setNodeValue(n1,Coord(0,0,0));
  _graph->getLayoutProperty("viewLayout").setNodeValue(n2,Coord(5,0,0));
  _graph->getIntegerProperty("viewTgtAnchorShape").setAllEdgeValue(EdgeExtremityShape::None);
}

EdgeExtremityGlyphPreviewRenderer::~EdgeExtremityGlyphPreviewRenderer() {
  delete _graph;
}

EdgeExtremityGlyphPreviewRenderer& EdgeExtremityGlyphPreviewRenderer::instance() {
  return _instance;
}

QPixmap EdgeExtremityGlyphPreviewRenderer::render(unsigned int pluginId) {

  if (_previews.find(EdgeExtremityShape::None) == _previews.end()) {
    _previews[EdgeExtremityShape::None] = QPixmap();
  }

  if(_previews.find(pluginId) == _previews.end()) {
    _graph->getIntegerProperty("viewTgtAnchorShape").setEdgeValue(_edge, pluginId);
    GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
    renderer->setViewPortSize(16,16);
    renderer->clearScene();
    renderer->addGraphToScene(_graph);
    GlGraphRenderingParameters &renderingParameters = renderer->getScene()->getMainGlGraph()->getRenderingParameters();
    renderingParameters.setInterpolateEdgesColors(false);
    renderingParameters.setInterpolateEdgesSizes(true);
    renderingParameters.setDisplayEdgesExtremities(true);
    adjustViewToBoundingBox(renderer->getScene()->getMainLayer()->getCamera(), BoundingBox(Coord(4.1, -0.1),Coord(4.5, 0.1)));
    renderer->renderScene(false, true);
    QImage preview = renderer->getImage();
    _previews[pluginId] = QPixmap::fromImage(preview);
  }

  return _previews[pluginId];
}

