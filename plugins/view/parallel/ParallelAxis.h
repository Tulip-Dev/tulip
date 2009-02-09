//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef PARALLEL_AXIS_H
#define PARALLEL_AXIS_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/GlComposite.h>
#include <tulip/GlRect.h>
#include <tulip/GlAxis.h>

#include <vector>
#include <set>

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include "ParallelTools.h"

namespace tlp {

class ParallelAxis : public GlComposite {

 public:

  virtual ~ParallelAxis();

  void translate(const Coord &c);
  void computeBoundingBox();
  void draw(float lod,Camera *camera);
  virtual void redraw();

  virtual Coord getPointCoordOnAxisForData(const unsigned int dataIdx) = 0;
  virtual void showConfigDialog() = 0;

  Coord getBaseCoord() const {return glAxis->getAxisBaseCoord();}
  std::string getAxisName() const {return glAxis->getAxisName();}
  float getAxisHeight() const {return glAxis->getAxisLength();}
  float getAxisGradsWidth() {return glAxis->getAxisGradsWidth();}
  float getLabelHeight() {return glAxis->getLabelHeight();}
  void setAxisHeight(const float axisHeight);
  Color getAxisColor() const {return glAxis->getAxisColor();}
  void setAxisColor(const Color &axisColor) {glAxis->setAxisColor(axisColor);}
  void setMaxCaptionWidth(const float maxCaptionWidth) {glAxis->setMaxCaptionWidth(maxCaptionWidth);}

  void setSlidersActivated(const bool slidersActivated) {this->slidersActivated = slidersActivated;}
  bool isSlidersActivated() {return slidersActivated;}
  Coord getTopSliderCoord() {return topSliderCoord;}
  void setTopSliderCoord(const Coord &topSliderCoord) {this->topSliderCoord = topSliderCoord;}
  Coord getBottomSliderCoord() {return bottomSliderCoord;}
  void setBottomSliderCoord(const Coord &bottomSliderCoord) {this->bottomSliderCoord = bottomSliderCoord;}

  virtual std::string getTopSliderTextValue() = 0;
  virtual std::string getBottomSliderTextValue() = 0;
  void resetSlidersPosition();

  void setVisible(const bool visible) {this->visible = visible;}
  bool isVisible() const {return visible;}

  virtual std::set<unsigned int> getDataInSlidersRange() = 0;
  virtual void updateSlidersWithDataSubset(const std::set<unsigned int> &dataSubset) = 0;

 protected :

  ParallelAxis(GlAxis *glAxis, const float 	axisAreaWidth);

  GlAxis *glAxis;

  bool slidersActivated;
  Coord topSliderCoord;
  Coord bottomSliderCoord;
  bool visible;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
