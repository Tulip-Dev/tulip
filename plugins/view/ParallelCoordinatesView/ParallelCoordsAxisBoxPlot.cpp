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

#include <tulip/TlpTools.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/GlLines.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlMainWidget.h>

#include "ParallelCoordinatesView.h"
#include "ParallelTools.h"
#include "QuantitativeParallelAxis.h"
#include "ParallelCoordsAxisBoxPlot.h"

using namespace std;

namespace tlp {

const Color lightBlue(0, 217, 255, 100);
const Color darkBlue(0, 0, 255, 255);

GlAxisBoxPlot::GlAxisBoxPlot(QuantitativeParallelAxis *axis, const Color& fillColor, const Color& outlineColor) :
  axis(axis), boxWidth(5 * axis->getAxisGradsWidth()), fillColor(fillColor), outlineColor(outlineColor),
  highlightRangeLowBound(NULL), highlightRangeHighBound(NULL) {}

void GlAxisBoxPlot::draw(float lod,Camera* camera) {

  float rotationAngle = axis->getRotationAngle();

  if (rotationAngle != 0) {
    glPushMatrix();
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
  }

  bottomOutlierCoord = axis->getBottomOutlierCoord();
  firstQuartileCoord = axis->getFirstQuartileCoord();
  medianCoord = axis->getMedianCoord();
  thirdQuartileCoord = axis->getThirdQuartileCoord();
  topOutlierCoord = axis->getTopOutlierCoord();

  Coord interQuartileRangeBoxCoords[4];

  if (axis->hasAscendingOrder()) {
    boundingBox.expand(Coord(bottomOutlierCoord.getX() - boxWidth / 2.0f, bottomOutlierCoord.getY(), 0.0f));
    boundingBox.expand(Coord(topOutlierCoord.getX() + boxWidth / 2.0f, topOutlierCoord.getY(), 0.0f));

    interQuartileRangeBoxCoords[0] = Coord(thirdQuartileCoord.getX() - boxWidth / 2.0f, thirdQuartileCoord.getY(), 0.0f);
    interQuartileRangeBoxCoords[1] = Coord(thirdQuartileCoord.getX() + boxWidth / 2.0f, thirdQuartileCoord.getY(), 0.0f);
    interQuartileRangeBoxCoords[2] = Coord(firstQuartileCoord.getX() + boxWidth / 2.0f, firstQuartileCoord.getY(), 0.0f);
    interQuartileRangeBoxCoords[3] = Coord(firstQuartileCoord.getX() - boxWidth / 2.0f, firstQuartileCoord.getY(), 0.0f);
  }
  else {
    boundingBox.expand(Coord(topOutlierCoord.getX() - boxWidth / 2.0f, topOutlierCoord.getY(), 0.0f));
    boundingBox.expand(Coord(bottomOutlierCoord.getX() + boxWidth / 2.0f, bottomOutlierCoord.getY(), 0.0f));

    interQuartileRangeBoxCoords[0] = Coord(firstQuartileCoord.getX() - boxWidth / 2.0f, firstQuartileCoord.getY(), 0.0f);
    interQuartileRangeBoxCoords[1] = Coord(firstQuartileCoord.getX() + boxWidth / 2.0f, firstQuartileCoord.getY(), 0.0f);
    interQuartileRangeBoxCoords[2] = Coord(thirdQuartileCoord.getX() + boxWidth / 2.0f, thirdQuartileCoord.getY(), 0.0f);
    interQuartileRangeBoxCoords[3] = Coord(thirdQuartileCoord.getX() - boxWidth / 2.0f, thirdQuartileCoord.getY(), 0.0f);
  }

  GlQuad interQuartileRangeBox(interQuartileRangeBoxCoords[0], interQuartileRangeBoxCoords[1],
                               interQuartileRangeBoxCoords[2], interQuartileRangeBoxCoords[3], fillColor);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
  glEnable(GL_LIGHTING);
  interQuartileRangeBox.draw(lod, camera);
  glDisable(GL_BLEND);
  glDisable(GL_LIGHTING);

  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GlLines::glDrawLine(interQuartileRangeBoxCoords[0], interQuartileRangeBoxCoords[1], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
  GlLines::glDrawLine(interQuartileRangeBoxCoords[1], interQuartileRangeBoxCoords[2], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
  GlLines::glDrawLine(interQuartileRangeBoxCoords[2], interQuartileRangeBoxCoords[3], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
  GlLines::glDrawLine(interQuartileRangeBoxCoords[3], interQuartileRangeBoxCoords[0], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);

  GlLines::glDrawLine(bottomOutlierCoord + Coord(-(boxWidth / 2.0f), 0.0f, 0.0f), bottomOutlierCoord + Coord(boxWidth / 2.0f, 0.0f, 0.0f), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
  GlLines::glDrawLine(medianCoord + Coord(-(boxWidth / 2.0f), 0.0f, 0.0f), medianCoord + Coord(boxWidth / 2.0f, 0.0f, 0.0f), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
  GlLines::glDrawLine(topOutlierCoord + Coord(-(boxWidth / 2.0f), 0.0f, 0.0f), topOutlierCoord + Coord(boxWidth / 2.0f, 0.0f, 0.0f), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);

  GlLines::glDrawLine(bottomOutlierCoord, firstQuartileCoord, 2, GlLines::TLP_DASHED, outlineColor, outlineColor);
  GlLines::glDrawLine(thirdQuartileCoord, topOutlierCoord, 2, GlLines::TLP_DASHED, outlineColor, outlineColor);
  glDisable(GL_BLEND);

  drawLabel(bottomOutlierCoord, axis->getBottomOutlierStringValue(),camera);
  drawLabel(firstQuartileCoord, axis->getFirstQuartileStringValue(),camera);
  drawLabel(medianCoord, axis->getMedianStringValue(),camera);
  drawLabel(thirdQuartileCoord, axis->getThirdQuartileStringValue(),camera);
  drawLabel(topOutlierCoord, axis->getTopOutlierStringValue(),camera);

  if (highlightRangeLowBound != NULL && highlightRangeHighBound != NULL) {
    Coord highlightBoxCoords[4] = { Coord(highlightRangeHighBound->getX() - boxWidth / 2.0f, highlightRangeHighBound->getY(), 0.0f),
                                    Coord(highlightRangeHighBound->getX() + boxWidth / 2.0f, highlightRangeHighBound->getY(), 0.0f),
                                    Coord(highlightRangeLowBound->getX() + boxWidth / 2.0f, highlightRangeLowBound->getY(), 0.0f),
                                    Coord(highlightRangeLowBound->getX() - boxWidth / 2.0f, highlightRangeLowBound->getY(), 0.0f)
                                  };
    Color outlineColorTranslucent(outlineColor);
    outlineColorTranslucent.setA(10);
    GlQuad highlightBox(highlightBoxCoords[0], highlightBoxCoords[1], highlightBoxCoords[2], highlightBoxCoords[3], outlineColorTranslucent);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
    glEnable(GL_LIGHTING);
    highlightBox.draw(lod, camera);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    highlightRangeLowBound = NULL;
    highlightRangeHighBound = NULL;
  }

  if (rotationAngle != 0.0f) {
    glPopMatrix();
  }
}


void GlAxisBoxPlot::setHighlightRangeIfAny(Coord sceneCoords) {
  if (axis->hasAscendingOrder()) {
    if (sceneCoords.getY() < topOutlierCoord.getY() &&
        sceneCoords.getY() > thirdQuartileCoord.getY() &&
        sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
        sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeLowBound = &thirdQuartileCoord;
      highlightRangeHighBound = &topOutlierCoord;
      axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, TOP_OUTLIER);

    }
    else if (sceneCoords.getY() < thirdQuartileCoord.getY() &&
             sceneCoords.getY() > (medianCoord.getY() + (1.0f / 3.0f) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
             sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
             sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeLowBound = &medianCoord;
      highlightRangeHighBound = &thirdQuartileCoord;
      axis->setBoxPlotHighlightBounds(MEDIAN, THIRD_QUARTILE);

    }
    else if (sceneCoords.getY() > (medianCoord.getY() - (1.0f / 3.0f) * (medianCoord.getY() - firstQuartileCoord.getY())) &&
             sceneCoords.getY() < (medianCoord.getY() + (1.0f / 3.0f) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
             sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
             sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeLowBound = &firstQuartileCoord;
      highlightRangeHighBound = &thirdQuartileCoord;
      axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, THIRD_QUARTILE);

    }
    else if (sceneCoords.getY() > firstQuartileCoord.getY() &&
             sceneCoords.getY() < (medianCoord.getY() - (1.0f / 3.0f) * (medianCoord.getY() - firstQuartileCoord.getY()))&&
             sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
             sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {
      highlightRangeLowBound = &firstQuartileCoord;
      highlightRangeHighBound = &medianCoord;
      axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, MEDIAN);
    }
    else if (sceneCoords.getY() > bottomOutlierCoord.getY() &&
             sceneCoords.getY() < firstQuartileCoord.getY() &&
             sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
             sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeLowBound = &bottomOutlierCoord;
      highlightRangeHighBound = &firstQuartileCoord;
      axis->setBoxPlotHighlightBounds(BOTTOM_OUTLIER, FIRST_QUARTILE);

    }
    else {
      highlightRangeLowBound = NULL;
      highlightRangeHighBound = NULL;
      axis->setBoxPlotHighlightBounds(NO_VALUE, NO_VALUE);
    }
  }
  else {
    if (sceneCoords.getY() > topOutlierCoord.getY() &&
        sceneCoords.getY() < thirdQuartileCoord.getY()&&
        sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
        sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeHighBound = &thirdQuartileCoord;
      highlightRangeLowBound = &topOutlierCoord;
      axis->setBoxPlotHighlightBounds(TOP_OUTLIER, THIRD_QUARTILE);

    }
    else if (sceneCoords.getY() > thirdQuartileCoord.getY() &&
             sceneCoords.getY() < (medianCoord.getY() + (1.0f / 3.0f) * (thirdQuartileCoord.getY() - medianCoord.getY())) &&
             sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
             sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeHighBound = &medianCoord;
      highlightRangeLowBound = &thirdQuartileCoord;
      axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, MEDIAN);

    }
    else if (sceneCoords.getY() < (medianCoord.getY() - (1.0f / 3.0f) * (medianCoord.getY() - firstQuartileCoord.getY())) &&
             sceneCoords.getY() > (medianCoord.getY() + (1.0f / 3.0f) * (thirdQuartileCoord.getY() - medianCoord.getY())) &&
             sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
             sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeHighBound = &firstQuartileCoord;
      highlightRangeLowBound = &thirdQuartileCoord;
      axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, FIRST_QUARTILE);

    }
    else if (sceneCoords.getY() < firstQuartileCoord.getY() &&
             sceneCoords.getY() > (medianCoord.getY() - (1.0f / 3.0f) * (medianCoord.getY() - firstQuartileCoord.getY())) &&
             sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
             sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeHighBound = &firstQuartileCoord;
      highlightRangeLowBound = &medianCoord;
      axis->setBoxPlotHighlightBounds(MEDIAN, FIRST_QUARTILE);

    }
    else if (sceneCoords.getY() < bottomOutlierCoord.getY() &&
             sceneCoords.getY() > firstQuartileCoord.getY() &&
             sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2.0f)) &&
             sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2.0f))) {

      highlightRangeHighBound = &bottomOutlierCoord;
      highlightRangeLowBound = &firstQuartileCoord;
      axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, BOTTOM_OUTLIER);

    }
    else {
      highlightRangeLowBound = NULL;
      highlightRangeHighBound = NULL;
      axis->setBoxPlotHighlightBounds(NO_VALUE, NO_VALUE);
    }
  }
}



