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

#include <tulip/GlQuantitativeAxis.h>
#include <tulip/GlLines.h>
#include <tulip/GlLine.h>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/tulipconf.h>
#include <tulip/GlLabel.h>
#include <tulip/GlNode.h>
#include <tulip/GlMainWidget.h>

#include <QToolTip>
#include <QMenu>

#include <algorithm>

#include "HistogramMetricMapping.h"

using namespace std;

template <typename T>
std::string getStringFromNumber(T number, unsigned int precision = 5) {
  std::ostringstream oss;
  oss.precision(precision);
  oss << number;
  return oss.str();
}

namespace tlp {

struct CoordXOrdering : public binary_function<Coord, Coord, bool> {
  bool operator()(Coord c1, Coord c2) {
    return c1.getX() < c2.getX();
  }
};

Coord *computeStraightLineIntersection(const Coord line1[2], const Coord line2[2]) {

  Coord *intersectionPoint = NULL;
  bool line1ParallelToXaxis = false;
  bool line1ParallelToYaxis = false;
  bool line2ParallelToXaxis = false;
  bool line2ParallelToYaxis = false;
  bool parallelLines = false;
  float x, y;

  // compute the equation of the first line
  // y = al1 * x + bl1
  float xal1 = line1[0].getX();
  float xbl1 = line1[1].getX();
  float yal1 = line1[0].getY();
  float ybl1 = line1[1].getY();
  float al1 = 0;
  float bl1 = 0;

  if ((xbl1 - xal1) != 0) {
    al1 = (ybl1 - yal1) / (xbl1 - xal1);
    bl1 = ybl1 - al1 * xbl1;
  }
  else {
    line1ParallelToYaxis = true;
  }

  // compute the equation of the second line
  // y = al2 * x + bl2
  float xal2 = line2[0].getX();
  float xbl2 = line2[1].getX();
  float yal2 = line2[0].getY();
  float ybl2 = line2[1].getY();
  float al2 = 0;
  float bl2 = 0;

  if ((xbl2 - xal2) != 0) {
    al2 = (ybl2 - yal2) / (xbl2 - xal2);
    bl2 = ybl2 - al2 * xbl2;
  }
  else {
    line2ParallelToYaxis = true;
  }

  if (!line1ParallelToYaxis && al1 == 0) {
    line1ParallelToXaxis = true;
  }

  if (!line2ParallelToYaxis && al2 == 0) {
    line2ParallelToXaxis = true;
  }

  // compute the intersection point of the two lines if any
  if (line1ParallelToXaxis && line2ParallelToYaxis) {
    x = xal2;
    y = yal1;
  }
  else if (line2ParallelToXaxis && line1ParallelToYaxis) {
    x = xal1;
    y = yal2;
  }
  else if (line1ParallelToXaxis && al2 != 0) {
    y = yal1;
    x = (y - bl2) / al2;
  }
  else if (line2ParallelToXaxis && al1 != 0) {
    y = yal2;
    x = (y - bl1) / al1;
  }
  else if(line1ParallelToYaxis && !line2ParallelToYaxis) {
    x = xal1;
    y = al2 * x + bl2;
  }
  else if(line2ParallelToYaxis && !line1ParallelToYaxis) {
    x = xal2;
    y = al1 * x + bl1;
  }
  else if (al1 != al2) {
    float d1 = (bl2 - bl1);
    float d2 = (al1 - al2);
    x = d1 / d2;
    y = al1 *x + bl1;
  }
  else {
    parallelLines = true;
  }

  if (!parallelLines) {
    intersectionPoint = new Coord(x, y, 0);
  }

  return intersectionPoint;
}

const float CIRCLE_RADIUS = 5.;
const float scaleWidth = 60.;

GlEditableCurve::GlEditableCurve(const Coord &start, const Coord &end, const Color &curveColor)
  : startPoint(start), endPoint(end), minPoint(startPoint), maxPoint(endPoint), curveColor(curveColor), xAxis(NULL) {
  init();
}

GlEditableCurve::GlEditableCurve(const GlEditableCurve &curve) {
  startPoint = curve.getFirstCurvePoint();
  endPoint = curve.getLastCurvePoint();
  minPoint = curve.getMinPoint();
  maxPoint = curve.getMaxPoint();
  curvePoints = curve.getCurvePoints();
  curveColor = curve.getCurveColor();
  xAxis = curve.xAxis;
  init();
}

void GlEditableCurve::init() {
  boundingBox.expand(startPoint);
  boundingBox.expand(endPoint);
  basicCircle.resizePoints(30);
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(255, 0, 0, 255));
  basicCircle.setOutlineColor(Color(0, 0, 255, 255));
}

void GlEditableCurve::draw(float lod,Camera* camera) {

  std::sort(curvePoints.begin(), curvePoints.end(), CoordXOrdering());
  camera->initGl();
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  GlLines::glDrawCurve(startPoint, curvePoints, endPoint, 2., GlLines::TLP_PLAIN, curveColor, curveColor);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  vector<Coord> curvePointsCp(curvePoints);
  curvePointsCp.insert(curvePointsCp.begin(), startPoint);
  curvePointsCp.push_back(endPoint);
  vector<Coord>::iterator it;

  Camera camera2D(camera->getScene(), false);
  camera2D.setScene(camera->getScene());

  for (it = curvePointsCp.begin() ; it != curvePointsCp.end() ; ++it) {
    Coord anchor(*it);
    camera->initGl();
    Coord tmp(camera->worldTo2DScreen(anchor));
    tmp[2] = 0;
    camera2D.initGl();
    basicCircle.set(tmp, CIRCLE_RADIUS, 0.);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    basicCircle.draw(lod, camera);

    if (xAxis != NULL) {
      int factor = 2;
      double val = xAxis->getValueForAxisPoint(anchor);
      string labelText = getStringFromNumber(val);

      if (labelText.find(".") != string::npos) {
        factor = 5;
      }

      GlLabel label(Coord(tmp.getX(), tmp.getY() + factor * CIRCLE_RADIUS, 0), Size((2*factor)*CIRCLE_RADIUS, (2*factor)*CIRCLE_RADIUS), curveColor);
      label.setText(labelText);
      label.draw(lod, camera);
    }
  }

  camera->initGl();
}

