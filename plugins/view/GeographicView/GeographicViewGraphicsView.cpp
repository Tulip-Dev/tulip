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

#include <algorithm>

#include "GeographicViewGraphicsView.h"
#include "GeographicView.h"
#include "NominatimGeocoder.h"

#include <tulip/GlCPULODCalculator.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlSphere.h>
#include <tulip/GlLine.h>
#include <tulip/GlSceneZoomAndPan.h>
#include <tulip/GlyphManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/Gl2DRect.h>

#include <QPushButton>
#include <QTextStream>
#include <QTimeLine>
#include <QFile>
#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QMainWindow>
#include <QProgressDialog>
#include <QOpenGLFramebufferObject>
#include <QOpenGLPaintDevice>

using namespace std;

namespace tlp {

const string planisphereTextureId = ":/tulip/view/geographic/planisphere.jpg";

GlComposite *readPolyFile(QString fileName) {
  GlComposite *composite = new GlComposite;

  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly)) {
    return nullptr;
  }

  string polygonName = "";
  vector<vector<Coord>> datas;
  vector<Coord> currentVector;
  bool ok;
  QString line;

  while (!file.atEnd()) {
    line = file.readLine();

    if (line.isEmpty() || line == "\n")
      continue;

    line.toUInt(&ok);

    if (ok) {
      if (!currentVector.empty())
        datas.push_back(currentVector);

      currentVector.clear();
      continue;
    }

    if (line == "END\n")
      continue;

    QStringList strList = line.split(" ");

    bool findLng = false;
    bool findLat = false;
    float lng;
    float lat;

    for (const auto &s : strList) {
      s.toDouble(&ok);

      if (ok) {
        if (!findLng) {
          findLng = true;
          lng = s.toDouble();
        } else {
          findLat = true;
          lat = s.toDouble();
        }
      }
    }

    if (!findLat) {

      if (!polygonName.empty()) {

        if (!currentVector.empty())
          datas.push_back(currentVector);

        if (!datas.empty()) {

          composite->addGlEntity(
              new GlComplexPolygon(datas, Color(0, 0, 0, 50), Color(0, 0, 0, 255)), polygonName);
          datas.clear();
          currentVector.clear();
        }
      }

      polygonName = QStringToTlpString(line);
      continue;
    }

    if (lat == 90.)
      lat = 89.999f;

    double mercatorLatitude = lat * 2. / 360. * M_PI;
    mercatorLatitude = sin(abs(mercatorLatitude));
    mercatorLatitude = log((1. + mercatorLatitude) / (1. - mercatorLatitude)) / 2.;

    if (lat < 0)
      mercatorLatitude = 0. - mercatorLatitude;

    if (mercatorLatitude * 360. / M_PI < -360)
      mercatorLatitude = -M_PI;

    currentVector.emplace_back(lng * 2., mercatorLatitude * 360. / M_PI, 0);
  }

  if (!polygonName.empty()) {
    if (!currentVector.empty())
      datas.push_back(currentVector);

    composite->addGlEntity(new GlComplexPolygon(datas, Color(0, 0, 0, 50), Color(0, 0, 0, 255)),
                           polygonName);
  }

  return composite;
}

GlComposite *readCsvFile(QString fileName) {

  GlComposite *composite = new GlComposite;

  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly))
    return nullptr;

  vector<vector<Coord>> datas;
  vector<Coord> currentVector;
  int lastIndex = 0;

  while (!file.atEnd()) {
    QString line(file.readLine());
    QStringList strList = line.split("\t");

    if (strList.size() != 3) {
      if (!currentVector.empty())
        datas.push_back(currentVector);

      currentVector.clear();
      continue;
    }

    if (strList[0].toInt() != lastIndex) {
      if (!currentVector.empty())
        datas.push_back(currentVector);

      lastIndex = strList[0].toInt();
      currentVector.clear();
    }

    double mercatorLatitude = strList[1].toDouble();
    mercatorLatitude = sin(abs(mercatorLatitude));
    mercatorLatitude = log((1. + mercatorLatitude) / (1. - mercatorLatitude)) / 2.;

    if (strList[1].toDouble() < 0)
      mercatorLatitude = 0. - mercatorLatitude;

    currentVector.emplace_back(strList[2].toDouble() * 360. / M_PI, mercatorLatitude * 360. / M_PI,
                               0);
  }

  if (datas.empty())
    return nullptr;

  composite->addGlEntity(new GlComplexPolygon(datas, Color(0, 0, 0, 50), Color(0, 0, 0, 255)),
                         "polygon");

  return composite;
}

void simplifyPolyFile(QString fileName, float definition) {

  map<string, vector<vector<Coord>>> clearPolygons;

  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly)) {
    return;
  }

  string polygonName = "";
  vector<vector<Coord>> datas;
  vector<Coord> currentVector;
  bool ok;

  while (!file.atEnd()) {
    QString line(file.readLine());

    if (line.isEmpty() || line == "\n")
      continue;

    line.toUInt(&ok);

    if (ok) {
      if (!currentVector.empty())
        datas.push_back(currentVector);

      currentVector.clear();
      continue;
    }

    if (line == "END\n")
      continue;

    QStringList strList = line.split(" ");

    bool findLng = false;
    bool findLat = false;
    float lng;
    float lat;

    for (const auto &s : strList) {
      s.toDouble(&ok);

      if (ok) {
        if (!findLng) {
          findLng = true;
          lng = s.toDouble();
        } else {
          findLat = true;
          lat = s.toDouble();
        }
      }
    }

    if (!findLat) {

      if (!polygonName.empty()) {

        if (!currentVector.empty())
          datas.push_back(currentVector);

        if (!datas.empty()) {

          clearPolygons[polygonName] = datas;
          datas.clear();
          currentVector.clear();
        }
      }

      polygonName = QStringToTlpString(line);
      continue;
    }

    double mercatorLatitude = lat * 2. / 360. * M_PI;
    mercatorLatitude = sin(abs(mercatorLatitude));
    mercatorLatitude = log((1. + mercatorLatitude) / (1. - mercatorLatitude)) / 2.;

    if (lat < 0)
      mercatorLatitude = 0. - mercatorLatitude;

    currentVector.emplace_back(lng, lat, 0);
  }

  if (!polygonName.empty()) {
    if (!currentVector.empty())
      datas.push_back(currentVector);

    clearPolygons.emplace(polygonName, std::move(datas));
  }

  unordered_map<Coord, Coord> simplifiedCoord;

  QString newName(fileName);
  newName.replace(".poly", QString("_") + QString::number(definition) + ".poly");
  QFile fileW(newName);

  if (!fileW.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return;
  }

  QTextStream out(&fileW);

  Coord *lastCoord = nullptr;

  for (auto &poly : clearPolygons) {
    out << poly.first.c_str();

    unsigned int i = 1;

    for (auto &vcoord : poly.second) {
      out << i << "\n";

      for (auto &coord : vcoord) {
        if (lastCoord == nullptr) {
          out << coord[0] << " " << coord[1] << "\n";
          lastCoord = &coord;
        } else {
          if (lastCoord->dist(coord) > definition) {
            if (simplifiedCoord.count(coord) == 0) {
              out << coord[0] << " " << coord[1] << "\n";
              lastCoord = &coord;
            } else {
              lastCoord = &simplifiedCoord[coord];
              out << (*lastCoord)[0] << " " << (*lastCoord)[1] << "\n";
            }
          } else {
            if (simplifiedCoord.count(coord) == 0)
              simplifiedCoord[coord] = *lastCoord;
          }
        }
      }

      out << "END\n";

      ++i;
    }
  }
}