void GlAxisBoxPlot::drawLabel(const Coord& position, const string& labelName, Camera *camera) {
  float labelHeight = axis->getLabelHeight();

  float heightRef;

  if (axis->hasAscendingOrder()) {
    heightRef = topOutlierCoord.getY() - thirdQuartileCoord.getY();
  }
  else {
    heightRef = thirdQuartileCoord.getY() - topOutlierCoord.getY();
  }

  if (labelHeight > heightRef) {
    labelHeight = heightRef / 2.0f;
  }

  float labelWidth = labelName.length() * (labelHeight / 2.0f);

  if (labelName.length() == 1) {
    labelWidth *= 2.0f;
  }

  GlLabel labelToDraw(Coord(position.getX() - boxWidth / 2.0f - labelWidth / 2.0f, position.getY(), 0.0f),
                      Size(labelWidth, labelHeight), outlineColor);
  labelToDraw.setText(labelName);
  labelToDraw.draw(0, camera);
}

ParallelCoordsAxisBoxPlot::ParallelCoordsAxisBoxPlot() : parallelView(NULL), currentGraph(NULL), selectedAxis(NULL), lastNbAxis(0) {}

ParallelCoordsAxisBoxPlot::~ParallelCoordsAxisBoxPlot() {
  deleteGlAxisPlot();
}

