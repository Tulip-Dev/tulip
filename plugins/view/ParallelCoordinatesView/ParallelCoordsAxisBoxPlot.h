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

#ifndef PARALLELCOORDSAXISBOXPLOT_H_
#define PARALLELCOORDSAXISBOXPLOT_H_

#include <string>

#include <tulip/GlSimpleEntity.h>
#include <tulip/GLInteractor.h>
#include <tulip/Color.h>

namespace tlp {

class QuantitativeParallelAxis;

class GlAxisBoxPlot : public GlSimpleEntity {

public:
  GlAxisBoxPlot(QuantitativeParallelAxis *axis, const Color &fillColor, const Color &outlineColor);
  ~GlAxisBoxPlot() override {}

  void draw(float lod, Camera *camera) override;

  void getXML(std::string &) override {}

  void setWithXML(const std::string &, unsigned int &) override {}

  void setHighlightRangeIfAny(Coord sceneCoords);

private:
  void drawLabel(const Coord &position, const std::string &labelName, Camera *camera);

  QuantitativeParallelAxis *axis;
  Coord bottomOutlierCoord;
  Coord firstQuartileCoord;
  Coord medianCoord;
  Coord thirdQuartileCoord;
  Coord topOutlierCoord;
  float boxWidth;
  Color fillColor, outlineColor;
  Coord *highlightRangeLowBound;
  Coord *highlightRangeHighBound;
};

class ParallelAxis;
class ParallelCoordinatesView;

class ParallelCoordsAxisBoxPlot : public GLInteractorComponent {

public:
  ParallelCoordsAxisBoxPlot();
  ~ParallelCoordsAxisBoxPlot() override;
  bool eventFilter(QObject *, QEvent *) override;
  bool draw(GlMainWidget *glMainWidget) override;
  bool compute(GlMainWidget *glMainWidget) override;
  void viewChanged(View *view) override;

private:
  void buildGlAxisPlot(std::vector<ParallelAxis *> currentAxis);
  void deleteGlAxisPlot();

  void initOrUpdateBoxPlots();

  ParallelCoordinatesView *parallelView;
  Graph *currentGraph;
  std::map<QuantitativeParallelAxis *, GlAxisBoxPlot *> axisBoxPlotMap;
  ParallelAxis *selectedAxis;
  unsigned int lastNbAxis;
};
}

#endif /* PARALLELCOORDSBOXPLOT_H_ */
