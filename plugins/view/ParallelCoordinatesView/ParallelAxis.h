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

#ifndef PARALLEL_AXIS_H
#define PARALLEL_AXIS_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/GlAxis.h>
#include <tulip/GlRect.h>

#include <vector>
#include <set>

#include "ParallelTools.h"

namespace tlp {

class ParallelAxis : public GlSimpleEntity {

public:

  virtual ~ParallelAxis();

  BoundingBox getBoundingBox();
  void setStencil(int stencil) {
    glAxis->setStencil(stencil);
  }
  void draw(float lod,Camera *camera);
  void translate(const Coord &c);
  void getXML(std::string &) {}
  void setWithXML(const std::string &, unsigned int &) {}

  virtual void redraw();
  virtual Coord getPointCoordOnAxisForData(const unsigned int dataIdx) = 0;
  virtual void showConfigDialog() = 0;

  void setBaseCoord(const Coord &baseCoord);
  Coord getBaseCoord() const;
  Coord getTopCoord() const;
  std::string getAxisName() const {
    return glAxis->getAxisName();
  }
  float getAxisHeight() const {
    return glAxis->getAxisLength();
  }
  float getAxisGradsWidth() {
    return glAxis->getAxisGradsWidth();
  }
  float getLabelHeight() {
    return glAxis->getLabelHeight();
  }
  void setAxisHeight(const float axisHeight);
  Color getAxisColor() const {
    return glAxis->getAxisColor();
  }
  void setAxisColor(const Color &axisColor) {
    glAxis->setAxisColor(axisColor);
  }
  void setMaxCaptionWidth(const float maxCaptionWidth) {
    glAxis->setMaxCaptionWidth(maxCaptionWidth/2);
  }
  void setCaptionPosition(const GlAxis::CaptionLabelPosition captionPosition);

  void setSlidersActivated(const bool slidersActivated) {
    this->slidersActivated = slidersActivated;
  }
  bool isSlidersActivated() {
    return slidersActivated;
  }
  Coord getTopSliderCoord() {
    return topSliderCoord;
  }
  void setTopSliderCoord(const Coord &topSliderCoord) {
    this->topSliderCoord = topSliderCoord;
  }
  Coord getBottomSliderCoord() {
    return bottomSliderCoord;
  }
  void setBottomSliderCoord(const Coord &bottomSliderCoord) {
    this->bottomSliderCoord = bottomSliderCoord;
  }

  virtual std::string getTopSliderTextValue() = 0;
  virtual std::string getBottomSliderTextValue() = 0;
  void resetSlidersPosition();

  void setHidden(const bool hidden) {
    this->hidden = hidden;
  }
  bool isHidden() const {
    return hidden;
  }

  virtual const std::set<unsigned int> &getDataInSlidersRange() = 0 ;
  virtual void updateSlidersWithDataSubset(const std::set<unsigned int> &dataSubset) = 0;

  void enableTrickForSelection();
  void disableTrickForSelection();

  void setRotationAngle(const float rotationAngle);
  float getRotationAngle() const {
    return rotationAngle;
  }
  Array<Coord, 4> getBoundingPolygonCoords() const;


protected :

  ParallelAxis(GlAxis *glAxis, const float  axisAreaWidth, const float rotationAngle, const GlAxis::CaptionLabelPosition captionPosition = GlAxis::BELOW);

  GlAxis *glAxis;
  GlRect *emptyRect;

  float axisAreaWidth;
  bool slidersActivated;
  Coord topSliderCoord;
  Coord bottomSliderCoord;
  float rotationAngle;
  bool hidden;
  std::set<unsigned int> dataSubset;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