void ParallelCoordsAxisBoxPlot::viewChanged(View *view) {
  if (view == NULL) {
    parallelView = NULL;
    return;
  }

  parallelView = dynamic_cast<ParallelCoordinatesView *>(view);
  initOrUpdateBoxPlots();
}

bool ParallelCoordsAxisBoxPlot::compute(GlMainWidget *) {
  initOrUpdateBoxPlots();
  return true;
}

void ParallelCoordsAxisBoxPlot::initOrUpdateBoxPlots() {
  vector<ParallelAxis *> allAxis = parallelView->getAllAxis();

  if (axisBoxPlotMap.empty()) {
    buildGlAxisPlot(allAxis);
    lastNbAxis = allAxis.size();
    parallelView->refresh();
    return;
  }

  if ((lastNbAxis != 0 && lastNbAxis != allAxis.size()) || (currentGraph != parallelView->getGraphProxy()->getGraph())) {
    deleteGlAxisPlot();
    buildGlAxisPlot(allAxis);
    selectedAxis = NULL;
    parallelView->refresh();
  }

  lastNbAxis = allAxis.size();
  currentGraph = parallelView->getGraphProxy()->getGraph();
}

void ParallelCoordsAxisBoxPlot::buildGlAxisPlot(vector<ParallelAxis *> currentAxis) {
  for (unsigned int i = 0 ; i < currentAxis.size() ; ++i) {
    if (dynamic_cast<QuantitativeParallelAxis *>(currentAxis[i])) {
      QuantitativeParallelAxis *quantitativeAxis = static_cast<QuantitativeParallelAxis *>(currentAxis[i]);

      if (quantitativeAxis->getMedianStringValue() != "KO")
        axisBoxPlotMap[quantitativeAxis] = new GlAxisBoxPlot(quantitativeAxis, lightBlue, darkBlue);
    }
  }
}

