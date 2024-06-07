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
#include <cmath>

#include <QDebug>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QScreen>
#include <QTimer>

#include "GeoMapWidget.h"

#define TILE_SIZE 256

#define RAD_TO_DEG(x) (x * (180 / M_PI))
#define DEG_TO_RAD(x) (x * (M_PI / 180.0))

static const int kDefaultTimeoutDelaySecs = 30;
static const int kDefaultPixmapCacheSizeKB = 20000;

GeoMapWidget::GeoMapWidget(const std::vector<MapLayer> &mapLayers, QSize size, QWidget *parent,
                           Qt::WindowFlags windowFlags)
    : QWidget(parent, windowFlags), offscreenPxTranslation(QPoint(0, 0)), size(size),
      mousePressed(false), m_doubleBuffer(nullptr), mapLayers(mapLayers), currentLayer(-1),
      emptyTile(QPixmap(1, 1)), loadingTile(QPixmap(256, 256)),
      http(new QNetworkAccessManager(this)) {
  init();
}

void GeoMapWidget::setMapLayer(QString name, QString templUrl, QString attrib, int minZ, int maxZ,
                               bool) {
  if (name == lname)
    return;

  bool needUpdate = !lname.isEmpty();

  // forget current map tiles loading
  if (needUpdate)
    abortPendingLoads();

  // record new configuration
  lname = name;
  tileTemplUrl = templUrl;
  attribution = attrib;
  minZoom = minZ;
  maxZoom = maxZ;

  // adapt currentZoom if needed
  if (currentZoom < minZoom)
    currentZoom = minZoom;
  if (currentZoom > maxZoom)
    currentZoom = maxZoom;
  nbTiles = nbZoomTiles();

  if (needUpdate) {
    forceRedraw();
    emit refreshMap();
  }
}

void GeoMapWidget::switchToMapLayer(unsigned int layer) {
  if (currentLayer == layer)
    return;
  currentLayer = layer;
  const MapLayer &mapLayer = mapLayers[currentLayer];
  // first check zoom
  if (mapLayer.maxZoom < getCurrentZoom())
    setCurrentZoom(mapLayer.maxZoom);
  setMapLayer(mapLayer.name, mapLayer.url, mapLayer.attrib, 0, mapLayer.maxZoom, mapLayer.dark);
}

void GeoMapWidget::switchToCustomTileLayer(QString url, QString attrib) {
  setMapLayer("custom", url, attrib, 0, 21);
}

void GeoMapWidget::init() {
  if (size.isNull())
    size = QGuiApplication::primaryScreen()->size();

  wCenter = QPoint(size.width() / 2, size.height() / 2);

  mousePressed = false;

  offscreenPx = QPixmap(2 * size);
  zoomPx = QPixmap(size);
  zoomPx.fill(Qt::white);

  // init loading stuff
  emptyTile.fill(Qt::transparent);
  loadingTile.fill(Qt::transparent);
  QPainter painter(&loadingTile);
  QBrush brush(Qt::lightGray, Qt::Dense5Pattern);
  painter.fillRect(loadingTile.rect(), brush);
  painter.end();

  if (QPixmapCache::cacheLimit() <= kDefaultPixmapCacheSizeKB)
    QPixmapCache::setCacheLimit(kDefaultPixmapCacheSizeKB);

  connect(http, SIGNAL(finished(QNetworkReply *)), this, SLOT(requestFinished(QNetworkReply *)));

  // init default map layer
  currentZoom = 3;
  switchToMapLayer(0);
  setMapCenter(QPointF(-40, 44.8084));

  update();
}

/*
void GeoMapWidget::moveTo(QPointF &coordinate) {
  target = coordinate;
  steps = 25;
  if (moveMutex.tryLock())
    QTimer::singleShot(40, this, SLOT(moveStep()));
  else
    moveMutex.unlock();
}

void GeoMapWidget::moveStep() {
  QPoint start = geoToScreenPos(centerM);
  QPoint dest = geoToScreenPos(target);

  QPoint step = (dest-start)/steps;
  translateView(step);

  update();
  forceRedraw();
  if (--steps > 0)
    QTimer::singleShot(50, this, SLOT(moveStep()));
  else
    moveMutex.unlock();
}
*/

