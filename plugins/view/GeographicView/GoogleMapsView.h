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

#include "GoogleMapsViewConfigWidget.h"
#include "GeolocalisationConfigWidget.h"
#include "GoogleMaps.h"
#include "GoogleMapsGraphicsView.h"

#include "../../utils/ViewNames.h"

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
 *  <b>LICENCE</b>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 */
class GoogleMapsView : public View {

  Q_OBJECT

  PLUGININFORMATION(ViewName::GoogleMapsViewName, "Antoine Lambert and Mathiaut Morgan", "06/2012",
                    "<p>The Geographic view allows to visualize a geolocated Tulip graph on top of Google Maps.</p>"
                    "<p>If geographic properties are attached to graph nodes (address or latitude/longitude), they are used used to layout the nodes on the map.</p>"
                    "<p>An interactor for performing selection on graph elements is also bundled with the view.</p>", "2.0","View")

public :

  enum ViewType {
    GoogleRoadMap=0,
    GoogleSatellite,
    GoogleTerrain,
    GoogleHybrid,
    Polygon,
    Globe
  };

  GoogleMapsView(PluginContext *);
  ~GoogleMapsView();

  std::string icon() const {
    return ":/geographic_view.png";
  }

  void setupUi();

  QPixmap snapshot(const QSize&) const {
    return QPixmap();
  }

  void setState(const DataSet &dataSet);
  DataSet state() const;

  QGraphicsView *graphicsView() const {
    return googleMapsGraphicsView;
  }

  QList<QWidget *> configurationWidgets() const;

  QGraphicsItem* centralItem() const;

  GoogleMaps *getGoogleMap() {
    return googleMapsGraphicsView->getGoogleMapsPage();
  }

  GoogleMapsGraphicsView *getGoogleMapGraphicsView() {
    return googleMapsGraphicsView;
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

public slots :

  void computeGeoLayout();

  void draw();

  void refresh();

  void graphChanged(Graph *) {
    setState(DataSet());
  }

  void graphDeleted(tlp::Graph*) {}

  void applySettings();

  void updateSharedProperties();

  void currentInteractorChanged(tlp::Interactor *i) {
    i->install(googleMapsGraphicsView->getGlMainWidget());
  }

  void mapToPolygon() {
    googleMapsGraphicsView->mapToPolygon();
  }

  void centerView();

  void viewTypeChanged(QString viewTypeName);

  void zoomIn();
  void zoomOut();
  void currentZoomChanged();

protected slots:

  void fillContextMenu(QMenu *, const QPointF &);

private :

  void updatePoly(bool force=false);

  void loadStoredPolyInformations(const DataSet &dataset);

  void saveStoredPolyInformations(DataSet &dataset) const;

  QGraphicsRectItem* _placeholderItem;
  GoogleMapsGraphicsView *googleMapsGraphicsView;
  GoogleMapsViewConfigWidget *googleMapsViewConfigWidget;
  GeolocalisationConfigWidget *geolocalisationConfigWidget;
  SceneConfigWidget* sceneConfigurationWidget;
  SceneLayersConfigWidget* sceneLayersConfigurationWidget;

  QAction *centerViewAction;
  QAction *showConfPanelAction;

  ViewType _viewType;

  bool useSharedLayoutProperty;
  bool useSharedSizeProperty;
  bool useSharedShapeProperty;

};

}


#endif /* GOOGLEMAPSVIEW_H_ */
