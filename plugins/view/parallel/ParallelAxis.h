//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

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

#include <vector>
#include <set>

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include "ParallelTools.h"

namespace tlp {

class TLP_GL_SCOPE ParallelAxis : public GlComposite {

 public:

  ~ParallelAxis();

  const Coord &getBaseCoord() const;
  void setBaseCoord(const Coord &newCoord);

  void translate(const Coord &c);
  void computeBoundingBox();

  const std::string &getAxisName() const {return axisName;}
  void drawAxisLine();
  void addCaption(const std::string &caption);

  virtual Coord getPointCoordOnAxisForData(const unsigned int dataIdx) = 0;
  virtual void redraw() = 0;
  virtual void showConfigDialog() = 0;

  void draw(float lod,Camera *camera);

  float getAxisHeight() const {return axisHeight;}
  float getAxisGradWidth() const {return gradsWidth;}
  void setAxisHeight(const float axisHeight);
  Color getAxisColor() const {return axisColor;}
  void setAxisColor(const Color &axisColor) {this->axisColor = axisColor;}
  void setAxisAreaWidth(const float axisAreaWidth) {this->axisAreaWidth = axisAreaWidth;}

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

  ParallelAxis(const Coord &baseCoord, const float height, const float axisAreaWidth, const std::string &name, const Color &axisColor);

  void addLabelDrawing(const std::string &labelName, const float yCoord);

  void computeLabelsHeight(const unsigned int dataCount);

  std::string axisName;

  Coord baseCoord;
  float axisHeight;
  float axisAreaWidth;
  float gradsWidth;
  float gradsHeight;
  float spaceBetweenAxisGrads;
  Color axisColor;

  bool slidersActivated;
  Coord topSliderCoord;
  Coord bottomSliderCoord;

  bool visible;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
