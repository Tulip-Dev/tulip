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
#include <tulip/TulipSettings.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/Gl2DRect.h>
#include <tulip/WorkspacePanel.h>
#include <tulip/NeedConfigurationMsgBox.h>

#include <QComboBox>
#include <QDesktopServices>
#include <QPushButton>
#include <QTextStream>
#include <QTimeLine>
#include <QFile>
#include <QApplication>
#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QProgressDialog>
#include <QOpenGLFramebufferObject>
#include <QOpenGLPaintDevice>
#include <QHBoxLayout>
#include <QVBoxLayout>

using namespace std;

namespace tlp {

const string planisphereTextureId = ":/tulip/view/geographic/planisphere.png";

GlComposite *readPolyFile(const QString &fileName) {
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
    float lng = 0.;
    float lat = 0.;

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

// latitude and longitude need some conversion
// to be displayed in a GlScene
#define GLSCENE_LAT(lat) latitudeToMercator((lat).first * 2.)
#define GLSCENE_LNG(lng) ((lng).second * 2.)

GeographicViewGraphicsView::GeographicViewGraphicsView(GeographicView *geoView,
                                                       QGraphicsScene *graphicsScene,
                                                       QWidget *parent)
    : QGraphicsView(graphicsScene, parent), _geoView(geoView), graph(nullptr), _geoMW(nullptr),
      globeCameraBackup(nullptr, true), mapCameraBackup(nullptr, true), latProp(nullptr),
      lngProp(nullptr), geoLayout(nullptr), geoLayoutBackup(nullptr), geoViewSize(nullptr),
      geoViewShape(nullptr), geocodingActive(false), abortGeocoding(false), polygonEntity(nullptr),
      planisphereEntity(nullptr), addressSelectionDialog(nullptr), noLayoutMsgBox(nullptr),
      firstGlobeSwitch(true), geoLayoutComputed(false), displayScale(false), displayCenter(false),
      scaleWidth(0), renderFbo(nullptr) {
  mapTextureId = "geoMap" + to_string(reinterpret_cast<uintptr_t>(this));
  setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing |
                 QPainter::TextAntialiasing);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  _geoMW = new GeoMapWidget(geoView->getMapLayers());
  _geoMW->setMouseTracking(false);
  _geoMW->resize(QSize(512, 512));
  connect(_geoMW, SIGNAL(currentZoomChanged()), this, SLOT(currentZoomChanged()));
  connect(_geoMW, SIGNAL(refreshMap()), this, SLOT(refreshMap()));

  _placeholderItem = new QGraphicsRectItem(0, 0, 1, 1);
  _placeholderItem->setBrush(Qt::transparent);
  _placeholderItem->setPen(QPen(Qt::transparent));
  scene()->addItem(_placeholderItem);

  auto proxyGM = scene()->addWidget(_geoMW);
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

  while (!_geoMW->mapLoaded()) {
    QApplication::processEvents();
  }

  // re-enable user input
  tlp::enableQtUserInput();

  scene()->addItem(glWidgetItem);
  glWidgetItem->setParentItem(_placeholderItem);

  // combo box to choose the map type
  mapTypeComboBox = new QComboBox;
  // add view types, OpenStreetMap is the default one
  mapTypeComboBox->addItem(GeographicView::getViewName(GeographicView::OpenStreetMap));
  mapTypeComboBox->insertSeparator(1);
  for (auto &layer : GeographicView::getMapLayers())
    mapTypeComboBox->addItem(layer.name);

  connect(mapTypeComboBox, SIGNAL(currentIndexChanged(int)), _geoView, SLOT(mapTypeChanged(int)));

  // 2 push buttons
  // zoom +
  bool darkMode = TulipSettings::isDisplayInDarkMode();
  zoomInButton = new QPushButton(QIcon(darkMode ? ":/tulip/view/geographic/white-zoom+.png"
                                                : ":/tulip/view/geographic/zoom+.png"),
                                 "");
  zoomInButton->setFixedSize(24, 24);
  connect(zoomInButton, SIGNAL(pressed()), this, SLOT(zoomIn()));
  // zoom -
  zoomOutButton = new QPushButton(QIcon(darkMode ? ":/tulip/view/geographic/white-zoom-.png"
                                                 : ":/tulip/view/geographic/zoom-.png"),
                                  "");
  zoomOutButton->setFixedSize(24, 24);
  connect(zoomOutButton, SIGNAL(pressed()), this, SLOT(zoomOut()));

  // configure a QFrame to layout map type combox and zoom buttons
  QFrame *frame = new QFrame();
  frame->setFrameStyle(QFrame::NoFrame);
  Perspective::setStyleSheet(frame);
  auto ss = Perspective::styleSheet();
  ss.append("#geoviewFrame { background-color: transparent; }");
  frame->setObjectName("geoviewFrame");
  frame->setStyleSheet(ss);
  QVBoxLayout *vLayout = new QVBoxLayout(frame);
  vLayout->addWidget(mapTypeComboBox);
  auto zoomLayout = new QVBoxLayout(nullptr);
  zoomLayout->setContentsMargins(0, 0, 0, 0);
  zoomLayout->setSpacing(0);
  zoomLayout->addWidget(zoomInButton);
  zoomLayout->addWidget(zoomOutButton);
  auto hLayout = new QHBoxLayout(nullptr);
  hLayout->addLayout(zoomLayout);
  hLayout->insertStretch(-1);
  vLayout->addLayout(hLayout);

  QGraphicsProxyWidget *frameProxy = scene()->addWidget(frame);
  frameProxy->setParentItem(_placeholderItem);
  frameProxy->setPos(20, 20);
  frameProxy->setZValue(1);

  QPushButton *okButton;
  noLayoutMsgBox = new_NeedConfigurationMsgBox("<font size=\"+1\"><b>The geolocated layout<br/>"
                                               "has not been initialized yet.</b></font><br/><br/>"
                                               "Open the <b>Geolocation</b> configuration tab<br/>"
                                               "to proceed.",
                                               &okButton);
  connect(okButton, SIGNAL(released()), this, SLOT(showGeolocationWidget()));
  connect(okButton, SIGNAL(released()), noLayoutMsgBox, SLOT(hide()));
  scene()->addWidget(noLayoutMsgBox);

  // init map attribution label
  attributionLabel = new QLabel();
  scene()->addWidget(attributionLabel)->setParentItem(_placeholderItem);
  attributionLabel->setWordWrap(true);
  attributionLabel->setStyleSheet("color: black; background-color: rgba(220, 220, 220, 180)");
  // reduce font size
  QFont font(attributionLabel->font());
  auto ps = font.pixelSize();
  if (ps != -1)
    font.setPixelSize(ps - 2);
  else {
    auto psf = font.pointSizeF();
    font.setPointSizeF(psf * 0.8);
  }
  attributionLabel->setFont(font);
  // text must be considered as html
  attributionLabel->setTextFormat(Qt::RichText);
  // allow to open attribution links in default browser
  connect(attributionLabel, SIGNAL(linkActivated(const QString &)), this,
          SLOT(openUrlInBrowser(const QString &)));

  // add scale
  scale = new QGraphicsItemGroup();
  scene()->addItem(scale);
  scale->setParentItem(_placeholderItem);
  scale->addToGroup(new QGraphicsLineItem(0, 5, 0, 9));
  scale->addToGroup(new QGraphicsLineItem(0, 10, 20, 10));
  scale->addToGroup(new QGraphicsLineItem(20, 9, 20, 5));
  auto distText = new QGraphicsSimpleTextItem("100 m");
  // display distance with same font as attributionLabel
  font.setWeight(QFont::ExtraLight);
  distText->setFont(font);
  // distText->setPos(25, font.pointSize() - 2);
  scale->addToGroup(distText);
  scale->setEnabled(false);
  recomputeScale();

  // add center
  center = new QGraphicsItemGroup();
  scene()->addItem(center);
  center->setParentItem(_placeholderItem);
  center->addToGroup(new QGraphicsLineItem(5, 0, 5, 10));
  center->addToGroup(new QGraphicsLineItem(0, 5, 10, 5));
  center->setEnabled(false);

  setAcceptDrops(false);
}

void GeographicViewGraphicsView::openUrlInBrowser(const QString &url) {
  QDesktopServices::openUrl(QUrl(url));
}

void GeographicViewGraphicsView::showGeolocationWidget() {
  WorkspacePanel *wp = static_cast<WorkspacePanel *>(parentWidget());
  wp->showConfigurationTab("Geolocation");
}

GeographicViewGraphicsView::~GeographicViewGraphicsView() {
  if (geocodingActive) {
    if (addressSelectionDialog->isVisible()) {
      addressSelectionDialog->accept();
    }

    abortGeocoding = true;

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

void GeographicViewGraphicsView::setGraph(Graph *g) {
  if (graph != g) {

    GlGraphRenderingParameters rp;

    if (graph) {
      rp = glMainWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
    } else {
      rp.setNodesLabelStencil(1);
      rp.setLabelsAreBillboarded(true);
    }

    cleanup();
    graph = g;

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

void GeographicViewGraphicsView::loadFile(const bool isCSV, const QString &fileName) {
  bool oldPolyVisible = false;

  if (polygonEntity != nullptr) {
    oldPolyVisible = polygonEntity->isVisible();
    delete polygonEntity;
  }

  if (fileName.isEmpty())
    polygonEntity = readCsvFile(":/tulip/view/geographic/MAPAGR4.txt");
  else if (isCSV)
    polygonEntity = readCsvFile(fileName);
  else
    polygonEntity = readPolyFile(fileName);

  if (!polygonEntity) {
    QMessageBox::critical(Perspective::instance()->mainWindow()->centralWidget(),
                          "Cannot read .poly file",
                          "We cannot read csv file: " + fileName + "\nCheck the file.");
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
  _geoMW->zoomIn();
}

void GeographicViewGraphicsView::zoomOut() {
  _geoMW->zoomOut();
}

void GeographicViewGraphicsView::currentZoomChanged() {
  auto currentZoom = _geoMW->getCurrentZoom();

  zoomInButton->setEnabled(currentZoom < _geoMW->getMaxZoom());
  zoomOutButton->setEnabled(currentZoom != 0);
  recomputeScale();
}

GlGraphComposite *GeographicViewGraphicsView::getGlGraphComposite() const {
  return glMainWidget->getScene()->getGlGraphComposite();
}

void GeographicViewGraphicsView::createLayoutWithAddresses(const string &addressPropertyName,
                                                           bool createLatAndLngProps,
                                                           bool resetLatAndLngValues,
                                                           bool automaticChoice) {
  nodeLatLng.clear();

  if (graph->existProperty(addressPropertyName)) {
    geocodingActive = true;
    Observable::holdObservers();

    StringProperty *addressProperty = graph->getProperty<StringProperty>(addressPropertyName);

    if (createLatAndLngProps) {
      latProp = graph->getProperty<DoubleProperty>("latitude");
      lngProp = graph->getProperty<DoubleProperty>("longitude");
    }

    int nbNodes = graph->numberOfNodes();
    int nbNodesProcessed = 0;

    pair<double, double> latLng;
    tlp_hash_map<string, pair<double, double>> addressesLatLngMap;

    NominatimGeocoder nominatimGeocoder;

    int failures = 0;

    // hide msg
    noLayoutMsgBox->setVisible(false);
    {
      QProgressDialog progress("Retrieving latitude/longitude for address: ", "Abort", 0, nbNodes,
                               Perspective::instance()->mainWindow());
      progress.setWindowTitle("Retrieving latitude/longitude for address: ");
      progress.setMinimumWidth(400);
      progress.setWindowModality(Qt::WindowModal);
      progress.setValue(0);
      progress.show();
      // ensure progress visibility from the beginning
      // to avoid to block addressSelectionDialog input
      QApplication::processEvents();

      abortGeocoding = false;
      for (auto n : graph->nodes()) {
        progress.setValue(++nbNodesProcessed);
        if (progress.wasCanceled() || abortGeocoding) {
          abortGeocoding = true;
          break;
        }

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
                if (!addressSelectionDialog)
                  addressSelectionDialog =
                      new AddressSelectionDialog(Perspective::instance()->mainWindow());

                addressSelectionDialog->clearList();
                addressSelectionDialog->setBaseAddress(tlpStringToQString(addr));

                for (auto &result : geocodingResults) {
                  addressSelectionDialog->addResultToList(tlpStringToQString(result.address));
                }

                if (addressSelectionDialog->exec())
                  idx = addressSelectionDialog->getPickedResultIdx();
                else {
                  abortGeocoding = true;
                  break;
                }
              }
            } else if (geocodingResults.empty()) {
              qWarning() << "No geolocation found for" << tlpStringToQString(addrValue);
              failures++;
              continue;
            }

            const pair<double, double> &latLng = geocodingResults[idx].latLng;
            nodeLatLng[n] = latLng;

            if (createLatAndLngProps) {
              latProp->setNodeValue(n, latLng.first);
              lngProp->setNodeValue(n, latLng.second);
            }

            if (automaticChoice || addressSelectionDialog->rememberAddressChoice())
              addressesLatLngMap[addr] = latLng;
          }
        }
      }
    }
    if (abortGeocoding)
      QMessageBox::warning(Perspective::instance()->mainWindow(), "Geolocation abortion",
                           QString("Geolocation by address aborted by user."), QMessageBox::Ok);
    else if (failures) {
      QString msg = QString("%1 %2 have not been geolocated.\nDo you want to see %3?")
                        .arg(failures > 1 ? QString::number(failures) : QString("One"))
                        .arg(failures > 1 ? "addresses" : "address")
                        .arg(failures > 1 ? "them" : "it");

      if (QMessageBox::warning(Perspective::instance()->mainWindow(), "Geolocation failed", msg,
                               QMessageBox::Yes | QMessageBox::No,
                               QMessageBox::Yes) == QMessageBox::Yes)
        Perspective::showLogMessages();
    }
    Observable::unholdObservers();
    geocodingActive = false;

  } else
    // show msg
    noLayoutMsgBox->setVisible(true);
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
  // resize scene stuff
  scene()->setSceneRect(QRect(QPoint(0, 0), size()));
  _geoMW->resize(width(), height());
  glWidgetItem->resize(width(), height());
  resizeAttributionLabel();

  // update scene items position
  if (noLayoutMsgBox)
    noLayoutMsgBox->move(width() / 2 - noLayoutMsgBox->width() / 2,
                         height() / 2 - noLayoutMsgBox->height() / 2);
  center->setPos((width() - 10) / 2, (height() - 10) / 2);
  scale->setPos(3, height() - 12);

  scene()->update();
}

void GeographicViewGraphicsView::recomputeScale() {
  static std::vector<double> distances{5000000, 2000000, 1000000, 1000000, 1000000, 100000, 100000,
                                       50000,   50000,   10000,   10000,   10000,   1000,   1000,
                                       500,     200,     100,     50,      25,      25,     10};

  // we assume that currentZoom < distances.size()
  auto currentZoom = _geoMW->getCurrentZoom();
  auto currentDistance = distances[currentZoom];
  double line = currentDistance / pow(2.0, 18 - currentZoom) / 0.597164;

  // configure scale
  // horizontal line
  QGraphicsLineItem *lineItem = qgraphicsitem_cast<QGraphicsLineItem *>(scale->childItems()[1]);
  lineItem->setLine(0, 10, line, 10);
  // right vertical line
  lineItem = qgraphicsitem_cast<QGraphicsLineItem *>(scale->childItems()[2]);
  lineItem->setLine(line, 9, line, 5);
  // distance text
  QGraphicsSimpleTextItem *textItem =
      qgraphicsitem_cast<QGraphicsSimpleTextItem *>(scale->childItems()[3]);
  QString distance;
  if (currentDistance >= 1000)
    distance = QString::number(currentDistance / 1000) + " km";
  else
    distance = QString::number(currentDistance) + " m";
  textItem->setText(distance);
  QFontMetrics fm(textItem->font());
  auto br = fm.boundingRect(distance);
  double dy = 12 - br.height();
  /*auto dy = (attributionLabel->devicePixelRatioF() != 1.0)
    ? textItem->font().pointSizeF()/attributionLabel->devicePixelRatioF()/2
    : 2. - textItem->font().pointSize()/2;*/
  textItem->setPos(line + 5, dy);

  // record scale width
  scaleWidth = line + br.width() + 10;
}

void GeographicViewGraphicsView::resizeAttributionLabel() {
  // remove html infos before computing
  // the bounding rect
  auto label = attributionLabel->text();
  label.replace("&copy;", QChar(169));   // 169 => ©
  label.replace("&mdash;", QChar(8212)); // 8212 => —
  int pos = label.indexOf('<');
  while (pos != -1) {
    int endPos = label.indexOf('>', pos);
    if (endPos != -1) {
      label.remove(pos, endPos + 1 - pos);
      pos = label.indexOf('<', pos);
    } else
      break;
  }

  QFontMetrics fm(attributionLabel->font());
  auto br = fm.boundingRect(label);
  // workaround for display of reduced size font
  br.setWidth(br.width() + 5);

  // if needed enlarge display rect of attributionLabel
  // to ensure attribution text visibility
  auto sz = size();
  int margin = 3;
  auto maxWidth = sz.width() - (displayScale ? scaleWidth : 0);
  if (br.width() > maxWidth) {
    // multiline display
    int nl = br.width() / maxWidth + 1;
    br.setSize(
        QSize(maxWidth, (br.height() * br.width()) / maxWidth + br.height() + (nl - 1) * margin));
  }

  br.moveTopLeft(QPoint(sz.width() - br.width(), sz.height() - br.height()));
  attributionLabel->setGeometry(br);
}

void GeographicViewGraphicsView::refreshMap() {
  if (!_geoMW->isVisible() || !_geoMW->mapLoaded() || !glMainWidget->getScene()->getLayer("Main")) {
    return;
  }

  if (geoLayoutComputed) {
    GlOffscreenRenderer::getInstance()->makeOpenGLContextCurrent();
    auto sw = _geoMW->getMapSouthWest();
    auto ne = _geoMW->getMapNorthEast();
    if (sw.second != ne.second) {
      BoundingBox bb(Coord(GLSCENE_LNG(sw), GLSCENE_LAT(sw), 0),
                     Coord(GLSCENE_LNG(ne), GLSCENE_LAT(ne), 0));

      // A first centering is needed to ensure
      // the success of the first zoom and pan animation
      // If not, nothing may be displayed and an assertion may failed
      // in debug mode
      Camera &camera = glMainWidget->getScene()->getGraphCamera();
      if (camera.getCenter() == Coord())
        camera.setCenter(bb.center());
      GlSceneZoomAndPan sceneZoomAndPan(glMainWidget->getScene(), bb, "Main", 1);
      sceneZoomAndPan.zoomAndPanAnimationStep(1);
    }
  }

  updateMapTexture();
  glWidgetItem->setRedrawNeeded(true);
  scene()->update();
  // ensure removal of overrideCursor set in GeoMapWidget::draw
  QGuiApplication::restoreOverrideCursor();
}

void GeographicViewGraphicsView::centerView() {
  if (_geoMW->isVisible()) {
    _geoMW->setMapBounds(graph, nodeLatLng);
  } else {
    glMainWidget->centerScene();
  }
}

void GeographicViewGraphicsView::centerMapOnNode(const node n) {
  if (nodeLatLng.find(n) != nodeLatLng.end()) {
    _geoMW->setMapCenter(nodeLatLng[n].first, nodeLatLng[n].second);
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

void setChildItemsPen(QGraphicsItemGroup *group, bool darkBackground) {
  QPen pen(QColor(darkBackground ? Qt::white : Qt::black));
  for (auto item : group->childItems()) {
    switch (item->type()) {
    case QGraphicsLineItem::Type:
      qgraphicsitem_cast<QGraphicsLineItem *>(item)->setPen(pen);
      break;
    case QGraphicsSimpleTextItem::Type:
      qgraphicsitem_cast<QGraphicsSimpleTextItem *>(item)->setPen(pen);
    default:
      break;
    }
  }
}

void GeographicViewGraphicsView::switchMapType() {
  GeographicView::MapType mapType = _geoView->mapType();

  bool enableGeoMap = false;
  bool enablePolygon = false;
  bool enablePlanisphere = false;
  QString attribution;

  switch (mapType) {

  case GeographicView::CustomTileLayer: {
    enableGeoMap = true;
    QString url = _geoView->getConfigWidget()->getCustomTileLayerUrl();
    attribution = _geoView->getConfigWidget()->getCustomTilesAttribution();
    // if attribution is empty or fill with white spaces
    // set it to url
    if (attribution.simplified().isEmpty())
      attribution = url;
    _geoMW->switchToCustomTileLayer(url, attribution);
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
    enableGeoMap = true;
    _geoMW->switchToMapLayer(mapType);
    const MapLayer &mapLayer = _geoView->getMapLayers()[mapType];
    attribution = mapLayer.attrib;
  }

  if (enableGeoMap) {
    // setup attribution label
    auto label = attribution;
    // set color and suppress underline of links
    label.replace("<a", "<a style=\"color: rgb(0, 85, 255); text-decoration: none;\"");
    attributionLabel->setText(label);
    resizeAttributionLabel();
    bool darkBackground = _geoView->getMapLayers()[mapType].dark;
    // configure center and scale
    center->setVisible(displayCenter);
    setChildItemsPen(center, darkBackground);
    scale->setVisible(displayScale);
    setChildItemsPen(scale, darkBackground);
  } else {
    // hide center and scale
    center->setVisible(false);
    scale->setVisible(false);
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

  _geoMW->setVisible(enableGeoMap);
  backgroundLayer->setVisible(enableGeoMap);

  if (polygonEntity)
    polygonEntity->setVisible(enablePolygon);

  layer->setCamera(new Camera(glMainWidget->getScene()));

  if (mapType != GeographicView::Globe && geoLayoutComputed) {
    geoLayout->removeListener(this);
    SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");

    for (auto n : graph->nodes()) {
      if (viewSize != geoViewSize) {
        const Size &nodeSize = viewSize->getNodeValue(n);
        geoViewSize->setNodeValue(n, nodeSize);
      }

      if (nodeLatLng.find(n) != nodeLatLng.end()) {
        geoLayout->setNodeValue(n, Coord(GLSCENE_LNG(nodeLatLng[n]), GLSCENE_LAT(nodeLatLng[n])));
      }
    }

    if (!edgeBendsLatLng.empty()) {
      for (auto e : graph->edges()) {
        auto &eb = edgeBendsLatLng[e];
        vector<Coord> edgeBendsCoords;
        edgeBendsCoords.reserve(eb.size());
        for (unsigned int i = 0; i < eb.size(); ++i) {
          edgeBendsCoords.emplace_back(GLSCENE_LNG(eb[i]), GLSCENE_LAT(eb[i]));
        }

        geoLayout->setEdgeValue(e, edgeBendsCoords);
      }
    }
    geoLayout->addListener(this);

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

      geoLayout->addListener(this);
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
      geoLayout->addListener(this);
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
      mapType == GeographicView::Globe);

  Observable::unholdObservers();

  graph->popIfNoUpdates();

  draw();
}

void GeographicViewGraphicsView::setGeoLayoutComputed() {
  geoLayoutComputed = true;
  noLayoutMsgBox->setVisible(false);
  glMainWidget->getScene()->getGlGraphComposite()->setVisible(true);
}

void GeographicViewGraphicsView::showScale(bool show) {
  scale->setVisible(displayScale = show);
  resizeAttributionLabel();
}

void GeographicViewGraphicsView::showCenter(bool show) {
  center->setVisible(displayCenter = show);
}

void GeographicViewGraphicsView::updateMapTexture() {
  int width = _geoMW->geometry().width();
  int height = _geoMW->geometry().height();

  QImage image(width, height, QImage::Format_RGB32);
  QPainter painter(&image);
  _geoMW->render(&painter);
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
