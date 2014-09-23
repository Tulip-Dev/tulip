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

#ifndef Tulip_PARALLELVIEW_H
#define Tulip_PARALLELVIEW_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include "ParallelCoordinatesDrawing.h"
#include "QuantitativeParallelAxis.h"
#include "ParallelCoordsDrawConfigWidget.h"

#include "../../utils/ViewNames.h"

#include <tulip/GlMainView.h>

#include <set>

class QMenu;

namespace tlp {

struct SelectedEntity;
class GlGraphComposite;
class ViewGraphPropertiesSelectionWidget;

/*@{*/
/** \file
 *  \brief  Tulip Parallel Coordinates view

 * This view plugin allows to create a parallel coordinates visualization
 * from graph elements.
 *
 * Parallel coordinates is a common way of visualizing high-dimensional geometry and analyzing multivariate data.
 * The idea is to visualize a set of points in an n-dimensional space. To do so, the n dimensions are represented
 * by n parallel axis, typically vertical and equally spaced. A point in n-dimensional space is represented as a
 * polyline with vertices on the parallel axis. The position of the vertex on the i-th axis corresponds to the
 * i-th coordinate of the point.
 *
 * By selecting a set of graph properties (supported types are Metric, Integer and String), nodes and edges
 * of a graph can be visualized as parallel coordinates. Each axis corresponds to a graph property and nodes
 * (or edges) of the graph are represented as polylines or curves.
 *
 * A set of interactors are bundled with the view to perform brushing, elements selection, data filtering, ...
 *
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */
class ParallelCoordinatesView : public GlMainView {

  Q_OBJECT

public :

  PLUGININFORMATION(ViewName::ParallelCoordinatesViewName, "Antoine Lambert", "16/04/2008",
                    "<p>The Parallel Coordinates view allows to create a parallel coordinates visualization from graph elements."
                    "Parallel coordinates are a common way of visualizing high-dimensional geometry and analyzing multivariate data.\n"
                    "The basic idea is to visualize a set of points in an n-dimensional space. To do so, the n dimensions are represented by n parallel axis, typically vertical and equally spaced. A point in n-dimensional space is represented as a polyline with vertices on the parallel axis. The position of the vertex on the i-th axis corresponds to the i-th coordinate of the point.</p>"
                    "<p>By selecting a set of graph properties (supported types are Metric, Integer and String), nodes and edges of a graph can be visualized as parallel coordinates. Each axis corresponds to a graph property and nodes (or edges) of the graph are represented as polylines or curves.</p>"
                    "<p>A set of interactors are bundled with the view to perform brushing, elements selection, data filtering, ...</p>", "1.2", "View")

  ParallelCoordinatesView(const tlp::PluginContext *);
  ~ParallelCoordinatesView();
  std::string icon() const {
    return ":/parallel_coordinates_view.png";
  }

  // View methods
  void setState(const DataSet &dataSet);
  tlp::DataSet state() const;
  bool eventFilter(QObject *object,QEvent *event);
  void fillContextMenu(QMenu *menu,const QPointF &point);
  QList<QWidget *> configurationWidgets() const;

  // methods called by interactors
  void setDataUnderPointerSelectFlag(const int x, const int y, const bool selectFlag);
  void setDataInRegionSelectFlag(const int x, const int y, const unsigned int width, const unsigned int height, const bool selectFlag);
  void resetSelection();
  void deleteDataUnderPointer(const int x, const int y);
  bool getDataUnderPointerProperties(const int x, const int y,SelectedEntity &selectedEntity);
  void highlightDataUnderPointer(const int x, const int y, const bool addEltToMagnifyFlag);
  void highlightDataInRegion(const int x, const int y, const int width, const int height, const bool addEltToMagnifyFlag);
  void resetHighlightedElements();
  bool highlightedElementsSet() const;
  ParallelAxis *getAxisUnderPointer(const int x, const int y) const;
  void swapAxis(ParallelAxis *firstAxis, ParallelAxis *secondAxis);
  void removeAxis(ParallelAxis *axis);
  void addAxis(ParallelAxis *axis);
  std::vector<ParallelAxis *> getAllAxis();
  void resetAxisLayoutNextUpdate() {
    parallelCoordsDrawing->resetAxisLayoutNextUpdate();
  }

