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

#include <QApplication>

#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/Iterator.h>
#include <tulip/BoundingBox.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlPolyQuad.h>
#include <tulip/GlCatmullRomCurve.h>
#include <tulip/GlCubicBSplineInterpolation.h>
#include <tulip/GlLine.h>
#include <tulip/GlProgressBar.h>
#include <tulip/GlScene.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/TulipViewSettings.h>

#include "ParallelCoordinatesDrawing.h"
#include "NominalParallelAxis.h"
#include "QuantitativeParallelAxis.h"
#include "ParallelTools.h"

#include <sstream>
#include <climits>
#include <algorithm>
#include <typeinfo>

using namespace std;

namespace tlp {

ParallelCoordinatesDrawing::ParallelCoordinatesDrawing(ParallelCoordinatesGraphProxy *graph, Graph *axisPointsGraph) :
  nbAxis(0), firstAxisPos(Coord(0.0f, 0.0f, 0.0f)), width(0), height(DEFAULT_AXIS_HEIGHT), spaceBetweenAxis(height/2), linesColorAlphaValue(DEFAULT_LINES_COLOR_ALPHA_VALUE), drawPointsOnAxis(true),
  graphProxy(graph), backgroundColor(Color(255,255,255)), createAxisFlag(true), axisPointsGraph(axisPointsGraph), layoutType(PARALLEL), linesType(STRAIGHT),
  linesThickness(THICK), resetAxisLayout(false) {
  axisPointsGraphLayout = axisPointsGraph->getProperty<LayoutProperty>("viewLayout");
  axisPointsGraphSize = axisPointsGraph->getProperty<SizeProperty>("viewSize");
  axisPointsGraphShape = axisPointsGraph->getProperty<IntegerProperty>("viewShape");
  axisPointsGraphLabels = axisPointsGraph->getProperty<StringProperty>("viewLabel");
  axisPointsGraphColors = axisPointsGraph->getProperty<ColorProperty>("viewColor");
  axisPointsGraphSelection = axisPointsGraph->getProperty<BooleanProperty>("viewSelection");

  dataPlotComposite = new GlComposite();
  axisPlotComposite = new GlComposite();
  addGlEntity(dataPlotComposite, "data plot composite");
  addGlEntity(axisPlotComposite, "axis plot composite");
}

ParallelCoordinatesDrawing::~ParallelCoordinatesDrawing() {
}

void ParallelCoordinatesDrawing::createAxis(GlMainWidget *glWidget,
    GlProgressBar* progressBar) {

  GlMainWidget::getFirstQGLWidget()->makeCurrent();

  unsigned int pos = 0;
  vector<string> selectedProperties(graphProxy->getSelectedProperties());
  vector<string>::iterator it;
  GlAxis::CaptionLabelPosition captionPosition;

  static LayoutType lastLayouType = PARALLEL;

  if (axisOrder.size() != selectedProperties.size() || lastLayouType != layoutType) {
    resetAxisLayout = true;
  }

  static vector<Coord> lastAxisCoord;

  if (progressBar) {
    progressBar->setComment("Creating parallel axes ...");
    progressBar->progress(0, selectedProperties.size());
    glWidget->draw();
    // needed to display progressBar
    QApplication::processEvents();
  }


  if (layoutType == PARALLEL) {
    lastAxisCoord.clear();
    vector<ParallelAxis *> lastAxisOrder(getAllAxis());

    for (size_t i = 0 ; i < lastAxisOrder.size() ; ++i) {
      lastAxisCoord.push_back(lastAxisOrder[i]->getBaseCoord());
    }
  }

  static vector<float> lastAxisRotAngle;

  if (layoutType == CIRCULAR) {
    lastAxisRotAngle.clear();
    vector<ParallelAxis *> lastAxisOrder(getAllAxis());

    for (size_t i = 0 ; i < lastAxisOrder.size() ; ++i) {
      lastAxisRotAngle.push_back(lastAxisOrder[i]->getRotationAngle());
    }
  }

  map<string, ParallelAxis *>::iterator it2;

  for (it2 = parallelAxis.begin() ; it2 != parallelAxis.end() ; ++it2) {
    (it2->second)->setHidden(true);
  }

  nbAxis = graphProxy->getNumberOfSelectedProperties();
  axisOrder.clear();

  if (nbAxis > 1) {
    width = spaceBetweenAxis * (nbAxis - 1);
  }
  else {
    width = spaceBetweenAxis;
  }

  float circleLayoutYOffset = (nbAxis * 100.0f) / 4.0f;

  Color axisColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    axisColor = Color(255,255,255);
  }
  else {
    axisColor = Color(0,0,0);
  }