static double latitudeToMercator(double latitude) {
  double mercatorLatitude = latitude * M_PI / 360.;
  mercatorLatitude = sin(abs(mercatorLatitude));
  mercatorLatitude = log((1. + mercatorLatitude) / (1. - mercatorLatitude)) / 2.;

  if (latitude < 0)
    return -mercatorLatitude / M_PI * 360.;

  return mercatorLatitude / M_PI * 360.;
}

static double mercatorToLatitude(double mercator) {
  return atan(sinh(mercator / 360. * M_PI)) / M_PI * 360.;
}

GeographicViewGraphicsView::GeographicViewGraphicsView(GeographicView *geoView,
                                                       QGraphicsScene *graphicsScene,
                                                       QWidget *parent)
    : QGraphicsView(graphicsScene, parent), _geoView(geoView), graph(nullptr), leafletMaps(nullptr),
      globeCameraBackup(nullptr, true), mapCameraBackup(nullptr, true), geoLayout(nullptr),
      geoViewSize(nullptr), geoViewShape(nullptr), geoLayoutBackup(nullptr),
      mapTranslationBlocked(false), geocodingActive(false), cancelGeocoding(false),
      polygonEntity(nullptr), planisphereEntity(nullptr), noLayoutMsgBox(nullptr),
      firstGlobeSwitch(true), geoLayoutComputed(false), renderFbo(nullptr), latProp(nullptr),
      lngProp(nullptr) {
  mapTextureId = "leafletMap" + to_string(reinterpret_cast<uintptr_t>(this));
  setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing |
                 QPainter::TextAntialiasing);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  leafletMaps = new LeafletMaps();
  leafletMaps->setMouseTracking(false);
  leafletMaps->resize(512, 512);
  connect(leafletMaps, SIGNAL(currentZoomChanged()), _geoView, SLOT(currentZoomChanged()));
  _placeholderItem = new QGraphicsRectItem(0, 0, 1, 1);
  _placeholderItem->setBrush(Qt::transparent);
  _placeholderItem->setPen(QPen(Qt::transparent));
  scene()->addItem(_placeholderItem);

  addressSelectionDialog = new AddressSelectionDialog(Perspective::instance()->mainWindow());

  auto proxyGM = scene()->addWidget(leafletMaps);
  proxyGM->setPos(0, 0);
  proxyGM->setParentItem(_placeholderItem);

  glMainWidget = new GlMainWidget(nullptr, geoView);
  delete glMainWidget->getScene()->getCalculator();
  glMainWidget->getScene()->setCalculator(new GlCPULODCalculator());
  glMainWidget->getScene()->setBackgroundColor(Color::White);

  glWidgetItem = new GlMainWidgetGraphicsItem(glMainWidget, 512, 512);
  glWidgetItem->setPos(0, 0);

  // disable user input
  // before allowing some display feedback
  tlp::disableQtUserInput();

  while (!leafletMaps->pageInit()) {
    QApplication::processEvents();
  }

#ifdef QT_HAS_WEBENGINE
  tId = 0;
  connect(leafletMaps, SIGNAL(refreshMap()), this, SLOT(queueMapRefresh()));
#else
  connect(leafletMaps, SIGNAL(refreshMap()), this, SLOT(refreshMap()));
#endif

  // re-enable user input
  tlp::enableQtUserInput();

  scene()->addItem(glWidgetItem);
  glWidgetItem->setParentItem(_placeholderItem);

  // combo box to choose the map type
  viewTypeComboBox = new QComboBox;
  viewTypeComboBox->addItems(
      QStringList() << _geoView->getViewNameFromType(GeographicView::OpenStreetMap)
                    << _geoView->getViewNameFromType(GeographicView::OpenStreetMap)
                    << _geoView->getViewNameFromType(GeographicView::EsriSatellite)
                    << _geoView->getViewNameFromType(GeographicView::EsriTerrain)
                    << _geoView->getViewNameFromType(GeographicView::EsriGrayCanvas)
                    << _geoView->getViewNameFromType(GeographicView::LeafletCustomTileLayer)
                    << _geoView->getViewNameFromType(GeographicView::Polygon)
                    << _geoView->getViewNameFromType(GeographicView::Globe));
  viewTypeComboBox->insertSeparator(1);

  QGraphicsProxyWidget *comboBoxProxy = scene()->addWidget(viewTypeComboBox);
  comboBoxProxy->setParentItem(_placeholderItem);
  comboBoxProxy->setPos(20, 20);
  comboBoxProxy->setZValue(1);

  connect(viewTypeComboBox, SIGNAL(currentIndexChanged(QString)), _geoView,
          SLOT(viewTypeChanged(QString)));

  // 2 push buttons
  // zoom +
  zoomInButton = new QPushButton(QIcon(":/tulip/view/geographic/zoom+.png"), "");
  zoomInButton->setFixedSize(29, 27);
  zoomInButton->setContentsMargins(0, 0, 0, 0);
  connect(zoomInButton, SIGNAL(pressed()), _geoView, SLOT(zoomIn()));
  QGraphicsProxyWidget *buttonProxy = scene()->addWidget(zoomInButton);
  buttonProxy->setParentItem(_placeholderItem);
  buttonProxy->setPos(20, 50);

  // zoom -
  zoomOutButton = new QPushButton(QIcon(":/tulip/view/geographic/zoom-.png"), "");
  zoomOutButton->setFixedSize(29, 27);
  zoomOutButton->setContentsMargins(0, 0, 0, 0);
  connect(zoomOutButton, SIGNAL(pressed()), _geoView, SLOT(zoomOut()));
  buttonProxy = scene()->addWidget(zoomOutButton);
  buttonProxy->setParentItem(_placeholderItem);
  buttonProxy->setPos(20, 76);

  QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, "Geolocated layout not initialized",
                                        "Warning: the geolocated layout\n"
                                        "has not been initialized yet.\n"
                                        "The graph will not be displayed until\n"
                                        "that operation has been performed.\n\n"
                                        "Open the Geolocation configuration tab\n"
                                        "to proceed.");
  msgBox->setModal(false);
  noLayoutMsgBox = scene()->addWidget(msgBox);
  noLayoutMsgBox->setParentItem(_placeholderItem);

  setAcceptDrops(false);
}

