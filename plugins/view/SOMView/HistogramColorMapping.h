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

#ifndef HISTOGRAMCOLORMAPPING_H_
#define HISTOGRAMCOLORMAPPING_H_

#include <QCursor>
#include <QEvent>

#include <tulip/tulipconf.h>
#include <tulip/GWInteractor.h>
#include <tulip/TlpTools.h>
#include <tulip/GWInteractor.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlCircle.h>
#include <tulip/GlPolyQuad.h>

#include <map>

namespace tlp {

struct ltCoord {
  bool operator()(const Coord s1, const Coord s2) const {
    return s1.getY() < s2.getY();
  }
};

class GlEditableCurve: public GlSimpleEntity {

public:

  GlEditableCurve(const Coord &startPoint, const Coord &endPoint,
                  const Color &curveColor);
  GlEditableCurve(const GlEditableCurve &curve);
  ~GlEditableCurve() {
  }

  bool pointBelong(const Coord &point);
  void addCurveAnchor(const Coord &point);
  Coord *getCurveAnchorAtPointIfAny(const Coord &point);
  void removeCurveAnchor(const Coord &curveAnchor);
  Coord translateCurveAnchorToPoint(const Coord &curveAnchor,
                                    const Coord &targetPoint);
  float getYCoordForX(const float xCoord);
  void updateSize(const Coord &newMinPoint, const Coord &newMaxPoint);

  void draw(float lod, Camera* camera);
  void getXML(xmlNodePtr rootNode) {
  }
  void setWithXML(xmlNodePtr rootNode) {
  }

  Coord getMinPoint() const {
    return minPoint;
  }
  Coord getMaxPoint() const {
    return maxPoint;
  }
  Coord getFirstCurvePoint() const {
    return startPoint;
  }
  Coord getLastCurvePoint() const {
    return endPoint;
  }
  std::vector<Coord> getCurvePoints() const {
    return curvePoints;
  }
  Color getCurveColor() const {
    return curveColor;
  }

private:

  void init();

  Coord startPoint, endPoint;
  Coord minPoint, maxPoint;
  std::vector<Coord> curvePoints;
  Color curveColor;
  GlCircle basicCircle;
};

class ColorScale {

public:
  ColorScale();
  ColorScale(const ColorScale& scale);
  virtual ~ColorScale();

  virtual void setColorScale(const std::vector<Color>& colors);
  void setColorScaleFromImage(const std::string &imageFile);
  Color getColorAtLen(const float pos) const;

  std::map<float, Color> getColorMap() const {
    return colorMap;
  }
protected:
  std::map<float, Color> colorMap;
};

class GlColorScale: public GlSimpleEntity {

public:
  enum Orientation {
    Horizontal, Vertival
  };
  GlColorScale(const Coord &baseCoord, const float length,
               const float thickness, Orientation orientation,ColorScale *colorScale=NULL);
  GlColorScale(const GlColorScale &colorScale);
  ~GlColorScale();

  //void setColorScale(const std::vector<Color> colors);
  Color getColorAtPos(Coord pos);

  void draw(float lod, Camera* camera);
  void getXML(xmlNodePtr rootNode) {
  }
  void setWithXML(xmlNodePtr rootNode) {
  }

  Coord getBaseCoord() const {
    return baseCoord;
  }
  float getThickness() const {
    return thickness;
  }
  float getLength() const {
    return length;
  }
  GlPolyQuad *getColorScalePolyQuad() const {
    return colorScalePolyQuad;
  }

  void setColorScale(ColorScale * scale);
  ColorScale *getColorScale() {
    return scale;
  }

private:
  void updateDrawing();

  ColorScale *scale;
  Coord baseCoord;
  float length, thickness;
  GlPolyQuad *colorScalePolyQuad;
  Orientation orientation;
};

}

#endif /* HISTOGRAMCOLORMAPPING_H_ */
