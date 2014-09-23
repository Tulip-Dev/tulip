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

#include <tulip/GlTextureManager.h>
#include <tulip/Gl2DRect.h>
#include <tulip/ForEach.h>
#include <tulip/GlLabel.h>
#include <tulip/GlProgressBar.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlGraphComposite.h>

#include "PixelOrientedOverview.h"


using namespace pocore;
using namespace std;

template <typename T>
std::string getStringFromNumber(T number) {
  std::ostringstream oss;
  oss.precision(5);
  oss << number;
  return oss.str();
}

static void setGraphView (tlp::GlGraphComposite *glGraph) {
  tlp::GlGraphRenderingParameters param = glGraph->getRenderingParameters ();
  param.setAntialiasing (true);
  param.setViewNodeLabel (false);
  param.setFontsType (2);
  param.setSelectedNodesStencil(1);
  param.setNodesStencil(0xFFFF);
  param.setNodesLabelStencil(0xFFFF);
  param.setDisplayEdges(false);
  param.setDisplayNodes(true);
  glGraph->setRenderingParameters (param);
}

namespace tlp {

int PixelOrientedOverview::overviewCpt(0);

PixelOrientedOverview::PixelOrientedOverview(TulipGraphDimension *data, PixelOrientedMediator *pixelOrientedMediator,
    Coord blCornerPos, const std::string &dimName, const Color &backgroundColor, const Color &textColor)
  : data(data), pixelOrientedMediator(pixelOrientedMediator), blCornerPos(blCornerPos),
    dimName(dimName), frame(NULL), frame2(NULL), overviewGen(false), backgroundColor(backgroundColor), textColor(textColor) {

  if (this->dimName == "") {
    this->dimName = data->getDimensionName();
  }

  overviewId = overviewCpt++;

  textureName = dimName + " texture " + getStringFromNumber(overviewId);

  unsigned int width = pixelOrientedMediator->getImageWidth();
  unsigned int height = pixelOrientedMediator->getImageHeight();

  unsigned int labelHeight = height / 4;

  Graph *graph = data->getTulipGraph();
  pixelLayout = new LayoutProperty(graph);
  pixelSize = new SizeProperty(graph);
  graphComposite = new GlGraphComposite(graph);
  setGraphView(graphComposite);
  GlGraphInputData *glGraphInputData = graphComposite->getInputData();
  glGraphInputData->setElementLayout(pixelLayout);
  glGraphInputData->setElementSize(pixelSize);

  frame = new GlRect(Coord(blCornerPos.getX() - 3, blCornerPos.getY() + height + 3),
                     Coord(blCornerPos.getX() + width + 3, blCornerPos.getY() - 3),
                     Color(0, 0, 0), Color(0, 0, 0), false, true);
  addGlEntity(frame, dimName + "frame");
  frame2 = new GlRect(Coord(blCornerPos.getX() - 4, blCornerPos.getY() + height + 4),
                      Coord(blCornerPos.getX() + width + 4, blCornerPos.getY() - 4),
                      Color(0, 0, 0), Color(0, 0, 0), false, true);
  addGlEntity(frame2, dimName + "frame 2");

  backgroundRect = new GlRect(Coord(blCornerPos.getX(), blCornerPos.getY() + height), Coord(blCornerPos.getX() + width, blCornerPos.getY()), Color(255,255,255), Color(255,255,255), true, false);
  addGlEntity(backgroundRect, "background rect");
  clickLabel = new GlLabel(Coord(blCornerPos.getX() + width / 2, blCornerPos.getY() + height / 2), Size(width, height / 4), Color(0,0,0));
  clickLabel->setText("Double Click to generate overview");
  addGlEntity(clickLabel, "label");


  computeBoundingBox();

  overviewLabel = new GlLabel(Coord(blCornerPos.getX() + width / 2, blCornerPos.getY() - labelHeight / 2), Size(width, labelHeight), textColor);
  overviewLabel->setText(dimName);
  addGlEntity(overviewLabel, "overview label");
}

PixelOrientedOverview::~PixelOrientedOverview() {
  GlTextureManager::getInst().deleteTexture(textureName);
  reset(true);
}

struct NodeCoordXOrdering : public binary_function<pair<node, Coord>, pair<node, Coord>, bool> {
  bool operator()(pair<node, Coord> p1, pair<node, Coord> p2) {
    return p1.second.getX() < p2.second.getX();
  }
};

struct NodeCoordYOrdering : public binary_function<pair<node, Coord>, pair<node, Coord>, bool> {
  bool operator()(pair<node, Coord> p1, pair<node, Coord> p2) {
    return p1.second.getY() > p2.second.getY();
  }
};

void PixelOrientedOverview::computePixelView(GlMainWidget *glWidget) {

  reset(false);

  if (clickLabel != NULL) {
    delete clickLabel;
    clickLabel = NULL;
  }

  if (backgroundRect != NULL) {
    delete backgroundRect;
    backgroundRect = NULL;
  }

  if (frame != NULL) {
    delete frame;
    frame = NULL;
  }

  if (frame2 != NULL) {
    delete frame2;
    frame2 = NULL;
  }

  Graph *graph = data->getTulipGraph();

  unsigned int width = pixelOrientedMediator->getImageWidth();
  unsigned int height = pixelOrientedMediator->getImageHeight();

  GlProgressBar *glProgressBar = NULL;

  if (glWidget != NULL) {
    glProgressBar = new GlProgressBar(Coord(blCornerPos.getX() + width / 2, blCornerPos.getY() + height / 2), width, height, Color(0,0,255));
    glProgressBar->setComment("Generating overview ...");
    addGlEntity(glProgressBar, "progress bar");
  }

  unsigned int currentStep = 0;
  unsigned int maxStep = graph->numberOfNodes();
  unsigned int drawStep = maxStep / 10;

  set<int> xCoordSet;

  for (unsigned int i = 0 ; i < graph->numberOfNodes() ; ++i) {
    node n = node(data->getItemIdAtRank(i));
    pocore::Vec2i pos = pixelOrientedMediator->getPixelPosForRank(i);
    Coord nodeCoord = Coord(pos[0], pos[1], 0);
    xCoordSet.insert(pos[0]);
    pixelLayout->setNodeValue(n, nodeCoord);
    ++currentStep;

    if (glWidget != NULL && currentStep % drawStep == 0) {
      glProgressBar->progress(currentStep, maxStep);
      glWidget->draw();
    }
  }

  if (xCoordSet.size() < 2)
    return;

  set<int>::iterator it = xCoordSet.begin();
  int x1 = *(it++);
  int x2 = *it;
  int size = x2 - x1;

  pixelSize->setAllNodeValue(Size(size,size,size));

  overviewLabel->setColor(textColor);

  GlOffscreenRenderer *glOffscreenRenderer = GlOffscreenRenderer::getInstance();

  glOffscreenRenderer->setViewPortSize(width, height);
  glOffscreenRenderer->clearScene();
  glOffscreenRenderer->setSceneBackgroundColor(backgroundColor);
  glOffscreenRenderer->addGraphCompositeToScene(graphComposite);
  glOffscreenRenderer->renderScene(true);

  if (glWidget != NULL) {
    glProgressBar->progress(maxStep, maxStep);
    glWidget->draw();

    deleteGlEntity(glProgressBar);
    delete glProgressBar;
  }


  GLuint textureId = glOffscreenRenderer->getGLTexture(true);
  GlTextureManager::getInst().deleteTexture(textureName);
  GlTextureManager::getInst().registerExternalTexture(textureName, textureId);

  if (findGlEntity(dimName) == NULL) {
    addGlEntity(new Gl2DRect(blCornerPos.getY() + pixelOrientedMediator->getImageHeight() , blCornerPos.getY()
                             , blCornerPos.getX(), blCornerPos.getX()
                             + pixelOrientedMediator->getImageWidth(), textureName), dimName);
    addGlEntity(overviewLabel, "overview label");
    computeBoundingBox();
  }

  overviewGen = true;
}

void PixelOrientedOverview::setBLCorner(const Coord &blCorner) {
  GlComposite::translate(blCorner - blCornerPos);
  blCornerPos = blCorner;
  computeBoundingBox();
}

}
