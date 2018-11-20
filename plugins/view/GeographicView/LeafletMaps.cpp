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

#include "LeafletMaps.h"

#include <QTimer>

#ifdef QT_HAS_WEBENGINE
#include <QWebChannel>
#endif

using namespace std;

namespace tlp {

const string htmlMap =
    ""
    "<html>"
    "<head>"
#ifdef QT_HAS_WEBENGINE
    "<script type=\"text/javascript\" src=\"qrc:///qtwebchannel/qwebchannel.js\"></script>"
#endif
    "<link rel=\"stylesheet\" href=\"https://unpkg.com/leaflet@1.3.4/dist/leaflet.css\" />"
    "<script src=\"https://unpkg.com/leaflet@1.3.4/dist/leaflet.js\"></script>"
    "<script type=\"text/javascript\">"
    "var map;"
    "var mapBounds;"
    "var osm;"
    "var esriSatellite;"
    "var esriTerrain;"
    "var esriGrayCanvas;"
    "var currentLayer;"
    "function init(lat, lng, zoom) { "
    " map = L.map('map_canvas', {"
    "   zoomControl: false"
    " });"
    " osm = L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {"
    "    attribution: '&copy; <a "
    "href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a> contributors'"
    " });"
    " osm.addTo(map);"
    " esriSatellite = "
    "L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/"
    "tile/{z}/{y}/{x}', {"
    "   attribution: 'Tiles &copy; Esri &mdash; Source: Esri, i-cubed, USDA, USGS, AEX, GeoEye, "
    "Getmapping, Aerogrid, IGN, IGP, UPR-EGP, and the GIS User Community'"
    " });"
    " esriTerrain = "
    "L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/"
    "tile/{z}/{y}/{x}', {"
    "   attribution: 'Tiles &copy; Esri &mdash; Esri, DeLorme, NAVTEQ, TomTom, Intermap, iPC, "
    "USGS, FAO, NPS, NRCAN, GeoBase, Kadaster NL, Ordnance Survey, Esri Japan, METI, Esri China "
    "(Hong Kong), and the GIS User Community'"
    " });"
    " esriGrayCanvas = "
    "L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/Canvas/"
    "World_Light_Gray_Base/MapServer/tile/{z}/{y}/{x}', {"
    "   attribution: 'Tiles &copy; Esri &mdash; Esri, DeLorme, NAVTEQ',"
    "   maxZoom: 16"
    " });"
    " currentLayer = osm;"
    " map.setView(L.latLng(lat, lng), zoom);"
#ifdef QT_HAS_WEBENGINE
    " map.on('zoomstart', function() {"
    "  leafletMapsQObject.refreshMap();"
    " });"
    " map.on('zoom', function() {"
    "  leafletMapsQObject.refreshMap();"
    " });"
    " map.on('movestart', function() {"
    "  leafletMapsQObject.refreshMap();"
    " });"
    " map.on('move', function() {"
    "  leafletMapsQObject.refreshMap();"
    " });"
#endif
    " map.on('moveend', function() {"
    "  leafletMapsQObject.refreshMap();"
    " });"
    " map.on('zoomend', function() {"
    "  leafletMapsQObject.refreshMap();"
    " });"
    "}"
    "function setMapBounds(latLngArray) {"
    "  var latLngBounds = L.latLngBounds();"
    "  for (var i = 0 ; i < latLngArray.length ; ++i) {"
    "    latLngBounds.extend(latLngArray[i]);"
    "  }"
    "  map.flyToBounds(latLngBounds);"
    "}"
    "function getWorldWidth() {"
    "  var bounds = map.getPixelWorldBounds();"
    "  return bounds.getTopRight().x - bounds.getTopLeft().x;"
    "}"
    "function switchToOpenStreetMap() {"
    "  map.removeLayer(currentLayer);"
    "  map.addLayer(osm);"
    "  currentLayer = osm;"
    "  leafletMapsQObject.refreshMap();"
    "}"
    "function switchToEsriSatellite() {"
    "  map.removeLayer(currentLayer);"
    "  map.addLayer(esriSatellite);"
    "  currentLayer = esriSatellite;"
    "  leafletMapsQObject.refreshMap();"
    "}"
    "function switchToEsriTerrain() {"
    "  map.removeLayer(currentLayer);"
    "  map.addLayer(esriTerrain);"
    "  currentLayer = esriTerrain;"
    "  leafletMapsQObject.refreshMap();"
    "}"
    "function switchToEsriGrayCanvas() {"
    "  map.removeLayer(currentLayer);"
    "  map.addLayer(esriGrayCanvas);"
    "  currentLayer = esriGrayCanvas;"
    "  leafletMapsQObject.refreshMap();"
    "}"
#ifdef QT_HAS_WEBENGINE
    "document.addEventListener(\"DOMContentLoaded\", function () {"
    "  new QWebChannel(qt.webChannelTransport, function (channel) {"
    "    leafletMapsQObject = channel.objects.leafletMapsQObject;"
    "    leafletMapsQObject.refreshMap();"
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

JsCallback *JsCallback::_lastCreatedInstance = nullptr;

#endif

#ifdef QT_HAS_WEBKIT
LeafletMaps::LeafletMaps(QWidget *parent) : QWebView(parent), init(false) {
#else
LeafletMaps::LeafletMaps(QWidget *parent) : QWebEngineView(parent), init(false) {
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
  channel->registerObject(QStringLiteral("leafletMapsQObject"), mapRefresher);
#endif
  frame->setHtml(content);
  QTimer::singleShot(1500, this, SLOT(triggerLoading()));
}

QVariant LeafletMaps::executeJavascript(const QString &jsCode) {
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

void LeafletMaps::triggerLoading() {
#ifdef QT_HAS_WEBKIT
  frame->addToJavaScriptWindowObject("leafletMapsQObject", this);
#endif
  // map is first centered in the gulf of gascogne
  // at the latitude of Bordeaux
  // zoom level is pretty low to minimize the map display time
  QString code = "init(44.8084000, -2.0, 2)";
  executeJavascript(code);
  init = true;
}

void LeafletMaps::switchToOpenStreetMap() {
  QString code = "switchToOpenStreetMap()";
  executeJavascript(code);
}

void LeafletMaps::switchToEsriSatellite() {
  QString code = "switchToEsriSatellite()";
  executeJavascript(code);
}

void LeafletMaps::switchToEsriTerrain() {
  QString code = "switchToEsriTerrain()";
  executeJavascript(code);
}

void LeafletMaps::switchToEsriGrayCanvas() {
  QString code = "switchToEsriGrayCanvas()";
  executeJavascript(code);
}

void LeafletMaps::setMapCenter(double latitude, double longitude) {
  QString code = "map.setView(L.latLng(%1, %2), map.getZoom());";
  executeJavascript(code.arg(latitude).arg(longitude));
}

Coord LeafletMaps::getPixelPosOnScreenForLatLng(double lat, double lng) {
  QString code = "map.latLngToContainerPoint(L.latLng(%1, %2)).toString();";
  QVariant ret = executeJavascript(code.arg(lat).arg(lng));

  QString pointStr = ret.toString();
  int pos = pointStr.indexOf('(') + 1;
  QString xStr = pointStr.mid(pos, pointStr.lastIndexOf(',') - pos);
  pos = pointStr.lastIndexOf(',') + 1;
  QString yStr = pointStr.mid(pos, pointStr.lastIndexOf(')') - pos);

  bool ok;
  return Coord(xStr.toDouble(&ok), yStr.toDouble(&ok), 0);
}

Coord LeafletMaps::mercatorProjection(const Coord &swPixel, const Coord &nePixel,
                                      const double latitude, const double longitude) {
  double dLng = longitude + 180.0;
  double latRadians = M_PI * latitude / 180.0;
  double worldHeight = nePixel[1] - swPixel[1];
  double worldWidth = nePixel[0] - swPixel[0];
  double y = worldHeight / 2.0 + log(tan(M_PI / 4.0 + latRadians / 2.0)) * worldWidth / (2 * M_PI);
  return Coord(swPixel.getX() + (dLng / 360.0) * worldWidth, swPixel.getY() + y);
}

std::pair<double, double> LeafletMaps::getLatLngForPixelPosOnScreen(int x, int y) {
  QString code = "map.containerPointToLatLng(L.point(%1, %2)).toString();";
  QVariant ret = executeJavascript(code.arg(x).arg(y));

  QString latLngStr = ret.toString();
  int pos = latLngStr.indexOf('(') + 1;
  QString latStr = latLngStr.mid(pos, latLngStr.lastIndexOf(',') - pos);
  pos = latLngStr.lastIndexOf(',') + 1;
  QString lngStr = latLngStr.mid(pos, latLngStr.lastIndexOf(')') - pos);
  return make_pair(latStr.toDouble(), lngStr.toDouble());
}

int LeafletMaps::getWorldWidth() {
  QString code = "getWorldWidth();";
  QVariant ret = executeJavascript(code);
  QString retStr = ret.toString();
  return int(retStr.toDouble() + 1);
}

int LeafletMaps::getCurrentMapZoom() {
  QString code = "map.getZoom();";
  QVariant ret = executeJavascript(code);
  return ret.toInt();
}

static int clamp(int i, int minVal, int maxVal) {
  return min(max(i, minVal), maxVal);
}

void LeafletMaps::setCurrentZoom(int zoom) {
  QString code = "map.setZoom(%1);";
  executeJavascript(code.arg(clamp(zoom, 0, 20)));
  emit currentZoomChanged();
}

pair<double, double> LeafletMaps::getCurrentMapCenter() {
  QString code = "map.getCenter().toString();";
  QVariant ret = executeJavascript(code);

  pair<double, double> latLng;

  if (!ret.isNull()) {
    QString pointStr = ret.toString();
    int pos = pointStr.indexOf('(') + 1;
    QString xStr = pointStr.mid(pos, pointStr.lastIndexOf(',') - pos);
    pos = pointStr.lastIndexOf(',') + 1;
    QString yStr = pointStr.mid(pos, pointStr.lastIndexOf(')') - pos);
    latLng = make_pair(xStr.toDouble(), yStr.toDouble());
  }

  return latLng;
}

void LeafletMaps::setMapBounds(Graph *graph, const map<node, pair<double, double>> &nodesLatLngs) {

  if (!nodesLatLngs.empty()) {

    pair<double, double> minLatLng = make_pair(90, 180);
    pair<double, double> maxLatLng = make_pair(-90, -180);

    map<node, pair<double, double>>::const_iterator it;

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
    code = "mapBounds.push(L.latLng(%1, %2));";
    executeJavascript(code.arg(minLatLng.first).arg(minLatLng.second));
    executeJavascript(code.arg(maxLatLng.first).arg(maxLatLng.second));
    code = "setMapBounds(mapBounds);";
    executeJavascript(code);
  }
}

void LeafletMaps::setMapBounds(Coord nw, Coord se) {
  QString code = "mapBounds = [];";
  executeJavascript(code);

  code = "mapBounds.push(L.latLng(%1, %2));";
  executeJavascript(code.arg(nw[1]).arg(nw[0]));

  code = "mapBounds.push(L.latLng(%1, %2));";
  executeJavascript(code.arg(se[1]).arg(se[0]));

  code = "setMapBounds(mapBounds);";
  executeJavascript(code);
}

void LeafletMaps::panMap(int dx, int dy) {
  QString code = "map.panBy(L.point(%1, %2));";
  executeJavascript(code.arg(dx).arg(dy));
}

pair<double, double> LeafletMaps::getMapCurrentSouthWestLatLng() {
  QString code = "map.getBounds().getSouthWest().toString();";
  QVariant ret = executeJavascript(code);

  pair<double, double> latLng;

  if (!ret.isNull()) {
    QString pointStr = ret.toString();
    int pos = pointStr.indexOf('(') + 1;
    QString xStr = pointStr.mid(pos, pointStr.lastIndexOf(',') - pos);
    pos = pointStr.lastIndexOf(',') + 1;
    QString yStr = pointStr.mid(pos, pointStr.lastIndexOf(')') - pos);
    latLng = make_pair(xStr.toDouble(), yStr.toDouble());
  }

  return latLng;
}

pair<double, double> LeafletMaps::getMapCurrentNorthEastLatLng() {
  QString code = "map.getBounds().getNorthEast().toString();";
  QVariant ret = executeJavascript(code);

  pair<double, double> latLng;

  if (!ret.isNull()) {
    QString pointStr = ret.toString();
    int pos = pointStr.indexOf('(') + 1;
    QString xStr = pointStr.mid(pos, pointStr.lastIndexOf(',') - pos);
    pos = pointStr.lastIndexOf(',') + 1;
    QString yStr = pointStr.mid(pos, pointStr.lastIndexOf(')') - pos);
    latLng = make_pair(xStr.toDouble(), yStr.toDouble());
  }

  return latLng;
}

} // namespace tlp
