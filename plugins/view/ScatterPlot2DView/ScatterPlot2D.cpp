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

static void setGraphView (GlGraphComposite *glGraph, bool displayEdges) {
  GlGraphRenderingParameters param = glGraph->getRenderingParameters ();
  param.setAntialiasing (true);
  param.setViewNodeLabel (true);
  param.setFontsType (2);
  param.setSelectedNodesStencil(1);
  param.setNodesStencil(0xFFFF);
  param.setNodesLabelStencil(0xFFFF);
  param.setDisplayEdges(displayEdges);
  param.setEdgesStencil(0xFFFF);
  param.setSelectedEdgesStencil(1);
  param.setDisplayNodes(true);
  param.setDisplayMetaNodes(true);
  glGraph->setRenderingParameters (param);
}

ScatterPlot2D::ScatterPlot2D(Graph *graph, const string& xDim, const string& yDim, const ElementType &dataLocation, Coord blCorner, unsigned int size, const Color &backgroundColor, const Color &foregroundColor)
  : xDim(xDim), yDim(yDim), blCorner(blCorner), size(size), graph(graph), scatterLayout(new LayoutProperty(graph)), xAxis(NULL), yAxis(NULL), overviewGen(false), backgroundColor(backgroundColor),
    foregroundColor(foregroundColor), mapBackgroundColorToCoeff(false), edgeAsNodeGraph(newGraph()), dataLocation(dataLocation), xAxisScaleDefined(false), yAxisScaleDefined(false),
    xAxisScale(make_pair(0,0)), yAxisScale(make_pair(0,0)), initXAxisScale(make_pair(0,0)), initYAxisScale(make_pair(0,0)), displayEdges(false) {
  edge e;
  ColorProperty* edgeAsNodeGraphColor = edgeAsNodeGraph->getProperty<ColorProperty>("viewColor");
  ColorProperty* graphColor = graph->getProperty<ColorProperty>("viewColor");
  BooleanProperty* edgeAsNodeGraphSelection = edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection");
  BooleanProperty* graphSelection = graph->getProperty<BooleanProperty>("viewSelection");
  StringProperty* edgeAsNodeGraphLabel = edgeAsNodeGraph->getProperty<StringProperty>("viewLabel");
  StringProperty* graphLabel = graph->getProperty<StringProperty>("viewLabel");
  forEach(e, graph->getEdges()) {
    node n = edgeToNode[e] = edgeAsNodeGraph->addNode();
    nodeToEdge[edgeToNode[e]] = e;
    edgeAsNodeGraphColor->setNodeValue(n, graphColor->getEdgeValue(e));
    edgeAsNodeGraphSelection->setNodeValue(n, graphSelection->getEdgeValue(e));
    edgeAsNodeGraphLabel->setNodeValue(n, graphLabel->getEdgeValue(e));
  }
  edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection")->addListener(this);
  edgeAsNodeGraph->getProperty<IntegerProperty>("viewShape")->setAllNodeValue(NodeShape::Circle);

  if (dataLocation == NODE) {
    glGraphComposite = new GlGraphComposite(graph);
    GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
    glGraphInputData->setElementLayout(scatterLayout);
    glGraphInputData->setElementSize(graph->getProperty<SizeProperty>("viewSize"));
  }
  else {
    glGraphComposite = new GlGraphComposite(edgeAsNodeGraph);
    GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
    glGraphInputData->setElementLayout(edgeAsNodeGraph->getProperty<LayoutProperty>("viewLayout"));
    glGraphInputData->setElementSize(edgeAsNodeGraph->getProperty<SizeProperty>("viewSize"));
  }

  setGraphView(glGraphComposite, (dataLocation == NODE) ? displayEdges : false);
  backgroundRect = new GlRect(Coord(blCorner.getX(), blCorner.getY() + size), Coord(blCorner.getX() + size, blCorner.getY()), backgroundColor, backgroundColor, true, false);
  addGlEntity(backgroundRect, "background rect");
  clickLabel = new GlLabel(Coord(blCorner.getX() + size / 2.0f, blCorner.getY() + size / 2.0f), Size(size, size / 4.0f), foregroundColor);
  clickLabel->setText("Double Click to generate overview");
  addGlEntity(clickLabel, "label");
  computeBoundingBox();
  overviewId = overviewCpt++;
  textureName = xDim + "_" + yDim + " " + getStringFromNumber(overviewId);

  graph->addListener(this);
  graph->getProperty(xDim)->addListener(this);
  graph->getProperty(yDim)->addListener(this);
  graph->getProperty("viewColor")->addListener(this);
  graph->getProperty("viewLabel")->addListener(this);
  graph->getProperty("viewSize")->addListener(this);
  graph->getProperty("viewShape")->addListener(this);
  graph->getProperty("viewSelection")->addListener(this);
  graph->getProperty("viewTexture")->addListener(this);
}