bool GlEditableCurve::pointBelong(const Coord &point) {

  bool ret = false;

  vector<Coord> curvePointsCp(curvePoints);
  curvePointsCp.insert(curvePointsCp.begin(), startPoint);
  curvePointsCp.push_back(endPoint);

  for (size_t i = 0 ; i < curvePointsCp.size() - 1 ; ++i) {
    double startToEndDist = curvePointsCp[i].dist(curvePointsCp[i+1]);
    double startToPointDist = curvePointsCp[i].dist(point);
    double pointToEndDist = point.dist(curvePointsCp[i+1]);
    ret = ret || (((startToPointDist + pointToEndDist) - startToEndDist)/startToEndDist < 1E-3);
  }

  return ret;
}

void GlEditableCurve::addCurveAnchor(const Coord &point) {
  Coord anchorPoint(point);
  anchorPoint.setZ(startPoint.getZ());
  boundingBox.expand(anchorPoint);

  if (anchorPoint != startPoint && anchorPoint != endPoint) {
    curvePoints.push_back(anchorPoint);
  }
}

Coord *GlEditableCurve::getCurveAnchorAtPointIfAny(const Coord &point, Camera *camera) {
  Coord *anchor = NULL;
  vector<Coord> curvePointsCp(curvePoints);
  curvePointsCp.insert(curvePointsCp.begin(), startPoint);
  curvePointsCp.push_back(endPoint);
  vector<Coord>::iterator it;
  camera->initGl();

  for (it = curvePointsCp.begin() ; it != curvePointsCp.end() ; ++it) {
    Coord anchorCenter(camera->worldTo2DScreen(*it));

    if (point.getX() > (anchorCenter.getX() - CIRCLE_RADIUS) && point.getX() < (anchorCenter.getX() + CIRCLE_RADIUS) &&
        point.getY() > (anchorCenter.getY() - CIRCLE_RADIUS) && point.getY() < (anchorCenter.getY() + CIRCLE_RADIUS)) {
      anchor = new Coord(*it);
      break;
    }
  }

  return anchor;
}

Coord GlEditableCurve::translateCurveAnchorToPoint(const Coord &curveAnchor, const Coord &targetPoint) {
  Coord translationVector(targetPoint - curveAnchor);
  Coord newAnchorCenter(curveAnchor + translationVector);

  if (newAnchorCenter.getX() < minPoint.getX()) {
    newAnchorCenter.setX(minPoint.getX());
  }
  else if (newAnchorCenter.getX() > maxPoint.getX()) {
    newAnchorCenter.setX(maxPoint.getX());
  }

  if (newAnchorCenter.getY() < minPoint.getY()) {
    newAnchorCenter.setY(minPoint.getY());
  }
  else if (newAnchorCenter.getY() > maxPoint.getY()) {
    newAnchorCenter.setY(maxPoint.getY());
  }

  newAnchorCenter[2] = 0;

  if (curveAnchor.getX() == startPoint.getX() &&  curveAnchor.getY() == startPoint.getY()) {
    newAnchorCenter.setX(startPoint.getX());
    startPoint = newAnchorCenter;
  }
  else if (curveAnchor.getX() == endPoint.getX() &&  curveAnchor.getY() == endPoint.getY()) {
    newAnchorCenter.setX(endPoint.getX());
    endPoint = newAnchorCenter;
  }
  else {
    std::replace(curvePoints.begin(), curvePoints.end(), curveAnchor, newAnchorCenter);
  }

  return newAnchorCenter;
}

void GlEditableCurve::removeCurveAnchor(const Coord &curveAnchor) {
  curvePoints.erase(std::remove(curvePoints.begin(), curvePoints.end(), curveAnchor), curvePoints.end());
}

float GlEditableCurve::getYCoordForX(const float xCoord) {
  Coord line1[2] = {Coord(xCoord, 0), Coord(xCoord, 10)};
  Coord line2[2];

  vector<Coord> curvePointsCp(curvePoints);
  curvePointsCp.insert(curvePointsCp.begin(), startPoint);
  curvePointsCp.push_back(endPoint);

  for (size_t i = 0 ; i < curvePointsCp.size() - 1 ; ++i) {
    if (xCoord >= curvePointsCp[i].getX() && xCoord <= curvePointsCp[i+1].getX()) {
      line2[0] = curvePointsCp[i];
      line2[1] = curvePointsCp[i+1];
      break;
    }
  }

  float ret;
  Coord *intersectionPoint = computeStraightLineIntersection(line1, line2);

  if (intersectionPoint != NULL) {
    ret = intersectionPoint->getY();
    delete intersectionPoint;
  }
  else {
    ret = line2[1].getY();
  }

  return ret;
}

void GlEditableCurve::updateSize(const Coord &newMinPoint, const Coord &newMaxPoint) {
  float oldLength = maxPoint.getX() - minPoint.getX();
  float newLength = newMaxPoint.getX() - newMinPoint.getX();

  for (size_t i = 0 ; i < curvePoints.size() ; ++i) {
    curvePoints[i].setX(newMinPoint.getX() + ((curvePoints[i].getX() - minPoint.getX()) * newLength) / oldLength);
  }

  minPoint = newMinPoint;
  maxPoint = newMaxPoint;
  startPoint.setX(minPoint.getX());
  endPoint.setX(maxPoint.getX());
}

void GlEditableCurve::resetCurve() {
  curvePoints.clear();
  startPoint = minPoint;
  endPoint = maxPoint;
}

