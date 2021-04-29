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

#ifndef GEOGRAPHIC_VIEW_GRAPHICSVIEW_H
#define GEOGRAPHIC_VIEW_GRAPHICSVIEW_H

#include <unordered_map>

#include "AddressSelectionDialog.h"
#include "LeafletMaps.h"

#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainWidgetGraphicsItem.h>
#include <tulip/Camera.h>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QComboBox>

class QOpenGLFramebufferObject;

namespace tlp {

class GeographicView;

class GeographicViewGraphicsView : public QGraphicsView, public Observable {

  Q_OBJECT

public:
  GeographicViewGraphicsView(GeographicView *_geoView, QGraphicsScene *graphicsScene,
                             QWidget *parent = nullptr);
  ~GeographicViewGraphicsView() override;

  void setGraph(Graph *graph);
  void createLayoutWithAddresses(const std::string &addressPropertyName, bool createLatAndLngProps,
                                 bool resetLatAndLngValues, bool automaticChoice);
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

  LeafletMaps *getLeafletMapsPage() const {
    return leafletMaps;
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
#ifdef QT_HAS_WEBENGINE
  void queueMapRefresh();
#endif
  void refreshMap();

protected:
  void cleanup();
  void resizeEvent(QResizeEvent *event) override;
#ifdef QT_HAS_WEBENGINE
  int tId;
  void timerEvent(QTimerEvent *event) override;
#endif
  void updateMapTexture();

private:
  GeographicView *_geoView;
  Graph *graph;
  LeafletMaps *leafletMaps;
  std::unordered_map<node, std::pair<double, double>> nodeLatLng;
  std::unordered_map<edge, std::vector<std::pair<double, double>>> edgeBendsLatLng;
  Camera globeCameraBackup;
  Camera mapCameraBackup;

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
  QGraphicsProxyWidget *noLayoutMsgBox;

  bool firstGlobeSwitch;

  QGraphicsRectItem *_placeholderItem;

  bool geoLayoutComputed;

  QOpenGLFramebufferObject *renderFbo;
  GlLayer *backgroundLayer;
  std::string mapTextureId;
  DoubleProperty *latProp, *lngProp;
};
} // namespace tlp

#endif // GEOGRAPHIC_VIEW_GRAPHICSVIEW_H