ScatterPlot2D::~ScatterPlot2D() {
  clean();
  delete glGraphComposite;
  delete scatterLayout;
  delete edgeAsNodeGraph;
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
    }
    else {
      glGraphComposite = new GlGraphComposite(edgeAsNodeGraph);
      GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
      glGraphInputData->setElementLayout(edgeAsNodeGraph->getProperty<LayoutProperty>("viewLayout"));
      glGraphInputData->setElementSize(edgeAsNodeGraph->getProperty<SizeProperty>("viewSize"));
    }
  }

  this->dataLocation = dataLocation;
}

void ScatterPlot2D::generateOverview(GlMainWidget *glWidget, LayoutProperty *reverseLayout) {
  clean();
  clickLabel = NULL;
  backgroundRect = NULL;
  createAxis();
  glProgressBar = new GlProgressBar(Coord(blCorner.getX() + size / 2.0f, blCorner.getY() + size / 2.0f), size, size, Color(0,0,255));
  glProgressBar->setComment("Generating overview ...");
  addGlEntity(glProgressBar, "progress bar");
  computeScatterPlotLayout(glWidget, reverseLayout);

  if (mapBackgroundColorToCoeff) {
    Color startColor = zeroColor, endColor;

    if (correlationCoeff < 0) {
      endColor = minusOneColor;
    }
    else {
      endColor = oneColor;
    }

    for (unsigned int i = 0; i < 4; ++i) {
      backgroundColor[i] = static_cast<unsigned char>((double(startColor[i]) + (double(endColor[i])- double(startColor[i])) * abs(correlationCoeff)));
    }

    int bgV = backgroundColor.getV();

    if (bgV < 128) {
      foregroundColor = Color(255,255,255);
    }
    else {
      foregroundColor = Color(0,0,0);
    }
  }

  GlOffscreenRenderer *glOffscreenRenderer = GlOffscreenRenderer::getInstance();
  glOffscreenRenderer->setViewPortSize(size, size);
  glOffscreenRenderer->clearScene();

  GlScene *scene = glOffscreenRenderer->getScene();

  if (mapBackgroundColorToCoeff) {
    GlLayer *backgroundLayer = scene->getLayer("Background");
    Gl2DRect *background = new Gl2DRect(1.0f, 0.0f, 0.0f, 1.0f,"gaussian_tex_back", true);
    backgroundLayer->addGlEntity(background,"background");
  }

  setGraphView(glGraphComposite, displayEdges);

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
  Gl2DRect *rectTextured = new Gl2DRect(blCorner.getY()+size, blCorner.getY(), blCorner.getX(), blCorner.getX() + size, textureName);
  addGlEntity(rectTextured, textureName + " overview");
  computeBoundingBox();
  overviewGen = true;
}

void ScatterPlot2D::clean() {
  delete xAxis;
  xAxis = NULL;
  delete yAxis;
  yAxis = NULL;
  reset(true);
}

