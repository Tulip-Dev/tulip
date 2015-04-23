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

#ifndef THRESHOLDINTERACTOR_H_
#define THRESHOLDINTERACTOR_H_

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

//#include <tulip/GLInteractor.h>
#include "EditColorScaleInteractor.h"

#include <tulip/GlComposite.h>
#include <tulip/Observable.h>
#include <tulip/Size.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
//#include <tulip/GlQuad.h>
//#include <tulip/GlPolygon.h>
//#include <tulip/GlLabel.h>
//#include <tulip/GlRect.h>
//#include <tulip/DoubleProperty.h>
//#include "SOMView.h"

#include <QMutex>

namespace tlp {

class GlLabelledColorScale;
class GlPolygon;
class GlQuad;
class GlLabel;

class Slider {
public:
  virtual ~Slider() {
  }

  virtual float getLeftBound() = 0;
  virtual float getRightBound() = 0;
  virtual void beginShift() = 0;
  virtual void shift(float shift)=0;
  virtual void endShift() = 0;
};

class ColorScaleSlider: public Slider, public GlComposite, public Observable {
public:
  enum SliderWay {
    ToLeft, ToRight
  };
  ColorScaleSlider(SliderWay way, Size size, GlLabelledColorScale *colorScale,const std::string& textureName);
  ~ColorScaleSlider();
  void draw(float lod, tlp::Camera *camera);
  void setColor(Color c);
  Coord getBasePosition() {
    return position;
  }
  Size getSize() {
    return size;
  }
  SliderWay getWay() {
    return way;
  }
  void setLinkedSlider(ColorScaleSlider* linkedSlider);
  ColorScaleSlider* getLinkedSlider() {
    return linkedSlider;
  }
  float getLeftBound();
  float getRightBound();
  void beginShift();
  void shift(float shift);
  void endShift();
  double getValue();
  float getCurrentShift() const {
    return currentShift;
  }

  void setValue(double value);
  void update(std::set<Observable *>::iterator begin,
              std::set<Observable *>::iterator end);
  void observableDestroyed(Observable *);
protected:
  void buildComposite(const std::string& textureName);
  void updatePosition();
  void computeBoundingBox();
  SliderWay way;
  Coord position;
  Size size;
  GlPolygon *arrow;
  GlQuad *rect;
  GlLabel *label;
  ColorScaleSlider* linkedSlider;
  GlLabelledColorScale *linkedScale;

  float currentShift;
};

class SliderBar: public Slider, public tlp::GlSimpleEntity {

public:
  SliderBar(ColorScaleSlider* left, ColorScaleSlider* right,const std::string& textureName);
  ~SliderBar();
  float getLeftBound();
  float getRightBound();
  void beginShift();
  void shift(float shift);
  void endShift();
  void draw(float lod, tlp::Camera *camera);
  void getXML(std::string &) {}
  void setWithXML(const std::string &, unsigned int &) {}

protected:
  ColorScaleSlider* left;
  ColorScaleSlider* right;
  std::string texture;
  bool isVisible;
};

class ThresholdInteractor: public EditColorScaleInteractor {
public:
  ThresholdInteractor();
  virtual ~ThresholdInteractor();
  bool draw(tlp::GlMainWidget *glMainWidget);
  bool eventFilter(QObject *, QEvent *);
  void setView(View *view);
protected:
  void screenSizeChanged(SOMView* somView);
  void propertyChanged(SOMView* somView,const std::string& propertyName, tlp::DoubleProperty *newProperty);
  void performSelection(SOMView *somView, Iterator<node> *it);
  void buildSliders(SOMView* somView);
  void clearSliders();
  void generateSliderTexture(tlp::GlMainWidget* widget);

  tlp::GlLayer *layer;

  Slider *mouvingSlider;
  ColorScaleSlider *rSlider;
  ColorScaleSlider *lSlider;
  bool startDrag;
  int XPosCursor;

  QMutex lock;
  std::string textureName;
  GLuint textureId;
};
}
#endif /* THRESHOLDINTERACTOR_H_ */