GlSizeScale::GlSizeScale(const float minSize, const float maxSize, const Coord &baseCoord, const float length, const float thickness, const Color &color, Orientation orientation) :
  minSize(minSize), maxSize(maxSize), baseCoord(baseCoord), length(length), thickness(thickness), color(color), orientation(orientation), polyquad(new GlPolyQuad()) {

  float labelWidth = 80;
  float labelHeight = labelWidth / (1.5);

  polyquad->setOutlined(true);

  if (orientation == Vertical) {
    for (unsigned int i = 0 ; i <= 100 ; ++i) {
      float edgeLength = (i/100.) * thickness;
      float y = baseCoord.getY() + (i/100.) * length;
      polyquad->addQuadEdge(Coord(baseCoord.getX() - edgeLength / 2, y), Coord(baseCoord.getX() + edgeLength / 2, y), color);
    }

    minLabel = new GlLabel(Coord(baseCoord.getX() - labelWidth / 2 - (1./5.) * labelWidth, baseCoord.getY()), Size(labelWidth, labelHeight), color);
    maxLabel = new GlLabel(Coord(baseCoord.getX() - labelWidth / 2 - (1./5.) * labelWidth, baseCoord.getY() + length), Size(labelWidth, labelHeight), color);
    boundingBox.expand(Coord(baseCoord.getX() - thickness / 2, baseCoord.getY()));
    boundingBox.expand(Coord(baseCoord.getX() + thickness / 2, baseCoord.getY() + length));
  }
  else {
    for (unsigned int i = 0 ; i <= 100 ; ++i) {
      float edgeLength = (i/100.) * thickness;
      float x = baseCoord.getX() + (i/100.) * length;
      polyquad->addQuadEdge(Coord(x, baseCoord.getY() - edgeLength / 2), Coord(x, baseCoord.getY() - edgeLength / 2), color);
    }

    minLabel = new GlLabel(Coord(baseCoord.getX(), baseCoord.getY() - labelHeight / 2 - (1./5.) * labelHeight), Size(labelWidth, labelHeight), color);
    maxLabel = new GlLabel(Coord(baseCoord.getX() + length, baseCoord.getY() - labelHeight / 2 - (1./5.) * labelHeight), Size(labelWidth, labelHeight), color);
    boundingBox.expand(Coord(baseCoord.getX(), baseCoord.getY() - thickness / 2));
    boundingBox.expand(Coord(baseCoord.getX() + length, baseCoord.getY() + thickness / 2));
  }

}

GlSizeScale::~GlSizeScale() {
  delete polyquad;
  delete minLabel;
  delete maxLabel;
}

float GlSizeScale::getSizeAtPos(const Coord &pos) {
  if (orientation == Vertical) {
    if (pos.getY() < baseCoord.getY()) {
      return minSize;
    }
    else if (pos.getY() > baseCoord.getY() + length) {
      return maxSize;
    }
    else {
      float ratio = (pos.getY() - baseCoord.getY()) / length;
      return minSize + ratio * (maxSize - minSize);
    }
  }
  else {
    if (pos.getX() < baseCoord.getX()) {
      return minSize;
    }
    else if (pos.getX() > baseCoord.getX() + length) {
      return maxSize;
    }
    else {
      float ratio = (pos.getX() - baseCoord.getX()) / length;
      return minSize + ratio * (maxSize - minSize);
    }
  }
}

void GlSizeScale::draw(float lod, Camera* camera) {
  minLabel->setText(getStringFromNumber(minSize));
  maxLabel->setText(getStringFromNumber(maxSize));
  minLabel->setColor(color);
  maxLabel->setColor(color);
  polyquad->setColor(color);
  polyquad->setOutlineColor(color);
  polyquad->draw(lod, camera);
  minLabel->draw(lod, camera);
  maxLabel->draw(lod, camera);
}

void GlSizeScale::translate(const Coord &move) {
  polyquad->translate(move);
  minLabel->translate(move);
  maxLabel->translate(move);
  baseCoord += move;
}


GlGlyphScale::GlGlyphScale(const Coord &baseCoord, const float length, Orientation orientation) :
  glyphGraph(newGraph()), glyphGraphInputData(new GlGraphInputData(glyphGraph, &glyphGraphRenderingParameters)), baseCoord(baseCoord), length(length), orientation(orientation) {
  glyphGraphLayout = glyphGraph->getProperty<LayoutProperty>("viewLayout");
  glyphGraphSize = glyphGraph->getProperty<SizeProperty>("viewSize");
  glyphGraphColor = glyphGraph->getProperty<ColorProperty>("viewColor");
  glyphGraphShape = glyphGraph->getProperty<IntegerProperty>("viewShape");
}

GlGlyphScale::~GlGlyphScale() {
  delete glyphGraph;
  delete glyphGraphInputData;
}

void GlGlyphScale::setGlyphsList(const vector<int> &glyphsList) {

  BooleanProperty *wholeGlyphGraphSelec = new BooleanProperty(glyphGraph);
  wholeGlyphGraphSelec->setAllNodeValue(true);
  wholeGlyphGraphSelec->setAllEdgeValue(true);
  removeFromGraph(glyphGraph, wholeGlyphGraphSelec);
  delete wholeGlyphGraphSelec;

  glyphScaleMap.clear();

  for (size_t i = 0 ; i < glyphsList.size() ; ++i) {
    node n = glyphGraph->addNode();
    glyphGraphShape->setNodeValue(n, glyphsList[i]);
  }

  size = length / glyphsList.size();
  glyphGraphSize->setAllNodeValue(Size(size, size, size));
  glyphGraphColor->setAllNodeValue(Color(255, 0, 0));

  int i = 0;

  Iterator<node> *nodesIt = glyphGraph->getNodes();

  if (orientation == Vertical) {
    float xCenter = baseCoord.getX() - size / 2;

    while (nodesIt->hasNext()) {
      node n = nodesIt->next();
      glyphGraphLayout->setNodeValue(n, Coord(xCenter, baseCoord.getY() + i * size + size / 2));
      int oldI = i++;
      glyphScaleMap[make_pair(baseCoord.getY() + oldI * size, baseCoord.getY() + i * size)] = glyphGraphShape->getNodeValue(n);
    }

    boundingBox[0] = Coord(baseCoord.getX() - size, baseCoord.getY());
    boundingBox[1] = Coord(baseCoord.getX() + size, baseCoord.getY() + length);
  }
  else {
    float yCenter = baseCoord.getY() - size / 2;

    while (nodesIt->hasNext()) {
      node n = nodesIt->next();
      glyphGraphLayout->setNodeValue(n, Coord(baseCoord.getX() + i++ * size + size / 2, yCenter));
      int oldI = i++;
      glyphScaleMap[make_pair(baseCoord.getX() + oldI * size, baseCoord.getX() + i * size)] = glyphGraphShape->getNodeValue(n);
    }

    boundingBox[0] = Coord(baseCoord.getX(), baseCoord.getY() - size);
    boundingBox[1] = Coord(baseCoord.getX() + length, baseCoord.getY() + size);
  }

  delete nodesIt;
}

