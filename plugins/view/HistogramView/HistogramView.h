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

#ifndef HISTOGRAMVIEW_H_
#define HISTOGRAMVIEW_H_

#include <tulip/GlMainView.h>

#include <vector>
#include <map>

#include "../../utils/ViewNames.h"
#include "Histogram.h"

#if defined(_MSC_VER)
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class QMenu;

namespace tlp {

class Graph;
class GlLayer;
class GlComposite;
class Interactor;
class GlRect;
class ViewGraphPropertiesSelectionWidget;
class HistoOptionsWidget;

/*@{*/
/** \file
 *  \brief  Tulip Histogram view

 * This view plugin allows to create frequency histograms from graph properties (supported types are Metric and Integer).
 * By selecting a set of graph properties, frequency histograms are computed and displayed for values associated
 * to nodes (or edges).
 *
 * This view also allows to map visual properties (colors, sizes, glyphs, ..) of the graph elements
 * with respect to a graph metric in a visual way. These operations can be done with the "Metric mapping"
 * interactor. Many interactors are also bundled with the view to perform elements selection, statistical analysis, ...
 *
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */
class HistogramView : public GlMainView {

  Q_OBJECT

public :

  PLUGININFORMATION(ViewName::HistogramViewName, "Antoine Lambert", "02/2009",
                    "<p>The Histogram view allows to create frequency histograms from graph properties (supported types are Metric and Integer)."
                    "By selecting a set of graph properties, frequency histograms are computed and displayed for values associated to nodes (or edges).</p>"
                    "<p>This view also allows to map visual properties (colors, sizes, glyphs, ..) of the graph elements with respect to a graph metric in a visual way. These operations can be done with the \"Metric mapping\" interactor. Many interactors are also bundled with the view to perform elements selection, statistical analysis, ...</p>", "1.1", "View")

  HistogramView(const PluginContext *);
  ~HistogramView();

  std::string icon() const {
    return ":/histogram_view.png";
  }


  void setState(const DataSet &dataSet);
  DataSet state() const;
  void graphChanged(Graph *graph);
  Graph *histoGraph() {
    return _histoGraph;
  }
  bool eventFilter(QObject *object,QEvent *event);
  QList<QWidget*> configurationWidgets() const;
  void setInteractors(const std::list<Interactor *> &interactors);

  std::vector<Histogram *> getHistograms() const;
  bool smallMultiplesViewSet() const {
    return smallMultiplesView;
  }

  void switchFromSmallMultiplesToDetailedView(Histogram *histogramToDetail);
  void switchFromDetailedViewToSmallMultiples();
  BoundingBox getSmallMultiplesBoundingBox() const;
  Histogram *getDetailedHistogram() const {
    return detailedHistogram;
  }

  void toggleInteractors(const bool activate);
  bool interactorsEnabled() const {
    return interactorsActivated;
  }

  ElementType getDataLocation() const {
    return dataLocation;
  }

  void updateHistograms(Histogram *detailOverview = NULL);

public slots :

  void viewConfigurationChanged();
  void draw();
  void refresh();
  void applySettings();

protected slots :

private :

  void interactorsInstalled(const QList<tlp::Interactor*>&);
  void registerTriggers();
  void initGlWidget(Graph *graph);
  void buildHistograms();

  void updateDetailedHistogramAxis();
  void destroyHistogramsIfNeeded();
  void cleanupGlScene();
  void addEmptyViewLabel();
  void removeEmptyViewLabel();


  ViewGraphPropertiesSelectionWidget *propertiesSelectionWidget;
  HistoOptionsWidget *histoOptionsWidget;

  GlQuantitativeAxis *xAxisDetail, *yAxisDetail;
  Graph *_histoGraph, *emptyGraph;
  GlGraphComposite *emptyGlGraphComposite;

  GlComposite *histogramsComposite, *labelsComposite, *axisComposite;
  std::map<std::string, Histogram *> histogramsMap;

  bool smallMultiplesView;
  GlLayer *mainLayer;
  std::vector<std::string> selectedProperties;

  Histogram *detailedHistogram;
  std::string detailedHistogramPropertyName;

  double sceneRadiusBak;
  double zoomFactorBak;
  Coord eyesBak;
  Coord centerBak;
  Coord upBak;

  GlLabel *noDimsLabel, *noDimsLabel1;
  GlLabel *noDimsLabel2;
  GlRect *emptyRect;
  GlRect *emptyRect2;

  int lastViewWindowWidth, lastViewWindowHeight;

  bool interactorsActivated;

  static GLuint binTextureId;
  static unsigned int histoViewInstancesCount;

  bool isConstruct;
  unsigned int lastNbHistograms;

  ElementType dataLocation;
  bool needUpdateHistogram;
};

}

#endif /* HISTOGRAMVIEW_H_ */
