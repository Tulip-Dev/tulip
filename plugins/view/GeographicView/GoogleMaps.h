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

#ifndef GOOGLEMAPS_H
#define GOOGLEMAPS_H

#ifdef QT_HAS_WEBKIT
#include <QWebView>
#include <QWebFrame>
#else
#include <QWebEngineView>
#include <QEventLoop>
#endif
#include <tulip/Coord.h>
#include <tulip/Graph.h>

#include "ProgressWidgetGraphicsProxy.h"
#include "AddressSelectionDialog.h"

namespace tlp {

#ifdef QT_HAS_WEBENGINE

class MapRefresher : public QObject {

  Q_OBJECT

public slots:

  void refreshMap();

signals:

  void refreshMapSignal();
};

#endif

#ifdef QT_HAS_WEBKIT
class GoogleMaps : public QWebView {
#else
class GoogleMaps : public QWebEngineView {
#endif

  Q_OBJECT

public:
  GoogleMaps(QWidget *parent = 0);

  void setMapCenter(double latitude, double longitude);

  int getCurrentMapZoom();

  void setCurrentZoom(int zoom);

  std::pair<double, double> getCurrentMapCenter();

  std::string getLatLngForAddress(const QString &address, std::pair<double, double> &latLng,
                                  bool skipMultipleResults = false);

  Coord getPixelPosOnScreenForLatLng(double lat, double lng);

  Coord mercatorProjection(const Coord &swPixel, const Coord &nePixel, const double latitude,
                           const double longitude);

  std::pair<double, double> getLatLngForPixelPosOnScreen(int x, int y);

  bool pageInit() const {
    return init;
  }

  void setMapBounds(Graph *graph, const std::map<node, std::pair<double, double> > &nodesLatLngs);

  void setMapBounds(Coord nw, Coord se);

  void panMap(int dx, int dy);

  std::pair<double, double> getMapCurrentSouthWestLatLng();

  std::pair<double, double> getMapCurrentNorthEastLatLng();

  int getWorldWidth();

  void wheelEvent(QWheelEvent *ev);

  void mouseMoveEvent(QMouseEvent *ev);
  void mousePressEvent(QMouseEvent *ev);

  void switchToSatelliteView();
  void switchToRoadMapView();
  void switchToTerrainView();
  void switchToHybridView();

  void setProgressWidget(ProgressWidgetGraphicsProxy *progressWidget) {
    this->progressWidget = progressWidget;
  }

  void setAdresseSelectionDialog(AddressSelectionDialog *addressSelectionDialog,
                                 QGraphicsProxyWidget *addresseSelectionProxy) {
    this->addressSelectionDialog = addressSelectionDialog;
    this->addresseSelectionProxy = addresseSelectionProxy;
  }

signals:

  void currentZoomChanged();
  void refreshMap();

private slots:

  void triggerLoading();

private:
  QVariant executeJavascript(const QString &jsCode);

  bool init;
#ifdef QT_HAS_WEBKIT
  QWebFrame *frame;
#else
  QWebEnginePage *frame;
#endif
  int x, y;

  AddressSelectionDialog *addressSelectionDialog;
  QGraphicsProxyWidget *addresseSelectionProxy;
  ProgressWidgetGraphicsProxy *progressWidget;

#ifdef QT_HAS_WEBENGINE
  MapRefresher *mapRefresher;
#endif
};

#ifdef QT_HAS_WEBENGINE

// Use this class as a hack to simulate synchronous javascript callbacks
// as QtWebEngine is asynchronous while QtWebKit was not
class JsCallback : public QObject {

  Q_OBJECT

public:
  JsCallback(QVariant *ret) : _ret(ret) {
    _lastCreatedInstance = this;
  }

  JsCallback(const JsCallback &jscb) : QObject() {
    _ret = jscb._ret;
    _lastCreatedInstance = this;
  }

  void operator()(const QVariant &result) {
    *_ret = result;
    emit called();
  }

  static void waitForCallback() {
    QEventLoop loop;
    connect(_lastCreatedInstance, SIGNAL(called()), &loop, SLOT(quit()));
    loop.exec();
  }

signals:

  void called();

private:
  QVariant *_ret;

  static JsCallback *_lastCreatedInstance;
};

#endif
}

#endif // GOOGLEMAPS_H