int GlGlyphScale::getGlyphAtPos(const Coord &pos) {
  if (orientation == Vertical) {
    if (pos.getY() < baseCoord.getY()) {
      return glyphScaleMap[make_pair(baseCoord.getY(), baseCoord.getY() + size)];
    }
    else if (pos.getY() > baseCoord.getY() + length) {
      return glyphScaleMap[make_pair(baseCoord.getY() + length - size, baseCoord.getY() + length)];
    }
    else {
      map<pair<float, float>, int>::iterator it;

      for (it = glyphScaleMap.begin() ; it != glyphScaleMap.end() ; ++it) {
        if (pos.getY() >= (*it).first.first && pos.getY() < (*it).first.second) {
          return (*it).second;
        }
      }
    }
  }
  else {
    if (pos.getX() < baseCoord.getX()) {
      return glyphScaleMap[make_pair(baseCoord.getX(), baseCoord.getX() + size)];
    }
    else if (pos.getX() > baseCoord.getX() + length) {
      return glyphScaleMap[make_pair(baseCoord.getX() + length - size, baseCoord.getX() + length)];
    }
    else {
      map<pair<float, float>, int>::iterator it;

      for (it = glyphScaleMap.begin() ; it != glyphScaleMap.end() ; ++it) {
        if (pos.getX() >= (*it).first.first && pos.getX() < (*it).first.second) {
          return (*it).second;
        }
      }
    }
  }

  return 0;
}

void GlGlyphScale::draw(float lod, Camera* camera) {
  (void) lod;
  node n;
  GlNode glNode(0);
  glEnable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  forEach(n, glyphGraph->getNodes()) {
    glNode.id = n.id;
    glNode.draw(30, glyphGraphInputData, camera);
  }
}

void GlGlyphScale::translate(const Coord &move) {
  node n;
  forEach(n, glyphGraph->getNodes()) {
    Coord currentNodeCoord(glyphGraphLayout->getNodeValue(n));
    glyphGraphLayout->setNodeValue(n, currentNodeCoord + move);
  }
}

HistogramMetricMapping::HistogramMetricMapping() :
  histoView(NULL), curve(NULL), curveDragStarted(false), selectedAnchor(NULL),
  colorScale(new ColorScale()), glColorScale(NULL), glSizeScale(NULL), glGlyphScale(NULL),
  colorScaleConfigDialog(NULL), sizeScaleConfigDialog(NULL), glyphScaleConfigDialog(NULL), lastXAxisLength(0), histoXAxis(NULL),
  mappinqPolyQuad(NULL), scaleAxisOffset(0), glyphMappingGraph(newGraph()), glyphMappingGraphInputData(new GlGraphInputData(glyphMappingGraph, &glyphMapppingGraphRenderingParameters)),
  mappingType(VIEWCOLOR_MAPPING), popupMenu(NULL), colorMappingMenu(NULL), viewColorMappingAction(NULL), viewBorderColorMappingAction(NULL), sizeMapping(NULL), glyphMapping(NULL) {
}

HistogramMetricMapping::HistogramMetricMapping(const HistogramMetricMapping &histoMetricMapping)
  : curve(NULL), curveDragStarted(false), selectedAnchor(NULL),
    colorScale(NULL), glColorScale(NULL), glSizeScale(NULL), glGlyphScale(NULL),
    histoXAxis(NULL), mappinqPolyQuad(NULL), scaleAxisOffset(0), glyphMappingGraph(newGraph()),
    glyphMappingGraphInputData(new GlGraphInputData(glyphMappingGraph, &glyphMapppingGraphRenderingParameters)),
    popupMenu(NULL), colorMappingMenu(NULL), viewColorMappingAction(NULL),
    viewBorderColorMappingAction(NULL), sizeMapping(NULL), glyphMapping(NULL) {

  GlEditableCurve *lastCurve = histoMetricMapping.curve;

  if (lastCurve != NULL) {
    curve = new GlEditableCurve(*lastCurve);
  }

  Coord *lastSelectedAnchor = histoMetricMapping.selectedAnchor;

  if (lastSelectedAnchor != NULL) {
    selectedAnchor = new Coord(*lastSelectedAnchor);
  }

  ColorScale *lastColorScale = histoMetricMapping.colorScale;

  if (lastColorScale != NULL) {
    colorScale = new ColorScale(*lastColorScale);
  }

  colorScaleConfigDialog = histoMetricMapping.colorScaleConfigDialog;
  sizeScaleConfigDialog = histoMetricMapping.sizeScaleConfigDialog;
  glyphScaleConfigDialog = histoMetricMapping.glyphScaleConfigDialog;

  if (colorScaleConfigDialog != NULL) {
    colorScaleConfigDialog->setColorScale(colorScale);
  }

  GlPolyQuad *lastColorMappingPolyQuad = histoMetricMapping.mappinqPolyQuad;

  if (lastColorMappingPolyQuad != NULL) {
    mappinqPolyQuad = new GlPolyQuad(*lastColorMappingPolyQuad);
  }

  lastXAxisLength = histoMetricMapping.lastXAxisLength;
  histoView = histoMetricMapping.histoView;
  mappingType = histoMetricMapping.mappingType;
  curveShapeForMapping = histoMetricMapping.curveShapeForMapping;
}

HistogramMetricMapping::~HistogramMetricMapping() {
  if (glColorScale != NULL) {
    delete glColorScale;
  }

  if (glSizeScale != NULL) {
    delete glSizeScale;
  }

  if (glGlyphScale != NULL) {
    delete glGlyphScale;
  }

  if (colorScale != NULL) {
    delete colorScale;
  }

  if (mappinqPolyQuad != NULL) {
    delete mappinqPolyQuad;
  }

  if (popupMenu != NULL) {
    delete popupMenu;
  }

  delete glyphMappingGraph;
  delete glyphMappingGraphInputData;
}