void ScatterPlot2D::createAxis() {
  assert(dynamic_cast<NumericProperty *>(graph->getProperty(xDim)));
  assert(dynamic_cast<NumericProperty *>(graph->getProperty(yDim)));
  NumericProperty* xProp = (NumericProperty *) graph->getProperty(xDim);
  NumericProperty* yProp = (NumericProperty *) graph->getProperty(yDim);
  xType = graph->getProperty(xDim)->getTypename();
  yType = graph->getProperty(yDim)->getTypename();

  double xMin, xMax, yMin, yMax;

  if(dataLocation == NODE) {
    xMin = xProp->getNodeDoubleMin(graph);
    xMax = xProp->getNodeDoubleMax(graph);
    yMin = yProp->getNodeDoubleMin(graph);
    yMax = yProp->getNodeDoubleMax(graph);
  }
  else {
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

  xAxis = new GlQuantitativeAxis(xDim, Coord(0.0f, 0.0f, 0.0f), DEFAULT_AXIS_LENGTH, GlAxis::HORIZONTAL_AXIS, foregroundColor, true);

  if (xType == "double") {
    xAxis->setAxisParameters(xMin ,xMax, DEFAULT_NB_GRADS, GlAxis::LEFT_OR_BELOW, true);
  }
  else {
    unsigned int step = static_cast<unsigned int>((xMax - xMin) / 20);
    xAxis->setAxisParameters(static_cast<int>(xMin), static_cast<int>(xMax),
                             step ? step : 1, GlAxis::LEFT_OR_BELOW, true);
  }

  xAxis->setAxisGraduationsMaxLabelWidth(300.0f);
  xAxis->addCaption(GlAxis::BELOW, 100.0f, false, 300.0f, 155.0f);
  xAxis->updateAxis();

  yAxis = new GlQuantitativeAxis(yDim, Coord(0.0f, 0.0f, 0.0f), DEFAULT_AXIS_LENGTH, GlAxis::VERTICAL_AXIS, foregroundColor, true);

  if (yType == "double") {
    yAxis->setAxisParameters(yMin ,yMax, DEFAULT_NB_GRADS, GlAxis::LEFT_OR_BELOW, true);
  }
  else {
    unsigned int step = static_cast<unsigned int>((yMax - yMin) / 20);
    yAxis->setAxisParameters(static_cast<int>(yMin) ,static_cast<int>(yMax),
                             step ? step : 1, GlAxis::LEFT_OR_BELOW, true);
  }

  yAxis->addCaption(GlAxis::LEFT, 100.0f, false, 300.0f, 155.0f);
  yAxis->updateAxis();

  if(xAxis->getCaptionHeight()>yAxis->getCaptionHeight())
    xAxis->setCaptionHeight(yAxis->getCaptionHeight(),false);
  else
    yAxis->setCaptionHeight(xAxis->getCaptionHeight(),false);

}

void ScatterPlot2D::computeScatterPlotLayout(GlMainWidget *glWidget, LayoutProperty *reverseLayout) {
  Graph *_graph = glGraphComposite->getGraph();
  double sumxiyi = 0.0, sumxi = 0.0, sumyi = 0.0, sumxi2 = 0.0, sumyi2 = 0.0;
  unsigned int nbGraphNodes = _graph->numberOfNodes();

  LayoutProperty *edgeAsNodeGraphLayout = edgeAsNodeGraph->getProperty<LayoutProperty>("viewLayout");

  node n;
  currentStep = 0;
  maxStep = nbGraphNodes;
  drawStep = maxStep / 20;

  if(!drawStep)
    drawStep=1;

  assert(dynamic_cast<NumericProperty *>(graph->getProperty(xDim)));
  assert(dynamic_cast<NumericProperty *>(graph->getProperty(yDim)));
  NumericProperty* xProp = (NumericProperty *) graph->getProperty(xDim);
  NumericProperty* yProp = (NumericProperty *) graph->getProperty(yDim);

  forEach(n, _graph->getNodes()) {
    Coord nodeCoord;
    double xValue, yValue;

    if(dataLocation == NODE) {
      xValue = xProp->getNodeDoubleValue(n);
      yValue = yProp->getNodeDoubleValue(n);
    }
    else {   //EDGE
      xValue = xProp->getEdgeDoubleValue(nodeToEdge[n]);
      yValue = yProp->getEdgeDoubleValue(nodeToEdge[n]);
    }

    sumxi += xValue;
    sumxi2 += (xValue * xValue);

    sumyi += yValue;
    sumyi2 += (yValue * yValue);
    sumxiyi += (xValue * yValue);

    if (reverseLayout == NULL || dataLocation != NODE) {
      Coord xValueAxisCoord = xAxis->getAxisPointCoordForValue(xValue);
      Coord yValueAxisCoord = yAxis->getAxisPointCoordForValue(yValue);
      nodeCoord = Coord(xValueAxisCoord.getX(), yValueAxisCoord.getY(), 0.0f);
    }
    else {
      Coord nodeCoordReverse = reverseLayout->getNodeValue(n);
      nodeCoord = Coord(nodeCoordReverse.getY(), nodeCoordReverse.getX(), 0.0f);
    }

    if (dataLocation == NODE) {
      scatterLayout->setNodeValue(n, nodeCoord);
    }
    else {
      edgeAsNodeGraphLayout->setNodeValue(n, nodeCoord);
    }

    ++currentStep;

    if (glWidget != NULL && currentStep % drawStep == 0) {
      glProgressBar->progress(currentStep, maxStep);
      glWidget->draw();
    }
  }

  double numerator = sumxiyi - (1./nbGraphNodes) * sumxi * sumyi;
  double denominator = sqrt(sumxi2 - (1./nbGraphNodes) * (sumxi * sumxi)) * sqrt(sumyi2 - (1./nbGraphNodes) * (sumyi * sumyi));

  if (denominator == 0) {
    correlationCoeff = 0;
  }
  else {
    correlationCoeff = numerator / denominator;
  }
}

Coord ScatterPlot2D::getOverviewCenter() const {
  return Coord(blCorner.getX() + size / 2.0f, blCorner.getY() + size / 2.0f, 0.0f);
}

void ScatterPlot2D::setUniformBackgroundColor(const Color &backgroundColor) {
  this->backgroundColor = backgroundColor;
  mapBackgroundColorToCoeff = false;

  if (backgroundRect != NULL) {
    backgroundRect->setTopLeftColor(backgroundColor);
    backgroundRect->setBottomRightColor(backgroundColor);
  }
}

void ScatterPlot2D::mapBackgroundColorToCorrelCoeff(const bool mapBackgroundColor, const Color &minusOneColor, const Color &zeroColor, const Color &oneColor) {
  mapBackgroundColorToCoeff = mapBackgroundColor;
  this->minusOneColor = minusOneColor;
  this->zeroColor = zeroColor;
  this->oneColor = oneColor;
}

void ScatterPlot2D::setForegroundColor(const Color &foregroundColor) {
  this->foregroundColor = foregroundColor;

  if (clickLabel != NULL) {
    clickLabel->setColor(foregroundColor);
  }
}

void ScatterPlot2D::treatEvent(const Event &message) {
  if (typeid(message) == typeid(GraphEvent)) {
    const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&message);

    if(graphEvent) {
      if(graphEvent->getType()==GraphEvent::TLP_ADD_NODE)
        addNode(graphEvent->getGraph(),graphEvent->getNode());

      if(graphEvent->getType()==GraphEvent::TLP_ADD_EDGE)
        addEdge(graphEvent->getGraph(),graphEvent->getEdge());

      if(graphEvent->getType()==GraphEvent::TLP_DEL_NODE)
        delNode(graphEvent->getGraph(),graphEvent->getNode());

      if(graphEvent->getType()==GraphEvent::TLP_DEL_EDGE)
        delEdge(graphEvent->getGraph(),graphEvent->getEdge());
    }
  }

  if(typeid(message) == typeid(PropertyEvent)) {
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&message);

    if(propertyEvent) {
      if(propertyEvent->getType()==PropertyEvent::TLP_AFTER_SET_NODE_VALUE)
        afterSetNodeValue(propertyEvent->getProperty(),propertyEvent->getNode());

      if(propertyEvent->getType()==PropertyEvent::TLP_AFTER_SET_EDGE_VALUE)
        afterSetEdgeValue(propertyEvent->getProperty(),propertyEvent->getEdge());

      if(propertyEvent->getType()==PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE)
        afterSetAllNodeValue(propertyEvent->getProperty());

      if(propertyEvent->getType()==PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE)
        afterSetAllEdgeValue(propertyEvent->getProperty());

    }
  }
}