GeographicViewGraphicsView::~GeographicViewGraphicsView() {
#ifdef QT_HAS_WEBENGINE
  // first kill refreshMap timer if any
  // and reset tId to try to ensure refreshMap
  // will not be called later
  if (tId) {
    killTimer(tId);
    tId = 0;
  }
#endif
  if (geocodingActive) {
    if (addressSelectionDialog->isVisible()) {
      addressSelectionDialog->accept();
    }

    cancelGeocoding = true;

    // disable user input
    // before allowing some display feedback
    tlp::disableQtUserInput();

    while (geocodingActive) {
      QApplication::processEvents();
    }

    // reenable user input
    tlp::enableQtUserInput();
  }

  cleanup();
  // delete the graphics scene and all the items it contains
  delete scene();
}

void GeographicViewGraphicsView::cleanup() {
  if (graph) {

    GlScene *scene = glMainWidget->getScene();
    scene->clearLayersList();

    if (geoLayout != graph->getProperty<LayoutProperty>("viewLayout"))
      delete geoLayout;

    if (geoViewSize != graph->getProperty<SizeProperty>("viewSize"))
      delete geoViewSize;

    if (geoViewShape != graph->getProperty<IntegerProperty>("viewShape"))
      delete geoViewShape;

    // those entities have been deleted by the prior call to GlScene::clearLayersList,
    // so reset the pointers to nullptr value
    polygonEntity = nullptr;
    planisphereEntity = nullptr;
  }
}

void GeographicViewGraphicsView::setGraph(Graph *graph) {
  if (this->graph != graph) {

    GlGraphRenderingParameters rp;

    if (this->graph) {
      rp = glMainWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
    } else {
      rp.setNodesLabelStencil(1);
      rp.setLabelsAreBillboarded(true);
    }

    cleanup();
    this->graph = graph;

    GlScene *scene = glMainWidget->getScene();
    GlGraphComposite *graphComposite = new GlGraphComposite(graph);
    graphComposite->setVisible(false);
    graphComposite->setRenderingParameters(rp);
    GlLayer *layer = scene->createLayer("Main");

    layer->addGlEntity(graphComposite, "graph");

    backgroundLayer = new GlLayer("Background");
    backgroundLayer->set2DMode();
    Gl2DRect *backgroundRect = new Gl2DRect(0, 1, 0, 1, mapTextureId, true);
    backgroundLayer->addGlEntity(backgroundRect, "geoview_background");
    scene->addExistingLayerBefore(backgroundLayer, "Main");

    if (geoLayout)
      geoLayout->removeListener(this);
    geoLayout = graph->getProperty<LayoutProperty>("viewLayout");
    geoLayout->addListener(this);

    geoViewSize = graph->getProperty<SizeProperty>("viewSize");
    geoViewShape = graph->getProperty<IntegerProperty>("viewShape");
    polygonEntity = nullptr;

    draw();
  }
}

static unsigned char utf8ToLower(unsigned char c1, unsigned char c2) {
  // C3 80 C3 81 C3 82 C3 83 C3 84 C3 85 C3 A0 C3 A1 C3 A2 C3 A3 C3 A4 C3 A5 C2 AA // two-byte codes
  // for "a" C3 88 C3 89 C3 8A C3 8B C3 A8 C3 A9 C3 AA C3 AB // two-byte codes for "e" C3 8C C3 8D
  // C3 8E C3 8F C3 AC C3 AD C3 AE C3 AF // two-byte codes for "i" C3 92 C3 93 C3 94 C3 95 C3 96 C3
  // B2 C3 B3 C3 B4 C3 B5 C3 B6 C2 BA // two-byte codes for "o" C3 99 C3 9A C3 9B C3 9C C3 B9 C3 BA
  // C3 BB C3 BC // two-byte codes for "u" C2 A9 C3 87 C3 A7 // two-byte codes for "c" C3 91 C3 B1
  // // two-byte codes for "n" C2 AE // two-byte codes for "r" C3 9F // two-byte codes for "s" C5 BD
  // C5 BE // two-byte codes for "z" C5 B8 C3 9D C3 BD C3 BF // two-byte codes for "y"

  if (c1 == 0xC2) {
    if (c2 == 0xAA) {
      return 'a';
    }
    if (c2 == 0xBA) {
      return 'o';
    }
    if (c2 == 0xA9) {
      return 'c';
    }
    if (c2 == 0xAE) {
      return 'r';
    }
  }

  if (c1 == 0xC3) {
    if (c2 >= 0x80 && c2 <= 0x85) {
      return 'a';
    }
    if (c2 >= 0xA0 && c2 <= 0xA5) {
      return 'a';
    }
    if (c2 >= 0x88 && c2 <= 0x8B) {
      return 'e';
    }
    if (c2 >= 0xA8 && c2 <= 0xAB) {
      return 'e';
    }
    if (c2 >= 0x8C && c2 <= 0x8F) {
      return 'i';
    }
    if (c2 >= 0xAC && c2 <= 0xAF) {
      return 'i';
    }
    if (c2 >= 0x92 && c2 <= 0x96) {
      return 'o';
    }
    if (c2 >= 0xB2 && c2 <= 0xB6) {
      return 'o';
    }
    if (c2 >= 0x99 && c2 <= 0x9C) {
      return 'u';
    }
    if (c2 >= 0xB9 && c2 <= 0xBC) {
      return 'u';
    }
    if (c2 == 0x87) {
      return 'c';
    }
    if (c2 == 0xA7) {
      return 'c';
    }
    if (c2 == 0x91) {
      return 'n';
    }
    if (c2 == 0xB1) {
      return 'n';
    }
    if (c2 == 0x9F) {
      return 's';
    }
    if (c2 == 0x9D) {
      return 'y';
    }
    if (c2 == 0xBD || c2 == 0xBF) {
      return 'y';
    }
  }

  if (c1 == 0xC5) {
    if (c2 == 0xBD || c2 == 0xBE) {
      return 'z';
    }
    if (c2 == 0xB8) {
      return 'y';
    }
  }

  return c1;
}

