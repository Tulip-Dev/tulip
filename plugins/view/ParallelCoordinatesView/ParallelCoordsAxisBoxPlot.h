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

#ifndef PARALLELCOORDSAXISBOXPLOT_H_
#define PARALLELCOORDSAXISBOXPLOT_H_

#include <QCursor>
#include <QMouseEvent>

#include <string>
#include <map>

#include <tulip/GlPolygon.h>
#include <tulip/GlQuad.h>
#include <tulip/GlLines.h>
#include <tulip/GlLabel.h>
#include <tulip/GLInteractor.h>

namespace tlp {

class GlAxisBoxPlot : public GlSimpleEntity {

public :

  GlAxisBoxPlot(QuantitativeParallelAxis *axis, const Color& fillColor, const Color& outlineColor);
  ~GlAxisBoxPlot() {}

  void draw(float lod,Camera* camera);

  void getXML(std::string &) {}

  void setWithXML(const std::string &, unsigned int &) {}

  void setHighlightRangeIfAny(Coord sceneCoords);


private :

  void drawLabel(const Coord& position, const std::string& labelName, Camera *camera);

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

class ParallelCoordsAxisBoxPlot : public GLInteractorComponent {

public :

  ParallelCoordsAxisBoxPlot();
  ~ParallelCoordsAxisBoxPlot();
  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget *glMainWidget);
  bool compute(GlMainWidget *glMainWidget);
  void viewChanged(View *view);

private :

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