  float maxCaptionWidth = (8.0f/10.0f) * spaceBetweenAxis;

  if (selectedProperties.size() < 3) {
    layoutType = PARALLEL;
  }

  float rotationAngleBase = 0.0f;

  if (layoutType == CIRCULAR) {
    rotationAngleBase = -(2.0f * M_PI) / selectedProperties.size();
    captionPosition = GlAxis::ABOVE;
  }
  else {
    captionPosition = GlAxis::BELOW;
  }

  unsigned int cpt = 0;

  for (it = selectedProperties.begin(); it != selectedProperties.end() ; ++it) {

    ParallelAxis *axis = NULL;
    float rotationAngle = (cpt++ * rotationAngleBase) * (180.0f / M_PI);
    Coord coord;

    if (layoutType == PARALLEL) {
      if (nbAxis!=1) {
        coord = Coord(firstAxisPos.getX() + pos * (width/(nbAxis - 1)),firstAxisPos.getY());
      }
      else {
        coord = Coord(firstAxisPos.getX() + (width/2.0f), firstAxisPos.getY());
      }
    }
    else {
      coord = Coord(0, circleLayoutYOffset, 0.0f);
    }

    if (parallelAxis.find(*it) != parallelAxis.end()) {
      axis = (parallelAxis.find(*it))->second;

      if (layoutType == PARALLEL) {
        axis->setRotationAngle(0.0f);

        if (!resetAxisLayout && pos < lastAxisCoord.size()) {
          axis->translate(lastAxisCoord[pos] - axis->getBaseCoord());
        }
        else {
          axis->translate(coord - axis->getBaseCoord());
        }
      }
      else {
        axis->setBaseCoord(coord);

        if (!resetAxisLayout) {
          axis->setRotationAngle(lastAxisRotAngle[pos]);
        }
        else {
          axis->setRotationAngle(rotationAngle);
        }
      }

      axis->setCaptionPosition(captionPosition);
      axis->setAxisHeight(height);
      axis->setMaxCaptionWidth(maxCaptionWidth);
      axis->setAxisColor(axisColor);
      axis->redraw();
      axis->setHidden(false);
    }
    else {
      string typeName((graphProxy->getProperty(*it))->getTypename());

      if (typeName == "string") {
        axis = new NominalParallelAxis(coord, height, maxCaptionWidth, graphProxy, *it, axisColor, rotationAngle, captionPosition);
      }
      else if (typeName == "int" || typeName == "double") {
        axis = new QuantitativeParallelAxis(coord, height, maxCaptionWidth, graphProxy, *it, true, axisColor, rotationAngle, captionPosition);
      }
    }

    if (axis != NULL) {
      axisPlotComposite->addGlEntity(axis, *it);
      axisOrder.push_back(*it);
      parallelAxis[*it] = axis;
      ++pos;
    }

    if (progressBar) {
      progressBar->progress(pos, selectedProperties.size());
      glWidget->draw();
      // needed to display progressBar
      QApplication::processEvents();
    }
  }

  resetAxisLayout = false;
  lastLayouType = layoutType;
}

void ParallelCoordinatesDrawing::destroyAxisIfNeeded() {
  map<string, ParallelAxis *>::iterator it;

  for (it = parallelAxis.begin() ; it != parallelAxis.end() ; ++it) {
    if (!graphProxy->existProperty(it->first)) {
      delete it->second;
      parallelAxis.erase(it->first);
    }
  }
}

