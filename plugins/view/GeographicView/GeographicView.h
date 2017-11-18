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

#ifndef GOOGLEMAPSVIEW_H_
#define GOOGLEMAPSVIEW_H_

#include <tulip/OcclusionTest.h>
#include <tulip/SceneConfigWidget.h>
#include <tulip/SceneLayersConfigWidget.h>
#include <tulip/Interactor.h>
#include <tulip/View.h>

#include <QGraphicsScene>
#include <QResizeEvent>
#include <QGraphicsItem>
#include <QDialog>
#include <QThread>

#include <map>

#include "GeographicViewConfigWidget.h"
#include "GeolocalisationConfigWidget.h"
#include "GoogleMaps.h"
#include "GeographicViewGraphicsView.h"

#include "../../utils/PluginNames.h"

namespace tlp {

/*@{*/
/** \file
 *  \brief  Tulip Geographic View

 * This view plugin allows to visualize a geolocated Tulip graph on top of Google Maps.
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

  PLUGININFORMATION(ViewName::GeographicViewName, "Antoine Lambert and Morgan Mathiaut", "06/2012",
                    "<p>The Geographic view allows to visualize a geolocated Tulip graph on top of "
                    "Google Maps.</p>"
                    "<p>If geographic properties are attached to graph nodes (address or "
                    "latitude/longitude), they are used to layout the nodes on the map.</p>"
                    "<p>An interactor for performing selection on graph elements is also bundled "
                    "with the view.</p>",
                    "2.2", "View")

public:
  enum ViewType { GoogleRoadMap = 0, GoogleSatellite, GoogleTerrain, GoogleHybrid, Polygon, Globe };

  GeographicView(PluginContext *);
  ~GeographicView();

  std::string icon() const {
    return ":/geographic_view.png";
  }

  void setupUi();

  QPixmap snapshot(const QSize &) const;

  void setState(const DataSet &dataSet);
  DataSet state() const;

  QGraphicsView *graphicsView() const {
    return geoViewGraphicsView;
  }

  QList<QWidget *> configurationWidgets() const;

  QGraphicsItem *centralItem() const;

  GoogleMaps *getGoogleMap() {
    return geoViewGraphicsView->getGoogleMapsPage();
  }

  void registerTriggers();

  ViewType viewType() {
    return _viewType;
  }

  // inherited from View
  virtual void centerView(bool) {
    // call the Qt slot declared below
    centerView();
  }

  GeographicViewGraphicsView *getGeographicViewGraphicsView() const {
    return geoViewGraphicsView;
  }

public slots:

  void computeGeoLayout();

  void draw();

  void refresh();

  void graphChanged(Graph *) {
    setState(DataSet());
  }

  void graphDeleted(tlp::Graph *) {}

  void applySettings();

  void updateSharedProperties();

  void currentInteractorChanged(tlp::Interactor *i) {
    i->install(geoViewGraphicsView->getGlMainWidget());
  }

  void mapToPolygon() {
    geoViewGraphicsView->mapToPolygon();
  }

  void centerView();

  void viewTypeChanged(QString viewTypeName);

  void zoomIn();
  void zoomOut();
  void currentZoomChanged();

  void openSnapshotDialog();

protected slots:

  void fillContextMenu(QMenu *, const QPointF &);

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

  QAction *centerViewAction;
  QAction *showConfPanelAction;

  ViewType _viewType;

  bool useSharedLayoutProperty;
  bool useSharedSizeProperty;
  bool useSharedShapeProperty;

  double mapCenterLatitudeInit, mapCenterLongitudeInit;
  int mapZoomInit;
};
}

#endif /* GOOGLEMAPSVIEW_H_ */