static unsigned char utf8ToLower(unsigned char c) {
  // C0 C1 C2 C3 C4 C5 E0 E1 E2 E3 E4 E5 AA // one-byte codes for "a"
  // C8 C9 CA CB E8 E9 EA EB // one-byte codes for "e"
  // CC CD CE CF EC ED EE EF // one-byte codes for "i"
  // D2 D3 D4 D5 D6 F2 F3 F4 F5 F6 BA // one-byte codes for "o"
  // D9 DA DB DC F9 FA FB FC // one-byte codes for "u"
  // A9 C7 E7 // one-byte codes for "c"
  // D1 F1 // one-byte codes for "n"
  // AE // one-byte codes for "r"
  // DF // one-byte codes for "s"
  // 8E 9E // one-byte codes for "z"
  // 9F DD FD FF // one-byte codes for "y"

  if ((c >= 0xC0 && c <= 0xC5) || (c >= 0xE1 && c <= 0xE5) || c == 0xAA) {
    return 'a';
  }

  if ((c >= 0xC8 && c <= 0xCB) || (c >= 0xE8 && c <= 0xEB)) {
    return 'e';
  }

  if ((c >= 0xCC && c <= 0xCF) || (c >= 0xEC && c <= 0xEF)) {
    return 'i';
  }

  if ((c >= 0xD2 && c <= 0xD6) || (c >= 0xF2 && c <= 0xF6) || c == 0xBA) {
    return 'o';
  }

  if ((c >= 0xD9 && c <= 0xDC) || (c >= 0xF9 && c <= 0xFC)) {
    return 'u';
  }

  if (c == 0xA9 || c == 0xC7 || c == 0xE7) {
    return 'c';
  }

  if (c == 0xD1 || c == 0xF1) {
    return 'n';
  }

  if (c == 0xAE) {
    return 'r';
  }

  if (c == 0xDF) {
    return 's';
  }

  if (c == 0x8E || c == 0x9E) {
    return 'z';
  }

  if (c == 0x9F || c == 0xDD || c == 0xFD || c == 0xFF) {
    return 'y';
  }

  return c;
}

static void utf8ToLower(string &s) {
  // C0 C1 C2 C3 C4 C5 E0 E1 E2 E3 E4 E5 AA // one-byte codes for "a"
  // C3 80 C3 81 C3 82 C3 83 C3 84 C3 85 C3 A0 C3 A1 C3 A2 C3 A3 C3 A4 C3 A5 C2 AA // two-byte codes
  // for "a"

  // C8 C9 CA CB E8 E9 EA EB // one-byte codes for "e"
  // C3 88 C3 89 C3 8A C3 8B C3 A8 C3 A9 C3 AA C3 AB // two-byte codes for "e"

  // CC CD CE CF EC ED EE EF // one-byte codes for "i"
  // C3 8C C3 8D C3 8E C3 8F C3 AC C3 AD C3 AE C3 AF // two-byte codes for "i"

  // D2 D3 D4 D5 D6 F2 F3 F4 F5 F6 BA // one-byte codes for "o"
  // C3 92 C3 93 C3 94 C3 95 C3 96 C3 B2 C3 B3 C3 B4 C3 B5 C3 B6 C2 BA // two-byte codes for "o"

  // D9 DA DB DC F9 FA FB FC // one-byte codes for "u"
  // C3 99 C3 9A C3 9B C3 9C C3 B9 C3 BA C3 BB C3 BC // two-byte codes for "u"

  // A9 C7 E7 // one-byte codes for "c"
  // C2 A9 C3 87 C3 A7 // two-byte codes for "c"

  // D1 F1 // one-byte codes for "n"
  // C3 91 C3 B1 // two-byte codes for "n"

  // AE // one-byte codes for "r"
  // C2 AE // two-byte codes for "r"

  // DF // one-byte codes for "s"
  // C3 9F // two-byte codes for "s"

  // 8E 9E // one-byte codes for "z"
  // C5 BD C5 BE // two-byte codes for "z"

  // 9F DD FD FF // one-byte codes for "y"
  // C5 B8 C3 9D C3 BD C3 BF // two-byte codes for "y"

  int n = s.size();
  int pos = 0;
  for (int i = 0; i < n; ++i, ++pos) {
    unsigned char c = static_cast<unsigned char>(s[i]);
    if (c >= 0x80) {
      if (i < (n - 1) && static_cast<unsigned char>(s[i + 1]) >= 0x80) {
        unsigned char c2 = utf8ToLower(c, static_cast<unsigned char>(s[i + 1]));
        if (c2 < 0x80) {
          s[pos] = c2;
          i++;
        } else {
          // s.at(pos) = utf8ToLower(c);
          // if it's a double code we don't recognize, skip both characters;
          // this does mean that we lose the chance to handle back-to-back extended ascii characters
          // but we'll assume that is less likely than a unicode "combining character" or other
          // unrecognized unicode character for data
          i++;
        }
      } else {
        unsigned char c2 = utf8ToLower(c);
        if (c2 < 0x80) {
          s[pos] = c2;
        } else {
          // skip unrecognized single-byte codes
          pos--;
        }
      }
    } else {
      if (c >= 'A' && c <= 'Z') {
        s[pos] = c + ('a' - 'A');
      } else {
        s[pos] = c;
      }
    }
  }
  if (pos < n) {
    s.resize(pos);
  }
}

