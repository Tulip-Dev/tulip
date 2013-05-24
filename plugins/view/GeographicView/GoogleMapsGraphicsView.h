/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <QGLWidget>
#include <QComboBox>

namespace tlp {

class GoogleMapsView;

class GoogleMapsGraphicsView : public QGraphicsView, public Observable {

public :

  GoogleMapsGraphicsView(GoogleMapsView *_googleMapsView, QGraphicsScene *graphicsScene, QWidget *parent = 0);
  ~GoogleMapsGraphicsView();

  void setGraph(Graph *graph);
  void createLayoutWithAddresses(std::string addressPropertyName, bool createLatAndLngProps);
  void createLayoutWithLatLngs(std::string latitudePropertyName, std::string longitudePropertyName);

  GlGraphComposite *getGlGraphComposite() const;

  std::pair<double, double> getLatLngForNode(node n) {
    return nodeLatLng[n];
  }

  void setNodeLatLng(node n, const std::pair<double, double> &latLng) {
    nodeLatLng[n] = latLng;
  }

  void setEdgeBendsLatLng(edge e, const std::vector<std::pair< double, double> > &bendsLatLng) {
    edgeBendsLatLng[e] = bendsLatLng;
  }

  void draw() {
    scene()->update();
  }

  void setMapTranslationBlocked(const bool mapTranslationBlocked);

  void centerView();

  void centerMapOnNode(const node n);

  QGLWidget *getGLWidget() {
    return glWidget;
  }

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

  void treatEvent(const Event& ev);

  void afterSetNodeValue(PropertyInterface*, const node);

  void afterSetAllNodeValue(PropertyInterface*);

  void forceRedraw() {
    currentMapZoom = 0;
  }

  GlMainWidgetGraphicsItem *getGlMainWidgetItem() {
    return glWidgetItem;
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

public slots :

  void mapToPolygon();

protected :

  void cleanup();

  void paintEvent (QPaintEvent * event);
  void resizeEvent(QResizeEvent *event);

private :

  GoogleMapsView *_googleMapsView;
  QGLWidget *glWidget;
  Graph *graph;
  GoogleMaps *googleMaps;
  std::map<node, std::pair<double, double> > nodeLatLng;
  std::map<node, std::pair<double, double> > nodeLatLngFOR;
  std::map<edge, std::vector<std::pair<double, double> > > edgeBendsLatLng;
  std::pair<double, double> currentMapCenter;
  unsigned int currentMapZoom;
  Camera globeCameraBackup;
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
  GlComposite *polygonEntity;
  GlSimpleEntity *planisphereEntity;

  AddressSelectionDialog *addressSelectionDialog;
  QGraphicsProxyWidget *addresseSelectionProxy;
  ProgressWidgetGraphicsProxy *progressWidget;

  bool firstGlobeSwitch;
};

}

#endif // GOOGLEMAPSGRAPHICSVIEW_H
