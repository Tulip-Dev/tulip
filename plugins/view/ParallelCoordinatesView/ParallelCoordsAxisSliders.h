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

#ifndef PARALLELCOORDSAXISSLIDERS_H_
#define PARALLELCOORDSAXISSLIDERS_H_

#include <QCursor>
#include <QMouseEvent>

#include <string>

#include <tulip/GlRect.h>
#include <tulip/GlQuad.h>
#include <tulip/GlLabel.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>

#include "ParallelCoordinatesDrawing.h"
#include "ParallelCoordsGlEntitiesSelector.h"

namespace tlp {

enum sliderType {TOP_SLIDER = 0, BOTTOM_SLIDER = 1};

class AxisSlider : public GlSimpleEntity {

public :

  AxisSlider(const sliderType type, const Coord &sliderCoord, const float halfWidth, const float halfHeight, const Color &sliderColor, const Color &labelColor, const float rotationAngle = 0);
  ~AxisSlider();

  void setSliderFillColor(const Color &color);
  void setSliderOutlineColor(const Color &color);
  void setSliderLabel(const std::string &label) {
    sliderLabel->setText(label);
  }
  void setRotationAngle(const float rotationAngle) {
    this->rotationAngle = rotationAngle;
  }

  void draw(float lod,Camera *camera);
  BoundingBox getBoundingBox();
  Coord getSliderCoord() const {
    return sliderCoord;
  }
  void translate(const Coord &move);
  void moveToCoord(const Coord &coord) {
    translate(coord - sliderCoord);
  }
  sliderType getSliderType() const {
    return type;
  }
  Color getColor() {
    return arrowPolygon->getFillColor(0);
  }

  void getXML(std::string &)  {}
  void setWithXML(const std::string &, unsigned int &) {}

private :

  sliderType type;

  GlComposite *sliderComposite;
  GlQuad *sliderQuad;
  GlPolygon *sliderPolygon;
  GlPolygon *arrowPolygon;
  GlLabel *sliderLabel;
  Coord sliderCoord;
  float rotationAngle;
};

class ParallelCoordsAxisSliders : public GLInteractorComponent {

public :

  ParallelCoordsAxisSliders();
  ~ParallelCoordsAxisSliders();
  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget *glMainWidget);
  bool compute(GlMainWidget *glMainWidget);
  void viewChanged(View *view);

private :

  void initOrUpdateSliders();
  AxisSlider *getSliderUnderPointer(GlMainWidget *glWidget, ParallelAxis *axis, int x, int y);
  void updateOtherAxisSliders();
  void buildGlSliders(std::vector<ParallelAxis *> axis);
  void deleteGlSliders();
  void setSlidersColor(const Color &color);
  void updateSlidersYBoundaries();

  ParallelCoordinatesView *parallelView;
  Graph *currentGraph;
  std::map<ParallelAxis *, std::vector<AxisSlider *> > axisSlidersMap;
  ParallelAxis *selectedAxis;
  std::vector<ParallelAxis *> lastSelectedAxis;
  AxisSlider *selectedSlider;
  bool axisSliderDragStarted;
  bool pointerBetweenSliders;
  bool slidersRangeDragStarted;
  int slidersRangeLength;
  int xClick, yClick;
  float lastAxisHeight;
  unsigned int lastNbAxis;
  ParallelCoordinatesDrawing::HighlightedEltsSetOp highlightedEltsSetOperation;
  std::map<ParallelAxis *, std::pair<float, float> > slidersYBoundaries;
  GlLayer *selectionLayer;
};


}

#endif /* PARALLELCOORDSAXISSLIDERS_H_ */
