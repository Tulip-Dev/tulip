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

#include "GoogleMaps.h"

#include <QTimer>
#include <QApplication>

#ifdef QT_HAS_WEBENGINE
#include <QWebChannel>
#endif

#include <tulip/TlpQtTools.h>

using namespace std;

namespace tlp {

const string htmlMap =
    ""
    "<html>"
    "<head>"
#ifdef QT_HAS_WEBENGINE
    "<script type=\"text/javascript\" src=\"qrc:///qtwebchannel/qwebchannel.js\"></script>"
#endif
    "<script type=\"text/javascript\" "
    "src=\"http://maps.google.com/maps/api/js?sensor=false\"></script>"
    "<script type=\"text/javascript\">"
    "var map;"
    "var mapProjectionAccessor;"
    "var geocoder;"
    "var mapBounds;"
    "function MapProjectionAccessor(map) {"
    "  this.setMap(map);"
    "}"
    "MapProjectionAccessor.prototype = new google.maps.OverlayView();"
    "MapProjectionAccessor.prototype.onAdd = function() {};"
    "MapProjectionAccessor.prototype.onRemove = function() {};"
    "MapProjectionAccessor.prototype.draw = function() {};"
    "MapProjectionAccessor.prototype.getPixelPositionOnScreenForLatLng = function(lat, lng) {"
    "  var screenPos = this.getProjection().fromLatLngToContainerPixel(new google.maps.LatLng(lat, "
    "lng));"
    "  return screenPos.toString();"
    "};"
    "MapProjectionAccessor.prototype.getLatLngForPixelPosition = function(x, y) {"
    "  var latLng = this.getProjection().fromContainerPixelToLatLng(new google.maps.Point(x, y));"
    "  return latLng.toString();"
    "};"
    "function init(lat, lng) { "
    "  map = new google.maps.Map(document.getElementById(\"map_canvas\"), { zoom: 5, center: new "
    "google.maps.LatLng(lat, lng), mapTypeControl: false,"
    "  mapTypeControlOptions: {style: google.maps.MapTypeControlStyle.DROPDOWN_MENU},"
    "  disableDefaultUI: true,"
    "  mapTypeId: google.maps.MapTypeId.ROADMAP  });"
    "  google.maps.event.addListener(map, 'idle', function(evt) {"
    "    googleMapsQObject.refreshMap();"
    "  });"
    "  google.maps.event.addListener(map, 'tilesloaded', function(evt) {"
    "    googleMapsQObject.refreshMap();"
    "  });"
    "  mapProjectionAccessor = new MapProjectionAccessor(map);"
    "  geocoder = new google.maps.Geocoder();"
    "  geocoder.results = null;"
    "  geocoder.done = false;"
    "  geocoder.requestStatus = null;"
    "}"
    "function setMapBounds(latLngArray) {"
    "  var latLngBounds = new google.maps.LatLngBounds();"
    "  for (var i = 0 ; i < latLngArray.length ; ++i) {"
    "    latLngBounds.extend(latLngArray[i]);"
    "  }"
    "  map.setCenter(latLngBounds.getCenter());"
    "  map.fitBounds(latLngBounds);"
    "}"
    "function geocodeResult(results, status) {"
    "  if (status == google.maps.GeocoderStatus.OK) {"
    "    geocoder.results = results;"
    "  } "
    "  geocoder.done = true;"
    "  geocoder.requestStatus = status;"
    "}"
    "function codeAddress(address) {"
    "  geocoder.done = false;"
    "  geocoder.results = null;"
    "  geocoder.geocode({'address' : address}, geocodeResult);"
    "}"
    "function geocodingDone() {"
    "  return geocoder.done;"
    "}"
    "function getGeocodingStatus() {"
    "  if (geocoder.requestStatus == google.maps.GeocoderStatus.OK) {"
    "    return \"OK\";"
    "  } else if (geocoder.requestStatus == google.maps.GeocoderStatus.UNKNOWN_ERROR) {"
    "    return \"UNKNOWN_ERROR\";"
    "  } else if (geocoder.requestStatus == google.maps.GeocoderStatus.OVER_QUERY_LIMIT) {"
    "    return \"OVER_QUERY_LIMIT\";"
    "  } else if (geocoder.requestStatus == google.maps.GeocoderStatus.REQUEST_DENIED) {"
    "    return \"REQUEST_DENIED\";"
    "  } else if (geocoder.requestStatus == google.maps.GeocoderStatus.INVALID_REQUEST) {"
    "    return \"INVALID_REQUEST\";"
    "  } else if (geocoder.requestStatus == google.maps.GeocoderStatus.ZERO_RESULTS) {"
    "    return \"ZERO_RESULTS\";"
    "  } else if (geocoder.requestStatus == google.maps.GeocoderStatus.ERROR) {"
    "    return \"ERROR\";"
    "  }"
    "}"
    "function getGeocodingNumberOfResults() {"
    "  if (geocoder.results) {"
    "    return geocoder.results.length;"
    "  } else {"
    "    return 0;"
    "  }"
    "}"
    "function getGeocodingResultAddress(idx) {"
    "  if (geocoder.results) {"
    "    return geocoder.results[idx].formatted_address.toString();"
    "  } else {"
    "    return null;"
    "  }"
    "}"
    "function getGeocodingResultLatLng(idx) {"
    "  if (geocoder.results) {"
    "    return geocoder.results[idx].geometry.location.toString();"
    "  } else {"
    "    return null;"
    "  }"
    "}"
    "function switchToSatelliteView() {"
    "  map.setMapTypeId(google.maps.MapTypeId.SATELLITE);"
    "  googleMapsQObject.refreshMap();"
    "}"
    "function switchToRoadMapView() {"
    "  map.setMapTypeId(google.maps.MapTypeId.ROADMAP);"
    "  googleMapsQObject.refreshMap();"
    "}"
    "function switchToHybridView() {"
    "  map.setMapTypeId(google.maps.MapTypeId.HYBRID);"
    "  googleMapsQObject.refreshMap();"
    "}"
    "function switchToTerrainView() {"
    "  map.setMapTypeId(google.maps.MapTypeId.TERRAIN);"
    "  googleMapsQObject.refreshMap();"
    "}"
#ifdef QT_HAS_WEBENGINE
    "document.addEventListener(\"DOMContentLoaded\", function () {"
    "  new QWebChannel(qt.webChannelTransport, function (channel) {"
    "    googleMapsQObject = channel.objects.googleMapsQObject;"
    "    googleMapsQObject.refreshMap();"
    "  });"
    "});"
#endif
    "</script>"
    "</head>"
    "<body style=\"margin:0px; padding:0px;\" >"
    "<div id=\"map_canvas\" style=\"width:100%; height:100%\"></div>"
    "</body>"
    "</html>";

#ifdef QT_HAS_WEBENGINE

void MapRefresher::refreshMap() {
  emit refreshMapSignal();
}

JsCallback *JsCallback::_lastCreatedInstance = NULL;

#endif

#ifdef QT_HAS_WEBKIT
GoogleMaps::GoogleMaps(QWidget *parent) : QWebView(parent), init(false) {
#else
GoogleMaps::GoogleMaps(QWidget *parent) : QWebEngineView(parent), init(false) {
#endif
  QString content(htmlMap.c_str());
#ifdef QT_HAS_WEBKIT
  frame = page()->mainFrame();
  frame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  frame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
#else
  frame = page();
  mapRefresher = new MapRefresher;
  connect(mapRefresher, SIGNAL(refreshMapSignal()), this, SIGNAL(refreshMap()));
  QWebChannel *channel = new QWebChannel(frame);
  frame->setWebChannel(channel);
  channel->registerObject(QStringLiteral("googleMapsQObject"), mapRefresher);
#endif
  frame->setHtml(content);
  QTimer::singleShot(1500, this, SLOT(triggerLoading()));
}

QVariant GoogleMaps::executeJavascript(const QString &jsCode) {
#ifdef QT_HAS_WEBKIT
  return frame->evaluateJavaScript(jsCode);
#else
  QVariant ret;
  JsCallback jscb(&ret);
  frame->runJavaScript(jsCode, jscb);
  JsCallback::waitForCallback();
  return ret;
#endif
}

void GoogleMaps::triggerLoading() {
#ifdef QT_HAS_WEBKIT
  frame->addToJavaScriptWindowObject("googleMapsQObject", this);
#endif
  QString code = "init(44.8084000, -0.5968050)";
  executeJavascript(code);
  init = true;
}

void GoogleMaps::switchToSatelliteView() {
  QString code = "switchToSatelliteView()";
  executeJavascript(code);
}

void GoogleMaps::switchToRoadMapView() {
  QString code = "switchToRoadMapView()";
  executeJavascript(code);
}

void GoogleMaps::switchToTerrainView() {
  QString code = "switchToTerrainView()";
  executeJavascript(code);
}

void GoogleMaps::switchToHybridView() {
  QString code = "switchToHybridView()";
  executeJavascript(code);
}

void GoogleMaps::setMapCenter(double latitude, double longitude) {
  QString code = "map.setCenter(new google.maps.LatLng(%1, %2));";
  executeJavascript(code.arg(latitude).arg(longitude));
  executeJavascript(code.arg(latitude).arg(longitude));
}

Coord GoogleMaps::getPixelPosOnScreenForLatLng(double lat, double lng) {
  QString code = "mapProjectionAccessor.getPixelPositionOnScreenForLatLng(%1, %2);";
  QVariant ret = executeJavascript(code.arg(lat).arg(lng));

  QString pointStr = ret.toString();
  QString xStr = pointStr.mid(1, pointStr.lastIndexOf(',') - 1);
  QString yStr = pointStr.mid(pointStr.lastIndexOf(',') + 2,
                              pointStr.lastIndexOf(')') - pointStr.lastIndexOf(',') - 2);

  bool ok;
  return Coord(xStr.toDouble(&ok), yStr.toDouble(&ok), 0);
}

Coord GoogleMaps::mercatorProjection(const Coord &swPixel, const Coord &nePixel,
                                     const double latitude, const double longitude) {
  double dLng = longitude + 180.0;
  double latRadians = M_PI * latitude / 180.0;
  double worldHeight = nePixel[1] - swPixel[1];
  double worldWidth = nePixel[0] - swPixel[0];
  double y = worldHeight / 2.0 + log(tan(M_PI / 4.0 + latRadians / 2.0)) * worldWidth / (2 * M_PI);
  return Coord(swPixel.getX() + (dLng / 360.0) * worldWidth, swPixel.getY() + y);
}

std::pair<double, double> GoogleMaps::getLatLngForPixelPosOnScreen(int x, int y) {
  QString code = "mapProjectionAccessor.getLatLngForPixelPosition(%1, %2);";
  QVariant ret = executeJavascript(code.arg(x).arg(y));

  QString latLngStr = ret.toString();
  QString latStr = latLngStr.mid(1, latLngStr.lastIndexOf(',') - 1);
  QString lngStr = latLngStr.mid(latLngStr.lastIndexOf(',') + 1,
                                 latLngStr.lastIndexOf(')') - latLngStr.lastIndexOf(',') - 1);
  return make_pair(latStr.toDouble(), lngStr.toDouble());
}

int GoogleMaps::getWorldWidth() {
  QString code = "mapProjectionAccessor.getProjection().getWorldWidth();";
  QVariant ret = executeJavascript(code);

  QString retStr = ret.toString();
  return int(retStr.toDouble() + 1);
}

string GoogleMaps::getLatLngForAddress(const QString &address, pair<double, double> &latLng,
                                       bool skipMultipleResults) {
  QString code = "codeAddress(\"%1\")";
  executeJavascript(code.arg(address));
  code = "geocodingDone()";
  QVariant ret = executeJavascript(code);

  // disable user input
  // before allowing some display feedback
  tlp::disableQtUserInput();

  while (!ret.toBool()) {
    QApplication::processEvents();
    ret = executeJavascript(code);
  }

  // reenable user input
  tlp::enableQtUserInput();

  code = "getGeocodingNumberOfResults()";
  ret = executeJavascript(code);

  int nbResults = ret.toUInt();
  int idx = 0;

  if (nbResults > 1 && !skipMultipleResults) {
    bool showProgressWidget = false;

    if (progressWidget->isVisible()) {
      progressWidget->hide();
      showProgressWidget = true;
    }

    addressSelectionDialog->clearList();
    addressSelectionDialog->setBaseAddress(address);

    for (int i = 0; i < nbResults; ++i) {
      code = "getGeocodingResultAddress(%1)";

      ret = executeJavascript(code.arg(i));

      addressSelectionDialog->addResultToList(ret.toString());
    }

    addresseSelectionProxy->setPos(
        width() / 2 - addresseSelectionProxy->sceneBoundingRect().width() / 2,
        height() / 2 - addresseSelectionProxy->sceneBoundingRect().height() / 2);
    addresseSelectionProxy->show();

    if (addressSelectionDialog->exec() == QDialog::Accepted) {
      idx = addressSelectionDialog->getPickedResultIdx();
    }

    addressSelectionDialog->hide();

    if (showProgressWidget) {
      progressWidget->show();
    }
  } else if (nbResults > 1) {
    return "MULTIPLE_RESULTS";
  }

  code = "getGeocodingResultLatLng(%1)";

  ret = executeJavascript(code.arg(idx));

  if (!ret.isNull()) {
    QString pointStr = ret.toString();
    QString xStr = pointStr.mid(1, pointStr.lastIndexOf(',') - 1);
    QString yStr = pointStr.mid(pointStr.lastIndexOf(',') + 1,
                                pointStr.lastIndexOf(')') - pointStr.lastIndexOf(',') - 1);
    latLng = make_pair(xStr.toDouble(), yStr.toDouble());
  }

  code = "getGeocodingStatus()";

  ret = executeJavascript(code);

  return QStringToTlpString(ret.toString());
}

int GoogleMaps::getCurrentMapZoom() {
  QString code = "map.getZoom();";
  QVariant ret = executeJavascript(code);
  return ret.toInt();
}

static int clamp(int i, int minVal, int maxVal) {
  return min(max(i, minVal), maxVal);
}

void GoogleMaps::setCurrentZoom(int zoom) {
  QString code = "map.setZoom(%1);";
  executeJavascript(code.arg(clamp(zoom, 0, 20)));
  emit currentZoomChanged();
}

pair<double, double> GoogleMaps::getCurrentMapCenter() {
  QString code = "map.getCenter().toString();";
  QVariant ret = executeJavascript(code);

  pair<double, double> latLng;

  if (!ret.isNull()) {
    QString pointStr = ret.toString();
    QString xStr = pointStr.mid(1, pointStr.lastIndexOf(',') - 1);
    QString yStr = pointStr.mid(pointStr.lastIndexOf(',') + 1,
                                pointStr.lastIndexOf(')') - pointStr.lastIndexOf(',') - 1);
    latLng = make_pair(xStr.toDouble(), yStr.toDouble());
  }

  return latLng;
}

void GoogleMaps::setMapBounds(Graph *graph, const map<node, pair<double, double> > &nodesLatLngs) {

  if (!nodesLatLngs.empty()) {

    pair<double, double> minLatLng = make_pair(90, 180);
    pair<double, double> maxLatLng = make_pair(-90, -180);

    map<node, pair<double, double> >::const_iterator it;

    for (it = nodesLatLngs.begin(); it != nodesLatLngs.end(); ++it) {
      if (graph->isElement(it->first)) {
        minLatLng.first = std::min(minLatLng.first, (it->second).first);
        minLatLng.second = std::min(minLatLng.second, (it->second).second);
        maxLatLng.first = std::max(maxLatLng.first, (it->second).first);
        maxLatLng.second = std::max(maxLatLng.second, (it->second).second);
      }
    }

    QString code = "mapBounds = [];";
    executeJavascript(code);
    code = "mapBounds.push(new google.maps.LatLng(%1, %2));";
    executeJavascript(code.arg(minLatLng.first).arg(minLatLng.second));
    executeJavascript(code.arg(maxLatLng.first).arg(maxLatLng.second));
    code = "setMapBounds(mapBounds);";
    executeJavascript(code);
  }
}

void GoogleMaps::setMapBounds(Coord nw, Coord se) {
  QString code = "mapBounds = [];";
  executeJavascript(code);

  code = "mapBounds.push(new google.maps.LatLng(%1, %2));";
  executeJavascript(code.arg(nw[1]).arg(nw[0]));

  code = "mapBounds.push(new google.maps.LatLng(%1, %2));";
  executeJavascript(code.arg(se[1]).arg(se[0]));

  code = "setMapBounds(mapBounds);";
  executeJavascript(code);
}

void GoogleMaps::wheelEvent(QWheelEvent *ev) {
  setCurrentZoom(getCurrentMapZoom() + ev->delta() / 120);
}

void GoogleMaps::mouseMoveEvent(QMouseEvent *ev) {
  panMap(x - ev->x(), y - ev->y());
  x = ev->x();
  y = ev->y();
}

void GoogleMaps::mousePressEvent(QMouseEvent *ev) {
  x = ev->x();
  y = ev->y();
#ifdef QT_HAS_WEBKIT
  QWebView::mousePressEvent(ev);
#else
  QWebEngineView::mousePressEvent(ev);
#endif
}

void GoogleMaps::panMap(int dx, int dy) {
  QString code = "map.panBy(%1, %2);";
  executeJavascript(code.arg(dx).arg(dy));
}

pair<double, double> GoogleMaps::getMapCurrentSouthWestLatLng() {
  QString code = "map.getBounds().getSouthWest().toString();";
  QVariant ret = executeJavascript(code);

  pair<double, double> latLng;

  if (!ret.isNull()) {
    QString pointStr = ret.toString();
    QString xStr = pointStr.mid(1, pointStr.lastIndexOf(',') - 1);
    QString yStr = pointStr.mid(pointStr.lastIndexOf(',') + 1,
                                pointStr.lastIndexOf(')') - pointStr.lastIndexOf(',') - 1);
    latLng = make_pair(xStr.toDouble(), yStr.toDouble());
  }

  return latLng;
}

pair<double, double> GoogleMaps::getMapCurrentNorthEastLatLng() {
  QString code = "map.getBounds().getNorthEast().toString();";
  QVariant ret = executeJavascript(code);

  pair<double, double> latLng;

  if (!ret.isNull()) {
    QString pointStr = ret.toString();
    QString xStr = pointStr.mid(1, pointStr.lastIndexOf(',') - 1);
    QString yStr = pointStr.mid(pointStr.lastIndexOf(',') + 1,
                                pointStr.lastIndexOf(')') - pointStr.lastIndexOf(',') - 1);
    latLng = make_pair(xStr.toDouble(), yStr.toDouble());
  }

  return latLng;
}
}
