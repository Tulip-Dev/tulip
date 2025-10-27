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

#include <QProgressDialog>

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
#include <tulip/GlScene.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>

#include "ParallelCoordinatesDrawing.h"
#include "NominalParallelAxis.h"
#include "QuantitativeParallelAxis.h"
#include "ParallelTools.h"
#include "ParallelCoordinatesGraphProxy.h"

#include <sstream>

using namespace std;

namespace tlp {

ParallelCoordinatesDrawing::ParallelCoordinatesDrawing(ParallelCoordinatesGraphProxy *graph,
                                                       Graph *axisPointsGraph)
    : nbAxis(0), firstAxisPos(Coord(0.0f, 0.0f, 0.0f)), width(0), height(DEFAULT_AXIS_HEIGHT),
      spaceBetweenAxis(height / 2), linesColorAlphaValue(DEFAULT_LINES_COLOR_ALPHA_VALUE),
      drawPointsOnAxis(true), graphProxy(graph), backgroundColor(Color(255, 255, 255)),
      createAxisFlag(true), axisPointsGraph(axisPointsGraph), layoutType(PARALLEL),
      linesType(STRAIGHT), linesThickness(THICK), resetAxisLayout(false) {
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

ParallelCoordinatesDrawing::~ParallelCoordinatesDrawing() {}

void ParallelCoordinatesDrawing::createAxis(GlMainWidget *glWidget, QProgressDialog *progress) {

  glWidget->makeCurrent();

  unsigned int pos = 0;
  vector<string> selectedProperties(graphProxy->getSelectedProperties());
  GlAxis::CaptionLabelPosition captionPosition;

  static LayoutType lastLayouType = PARALLEL;

  if (axisOrder.size() != selectedProperties.size() || lastLayouType != layoutType) {
    resetAxisLayout = true;
  }

  static vector<Coord> lastAxisCoord;

  if (progress) {
    progress->setWindowTitle("Creating parallel axes ...");
    progress->setRange(0, selectedProperties.size());
    progress->setValue(0);
  }

  if (layoutType == PARALLEL) {
    lastAxisCoord.clear();

    for (auto axis : getAllAxis()) {
      lastAxisCoord.push_back(axis->getBaseCoord());
    }
  }

  static vector<float> lastAxisRotAngle;

  if (layoutType == CIRCULAR) {
    lastAxisRotAngle.clear();

    for (auto axis : getAllAxis()) {
      lastAxisRotAngle.push_back(axis->getRotationAngle());
    }
  }

  for (auto &it2 : parallelAxis) {
    it2.second->setHidden(true);
  }

  nbAxis = graphProxy->getNumberOfSelectedProperties();
  axisOrder.clear();

  if (nbAxis > 1) {
    width = spaceBetweenAxis * (nbAxis - 1);
  } else {
    width = spaceBetweenAxis;
  }

  float circleLayoutYOffset = (nbAxis * 100.0f) / 4.0f;

  Color axisColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    axisColor = Color(255, 255, 255);
  } else {
    axisColor = Color(0, 0, 0);
  }

  float maxCaptionWidth = (8.0f / 10.0f) * spaceBetweenAxis;

  if (selectedProperties.size() < 3) {
    layoutType = PARALLEL;
  }

  float rotationAngleBase = 0.0f;

  if (layoutType == CIRCULAR) {
    rotationAngleBase = -(2.0f * M_PI) / selectedProperties.size();
    captionPosition = GlAxis::ABOVE;
  } else {
    captionPosition = GlAxis::BELOW;
  }

  unsigned int cpt = 0;

  for (const string &selectedProp : selectedProperties) {

    ParallelAxis *axis = nullptr;
    float rotationAngle = (cpt++ * rotationAngleBase) * (180.0f / M_PI);
    Coord coord;

    if (layoutType == PARALLEL) {
      if (nbAxis != 1) {
        coord = Coord(firstAxisPos.getX() + pos * (width / (nbAxis - 1)), firstAxisPos.getY());
      } else {
        coord = Coord(firstAxisPos.getX() + (width / 2.0f), firstAxisPos.getY());
      }
    } else {
      coord = Coord(0, circleLayoutYOffset, 0.0f);
    }

    if (parallelAxis.find(selectedProp) != parallelAxis.end()) {
      axis = (parallelAxis.find(selectedProp))->second;

      if (layoutType == PARALLEL) {
        axis->setRotationAngle(0.0f);

        if (!resetAxisLayout && pos < lastAxisCoord.size()) {
          axis->translate(lastAxisCoord[pos] - axis->getBaseCoord());
        } else {
          axis->translate(coord - axis->getBaseCoord());
        }
      } else {
        axis->setBaseCoord(coord);

        if (!resetAxisLayout) {
          axis->setRotationAngle(lastAxisRotAngle[pos]);
        } else {
          axis->setRotationAngle(rotationAngle);
        }
      }

      axis->setCaptionPosition(captionPosition);
      axis->setAxisHeight(height);
      axis->setMaxCaptionWidth(maxCaptionWidth);
      axis->setAxisColor(axisColor);
      axis->redraw();
      axis->setHidden(false);
    } else {
      string typeName((graphProxy->getProperty(selectedProp))->getTypename());

      if (typeName == "string") {
        axis = new NominalParallelAxis(coord, height, maxCaptionWidth, graphProxy, selectedProp,
                                       axisColor, rotationAngle, captionPosition);
      } else if (typeName == "int" || typeName == "double") {
        axis =
            new QuantitativeParallelAxis(coord, height, maxCaptionWidth, graphProxy, selectedProp,
                                         true, axisColor, rotationAngle, captionPosition);
      }
    }

    if (axis != nullptr) {
      axisPlotComposite->addGlEntity(axis, selectedProp);
      axisOrder.push_back(selectedProp);
      parallelAxis[selectedProp] = axis;
      ++pos;
    }

    if (progress)
      progress->setValue(pos);
  }

  resetAxisLayout = false;
  lastLayouType = layoutType;
}

void ParallelCoordinatesDrawing::destroyAxisIfNeeded() {
  for (auto it = parallelAxis.begin(); it != parallelAxis.end(); ++it) {
    if (!graphProxy->existProperty(it->first)) {
      delete it->second;
      parallelAxis.erase(it->first);
    }
  }
}

void ParallelCoordinatesDrawing::plotAllData(GlMainWidget *glWidget, QProgressDialog *progress) {
  Color color;
  computeResizeFactor();

  int currentStep = 0;
  int maxStep = graphProxy->getDataCount();
  int drawStep = maxStep / 100;

  if (progress) {
    progress->setWindowTitle("Updating parallel coordinates ...");
    progress->setRange(0, maxStep);
    progress->setValue(0);
  }

  for (unsigned int dataId : graphProxy->getDataIterator()) {

    if (!graphProxy->isDataSelected(dataId)) {
      color = graphProxy->getDataColor(dataId);

      if (linesColorAlphaValue <= 255 &&
          ((graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(dataId)) ||
           (!graphProxy->highlightedEltsSet()))) {
        color.setA(linesColorAlphaValue);
      }
    } else {
      color =
          glWidget->getScene()->getGlGraphComposite()->getRenderingParameters().getSelectionColor();
    }

    plotData(dataId, color);

    if (progress && (++currentStep % drawStep == 0))
      progress->setValue(currentStep);
  }

  lastHighlightedElements = graphProxy->getHighlightedElts();
}

void ParallelCoordinatesDrawing::plotData(const unsigned int dataId, const Color &color) {

  Size eltMinSize(graphProxy->getProperty<SizeProperty>("viewSize")->getMin());
  Size dataViewSize(graphProxy->getDataViewSize(dataId));
  Size adjustedViewSize(axisPointMinSize + resizeFactor * (dataViewSize - eltMinSize));
  float pointRadius =
      ((adjustedViewSize[0] + adjustedViewSize[1] + adjustedViewSize[2]) / 3.0f) / 2.0f;
  float lineHalfWidth = pointRadius - (1.0f / 10.0f) * pointRadius;

  vector<Coord> polylineCoords;
  vector<Coord> splineCurvePassPoints;

  for (const auto &ao : axisOrder) {
    ParallelAxis *pa = parallelAxis[ao];
    const Coord &pointCoord = pa->getPointCoordOnAxisForData(dataId);
    float axisRotAngle = pa->getRotationAngle();
    ostringstream oss;
    oss << "data " << dataId << " var " << ao;

    if (drawPointsOnAxis) {

      if (!graphProxy->highlightedEltsSet() || graphProxy->isDataSelected(dataId)) {
        node n = axisPointsGraph->addNode();
        axisPointsDataMap[n] = dataId;
        axisPointsGraphLayout->setNodeValue(n, pointCoord);
        axisPointsGraphSize->setNodeValue(n, adjustedViewSize);

        if (graphProxy->getDataLocation() == NODE) {
          axisPointsGraphShape->setNodeValue(
              n, graphProxy->getPropertyValueForData<IntegerProperty, IntegerType>("viewShape",
                                                                                   dataId));
        } else {
          axisPointsGraphShape->setNodeValue(n, NodeShape::Circle);
        }

        axisPointsGraphLabels->setNodeValue(
            n,
            graphProxy->getPropertyValueForData<StringProperty, StringType>("viewLabel", dataId));
        axisPointsGraphColors->setNodeValue(
            n, graphProxy->getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId));

        if (graphProxy->isDataSelected(dataId)) {
          axisPointsGraphSelection->setNodeValue(n, true);
        }
      }
    }

    if (linesType == STRAIGHT) {
      if (linesThickness == THICK) {
        Coord vec1(0.0f, -lineHalfWidth);
        Coord vec2(0.0f, lineHalfWidth);

        if (axisRotAngle != 0.0f) {
          rotateVector(vec1, axisRotAngle, Z_ROT);
          rotateVector(vec2, axisRotAngle, Z_ROT);
        }

        polylineCoords.push_back(pointCoord + vec1);
        polylineCoords.push_back(pointCoord + vec2);
      } else {
        polylineCoords.push_back(pointCoord);
      }
    } else {
      splineCurvePassPoints.push_back(pointCoord);
    }
  }