void GeoMapWidget::paintEvent(QPaintEvent *) {
  if (m_doubleBuffer == nullptr) {
    m_doubleBuffer = new QPixmap(width(), height());
  }
  // check for resize change
  else if (m_doubleBuffer->width() != width() || m_doubleBuffer->height() != height()) {
    delete m_doubleBuffer;
    m_doubleBuffer = new QPixmap(width(), height());
  }
  QPainter dbPainter(m_doubleBuffer);

  dbPainter.drawPixmap(-offscreenPxTranslation.x() - wCenter.x(),
                       -offscreenPxTranslation.y() - wCenter.y(), offscreenPx);
  dbPainter.end();

  // finally draw in widget
  QPainter painter(this);
  painter.drawPixmap(rect(), *m_doubleBuffer, m_doubleBuffer->rect());
}

// mouse events
void GeoMapWidget::mousePressEvent(QMouseEvent *ev) {
  if (ev->button() == Qt::LeftButton) {
    mousePressed = true;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    mousePreviousPos = ev->pos();
#else
    mousePreviousPos = ev->position().toPoint();
#endif
  }
}

void GeoMapWidget::mouseReleaseEvent(QMouseEvent *) {
  mousePressed = false;
}

void GeoMapWidget::mouseMoveEvent(QMouseEvent *ev) {
  if (mousePressed) {
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    auto evPos = ev->pos();
#else
    auto evPos = ev->position().toPoint();
#endif
    translateView(mousePreviousPos - evPos);
    mousePreviousPos = evPos;
    update();
    emit refreshMap();
    emit mouseMove();
  }
}

void GeoMapWidget::wheelEvent(QWheelEvent *ev) {
  auto evPos = ev->position().toPoint();
  if (ev->angleDelta().y()) {
    // zoom in/out
    if (ev->angleDelta().y() > 0) {
      zoomIn();
      // ensure the map point under mouse cursor stay fixed
      QPoint newCenter = wCenterM + (evPos - wCenter);
      setMapCenter(screenToGeoPos(newCenter));
    } else {
      // ensure the map point under mouse cursor stay fixed
      QPoint newCenter = wCenterM - (evPos - wCenter);
      setMapCenter(screenToGeoPos(newCenter));
      zoomOut();
    }
    ev->accept();
  } else
    ev->ignore();
}

void GeoMapWidget::newOffscreenImage(bool clearImage, bool showZoomImage) {
  offscreenPxTranslation = QPoint(0, 0);

  if (clearImage || loadingQueueEmpty())
    offscreenPx.fill(Qt::white);

  QPainter painter(&offscreenPx);
  if (showZoomImage || !loadingQueueEmpty()) {
    painter.drawPixmap(wCenter.x() - zoomPxTranslation.x(), wCenter.y() - zoomPxTranslation.y(),
                       zoomPx);
  }

  draw(&painter, wCenterM);
  painter.end();

  update();
}

void GeoMapWidget::zoomIn() {
  if (currentZoom == maxZoom)
    return;

  abortPendingLoads();

  // prepare pixel zoomed image
  // from current offscreen image
  zoomPxTranslation = QPoint(0, 0);
  zoomPx.fill(Qt::white);

  QPixmap tmpImg =
      offscreenPx.copy(wCenter.x() + offscreenPxTranslation.x(),
                       wCenter.y() + offscreenPxTranslation.y(), size.width(), size.height());
  QPainter painter(&zoomPx);
  painter.translate(wCenter);
  painter.scale(2, 2);
  painter.translate(-wCenter);
  painter.drawPixmap(0, 0, tmpImg);
  painter.end();

  // increase zoom
  ++currentZoom;
  emit currentZoomChanged();
  nbTiles = nbZoomTiles();
  wCenterM = geoToScreenPos(centerM);

  // show scaled image while map loads
  newOffscreenImage(true, true);
  emit refreshMap();
}