static string cleanupAddress(const string &addr) {
  string s(addr);
  // remove quotes if any
  if (s[0] == '"' && s[s.length() - 1] == '"')
    s = s.substr(1, s.length() - 2);

  // remove parens if any
  auto bpos = s.find('(');
  if (bpos != string::npos) {
    auto epos = s.find(')', bpos + 1);
    if (epos != string::npos)
      s.erase(s.begin() + bpos, s.begin() + epos + 1);
    else
      s.erase(s.begin() + bpos, s.end());
  }
  // right trim
  s.erase(s.find_last_not_of(" \n\r\t") + 1);

  return s;
}

void GeographicViewGraphicsView::loadDefaultMap() {
  bool oldPolyVisible = false;

  if (polygonEntity != nullptr) {
    oldPolyVisible = polygonEntity->isVisible();
    delete polygonEntity;
  }

  polygonEntity = readCsvFile(":/tulip/view/geographic/MAPAGR4.txt");
  polygonEntity->setVisible(oldPolyVisible);

  GlScene *scene = glMainWidget->getScene();
  GlLayer *layer = scene->getLayer("Main");
  layer->addGlEntity(polygonEntity, "polygonMap");
}

void GeographicViewGraphicsView::loadCsvFile(QString fileName) {
  bool oldPolyVisible = false;

  if (polygonEntity != nullptr) {
    oldPolyVisible = polygonEntity->isVisible();
    delete polygonEntity;
  }

  polygonEntity = readCsvFile(fileName);

  if (!polygonEntity) {
    QMessageBox::critical(nullptr, "Can't read .poly file",
                          "We can't read csv file: " + fileName + "\nVerify the file.");
    return;
  }

  polygonEntity->setVisible(oldPolyVisible);

  GlScene *scene = glMainWidget->getScene();
  GlLayer *layer = scene->getLayer("Main");
  layer->addGlEntity(polygonEntity, "polygonMap");
}

void GeographicViewGraphicsView::loadPolyFile(QString fileName) {
  bool oldPolyVisible = false;

  if (polygonEntity != nullptr) {
    oldPolyVisible = polygonEntity->isVisible();
    delete polygonEntity;
  }

  polygonEntity = readPolyFile(fileName);

  if (!polygonEntity) {
    QMessageBox::critical(nullptr, "Can't read .poly file",
                          "We can't read .poly file: " + fileName + "\nVerify the file.");
    return;
  }

  polygonEntity->setVisible(oldPolyVisible);

  GlScene *scene = glMainWidget->getScene();
  GlLayer *layer = scene->getLayer("Main");
  layer->addGlEntity(polygonEntity, "polygonMap");
}

void GeographicViewGraphicsView::mapToPolygon() {
  GlComposite *composite = polygonEntity;

  if (!composite)
    return;

  const map<string, GlSimpleEntity *> &entities = composite->getGlEntities();

  for (auto n : graph->nodes()) {

    Coord nodePos = geoLayout->getNodeValue(n);

    for (auto it : entities) {
      if (it.second->getBoundingBox().contains(nodePos)) {
        GlComplexPolygon *polygon = static_cast<GlComplexPolygon *>(it.second);

        const vector<vector<Coord>> &polygonSides = polygon->getPolygonSides();

        for (const auto &polygonSide : polygonSides) {
          bool oddNodes = false;
          Coord lastCoord = polygonSide[0];

          for (auto it = (++polygonSide.begin()); it != polygonSide.end(); ++it) {
            if ((((*it)[1] < nodePos[1] && lastCoord[1] >= nodePos[1]) ||
                 (lastCoord[1] < nodePos[1] && (*it)[1] >= nodePos[1])) &&
                ((*it)[0] <= nodePos[0] || lastCoord[0] <= nodePos[0])) {
              oddNodes ^= ((*it)[0] + (nodePos[1] - (*it)[1]) / (lastCoord[1] - (*it)[1]) *
                                          (lastCoord[0] - (*it)[0]) <
                           nodePos[0]);
            }

            lastCoord = (*it);
          }

          if (oddNodes) {

            BoundingBox bb;

            for (const auto &c : polygonSides[0]) {
              bb.expand(c);
            }

            geoLayout->setNodeValue(n, bb.center());
            polygon->setFillColor(graph->getProperty<ColorProperty>("viewColor")->getNodeValue(n));
            polygon->setOutlineColor(
                graph->getProperty<ColorProperty>("viewBorderColor")->getNodeValue(n));
            break;
          }
        }
      }
    }
  }
}

void GeographicViewGraphicsView::zoomIn() {
  leafletMaps->setCurrentZoom(leafletMaps->getCurrentMapZoom() + 1);
}

void GeographicViewGraphicsView::zoomOut() {
  leafletMaps->setCurrentZoom(leafletMaps->getCurrentMapZoom() - 1);
}

void GeographicViewGraphicsView::currentZoomChanged() {
  zoomInButton->setEnabled(leafletMaps->getCurrentMapZoom() != 20);
  zoomOutButton->setEnabled(leafletMaps->getCurrentMapZoom() != 0);
}

GlGraphComposite *GeographicViewGraphicsView::getGlGraphComposite() const {
  return glMainWidget->getScene()->getGlGraphComposite();
}

