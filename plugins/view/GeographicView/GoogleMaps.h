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

#ifndef GOOGLEMAPS_H
#define GOOGLEMAPS_H

#include <QWebView>
#include <QWebFrame>

#include <tulip/Coord.h>
#include <tulip/Graph.h>

#include "ProgressWidgetGraphicsProxy.h"
#include "AddressSelectionDialog.h"

namespace tlp {

class GoogleMaps : public QWebView {

  Q_OBJECT

public :

  GoogleMaps(QWidget *parent = 0);

  void setMapCenter(double latitude, double longitude);

  int getCurrentMapZoom();

  void setCurrentZoom(int zoom);

  std::pair<double, double> getCurrentMapCenter();

  std::string getLatLngForAddress(const QString &address, std::pair<double, double> &latLng, bool skipMultipleResults = false);

  Coord getPixelPosOnScreenForLatLng(double lat, double lng);

  Coord mercatorProjection(const Coord &swPixel, const Coord &nePixel, const double latitude, const double longitude);

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

  QWebFrame *getMainFrame() const {
    return frame;
  }

  void wheelEvent(QWheelEvent * ev);

  void mouseMoveEvent(QMouseEvent * ev);
  void mousePressEvent(QMouseEvent * ev);

  void switchToSatelliteView();
  void switchToRoadMapView();
  void switchToTerrainView();
  void switchToHybridView();

  void setProgressWidget(ProgressWidgetGraphicsProxy *progressWidget) {
    this->progressWidget=progressWidget;
  }

  void setAdresseSelectionDialog(AddressSelectionDialog *addressSelectionDialog,QGraphicsProxyWidget *addresseSelectionProxy) {
    this->addressSelectionDialog=addressSelectionDialog;
    this->addresseSelectionProxy=addresseSelectionProxy;
  }

signals:

  void currentZoomChanged();
  void refreshMap();

private slots :

  void triggerLoading();

private :

  bool init;
  QWebFrame *frame;
  int x, y;

  AddressSelectionDialog *addressSelectionDialog;
  QGraphicsProxyWidget *addresseSelectionProxy;
  ProgressWidgetGraphicsProxy *progressWidget;
};

}

#endif // GOOGLEMAPS_H