bool HistogramMetricMapping::compute(GlMainWidget *) {
  initInteractor();
  return false;
}

void HistogramMetricMapping::viewChanged(View *view) {
  if (view == NULL) {
    histoView = NULL;
    return;
  }

  histoView = static_cast<HistogramView *>(view);
  initInteractor();
  popupMenu = new QMenu();
  popupMenu->addAction("MappingType")->setEnabled(false);
  popupMenu->addSeparator();
  colorMappingMenu = new QMenu("Color");
  viewColorMappingAction = colorMappingMenu->addAction("viewColor");
  viewBorderColorMappingAction = colorMappingMenu->addAction("viewBorderColor");
  popupMenu->addMenu(colorMappingMenu);
  sizeMapping = popupMenu->addAction("Size");
  glyphMapping = popupMenu->addAction("Glyph");
  viewColorMappingAction->setCheckable(true);
  viewColorMappingAction->setChecked(true);
  viewBorderColorMappingAction->setCheckable(true);
  sizeMapping->setCheckable(true);
  glyphMapping->setCheckable(true);
  histoView->refresh();
}

void HistogramMetricMapping::initInteractor() {
  if(!histoView->getDetailedHistogram())
    return;

  if (colorScaleConfigDialog == NULL) {
    colorScaleConfigDialog = new ColorScaleConfigDialog(colorScale, histoView->getGlMainWidget());
    *colorScale=colorScaleConfigDialog->getColorScale();
    colorScale->setColorMapTransparency(200);
  }

  if (sizeScaleConfigDialog == NULL) {
    sizeScaleConfigDialog = new SizeScaleConfigDialog(histoView->getGlMainWidget());
  }

  if (glyphScaleConfigDialog == NULL) {
    glyphScaleConfigDialog = new GlyphScaleConfigDialog(histoView->getGlMainWidget());
  }

  histoXAxis = histoView->getDetailedHistogram()->getXAxis();
  GlQuantitativeAxis *histoYAxis = histoView->getDetailedHistogram()->getYAxis();

  if (histoXAxis == NULL || histoYAxis == NULL) {
    return;
  }

  Coord curveStartPoint(histoXAxis->getAxisBaseCoord());

  if (curve == NULL || lastXAxisLength != histoXAxis->getAxisLength()) {
    Coord curveEndPoint(histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength(), histoYAxis->getAxisBaseCoord().getY() + histoYAxis->getAxisLength(), 0);

    if (curve != NULL) {
      curve->updateSize(curveStartPoint, curveEndPoint);
      updateMapping(histoXAxis, histoView->getDetailedHistogram()->getNbHistogramBins());
    }
    else {
      curve = new GlEditableCurve(curveStartPoint, curveEndPoint, histoXAxis->getAxisColor());
    }

    lastXAxisLength = histoXAxis->getAxisLength();
  }

  curve->setCurveColor(histoXAxis->getAxisColor());

  if (!histoView->getDetailedHistogram()->uniformQuantificationHistogram()) {
    curve->setXAxisScale(histoXAxis);
  }
  else {
    curve->setXAxisScale(NULL);
  }

  scaleAxisOffset = histoYAxis->getMaxLabelWidth() + scaleWidth;


  if (glColorScale == NULL) {
    glColorScale = new GlColorScale(colorScale, curveStartPoint - Coord(scaleAxisOffset,0,0), histoYAxis->getAxisLength(), scaleWidth, GlColorScale::Vertical);
  }
  else {
    Coord lastBaseCoord(glColorScale->getBaseCoord());
    Coord newBaseCoord(curveStartPoint - Coord(histoYAxis->getMaxLabelWidth() + scaleWidth,0,0));

    if (lastBaseCoord != newBaseCoord) {
      glColorScale->translate(newBaseCoord - lastBaseCoord);
    }
  }

  if (glSizeScale == NULL) {
    glSizeScale = new GlSizeScale(sizeScaleConfigDialog->getMinSize(), sizeScaleConfigDialog->getMaxSize(), curveStartPoint - Coord(scaleAxisOffset,0,0), histoYAxis->getAxisLength(), scaleWidth, Color(0,0,0), GlSizeScale::Vertical);
  }
  else {
    Coord lastBaseCoord(glSizeScale->getBaseCoord());
    Coord newBaseCoord(curveStartPoint - Coord(histoYAxis->getMaxLabelWidth() + scaleWidth,0,0));

    if (lastBaseCoord != newBaseCoord) {
      glSizeScale->translate(newBaseCoord - lastBaseCoord);
    }

    glSizeScale->setColor(histoXAxis->getAxisColor());
  }

  if (glGlyphScale == NULL) {
    glGlyphScale = new GlGlyphScale(curveStartPoint - Coord(histoYAxis->getMaxLabelWidth(),0,0), histoYAxis->getAxisLength(), GlGlyphScale::Vertical);
    glGlyphScale->setGlyphsList(glyphScaleConfigDialog->getSelectedGlyphsId());
  }
  else {
    Coord lastBaseCoord(glGlyphScale->getBaseCoord());
    Coord newBaseCoord(curveStartPoint - Coord(histoYAxis->getMaxLabelWidth(),0,0));

    if (lastBaseCoord != newBaseCoord) {
      glGlyphScale->translate(newBaseCoord - lastBaseCoord);
    }
  }

  updateMapping(histoXAxis, histoView->getDetailedHistogram()->getNbHistogramBins());
}

