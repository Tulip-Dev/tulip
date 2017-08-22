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

#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/GlPolyQuad.h>
#include <tulip/Gl2DRect.h>
#include <tulip/ForEach.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlTextureManager.h>
#include <tulip/TlpTools.h>
#include <tulip/TulipViewSettings.h>

#include "Histogram.h"

using namespace std;

const string BIN_RECT_TEXTURE = tlp::TulipBitmapDir + "histo_texture.png";

const float DEFAULT_AXIS_LENGTH = 1000.;

const std::string XAXIS_ID = "x axis";
const std::string YAXIS_ID = "y axis";


template <typename T>
std::string getStringFromNumber(T number, unsigned int precision = 5) {
  std::ostringstream oss;
  oss.precision(precision);
  oss << number;
  return oss.str();
}

namespace tlp {

static void setGraphView (GlGraphComposite *glGraph, bool displayEdges) {
  GlGraphRenderingParameters param = glGraph->getRenderingParameters();
  param.setAntialiasing (true);
  param.setViewNodeLabel (true);
  param.setFontsType (2);
  param.setSelectedNodesStencil(1);
  param.setNodesStencil(1);
  param.setNodesLabelStencil(1);
  param.setDisplayEdges(displayEdges);
  param.setEdgesStencil(2);
  param.setSelectedEdgesStencil(1);
  param.setDisplayNodes(true);
  param.setDisplayMetaNodes(true);
  glGraph->setRenderingParameters (param);
}

int Histogram::overviewCpt(0);

Histogram::Histogram(Graph *graph, Graph* edgeGraph,
                     std::map<edge, node>& edgeMap,
                     const std::string& propertyName, const ElementType &dataLocation, const Coord &blCorner, unsigned int size,
                     const Color &backgroundColor, const Color &textColor) :
  graph(graph), propertyName(propertyName), blCorner(blCorner), size(size), nbHistogramBins(100), xAxis(NULL), yAxis(NULL),
  xAxisLogScale(false), yAxisLogScale(false), nbXGraduations(15), yAxisIncrementStep(0), histogramLayout(new LayoutProperty(graph)), histogramEdgeLayout(new LayoutProperty(graph)), histogramSize(new SizeProperty(graph)), histoBinsComposite(new GlComposite()), uniformQuantification(false),
  cumulativeFreqHisto(false), lastCumulHisto(false), edgeAsNodeGraph(edgeGraph), edgeToNode(edgeMap), backgroundColor(backgroundColor), textColor(textColor), integerScale(false),
  dataLocation(dataLocation), displayEdges(false), layoutUpdateNeeded(true),sizesUpdateNeeded(true), textureUpdateNeeded(true), xAxisScaleDefined(false), yAxisScaleDefined(false),
  xAxisScale(make_pair(0,0)), yAxisScale(make_pair(0,0)), initXAxisScale(make_pair(0,0)), initYAxisScale(make_pair(0,0)) {

  if (dataLocation == NODE) {
    graphComposite = new GlGraphComposite(graph);
    GlGraphInputData *glGraphInputData = graphComposite->getInputData();
    glGraphInputData->setElementLayout(histogramLayout);
    glGraphInputData->setElementSize(histogramSize);
  }
  else {
    graphComposite = new GlGraphComposite(edgeAsNodeGraph);
    GlGraphInputData *glGraphInputData = graphComposite->getInputData();
    glGraphInputData->setElementLayout(histogramEdgeLayout);
  }

  setGraphView(graphComposite, (dataLocation == NODE) ? displayEdges : false);
  overviewId = overviewCpt++;
  textureName = propertyName + " histo texture " + getStringFromNumber(overviewId);
  update();
}

Histogram::~Histogram() {
  GlTextureManager::getInst().deleteTexture(textureName);
  delete histogramLayout;
  delete histogramEdgeLayout;
  delete histogramSize;
  delete histoBinsComposite;
  delete graphComposite;
}

void Histogram::setDataLocation(const ElementType &dataLocation) {
  if (dataLocation != this->dataLocation) {
    delete graphComposite;
    xAxisScaleDefined = false;
    yAxisScaleDefined = false;

    if (dataLocation == NODE) {
      graphComposite = new GlGraphComposite(graph);
      GlGraphInputData *glGraphInputData = graphComposite->getInputData();
      glGraphInputData->setElementLayout(histogramLayout);
      glGraphInputData->setElementSize(histogramSize);
    }
    else {
      graphComposite = new GlGraphComposite(edgeAsNodeGraph);
      GlGraphInputData *glGraphInputData = graphComposite->getInputData();
      glGraphInputData->setElementLayout(histogramEdgeLayout);
    }
  }

  this->dataLocation = dataLocation;
}

static unsigned int clamp(unsigned int ui, unsigned int minVal, unsigned int maxVal) {
  return min(max(ui, minVal), maxVal);
}

void Histogram::computeHistogram() {

  histogramBins.clear();
  binMinMaxMap.clear();

  histogramBins.resize(nbHistogramBins);

  if (graph->getProperty(propertyName)->getTypename() == "double") {
    if (dataLocation == NODE) {
      min = graph->getProperty<DoubleProperty>(propertyName)->getNodeMin(graph);
      max = graph->getProperty<DoubleProperty>(propertyName)->getNodeMax(graph);
    }
    else {
      min = graph->getProperty<DoubleProperty>(propertyName)->getEdgeMin(graph);
      max = graph->getProperty<DoubleProperty>(propertyName)->getEdgeMax(graph);
    }
  }
  else {
    if (dataLocation == NODE) {
      min = graph->getProperty<IntegerProperty>(propertyName)->getNodeMin(graph);
      max = graph->getProperty<IntegerProperty>(propertyName)->getNodeMax(graph);
    }
    else {
      min = graph->getProperty<IntegerProperty>(propertyName)->getEdgeMin(graph);
      max = graph->getProperty<IntegerProperty>(propertyName)->getEdgeMax(graph);
    }
  }

  initXAxisScale = make_pair(min, max);

  if (xAxisScaleDefined) {
    if (min > xAxisScale.first)
      min = xAxisScale.first;

    if (max < xAxisScale.second)
      max = xAxisScale.second;
  }

  xAxisScale.first = min;
  xAxisScale.second = max;

  if (min == max)
    max += 1;

  binWidth = (max - min) / nbHistogramBins;

  maxBinSize = 0;

  integerScale = true;

  if (uniformQuantification) {
    DoubleProperty *propertyCopy = new DoubleProperty(graph);

    if (graph->getProperty(propertyName)->getTypename() == "double") {
      *propertyCopy = *(graph->getProperty<DoubleProperty>(propertyName));
    }
    else {
      if (dataLocation ==  NODE) {
        Iterator<node> *nodesIt = graph->getNodes();

        while (nodesIt->hasNext()) {
          node n = nodesIt->next();
          propertyCopy->setNodeValue(n, graph->getProperty<IntegerProperty>(propertyName)->getNodeValue(n));
        }

        delete nodesIt;
      }
      else {
        Iterator<edge> *edgesIt = graph->getEdges();

        while (edgesIt->hasNext()) {
          edge e = edgesIt->next();
          propertyCopy->setEdgeValue(e, graph->getProperty<IntegerProperty>(propertyName)->getEdgeValue(e));
        }

        delete edgesIt;
      }
    }

    propertyCopy->uniformQuantification(nbHistogramBins);

    for (unsigned int i = 0 ; i < nbHistogramBins ; ++i) {
      binMinMaxMap[i].first = DBL_MAX;
      binMinMaxMap[i].second = 0;
    }

    if (dataLocation == NODE) {
      Iterator<node> *nodesIt = graph->getNodes();

      while (nodesIt->hasNext()) {
        node n = nodesIt->next();
        unsigned int binId = uint(propertyCopy->getNodeValue(n));
        histogramBins[binId].push_back(n.id);

        if (histogramBins[binId].size() > maxBinSize) {
          maxBinSize = histogramBins[binId].size();
        }

        double val;

        if (graph->getProperty(propertyName)->getTypename() == "double") {
          val = graph->getProperty<DoubleProperty>(propertyName)->getNodeValue(n);
        }
        else {
          val = graph->getProperty<IntegerProperty>(propertyName)->getNodeValue(n);
        }

        if (val < binMinMaxMap[binId].first) {
          binMinMaxMap[binId].first = val;
        }

        if (val > binMinMaxMap[binId].second) {
          binMinMaxMap[binId].second = val;
        }
      }

      delete nodesIt;

    }
    else {
      Iterator<edge> *edgesIt = graph->getEdges();

      while (edgesIt->hasNext()) {
        edge e = edgesIt->next();
        unsigned int binId = uint(propertyCopy->getEdgeValue(e));
        histogramBins[binId].push_back(e.id);

        if (histogramBins[binId].size() > maxBinSize) {
          maxBinSize = histogramBins[binId].size();
        }

        double val;

        if (graph->getProperty(propertyName)->getTypename() == "double") {
          val = graph->getProperty<DoubleProperty>(propertyName)->getEdgeValue(e);
        }
        else {
          val = graph->getProperty<IntegerProperty>(propertyName)->getEdgeValue(e);
        }

        if (val < binMinMaxMap[binId].first) {
          binMinMaxMap[binId].first = val;
        }

        if (val > binMinMaxMap[binId].second) {
          binMinMaxMap[binId].second = val;
        }
      }

      delete edgesIt;
    }

    uniformQuantificationAxisLabels.clear();

    for (unsigned int i = 0 ; i < nbHistogramBins ; ++i) {
      if (histogramBins[i].size() > 0) {
        uniformQuantificationAxisLabels.push_back(getStringFromNumber(binMinMaxMap[i].first));
      }
      else {
        if (histogramBins[i-1].size() > 0) {
          uniformQuantificationAxisLabels.push_back(getStringFromNumber(binMinMaxMap[i-1].second));
        }
        else {
          uniformQuantificationAxisLabels.push_back(" ");
        }
      }
    }

    uniformQuantificationAxisLabels.push_back(getStringFromNumber(binMinMaxMap[nbHistogramBins - 1].second));
    delete propertyCopy;

  }
  else {
    if (dataLocation == NODE) {
      Iterator<node> *nodesIt = graph->getNodes();

      while (nodesIt->hasNext()) {
        node n = nodesIt->next();
        double value;

        if (graph->getProperty(propertyName)->getTypename() == "double") {
          value = graph->getProperty<DoubleProperty>(propertyName)->getNodeValue(n);
          double intpart, fracpart;
          fracpart = modf(value, &intpart);
          integerScale = integerScale && (fracpart == 0);
        }
        else {
          value = graph->getProperty<IntegerProperty>(propertyName)->getNodeValue(n);
        }

        if (value != max) {
          unsigned int nodeHistoBin = clamp(uint(floor((value - min) / binWidth)), 0, nbHistogramBins - 1);
          histogramBins[nodeHistoBin].push_back(n.id);

          if (histogramBins[nodeHistoBin].size() > maxBinSize) {
            maxBinSize = histogramBins[nodeHistoBin].size();
          }
        }
        else {
          histogramBins[nbHistogramBins - 1].push_back(n.id);

          if (histogramBins[nbHistogramBins - 1].size() > maxBinSize) {
            maxBinSize = histogramBins[nbHistogramBins - 1].size();
          }
        }

      }

      delete nodesIt;
    }
    else {
      Iterator<edge> *edgesIt = graph->getEdges();

      while (edgesIt->hasNext()) {
        edge e = edgesIt->next();
        double value;

        if (graph->getProperty(propertyName)->getTypename() == "double") {
          value = graph->getProperty<DoubleProperty>(propertyName)->getEdgeValue(e);
          double intpart, fracpart;
          fracpart = modf(value, &intpart);
          integerScale = integerScale && (fracpart == 0);
        }
        else {
          value = graph->getProperty<IntegerProperty>(propertyName)->getEdgeValue(e);
        }

        if (value != max) {
          unsigned int nodeHistoBin = uint(floor((value - min) / binWidth));
          histogramBins[nodeHistoBin].push_back(e.id);

          if (histogramBins[nodeHistoBin].size() > maxBinSize) {
            maxBinSize = histogramBins[nodeHistoBin].size();
          }
        }
        else {
          histogramBins[nbHistogramBins - 1].push_back(e.id);

          if (histogramBins[nbHistogramBins - 1].size() > maxBinSize) {
            maxBinSize = histogramBins[nbHistogramBins - 1].size();
          }
        }
      }

      delete edgesIt;
    }
  }
}



void Histogram::createAxis() {

  if (xAxis != NULL && yAxis != NULL) {
    delete xAxis;
    delete yAxis;
    xAxis = NULL;
    yAxis = NULL;
  }

  const float axisLength = DEFAULT_AXIS_LENGTH;

  unsigned int maxAxisValue, minAxisValue = 0;

  if (cumulativeFreqHisto) {
    if (dataLocation == NODE) {
      maxAxisValue = graph->numberOfNodes();
    }
    else {
      maxAxisValue = graph->numberOfEdges();
    }
  }
  else {
    maxAxisValue = maxBinSize;
  }

  initYAxisScale = make_pair(minAxisValue, maxAxisValue);

  if (yAxisScaleDefined) {
    if (yAxisScale.first < double(minAxisValue))
      minAxisValue = uint(yAxisScale.first);

    if (yAxisScale.second > double(maxAxisValue))
      maxAxisValue = uint(yAxisScale.second);
  }

  yAxisScale.first = minAxisValue;
  yAxisScale.second = maxAxisValue;

  yAxisIncrementStep = maxAxisValue / 10;

  if (yAxisIncrementStep < 1) yAxisIncrementStep = 1;

  if (lastCumulHisto != cumulativeFreqHisto) {
    unsigned int n;

    if (!lastCumulHisto) {
      n = maxBinSize;
    }
    else {
      if (dataLocation == NODE) {
        n = graph->numberOfNodes();
      }
      else {
        n = graph->numberOfEdges();
      }
    }

    yAxisIncrementStep = (yAxisIncrementStep * maxAxisValue) / n;

    if (lastCumulHisto) ++yAxisIncrementStep;
  }

  yAxis = new GlQuantitativeAxis((dataLocation == NODE ? "number of nodes" : "number of edges"), Coord(0,0,0), axisLength, GlAxis::VERTICAL_AXIS, textColor);
  yAxis->setAxisParameters(int(minAxisValue) , int(maxAxisValue), yAxisIncrementStep, GlAxis::LEFT_OR_BELOW, true);
  yAxis->setLogScale(yAxisLogScale);
  float yAxisGradsWidth = axisLength / 20;
  yAxis->setAxisGradsWidth(yAxisGradsWidth);
  yAxis->updateAxis();
  yAxis->addCaption(GlAxis::BELOW, axisLength / 10, false, (axisLength / 2.), 0, " ");


  xAxis = new GlQuantitativeAxis(propertyName, Coord(0,0,0), axisLength, GlAxis::HORIZONTAL_AXIS, textColor, true);
  float xAxisGradWidth = axisLength / 20;

  if (uniformQuantification) {
    xAxisGradWidth /= 2.;
  }

  xAxis->setAxisGradsWidth(xAxisGradWidth);

  if (uniformQuantification) {
    xAxis->setAxisGraduations(uniformQuantificationAxisLabels, GlAxis::LEFT_OR_BELOW);
  }
  else {
    if (integerScale) {
      long long axisMin = static_cast<long long>(min);
      long long axisMax = static_cast<long long>(max);

      // check the [min, max] double values range
      // fit in a long long range
      if (axisMax != LLONG_MIN) {
        long long incrementStep = static_cast<long long>((max - min) / nbXGraduations);

        if (incrementStep <1) incrementStep = 1;

        xAxis->setAxisParameters(axisMin, axisMax, incrementStep, GlAxis::LEFT_OR_BELOW, true);
      }
      else
        xAxis->setAxisParameters(min ,max, nbXGraduations, GlAxis::LEFT_OR_BELOW, true);
    }
    else {
      xAxis->setAxisParameters(min ,max, nbXGraduations, GlAxis::LEFT_OR_BELOW, true);
    }
  }

  xAxis->setLogScale(xAxisLogScale);
  xAxis->addCaption(GlAxis::BELOW, 100, false, 300, 100, " ");
  xAxis->setAxisGraduationsMaxLabelWidth(300);
  xAxis->updateAxis();

  refSizeX = axisLength / nbHistogramBins;

  if (cumulativeFreqHisto) {
    if (dataLocation == NODE) {
      refSizeY = yAxis->getAxisPointCoordForValue(graph->numberOfNodes()).getY() / graph->numberOfNodes();
    }
    else {
      refSizeY = yAxis->getAxisPointCoordForValue(graph->numberOfEdges()).getY() / graph->numberOfEdges();
    }
  }
  else {
    refSizeY = yAxis->getAxisPointCoordForValue(maxBinSize).getY() / maxBinSize;
  }

  refSize = std::min(refSizeX, refSizeY);

  edgeAsNodeGraph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(refSize, refSize, 0));
}

