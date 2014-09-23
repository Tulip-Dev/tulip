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

#ifndef SCATTERPLOT2DVIEW_H_
#define SCATTERPLOT2DVIEW_H_

#include <tulip/GlMainView.h>
#include <tulip/BoundingBox.h>
#include <tulip/Coord.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "../../utils/ViewNames.h"

namespace tlp {

/*@{*/
/** \file
 *  \brief  Tulip Scatter Plot view

 * This view plugin allows to create 2d scatter plots of graph nodes from graph properties (supported types are Metric and Integer).
 * A scatter plot is a type of mathematical diagram using Cartesian coordinates to display values for two variables for a set of data.
 * The data is displayed as a collection of points, each having the value of one variable determining the position
 * on the horizontal axis and the value of the other variable determining the position on the vertical axis.
 *
 * By selecting a set of graph properties, a scatter plot matrix will be displayed for each combination
 * of two different properties. Each scatter plot can then be displayed individually in a more detailed way.
 *
 * A set of interactors are bundled with the view to perform selection, statistical analysis, ...
 *
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

class ScatterPlot2D;
class GlLayer;
class GlComposite;
class SizeProperty;
class GlGraphComposite;
class ScatterPlot2DOptionsWidget;
class ViewGraphPropertiesSelectionWidget;

class ScatterPlot2DView : public GlMainView {

  Q_OBJECT

  void registerTriggers();

public :

  PLUGININFORMATION(ViewName::ScatterPlot2DViewName, "Antoine Lambert", "03/2009",
                    "<p>The Scatter Plot 2D view allows to create 2d scatter plots of graph nodes from graph properties (supported types are Metric and Integer).</p>"
                    "<p>A scatter plot is a type of mathematical diagram using Cartesian coordinates to display values for two variables for a set of data. The data are displayed as a collection of points, each having the value of one variable determining the position on the horizontal axis and the value of the other variable determining the position on the vertical axis.</p>"
                    "<p>By selecting a set of graph properties, a scatter plot matrix will be displayed for each combination of two different properties. Each scatter plot can then be displayed individually in a more detailed way.</p>"
                    "<p>A set of interactors are bundled with the view to perform selection, statistical analysis, ...</p>", "1.0", "View")

  ScatterPlot2DView(const PluginContext *);
  ~ScatterPlot2DView();
  std::string icon() const {
    return ":/scatter_plot2d_view.png";
  }
  void setState(const DataSet &dataSet);
  DataSet state() const;
  Graph *getScatterPlotGraph();
  void graphChanged(Graph *graph);
  void toggleInteractors(const bool activate);
  bool interactorsEnabled() const {
    return interactorsActivated;
  }
  QList<QWidget *> configurationWidgets() const;
  void computeNodeSizes();
  void buildScatterPlotsMatrix();
  std::vector<ScatterPlot2D *> getSelectedScatterPlots() const;
  ScatterPlot2D *getDetailedScatterPlot() const {
    return detailedScatterPlot;
  }
  BoundingBox getMatrixBoundingBox();
  bool matrixViewSet() const {
    return matrixView;
  }
  void setMatrixView(const bool matrixView) {
    this->matrixView = matrixView;
  }
  void generateScatterPlot(ScatterPlot2D *scatterPlot, GlMainWidget *glWidget = NULL);

  void switchFromMatrixToDetailView(ScatterPlot2D *scatterPlot, bool center);
  void switchFromDetailViewToMatrixView();

  void draw();
  void refresh();

public slots :

  void init();
  // inherited from GlMainView
  void centerView(bool graphChanged = false);
  void applySettings();

private :

  void interactorsInstalled(const QList<tlp::Interactor*>&);
  void generateScatterPlots();
  void initGlWidget(Graph *graph);
  void destroyOverviewsIfNeeded();
  void destroyOverviews();
  void cleanScene();
  void addEmptyViewLabel();
  void removeEmptyViewLabel();

  ViewGraphPropertiesSelectionWidget *propertiesSelectionWidget;
  ScatterPlot2DOptionsWidget *optionsWidget;

  Graph *scatterPlotGraph, *emptyGraph;
  GlLayer *mainLayer;
  GlGraphComposite *glGraphComposite;
  SizeProperty *scatterPlotSize;
  std::vector<std::string> selectedGraphProperties;
  GlComposite *matrixComposite, *axisComposite, *labelsComposite;
  std::map<std::pair<std::string, std::string>, ScatterPlot2D *>  scatterPlotsMap;
  ScatterPlot2D *detailedScatterPlot;
  std::pair<std::string, std::string> detailedScatterPlotPropertyName;
  std::map<std::pair<std::string, std::string>, bool> scatterPlotsGenMap;
  bool center;
  bool matrixView;
  double sceneRadiusBak;
  double zoomFactorBak;
  Coord eyesBak;
  Coord centerBak;
  Coord upBak;
  Interactor *scatterPlotViewNavigator;
  bool matrixUpdateNeeded;
  bool newGraphSet;

  int lastViewWindowWidth, lastViewWindowHeight;

  bool interactorsActivated;
  bool initialized;

  static GLuint backgroundTextureId;
  static unsigned int scatterplotViewInstancesCount;

};

}

#endif /* SCATTERPLOT2DVIEW_H_ */