bool HistogramMetricMapping::eventFilter(QObject *widget, QEvent *e) {

  if(!dynamic_cast<QMouseEvent *>(e))
    return false;

  bool ret  = false;

  GlMainWidget *glWidget = (GlMainWidget *) widget;

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  Observable::holdObservers();

  initInteractor();

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *me = (QMouseEvent *) e;
    int x = glWidget->width() - me->x();
    int y = me->y();
    Coord screenCoords((double) x, (double) y, 0);
    Coord sceneCoords(glWidget->getScene()->getGraphCamera().screenTo3DWorld(screenCoords));

    if (!curveDragStarted) {
      Coord *anchor = curve->getCurveAnchorAtPointIfAny(Coord(me->x(), glWidget->height() - me->y(), 0), &glWidget->getScene()->getLayer("Main")->getCamera());
      bool pointerColorScale = pointerUnderScale(sceneCoords);

      if (selectedAnchor != NULL) {
        delete selectedAnchor;
      }

      selectedAnchor = anchor;

      if (selectedAnchor != NULL) {
        glWidget->setCursor(QCursor(Qt::SizeAllCursor));
      }
      else if (pointerColorScale) {
        glWidget->setCursor(QCursor(Qt::WhatsThisCursor));
      }
      else {
        glWidget->setCursor(QCursor(Qt::ArrowCursor));
      }
    }
    else if (selectedAnchor != NULL) {
      Coord newAnchorCord(curve->translateCurveAnchorToPoint(*selectedAnchor, sceneCoords));
      delete selectedAnchor;
      selectedAnchor = new Coord(newAnchorCord);
      updateMapping(histoXAxis, histoView->getDetailedHistogram()->getNbHistogramBins());
    }

    histoView->refresh();
    ret = true;
  }
  else if (e->type() == QEvent::MouseButtonDblClick) {
    if (selectedAnchor != NULL) {
      curve->removeCurveAnchor(*selectedAnchor);
      updateGraphWithMapping(histoView->histoGraph(), histoView->getDetailedHistogram()->getHistogramLayout());
      updateMapping(histoXAxis, histoView->getDetailedHistogram()->getNbHistogramBins());
      delete selectedAnchor;
      selectedAnchor = NULL;
    }
    else {
      QMouseEvent *me = (QMouseEvent *) e;
      int x = glWidget->width() - me->x();
      int y = me->y();
      Coord screenCoords((double) x, (double) y, 0);
      Coord sceneCoords(glWidget->getScene()->getGraphCamera().screenTo3DWorld(screenCoords));

      if (curve->pointBelong(sceneCoords)) {
        curve->addCurveAnchor(sceneCoords);
      }
      else if (pointerUnderScale(sceneCoords)) {
        bool updateMappingNeeded = false;

        if ((mappingType == VIEWCOLOR_MAPPING || mappingType == VIEWBORDERCOLOR_MAPPING) && colorScaleConfigDialog->exec() == QDialog::Accepted) {
          updateMappingNeeded = true;
        }
        else if (mappingType == SIZE_MAPPING && sizeScaleConfigDialog->exec() == QDialog::Accepted) {
          glSizeScale->setMinSize(sizeScaleConfigDialog->getMinSize());
          glSizeScale->setMaxSize(sizeScaleConfigDialog->getMaxSize());
          updateMappingNeeded = true;
        }
        else if (mappingType == GLYPH_MAPPING && glyphScaleConfigDialog->exec() == QDialog::Accepted) {
          glGlyphScale->setGlyphsList(glyphScaleConfigDialog->getSelectedGlyphsId());
          updateMappingNeeded = true;
        }

        if (updateMappingNeeded) {
          updateGraphWithMapping(histoView->histoGraph(), histoView->getDetailedHistogram()->getHistogramLayout());
          updateMapping(histoXAxis, histoView->getDetailedHistogram()->getNbHistogramBins());
        }
      }
    }

    ret =  true;
  }
  else if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *me = (QMouseEvent *) e;

    if (me->buttons() == Qt::LeftButton && selectedAnchor != NULL) {
      curveDragStarted = true;
    }
    else if (me->buttons() == Qt::RightButton) {
      int x = glWidget->width() - me->x();
      int y = me->y();
      Coord screenCoords((double) x, (double) y, 0);
      Coord sceneCoords(glWidget->getScene()->getGraphCamera().screenTo3DWorld(screenCoords));

      if (pointerUnderScale(sceneCoords)) {
        if (mappingType == VIEWCOLOR_MAPPING) {
          viewColorMappingAction->setChecked(true);
          viewBorderColorMappingAction->setChecked(false);
          sizeMapping->setChecked(false);
          glyphMapping->setChecked(false);
        }
        else if (mappingType == VIEWBORDERCOLOR_MAPPING) {
          viewColorMappingAction->setChecked(false);
          viewBorderColorMappingAction->setChecked(true);
          sizeMapping->setChecked(false);
          glyphMapping->setChecked(false);
        }
        else if (mappingType == SIZE_MAPPING) {
          viewColorMappingAction->setChecked(false);
          viewBorderColorMappingAction->setChecked(false);
          sizeMapping->setChecked(true);
          glyphMapping->setChecked(false);
        }
        else {
          viewColorMappingAction->setChecked(false);
          viewBorderColorMappingAction->setChecked(false);
          sizeMapping->setChecked(false);
          glyphMapping->setChecked(true);
        }

        QAction *clickedAction = popupMenu->exec(me->globalPos());
        MappingType oldMappingType = mappingType;

        if (clickedAction == viewColorMappingAction) {
          mappingType = VIEWCOLOR_MAPPING;
        }
        else if (clickedAction == viewBorderColorMappingAction) {
          mappingType = VIEWBORDERCOLOR_MAPPING;
        }
        else if (clickedAction == sizeMapping) {
          mappingType = SIZE_MAPPING;
        }
        else {
          mappingType = GLYPH_MAPPING;
        }

        if (mappingType != oldMappingType) {
          vector<Coord> oldCurvePoints;
          oldCurvePoints.push_back(curve->getFirstCurvePoint());
          vector<Coord> intermediateCurvePoints = curve->getCurvePoints();
          oldCurvePoints.insert(oldCurvePoints.end(), intermediateCurvePoints.begin(), intermediateCurvePoints.end());
          oldCurvePoints.push_back(curve->getLastCurvePoint());
          curveShapeForMapping[oldMappingType] = oldCurvePoints;

          if (curveShapeForMapping.find(mappingType) != curveShapeForMapping.end()) {
            vector<Coord> curvePoints(curveShapeForMapping[mappingType]);
            curve->setCurveStartPoint(curvePoints[0]);
            curve->setLastCurvePoint(curvePoints[curvePoints.size() - 1]);
            curvePoints.erase(curvePoints.begin());
            curvePoints.pop_back();
            curve->setCurvePoints(curvePoints);
          }
          else {
            curve->resetCurve();
          }
        }

        glWidget->draw();
      }
    }

    ret = true;
  }
  else if (e->type() == QEvent::MouseButtonRelease) {
    if (curveDragStarted) {
      updateGraphWithMapping(histoView->histoGraph(), histoView->getDetailedHistogram()->getHistogramLayout());
      updateMapping(histoXAxis, histoView->getDetailedHistogram()->getNbHistogramBins());
      curveDragStarted = false;
    }

    histoView->refresh();
    ret = true;
  }

  Observable::unholdObservers();

  return ret;
}