void GeoMapWidget::zoomOut() {
  if (currentZoom == minZoom)
    return;

  abortPendingLoads();

  // prepare pixel zoomed image
  // from current offscreen one
  zoomPxTranslation = QPoint(0, 0);
  zoomPx.fill(Qt::white);
  QPixmap tmpImg =
      offscreenPx.copy(wCenter.x() + offscreenPxTranslation.x(),
                       wCenter.y() + offscreenPxTranslation.y(), size.width(), size.height());
  QPainter painter(&zoomPx);
  painter.translate(wCenter);
  painter.scale(0.500001, 0.500001);
  painter.translate(-wCenter);
  painter.drawPixmap(0, 0, tmpImg);
  painter.end();

  // decrease zoom
  --currentZoom;
  emit currentZoomChanged();
  nbTiles = nbZoomTiles();
  wCenterM = geoToScreenPos(centerM);

  // show pixel zoomed image while map loads
  newOffscreenImage(true, true);
  emit refreshMap();
}

void GeoMapWidget::setZoom(int zoom) {
  if (zoom < minZoom || zoom > maxZoom)
    return;

  if (zoom < currentZoom)
    for (int i = currentZoom; i > zoom; --i)
      zoomOut();
  else
    for (int i = currentZoom; i < zoom; ++i)
      zoomIn();
}

void GeoMapWidget::forceRedraw() {
  newOffscreenImage(true, false);
}

void GeoMapWidget::translateView(const QPoint &point) {
  QPointF tmpCenter = screenToGeoPos(wCenterM + point);

  offscreenPxTranslation += point;
  zoomPxTranslation += point;
  wCenterM += point;

  centerM = tmpCenter;

  // check if a new offscreen image is needed
  QPoint upperLeft = wCenterM - wCenter;
  QPoint lowerRight = wCenterM + wCenter;
  QRect viewport = QRect(upperLeft, lowerRight);

  if (!offscreenViewport.contains(viewport))
    newOffscreenImage();
}

void GeoMapWidget::setMapCenter(const QPointF &coordinate) {
  wCenterM = geoToScreenPos(coordinate);
  centerM = coordinate;

  newOffscreenImage(true, false);
  emit refreshMap();
}

std::pair<double, double> GeoMapWidget::getMapSouthWest() {
  auto sw = getGeoViewport().topLeft();
  return std::make_pair<double, double>(sw.y(), sw.x());
}

std::pair<double, double> GeoMapWidget::getMapNorthEast() {
  auto ne = getGeoViewport().bottomRight();
  return std::make_pair<double, double>(ne.y(), ne.x());
}

void GeoMapWidget::setMapBounds(
    tlp::Graph *graph,
    const std::unordered_map<tlp::node, std::pair<double, double>> &nodesLatLngs) {

  if (!nodesLatLngs.empty()) {

    std::pair<double, double> minLatLng(90, 180);
    std::pair<double, double> maxLatLng(-90, -180);

    for (const auto &nll : nodesLatLngs) {
      if (graph->isElement(nll.first)) {
        minLatLng.first = std::min(minLatLng.first, nll.second.first);
        minLatLng.second = std::min(minLatLng.second, nll.second.second);
        maxLatLng.first = std::max(maxLatLng.first, nll.second.first);
        maxLatLng.second = std::max(maxLatLng.second, nll.second.second);
      }
    }

    zoomOnRectangle(minLatLng, maxLatLng);
  }
}

void GeoMapWidget::zoomOnRectangle(std::pair<double, double> &sw, std::pair<double, double> &ne) {
  // first center map
  QPointF swP(sw.second, sw.first);
  QPointF neP(ne.second, ne.first);
  QPoint swPos = geoToScreenPos(QPointF(sw.second, sw.first));
  QPoint nePos = geoToScreenPos(QPointF(ne.second, ne.first));
  setMapCenter(screenToGeoPos((swPos + nePos) / 2.0));

  auto containsRectangle = [&]() {
    QRectF bb = this->getGeoViewport();
    return bb.contains(swP) && bb.contains(neP);
  };

  while (!containsRectangle()) {
    zoomOut();
    // check min
    if (currentZoom == minZoom)
      break;
  }

  while (containsRectangle()) {
    zoomIn();
    // check max
    if (currentZoom == maxZoom)
      break;
  }

  if (!containsRectangle())
    zoomOut();
}

