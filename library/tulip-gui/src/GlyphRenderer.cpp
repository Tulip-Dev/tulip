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

#include <tulip/GlyphRenderer.h>
#include <tulip/GlyphManager.h>
#include <tulip/Graph.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/EdgeExtremityGlyph.h>

using namespace tlp;
using namespace std;

GlyphRenderer* GlyphRenderer::_instance=NULL;

GlyphRenderer::GlyphRenderer():_graph(newGraph()),_node(_graph->addNode()) {
  //Init graph parameters.
  GlGraphRenderingParameters parameters;
  {
    // need a block to ensure inputData
    // will be destroyed before _graph
    GlGraphInputData inputData(_graph,&parameters);
    inputData.getElementSize()->setAllNodeValue(Size(1,1,1));
    inputData.getElementColor()->setAllNodeValue(Color(192,192,192));
    inputData.getElementBorderColor()->setAllNodeValue(Color(0,0,0));
    inputData.getElementBorderWidth()->setAllNodeValue(1);

    // init previews
    std::list<std::string> glyphs(PluginLister::instance()->availablePlugins<Glyph>());

    for(std::list<std::string>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it) {
      std::string glyphName(*it);
      int glyphIndex = GlyphManager::getInst().glyphId(glyphName);
      //Create the glyph preview
      render(glyphIndex);
    }
  }

  // _graph is no longer needed
  delete _graph;
  _graph = NULL;
}

GlyphRenderer::~GlyphRenderer() {
  assert(_graph = NULL);
}

GlyphRenderer& GlyphRenderer::getInst() {
  if(_instance==NULL) {
    _instance = new GlyphRenderer();
  }

  return *_instance;
}

QPixmap GlyphRenderer::render(unsigned int pluginId) {
  if(_previews.find(pluginId) == _previews.end()) {
    _graph->getProperty<IntegerProperty>("viewShape")->setNodeValue(_node,pluginId);
    GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
    renderer->setViewPortSize(16,16);
    renderer->clearScene();
    renderer->addGraphToScene(_graph);
    renderer->renderScene(true);
    QImage preview = renderer->getImage();
    _previews[pluginId] = QPixmap::fromImage(preview);
  }

  return _previews[pluginId];
}

EdgeExtremityGlyphRenderer* EdgeExtremityGlyphRenderer::_instance = NULL;
EdgeExtremityGlyphRenderer::EdgeExtremityGlyphRenderer():_graph(newGraph()) {
  //No edge extremity pixmap
  _previews[EdgeExtremityShape::None] = QPixmap();

  //Init graph.
  GlGraphRenderingParameters parameters;
  GlGraphInputData inputData(_graph,&parameters);
  inputData.getElementSize()->setAllNodeValue(Size(0.01f,0.2f,0.1f));
  inputData.getElementSize()->setAllEdgeValue(Size(0.125f,0.125f,0.125f));
  inputData.getElementColor()->setAllNodeValue(Color(255,255,255,0));
  inputData.getElementBorderColor()->setAllNodeValue(Color(255,255,255,0));
  inputData.getElementColor()->setAllEdgeValue(Color(192,192,192));
  inputData.getElementBorderColor()->setAllEdgeValue(Color(0,0,0));
  node n1 = _graph->addNode();
  node n2= _graph->addNode();
  _edge = _graph->addEdge(n1,n2);
  inputData.getElementLayout()->setNodeValue(n1,Coord(0,0,0));
  inputData.getElementLayout()->setNodeValue(n2,Coord(0.3f,0,0));
  vector<Coord> bends;
  bends.push_back(Coord(0.01f,0,0));
  inputData.getElementLayout()->setAllEdgeValue(bends);

  inputData.getElementSrcAnchorShape()->setAllEdgeValue(EdgeExtremityShape::None);
  inputData.getElementTgtAnchorSize()->setAllEdgeValue(Size(2,2,1));

  // init previews
  std::list<std::string> glyphs(PluginLister::instance()->availablePlugins<EdgeExtremityGlyph>());

  for(std::list<std::string>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it) {
    std::string glyphName(*it);
    const tlp::Plugin& infos = PluginLister::pluginInformation(glyphName);
    int glyphIndex = infos.id();
    //Create the glyph preview
    render(glyphIndex);
  }

  // _graph is no longer needed
  delete _graph;
  _graph = NULL;
}

EdgeExtremityGlyphRenderer::~EdgeExtremityGlyphRenderer() {
  assert(_graph == NULL);
}

EdgeExtremityGlyphRenderer & EdgeExtremityGlyphRenderer::getInst() {
  if(_instance==NULL) {
    _instance = new EdgeExtremityGlyphRenderer();
  }

  return *_instance;
}

QPixmap EdgeExtremityGlyphRenderer::render(unsigned int pluginId) {
  if(_previews.find(pluginId) == _previews.end()) {
    _graph->getProperty<IntegerProperty>("viewTgtAnchorShape")->setEdgeValue(_edge,pluginId);
    GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
    renderer->setViewPortSize(16,16);
    renderer->clearScene();
    renderer->addGraphToScene(_graph);
    GlGraphRenderingParameters renderingParamerters = renderer->getScene()->getGlGraphComposite()->getRenderingParameters();
    renderingParamerters.setEdgeColorInterpolate(false);
    renderingParamerters.setEdgeSizeInterpolate(false);
    renderingParamerters.setViewArrow(true);
    renderer->getScene()->getGlGraphComposite()->setRenderingParameters(renderingParamerters);
    renderer->renderScene(true);
    QImage preview = renderer->getImage();
    _previews[pluginId] = QPixmap::fromImage(preview);
  }

  return _previews[pluginId];
}