bool HistogramMetricMapping::draw(GlMainWidget *glMainWidget) {
  if (curve != NULL) {
    Camera &camera = glMainWidget->getScene()->getLayer("Main")->getCamera();
    camera.initGl();
    float xStart = 0, yEnd = 0;
    glEnable(GL_BLEND);

    if ((mappingType == VIEWCOLOR_MAPPING || mappingType == VIEWBORDERCOLOR_MAPPING)) {
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColorScale->draw(0, &camera);

      if (mappinqPolyQuad != NULL) {
        mappinqPolyQuad->draw(0, &camera);
      }

      xStart = glColorScale->getBaseCoord().getX() + (glColorScale->getThickness() / 2.);
      yEnd = glColorScale->getBaseCoord().getY();
    }
    else if (mappingType == SIZE_MAPPING) {
      glSizeScale->draw(0, &camera);

      if (mappinqPolyQuad != NULL) {
        mappinqPolyQuad->draw(0, &camera);
      }

      xStart = glSizeScale->getBaseCoord().getX() + (glSizeScale->getThickness() / 2.);
      yEnd = glSizeScale->getBaseCoord().getY();
    }
    else {
      glGlyphScale->draw(0, &camera);
      GlNode glNode(0);
      node n;
      forEach(n, glyphMappingGraph->getNodes()) {
        glNode.id = n.id;
        glNode.draw(30, glyphMappingGraphInputData, &camera);
      }
      xStart = glGlyphScale->getBaseCoord().getX();
      yEnd = glGlyphScale->getBaseCoord().getY();
    }

    Coord startPoint(curve->getFirstCurvePoint());
    Coord endPoint(curve->getLastCurvePoint());
    vector<Coord> curvePoints(curve->getCurvePoints());

    vector<Coord> curvePointsCp(curvePoints);
    curvePointsCp.insert(curvePointsCp.begin(), startPoint);
    curvePointsCp.push_back(endPoint);

    Color lineColor(histoXAxis->getAxisColor());

    glDisable(GL_LIGHTING);

    for (size_t i = 0 ; i < curvePointsCp.size() ; ++i) {
      Coord startLine(xStart, curvePointsCp[i].getY(), 0);
      GlLines::glDrawLine(startLine, curvePointsCp[i], 0.5, GlLines::TLP_DASHED, lineColor, lineColor);
      Coord endLine(curvePointsCp[i].getX(), yEnd);
      GlLines::glDrawLine(curvePointsCp[i], endLine, 0.5, GlLines::TLP_DASHED, lineColor, lineColor);
    }

    glEnable(GL_LIGHTING);

    curve->draw(30, &camera);
  }

  return true;
}

bool HistogramMetricMapping::pointerUnderScale(const Coord &sceneCoords) {
  GlSimpleEntity *scale = NULL;

  if (mappingType == VIEWCOLOR_MAPPING || mappingType == VIEWBORDERCOLOR_MAPPING) {
    scale = glColorScale;
  }
  else if (mappingType == SIZE_MAPPING) {
    scale = glSizeScale;
  }
  else {
    scale = glGlyphScale;
  }

  if (scale == NULL) {
    return false;
  }
  else {
    BoundingBox scaleBB(scale->getBoundingBox());
    return (sceneCoords.getX() >= scaleBB[0][0] && sceneCoords.getX() <= scaleBB[1][0] &&
            sceneCoords.getY() >= scaleBB[0][1] && sceneCoords.getY() <= scaleBB[1][1]);
  }
}

void HistogramMetricMapping::updateGraphWithMapping(Graph *graph, LayoutProperty *histogramLayout) {

  *colorScale=colorScaleConfigDialog->getColorScale();
  colorScale->setColorMapTransparency(200);
  glColorScale->setColorScale(colorScale);

  if ((mappingType == VIEWCOLOR_MAPPING || mappingType == VIEWBORDERCOLOR_MAPPING) && !colorScale->colorScaleInitialized()) {
    return;
  }

  node n;
  ColorProperty *graphColors = NULL;

  if (mappingType == VIEWCOLOR_MAPPING) {
    graphColors = graph->getProperty<ColorProperty>("viewColor");
  }
  else if (mappingType == VIEWBORDERCOLOR_MAPPING) {
    graphColors = graph->getProperty<ColorProperty>("viewBorderColor");
  }

  SizeProperty *graphSizes = graph->getProperty<SizeProperty>("viewSize");
  DoubleProperty *graphBorderSizes = graph->getProperty<DoubleProperty>("viewBorderWidth");
  IntegerProperty *graphShapes = graph->getProperty<IntegerProperty>("viewShape");

  if (histoView->getDataLocation() == NODE) {
    Iterator<node> *nodesIt = graph->getNodes();

    while (nodesIt->hasNext()) {
      node n = nodesIt->next();
      Coord nodeHistoCoord(histogramLayout->getNodeValue(n));
      float yCurve = curve->getYCoordForX(nodeHistoCoord.getX());

      if (mappingType == VIEWCOLOR_MAPPING || mappingType == VIEWBORDERCOLOR_MAPPING) {
        Color newNodeColor(glColorScale->getColorAtPos(Coord(0,yCurve,0)));
        graphColors->setNodeValue(n, newNodeColor);
      }
      else if (mappingType == SIZE_MAPPING) {
        float newNodeSize = glSizeScale->getSizeAtPos(Coord(0,yCurve,0));

        if (sizeScaleConfigDialog->doMappingOnViewSize()) {
          Size newSize(newNodeSize, newNodeSize, newNodeSize);
          Size currentSize(graphSizes->getNodeValue(n));

          if (!sizeScaleConfigDialog->applySizeMappingOnX()) {
            newSize[0] = currentSize[0];
          }

          if (!sizeScaleConfigDialog->applySizeMappingOnY()) {
            newSize[1] = currentSize[1];
          }

          if (!sizeScaleConfigDialog->applySizeMappingOnZ()) {
            newSize[2] = currentSize[2];
          }

          graphSizes->setNodeValue(n, newSize);
        }
        else {
          graphBorderSizes->setNodeValue(n, newNodeSize);
        }
      }
      else {
        int newGlyphId = glGlyphScale->getGlyphAtPos(Coord(0, yCurve, 0));
        graphShapes->setNodeValue(n, newGlyphId);
      }
    }

    delete nodesIt;
  }
  else {
    Iterator<edge> *edgesIt = graph->getEdges();

    while (edgesIt->hasNext()) {
      edge e = edgesIt->next();
      Coord edgeHistoCoord(histogramLayout->getEdgeValue(e)[0]);
      float yCurve = curve->getYCoordForX(edgeHistoCoord.getX());

      if (mappingType == VIEWCOLOR_MAPPING) {
        Color newEdgeColor(glColorScale->getColorAtPos(Coord(0,yCurve,0)));
        graphColors->setEdgeValue(e, newEdgeColor);
      }
    }
  }
}

