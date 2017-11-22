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

#ifndef GOOGLEMAPSGRAPHICSVIEW_H
#define GOOGLEMAPSGRAPHICSVIEW_H

#include "GoogleMaps.h"

#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainWidgetGraphicsItem.h>
#include <tulip/Camera.h>

#include <QGraphicsView>
#include <QComboBox>

namespace tlp {

class GeographicView;

class GeographicViewGraphicsView : public QGraphicsView, public Observable {

  Q_OBJECT

public:
  GeographicViewGraphicsView(GeographicView *_geoView, QGraphicsScene *graphicsScene,
                             QWidget *parent = nullptr);
  ~GeographicViewGraphicsView();

  void setGraph(Graph *graph);
  void createLayoutWithAddresses(const std::string &addressPropertyName, bool createLatAndLngProps);
  void createLayoutWithLatLngs(const std::string &latitudePropertyName,
                               const std::string &longitudePropertyName,
                               const std::string &edgesPathsPropertyName);

  GlGraphComposite *getGlGraphComposite() const;

  std::pair<double, double> getLatLngForNode(node n) {
    return nodeLatLng[n];
  }

  void setNodeLatLng(node n, const std::pair<double, double> &latLng) {
    nodeLatLng[n] = latLng;
  }

  void setEdgeBendsLatLng(edge e, const std::vector<std::pair<double, double>> &bendsLatLng) {
    edgeBendsLatLng[e] = bendsLatLng;
  }

  void draw() {
    glWidgetItem->setRedrawNeeded(true);
    scene()->update();
  }

  void setMapTranslationBlocked(const bool mapTranslationBlocked);

  void centerView();

  void centerMapOnNode(const node n);

  GlMainWidget *getGlMainWidget() {
    return glMainWidget;
  }

  GoogleMaps *getGoogleMapsPage() const {
    return googleMaps;
  }

  LayoutProperty *getGeoLayout() const {
    return geoLayout;
  }

  SizeProperty *getGeoSizes() const {
    return geoViewSize;
  }

  void setGeoLayout(LayoutProperty *);

  void setGeoSizes(SizeProperty *);

  void setGeoShape(IntegerProperty *);

  void treatEvent(const Event &ev);

  void afterSetNodeValue(PropertyInterface *, const node);

  void afterSetAllNodeValue(PropertyInterface *);

  void forceRedraw() {
    currentMapZoom = 0;
  }

  GlMainWidgetGraphicsItem *getGlMainWidgetItem() {
    return glWidgetItem;
  }

  QGraphicsRectItem *getPlaceHolderItem() const {
    return _placeholderItem;
  }

  void switchViewType();

  void loadDefaultMap();
  void loadCsvFile(QString fileName);
  void loadPolyFile(QString fileName);

  QComboBox *getViewTypeComboBox() {
    return viewTypeComboBox;
  }

  GlComposite *getPolygon() {
    return polygonEntity;
  }

  void setGeoLayoutComputed();

public slots:

  void mapToPolygon();
  void zoomIn();
  void zoomOut();
  void currentZoomChanged();
  void queueMapRefresh();
  void refreshMap();

protected:
  void cleanup();

  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);

private:
  GeographicView *_geoView;
  GlMainWidget *glWidget;
  Graph *graph;
  GoogleMaps *googleMaps;
  std::map<node, std::pair<double, double>> nodeLatLng;
  std::map<node, std::pair<double, double>> nodeLatLngFOR;
  std::map<edge, std::vector<std::pair<double, double>>> edgeBendsLatLng;
  std::pair<double, double> currentMapCenter;
  unsigned int currentMapZoom;
  Camera globeCameraBackup;
  Camera mapCameraBackup;
  QRectF lastSceneRect;

  LayoutProperty *geoLayout;
  SizeProperty *geoViewSize;
  IntegerProperty *geoViewShape;
  LayoutProperty *geoLayoutBackup;

  bool mapTranslationBlocked;

  bool geocodingActive;
  bool cancelGeocoding;

  GlMainWidget *glMainWidget;
  GlMainWidgetGraphicsItem *glWidgetItem;
  QComboBox *viewTypeComboBox;
  QPushButton *zoomOutButton;
  QPushButton *zoomInButton;

  GlComposite *polygonEntity;
  GlSimpleEntity *planisphereEntity;

  AddressSelectionDialog *addressSelectionDialog;
  QGraphicsProxyWidget *addresseSelectionProxy;
  ProgressWidgetGraphicsProxy *progressWidget;
  QGraphicsProxyWidget *noLayoutMsgBox;

  bool firstGlobeSwitch;
  bool firstMapSwitch;

  std::pair<double, double> prevMapCenter;
  int prevMapZoom;
  QGraphicsRectItem *_placeholderItem;

  bool geoLayoutComputed;

  static unsigned int planisphereTextureId;
};
}

#endif // GOOGLEMAPSGRAPHICSVIEW_H
