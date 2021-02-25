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

void MapRefresher::refreshMap() {
  emit refreshMapSignal();
}

JsCallback *JsCallback::_lastCreatedInstance = nullptr;
#endif

using namespace std;

namespace tlp {

const char *htmlMap =
    R"(
<html>
<head>
)"
#ifdef QT_HAS_WEBENGINE
    R"(
<script type="text/javascript" src="qrc:///qtwebchannel/qwebchannel.js"></script>
)"
#endif
    R"(
<style>
/* Tulip may hangs after a click on a html link (in attribution)
   so disable all of them */
a {
 pointer-events: none;
}
</style>
<link rel="stylesheet" href="qrc:///tulip/view/geographic/leaflet/leaflet.css"/>
<script src="qrc:///tulip/view/geographic/leaflet/leaflet.js"></script>
<script type="text/javascript">
var map;
var mapBounds;
var osm;
var esriStreetMap;
var esriTopoMap;
var esriNatGeoMap;
var esriSatellite;
var esriLightGrayCanvas;
var esriDarkGrayCanvas;
var currentLayer;
function refreshMap() {
  leafletMapsQObject.refreshMap();
}
function refreshMapWithDelay() {
  setTimeout(function() {
    leafletMapsQObject.refreshMap();
  }, 500);
}
function addEventHandlersToLayer(layer) {
  layer.on('tileload', refreshMapWithDelay);
  layer.on('load', refreshMapWithDelay);
}
function init(lat, lng, zoom) {
  map = L.map('map_canvas', {
    zoomControl: false
  });
  osm = L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
     attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
  });
  addEventHandlersToLayer(osm);
  osm.addTo(map);
  esriStreetMap =
    L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer/tile/{z}/{y}/{x}', {
      attribution: 'Tiles &copy; Esri &mdash; Source: Esri, i-cubed, USDA, USGS, AEX, GeoEye, Getmapping, Aerogrid, IGN, IGP, UPR-EGP, and the GIS User Community'
  });
  addEventHandlersToLayer(esriStreetMap);
  esriTopoMap =
    L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/{z}/{y}/{x}', {
      attribution: 'Tiles &copy; Esri &mdash; Source: Esri, i-cubed, USDA, USGS, AEX, GeoEye, Getmapping, Aerogrid, IGN, IGP, UPR-EGP, and the GIS User Community'
  });
  addEventHandlersToLayer(esriTopoMap);
  esriNatGeoMap =
    L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/NatGeo_World_Map/MapServer/tile/{z}/{y}/{x}', {
      attribution: 'Tiles &copy; Esri &mdash; Source: Esri, i-cubed, USDA, USGS, AEX, GeoEye, Getmapping, Aerogrid, IGN, IGP, UPR-EGP, and the GIS User Community'
  });
  addEventHandlersToLayer(esriNatGeoMap);
  esriSatellite =
    L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', {
      attribution: 'Tiles &copy; Esri &mdash; Source: Esri, i-cubed, USDA, USGS, AEX, GeoEye, Getmapping, Aerogrid, IGN, IGP, UPR-EGP, and the GIS User Community'
  });
  addEventHandlersToLayer(esriSatellite);
  esriLightGrayCanvas =
    L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/Canvas/World_Light_Gray_Base/MapServer/tile/{z}/{y}/{x}', {
      attribution: 'Tiles &copy; Esri &mdash; Esri, DeLorme, NAVTEQ',
      maxZoom: 16
  });
  addEventHandlersToLayer(esriLightGrayCanvas);
  esriDarkGrayCanvas =
    L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/Canvas/World_Dark_Gray_Base/MapServer/tile/{z}/{y}/{x}', {
      attribution: 'Tiles &copy; Esri &mdash; Esri, DeLorme, NAVTEQ',
      maxZoom: 16
  });
  addEventHandlersToLayer(esriDarkGrayCanvas);
  currentLayer = osm;
  map.setView(L.latLng(lat, lng), zoom);
  map.on('zoomstart', refreshMap);
  map.on('zoom', refreshMap);
  map.on('zoomend', refreshMap);
  map.on('movestart', refreshMap);
  map.on('move', refreshMap);
  map.on('moveend', refreshMap);
}
function setMapBounds(latLngArray) {
  var latLngBounds = L.latLngBounds();
  for (var i = 0 ; i < latLngArray.length ; ++i) {
    latLngBounds.extend(latLngArray[i]);
  }
  map.flyToBounds(latLngBounds);
}
 function toOpenStreetMap() {
  map.removeLayer(currentLayer);
  map.addLayer(osm);
  currentLayer = osm;
  refreshMap();
 }
function toEsriStreetMap() {
  map.removeLayer(currentLayer);
  map.addLayer(esriStreetMap);
  currentLayer = esriStreetMap;
  refreshMap();
}
function toEsriTopoMap() {
  map.removeLayer(currentLayer);
  map.addLayer(esriTopoMap);
  currentLayer = esriTopoMap;
  refreshMap();
}
function toEsriNatGeoMap() {
  map.removeLayer(currentLayer);
  map.addLayer(esriNatGeoMap);
  currentLayer = esriNatGeoMap;
  refreshMap();
}
function toEsriSatellite() {
  map.removeLayer(currentLayer);
  map.addLayer(esriSatellite);
  currentLayer = esriSatellite;
  refreshMap();
}
function toEsriLightGrayCanvas() {
   map.removeLayer(currentLayer);
   map.addLayer(esriLightGrayCanvas);
  currentLayer = esriLightGrayCanvas;
  refreshMap();
 }