void Histogram::updateLayout() {
  computeHistogram();
  createAxis();

  unsigned int cumulativeSize = 0;

  for (unsigned int i = 0 ; i < nbHistogramBins ; ++i) {
    unsigned int binSize = histogramBins[i].size();
    cumulativeSize += binSize;
    float binXCoord, binXCoordEnd;

    if (!uniformQuantification) {
      binXCoord = xAxis->getAxisPointCoordForValue(min + i * binWidth).getX();
      binXCoordEnd = xAxis->getAxisPointCoordForValue(min + (i+1) * binWidth).getX();
    }
    else {
      binXCoord = i * refSizeX;
      binXCoordEnd = (i+1) * refSizeX;
    }

    float nodeXCoord = (binXCoord + binXCoordEnd) / 2.f;

    for (unsigned int j = 0 ; j < binSize ; ++j) {
      float nodeYCoord;

      if (!cumulativeFreqHisto) {
        nodeYCoord = (yAxis->getAxisPointCoordForValue(j).getY() + yAxis->getAxisPointCoordForValue(j+1).getY()) / 2.f;

      }
      else {
        nodeYCoord = (yAxis->getAxisPointCoordForValue(cumulativeSize - j - 1).getY() + yAxis->getAxisPointCoordForValue(cumulativeSize - j).getY()) / 2.;
      }

      Coord nodeCoord(nodeXCoord, nodeYCoord, 0);

      if (dataLocation == NODE) {
        histogramLayout->setNodeValue(node(histogramBins[i][j]), nodeCoord);
      }
      else {
        node n = edgeToNode[edge(histogramBins[i][j])];
        histogramEdgeLayout->setNodeValue(n, nodeCoord);

        vector<Coord> edgeHistoPointCoord;
        edgeHistoPointCoord.push_back(nodeCoord);
        histogramLayout->setEdgeValue(edge(histogramBins[i][j]), edgeHistoPointCoord);
      }
    }
  }

  layoutUpdateNeeded = false;
}