void GeoMapWidget::centerMap(const QList<QPointF> &coordinates) {
  QPoint c(0, 0);
  for (auto p : coordinates)
    c += geoToScreenPos(p);
  QPointF newCenter = screenToGeoPos(c / coordinates.size());
  setMapCenter(newCenter);
}

void GeoMapWidget::resizeEvent(QResizeEvent *ev) {
  size = ev->size();
  wCenter = QPoint(size.width() / 2, size.height() / 2);

  offscreenPx = QPixmap(size * 2);
  zoomPx = QPixmap(size);

  forceRedraw();

  emit refreshMap();
}

QRect GeoMapWidget::getScreenViewport() const {
  QPoint topLeft = QPoint(wCenterM.x() - wCenter.x(), wCenterM.y() - wCenter.y());

  QRect coordinateBB = QRect(topLeft, size);
  return coordinateBB;
}

QRectF GeoMapWidget::getGeoViewport() const {
  QPoint upperLeft = QPoint(wCenterM.x() - wCenter.x(), wCenterM.y() + wCenter.y());
  QPoint lowerRight = QPoint(wCenterM.x() + wCenter.x(), wCenterM.y() - wCenter.y());

  QPointF ulCoord = screenToGeoPos(upperLeft);
  QPointF lrCoord = screenToGeoPos(lowerRight);

  QRectF coordinateBB = QRectF(ulCoord, QSizeF((lrCoord - ulCoord).x(), (lrCoord - ulCoord).y()));
  return coordinateBB;
}

void GeoMapWidget::draw(QPainter *painter, const QPoint &wCenterM) {
  // position on center tile
  int cross_x = int(wCenterM.x()) % TILE_SIZE;
  int cross_y = int(wCenterM.y()) % TILE_SIZE;

  // computer the number tiles to drawn to fill the viewport
  int space = wCenter.x() - cross_x;
  int tilesOnLeft = space / TILE_SIZE;
  if (space > 0)
    ++tilesOnLeft;

  space = wCenter.y() - cross_y;
  int tilesAbove = space / TILE_SIZE;
  if (space > 0)
    ++tilesAbove;

  space = wCenter.x() - (TILE_SIZE - cross_x);
  int tilesOnRight = space / TILE_SIZE;
  if (space > 0)
    ++tilesOnRight;

  space = wCenter.y() - (TILE_SIZE - cross_y);
  int tilesOnBottom = space / TILE_SIZE;
  if (space > 0)
    ++tilesOnBottom;

  int wCenterM_tile_x = wCenterM.x() / TILE_SIZE;
  int wCenterM_tile_y = wCenterM.y() / TILE_SIZE;

  const QPoint from = QPoint((wCenterM_tile_x - tilesOnLeft) * TILE_SIZE,
                             (wCenterM_tile_y - tilesAbove) * TILE_SIZE);
  const QPoint to = QPoint((wCenterM_tile_x + tilesOnRight + 1) * TILE_SIZE,
                           (wCenterM_tile_y + tilesOnBottom + 1) * TILE_SIZE);

  offscreenViewport = QRect(from, to);

  // first ensure loading of the center tile
  if (isTileValid(wCenterM_tile_x, wCenterM_tile_y, currentZoom))
    painter->drawPixmap(size.width() - cross_x, size.height() - cross_y,
                        getTile(wCenterM_tile_x, wCenterM_tile_y, currentZoom));

  for (int i = wCenterM_tile_x - tilesOnLeft; i <= wCenterM_tile_x + tilesOnRight; ++i) {
    for (int j = wCenterM_tile_y - tilesAbove; j <= wCenterM_tile_y + tilesOnBottom; ++j) {
      // check if image is valid
      if (!(i == wCenterM_tile_x && j == wCenterM_tile_y)) {
        if (isTileValid(i, j, currentZoom)) {
          painter->drawPixmap(((i - wCenterM_tile_x) * TILE_SIZE) - cross_x + size.width(),
                              ((j - wCenterM_tile_y) * TILE_SIZE) - cross_y + size.height(),
                              getTile(i, j, currentZoom));
        }
      }
    }
  }
}