  if (axisOrder.size() < 2)
    return;

  GlSimpleEntity *line = nullptr;
  bool closeSpline = (layoutType == CIRCULAR);

  if (linesType == STRAIGHT) {
    if (layoutType == CIRCULAR) {
      polylineCoords.push_back(polylineCoords[0]);

      if (linesThickness == THICK) {
        polylineCoords.push_back(polylineCoords[1]);
      }
    }

    if (linesThickness == THICK) {
      GlPolyQuad *polyquad =
          new GlPolyQuad(polylineCoords, color, lineTextureFilename, true, 1, color);
      polyquad->setOutlined(false);
      line = polyquad;
    } else {
      vector<Color> lineColor;
      lineColor.push_back(color);
      line = new GlLine(polylineCoords, lineColor);
    }
  } else if (linesType == CATMULL_ROM_SPLINE) {
    float size = 1;
    string textureName = "";

    if (linesThickness == THICK) {
      size = pointRadius;
      textureName = lineTextureFilename;
    }

    GlCatmullRomCurve *catmull =
        new GlCatmullRomCurve(splineCurvePassPoints, color, color, size, size, closeSpline,
                              20 * splineCurvePassPoints.size() - 1);

    if (textureName == DEFAULT_TEXTURE_FILE) {
      catmull->setOutlined(true);
      catmull->setOutlineColor(Color(0, 0, 0, 0));
    }

    catmull->setTexture(textureName);
    line = catmull;
  } else {
    float size = 1;
    string textureName = "";

    if (linesThickness == THICK) {
      size = pointRadius;
      textureName = lineTextureFilename;
    }

    if (layoutType == CIRCULAR) {
      splineCurvePassPoints.push_back(splineCurvePassPoints[0]);
    }

    GlCubicBSplineInterpolation *cubicInterpolation = new GlCubicBSplineInterpolation(
        splineCurvePassPoints, color, color, size, size, 20 * splineCurvePassPoints.size() - 1);

    if (textureName == DEFAULT_TEXTURE_FILE) {
      cubicInterpolation->setOutlined(true);
      cubicInterpolation->setOutlineColor(Color(0, 0, 0, 0));
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
  int pi = 0, pj = 0;
  int pos = 0;

  for (auto it = axisOrder.begin(); it != axisOrder.end(); ++it, ++pos) {
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

    Coord ci = axis1->getBaseCoord();
    Coord cj = axis2->getBaseCoord();

    axis1->translate(cj - ci);
    axis2->translate(ci - cj);

  } else {

    float axis1RotAngle = axis1->getRotationAngle();
    axis1->setRotationAngle(axis2->getRotationAngle());
    axis2->setRotationAngle(axis1RotAngle);
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

void ParallelCoordinatesDrawing::update(GlMainWidget *glWidget, bool progressBar) {

  deleteGlEntity(axisPlotComposite);
  deleteGlEntity(dataPlotComposite);

  destroyAxisIfNeeded();

  // needed to have some feedback
  QProgressDialog *progress = nullptr;

  if (progressBar) {
    progress = new QProgressDialog(Perspective::instance()->mainWindow());
    progress->setCancelButton(nullptr);
    progress->setWindowTitle("Updating parallel coordinates ...");
    progress->setRange(0, graphProxy->numberOfNodes());
    progress->setValue(0);
    progress->setMinimumWidth(400);
    progress->setWindowModality(Qt::WindowModal);
  }

  if (createAxisFlag) {
    axisPlotComposite->reset(false);
    createAxis(glWidget, progress);
  }

  eraseDataPlot();
  plotAllData(glWidget, progress);

  if (progressBar)
    delete progress;

  createAxisFlag = true;

  addGlEntity(dataPlotComposite, "data plot composite");
  addGlEntity(axisPlotComposite, "axis plot composite");
}

void ParallelCoordinatesDrawing::eraseDataPlot() {
  dataPlotComposite->reset(true);
  axisPointsGraph->clear();
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
  if (!axisPlotComposite->findKey(axis).empty()) {
    axis->setHidden(true);
    axisPlotComposite->deleteGlEntity(axis);
  }
}

void ParallelCoordinatesDrawing::addAxis(ParallelAxis *axis) {
  if (axisPlotComposite->findKey(axis).empty()) {
    axis->setHidden(false);
    axisPlotComposite->addGlEntity(axis, axis->getAxisName());
  }
}

const vector<string> &ParallelCoordinatesDrawing::getAxisNames() const {
  return axisOrder;
}

void ParallelCoordinatesDrawing::computeResizeFactor() {
  Size eltMinSize(graphProxy->getProperty<SizeProperty>("viewSize")->getMin());
  Size eltMaxSize(graphProxy->getProperty<SizeProperty>("viewSize")->getMax());

  Size deltaSize(eltMaxSize - eltMinSize);

  for (unsigned int i = 0; i < 3; ++i) {
    if (deltaSize[i] != 0.0f) {
      resizeFactor[i] = (axisPointMaxSize[i] - axisPointMinSize[i]) / deltaSize[i];
    } else {
      resizeFactor[i] = 0.0f;
    }
  }
}

vector<ParallelAxis *> ParallelCoordinatesDrawing::getAllAxis() {
  vector<ParallelAxis *> axis;

  for (size_t i = 0; i < axisOrder.size(); ++i) {
    ParallelAxis *pa = parallelAxis[axisOrder[i]];

    if (pa == nullptr) {
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

void ParallelCoordinatesDrawing::updateWithAxisSlidersRange(
    ParallelAxis *axis, HighlightedEltsSetOp highlightedEltsSetOp) {
  set<unsigned int> dataSubset;

  if (highlightedEltsSetOp == INTERSECTION) {
    const set<unsigned int> &eltsInSlidersRange(axis->getDataInSlidersRange());
    const set<unsigned int> &currentHighlightedElts(graphProxy->getHighlightedElts());
    unsigned int size = eltsInSlidersRange.size() > currentHighlightedElts.size()
                            ? eltsInSlidersRange.size()
                            : currentHighlightedElts.size();
    vector<unsigned int> intersection(size);
    vector<unsigned int>::iterator intersectionEnd = std::set_intersection(
        eltsInSlidersRange.begin(), eltsInSlidersRange.end(), currentHighlightedElts.begin(),
        currentHighlightedElts.end(), intersection.begin());
    dataSubset = set<unsigned int>(intersection.begin(), intersectionEnd);
  } else if (highlightedEltsSetOp == UNION) {
    const set<unsigned int> &eltsInSlidersRange(axis->getDataInSlidersRange());
    const set<unsigned int> &currentHighlightedElts(graphProxy->getHighlightedElts());

    vector<unsigned int> unionSet(eltsInSlidersRange.size() + currentHighlightedElts.size());
    vector<unsigned int>::iterator unionEnd = std::set_union(
        eltsInSlidersRange.begin(), eltsInSlidersRange.end(), currentHighlightedElts.begin(),
        currentHighlightedElts.end(), unionSet.begin());
    dataSubset = set<unsigned int>(unionSet.begin(), unionEnd);
  } else {
    dataSubset = axis->getDataInSlidersRange();
  }

  if (!dataSubset.empty()) {
    graphProxy->unsetHighlightedElts();
    set<unsigned int>::iterator it;

    for (it = dataSubset.begin(); it != dataSubset.end(); ++it) {
      graphProxy->addOrRemoveEltToHighlight(*it);
    }

    for (auto it2 = parallelAxis.begin(); it2 != parallelAxis.end(); ++it2) {
      if ((it2->second) != axis) {
        (it2->second)->updateSlidersWithDataSubset(dataSubset);
      }
    }

    createAxisFlag = false;
  }
}

void ParallelCoordinatesDrawing::resetAxisSlidersPosition() {
  for (auto pa : getAllAxis()) {
    pa->resetSlidersPosition();
  }
}

void ParallelCoordinatesDrawing::delNode(Graph *, const node n) {
  if (graphProxy->getDataLocation() == NODE) {
    removeHighlightedElt(n.id);
  }
}

void ParallelCoordinatesDrawing::delEdge(Graph *, const edge e) {
  if (graphProxy->getDataLocation() == EDGE) {
    removeHighlightedElt(e.id);
  }
}

void ParallelCoordinatesDrawing::treatEvent(const tlp::Event &evt) {
  const GraphEvent *gEvt = dynamic_cast<const GraphEvent *>(&evt);

  if (gEvt) {
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_AFTER_DEL_NODE:
      delNode(graph, gEvt->getNode());
      break;

    case GraphEvent::TLP_AFTER_DEL_EDGE:
      delEdge(graph, gEvt->getEdge());
      break;

    default:
      break;
    }
  }
}

void ParallelCoordinatesDrawing::removeHighlightedElt(const unsigned int dataId) {
  if (lastHighlightedElements.erase(dataId)) {
    graphProxy->removeHighlightedElement(dataId);

    if (!graphProxy->highlightedEltsSet()) {
      graphProxy->colorDataAccordingToHighlightedElts();
    }
  }
}
} // namespace tlp
