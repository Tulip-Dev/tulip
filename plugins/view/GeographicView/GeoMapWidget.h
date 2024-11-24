/*
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
#ifndef GEOMAPWIDGET_H
#define GEOMAPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPixmapCache>
#include <QPoint>
#include <QPointF>
#include <QMutex>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QRectF>

#include <set>
#include <tulip/tuliphash.h>
#include <utility>

#include <tulip/Coord.h>
#include <tulip/Graph.h>

// The code of GeoMapWidget has been greatly inspired by many codes readings
// such as :
// https://github.com/AmonRaNet/QGeoView
// https://github.com/raptorswing/MapGraphics
// https://sourceforge.net/projects/qmapcontrol
// https://sourceforge.net/projects/qgmview
// https://sourceforge.net/p/simultra/code/ci/master/tree/mapgraphics
// The goal was to replace our previous implementation using Leaflet stuff
// needing a heavy Qt web browser engine (Qt WebEngine or Qt Webkit modules)
// by a lighter implementation only based on the Qt GUI and Qt Network modules.

struct MapLayer {
  const char *name;
  const char *url;
  const char *attrib;
  int maxZoom;
  bool dark;

  MapLayer(const char *name, const char *url = nullptr, const char *attrib = nullptr,
           int maxZoom = 21)
      : name(name), url(url), attrib(attrib), maxZoom(maxZoom) {
    dark = std::string(name).find(" Dark ") != std::string::npos;
  }
};

class GeoMapWidget : public QWidget {
  Q_OBJECT

public:
  // The constructor of GeoMapWidget
  /*
   * The GeoMapWidget is the widget which displays the maps.
   * The size describes the area which gets filled with map data
   * @param parent QWidget parent
   * @param windowFlags QWidget Window flags
   */
  GeoMapWidget(const std::vector<MapLayer> &mapLayers, QSize size = QSize(0, 0),
               QWidget *parent = 0, Qt::WindowFlags windowFlags = Qt::WindowFlags());

  ~GeoMapWidget() {
    delete http;
  }

  // returns the geo coordinates of the center
  // of the map visible region
  std::pair<double, double> getMapCenter() const {
    return std::make_pair<double, double>(centerM.x(), centerM.y());
  }

  // returns the geo coordinates of the bottom left corner
  // of the map visible region
  std::pair<double, double> getMapSouthWest();

  // returns the geo coordinates of the top right corner
  // of the map visible region
  std::pair<double, double> getMapNorthEast();

  int getCurrentZoom() const {
    return currentZoom;
  }

  void setCurrentZoom(int zoom) {
    setZoom(zoom);
  }

  int getMinZoom() const {
    return minZoom;
  }

  int getMaxZoom() const {
    return maxZoom;
  }

  // center the map visible region to the given geo coordinates
  void setMapCenter(const QPointF &coordinates);

  // center the map to the given geo point
  inline void setMapCenter(double latitude, double longitude) {
    setMapCenter(QPointF(longitude, latitude));
  }

  // set the map visible region to the one enclosing the given nodes
  // and geo coordinates
  void setMapBounds(tlp::Graph *graph,
                    const tlp_hash_map<tlp::node, std::pair<double, double>> &nodesLatLngs);

  // set the map visible region to the one enclosing
  // the south west and north east given points
  void zoomOnRectangle(std::pair<double, double> &sw, std::pair<double, double> &ne);

  void resizeEvent(QResizeEvent *ev);

  // geographic world coordinates => screen position
  QPoint geoToScreenPos(const QPointF &coordinate) const;

  inline tlp::Coord geoToScreenPos(double lat, double lng) const {
    auto screenPos = geoToScreenPos(QPointF(lng, lat));
    return tlp::Coord(screenPos.x(), screenPos.y());
  }

  // screen position => geographic world coordinates
  QPointF screenToGeoPos(const QPoint &point) const;

  std::pair<double, double> screenToGeoPos(int x, int y) const {
    auto geoPos = screenToGeoPos(QPoint(x, y));
    return std::make_pair<double, double>(geoPos.y(), geoPos.x());
  }

  // Get the visible rect of the map in pixel coordinates
  QRect getScreenViewport() const;

  // Get the visible rect of the map in geographic coordinates
  QRectF getGeoViewport() const;

  // view translation
  void translateView(const QPoint &offset);

  // map layer management
  void setMapLayer(QString name, QString templUrl, QString attribution, int minZoom, int maxZoom);

  void switchToMapLayer(unsigned int layer);

  void switchToCustomTileLayer(QString url, QString attrib);

  bool mapLoaded() {
    return loadingQueueEmpty();
  }

  static constexpr float initialCenterLat = 44.8084;
  static constexpr float initialCenterLng = -40;
  static constexpr int initialZoom = 3;

private:
  void init();
  void configureTiling(int zoom);

  void draw(QPainter *painter, const QPoint &wCenterM);
  bool isTileValid(int x, int y, int z) const;

  QPixmap getTile(int x, int y, int z);
  void loadTile(const QString &url);
  bool loadingQueueEmpty() const;
  void abortPendingLoads();

  // gui interaction
  QPoint wCenter; // widget center
  QPoint offscreenPxTranslation;
  QPoint zoomPxTranslation;

  QPoint mousePreviousPos; // used for view translation

  QSize size; // size of the widget

  bool mousePressed;

  QPixmap offscreenPx;
  QPixmap zoomPx;

  QPointF centerM; // world coordinate
  QPoint wCenterM; // projection in display coordinates

  QPixmap *m_doubleBuffer;

  // map layer config
  const std::vector<MapLayer> &mapLayers;
  unsigned int currentLayer;
  int minZoom, maxZoom, currentZoom, tilingSize;
  QString lname;
  QString tileTemplUrl;
  QString attribution;

  mutable QRect offscreenViewport;

  // Tiles loading from network
  QPixmap emptyTile;
  QPixmap loadingTile;

  QNetworkAccessManager *http;
  QList<QNetworkReply *> replyList;

  QHash<QString, QDateTime> failedFetches;

  std::set<QString> loadingUrls;
  mutable QMutex mtx;

  void newOffscreenImage(bool clearImage = true, bool showZoomImage = true);
  void centerMap(const QList<QPointF> &coordinates);

protected:
  void mousePressEvent(QMouseEvent *ev);
  void mouseReleaseEvent(QMouseEvent *ev);
  void mouseMoveEvent(QMouseEvent *ev);
  void wheelEvent(QWheelEvent *ev);
  void paintEvent(QPaintEvent *ev);

signals:
  void currentZoomChanged();
  void refreshMap();
  void mouseMove();

public slots:
  // zooms in one step
  void zoomIn();

  // zooms out one step
  void zoomOut();

  // sets the given zoomlevel
  void setZoom(int zoomlevel);

  // updates the whole map by creating a new offscreen image
  void forceRedraw();

private slots:
  void requestFinished(QNetworkReply *reply);
  // void moveStep();
};
#endif