void GeographicViewGraphicsView::createLayoutWithAddresses(const string &addressPropertyName,
                                                           bool createLatAndLngProps,
                                                           bool resetLatAndLngValues,
                                                           bool automaticChoice) {
  geocodingActive = true;
  nodeLatLng.clear();
  Observable::holdObservers();

  if (graph->existProperty(addressPropertyName)) {
    StringProperty *addressProperty = graph->getProperty<StringProperty>(addressPropertyName);

    if (createLatAndLngProps) {
      latProp = graph->getProperty<DoubleProperty>("latitude");
      lngProp = graph->getProperty<DoubleProperty>("longitude");
    }

    int nbNodes = graph->numberOfNodes();
    int nbNodesProcessed = 0;

    pair<double, double> latLng;
    unordered_map<string, pair<double, double>> addressesLatLngMap;

    NominatimGeocoder nominatimGeocoder;

    int failures = 0;

    // hide msg
    noLayoutMsgBox->setVisible(false);
    {
      QProgressDialog progress("Retrieving latitude/longitude for address: ", "Cancel", 0, nbNodes,
                               Perspective::instance()->mainWindow());
      progress.setWindowTitle("Retrieving latitude/longitude for address: ");
      progress.setMinimumWidth(400);
      progress.setWindowModality(Qt::WindowModal);

      for (auto n : graph->nodes()) {
        progress.setValue(++nbNodesProcessed);
        if (progress.wasCanceled() || cancelGeocoding)
          break;

        string addrValue = addressProperty->getNodeValue(n);

        if (addrValue.empty())
          continue;

        progress.setLabelText(tlpStringToQString(addrValue));

        if (nodeLatLng.find(n) == nodeLatLng.end()) {
          auto addr = cleanupAddress(addrValue);
          if (addressesLatLngMap.find(addr) != addressesLatLngMap.end()) {
            latLng = nodeLatLng[n] = addressesLatLngMap[addr];

            if (createLatAndLngProps) {
              latProp->setNodeValue(n, latLng.first);
              lngProp->setNodeValue(n, latLng.second);
            }
          } else {
            if (!resetLatAndLngValues) {
              // check if latitude/longitude are already set
              latLng.first = latProp->getNodeValue(n);
              latLng.second = lngProp->getNodeValue(n);
              if (latLng.first != 0 || latLng.second != 0) {
                nodeLatLng[n] = addressesLatLngMap[addr] = latLng;
                continue;
              }
            }

            unsigned int idx = 0;
            vector<NominatimGeocoderResult> &&geocodingResults =
                nominatimGeocoder.getLatLngForAddress(addr);

            if (geocodingResults.size() > 1) {
              if (automaticChoice) {
                // choose the returned address for whom the input address
                // is found the closest to its beginning
                // we use lower case conversion to improve matching
                utf8ToLower(addr);
                size_t bpos = string::npos;
                unsigned i = 0;
                idx = geocodingResults.size();

                for (auto &result : geocodingResults) {
                  auto &address = result.address;
                  // lower case conversion
                  utf8ToLower(address);

                  auto pos = address.find(addr);
                  if (pos == 0) {
                    idx = i;
                    break;
                  }
                  if (pos < bpos) {
                    bpos = pos;
                    idx = i;
                  }
                  ++i;
                }
                if (idx == geocodingResults.size()) {
                  // no exact match has been found
                  qInfo() << "No geolocation with exact match found for"
                          << tlpStringToQString(addrValue);
                  // we assume the first one is the best
                  idx = 0;
                }
              } else {
                addressSelectionDialog->clearList();
                addressSelectionDialog->setBaseAddress(tlpStringToQString(addr));

                for (auto &result : geocodingResults) {
                  addressSelectionDialog->addResultToList(tlpStringToQString(result.address));
                }

                addressSelectionDialog->exec();
                idx = addressSelectionDialog->getPickedResultIdx();
              }
            } else if (geocodingResults.empty()) {
              qWarning() << "No geolocation found for" << tlpStringToQString(addrValue);
              failures++;
              continue;
            }

            const pair<double, double> &latLng = geocodingResults[idx].latLng;
            nodeLatLng[n] = addressesLatLngMap[addr] = latLng;

            if (createLatAndLngProps) {
              latProp->setNodeValue(n, latLng.first);
              lngProp->setNodeValue(n, latLng.second);
            }
          }
        }
      }
    }
    if (failures) {
      QString msg = QString("%1 %2 have not been geolocated.\nDo you want to see %3?")
                        .arg(failures > 1 ? QString::number(failures) : QString("One"))
                        .arg(failures > 1 ? "addresses" : "address")
                        .arg(failures > 1 ? "them" : "it");

      if (QMessageBox::warning(Perspective::instance()->mainWindow(), "Geolocation failed", msg,
                               QMessageBox::Yes | QMessageBox::No,
                               QMessageBox::Yes) == QMessageBox::Yes)
        Perspective::showLogMessages();
    }
  }

  Observable::unholdObservers();
  geocodingActive = false;
}

void GeographicViewGraphicsView::createLayoutWithLatLngs(const std::string &latPropName,
                                                         const std::string &lngPropName,
                                                         const string &edgesPathsPropertyName) {
  nodeLatLng.clear();
  pair<double, double> latLng;

  if (graph->existProperty(latPropName) && graph->existProperty(lngPropName)) {
    latProp = graph->getProperty<DoubleProperty>(latPropName);
    lngProp = graph->getProperty<DoubleProperty>(lngPropName);
    for (auto n : graph->nodes()) {
      latLng.first = latProp->getNodeValue(n);
      latLng.second = lngProp->getNodeValue(n);
      nodeLatLng[n] = latLng;
    }
  }

  if (graph->existProperty(edgesPathsPropertyName)) {
    auto edgesPathsProperty = graph->getProperty<DoubleVectorProperty>(edgesPathsPropertyName);
    for (auto e : graph->edges()) {
      auto &edgePath = edgesPathsProperty->getEdgeValue(e);
      std::vector<std::pair<double, double>> latLngs;
      latLngs.reserve(edgePath.size() / 2);

      for (size_t i = 0; i < edgePath.size(); i += 2) {
        latLngs.emplace_back(edgePath[i], edgePath[i + 1]);
      }

      edgeBendsLatLng[e] = latLngs;
    }
  }
}

void GeographicViewGraphicsView::resizeEvent(QResizeEvent *event) {
  QGraphicsView::resizeEvent(event);
  scene()->setSceneRect(QRect(QPoint(0, 0), size()));
  leafletMaps->resize(width(), height());
  glWidgetItem->resize(width(), height());

  if (noLayoutMsgBox && noLayoutMsgBox->isVisible()) {
    noLayoutMsgBox->setPos(width() / 2 - noLayoutMsgBox->sceneBoundingRect().width() / 2,
                           height() / 2 - noLayoutMsgBox->sceneBoundingRect().height() / 2);
  }

  if (scene()) {
    scene()->update();
  }
}

#ifdef QT_HAS_WEBENGINE
void GeographicViewGraphicsView::queueMapRefresh() {
  tId = startTimer(10);
}

void GeographicViewGraphicsView::timerEvent(QTimerEvent *event) {
  killTimer(event->timerId());
  // call refreshMap if needed
  // accessing this->tId may result in a Free Memory Read
  // because surprisingly this method may be called
  // after this has been deleted
  if (tId == event->timerId()) {
    tId = 0;
    refreshMap();
  }
}
#endif

