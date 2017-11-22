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

#include <tulip/ForEach.h>
#include <QThread>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/Gl2DRect.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlLabel.h>
#include <tulip/GlProgressBar.h>
#include <tulip/GlQuantitativeAxis.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlTextureManager.h>

#include "ScatterPlot2D.h"

using namespace std;

const float DEFAULT_AXIS_LENGTH = 1000.0f;
const unsigned int DEFAULT_NB_GRADS = 15;

template <typename T>
std::string getStringFromNumber(T number) {
  std::ostringstream oss;
  oss.precision(5);
  oss << number;
  return oss.str();
}

namespace tlp {

int ScatterPlot2D::overviewCpt(0);

static void setGraphView(GlGraphComposite *glGraph, bool displayEdges, bool nodelabel, bool scale) {
  GlGraphRenderingParameters param = glGraph->getRenderingParameters();
  param.setAntialiasing(true);
  param.setViewNodeLabel(nodelabel);
  param.setFontsType(2);
  param.setSelectedNodesStencil(1);
  param.setNodesStencil(0xFFFF);
  param.setNodesLabelStencil(0xFFFF);
  param.setDisplayEdges(displayEdges);
  param.setEdgesStencil(0xFFFF);
  param.setSelectedEdgesStencil(1);
  param.setDisplayNodes(true);
  param.setDisplayMetaNodes(true);
  param.setLabelScaled(scale);
  glGraph->setRenderingParameters(param);
}

ScatterPlot2D::ScatterPlot2D(Graph *graph, Graph *edgeGraph, std::map<node, edge> &nodeMap,
                             const string &xDim, const string &yDim,
                             const ElementType &dataLocation, Coord blCorner, unsigned int size,
                             const Color &backgroundColor, const Color &foregroundColor)
    : xDim(xDim), yDim(yDim), blCorner(blCorner), size(size), graph(graph),
      scatterLayout(new LayoutProperty(graph)), scatterEdgeLayout(new LayoutProperty(graph)),
      xAxis(nullptr), yAxis(nullptr), overviewGen(false), backgroundColor(backgroundColor),
      foregroundColor(foregroundColor), mapBackgroundColorToCoeff(false),
      edgeAsNodeGraph(edgeGraph), nodeToEdge(nodeMap), dataLocation(dataLocation),
      xAxisScaleDefined(false), yAxisScaleDefined(false), xAxisScale(make_pair(0, 0)),
      yAxisScale(make_pair(0, 0)), initXAxisScale(make_pair(0, 0)), initYAxisScale(make_pair(0, 0)),
      displayEdges(false), displaylabels(true), scale(true) {

  if (dataLocation == NODE) {
    glGraphComposite = new GlGraphComposite(graph);
    GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
    glGraphInputData->setElementLayout(scatterLayout);
    glGraphInputData->setElementSize(graph->getProperty<SizeProperty>("viewSize"));
  } else {
    glGraphComposite = new GlGraphComposite(edgeAsNodeGraph);
    GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
    glGraphInputData->setElementLayout(scatterEdgeLayout);
    glGraphInputData->setElementSize(edgeAsNodeGraph->getProperty<SizeProperty>("viewSize"));
  }

  setGraphView(glGraphComposite, (dataLocation == NODE) ? displayEdges : false, displaylabels,
               scale);
  backgroundRect = new GlRect(Coord(blCorner.getX(), blCorner.getY() + size),
                              Coord(blCorner.getX() + size, blCorner.getY()), backgroundColor,
                              backgroundColor, true, false);
  addGlEntity(backgroundRect, "background rect");
  clickLabel = new GlLabel(Coord(blCorner.getX() + size / 2.0f, blCorner.getY() + size / 2.0f),
                           Size(size, size / 4.0f), foregroundColor);
  clickLabel->setText("Double Click to generate overview");
  addGlEntity(clickLabel, "label");
  computeBoundingBox();
  overviewId = overviewCpt++;
  textureName = xDim + "_" + yDim + " " + getStringFromNumber(overviewId);
}

ScatterPlot2D::~ScatterPlot2D() {
  clean();
  delete glGraphComposite;
  delete scatterLayout;
  delete scatterEdgeLayout;
  GlTextureManager::getInst().deleteTexture(textureName);
}

void ScatterPlot2D::setBLCorner(const Coord &blCorner) {
  GlComposite::translate(blCorner - this->blCorner);
  this->blCorner = blCorner;
  computeBoundingBox();
}

void ScatterPlot2D::setDataLocation(const ElementType &dataLocation) {
  if (dataLocation != this->dataLocation) {
    delete glGraphComposite;
    xAxisScaleDefined = false;
    yAxisScaleDefined = false;

    if (dataLocation == NODE) {
      glGraphComposite = new GlGraphComposite(graph);
      GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
      glGraphInputData->setElementLayout(scatterLayout);
      glGraphInputData->setElementSize(graph->getProperty<SizeProperty>("viewSize"));
    } else {
      glGraphComposite = new GlGraphComposite(edgeAsNodeGraph);
      GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
      glGraphInputData->setElementLayout(scatterEdgeLayout);
      glGraphInputData->setElementSize(edgeAsNodeGraph->getProperty<SizeProperty>("viewSize"));
    }
  }

  this->dataLocation = dataLocation;
}

void ScatterPlot2D::generateOverview(GlMainWidget *glWidget, LayoutProperty *reverseLayout) {
  clean();
  clickLabel = nullptr;
  backgroundRect = nullptr;
  createAxis();
  glProgressBar =
      new GlProgressBar(Coord(blCorner.getX() + size / 2.0f, blCorner.getY() + size / 2.0f), size,
                        size, Color(0, 0, 255));
  glProgressBar->setComment("Generating overview ...");
  addGlEntity(glProgressBar, "progress bar");
  computeScatterPlotLayout(glWidget, reverseLayout);

  if (mapBackgroundColorToCoeff) {
    Color startColor = zeroColor, endColor;

    if (correlationCoeff < 0) {
      endColor = minusOneColor;
    } else {
      endColor = oneColor;
    }

    for (unsigned int i = 0; i < 4; ++i) {
      backgroundColor[i] =
          uchar((double(startColor[i]) +
                 (double(endColor[i]) - double(startColor[i])) * abs(correlationCoeff)));
    }

    int bgV = backgroundColor.getV();

    if (bgV < 128) {
      foregroundColor = Color(255, 255, 255);
    } else {
      foregroundColor = Color(0, 0, 0);
    }
  }

  GlOffscreenRenderer *glOffscreenRenderer = GlOffscreenRenderer::getInstance();
  glOffscreenRenderer->setViewPortSize(size, size);
  glOffscreenRenderer->clearScene();

  GlScene *scene = glOffscreenRenderer->getScene();

  if (mapBackgroundColorToCoeff) {
    GlLayer *backgroundLayer = scene->getLayer("Background");
    Gl2DRect *background = new Gl2DRect(1.0f, 0.0f, 0.0f, 1.0f, "gaussian_tex_back", true);
    backgroundLayer->addGlEntity(background, "background");
  }

  setGraphView(glGraphComposite, displayEdges, displaylabels, scale);

  glOffscreenRenderer->setSceneBackgroundColor(backgroundColor);
  glOffscreenRenderer->addGraphCompositeToScene(glGraphComposite);
  glOffscreenRenderer->addGlEntityToScene(xAxis);
  glOffscreenRenderer->addGlEntityToScene(yAxis);
  glOffscreenRenderer->renderScene(true);

  GLuint textureId = glOffscreenRenderer->getGLTexture(true);
  GlTextureManager::getInst().deleteTexture(textureName);
  GlTextureManager::getInst().registerExternalTexture(textureName, textureId);

  glOffscreenRenderer->clearScene();

  deleteGlEntity(glProgressBar);
  delete glProgressBar;
  Gl2DRect *rectTextured = new Gl2DRect(blCorner.getY() + size, blCorner.getY(), blCorner.getX(),
                                        blCorner.getX() + size, textureName);
  addGlEntity(rectTextured, textureName + " overview");
  computeBoundingBox();
  overviewGen = true;
}

void ScatterPlot2D::clean() {
  delete xAxis;
  xAxis = nullptr;
  delete yAxis;
  yAxis = nullptr;
  reset(true);
}

void ScatterPlot2D::createAxis() {
  assert(dynamic_cast<NumericProperty *>(graph->getProperty(xDim)));
  assert(dynamic_cast<NumericProperty *>(graph->getProperty(yDim)));
  NumericProperty *xProp = static_cast<NumericProperty *>(graph->getProperty(xDim));
  NumericProperty *yProp = static_cast<NumericProperty *>(graph->getProperty(yDim));
  xType = graph->getProperty(xDim)->getTypename();
  yType = graph->getProperty(yDim)->getTypename();

  double xMin, xMax, yMin, yMax;

  if (dataLocation == NODE) {
    xMin = xProp->getNodeDoubleMin(graph);
    xMax = xProp->getNodeDoubleMax(graph);
    yMin = yProp->getNodeDoubleMin(graph);
    yMax = yProp->getNodeDoubleMax(graph);
  } else {
    xMin = xProp->getEdgeDoubleMin(graph);
    xMax = xProp->getEdgeDoubleMax(graph);
    yMin = yProp->getEdgeDoubleMin(graph);
    yMax = yProp->getEdgeDoubleMax(graph);
  }

  initXAxisScale = make_pair(xMin, xMax);
  initYAxisScale = make_pair(yMin, yMax);

  if (xAxisScaleDefined) {
    if (xMin > xAxisScale.first)
      xMin = xAxisScale.first;

    if (xMax < xAxisScale.second)
      xMax = xAxisScale.second;
  }

  xAxisScale.first = xMin;
  xAxisScale.second = xMax;

  if (xMin == xMax)
    xMax++;

  if (yAxisScaleDefined) {
    if (yMin > yAxisScale.first)
      yMin = yAxisScale.first;

    if (yMax < yAxisScale.second)
      yMax = yAxisScale.second;
  }

  yAxisScale.first = yMin;
  yAxisScale.second = yMax;

  if (yMin == yMax)
    yMax++;

  xAxis = new GlQuantitativeAxis(xDim, Coord(0.0f, 0.0f, 0.0f), DEFAULT_AXIS_LENGTH,
                                 GlAxis::HORIZONTAL_AXIS, foregroundColor, true);

  if (xType == "double") {
    xAxis->setAxisParameters(xMin, xMax, DEFAULT_NB_GRADS, GlAxis::LEFT_OR_BELOW, true);
  } else {
    unsigned int step = uint((xMax - xMin) / 20);
    xAxis->setAxisParameters(int(xMin), int(xMax), step ? step : 1, GlAxis::LEFT_OR_BELOW, true);
  }

  xAxis->setAxisGraduationsMaxLabelWidth(300.0f);
  xAxis->addCaption(GlAxis::BELOW, 100.0f, false, 300.0f, 155.0f);
  xAxis->updateAxis();

  yAxis = new GlQuantitativeAxis(yDim, Coord(0.0f, 0.0f, 0.0f), DEFAULT_AXIS_LENGTH,
                                 GlAxis::VERTICAL_AXIS, foregroundColor, true);

  if (yType == "double") {
    yAxis->setAxisParameters(yMin, yMax, DEFAULT_NB_GRADS, GlAxis::LEFT_OR_BELOW, true);
  } else {
    unsigned int step = uint((yMax - yMin) / 20);
    yAxis->setAxisParameters(int(yMin), int(yMax), step ? step : 1, GlAxis::LEFT_OR_BELOW, true);
  }

  yAxis->addCaption(GlAxis::LEFT, 100.0f, false, 300.0f, 155.0f);
  yAxis->updateAxis();

  if (xAxis->getCaptionHeight() > yAxis->getCaptionHeight())
    xAxis->setCaptionHeight(yAxis->getCaptionHeight(), false);
  else
    yAxis->setCaptionHeight(xAxis->getCaptionHeight(), false);
}

void ScatterPlot2D::computeScatterPlotLayout(GlMainWidget *glWidget,
                                             LayoutProperty *reverseLayout) {
  Graph *_graph = glGraphComposite->getGraph();
  double sumxiyi = 0.0, sumxi = 0.0, sumyi = 0.0, sumxi2 = 0.0, sumyi2 = 0.0;
  unsigned int nbGraphNodes = _graph->numberOfNodes();

  node n;
  currentStep = 0;
  maxStep = nbGraphNodes;
  drawStep = maxStep / 20;

  if (!drawStep)
    drawStep = 1;

  assert(dynamic_cast<NumericProperty *>(graph->getProperty(xDim)));
  assert(dynamic_cast<NumericProperty *>(graph->getProperty(yDim)));
  NumericProperty *xProp = static_cast<NumericProperty *>(graph->getProperty(xDim));
  NumericProperty *yProp = static_cast<NumericProperty *>(graph->getProperty(yDim));

  forEach(n, _graph->getNodes()) {
    Coord nodeCoord;
    double xValue, yValue;

    if (dataLocation == NODE) {
      xValue = xProp->getNodeDoubleValue(n);
      yValue = yProp->getNodeDoubleValue(n);
    } else { // EDGE
      xValue = xProp->getEdgeDoubleValue(nodeToEdge[n]);
      yValue = yProp->getEdgeDoubleValue(nodeToEdge[n]);
    }

    sumxi += xValue;
    sumxi2 += (xValue * xValue);

    sumyi += yValue;
    sumyi2 += (yValue * yValue);
    sumxiyi += (xValue * yValue);

    if (reverseLayout == nullptr || dataLocation != NODE) {
      Coord xValueAxisCoord = xAxis->getAxisPointCoordForValue(xValue);
      Coord yValueAxisCoord = yAxis->getAxisPointCoordForValue(yValue);
      nodeCoord = Coord(xValueAxisCoord.getX(), yValueAxisCoord.getY(), 0.0f);
    } else {
      Coord nodeCoordReverse = reverseLayout->getNodeValue(n);
      nodeCoord = Coord(nodeCoordReverse.getY(), nodeCoordReverse.getX(), 0.0f);
    }

    if (dataLocation == NODE) {
      scatterLayout->setNodeValue(n, nodeCoord);
    } else {
      scatterEdgeLayout->setNodeValue(n, nodeCoord);
    }

    ++currentStep;

    if (glWidget != nullptr && currentStep % drawStep == 0) {
      glProgressBar->progress(currentStep, maxStep);
      glWidget->draw();
    }
  }

  double numerator = sumxiyi - (1. / nbGraphNodes) * sumxi * sumyi;
  double denominator = sqrt(sumxi2 - (1. / nbGraphNodes) * (sumxi * sumxi)) *
                       sqrt(sumyi2 - (1. / nbGraphNodes) * (sumyi * sumyi));

  if (denominator == 0) {
    correlationCoeff = 0;
  } else {
    correlationCoeff = numerator / denominator;
  }
}

Coord ScatterPlot2D::getOverviewCenter() const {
  return Coord(blCorner.getX() + size / 2.0f, blCorner.getY() + size / 2.0f, 0.0f);
}

void ScatterPlot2D::setUniformBackgroundColor(const Color &backgroundColor) {
  this->backgroundColor = backgroundColor;
  mapBackgroundColorToCoeff = false;

  if (backgroundRect != nullptr) {
    backgroundRect->setTopLeftColor(backgroundColor);
    backgroundRect->setBottomRightColor(backgroundColor);
  }
}

void ScatterPlot2D::mapBackgroundColorToCorrelCoeff(const bool mapBackgroundColor,
                                                    const Color &minusOneColor,
                                                    const Color &zeroColor, const Color &oneColor) {
  mapBackgroundColorToCoeff = mapBackgroundColor;
  this->minusOneColor = minusOneColor;
  this->zeroColor = zeroColor;
  this->oneColor = oneColor;
}

void ScatterPlot2D::setForegroundColor(const Color &foregroundColor) {
  this->foregroundColor = foregroundColor;

  if (clickLabel != nullptr) {
    clickLabel->setColor(foregroundColor);
  }
}
}
