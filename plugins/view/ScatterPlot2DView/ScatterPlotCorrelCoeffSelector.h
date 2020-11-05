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

#ifndef SCATTERPLOTCORELCOEFFSELECTOR_H_
#define SCATTERPLOTCORELCOEFFSELECTOR_H_

#include <tulip/GlSimpleEntity.h>
#include <tulip/GlCircle.h>
#include <tulip/GLInteractor.h>

namespace tlp {

class ScatterPlotCorrelCoeffSelectorOptionsWidget;
class ScatterPlot2DView;

class GlEditableComplexPolygon : public GlSimpleEntity {

public:
  GlEditableComplexPolygon(std::vector<Coord> polygonPoints, const Color &color);

  void translate(const Coord &move) override;
  void draw(float lod, Camera *camera) override;
  BoundingBox getBoundingBox() override;
  void getXML(std::string &) override {}
  void setWithXML(const std::string &, unsigned int &) override {}

  unsigned int getNumberOfVertex() const {
    return polygonPoints.size();
  }

  bool pointInsidePolygon(const Coord &point);
  Coord *getPolygonVertexUnderPointerIfAny(const Coord &pointerScreenCoord, Camera *camera);
  std::pair<Coord, Coord> *getPolygonSegmentUnderPointerIfAny(const Coord &pointerSceneCoord);

  void addPolygonVertex(std::pair<Coord, Coord> polygonSegment, const Coord &newVertex);
  void removePolygonVertex(const Coord &vertex);
  void movePolygonVertexToPoint(const Coord &polygonVertex, const Coord &targetPoint);

  void setColor(const Color &color) {
    this->color = color;
  }
  Color getColor() const {
    return color;
  }
  void setSelected(const bool selected) {
    this->selected = selected;
  }
  bool isSelected() const {
    return selected;
  }

  const std::vector<Coord> &getPolygonVertices() const {
    return polygonPoints;
  }

private:
  std::vector<Coord> polygonPoints;
  Color color;
  GlCircle basicCircle;
  bool selected;
};

class ScatterPlotCorrelCoeffSelector : public GLInteractorComponent {

public:
  ScatterPlotCorrelCoeffSelector(ScatterPlotCorrelCoeffSelectorOptionsWidget *optionsWidget);
  ScatterPlotCorrelCoeffSelector(
      const ScatterPlotCorrelCoeffSelector &scatterPlotCorrelCoeffSelector);
  ~ScatterPlotCorrelCoeffSelector() override;

  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *glMainWidget) override;
  bool compute(GlMainWidget *glMainWidget) override;
  void viewChanged(View *view) override;
  bool showContextMenu(const QPoint &globalPoint, const QPointF &scenePoint) override;

private:
  void getPolygonAndPointUnderPointerIfAny(const Coord &pointerSceneCoord, Camera *camera);
  void mapPolygonColorToCorrelCoeffOfData(GlEditableComplexPolygon *polygon,
                                          GlMainWidget *glWidget);

  ScatterPlotCorrelCoeffSelectorOptionsWidget *optionsWidget;
  ScatterPlot2DView *scatterView;
  Coord currentPointerSceneCoords;
  std::vector<Coord> polygonEdit;
  std::vector<GlEditableComplexPolygon *> polygons;
  GlCircle basicCircle;
  GlEditableComplexPolygon *selectedPolygon;
  Coord *selectedPolygonPoint;
  bool dragStarted;
  int x, y;
  std::map<GlEditableComplexPolygon *, std::pair<std::vector<node>, double>>
      polygonsToNodesSubsetAndCorrelCoeff;
};
} // namespace tlp

#endif /* SCATTERPLOTCORELCOEFFSELECTOR_H_ */