void Histogram::updateSizes() {
  SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");

  Size resizeFactor;
  const Size &eltMinSize(viewSize->getMin());
  const Size &eltMaxSize(viewSize->getMax());
  float minSize = (refSize / 10.);
  Size deltaSize(eltMaxSize - eltMinSize);

  for (unsigned int i = 0 ; i < 2 ; ++i) {
    if (deltaSize[i] != 0) {
      resizeFactor[i] = (refSize - minSize)/ deltaSize[i];
    }
    else {
      resizeFactor[i] = 0;
    }
  }

  bool resize = true;

  if (eltMinSize[0] == eltMaxSize[0] && eltMinSize[1] == eltMaxSize[1]) {
    resize = false;
  }

  for (unsigned int i = 0 ; i < nbHistogramBins ; ++i) {

    unsigned int binSize = histogramBins[i].size();

    float binXCoord, binXCoordEnd;

    if (!uniformQuantification) {
      binXCoord = xAxis->getAxisPointCoordForValue(min + i * binWidth).getX();
      binXCoordEnd = xAxis->getAxisPointCoordForValue(min + (i+1) * binWidth).getX();
    }
    else {
      binXCoord = i * refSizeX;
      binXCoordEnd = (i+1) * refSizeX;
    }

    for (unsigned int j = 0 ; j < binSize ; ++j) {
      if (dataLocation == NODE) {
        const Size &currentNodeSize = viewSize->getNodeValue(node(histogramBins[i][j]));
        Size newNodeSize;

        if (resize) {
          newNodeSize[0] = minSize + resizeFactor[0] * (currentNodeSize[0] - eltMinSize[0]);
          newNodeSize[1] = minSize + resizeFactor[1] * (currentNodeSize[1] - eltMinSize[1]);
        }
        else {
          newNodeSize[0] = refSize;
          newNodeSize[1] = refSize;
        }

        if (newNodeSize[0] > (binXCoordEnd - binXCoord)) {
          newNodeSize[0] = (binXCoordEnd - binXCoord);
        }

        newNodeSize[2] = currentNodeSize[2];
        histogramSize->setNodeValue(node(histogramBins[i][j]), newNodeSize);
      }
    }
  }

  sizesUpdateNeeded = false;
}

