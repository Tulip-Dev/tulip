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

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <tulip/GlQuantitativeAxis.h>
#include <tulip/GlLines.h>
#include <tulip/GlLine.h>
#include <tulip/TulipPlugin.h>
#include <tulip/tulipconf.h>
#include <tulip/GlQuad.h>

#include <algorithm>

#include "HistogramColorMapping.h"

using namespace std;

namespace tlp {

struct CoordXOrdering: public binary_function<Coord, Coord, bool> {
  bool operator()(Coord c1, Coord c2) {
    return c1.getX() < c2.getX();
  }
};

Coord *computeStraightLineIntersection(const Coord line1[2],
                                       const Coord line2[2]) {

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
  float al1;
  float bl1;

  if ((xbl1 - xal1) != 0) {
    al1 = (ybl1 - yal1) / (xbl1 - xal1);
    bl1 = ybl1 - al1 * xbl1;
  }
  else {
    line1ParallelToXaxis = true;
  }

  // compute the equation of the second line
  // y = al2 * x + bl2
  float xal2 = line2[0].getX();
  float xbl2 = line2[1].getX();
  float yal2 = line2[0].getY();
  float ybl2 = line2[1].getY();
  float al2;
  float bl2;

  if ((xbl2 - xal2) != 0) {
    al2 = (ybl2 - yal2) / (xbl2 - xal2);
    bl2 = ybl2 - al2 * xbl2;
  }
  else {
    line2ParallelToXaxis = true;
  }

  if (!line1ParallelToXaxis && al1 == 0) {
    line1ParallelToYaxis = true;
  }

  if (!line2ParallelToXaxis && al2 == 0) {
    line2ParallelToYaxis = true;
  }

  // compute the intersection point of the two lines if any
  if (line1ParallelToXaxis && line2ParallelToYaxis) {
    x = xal1;
    y = yal2;
  }
  else if (line2ParallelToXaxis && line1ParallelToYaxis) {
    x = xal2;
    y = yal1;
  }
  else if (line1ParallelToXaxis && !line2ParallelToXaxis) {
    x = xal1;
    y = al2 * x + bl2;
  }
  else if (line2ParallelToXaxis && !line1ParallelToXaxis) {
    x = xal2;
    y = al1 * x + bl1;
  }
  else if ((!line1ParallelToXaxis && !line2ParallelToXaxis)
           && (!line1ParallelToYaxis && !line2ParallelToYaxis)) {

    float d1 = (bl2 - bl1);
    float d2 = (al1 - al2);

    if (d2 != 0) {
      x = d1 / d2;
      y = al1 * x + bl1;
    }
    else {
      parallelLines = true;
    }
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

GlEditableCurve::GlEditableCurve(const Coord &start, const Coord &end,
                                 const Color &curveColor) :
  startPoint(start), endPoint(end), minPoint(startPoint), maxPoint(endPoint),
  curveColor(curveColor) {
  init();
}

GlEditableCurve::GlEditableCurve(const GlEditableCurve &curve) {
  startPoint = curve.getFirstCurvePoint();
  endPoint = curve.getLastCurvePoint();
  minPoint = curve.getMinPoint();
  maxPoint = curve.getMaxPoint();
  curvePoints = curve.getCurvePoints();
  curveColor = curve.getCurveColor();
  init();
}

void GlEditableCurve::init() {
  boundingBox.expand(startPoint);
  boundingBox.expand(endPoint);
  basicCircle.resizePoints(30);
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(255, 0, 0, 200));
  basicCircle.setOutlineColor(Color(0, 0, 255, 200));
}

void GlEditableCurve::draw(float lod, Camera* camera) {

  std::sort(curvePoints.begin(), curvePoints.end(), CoordXOrdering());
  camera->initGl();
  glDisable(GL_LIGHTING);
  GlLines::glDrawCurve(startPoint, curvePoints, endPoint, 2.,
                       GlLines::TLP_PLAIN, curveColor, curveColor);
  glEnable(GL_LIGHTING);

  vector<Coord> curvePointsCp(curvePoints);
  curvePointsCp.insert(curvePointsCp.begin(), startPoint);
  curvePointsCp.push_back(endPoint);
  vector<Coord>::iterator it;

  Camera camera2D(camera->getScene(), false);
  camera2D.setScene(camera->getScene());

  for (it = curvePointsCp.begin(); it != curvePointsCp.end(); ++it) {
    Coord anchor = *it;
    camera->initGl();
    Coord tmp = camera->worldTo2DScreen(anchor);
    tmp[2] = 0;
    camera2D.initGl();
    basicCircle.set(tmp, CIRCLE_RADIUS, 0.);
    basicCircle.draw(lod, camera);
  }

}

bool GlEditableCurve::pointBelong(const Coord &point) {

  bool ret = false;

  vector<Coord> curvePointsCp(curvePoints);
  curvePointsCp.insert(curvePointsCp.begin(), startPoint);
  curvePointsCp.push_back(endPoint);

  for (unsigned int i = 0; i < curvePointsCp.size() - 1; ++i) {
    double startToEndDist = curvePointsCp[i].dist(curvePointsCp[i + 1]);
    double startToPointDist = curvePointsCp[i].dist(point);
    double pointToEndDist = point.dist(curvePointsCp[i + 1]);
    ret = ret || (((startToPointDist + pointToEndDist) - startToEndDist)
                  / startToEndDist < 1E-3);
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

Coord *GlEditableCurve::getCurveAnchorAtPointIfAny(const Coord &point) {
  Coord *anchor = NULL;
  vector<Coord> curvePointsCp(curvePoints);
  curvePointsCp.insert(curvePointsCp.begin(), startPoint);
  curvePointsCp.push_back(endPoint);
  vector<Coord>::iterator it;

  for (it = curvePointsCp.begin(); it != curvePointsCp.end(); ++it) {
    Coord anchorCenter = *it;
    ;

    if (point.getX() > (anchorCenter.getX() - CIRCLE_RADIUS) && point.getX()
        < (anchorCenter.getX() + CIRCLE_RADIUS) && point.getY()
        > (anchorCenter.getY() - CIRCLE_RADIUS) && point.getY()
        < (anchorCenter.getY() + CIRCLE_RADIUS)) {
      anchor = new Coord(*it);
      break;
    }
  }

  return anchor;
}

Coord GlEditableCurve::translateCurveAnchorToPoint(const Coord &curveAnchor,
    const Coord &targetPoint) {
  Coord translationVector = targetPoint - curveAnchor;
  Coord newAnchorCenter = curveAnchor + translationVector;

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

  if (curveAnchor.getX() == startPoint.getX() && curveAnchor.getY()
      == startPoint.getY()) {
    newAnchorCenter.setX(startPoint.getX());
    startPoint = newAnchorCenter;
  }
  else if (curveAnchor.getX() == endPoint.getX() && curveAnchor.getY()
           == endPoint.getY()) {
    newAnchorCenter.setX(endPoint.getX());
    endPoint = newAnchorCenter;
  }
  else {
    std::replace(curvePoints.begin(), curvePoints.end(), curveAnchor,
                 newAnchorCenter);
  }

  return newAnchorCenter;
}

void GlEditableCurve::removeCurveAnchor(const Coord &curveAnchor) {
  curvePoints.erase(std::remove(curvePoints.begin(), curvePoints.end(),
                                curveAnchor), curvePoints.end());
}

float GlEditableCurve::getYCoordForX(const float xCoord) {
  Coord line1[2];
  Coord line2[2];

  line1[0] = Coord(xCoord, 0);
  line1[1] = Coord(xCoord, 10);

  vector<Coord> curvePointsCp(curvePoints);
  curvePointsCp.insert(curvePointsCp.begin(), startPoint);
  curvePointsCp.push_back(endPoint);

  for (unsigned int i = 0; i < curvePointsCp.size() - 1; ++i) {
    if (xCoord >= curvePointsCp[i].getX() && xCoord
        <= curvePointsCp[i + 1].getX()) {
      line2[0] = curvePointsCp[i];
      line2[1] = curvePointsCp[i + 1];
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

void GlEditableCurve::updateSize(const Coord &newMinPoint,
                                 const Coord &newMaxPoint) {
  float oldLength = maxPoint.getX() - minPoint.getX();
  float newLength = newMaxPoint.getX() - newMinPoint.getX();

  for (unsigned int i = 0; i < curvePoints.size(); ++i) {
    curvePoints[i].setX(newMinPoint.getX() + ((curvePoints[i].getX()
                        - minPoint.getX()) * newLength) / oldLength);
  }

  minPoint = newMinPoint;
  maxPoint = newMaxPoint;
  startPoint.setX(minPoint.getX());
  endPoint.setX(maxPoint.getX());
}

ColorScale::ColorScale() {}
ColorScale::ColorScale(const ColorScale& scale): colorMap(scale.colorMap) {}
ColorScale::~ColorScale() {}

void ColorScale::setColorScale(const std::vector<Color>& colors) {
  colorMap.clear();

  if (!colors.empty()) {
    float shift = 1.0f / colors.size()-1;

    for (unsigned int i = 0; i < colors.size(); ++i) {
      if (i == colors.size() - 1 && i * shift != 1.0f) {
        //Ensure that the last color will be set to 1
        colorMap[1.0f] = colors[i];
      }
      else {
        colorMap[(float) (i * shift)] = colors[i];
      }
    }
  }
}
void ColorScale::setColorScaleFromImage(const std::string &imageFile) {
  QImage gradientImage(QString::fromUtf8(imageFile.c_str()));
  unsigned int imageHeight = gradientImage.height();
  vector<Color> colors;

  for (unsigned int i = 0; i < imageHeight; ++i) {
    QRgb pixelValue = gradientImage.pixel(0, i);
    colors.push_back(Color(qRed(pixelValue), qGreen(pixelValue), qBlue(
                             pixelValue), qAlpha(pixelValue)));
  }

  std::reverse(colors.begin(), colors.end());
  setColorScale(colors);
}

Color ColorScale::getColorAtLen(const float pos) const {
  assert(pos >= 0 && pos <= 1);

  if (colorMap.empty()) {
    return Color(255, 255, 255);
  }
  else {
    Color startColor;
    Color endColor;
    float startCoord, endCoord;
    map<float, Color>::const_iterator it = colorMap.begin();
    startCoord = endCoord = it->first;
    startColor = endColor = it->second;

    for (++it; it != colorMap.end(); ++it) {
      endColor = it->second;
      endCoord = it->first;

      if (pos >= startCoord && pos <= endCoord) {
        break;
      }
      else {
        startColor = endColor;
        startCoord = endCoord;
      }
    }

    Color ret;
    double ratio = (pos - startCoord) / (endCoord - startCoord);

    for (unsigned int i = 0; i < 3; ++i) {
      ret[i] = (unsigned char) (double(startColor[i]) + (double(endColor[i])
                                - double(startColor[i])) * ratio);
    }

    return ret;
  }
}

GlColorScale::GlColorScale(const Coord &baseCoord, const float length,
                           const float thickness, Orientation orientation, ColorScale *colorScale) :
  scale(colorScale), baseCoord(baseCoord), length(length),
  thickness(thickness), colorScalePolyQuad(NULL),orientation(orientation) {
  if (colorScale)
    updateDrawing();

}

GlColorScale::GlColorScale(const GlColorScale &colorScale): orientation(Horizontal) {
  if (colorScale.scale)
    scale = new ColorScale(*colorScale.scale);
  else
    scale = NULL;

  baseCoord = colorScale.getBaseCoord();
  thickness = colorScale.getThickness();
  length = colorScale.getLength();
  GlPolyQuad *polyQuad = colorScale.getColorScalePolyQuad();

  if (polyQuad != NULL) {
    colorScalePolyQuad = new GlPolyQuad(*polyQuad);
  }
  else {
    colorScalePolyQuad = NULL;
  }
}

GlColorScale::~GlColorScale() {
  if (colorScalePolyQuad != NULL) {
    delete colorScalePolyQuad;
  }
}

void GlColorScale::setColorScale(ColorScale * scale) {
  this->scale = scale;
  updateDrawing();
}

void GlColorScale::updateDrawing() {
  if (colorScalePolyQuad != NULL) {
    delete colorScalePolyQuad;
  }

  if (!scale)
    return;

  colorScalePolyQuad = new GlPolyQuad();
  map<float, Color> colorMap = scale->getColorMap();

  if (colorMap.size() == 1) {
    if (orientation == GlColorScale::Vertival) {
      Coord quadEdgeStart(baseCoord.getX() - thickness / 2, baseCoord.getY());
      Coord quadEdgeEnd(baseCoord.getX() + thickness / 2, baseCoord.getY());
      colorScalePolyQuad->addQuadEdge(quadEdgeStart, quadEdgeEnd,
                                      colorMap.begin()->second);

      Coord quadEdgeStart2(baseCoord.getX() - thickness / 2, baseCoord.getY()
                           + length);
      Coord quadEdgeEnd2(baseCoord.getX() + thickness / 2, baseCoord.getY()
                         + length);
      colorScalePolyQuad->addQuadEdge(quadEdgeStart2, quadEdgeEnd2,
                                      colorMap.begin()->second);

    }
    else {
      Coord quadEdgeStart(baseCoord.getX(), baseCoord.getY() - thickness / 2);
      Coord quadEdgeEnd(baseCoord.getX(), baseCoord.getY() + thickness / 2);
      colorScalePolyQuad->addQuadEdge(quadEdgeStart, quadEdgeEnd,
                                      colorMap.begin()->second);
      Coord quadEdgeStart2(baseCoord.getX() + length, baseCoord.getY()
                           - thickness / 2);
      Coord quadEdgeEnd2(baseCoord.getX() + length, baseCoord.getY()
                         + thickness / 2);
      colorScalePolyQuad->addQuadEdge(quadEdgeStart2, quadEdgeEnd2,
                                      colorMap.begin()->second);
    }
  }
  else {

    Coord currentMin, currentMax;
    assert(colorMap.begin()->first == 0 );

    for (map<float, Color>::iterator colorMapIt = colorMap.begin(); colorMapIt
         != colorMap.end(); ++colorMapIt) {
      if (orientation == Vertival) {
        currentMin.set(baseCoord.getX() - thickness / 2, baseCoord.getY()
                       + colorMapIt->first * length);
        currentMax.set(baseCoord.getX() + thickness / 2, baseCoord.getY()
                       + colorMapIt->first * length);
      }
      else {
        currentMin.set(baseCoord.getX() + colorMapIt->first * length,
                       baseCoord.getY() - thickness / 2, 0);
        currentMax.set(baseCoord.getX() + colorMapIt->first * length,
                       baseCoord.getY() + thickness / 2, 0);

      }

      colorScalePolyQuad->addQuadEdge(currentMin, currentMax,
                                      colorMapIt->second);
    }
  }

  boundingBox = colorScalePolyQuad->getBoundingBox();
}
void GlColorScale::draw(float lod, Camera* camera) {
  if (colorScalePolyQuad != NULL) {
    colorScalePolyQuad->draw(lod, camera);
  }
}

Color GlColorScale::getColorAtPos(Coord pos) {
  if (orientation == GlColorScale::Vertival) {
    return scale->getColorAtLen((pos.getY() - baseCoord.getY()) / length);
  }
  else {
    return scale->getColorAtLen((pos.getX() - baseCoord.getX()) / length);
  }
}

}