void ScatterPlot2D::afterSetNodeValue(PropertyInterface *p, const node n) {
  if (p->getGraph() == edgeAsNodeGraph && p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection = static_cast<BooleanProperty*>(p);
    BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");
    viewSelection->removeListener(this);
    viewSelection->setEdgeValue(nodeToEdge[n], edgeAsNodeGraphSelection->getNodeValue(n));
    viewSelection->addListener(this);
    return;
  }

  //afterSetAllNodeValue(p);
}

void ScatterPlot2D::afterSetEdgeValue(PropertyInterface *p, const edge e) {
  if (edgeToNode.find(e) == edgeToNode.end())
    return;

  if (p->getName() == "viewColor") {
    ColorProperty *edgeAsNodeGraphColors = edgeAsNodeGraph->getProperty<ColorProperty>("viewColor");
    ColorProperty *viewColor = static_cast<ColorProperty*>(p);
    edgeAsNodeGraphColors->setNodeValue(edgeToNode[e], viewColor->getEdgeValue(e));
  }
  else if (p->getName() == "viewLabel") {
    StringProperty *edgeAsNodeGraphLabels = edgeAsNodeGraph->getProperty<StringProperty>("viewLabel");
    StringProperty *viewLabel = static_cast<StringProperty*>(p);
    edgeAsNodeGraphLabels->setNodeValue(edgeToNode[e], viewLabel->getEdgeValue(e));
  }
  else if (p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection = edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection");
    BooleanProperty *viewSelection = static_cast<BooleanProperty*>(p);
    edgeAsNodeGraphSelection->removeListener(this);

    if (edgeAsNodeGraphSelection->getNodeValue(edgeToNode[e]) != viewSelection->getEdgeValue(e))
      edgeAsNodeGraphSelection->setNodeValue(edgeToNode[e], viewSelection->getEdgeValue(e));

    edgeAsNodeGraphSelection->addListener(this);
  }
}