function toEsriDarkGrayCanvas() {
   map.removeLayer(currentLayer);
   map.addLayer(esriDarkGrayCanvas);
  currentLayer = esriDarkGrayCanvas;
  refreshMap();
 }
function switchToCustomTileLayer(customTileLayerUrl) {
  map.removeLayer(currentLayer);
  var customTileLayer = L.tileLayer(customTileLayerUrl, {
      attribution: customTileLayerUrl,
      errorTileUrl: 'qrc:///tulip/view/geographic/leaflet/no-tile.png'
  });
  addEventHandlersToLayer(customTileLayer);
  map.addLayer(customTileLayer);
  currentLayer = customTileLayer;
  refreshMap();
}
)"
#ifdef QT_HAS_WEBENGINE
    R"(
document.addEventListener("DOMContentLoaded", function () {
  new QWebChannel(qt.webChannelTransport, function (channel) {
    leafletMapsQObject = channel.objects.leafletMapsQObject;
    refreshMap();
  });
});
)"
#endif
    R"(
</script>
</head>
<body style="margin:0px; padding:0px;" >
<div id="map_canvas" style="width:100%; height:100%"></div>
</body>
</html>
)";

LeafletMaps::LeafletMaps(QWidget *parent) : QWEBVIEW(parent), init(false) {
#ifdef QT_HAS_WEBKIT
  // disable output of "libpng warning: iCCP: known incorrect sRGB profile"
  // due to QtWebKit ill-formed png files
  enableWarningOutput(false);
  frame = page()->mainFrame();
  enableWarningOutput(true);
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
  frame->setHtml(htmlMap);
  QTimer::singleShot(500, this, SLOT(triggerLoading()));
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

bool LeafletMaps::pageLoaded() {
  return executeJavascript(R"(typeof init !== "undefined")").toBool();
}

bool LeafletMaps::mapLoaded() {
  return executeJavascript(R"(typeof map !== "undefined")").toBool();
}

void LeafletMaps::triggerLoading() {
  if (!pageLoaded()) {
    QTimer::singleShot(500, this, SLOT(triggerLoading()));
    return;
  }
#ifdef QT_HAS_WEBKIT
  frame->addToJavaScriptWindowObject("leafletMapsQObject", this);
#endif
  // map is first centered in the Atlantic Ocean
  // in order to emphasize the need to configure geolocation
  executeJavascript("init(44.8084, -40, 3)");
  init = true;
}

void LeafletMaps::switchMap(const char *switchFunction) {
  QString code = "%1()";
  executeJavascript(code.arg(switchFunction));
}

void LeafletMaps::switchToCustomTileLayer(const QString &customTileLayerUrl) {
  QString code = "switchToCustomTileLayer('%1')";
  executeJavascript(code.arg(customTileLayerUrl));
}

void LeafletMaps::setMapCenter(double latitude, double longitude) {
  QString code = "map.setView(L.latLng(%1, %2), map.getZoom());";
  executeJavascript(code.arg(latitude).arg(longitude));
}

Coord LeafletMaps::getPixelPosOnScreenForLatLng(double lat, double lng) {
  QString code = "map.latLngToContainerPoint(L.latLng(%1, %2)).toString();";
  QString pointStr = executeJavascript(code.arg(lat).arg(lng)).toString();

  int pos = pointStr.indexOf('(') + 1;
  QString xStr = pointStr.mid(pos, pointStr.lastIndexOf(',') - pos);
  pos = pointStr.lastIndexOf(',') + 1;
  QString yStr = pointStr.mid(pos, pointStr.lastIndexOf(')') - pos);

  bool ok;
  return Coord(xStr.toDouble(&ok), yStr.toDouble(&ok), 0);
}

std::pair<double, double> LeafletMaps::getLatLngForPixelPosOnScreen(int x, int y) {
  QString code = "map.containerPointToLatLng(L.point(%1, %2)).toString();";
  QString latLngStr = executeJavascript(code.arg(x).arg(y)).toString();

  int pos = latLngStr.indexOf('(') + 1;
  QString latStr = latLngStr.mid(pos, latLngStr.lastIndexOf(',') - pos);
  pos = latLngStr.lastIndexOf(',') + 1;
  QString lngStr = latLngStr.mid(pos, latLngStr.lastIndexOf(')') - pos);
  return make_pair(latStr.toDouble(), lngStr.toDouble());
}

int LeafletMaps::getCurrentMapZoom() {
  return executeJavascript("map.getZoom();").toInt();
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
  QVariant ret = executeJavascript("map.getCenter().toString();");

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

void LeafletMaps::setMapBounds(Graph *graph,
                               const unordered_map<node, pair<double, double>> &nodesLatLngs) {

  if (!nodesLatLngs.empty()) {

    pair<double, double> minLatLng(90, 180);
    pair<double, double> maxLatLng(-90, -180);

    for (const auto &nll : nodesLatLngs) {
      if (graph->isElement(nll.first)) {
        minLatLng.first = std::min(minLatLng.first, nll.second.first);
        minLatLng.second = std::min(minLatLng.second, nll.second.second);
        maxLatLng.first = std::max(maxLatLng.first, nll.second.first);
        maxLatLng.second = std::max(maxLatLng.second, nll.second.second);
      }
    }

    QString code = "mapBounds = [];";
    code += QString("mapBounds.push(L.latLng(%1, %2));").arg(minLatLng.first).arg(minLatLng.second);
    code += QString("mapBounds.push(L.latLng(%1, %2));").arg(maxLatLng.first).arg(maxLatLng.second);
    code += "setMapBounds(mapBounds);";
    executeJavascript(code);
  }
}

} // namespace tlp
