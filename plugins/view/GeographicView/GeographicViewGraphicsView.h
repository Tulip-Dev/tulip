/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef GEOGRAPHIC_VIEW_GRAPHICSVIEW_H
#define GEOGRAPHIC_VIEW_GRAPHICSVIEW_H

#include <tulip/tuliphash.h>

#include "AddressSelectionDialog.h"
#include "GeoMapWidget.h"

#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainWidgetGraphicsItem.h>
#include <tulip/Camera.h>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

class QComboBox;
class QOpenGLFramebufferObject;
class QGraphicsItemGroup;

namespace tlp {

class GeographicView;

class GeographicViewGraphicsView : public QGraphicsView, public Observable {

  Q_OBJECT

public:
  GeographicViewGraphicsView(GeographicView *_geoView, QGraphicsScene *graphicsScene,
                             QWidget *parent = nullptr);
  ~GeographicViewGraphicsView() override;

  Graph *getGraph() {
    return graph;
  }

  void setGraph(Graph *graph);

  void createLayoutWithAddresses(const std::string &addressPropertyName = "",
                                 bool createLatAndLngProps = false,
                                 bool resetLatAndLngValues = true, bool automaticChoice = true);
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

  void centerView();

  void centerMapOnNode(const node n);

  GlMainWidget *getGlMainWidget() {
    return glMainWidget;
  }

  GeoMapWidget *getGeoMapWidget() const {
    return _geoMW;
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

  void treatEvent(const Event &ev) override;

  GlMainWidgetGraphicsItem *getGlMainWidgetItem() {
    return glWidgetItem;
  }

  QGraphicsRectItem *getPlaceHolderItem() const {
    return _placeholderItem;
  }

  void switchMapType();

  void loadFile(const bool isCSV = false, const QString &fileName = "");

  QComboBox *getMapTypeComboBox() {
    return mapTypeComboBox;
  }

  GlComposite *getPolygon() {
    return polygonEntity;
  }

  void setGeoLayoutComputed();

  bool centerVisible() {
    return displayCenter;
  }

  bool scaleVisible() {
    return displayScale;
  }

public slots:

  void mapToPolygon();
  void zoomIn();
  void zoomOut();
  void currentZoomChanged();
  void refreshMap();
  void showGeolocationWidget();
  void openUrlInBrowser(const QString &url);
  void showCenter(bool);
  void showScale(bool);

protected:
  void cleanup();
  void resizeEvent(QResizeEvent *event) override;
  void updateMapTexture();
  void recomputeScale();
  void resizeAttributionLabel();

private:
  GeographicView *_geoView;
  Graph *graph;
  GeoMapWidget *_geoMW;
  tlp_hash_map<node, std::pair<double, double>> nodeLatLng;
  tlp_hash_map<edge, std::vector<std::pair<double, double>>> edgeBendsLatLng;
  Camera globeCameraBackup;
  Camera mapCameraBackup;

  DoubleProperty *latProp, *lngProp;
  LayoutProperty *geoLayout, *geoLayoutBackup;
  SizeProperty *geoViewSize;
  IntegerProperty *geoViewShape;

  bool geocodingActive;
  bool abortGeocoding;

  GlMainWidget *glMainWidget;
  GlMainWidgetGraphicsItem *glWidgetItem;
  QGraphicsRectItem *_placeholderItem;
  QComboBox *mapTypeComboBox;
  QPushButton *zoomOutButton;
  QPushButton *zoomInButton;
  QLabel *attributionLabel;

  GlComposite *polygonEntity;
  GlSimpleEntity *planisphereEntity;

  AddressSelectionDialog *addressSelectionDialog;
  QWidget *noLayoutMsgBox;

  bool firstGlobeSwitch, geoLayoutComputed, displayScale, displayCenter;
  int scaleWidth; // width of the displayed scale
  QGraphicsItemGroup *scale, *center;

  QOpenGLFramebufferObject *renderFbo;
  GlLayer *backgroundLayer;
  std::string mapTextureId;
};
} // namespace tlp

#endif // GEOGRAPHIC_VIEW_GRAPHICSVIEW_H