void ParallelCoordinatesDrawing::plotAllData(GlMainWidget *glWidget,
    GlProgressBar *progressBar) {
  Color color;
  computeResizeFactor();

  int currentStep = 0;
  int maxStep = graphProxy->numberOfNodes();
  int drawStep = maxStep / 20;

  if (progressBar) {
    progressBar->setComment("Updating parallel coordinates ...");
    progressBar->progress(0, graphProxy->numberOfNodes());
    glWidget->draw();
    // needed to display progressBar
    QApplication::processEvents();
  }

  Iterator<unsigned int> *dataUnselIt = graphProxy->getDataIterator();

  while (dataUnselIt->hasNext()) {
    unsigned int dataId = dataUnselIt->next();

    if (!graphProxy->isDataSelected(dataId)) {
      color = graphProxy->getDataColor(dataId);

      if (linesColorAlphaValue <= 255 &&
          ((graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(dataId)) || (!graphProxy->highlightedEltsSet()))) {
        color.setA(linesColorAlphaValue);
      }
    }
    else {
      color = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters().getSelectionColor();
    }

    plotData(dataId, color);
    ++currentStep;

    if (progressBar && currentStep % drawStep == 0) {
      progressBar->progress(currentStep, maxStep);
      glWidget->draw();
      // needed to display progressBar
      QApplication::processEvents();
    }
  }

  delete dataUnselIt;

  lastHighlightedElements = graphProxy->getHighlightedElts();
}

void ParallelCoordinatesDrawing::plotData(const unsigned int dataId, const Color &color) {

  Size eltMinSize(graphProxy->getProperty<SizeProperty>("viewSize")->getMin());
  Size dataViewSize(graphProxy->getDataViewSize(dataId));
  Size adjustedViewSize(axisPointMinSize + resizeFactor * (dataViewSize - eltMinSize));
  float pointRadius =((adjustedViewSize[0] + adjustedViewSize[1] + adjustedViewSize[2]) / 3.0f) / 2.0f;
  float lineHalfWidth = pointRadius - (1.0f/10.0f) * pointRadius;


  vector<Coord> polylineCoords;
  vector<Coord> splineCurvePassPoints;

  for (size_t j = 0; j < axisOrder.size() ; j++) {

    Coord pointCoord(parallelAxis[axisOrder[j]]->getPointCoordOnAxisForData(dataId));
    float axisRotAngle = parallelAxis[axisOrder[j]]->getRotationAngle();
    ostringstream oss;
    oss << "data " << dataId << " var " << axisOrder[j];

    if (drawPointsOnAxis) {

      if (!graphProxy->highlightedEltsSet() || (graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(dataId)) || graphProxy->isDataSelected(dataId)) {
        node n = axisPointsGraph->addNode();
        axisPointsDataMap[n] = dataId;
        axisPointsGraphLayout->setNodeValue(n, pointCoord);
        axisPointsGraphSize->setNodeValue(n, adjustedViewSize);

        if (graphProxy->getDataLocation() == NODE) {
          axisPointsGraphShape->setNodeValue(n, graphProxy->getPropertyValueForData<IntegerProperty, IntegerType>("viewShape", dataId));
        }
        else {
          axisPointsGraphShape->setNodeValue(n, NodeShape::Circle);
        }

        axisPointsGraphLabels->setNodeValue(n, graphProxy->getPropertyValueForData<StringProperty, StringType>("viewLabel", dataId));
        axisPointsGraphColors->setNodeValue(n, graphProxy->getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId));

        if (graphProxy->isDataSelected(dataId)) {
          axisPointsGraphSelection->setNodeValue(n, true);
        }
      }
    }

    if (linesType == STRAIGHT) {
      if (linesThickness == THICK) {
        Coord vec1(0.0f, - lineHalfWidth);
        Coord vec2(0.0f, lineHalfWidth);

        if (axisRotAngle != 0.0f) {
          rotateVector(vec1, axisRotAngle, Z_ROT);
          rotateVector(vec2, axisRotAngle, Z_ROT);
        }

        polylineCoords.push_back(pointCoord + vec1);
        polylineCoords.push_back(pointCoord + vec2);
      }
      else {
        polylineCoords.push_back(pointCoord);
      }
    }
    else {
      splineCurvePassPoints.push_back(pointCoord);
    }
  }


  if (axisOrder.size() < 2)
    return;

  GlSimpleEntity *line = NULL;
  bool closeSpline = (layoutType == CIRCULAR);

  if (linesType == STRAIGHT) {
    if (layoutType == CIRCULAR) {
      polylineCoords.push_back(polylineCoords[0]);

      if (linesThickness == THICK) {
        polylineCoords.push_back(polylineCoords[1]);
      }
    }

    if (linesThickness == THICK) {
      GlPolyQuad *polyquad = new GlPolyQuad(polylineCoords, color, lineTextureFilename, true, 1, color);
      polyquad->setOutlined(false);
      line = polyquad;
    }
    else {
      vector<Color> lineColor;
      lineColor.push_back(color);
      line = new GlLine(polylineCoords, lineColor);
    }
  }
  else if (linesType == CATMULL_ROM_SPLINE) {
    float size = 1;
    string textureName = "";

    if (linesThickness == THICK) {
      size = pointRadius;
      textureName = lineTextureFilename;
    }

    GlCatmullRomCurve *catmull = new GlCatmullRomCurve(splineCurvePassPoints, color, color, size, size, closeSpline, 20 * splineCurvePassPoints.size() - 1);

    if (textureName == DEFAULT_TEXTURE_FILE) {
      catmull->setOutlined(true);
      catmull->setOutlineColor(Color(0,0,0,0));
    }

    catmull->setTexture(textureName);
    line = catmull;
  }
  else {
    float size = 1;
    string textureName = "";

    if (linesThickness == THICK) {
      size = pointRadius;
      textureName = lineTextureFilename;
    }

    if (layoutType == CIRCULAR) {
      splineCurvePassPoints.push_back(splineCurvePassPoints[0]);
    }

    GlCubicBSplineInterpolation *cubicInterpolation = new GlCubicBSplineInterpolation(splineCurvePassPoints, color, color, size, size, 20 * splineCurvePassPoints.size() - 1);

    if (textureName == DEFAULT_TEXTURE_FILE) {
      cubicInterpolation->setOutlined(true);
      cubicInterpolation->setOutlineColor(Color(0,0,0,0));
    }

    cubicInterpolation->setTexture(textureName);
    line = cubicInterpolation;
  }

  if (graphProxy->isDataHighlighted(dataId)) {
    line->setStencil(4);
  }

  if (graphProxy->isDataSelected(dataId)) {
    line->setStencil(3);
  }

  ostringstream oss2;
  oss2 << "data " << dataId << " line ";
  dataPlotComposite->addGlEntity(line, oss2.str());
  glEntitiesDataMap[line] = dataId;
}