void GeographicViewGraphicsView::refreshMap() {
  if (!leafletMaps->isVisible() || !leafletMaps->mapLoaded() || !glMainWidget->isCurrent()) {
    return;
  }

  BoundingBox bb;
  Coord rightCoord = leafletMaps->getPixelPosOnScreenForLatLng(180, 180);
  Coord leftCoord = leafletMaps->getPixelPosOnScreenForLatLng(0, 0);

  if (rightCoord[0] - leftCoord[0]) {
    float mapWidth = (width() / (rightCoord - leftCoord)[0]) * 180.;
    float middleLng =
        leafletMaps->getLatLngForPixelPosOnScreen(width() / 2., height() / 2.).second * 2.;
    bb.expand(Coord(middleLng - mapWidth / 2.,
                    latitudeToMercator(leafletMaps->getLatLngForPixelPosOnScreen(0, 0).first * 2.),
                    0));
    bb.expand(Coord(
        middleLng + mapWidth / 2.,
        latitudeToMercator(leafletMaps->getLatLngForPixelPosOnScreen(width(), height()).first * 2.),
        0));
    GlSceneZoomAndPan sceneZoomAndPan(glMainWidget->getScene(), bb, "Main", 1);
    sceneZoomAndPan.zoomAndPanAnimationStep(1);
  }

  updateMapTexture();
  glWidgetItem->setRedrawNeeded(true);
  scene()->update();
}

void GeographicViewGraphicsView::setMapTranslationBlocked(const bool translationBlocked) {
  mapTranslationBlocked = translationBlocked;
}

void GeographicViewGraphicsView::centerView() {
  if (leafletMaps->isVisible()) {
    leafletMaps->setMapBounds(graph, nodeLatLng);
  } else {
    glMainWidget->centerScene();
  }
}
void GeographicViewGraphicsView::centerMapOnNode(const node n) {
  if (nodeLatLng.find(n) != nodeLatLng.end()) {
    leafletMaps->setMapCenter(nodeLatLng[n].first, nodeLatLng[n].second);
  }
}

void GeographicViewGraphicsView::setGeoLayout(LayoutProperty *property) {
  if (property != geoLayout) {
    geoLayout->removeListener(this);
    *property = *geoLayout;
    geoLayout = property;
    geoLayout->addListener(this);
    glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementLayout(geoLayout);
  }
}

void GeographicViewGraphicsView::setGeoSizes(SizeProperty *property) {
  *property = *geoViewSize;
  geoViewSize = property;
  glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementSize(geoViewSize);
}

void GeographicViewGraphicsView::setGeoShape(IntegerProperty *property) {
  *property = *geoViewShape;
  geoViewShape = property;
  glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementShape(geoViewShape);
}

void GeographicViewGraphicsView::treatEvent(const Event &ev) {
  const PropertyEvent *propEvt = dynamic_cast<const PropertyEvent *>(&ev);

  if (propEvt && propEvt->getType() == PropertyEvent::TLP_AFTER_SET_NODE_VALUE &&
      propEvt->getProperty() == geoLayout) {
    // update latitude/longitude properties
    if (latProp && lngProp) {
      node n = propEvt->getNode();
      pair<double, double> latLng;
      const auto &l = geoLayout->getNodeValue(n);
      latProp->setNodeValue(n, latLng.first = mercatorToLatitude(l.y()) / 2.);
      lngProp->setNodeValue(n, latLng.second = l.x() / 2.);
      nodeLatLng[n] = latLng;
    }
  }
}

