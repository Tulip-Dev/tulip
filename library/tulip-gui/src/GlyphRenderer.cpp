/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <map>

#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/GlyphRenderer.h>
#include <tulip/GlyphManager.h>
#include <tulip/Graph.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Camera.h>

using namespace tlp;
using namespace std;

QPixmap GlyphRenderer::render(int glyphId) {
  static std::unordered_map<int, QPixmap> previews;
  static bool inited = false;
  if (!inited) {
    inited = true;
    // Init graph parameters.
    GlGraphRenderingParameters parameters;
    auto graph = newGraph();
    auto node = graph->addNode();
    {
      // need a block to ensure inputData
      // will be destroyed before graph
      GlGraphInputData inputData(graph, &parameters);
      inputData.getElementSize()->setAllNodeValue(Size(1, 1, 1));
      inputData.getElementColor()->setAllNodeValue(Color(192, 192, 192));
      inputData.getElementBorderColor()->setAllNodeValue(Color(0, 0, 0));
      inputData.getElementBorderWidth()->setAllNodeValue(1);

      GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
      renderer->setViewPortSize(16, 16);
      renderer->clearScene();
      renderer->addGraphToScene(graph);
      renderer->getScene()->centerScene();
      renderer->getScene()->getGraphCamera().setZoomFactor(0.9);
      // init previews
      for (const std::string &glyphName : PluginLister::availablePlugins<Glyph>()) {
        auto glId = GlyphManager::glyphId(glyphName);
        // Create the glyph preview
        graph->getProperty<IntegerProperty>("viewShape")->setNodeValue(node, glId);
        renderer->renderScene(false, true);
        previews.emplace(std::make_pair(glId, QPixmap::fromImage(renderer->getImage())));
      }
      renderer->clearScene(true);
    }
    // graph is no longer needed
    delete graph;
  }
  auto it = previews.find(glyphId);
  if (it == previews.end())
    return QPixmap(":/tulip/gui/icons/i_invalid.png");
  return it->second;
}

QPixmap EdgeExtremityGlyphRenderer::render(int glyphId) {
  static std::unordered_map<int, QPixmap> previews;
  static bool inited = false;
  if (!inited) {
    inited = true;
    previews[EdgeExtremityShape::None] = QPixmap();
    // Init graph parameters.
    GlGraphRenderingParameters parameters;
    auto graph = newGraph();
    node n1 = graph->addNode();
    node n2 = graph->addNode();
    edge e = graph->addEdge(n1, n2);
    {
      // need a block to ensure inputData
      // will be destroyed before graph
      GlGraphInputData inputData(graph, &parameters);
      Color white(255, 255, 255, 0), black(0, 0, 0, 0);
      inputData.getElementSize()->setAllNodeValue(Size(0.01f, 0.2f, 0.1f));
      inputData.getElementSize()->setAllEdgeValue(Size(0.125f, 0.125f, 0.125f));
      inputData.getElementColor()->setAllNodeValue(white);
      inputData.getElementBorderColor()->setAllNodeValue(white);
      inputData.getElementColor()->setAllEdgeValue(Color(192, 192, 192));
      inputData.getElementBorderColor()->setAllEdgeValue(black);
      inputData.getElementLayout()->setNodeValue(n1, Coord(0, 0, 0));
      inputData.getElementLayout()->setNodeValue(n2, Coord(0.3f, 0, 0));
      vector<Coord> bends;
      bends.push_back(Coord(0.01f, 0, 0));
      inputData.getElementLayout()->setAllEdgeValue(bends);

      inputData.getElementSrcAnchorShape()->setAllEdgeValue(EdgeExtremityShape::None);
      inputData.getElementTgtAnchorSize()->setAllEdgeValue(Size(2, 2, 1));

      GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
      renderer->setViewPortSize(16, 16);
      renderer->clearScene();
      renderer->addGraphToScene(graph);
      GlGraphRenderingParameters renderingParamerters =
          renderer->getScene()->getGlGraphComposite()->getRenderingParameters();
      renderingParamerters.setEdgeColorInterpolate(false);
      renderingParamerters.setEdgeSizeInterpolate(false);
      renderingParamerters.setViewArrow(true);
      renderer->getScene()->getGlGraphComposite()->setRenderingParameters(renderingParamerters);
      // init previews
      for (std::string glyphName :
           PluginLister::availablePlugins<EdgeExtremityGlyph>()) {
        const tlp::Plugin &info = PluginLister::pluginInformation(glyphName);
        int glId = info.id();
        // Create the glyph preview
        graph->getProperty<IntegerProperty>("viewTgtAnchorShape")->setEdgeValue(e, glId);
        renderer->renderScene(true);
        previews.emplace(std::make_pair(glId, QPixmap::fromImage(renderer->getImage())));
      }
      renderer->clearScene(true);
    }
    // graph is no longer needed
    delete graph;
  }
  auto it = previews.find(glyphId);
  if (it == previews.end())
    return QPixmap(":/tulip/gui/icons/i_invalid.png");
  return it->second;
}