unsigned int ParallelCoordinatesDrawing::nbParallelAxis() const {
  return nbAxis;
}

void ParallelCoordinatesDrawing::swapAxis(ParallelAxis *axis1, ParallelAxis *axis2) {
  vector<string>::iterator it;
  int pi = 0, pj = 0;
  int pos = 0;

  for (it = axisOrder.begin(); it != axisOrder.end(); ++it, ++pos) {
    if (*it == axis1->getAxisName()) {
      pi = pos;
    }

    if (*it == axis2->getAxisName()) {
      pj = pos;
    }
  }

  string tmp(axisOrder[pi]);
  axisOrder[pi] = axisOrder[pj];
  axisOrder[pj] = tmp;

  if (layoutType == PARALLEL) {

    Coord ci(parallelAxis[axis1->getAxisName()]->getBaseCoord());
    Coord cj(parallelAxis[axis2->getAxisName()]->getBaseCoord());

    parallelAxis[axis1->getAxisName()]->translate(cj - ci);
    parallelAxis[axis2->getAxisName()]->translate(ci - cj);

  }
  else {

    float axis1RotAngle = parallelAxis[axis1->getAxisName()]->getRotationAngle();
    parallelAxis[axis1->getAxisName()]->setRotationAngle(parallelAxis[axis2->getAxisName()]->getRotationAngle());
    parallelAxis[axis2->getAxisName()]->setRotationAngle(axis1RotAngle);
  }

  graphProxy->setSelectedProperties(axisOrder);

  createAxisFlag = false;
}