void GeographicViewGraphicsView::switchViewType() {
  bool makeCurrent = !glMainWidget->isCurrent();
  if (makeCurrent)
    glMainWidget->makeCurrent();

  GeographicView::ViewType viewType = _geoView->viewType();

  bool enableLeafletMap = false;
  bool enablePolygon = false;
  bool enablePlanisphere = false;

  switch (viewType) {
  case GeographicView::OpenStreetMap: {
    enableLeafletMap = true;
    leafletMaps->switchToOpenStreetMap();
    break;
  }

  case GeographicView::EsriSatellite: {
    enableLeafletMap = true;
    leafletMaps->switchToEsriSatellite();
    break;
  }

  case GeographicView::EsriTerrain: {
    enableLeafletMap = true;
    leafletMaps->switchToEsriTerrain();
    break;
  }

  case GeographicView::EsriGrayCanvas: {
    enableLeafletMap = true;
    leafletMaps->switchToEsriGrayCanvas();
    break;
  }

  case GeographicView::LeafletCustomTileLayer: {
    enableLeafletMap = true;
    QString customTileLayerUrl = _geoView->getConfigWidget()->getCustomTileLayerUrl();
    leafletMaps->switchToCustomTileLayer(customTileLayerUrl);
    break;
  }

  case GeographicView::Polygon: {
    enablePolygon = true;
    glWidgetItem->setRedrawNeeded(true);
    break;
  }

  case GeographicView::Globe: {
    enablePlanisphere = true;
    break;
  }

  default:
    break;
  }

  if (planisphereEntity && planisphereEntity->isVisible()) {
    globeCameraBackup = glMainWidget->getScene()->getGraphCamera();
  } else {
    mapCameraBackup = glMainWidget->getScene()->getGraphCamera();
  }

  if (geoLayoutBackup != nullptr && geoLayoutComputed) {
    *geoLayout = *geoLayoutBackup;
    delete geoLayoutBackup;
    geoLayoutBackup = nullptr;
  }

  GlLayer *layer = glMainWidget->getScene()->getLayer("Main");

  if (geoLayout == graph->getProperty<LayoutProperty>("viewLayout") && geoLayoutComputed)
    graph->push();

  Observable::holdObservers();

  leafletMaps->setVisible(enableLeafletMap);
  backgroundLayer->setVisible(enableLeafletMap);

  if (polygonEntity)
    polygonEntity->setVisible(enablePolygon);

  layer->setCamera(new Camera(glMainWidget->getScene()));

  if (viewType != GeographicView::Globe && geoLayoutComputed) {
    geoLayout->removeListener(this);
    SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");

    for (auto n : graph->nodes()) {
      if (viewSize != geoViewSize) {
        const Size &nodeSize = viewSize->getNodeValue(n);
        geoViewSize->setNodeValue(n, nodeSize);
      }

      if (nodeLatLng.find(n) != nodeLatLng.end()) {
        geoLayout->setNodeValue(
            n, Coord(nodeLatLng[n].second * 2., latitudeToMercator(nodeLatLng[n].first * 2.), 0));
      }
    }
    geoLayout->addListener(this);

    if (!edgeBendsLatLng.empty()) {
      for (auto e : graph->edges()) {
        auto &eb = edgeBendsLatLng[e];
        vector<Coord> edgeBendsCoords;
        edgeBendsCoords.reserve(eb.size());
        for (unsigned int i = 0; i < eb.size(); ++i) {
          edgeBendsCoords.emplace_back(eb[i].second * 2., latitudeToMercator(eb[i].first * 2.), 0);
        }

        geoLayout->setEdgeValue(e, edgeBendsCoords);
      }
    }

    Camera &camera = glMainWidget->getScene()->getGraphCamera();
    camera.setEyes(mapCameraBackup.getEyes());
    camera.setCenter(mapCameraBackup.getCenter());
    camera.setUp(mapCameraBackup.getUp());
    camera.setZoomFactor(mapCameraBackup.getZoomFactor());
    camera.setSceneRadius(mapCameraBackup.getSceneRadius());
  }

  else {

    if (!planisphereEntity) {
      GlOffscreenRenderer::getInstance()->makeOpenGLContextCurrent();
      GlTextureManager::loadTexture(planisphereTextureId);
      planisphereEntity = new GlSphere(Coord(0., 0., 0.), 50., planisphereTextureId, 255, 0, 0, 90);
      glMainWidget->getScene()->getLayer("Main")->addGlEntity(planisphereEntity, "globeMap");
    }

    if (geoLayoutComputed) {

      SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");

      assert(geoLayoutBackup == nullptr);
      geoLayoutBackup = new LayoutProperty(graph);
      *geoLayoutBackup = *geoLayout;

      geoViewShape->setAllNodeValue(NodeShape::Sphere);
      geoViewShape->setAllEdgeValue(EdgeShape::CubicBSplineCurve);

      for (auto n : graph->nodes()) {
        if (viewSize != geoViewSize) {
          const Size &nodeSize = viewSize->getNodeValue(n);
          geoViewSize->setNodeValue(n, nodeSize);
        }

        if (nodeLatLng.find(n) != nodeLatLng.end()) {
          Coord tmp(nodeLatLng[n].first * 2. / 360. * M_PI, nodeLatLng[n].second * 2. / 360. * M_PI,
                    0);

          float lambda = tmp[1];
          float theta;

          if (lambda <= M_PI)
            theta = lambda;
          else
            theta = lambda + 2. * M_PI;

          float phi = M_PI / 2.0 - tmp[0];

          tmp = Coord(50. * sin(phi) * cos(theta), 50. * sin(phi) * sin(theta), 50. * cos(phi));
          geoLayout->setNodeValue(n, tmp);
        }
      }

      for (auto e : graph->edges()) {
        auto eEnds = graph->ends(e);
        node src = eEnds.first;
        node tgt = eEnds.second;
        Coord srcC(nodeLatLng[src].first * 2. / 360. * M_PI,
                   nodeLatLng[src].second * 2. / 360. * M_PI, 0);
        Coord tgtC(nodeLatLng[tgt].first * 2. / 360. * M_PI,
                   nodeLatLng[tgt].second * 2. / 360. * M_PI, 0);

        unsigned int bendsNumber = 2;
        vector<Coord> bends;
        bends.reserve(bendsNumber);

        for (unsigned int i = 0; i < bendsNumber; ++i) {
          Coord &&tmp = srcC + ((tgtC - srcC) / (bendsNumber + 1.f)) * (i + 1.f);
          float lambda = tmp[1];
          float theta;

          if (lambda <= M_PI)
            theta = lambda;
          else
            theta = lambda + 2. * M_PI;

          float phi = M_PI / 2.0 - tmp[0];

          bends.emplace_back(75. * sin(phi) * cos(theta), 75. * sin(phi) * sin(theta),
                             75. * cos(phi));
        }

        geoLayout->setEdgeValue(e, bends);
      }
    }

    if (firstGlobeSwitch) {
      firstGlobeSwitch = false;

      glMainWidget->getScene()->centerScene();
      Camera &camera = glMainWidget->getScene()->getGraphCamera();
      float centerEyeDistance = (camera.getEyes() - camera.getCenter()).norm();
      camera.setCenter(Coord(0, 0, 0));
      camera.setEyes(Coord(centerEyeDistance, 0, 0));
      camera.setUp(Coord(0, 0, 1));

      globeCameraBackup = camera;

    } else {
      Camera &camera = glMainWidget->getScene()->getGraphCamera();
      camera.setEyes(globeCameraBackup.getEyes());
      camera.setCenter(globeCameraBackup.getCenter());
      camera.setUp(globeCameraBackup.getUp());
      camera.setZoomFactor(globeCameraBackup.getZoomFactor());
      camera.setSceneRadius(globeCameraBackup.getSceneRadius());
    }
  }

  if (planisphereEntity)
    planisphereEntity->setVisible(enablePlanisphere);

  glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->setEdge3D(
      viewType == GeographicView::Globe);

  Observable::unholdObservers();

  graph->popIfNoUpdates();

  if (makeCurrent)
    glMainWidget->doneCurrent();

  draw();
}

void GeographicViewGraphicsView::setGeoLayoutComputed() {
  geoLayoutComputed = true;
  noLayoutMsgBox->setVisible(false);
  glMainWidget->getScene()->getGlGraphComposite()->setVisible(true);
}

void GeographicViewGraphicsView::updateMapTexture() {
  int width = leafletMaps->geometry().width();
  int height = leafletMaps->geometry().height();

  QImage image(width, height, QImage::Format_RGB32);
  QPainter painter(&image);
  leafletMaps->render(&painter);
  painter.end();

  GlOffscreenRenderer::getInstance()->makeOpenGLContextCurrent();

  if (renderFbo == nullptr || renderFbo->width() != width || renderFbo->height() != height) {
    delete renderFbo;
    renderFbo = new QOpenGLFramebufferObject(width, height);
    GlTextureManager::registerExternalTexture(mapTextureId, renderFbo->texture());
  }

  renderFbo->bind();
  QOpenGLPaintDevice device(width, height);
  QPainter fboPainter(&device);
  fboPainter.drawImage(QRect(0, 0, width, height), image);
  fboPainter.end();
  renderFbo->release();
}

} // namespace tlp