void Histogram::update() {

  if (!textureUpdateNeeded)
    return;

  if (layoutUpdateNeeded)
    updateLayout();

  if (sizesUpdateNeeded)
    updateSizes();

  ostringstream oss;
  edgeAsNodeGraph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(refSize,refSize,refSize));
  ColorProperty *viewColor = graph->getProperty<ColorProperty>("viewColor");

  histoBinsComposite->reset(true);

  unsigned int cumulativeSize = 0;
  unsigned int rectCpt = 0;
  GlPolyQuad *cumulativeHistogram = new GlPolyQuad();

  for (unsigned int i = 0 ; i < nbHistogramBins ; ++i) {
    Vector<int, 4> quadColorCumul;
    quadColorCumul.fill(0);
    Color quadColor;
    unsigned int binSize = histogramBins[i].size();
    cumulativeSize += binSize;

    for (unsigned int j = 0 ; j < binSize ; ++j) {
      if (dataLocation == NODE) {
        for (unsigned int k = 0 ; k < 4 ; ++k ) {
          quadColorCumul[k] += uint(viewColor->getNodeValue(node(histogramBins[i][j]))[k]);
        }
      }
    }

    float binXCoord, binXCoordEnd;

    if (!uniformQuantification) {
      binXCoord = xAxis->getAxisPointCoordForValue(min + i * binWidth).getX();
      binXCoordEnd = xAxis->getAxisPointCoordForValue(min + (i+1) * binWidth).getX();
    }
    else {
      binXCoord = i * refSizeX;
      binXCoordEnd = (i+1) * refSizeX;
    }

    if (binSize > 0) {
      float binYTop, binYBottom;

      if (!cumulativeFreqHisto) {
        binYTop = yAxis->getAxisPointCoordForValue(binSize).getY();
        binYBottom = yAxis->getAxisPointCoordForValue(0).getY();
      }
      else {
        binYTop = yAxis->getAxisPointCoordForValue(cumulativeSize).getY();
        binYBottom = yAxis->getAxisPointCoordForValue(cumulativeSize - binSize).getY();
      }

      Coord startLeftVertex(binXCoord, binYBottom);
      Coord endLeftVertex(binXCoord, binYTop);
      Coord startRightVertex(binXCoordEnd, binYBottom);
      Coord endRightVertex(binXCoordEnd, binYTop);
      vector<Coord> polyQuadCoords;
      polyQuadCoords.push_back(startLeftVertex);
      polyQuadCoords.push_back(startRightVertex);
      polyQuadCoords.push_back(endLeftVertex);
      polyQuadCoords.push_back(endRightVertex);

      if (dataLocation == NODE) {
        quadColorCumul /= binSize;
        quadColor = Color(quadColorCumul[0], quadColorCumul[1], quadColorCumul[2], quadColorCumul[3]);
      }
      else {
        quadColor = Color(255,0,0);
      }

      GlPolyQuad *binRect = new GlPolyQuad(polyQuadCoords, quadColor, BIN_RECT_TEXTURE, true, 1, quadColor);
      binRect->setStencil(1);
      oss.str("");
      oss << "bin rect " << ++rectCpt;
      histoBinsComposite->addGlEntity(binRect, oss.str());
    }

    if (cumulativeFreqHisto) {
      float quadEdgeYTopCoord = yAxis->getAxisPointCoordForValue(cumulativeSize).getY();

      if ((i == 0 || i == (nbHistogramBins - 1)) || binSize > 0) {
        cumulativeHistogram->addQuadEdge(Coord(binXCoord, xAxis->getAxisBaseCoord().getY()), Coord(binXCoord, quadEdgeYTopCoord, 0), quadColor);

        if (i == (nbHistogramBins - 1)) {
          cumulativeHistogram->addQuadEdge(Coord(binXCoordEnd, xAxis->getAxisBaseCoord().getY()), Coord(binXCoordEnd, quadEdgeYTopCoord, 0),quadColor);
        }
      }
    }
  }

  if (cumulativeFreqHisto) {
    histoBinsComposite->addGlEntity(cumulativeHistogram, "cumulative histogram");
  }
  else {
    delete cumulativeHistogram;
  }

  setGraphView(graphComposite, (dataLocation == NODE) ? displayEdges : false);

  GlOffscreenRenderer *glOffscreenRenderer = GlOffscreenRenderer::getInstance();
  glOffscreenRenderer->setViewPortSize(size, size);
  glOffscreenRenderer->clearScene();
  glOffscreenRenderer->setSceneBackgroundColor(backgroundColor);
  glOffscreenRenderer->addGlEntityToScene(xAxis);
  glOffscreenRenderer->addGlEntityToScene(yAxis);
  glOffscreenRenderer->addGlEntityToScene(histoBinsComposite);
  glOffscreenRenderer->addGraphCompositeToScene(graphComposite);
  glOffscreenRenderer->renderScene(true);
  GLuint textureId = glOffscreenRenderer->getGLTexture(true);
  GlTextureManager::getInst().deleteTexture(textureName);
  GlTextureManager::getInst().registerExternalTexture(textureName, textureId);
  glOffscreenRenderer->clearScene();


  Gl2DRect *rectTextured = new Gl2DRect(blCorner.getY()+size, blCorner.getY(), blCorner.getX(), blCorner.getX() + size, textureName);
  rectTextured->setFillColor(Color(255,255,255));
  addGlEntity(rectTextured, textureName + " overview");
  computeBoundingBox();

  lastCumulHisto = cumulativeFreqHisto;

  textureUpdateNeeded = false;
}

void Histogram::setBLCorner(const Coord &blCorner) {
  GlComposite::translate(blCorner - this->blCorner);
  this->blCorner = blCorner;
  computeBoundingBox();
}

void Histogram::setTextColor(const Color &color) {
  if (xAxis != NULL) {
    xAxis->setAxisColor(color);
  }

  if (yAxis != NULL) {
    yAxis->setAxisColor(color);
  }

  textColor = color;
}

void Histogram::setLayoutUpdateNeeded() {
  layoutUpdateNeeded = true;
  textureUpdateNeeded = true;
}

void Histogram::setSizesUpdateNeeded() {
  sizesUpdateNeeded = true;
  textureUpdateNeeded = true;
}

void Histogram::setTextureUpdateNeeded() {
  textureUpdateNeeded = true;
}

}