void ScatterPlot2D::afterSetAllNodeValue(PropertyInterface *p) {
  if (p->getName() == "viewSelection") {
    if (p->getGraph() == edgeAsNodeGraph) {
      BooleanProperty *edgeAsNodeGraphSelection = static_cast<BooleanProperty*>(p);
      BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");
      viewSelection->setAllEdgeValue(edgeAsNodeGraphSelection->getNodeValue(edgeAsNodeGraph->getOneNode()));
    }
  }
}

void ScatterPlot2D::afterSetAllEdgeValue(PropertyInterface *p) {

  if (p->getName() == "viewColor") {
    ColorProperty *edgeAsNodeGraphColors = edgeAsNodeGraph->getProperty<ColorProperty>("viewColor");
    ColorProperty *viewColor = static_cast<ColorProperty*>(p);
    edgeAsNodeGraphColors->setAllNodeValue(viewColor->getEdgeValue(graph->getOneEdge()));
  }
  else if (p->getName() == "viewLabel") {
    StringProperty *edgeAsNodeGraphLabels = edgeAsNodeGraph->getProperty<StringProperty>("viewLabel");
    StringProperty *viewLabel = static_cast<StringProperty*>(p);
    edgeAsNodeGraphLabels->setAllNodeValue(viewLabel->getEdgeValue(graph->getOneEdge()));
  }
  else if (p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection = edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection");
    BooleanProperty *viewSelection = static_cast<BooleanProperty*>(p);
    edge e;
    forEach(e, graph->getEdges()) {
      if (edgeAsNodeGraphSelection->getNodeValue(edgeToNode[e]) != viewSelection->getEdgeValue(e)) {
        edgeAsNodeGraphSelection->setNodeValue(edgeToNode[e], viewSelection->getEdgeValue(e));
      }
    }
  }
}

void ScatterPlot2D::addNode(Graph *, const node ) {
}

void ScatterPlot2D::addEdge(Graph *, const edge e) {
  edgeToNode[e] = edgeAsNodeGraph->addNode();
}

void ScatterPlot2D::delNode(Graph *,const node ) {
}

void ScatterPlot2D::delEdge(Graph *,const edge e) {
  edgeAsNodeGraph->delNode(edgeToNode[e]);
  edgeToNode.erase(e);
}

unsigned int ScatterPlot2D::getMappedId(unsigned int id) {
  if (dataLocation == EDGE)
    return nodeToEdge[node(id)].id;

  return id;
}

}