void HistogramMetricMapping::updateMapping(GlQuantitativeAxis *histoXAxis, unsigned int nbHistogramBins) {
  if (mappinqPolyQuad != NULL) {
    delete mappinqPolyQuad;
    mappinqPolyQuad = NULL;
  }

  if (mappingType == VIEWCOLOR_MAPPING || mappingType == VIEWBORDERCOLOR_MAPPING || mappingType == SIZE_MAPPING) {
    mappinqPolyQuad = new GlPolyQuad();
  }

  float increment = histoXAxis->getAxisLength() / (nbHistogramBins - 1);
  float offset = (histoXAxis->getAxisGradsWidth() / 2.) + histoXAxis->getLabelHeight();

  if (mappingType == VIEWCOLOR_MAPPING || mappingType == VIEWBORDERCOLOR_MAPPING) {

    if (!colorScale->colorScaleInitialized()) {
      mappinqPolyQuad->setOutlined(true);
      mappinqPolyQuad->setOutlineWidth(2);
    }

    Color color;

    for (float x = histoXAxis->getAxisBaseCoord().getX() ; x <= histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength() ; x += increment) {
      float yCurve = curve->getYCoordForX(x);

      if (yCurve < 0) yCurve = 0;

      color = glColorScale->getColorAtPos(Coord(0,yCurve,0));
      mappinqPolyQuad->addQuadEdge(Coord(x, histoXAxis->getAxisBaseCoord().getY() - offset - scaleWidth, 0),
                                   Coord(x, histoXAxis->getAxisBaseCoord().getY() - offset, 0),
                                   color);
    }

    mappinqPolyQuad->addQuadEdge(Coord(histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength(), histoXAxis->getAxisBaseCoord().getY() - offset - scaleWidth, 0),
                                 Coord(histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength(), histoXAxis->getAxisBaseCoord().getY() - offset, 0),
                                 color);
  }
  else if (mappingType == SIZE_MAPPING) {
    Color color(histoXAxis->getAxisColor());
    mappinqPolyQuad->setOutlined(true);
    mappinqPolyQuad->setOutlineColor(histoXAxis->getAxisColor());

    for (float x = histoXAxis->getAxisBaseCoord().getX() ; x <= histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength() ; x += increment) {
      float yCurve = curve->getYCoordForX(x);

      if (yCurve < 0) yCurve = 0;

      float minSize = glSizeScale->getMinSize();
      float maxSize = glSizeScale->getMaxSize();
      float size = glSizeScale->getSizeAtPos(Coord(0, yCurve, 0));
      float offset2 = (scaleWidth - (((size - minSize) * scaleWidth) / (maxSize))) / 2.;
      mappinqPolyQuad->addQuadEdge(Coord(x, histoXAxis->getAxisBaseCoord().getY() - offset - scaleWidth + offset2, 0),
                                   Coord(x, histoXAxis->getAxisBaseCoord().getY() - offset - offset2, 0), color);
    }
  }
  else {
    BooleanProperty *wholeGlyphMappingGraphSelec = new BooleanProperty(glyphMappingGraph);
    wholeGlyphMappingGraphSelec->setAllNodeValue(true);
    wholeGlyphMappingGraphSelec->setAllEdgeValue(true);
    removeFromGraph(glyphMappingGraph, wholeGlyphMappingGraphSelec);
    delete wholeGlyphMappingGraphSelec;
    IntegerProperty *glyphGraphShape = glyphMappingGraph->getProperty<IntegerProperty>("viewShape");
    LayoutProperty *glyphGraphLayout = glyphMappingGraph->getProperty<LayoutProperty>("viewLayout");
    SizeProperty *glyphGraphSize = glyphMappingGraph->getProperty<SizeProperty>("viewSize");
    ColorProperty *glyphGraphColor = glyphMappingGraph->getProperty<ColorProperty>("viewColor");
    glyphGraphSize->setAllNodeValue(Size(increment, increment, increment));
    glyphGraphColor->setAllNodeValue(Color(255,0,0));

    for (float x = histoXAxis->getAxisBaseCoord().getX() ; x <= histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength() ; x += increment) {
      if (x+increment/2 > histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength()) {
        break;
      }

      node n = glyphMappingGraph->addNode();
      float yCurve = curve->getYCoordForX(x+increment/2);

      if (yCurve < 0) yCurve = 0;

      int glyphId = glGlyphScale->getGlyphAtPos(Coord(0, yCurve, 0));
      glyphGraphShape->setNodeValue(n, glyphId);
      glyphGraphLayout->setNodeValue(n, Coord(x+increment/2, histoXAxis->getAxisBaseCoord().getY() - offset - increment/2, 0));
    }
  }
}

}
