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

#ifndef GEOGRAPHIC_VIEW_H
#define GEOGRAPHIC_VIEW_H

#include <tulip/OcclusionTest.h>
#include <tulip/SceneConfigWidget.h>
#include <tulip/SceneLayersConfigWidget.h>
#include <tulip/Interactor.h>
#include <tulip/View.h>
#include <tulip/GlMainView.h>
#include <tulip/ViewActionsManager.h>

#include <QGraphicsScene>
#include <QResizeEvent>
#include <QGraphicsItem>
#include <QDialog>
#include <QThread>
#include <QMap>

#include <map>

#include "GeographicViewConfigWidget.h"
#include "GeolocalisationConfigWidget.h"
#include "LeafletMaps.h"
#include "GeographicViewGraphicsView.h"

#include "../../utils/PluginNames.h"

namespace tlp {

/*@{*/
/** \file
 *  \brief  Tulip Geographic View

 * This view plugin allows to visualize a geolocated Tulip graph on top of maps.
 * If geographic properties are attached to graph nodes (address or latitude/longitude), this
 * plugin uses them to layout the nodes on the map.
 *
 * An interactor for performing selection on graph elements is also bundled
 * with the view.
 *
 *
 */
class GeographicView : public View {

  Q_OBJECT

  PLUGININFORMATION(
      ViewName::GeographicViewName, "Antoine Lambert and Morgan Mathiaut", "06/2012",
      "<p>The Geographic view allows to visualize a geolocated Tulip graph on top of "
      "maps or projected on a globe.</p>"
      "<p>If geographic properties are attached to graph nodes (address or "
      "latitude/longitude), they are used to layout the nodes on the maps or on the globe.</p>"
      "<p>An interactor for performing selection on graph elements is also bundled "
      "with the view.</p>",
      "3.1", "View")

public:
  enum ViewType {
    OpenStreetMap = 0,
    OpenTopoMap,
    EsriStreetMap,
    EsriTopoMap,
    EsriNatGeoMap,
    EsriSatellite,
    EsriLightGrayCanvas,
    EsriDarkGrayCanvas,
    CartoDB,
    CartoDBLight,
    CartoDBDark,
    WikimediaMap,
    LeafletCustomTileLayer,
    Polygon,
    Globe
  };

  GeographicView(PluginContext *);
  ~GeographicView() override;

  std::string icon() const override {
    return ":/tulip/view/geographic/geographic_view.png";
  }

  void setupUi() override;

  QPixmap snapshot(const QSize &) const override;

  void setState(const DataSet &dataSet) override;
  DataSet state() const override;

  QGraphicsView *graphicsView() const override {
    return geoViewGraphicsView;
  }

  QList<QWidget *> configurationWidgets() const override;

  QGraphicsItem *centralItem() const override;

  LeafletMaps *getLeafletMap() {
    return geoViewGraphicsView->getLeafletMapsPage();
  }

  void registerTriggers();

  ViewType viewType() {
    return _viewType;
  }

  // inherited from View
  void centerView(bool) override {
    // call the Qt slot declared below
    centerView();
  }

  GeographicViewGraphicsView *getGeographicViewGraphicsView() const {
    return geoViewGraphicsView;
  }

  bool getNodeOrEdgeAtViewportPos(int x, int y, node &n, edge &e) const override {
    return GlMainView::getNodeOrEdgeAtViewportPos(geoViewGraphicsView->getGlMainWidget(), x, y, n,
                                                  e);
  }

  GeographicViewConfigWidget *getConfigWidget() const {
    return geoViewConfigWidget;
  }

public slots:

  void computeGeoLayout();

  void draw() override;

  void refresh() override;

  void graphChanged(tlp::Graph *) override;

  void graphDeleted(tlp::Graph *) override {}

  void applySettings() override;

  void updateSharedProperties();

  void currentInteractorChanged(tlp::Interactor *i) override {
    i->install(geoViewGraphicsView->getGlMainWidget());
  }

  void mapToPolygon() {
    geoViewGraphicsView->mapToPolygon();
  }

  void centerView();
  void centerOnNode();

  void viewTypeChanged(QString viewTypeName);

  void zoomIn();
  void zoomOut();
  void currentZoomChanged();

  ViewType getViewTypeFromName(const QString &name) const;
  const char *getViewNameFromType(ViewType viewType) const;

protected slots:

  void fillContextMenu(QMenu *, const QPointF &) override;

  void initMap();

private:
  void updatePoly(bool force = false);

  void loadStoredPolyInformation(const DataSet &dataset);

  void saveStoredPolyInformation(DataSet &dataset) const;

  GeographicViewGraphicsView *geoViewGraphicsView;
  GeographicViewConfigWidget *geoViewConfigWidget;
  GeolocalisationConfigWidget *geolocalisationConfigWidget;
  SceneConfigWidget *sceneConfigurationWidget;
  SceneLayersConfigWidget *sceneLayersConfigurationWidget;

  QAction *centerOnNodeAction;
  QAction *showConfPanelAction;

  ViewType _viewType;

  bool useSharedLayoutProperty;
  bool useSharedSizeProperty;
  bool useSharedShapeProperty;

  double mapCenterLatitudeInit, mapCenterLongitudeInit;
  int mapZoomInit;
  ViewActionsManager *_viewActionsManager;

  QMap<ViewType, const char *> _viewTypeToName;
  node _nodeUnderMouse;
};
} // namespace tlp

#endif // GEOGRAPHIC_VIEW_H