bool ParallelCoordinatesDrawing::getDataIdFromGlEntity(GlEntity *glEntity, unsigned int &dataId) {

  bool dataMatch = glEntitiesDataMap.find(glEntity) != glEntitiesDataMap.end();

  if (dataMatch) {
    dataId = glEntitiesDataMap[glEntity];
  }

  return dataMatch;
}

bool ParallelCoordinatesDrawing::getDataIdFromAxisPoint(node axisPoint, unsigned int &dataId) {

  bool dataMatch = axisPointsDataMap.find(axisPoint) != axisPointsDataMap.end();

  if (dataMatch) {
    dataId = axisPointsDataMap[axisPoint];
  }

  return dataMatch;
}

void ParallelCoordinatesDrawing::update(GlMainWidget *glWidget, bool updateWithoutProgressBar) {

  deleteGlEntity(axisPlotComposite);
  deleteGlEntity(dataPlotComposite);

  destroyAxisIfNeeded();

  // needed to have some feedback
  GlProgressBar *progressBar = NULL;

  if (!updateWithoutProgressBar) {
    progressBar =
      new GlProgressBar(Coord(0.0f, 0.0f, 0.0f), 600, 100,
                        // use same green color as the highlighting one
                        // in workspace panel
                        Color(0xCB, 0xDE, 0x5D));
    progressBar->setComment("Updating parallel coordinates ...");
    progressBar->progress(0, graphProxy->numberOfNodes());
    addGlEntity(progressBar, "progress bar");
    glWidget->draw();
    // needed to display progressBar
    QApplication::processEvents();
  }

  if (createAxisFlag) {
    axisPlotComposite->reset(false);
    createAxis(glWidget, progressBar);
  }

  eraseDataPlot();
  plotAllData(glWidget, progressBar);

  if (progressBar != NULL) {
    deleteGlEntity(progressBar);
    delete progressBar;
  }

  createAxisFlag = true;

  addGlEntity(dataPlotComposite, "data plot composite");
  addGlEntity(axisPlotComposite, "axis plot composite");
}

void ParallelCoordinatesDrawing::eraseDataPlot() {
  dataPlotComposite->reset(true);
  BooleanProperty *wholeGraphSelec = new BooleanProperty(axisPointsGraph);
  wholeGraphSelec->setAllNodeValue(true);
  removeFromGraph(axisPointsGraph, wholeGraphSelec);
  delete wholeGraphSelec;
  glEntitiesDataMap.clear();
  axisPointsDataMap.clear();
}

void ParallelCoordinatesDrawing::eraseAxisPlot() {
  axisPlotComposite->reset(true);
  parallelAxis.clear();
}

void ParallelCoordinatesDrawing::erase() {
  eraseDataPlot();
  eraseAxisPlot();
}

void ParallelCoordinatesDrawing::removeAxis(ParallelAxis *axis) {
  if (axisPlotComposite->findKey(axis) != "") {
    axis->setHidden(true);
    axisPlotComposite->deleteGlEntity(axis);
  }
}

void ParallelCoordinatesDrawing::addAxis(ParallelAxis *axis) {
  if (axisPlotComposite->findKey(axis) == "") {
    axis->setHidden(false);
    axisPlotComposite->addGlEntity(axis, axis->getAxisName());
  }
}

const vector<string> & ParallelCoordinatesDrawing::getAxisNames() const {
  return axisOrder;
}

void ParallelCoordinatesDrawing::computeResizeFactor() {
  Size eltMinSize(graphProxy->getProperty<SizeProperty>("viewSize")->getMin());
  Size eltMaxSize(graphProxy->getProperty<SizeProperty>("viewSize")->getMax());

  Size deltaSize(eltMaxSize - eltMinSize);

  for (unsigned int i = 0 ; i < 3 ; ++i) {
    if (deltaSize[i] != 0.0f) {
      resizeFactor[i] = (axisPointMaxSize[i] - axisPointMinSize[i]) / deltaSize[i];
    }
    else {
      resizeFactor[i] = 0.0f;
    }
  }
}

vector<ParallelAxis *> ParallelCoordinatesDrawing::getAllAxis() {
  vector<ParallelAxis *> axis;

  for (size_t i = 0 ; i < axisOrder.size() ; ++i) {
    ParallelAxis *pa = parallelAxis[axisOrder[i]];

    if (pa == NULL) {
      // on the fly clean up for deleted properties
      parallelAxis.erase(axisOrder[i]);
      continue;
    }

    if (!pa->isHidden()) {
      axis.push_back(pa);
    }
  }

  return axis;
}