#define MAX_LAT 85.0511287798
#define EARTH_RADIUS 6378137
QPoint GeoMapWidget::geoToScreenPos(const QPointF &coordinate) const {
  // coord to pixel
  qreal x = (coordinate.x() + 180) * (nbTiles * TILE_SIZE) / 360.;
  qreal y = (1 - (log(tan(M_PI / 4 + DEG_TO_RAD(coordinate.y()) / 2)) / M_PI)) / 2 *
            (nbTiles * TILE_SIZE);

  return QPoint(int(x), int(y));
}

QPointF GeoMapWidget::screenToGeoPos(const QPoint &point) const {
  // pixel to coord
  qreal longitude = (point.x() * (360 / (nbTiles * TILE_SIZE))) - 180;
  qreal latitude = RAD_TO_DEG(atan(sinh((1 - point.y() * (2 / (nbTiles * TILE_SIZE))) * M_PI)));

  return QPointF(longitude, latitude);
}

bool GeoMapWidget::isTileValid(int x, int y, int z) const {
  return !((x < 0) || (x > (1 << z) - 1) || (y < 0) || (y > (1 << z) - 1));
}

double GeoMapWidget::nbZoomTiles() const {
  return pow(2.0, currentZoom);
}

QPixmap GeoMapWidget::getTile(int x, int y, int z) {
  // get tile url from the template one
  QString url(QString(tileTemplUrl)
                  .replace("{z}", QString::number(z))
                  .replace("{x}", QString::number(x))
                  .replace("{y}", QString::number(y)));
  {
    QMutexLocker lock(&mtx);
    if (loadingUrls.find(url) != loadingUrls.end())
      // tile is currently loading
      return loadingTile;
  }
  QPixmap pm;
  // is tile cached
  if (QPixmapCache::find(url, &pm) && !pm.isNull())
    // return cached tile
    return pm;
  else if (!failedFetches.contains(url) ||
           !(failedFetches[url].secsTo(QDateTime::currentDateTime()) < kDefaultTimeoutDelaySecs))
    // start tile loading
    loadTile(url);
  // add empty image while tile is loading
  return emptyTile;
}

void GeoMapWidget::loadTile(const QString &url) {
  QNetworkRequest request = QNetworkRequest(QUrl(url));

  request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows; U; MSIE "
                                     "6.0; Windows NT 5.1; SV1; .NET "
                                     "CLR 2.0.50727)");
  request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

  QMutexLocker lock(&mtx);
  replyList.append(http->get(request));
  loadingUrls.insert(url);
}

void GeoMapWidget::requestFinished(QNetworkReply *reply) {
  if (!reply)
    return;

  if (reply->error() == QNetworkReply::NoError) {
    QString url = reply->url().toString();
    // check if url is loading
    {
      QMutexLocker lock(&mtx);
      auto it = loadingUrls.find(url);
      if (it != loadingUrls.end())
        loadingUrls.erase(it);
      else
        url.clear();
    }

    if (!url.isEmpty() && reply->bytesAvailable() > 0) {
      QPixmap pm;

      if (pm.loadFromData(reply->readAll()) && pm.size().width() > 1 && pm.size().height() > 1) {
        // remove from failed list
        failedFetches.remove(url);
        QPixmapCache::insert(url, pm);

        forceRedraw();
      } else {
        // store current time for this failed image
        // to prevent loading it again until
        failedFetches.insert(url, QDateTime::currentDateTime());
      }
    }
  }

  if (loadingQueueEmpty()) {
    zoomPx.fill(Qt::white);
    forceRedraw();
    emit refreshMap();
  }

  QMutexLocker lock(&mtx);
  replyList.removeAll(reply);
  reply->deleteLater();
}

void GeoMapWidget::abortPendingLoads() {
  if (loadingQueueEmpty())
    return;

  QListIterator<QNetworkReply *> iter(replyList);
  for (auto reply : replyList) {
    if (reply->isRunning())
      reply->abort();
    reply->deleteLater();
  }
  QMutexLocker lock(&mtx);
  replyList.clear();
  loadingUrls.clear();
}

bool GeoMapWidget::loadingQueueEmpty() const {
  QMutexLocker lock(&mtx);
  return loadingUrls.empty();
}