  void updateWithAxisSlidersRange(ParallelAxis *axis, ParallelCoordinatesDrawing::HighlightedEltsSetOp highlightedEltsSetOp);
  void updateAxisSlidersPosition();
  bool hasHighlightedElts() const {
    return graphProxy->highlightedEltsSet();
  }
  void highlightDataInAxisBoxPlotRange(QuantitativeParallelAxis *axis);

  ParallelCoordinatesDrawing::LayoutType getLayoutType() const;
  ParallelCoordinatesDrawing::LinesType getLinesType() const;
  ParallelCoordinatesDrawing::LinesThickness getLinesThickness() const;

  //unsigned int getSpaceBetweenAxis() {return drawConfigWidget->getSpaceBetweenAxis();}

public slots:

  void draw();
  void refresh();
  void init();
  ParallelCoordinatesGraphProxy *getGraphProxy() {
    return graphProxy;
  }

  //TODO : reactivate this
  void elementSelectedSlot(unsigned int , bool ) {
    //emit elementSelected(id,isNode);
  }
  void centerSetupAndDrawView();
  void setupAndDrawView();

  void applySettings();

protected slots:

  void graphChanged(tlp::Graph*);

  void axisConfigurationSlot();
  void removeAxisSlot();
  void selectHighlightedElementsSlot();
  void resetHightlightedElementsSlot();

private :

  const std::set<unsigned int> &mapGlEntitiesInRegionToData(const int x, const int y, const unsigned int width, const unsigned int height);
  void initGlWidget();
  void buildContextMenu();
  void showAxisConfigDialog(ParallelAxis *axis);

  void updateWithProgressBar();
  void updateWithoutProgressBar();

  void addEmptyViewLabel();
  void removeEmptyViewLabel();

  void registerTriggers();
  void removeTriggers();

  QMenu *viewSetupMenu;
  QMenu *optionsMenu;
  QAction *classicLayout;
  QAction *circularLayout;
  QAction *straightLinesType;
  QAction *catmullRomSplineLinesType;
  QAction *cubicBSplineInterpolationLinesType;
  QAction *thickLines;
  QAction *thinLines;
  QAction *showToolTips;
  QAction *addRemoveDataFromSelection;
  QAction *selectData;
  QAction *deleteData;
  QAction *showDataProperties;
  QAction *axisMenuSeparator;
  QAction *axisConfiguration;
  QAction *removeAxisAction;
  QAction *highlightMenuSeparator;
  QAction *selectHighlightedElements;
  QAction *resetHightlightedElements;

  unsigned int dataUnderMousePointer;
  GlLayer *mainLayer;
  GlLayer *axisSelectionLayer;

  GlGraphComposite *glGraphComposite;
  Graph *axisPointsGraph;
  DataSet *dataSet;

  ParallelCoordinatesGraphProxy *graphProxy;
  ParallelCoordinatesDrawing *parallelCoordsDrawing;
  ParallelAxis *axisUnderPointer;

  ViewGraphPropertiesSelectionWidget *dataConfigWidget;
  ParallelCoordsDrawConfigWidget *drawConfigWidget;

  bool firstSet;
  unsigned int lastNbSelectedProperties;
  bool center;

  int lastViewWindowWidth, lastViewWindowHeight;

  bool isConstruct;
  bool dontCenterViewAfterConfLoaded;
  bool needDraw;

  std::set<unsigned int> mappedData;

  static GLuint linesTextureId;
  static GLuint slidersTextureId;
  static unsigned int parallelViewInstancesCount;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_PARALLELVIEW_H
