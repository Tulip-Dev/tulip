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

#ifndef PARALLELCOORDSAXISSLIDERS_H_
#define PARALLELCOORDSAXISSLIDERS_H_

#include <tulip/GlSimpleEntity.h>
#include <tulip/GLInteractor.h>
#include <tulip/GlLabel.h>
#include <tulip/GlPolygon.h>

#include "ParallelCoordinatesDrawing.h"

namespace tlp {

class GlQuad;
class ParallelCoordinatesView;

enum sliderType { TOP_SLIDER = 0, BOTTOM_SLIDER = 1 };

class AxisSlider : public GlSimpleEntity {

public:
  AxisSlider(const sliderType type, const Coord &sliderCoord, const float halfWidth,
             const float halfHeight, const Color &sliderColor, const Color &labelColor,
             const float rotationAngle = 0);
  ~AxisSlider() override;

  void setSliderFillColor(const Color &color);
  void setSliderOutlineColor(const Color &color);
  void setSliderLabel(const std::string &label) {
    sliderLabel->setText(label);
  }
  void setRotationAngle(const float rotationAngle) {
    this->rotationAngle = rotationAngle;
  }

  void draw(float lod, Camera *camera) override;
  BoundingBox getBoundingBox() override;
  Coord getSliderCoord() const {
    return sliderCoord;
  }
  void translate(const Coord &move) override;
  void moveToCoord(const Coord &coord) {
    translate(coord - sliderCoord);
  }
  sliderType getSliderType() const {
    return type;
  }
  Color getColor() {
    return arrowPolygon->getFillColor(0);
  }

  void getXML(std::string &) override {}
  void setWithXML(const std::string &, unsigned int &) override {}

private:
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

public:
  ParallelCoordsAxisSliders();
  ~ParallelCoordsAxisSliders() override;
  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *glMainWidget) override;
  bool compute(GlMainWidget *glMainWidget) override;
  void viewChanged(View *view) override;

private:
  void initOrUpdateSliders();
  AxisSlider *getSliderUnderPointer(GlMainWidget *glWidget, ParallelAxis *axis, int x, int y);
  void updateOtherAxisSliders();
  void buildGlSliders(const std::vector<ParallelAxis *> &axis);
  void deleteGlSliders();
  void setSlidersColor(const Color &color);
  void updateSlidersYBoundaries();

  ParallelCoordinatesView *parallelView;
  Graph *currentGraph;
  std::map<ParallelAxis *, std::vector<AxisSlider *>> axisSlidersMap;
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
  std::map<ParallelAxis *, std::pair<float, float>> slidersYBoundaries;
  GlLayer *selectionLayer;
};
} // namespace tlp

#endif /* PARALLELCOORDSAXISSLIDERS_H_ */
