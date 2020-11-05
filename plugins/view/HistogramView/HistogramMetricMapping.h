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

#ifndef HISTOGRAMCOLORMAPPING_H_
#define HISTOGRAMCOLORMAPPING_H_

#include <QCursor>
#include <QMouseEvent>

#include <tulip/tulipconf.h>
#include <tulip/GLInteractor.h>
#include <tulip/TlpTools.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlCircle.h>
#include <tulip/GlLabel.h>
#include <tulip/GlPolyQuad.h>
#include <tulip/ColorScale.h>
#include <tulip/GlColorScale.h>
#include <tulip/ColorScaleConfigDialog.h>

#include <map>

#include "HistogramView.h"
#include "SizeScaleConfigDialog.h"
#include "GlyphScaleConfigDialog.h"

namespace tlp {

class GlEditableCurve : public GlSimpleEntity {

public:
  GlEditableCurve(const Coord &startPoint, const Coord &endPoint, const Color &curveColor);
  GlEditableCurve(const GlEditableCurve &curve);
  ~GlEditableCurve() override {}

  void setXAxisScale(GlQuantitativeAxis *xAxis) {
    this->xAxis = xAxis;
  }
  bool pointBelong(const Coord &point);
  void addCurveAnchor(const Coord &point);
  Coord *getCurveAnchorAtPointIfAny(const Coord &point, Camera *camera);
  void removeCurveAnchor(const Coord &curveAnchor);
  Coord translateCurveAnchorToPoint(const Coord &curveAnchor, const Coord &targetPoint);
  float getYCoordForX(const float xCoord);
  void updateSize(const Coord &newMinPoint, const Coord &newMaxPoint);
  void resetCurve();

  void draw(float lod, Camera *camera) override;
  void getXML(std::string &) override {}
  void setWithXML(const std::string &, unsigned int &) override {}

  const Coord &getMinPoint() const {
    return minPoint;
  }
  const Coord &getMaxPoint() const {
    return maxPoint;
  }

  const Coord &getFirstCurvePoint() const {
    return startPoint;
  }
  void setCurveStartPoint(const Coord &startPoint) {
    this->startPoint = startPoint;
  }

  const Coord &getLastCurvePoint() const {
    return endPoint;
  }
  void setLastCurvePoint(const Coord &endPoint) {
    this->endPoint = endPoint;
  }

  const std::vector<Coord> &getCurvePoints() const {
    return curvePoints;
  }
  void setCurvePoints(const std::vector<Coord> &curvePoints) {
    this->curvePoints = curvePoints;
  }

  const Color &getCurveColor() const {
    return curveColor;
  }
  void setCurveColor(const Color &color) {
    curveColor = color;
  }

private:
  void init();

  Coord startPoint, endPoint;
  Coord minPoint, maxPoint;
  std::vector<Coord> curvePoints;
  Color curveColor;
  GlCircle basicCircle;
  GlQuantitativeAxis *xAxis;
};

class GlSizeScale : public GlSimpleEntity {

public:
  enum Orientation { Horizontal, Vertical };

  GlSizeScale(const float minSize, const float maxSize, const Coord &baseCoord, const float length,
              const float thickness, const Color &color, Orientation orientation);
  ~GlSizeScale() override;

  float getSizeAtPos(const Coord &pos);

  void draw(float lod, Camera *camera) override;

  void translate(const Coord &move) override;

  void getXML(std::string &) override {}

  void setWithXML(const std::string &, unsigned int &) override {}

  Coord getBaseCoord() const {
    return baseCoord;
  }

  float getThickness() const {
    return thickness;
  }

  float getLength() const {
    return length;
  }

  float getMinSize() const {
    return minSize;
  }
  float getMaxSize() const {
    return maxSize;
  }

  void setMinSize(const float minSize) {
    this->minSize = minSize;
  }
  void setMaxSize(const float maxSize) {
    this->maxSize = maxSize;
  }

  void setColor(const Color &color) {
    this->color = color;
  }

private:
  float minSize, maxSize;
  Coord baseCoord;
  float length, thickness;
  Color color;
  Orientation orientation;
  GlPolyQuad *polyquad;
  GlLabel *minLabel, *maxLabel;
};

class GlGlyphScale : public GlSimpleEntity {

public:
  enum Orientation { Horizontal, Vertical };

  GlGlyphScale(const Coord &baseCoord, const float length, Orientation orientation);
  ~GlGlyphScale() override;

  void setGlyphsList(const std::vector<int> &glyphsList);

  int getGlyphAtPos(const Coord &pos);

  void draw(float lod, Camera *camera) override;

  void translate(const Coord &move) override;

  void getXML(std::string &) override {}

  void setWithXML(const std::string &, unsigned int &) override {}

  Coord getBaseCoord() const {
    return baseCoord;
  }

  float getLength() const {
    return length;
  }

private:
  GlGraphRenderingParameters glyphGraphRenderingParameters;
  Graph *glyphGraph;
  LayoutProperty *glyphGraphLayout;
  SizeProperty *glyphGraphSize;
  ColorProperty *glyphGraphColor;
  IntegerProperty *glyphGraphShape;
  GlGraphInputData *glyphGraphInputData;
  Coord baseCoord;
  float length;
  Orientation orientation;
  std::map<std::pair<float, float>, int> glyphScaleMap;
  float size;
};

class HistogramMetricMapping : public GLInteractorComponent {

  enum MappingType { VIEWCOLOR_MAPPING, VIEWBORDERCOLOR_MAPPING, SIZE_MAPPING, GLYPH_MAPPING };

public:
  HistogramMetricMapping();
  HistogramMetricMapping(const HistogramMetricMapping &histogramMetricMapping);
  ~HistogramMetricMapping() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *glMainWidget) override;
  bool compute(GlMainWidget *glMainWidget) override;
  void viewChanged(View *view) override;
  bool showContextMenu(const QPoint &globalPoint, const QPointF &scenePoint) override;

  bool pointerUnderScale(const Coord &sceneCoords);
  void updateGraphWithMapping(Graph *graph, LayoutProperty *histogramLayout);
  void updateMapping(GlQuantitativeAxis *histoXAxis, unsigned int nbHistogramBins);

private:
  void initInteractor();

protected:
  HistogramView *histoView;
  GlEditableCurve *curve;
  bool curveDragStarted;
  Coord *selectedAnchor;
  ColorScale *colorScale, dialogColorScale;
  GlColorScale *glColorScale;
  GlSizeScale *glSizeScale;
  GlGlyphScale *glGlyphScale;
  ColorScaleConfigDialog *colorScaleConfigDialog;
  SizeScaleConfigDialog *sizeScaleConfigDialog;
  GlyphScaleConfigDialog *glyphScaleConfigDialog;
  float lastXAxisLength;
  GlQuantitativeAxis *histoXAxis;
  GlPolyQuad *mappinqPolyQuad;
  float scaleAxisOffset;

  Graph *glyphMappingGraph;
  GlGraphRenderingParameters glyphMapppingGraphRenderingParameters;
  GlGraphInputData *glyphMappingGraphInputData;

  MappingType mappingType;

  QMenu *popupMenu;
  QMenu *colorMappingMenu;
  QAction *viewColorMappingAction;
  QAction *viewBorderColorMappingAction;
  QAction *sizeMapping;
  QAction *glyphMapping;

  std::map<MappingType, std::vector<Coord>> curveShapeForMapping;
};
} // namespace tlp

#endif /* HISTOGRAMCOLORMAPPING_H_ */