void ParallelCoordsAxisBoxPlot::deleteGlAxisPlot() {
  map<QuantitativeParallelAxis *, GlAxisBoxPlot *>::iterator it;

  for (it = axisBoxPlotMap.begin() ; it != axisBoxPlotMap.end() ; ++it) {
    delete (it->second);
  }

  axisBoxPlotMap.clear();
}

bool ParallelCoordsAxisBoxPlot::eventFilter(QObject *widget, QEvent *e) {

  GlMainWidget *glWidget = dynamic_cast<GlMainWidget *>(widget);

  if(!glWidget)
    return false;

  initOrUpdateBoxPlots();

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *me = (QMouseEvent *) e;
    int x = glWidget->width() - me->x();
    int y = me->y();
    Coord screenCoords(x, y, 0.0f);
    Coord sceneCoords(glWidget->getScene()->getGraphCamera().screenTo3DWorld(screenCoords));
    selectedAxis = parallelView->getAxisUnderPointer(me->x(), me->y());

    if (selectedAxis != NULL && dynamic_cast<QuantitativeParallelAxis *>(selectedAxis)) {
      if (axisBoxPlotMap.find(static_cast<QuantitativeParallelAxis *>(selectedAxis)) != axisBoxPlotMap.end())
        if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
          rotateVector(sceneCoords, -(selectedAxis->getRotationAngle()), Z_ROT);
        }

      axisBoxPlotMap[static_cast<QuantitativeParallelAxis *>(selectedAxis)]->setHighlightRangeIfAny(sceneCoords);
    }

    parallelView->refresh();
    return true;
  }

  if (e->type() == QEvent::MouseButtonPress) {
    return false;
  }

  if (e->type() == QEvent::MouseButtonRelease) {
    if (selectedAxis != NULL && dynamic_cast<QuantitativeParallelAxis *>(selectedAxis)) {
      Observable::holdObservers();

      if (axisBoxPlotMap.find(static_cast<QuantitativeParallelAxis *>(selectedAxis)) != axisBoxPlotMap.end())
        parallelView->highlightDataInAxisBoxPlotRange(static_cast<QuantitativeParallelAxis *>(selectedAxis));

      Observable::unholdObservers();
      selectedAxis = NULL;
      parallelView->refresh();
      return true;
    }
  }

  return false;
}

bool ParallelCoordsAxisBoxPlot::draw(GlMainWidget *glMainWidget) {

  Camera &camera = glMainWidget->getScene()->getLayer("Main")->getCamera();
  camera.initGl();

  map<QuantitativeParallelAxis *, GlAxisBoxPlot *>::iterator it;

  for (it = axisBoxPlotMap.begin(); it != axisBoxPlotMap.end(); ++it) {
    (it->second)->draw(0,&camera);
  }

  return true;
}

}
