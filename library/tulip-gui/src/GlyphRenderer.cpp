/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/tuliphash.h>

#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/GlyphRenderer.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/GraphImpl.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Camera.h>
#include <tulip/PluginLister.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;
using namespace std;

#define ALL_GLYPHS -1
static tlp_hash_map<int, QPixmap> nodePreviews;
static QPixmap renderNodeGlyph(int glyphId, int dim) {
  // Init graph parameters.
  GlGraphRenderingParameters parameters;
  auto graph = GraphImpl::newGraph();
  auto node = graph->addNode();
  QPixmap pxmp;
  {
    // need a block to ensure inputData
    // will be destroyed before graph
    GlGraphInputData inputData(graph, &parameters);
    inputData.getElementSize()->setAllNodeValue(Size(1, 1, 1));
    inputData.getElementColor()->setAllNodeValue(Color(192, 192, 192));
    inputData.getElementBorderColor()->setAllNodeValue(Color(0, 0, 0));
    inputData.getElementBorderWidth()->setAllNodeValue(1);

    GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
    renderer->setViewPortSize(dim, dim);
    renderer->clearScene();
    // use a transparent background color
    renderer->setSceneBackgroundColor(Color(255, 255, 255, 0));
    renderer->addGraphToScene(graph);
    renderer->getScene()->centerScene();
    renderer->getScene()->getGraphCamera().setZoomFactor(0.9);
    if (glyphId == ALL_GLYPHS) {
      // init previews
      for (const auto &glyphName : PluginLister::availablePlugins<Glyph>()) {
        auto glId = GlyphManager::glyphId(glyphName);
        // Create the glyph preview
        graph->getProperty<IntegerProperty>("viewShape")->setNodeValue(node, glId);
        renderer->renderScene(false, true);
        // get preview image with transparency
        nodePreviews.emplace(glId, QPixmap::fromImage(renderer->getImage(true)));
      }
    } else {
      // render the glyph
      graph->getProperty<IntegerProperty>("viewShape")->setNodeValue(node, glyphId);
      renderer->renderScene(false, true);
      // get pixmap
      pxmp = QPixmap::fromImage(renderer->getImage(true));
    }
    renderer->clearScene(true);
  }
  // graph is no longer needed
  delete graph;
  return pxmp;
}

QPixmap GlyphRenderer::render(int glyphId, int dim) {
  if (dim == GLYPH_PREVIEW_DIM) {
    if (nodePreviews.empty()) {
      renderNodeGlyph(ALL_GLYPHS, GLYPH_PREVIEW_DIM);
    }
  }
  auto it = nodePreviews.find(glyphId);
  if (it != nodePreviews.end())
    return (dim == GLYPH_PREVIEW_DIM) ? it->second : renderNodeGlyph(glyphId, dim);
  return QPixmap(":/tulip/gui/icons/i_invalid.png");
}

QPixmap GlyphRenderer::render(QString glyphName, int dim) {
  return render(GlyphManager::glyphId(QStringToTlpString(glyphName)), dim);
}

static tlp_hash_map<int, QPixmap> eePreviews;
static QPixmap renderEEGlyph(int glyphId, int dim) {
  QPixmap pxmp;
  eePreviews[EdgeExtremityShape::None] = pxmp;
  // Init graph parameters.
  GlGraphRenderingParameters parameters;
  auto graph = GraphImpl::newGraph();
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
    bends.emplace_back(0.01f, 0, 0);
    inputData.getElementLayout()->setAllEdgeValue(bends);

    inputData.getElementSrcAnchorShape()->setAllEdgeValue(EdgeExtremityShape::None);
    inputData.getElementTgtAnchorSize()->setAllEdgeValue(Size(2, 2, 1));

    GlOffscreenRenderer *renderer = GlOffscreenRenderer::getInstance();
    renderer->setViewPortSize(dim, dim);
    renderer->clearScene();
    // use a transparent background color
    renderer->setSceneBackgroundColor(Color(255, 255, 255, 0));
    renderer->addGraphToScene(graph);
    GlGraphRenderingParameters renderingParamerters =
        renderer->getScene()->getGlGraphComposite()->getRenderingParameters();
    renderingParamerters.setEdgeColorInterpolate(false);
    renderingParamerters.setEdgeSizeInterpolate(false);
    renderingParamerters.setViewArrow(true);
    renderer->getScene()->getGlGraphComposite()->setRenderingParameters(renderingParamerters);
    if (glyphId == ALL_GLYPHS) {
      // init previews
      for (const auto &glyphName : PluginLister::availablePlugins<EdgeExtremityGlyph>()) {
        const tlp::Plugin &info = PluginLister::pluginInformation(glyphName);
        int glId = info.id();
        // Create the glyph preview
        graph->getProperty<IntegerProperty>("viewTgtAnchorShape")->setEdgeValue(e, glId);
        renderer->renderScene(true);
        eePreviews.emplace(glId, QPixmap::fromImage(renderer->getImage(true)));
      }
    } else {
      // render the glyph
      graph->getProperty<IntegerProperty>("viewTgtAnchorShape")->setEdgeValue(e, glyphId);
      renderer->renderScene(true);
      // get pixmap
      pxmp = QPixmap::fromImage(renderer->getImage(true));
    }
    renderer->clearScene(true);
  }
  // graph is no longer needed
  delete graph;
  return pxmp;
}

QPixmap EdgeExtremityGlyphRenderer::render(int glyphId, int dim) {
  if (dim == GLYPH_PREVIEW_DIM) {
    if (eePreviews.empty()) {
      renderEEGlyph(ALL_GLYPHS, GLYPH_PREVIEW_DIM);
    }
  }
  auto it = eePreviews.find(glyphId);
  if (it != eePreviews.end())
    return (dim == GLYPH_PREVIEW_DIM) ? it->second : renderEEGlyph(glyphId, dim);
  return QPixmap(":/tulip/gui/icons/i_invalid.png");
}

QPixmap EdgeExtremityGlyphRenderer::render(QString glyphName, int dim) {
  return render(EdgeExtremityGlyphManager::glyphId(QStringToTlpString(glyphName)), dim);
}