void ParallelCoordinatesDrawing::updateWithAxisSlidersRange(ParallelAxis *axis, HighlightedEltsSetOp highlightedEltsSetOp) {
  set<unsigned int> dataSubset;

  if (highlightedEltsSetOp == INTERSECTION) {
    const set<unsigned int> &eltsInSlidersRange(axis->getDataInSlidersRange());
    const set<unsigned int> &currentHighlightedElts(graphProxy->getHighlightedElts());
    unsigned int size = eltsInSlidersRange.size() > currentHighlightedElts.size() ? eltsInSlidersRange.size() : currentHighlightedElts.size();
    vector<unsigned int> intersection(size);
    vector<unsigned int>::iterator intersectionEnd = std::set_intersection(eltsInSlidersRange.begin(), eltsInSlidersRange.end(),
        currentHighlightedElts.begin(), currentHighlightedElts.end(),
        intersection.begin());
    dataSubset = set<unsigned int>(intersection.begin(), intersectionEnd);
  }
  else if (highlightedEltsSetOp == UNION) {
    const set<unsigned int> &eltsInSlidersRange(axis->getDataInSlidersRange());
    const set<unsigned int> &currentHighlightedElts(graphProxy->getHighlightedElts());

    vector<unsigned int> unionSet(eltsInSlidersRange.size() + currentHighlightedElts.size());
    vector<unsigned int>::iterator unionEnd = std::set_union(eltsInSlidersRange.begin(), eltsInSlidersRange.end(),
        currentHighlightedElts.begin(), currentHighlightedElts.end(),
        unionSet.begin());
    dataSubset = set<unsigned int>(unionSet.begin(), unionEnd);
  }
  else {
    dataSubset = axis->getDataInSlidersRange();
  }

  if (!dataSubset.empty()) {
    graphProxy->unsetHighlightedElts();
    set<unsigned int>::iterator it;

    for (it = dataSubset.begin() ; it != dataSubset.end() ; ++it) {
      graphProxy->addOrRemoveEltToHighlight(*it);
    }

    map<string, ParallelAxis *>::iterator it2;

    for (it2 = parallelAxis.begin() ; it2 != parallelAxis.end() ; ++it2) {
      if ((it2->second) != axis) {
        (it2->second)->updateSlidersWithDataSubset(dataSubset);
      }
    }

    createAxisFlag = false;
  }
}

void ParallelCoordinatesDrawing::resetAxisSlidersPosition() {
  vector<ParallelAxis *> axis = getAllAxis();
  vector<ParallelAxis *>::iterator it;

  for (it = axis.begin() ; it != axis.end() ; ++it) {
    (*it)->resetSlidersPosition();
  }
}

void ParallelCoordinatesDrawing::delNode(Graph *,const node n) {
  if (graphProxy->getDataLocation() == NODE) {
    removeHighlightedElt(n.id);
  }
}

void ParallelCoordinatesDrawing::delEdge(Graph *,const edge e) {
  if (graphProxy->getDataLocation() == EDGE) {
    removeHighlightedElt(e.id);
  }
}

void ParallelCoordinatesDrawing::treatEvent(const tlp::Event& evt) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

  if (gEvt) {
    Graph* graph = gEvt->getGraph();

    switch(gEvt->getType()) {
    case GraphEvent::TLP_DEL_NODE:
      delNode(graph, gEvt->getNode());
      break;

    case GraphEvent::TLP_DEL_EDGE:
      delEdge(graph, gEvt->getEdge());
      break;

    default:
      break;
    }
  }
}

void ParallelCoordinatesDrawing::removeHighlightedElt(const unsigned int dataId) {
  if (lastHighlightedElements.find(dataId) != lastHighlightedElements.end()) {
    lastHighlightedElements.erase(dataId);
    graphProxy->removeHighlightedElement(dataId);

    if (!graphProxy->highlightedEltsSet()) {
      graphProxy->colorDataAccordingToHighlightedElts();
    }
  }
}

}

