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

#ifndef HISTOGRAMVIEW_H_
#define HISTOGRAMVIEW_H_

#include <tulip/GlMainView.h>

#include <vector>
#include <unordered_map>

#include "../../utils/PluginNames.h"
#include "Histogram.h"

#include <tulip/OpenGlIncludes.h>

namespace tlp {

class Graph;
class GlLayer;
class GlComposite;
class Interactor;
class GlRect;
class ViewGraphPropertiesSelectionWidget;
class HistoOptionsWidget;
class QuickAccessBarImpl;

/*@{*/
/** \file
 *  \brief  Tulip Histogram view

 * This view plugin allows to create frequency histograms from graph properties (supported types are
 Double and Integer).
 * By selecting a set of graph properties, frequency histograms are computed and displayed for
 values associated
 * to nodes (or edges).
 *
 * This view also allows to map visual properties (colors, sizes, glyphs, ..) of the graph elements
 * with respect to a graph metric in a visual way. These operations can be done with the "Metric
 mapping"
 * interactor. Many interactors are also bundled with the view to perform elements selection,
 statistical analysis, ...
 *
 *
 */
class HistogramView : public GlMainView {

  Q_OBJECT

public:
  PLUGININFORMATION(ViewName::HistogramViewName, "Antoine Lambert", "02/2009",
                    "<p>The Histogram view allows to create frequency histograms from graph "
                    "properties (supported types are Double and Integer). "
                    "By selecting a set of graph properties, frequency histograms are computed and "
                    "displayed for values associated to nodes (or edges).</p>"
                    "<p>This view also allows to map visual properties (colors, sizes, glyphs, ..) "
                    "of the graph elements with respect to a graph metric in a visual way. These "
                    "operations can be done with the \"Metric mapping\" interactor. Many "
                    "interactors are also bundled with the view to perform elements selection, "
                    "statistical analysis, ...</p>",
                    "1.1", "View")

  HistogramView(const PluginContext *);
  ~HistogramView() override;

  std::string icon() const override {
    return ":/histogram_view.png";
  }

  void setState(const DataSet &dataSet) override;
  DataSet state() const override;
  void graphChanged(Graph *graph) override;
  Graph *histoGraph() {
    return _histoGraph;
  }
  bool eventFilter(QObject *object, QEvent *event) override;
  QList<QWidget *> configurationWidgets() const override;

  std::vector<Histogram *> getHistograms() const;
  bool smallMultiplesViewSet() const {
    return smallMultiplesView;
  }

  QuickAccessBar *getQuickAccessBarImpl() override;

  void switchFromSmallMultiplesToDetailedView(Histogram *histogramToDetail);
  void switchFromDetailedViewToSmallMultiples();
  BoundingBox getSmallMultiplesBoundingBox() const;
  Histogram *getDetailedHistogram() const {
    return detailedHistogram;
  }

  void toggleInteractors(const bool activate);

  ElementType getDataLocation() const {
    return dataLocation;
  }

  void updateHistograms(Histogram *detailOverview = nullptr);

  void treatEvent(const Event &message) override;

  void afterSetNodeValue(PropertyInterface *, const node);
  void afterSetEdgeValue(PropertyInterface *, const edge);
  void afterSetAllNodeValue(PropertyInterface *);
  void afterSetAllEdgeValue(PropertyInterface *);

  virtual void addNode(Graph *, const node);
  virtual void addEdge(Graph *, const edge);
  virtual void delNode(Graph *, const node);
  virtual void delEdge(Graph *, const edge);

  // return the id of the corresponding graph elt
  // see ScatterPlot2DMouseShowElementInfo
  // in ScatterPlot2DInteractors.cpp
  unsigned int getMappedId(unsigned int id);

public slots:

  void viewConfigurationChanged();
  void draw() override;
  void refresh() override;
  void applySettings() override;

protected slots:

private:
  void interactorsInstalled(const QList<tlp::Interactor *> &) override;
  void registerTriggers();
  void initGlWidget();
  void buildHistograms();

  void updateDetailedHistogramAxis();
  void destroyHistogramsIfNeeded();
  void cleanupGlScene();
  void addEmptyViewLabel();
  void removeEmptyViewLabel();

  void setLayoutUpdateNeeded() {
    if (detailedHistogram)
      detailedHistogram->setLayoutUpdateNeeded();
  }

  void setSizesUpdateNeeded() {
    if (detailedHistogram)
      detailedHistogram->setSizesUpdateNeeded();
  }

  void setUpdateNeeded() {
    if (detailedHistogram)
      detailedHistogram->setUpdateNeeded();
  }

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

  bool isConstruct;
  unsigned int lastNbHistograms;

  ElementType dataLocation;
  bool needUpdateHistogram;

  Graph *edgeAsNodeGraph;
  std::unordered_map<edge, node> edgeToNode;
  std::unordered_map<node, edge> nodeToEdge;
};
} // namespace tlp

#endif /* HISTOGRAMVIEW_H_ */
