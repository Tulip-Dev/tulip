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

#include "GeographicViewGraphicsView.h"
#include "GeographicView.h"

#include <tulip/GlCPULODCalculator.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlSphere.h>
#include <tulip/GlLine.h>
#include <tulip/GlSceneZoomAndPan.h>
#include <tulip/GlyphManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/TlpQtTools.h>

#include <QPushButton>
#include <QTextStream>
#include <QTimeLine>
#include <QFile>
#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

using namespace std;

namespace tlp {

GlComposite *readPolyFile(QString fileName) {
  GlComposite *composite=new GlComposite;

  QFile file(fileName);

  if(!file.open(QIODevice::ReadOnly)) {
    return NULL;
  }

  string polygonName="";
  vector<vector<Coord> > datas;
  vector<Coord> currentVector;
  bool ok;
  QString line;

  while(!file.atEnd()) {
    line=file.readLine();

    if(line=="" || line=="\n")
      continue;

    line.toUInt(&ok);

    if(ok) {
      if(!currentVector.empty())
        datas.push_back(currentVector);

      currentVector=vector<Coord>();
      continue;
    }

    if(line=="END\n")
      continue;

    QStringList strList=line.split(" ");

    bool findLng=false;
    bool findLat=false;
    float lng;
    float lat;

    for(QStringList::iterator it=strList.begin(); it!=strList.end(); ++it) {
      (*it).toDouble(&ok);

      if(ok) {
        if(!findLng) {
          findLng=true;
          lng=(*it).toDouble();
        }
        else {
          findLat=true;
          lat=(*it).toDouble();
        }
      }
    }

    if(!findLat) {

      if(polygonName!="") {

        if(!currentVector.empty())
          datas.push_back(currentVector);

        if(!datas.empty()) {

          composite->addGlEntity(new GlComplexPolygon(datas,Color(0,0,0,50),Color(0,0,0,255)),polygonName);
          datas.clear();
          currentVector.clear();
        }
      }

      polygonName=QStringToTlpString(line);
      continue;
    }

    if(lat==90.)
      lat=89.999f;

    double mercatorLatitude=lat*2./360.*M_PI;
    mercatorLatitude=sin(abs(mercatorLatitude));
    mercatorLatitude=log((1.+mercatorLatitude)/(1. - mercatorLatitude))/2.;

    if(lat<0)
      mercatorLatitude=0.-mercatorLatitude;

    if(mercatorLatitude*360./M_PI < -360)
      mercatorLatitude=-M_PI;

    currentVector.push_back(Coord(lng*2.,mercatorLatitude*360./M_PI,0));
  }

  if(polygonName!="") {
    if(!currentVector.empty())
      datas.push_back(currentVector);

    composite->addGlEntity(new GlComplexPolygon(datas,Color(0,0,0,50),Color(0,0,0,255)),polygonName);
  }

  return composite;
}

GlComposite *readCsvFile(QString fileName) {

  GlComposite *composite=new GlComposite;

  QFile file(fileName);

  if(!file.open(QIODevice::ReadOnly))
    return NULL;

  vector<vector<Coord> > datas;
  vector<Coord> currentVector;
  int lastIndex=0;

  while(!file.atEnd()) {
    QString line(file.readLine());
    QStringList strList=line.split("\t");

    if(strList.size()!=3) {
      if(!currentVector.empty())
        datas.push_back(currentVector);

      currentVector=vector<Coord>();
      continue;
    }

    if(strList[0].toInt()!=lastIndex) {
      if(!currentVector.empty())
        datas.push_back(currentVector);

      lastIndex=strList[0].toInt();
      currentVector=vector<Coord>();
    }

    double mercatorLatitude=strList[1].toDouble();
    mercatorLatitude=sin(abs(mercatorLatitude));
    mercatorLatitude=log((1.+mercatorLatitude)/(1. - mercatorLatitude))/2.;

    if(strList[1].toDouble()<0)
      mercatorLatitude=0.-mercatorLatitude;

    currentVector.push_back(Coord((strList[2].toDouble())*360./M_PI,mercatorLatitude*360./M_PI,0));
  }

  if(datas.empty())
    return NULL;

  composite->addGlEntity(new GlComplexPolygon(datas,Color(0,0,0,50),Color(0,0,0,255)),"polygon");

  return composite;
}

void simplifyPolyFile(QString fileName,float definition) {

  map<string,vector<vector<Coord> > > clearPolygons;

  QFile file(fileName);

  if(!file.open(QIODevice::ReadOnly)) {
    return;
  }

  string polygonName="";
  vector<vector<Coord> > datas;
  vector<Coord> currentVector;
  bool ok;

  while(!file.atEnd()) {
    QString line(file.readLine());

    if(line=="" || line=="\n")
      continue;

    line.toUInt(&ok);

    if(ok) {
      if(!currentVector.empty())
        datas.push_back(currentVector);

      currentVector=vector<Coord>();
      continue;
    }

    if(line=="END\n")
      continue;

    QStringList strList=line.split(" ");

    bool findLng=false;
    bool findLat=false;
    float lng;
    float lat;

    for(QStringList::iterator it=strList.begin(); it!=strList.end(); ++it) {
      (*it).toDouble(&ok);

      if(ok) {
        if(!findLng) {
          findLng=true;
          lng=(*it).toDouble();
        }
        else {
          findLat=true;
          lat=(*it).toDouble();
        }
      }
    }

    if(!findLat) {

      if(polygonName!="") {

        if(!currentVector.empty())
          datas.push_back(currentVector);

        if(!datas.empty()) {

          clearPolygons[polygonName]=datas;
          datas.clear();
          currentVector.clear();
        }
      }

      polygonName=QStringToTlpString(line);
      continue;
    }

    double mercatorLatitude=lat*2./360.*M_PI;
    mercatorLatitude=sin(abs(mercatorLatitude));
    mercatorLatitude=log((1.+mercatorLatitude)/(1. - mercatorLatitude))/2.;

    if(lat<0)
      mercatorLatitude=0.-mercatorLatitude;

    currentVector.push_back(Coord(lng,lat,0));
  }

  if(polygonName!="") {
    if(!currentVector.empty())
      datas.push_back(currentVector);

    clearPolygons[polygonName]=datas;
  }

  map<Coord,Coord> simplifiedCoord;

  QString newName(fileName);
  newName.replace(".poly",QString("_")+QString::number(definition)+".poly");
  cout << "create : " << QStringToTlpString(newName) << endl;
  QFile fileW(newName);

  if(!fileW.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return;
  }

  QTextStream out(&fileW);

  Coord *lastCoord=NULL;

  for(map<string,vector<vector<Coord> > >::iterator it1=clearPolygons.begin(); it1!=clearPolygons.end(); ++it1) {
    out << (*it1).first.c_str();

    unsigned int i=1;

    for(vector<vector<Coord> >::iterator it2=(*it1).second.begin(); it2!=(*it1).second.end(); ++it2) {
      out << i << "\n";

      for(vector<Coord>::iterator it3=(*it2).begin(); it3!=(*it2).end(); ++it3) {
        if(lastCoord==NULL) {
          out << (*it3)[0] << " " << (*it3)[1] << "\n";
          lastCoord=&(*it3);
        }
        else {
          if((*lastCoord).dist(*it3)>definition) {
            if(simplifiedCoord.count(*it3)==0) {
              out << (*it3)[0] << " " << (*it3)[1] << "\n";
              lastCoord=&(*it3);
            }
            else {
              lastCoord = &simplifiedCoord[*it3];
              out << (*lastCoord)[0] << " " << (*lastCoord)[1] << "\n";

            }
          }
          else {
            if(simplifiedCoord.count(*it3)==0)
              simplifiedCoord[*it3]=*lastCoord;
          }
        }
      }

      out << "END\n";

      ++i;
    }
  }
}

double latitudeToMercator(double latitude) {
  double mercatorLatitude=latitude*M_PI/360.;
  mercatorLatitude=sin(abs(mercatorLatitude));
  mercatorLatitude=log((1.+mercatorLatitude)/(1. - mercatorLatitude))/2.;

  if(latitude<0)
    return -mercatorLatitude/M_PI*360.;

  return mercatorLatitude/M_PI*360.;
}

double mercatorToLatitude(double mercator) {
  return atan(sinh(mercator/360.*M_PI)) /M_PI*360.;
}


QGraphicsProxyWidget *proxyGM = NULL;

unsigned int GeographicViewGraphicsView::planisphereTextureId = 0;

GeographicViewGraphicsView::GeographicViewGraphicsView(GeographicView *geoView, QGraphicsScene *graphicsScene, QWidget *parent) :
  QGraphicsView(graphicsScene, parent),
  _geoView(geoView), graph(NULL), googleMaps(NULL),
  currentMapZoom(0),globeCameraBackup(NULL,true),mapCameraBackup(NULL,true),geoLayout(NULL),
  geoViewSize(NULL), geoViewShape(NULL), geoLayoutBackup(NULL),
  mapTranslationBlocked(false), geocodingActive(false), cancelGeocoding(false),
  polygonEntity(NULL), planisphereEntity(NULL), noLayoutMsgBox(NULL), firstGlobeSwitch(true), firstMapSwitch(true), geoLayoutComputed(false) {
  setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
  glWidget = new GlMainWidget();
  setViewport(glWidget);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  googleMaps = new GoogleMaps();
// workaround to get rid of Qt5 warning messages : "QMacCGContext:: Unsupported painter devtype type 1"
// see https://bugreports.qt.io/browse/QTBUG-32639
#if defined(__APPLE__) && QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
  googleMaps->setWindowOpacity(0.99);
#endif
  googleMaps->setMouseTracking(false);
  googleMaps->resize(512,512);
  progressWidget = new ProgressWidgetGraphicsProxy();
  progressWidget->hide();
  progressWidget->setZValue(2);
  addressSelectionDialog = new AddressSelectionDialog(googleMaps);
// workaround to get rid of Qt5 warning messages : "QMacCGContext:: Unsupported painter devtype type 1"
// see https://bugreports.qt.io/browse/QTBUG-32639
#if defined(__APPLE__) && QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
  addressSelectionDialog->setWindowOpacity(0.99);
#endif
  scene()->addItem(progressWidget);
  addresseSelectionProxy = scene()->addWidget(addressSelectionDialog, Qt::Dialog);
  addresseSelectionProxy->hide();
  addresseSelectionProxy->setZValue(3);

  googleMaps->setProgressWidget(progressWidget);
  googleMaps->setAdresseSelectionDialog(addressSelectionDialog,addresseSelectionProxy);

  connect(googleMaps, SIGNAL(currentZoomChanged()), _geoView, SLOT(currentZoomChanged()));
  connect(googleMaps, SIGNAL(refreshMap()), this, SLOT(queueMapRefresh()));

  _placeholderItem = new QGraphicsRectItem(0,0,1,1);
  _placeholderItem->setBrush(Qt::transparent);
  _placeholderItem->setPen(QPen(Qt::transparent));
  scene()->addItem(_placeholderItem);

  QGraphicsProxyWidget *proxyGM = scene()->addWidget(googleMaps);
  proxyGM->setPos(0,0);
  proxyGM->setParentItem(_placeholderItem);

  glMainWidget = new GlMainWidget(0, geoView);
  glMainWidget->getScene()->setCalculator(new GlCPULODCalculator());
  glMainWidget->getScene()->setBackgroundColor(Color(255,255,255,0));
  glMainWidget->getScene()->setClearBufferAtDraw(false);

  glWidgetItem = new GlMainWidgetGraphicsItem(glMainWidget, 512, 512);
  glWidgetItem->setPos(0,0);

  // disable user input
  // before allowing some display feedback
  tlp::disableQtUserInput();

  while (!googleMaps->pageInit()) {
    QApplication::processEvents();
  }

  // reenable user input
  tlp::enableQtUserInput();

  scene()->addItem(glWidgetItem);
  glWidgetItem->setParentItem(_placeholderItem);

  // combo box to choose the map type
  viewTypeComboBox=new QComboBox;
// workaround to get rid of Qt5 warning messages : "QMacCGContext:: Unsupported painter devtype type 1"
// see https://bugreports.qt.io/browse/QTBUG-32639
#if defined(__APPLE__) && QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
  viewTypeComboBox->setWindowOpacity(0.99);
#endif
  viewTypeComboBox->addItems(QStringList()<<"RoadMap"<<"RoadMap"<<"Satellite" << "Terrain" << "Hybrid" <<"Polygon" << "Globe");
  viewTypeComboBox->insertSeparator(1);

  QGraphicsProxyWidget *comboBoxProxy = scene()->addWidget(viewTypeComboBox);
  comboBoxProxy->setParentItem(_placeholderItem);
  comboBoxProxy->setPos(20,20);
  comboBoxProxy->setZValue(1);

  connect(viewTypeComboBox,SIGNAL(currentIndexChanged(QString)),_geoView,SLOT(viewTypeChanged(QString)));

  // 2 push buttons
  // zoom +
  zoomInButton = new QPushButton(QIcon(":/zoom+.png"), "");
// workaround to get rid of Qt5 warning messages : "QMacCGContext:: Unsupported painter devtype type 1"
// see https://bugreports.qt.io/browse/QTBUG-32639
#if defined(__APPLE__) && QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
  zoomInButton->setWindowOpacity(0.99);
#endif
  zoomInButton->setFixedSize(29, 27);
  zoomInButton->setContentsMargins(0, 0, 0, 0);
  connect(zoomInButton, SIGNAL(pressed()), _geoView, SLOT(zoomIn()));
  QGraphicsProxyWidget *buttonProxy = scene()->addWidget(zoomInButton);
  buttonProxy->setParentItem(_placeholderItem);
  buttonProxy->setPos(20, 50);

  // zoom -
  zoomOutButton = new QPushButton(QIcon(":/zoom-.png"), "");
// workaround to get rid of Qt5 warning messages : "QMacCGContext:: Unsupported painter devtype type 1"
// see https://bugreports.qt.io/browse/QTBUG-32639
#if defined(__APPLE__) && QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
  zoomOutButton->setWindowOpacity(0.99);
#endif
  zoomOutButton->setFixedSize(29, 27);
  zoomOutButton->setContentsMargins(0, 0, 0, 0);
  connect(zoomOutButton, SIGNAL(pressed()), _geoView, SLOT(zoomOut()));
  buttonProxy = scene()->addWidget(zoomOutButton);
  buttonProxy->setParentItem(_placeholderItem);
  buttonProxy->setPos(20, 76);


  QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, "Geolocated layout not initialized",
                                        "Warning : the geolocated layout\n"
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
  delete glMainWidget;
}

void GeographicViewGraphicsView::cleanup() {
  if(graph) {

    GlScene *scene=glMainWidget->getScene();
    scene->clearLayersList();

    if(geoLayout!=graph->getProperty<LayoutProperty>("viewLayout"))
      delete geoLayout;

    if(geoViewSize!=graph->getProperty<SizeProperty>("viewSize"))
      delete geoViewSize;

    if(geoViewShape!=graph->getProperty<IntegerProperty>("viewShape"))
      delete geoViewShape;

    // those entities have been deleted by the prior call to GlScene::clearLayersList,
    // so reset the pointers to NULL value
    polygonEntity = NULL;
    planisphereEntity = NULL;

  }
}

void GeographicViewGraphicsView::setGraph(Graph *graph) {
  if (this->graph != graph) {

    GlGraphRenderingParameters rp;

    if (this->graph) {
      rp = glMainWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
    }
    else {
      rp.setNodesLabelStencil(1);
      rp.setLabelsAreBillboarded(true);
    }

    cleanup();
    this->graph = graph;

    GlScene *scene=glMainWidget->getScene();
    GlGraphComposite *graphComposite=new GlGraphComposite(graph);
    graphComposite->setVisible(false);
    graphComposite->setRenderingParameters(rp);
    GlLayer *layer=scene->createLayer("Main");

    layer->addGlEntity(graphComposite,"graph");

    geoLayout = graph->getProperty<LayoutProperty>("viewLayout");
    geoViewSize = graph->getProperty<SizeProperty>("viewSize");
    geoViewShape = graph->getProperty<IntegerProperty>("viewShape");
    currentMapZoom = 0;
    polygonEntity=NULL;

    draw();
  }
}

static string removeQuotesIfAny(const string &s) {
  if (s[0] == '"' && s[s.length() - 1] == '"') {
    return s.substr(1, s.length() - 2);
  }
  else {
    return s;
  }
}

void GeographicViewGraphicsView::loadDefaultMap() {
  bool oldPolyVisible=false;

  if(polygonEntity!=NULL) {
    oldPolyVisible=polygonEntity->isVisible();
    delete polygonEntity;
  }

  polygonEntity = readCsvFile(":/MAPAGR4.txt");
  polygonEntity->setVisible(oldPolyVisible);

  GlScene *scene=glMainWidget->getScene();
  GlLayer *layer=scene->getLayer("Main");
  layer->addGlEntity(polygonEntity,"polygonMap");
}

void GeographicViewGraphicsView::loadCsvFile(QString fileName) {
  bool oldPolyVisible=false;

  if(polygonEntity!=NULL) {
    oldPolyVisible=polygonEntity->isVisible();
    delete polygonEntity;
  }

  polygonEntity = readCsvFile(fileName);

  if(!polygonEntity) {
    QMessageBox::critical(NULL,"Can't read .poly file","We can't read csv file : "+fileName+"\nVerify the file.");
    return;
  }

  polygonEntity->setVisible(oldPolyVisible);

  GlScene *scene=glMainWidget->getScene();
  GlLayer *layer=scene->getLayer("Main");
  layer->addGlEntity(polygonEntity,"polygonMap");
}

void GeographicViewGraphicsView::loadPolyFile(QString fileName) {
  bool oldPolyVisible=false;

  if(polygonEntity!=NULL) {
    oldPolyVisible=polygonEntity->isVisible();
    delete polygonEntity;
  }

  polygonEntity = readPolyFile(fileName);

  //simplifyPolyFile(fileName,0.025);
  //simplifyPolyFile(fileName,0.05);
  if(!polygonEntity) {
    QMessageBox::critical(NULL,"Can't read .poly file","We can't read .poly file : "+fileName+"\nVerify the file.");
    return;
  }

  polygonEntity->setVisible(oldPolyVisible);

  GlScene *scene=glMainWidget->getScene();
  GlLayer *layer=scene->getLayer("Main");
  layer->addGlEntity(polygonEntity,"polygonMap");
}

void GeographicViewGraphicsView::mapToPolygon() {
  GlComposite *composite=polygonEntity;

  if(!composite)
    return;

  const map<string, GlSimpleEntity*> entities=composite->getGlEntities();

  Iterator<node> *nodesIt = graph->getNodes();
  node n;

  while (nodesIt->hasNext() ) {
    n = nodesIt->next();

    Coord nodePos=geoLayout->getNodeValue(n);

    for(map<string,GlSimpleEntity*>::const_iterator it=entities.begin(); it!=entities.end(); ++it) {
      if((*it).second->getBoundingBox().contains(nodePos)) {
        GlComplexPolygon *polygon=static_cast<GlComplexPolygon*>((*it).second);

        const vector<vector<Coord> > polygonSides=polygon->getPolygonSides();

        for(vector<vector<Coord> >::const_iterator it2=polygonSides.begin(); it2!=polygonSides.end(); ++it2) {
          vector<Coord> polygonSide=(*it2);
          bool oddNodes=false;
          Coord lastCoord=polygonSide[0];

          for (vector<Coord>::const_iterator it=(++polygonSide.begin()); it!=polygonSide.end(); ++it) {
            if ((((*it)[1]< nodePos[1] && lastCoord[1]>=nodePos[1])
                 ||   (lastCoord[1]< nodePos[1] && (*it)[1]>=nodePos[1]))
                &&  ((*it)[0]<=nodePos[0] || lastCoord[0]<=nodePos[0])) {
              oddNodes^=((*it)[0]+(nodePos[1]-(*it)[1])/(lastCoord[1]-(*it)[1])*(lastCoord[0]-(*it)[0])<nodePos[0]);
            }

            lastCoord=(*it);
          }

          if(oddNodes) {

            BoundingBox bb;

            for(vector<Coord>::const_iterator it3=polygonSides[0].begin(); it3!=polygonSides[0].end(); ++it3) {
              bb.expand(*it3);
            }

            geoLayout->setNodeValue(n,bb.center());
            polygon->setFillColor(graph->getProperty<ColorProperty>("viewColor")->getNodeValue(n));
            polygon->setOutlineColor(graph->getProperty<ColorProperty>("viewBorderColor")->getNodeValue(n));
            break;
          }
        }
      }
    }
  }

  delete nodesIt;
}

void GeographicViewGraphicsView::zoomIn() {
  googleMaps->setCurrentZoom(googleMaps->getCurrentMapZoom()+1);
}

void GeographicViewGraphicsView::zoomOut() {
  googleMaps->setCurrentZoom(googleMaps->getCurrentMapZoom()-1);
}

void GeographicViewGraphicsView::currentZoomChanged() {
  zoomInButton->setEnabled(googleMaps->getCurrentMapZoom() != 20);
  zoomOutButton->setEnabled(googleMaps->getCurrentMapZoom() != 0);
}

GlGraphComposite *GeographicViewGraphicsView::getGlGraphComposite() const {
  return glMainWidget->getScene()->getGlGraphComposite();

}

void GeographicViewGraphicsView::createLayoutWithAddresses(const string& addressPropertyName, bool createLatAndLngProps) {
  geocodingActive = true;
  nodeLatLng.clear();
  Observable::holdObservers();

  if (graph->existProperty(addressPropertyName)) {
    StringProperty *addressProperty = graph->getProperty<StringProperty>(addressPropertyName);
    DoubleProperty *latitudeProperty = NULL;
    DoubleProperty *longitudeProperty = NULL;

    if (createLatAndLngProps) {
      latitudeProperty = graph->getProperty<DoubleProperty>("latitude");
      longitudeProperty = graph->getProperty<DoubleProperty>("longitude");
    }

    int nbNodes = graph->numberOfNodes();
    int nbNodesProcessed = 0;
    progressWidget->setFrameColor(Qt::green);
    progressWidget->setProgress(nbNodesProcessed, nbNodes);
    progressWidget->setPos(width() / 2 - progressWidget->sceneBoundingRect().width() / 2, height() / 2 - progressWidget->sceneBoundingRect().height() / 2);
    progressWidget->show();

    pair<double, double> latLng;
    vector<pair<node, string> >multipleResultsAddresses;
    map<string, pair<double, double> > addressesLatLngMap;

    Iterator<node> *nodesIt = graph->getNodes();
    node n;
    bool grabNextNode = true;

    // disable user input
    // before allowing some display feedback
    tlp::disableQtUserInput();

    while (nodesIt->hasNext() && !progressWidget->cancelRequested() && !cancelGeocoding) {
      if (grabNextNode) {
        n = nodesIt->next();
        progressWidget->setProgress(++nbNodesProcessed, nbNodes);
      }

      grabNextNode = true;
      string addr = removeQuotesIfAny(addressProperty->getNodeValue(n));

      if (addr == "") continue;

      progressWidget->setComment("Retrieving latitude and longitude for address : \n" + tlpStringToQString(addr));

      if (nodeLatLng.find(n) == nodeLatLng.end()) {

        if (addressesLatLngMap.find(addr) != addressesLatLngMap.end()) {
          nodeLatLng[n] = addressesLatLngMap[addr];

          if (createLatAndLngProps) {
            latitudeProperty->setNodeValue(n, nodeLatLng[n].first);
            longitudeProperty->setNodeValue(n, nodeLatLng[n].second);
          }
        }
        else {
          string geocodingRequestStatus = googleMaps->getLatLngForAddress(tlpStringToQString(addr), latLng, true);

          if (geocodingRequestStatus == "OK") {
            nodeLatLng[n] = latLng;
            addressesLatLngMap[addr] = latLng;

            if (createLatAndLngProps) {
              latitudeProperty->setNodeValue(n, latLng.first);
              longitudeProperty->setNodeValue(n, latLng.second);
            }
          }

          else if (geocodingRequestStatus == "MULTIPLE_RESULTS") {
            multipleResultsAddresses.push_back(make_pair(n, addr));
          }
          else if (geocodingRequestStatus != "ZERO_RESULTS") {
            // the number of geocoding requests to the google servers in a short period of time is limited
            // So wait 3,5 seconds before sending a new request to avoid errors
            progressWidget->setFrameColor(Qt::red);
            progressWidget->setComment("Geocoding requests limit reached. \n Waiting 3,5 seconds ...");
            draw();
            QTimeLine timeLine(3500);
            timeLine.start();

            // disable user input
            // before allowing some display feedback
            tlp::disableQtUserInput();

            while (timeLine.state() != QTimeLine::NotRunning) {
              QApplication::processEvents();
            }

            // reenable user input
            tlp::enableQtUserInput();

            progressWidget->setFrameColor(Qt::green);
            grabNextNode = false;
          }
          else {
            progressWidget->hide();
            QMessageBox::warning(NULL, "Geolocation failed", "No results were found for address : \n" + tlpStringToQString(addr));
            progressWidget->show();
          }

        }

        QApplication::processEvents();
      }
    }

    // reenable user input
    tlp::enableQtUserInput();

    delete nodesIt;
    progressWidget->hide();

    for (unsigned int i = 0 ; i < multipleResultsAddresses.size() ; ++i) {
      string addr = multipleResultsAddresses[i].second;
      n = multipleResultsAddresses[i].first;

      if (addressesLatLngMap.find(addr) != addressesLatLngMap.end()) {
        latLng = addressesLatLngMap[addr];
        nodeLatLng[n] = latLng;

        if (createLatAndLngProps) {
          latitudeProperty->setNodeValue(n, latLng.first);
          longitudeProperty->setNodeValue(n, latLng.second);
        }
      }
      else {
        string geocodingRequestStatus = googleMaps->getLatLngForAddress(tlpStringToQString(addr), latLng);

        if (geocodingRequestStatus != "OK") {
          QTimeLine timeLine(3500);
          timeLine.start();

          // disable user input
          // before allowing some display feedback
          tlp::disableQtUserInput();

          while (timeLine.state() != QTimeLine::NotRunning) {
            QApplication::processEvents();
          }

          // reenable user input
          tlp::enableQtUserInput();

          --i;
        }
        else {
          nodeLatLng[n] = latLng;

          if (createLatAndLngProps) {
            latitudeProperty->setNodeValue(n, latLng.first);
            longitudeProperty->setNodeValue(n, latLng.second);
          }

          if (addressSelectionDialog->rememberAddressChoice()) {
            addressesLatLngMap[addr] = latLng;
          }
        }
      }
    }
  }

  Observable::unholdObservers();
  geocodingActive = false;
}

void GeographicViewGraphicsView::createLayoutWithLatLngs(const std::string& latitudePropertyName, const std::string& longitudePropertyName, const string &edgesPathsPropertyName) {
  nodeLatLng.clear();
  pair<double, double> latLng;

  if (graph->existProperty(latitudePropertyName) &&  graph->existProperty(longitudePropertyName)) {
    DoubleProperty *latitudeProperty = graph->getProperty<DoubleProperty>(latitudePropertyName);
    DoubleProperty *longitudeProperty = graph->getProperty<DoubleProperty>(longitudePropertyName);
    node n;
    forEach(n, graph->getNodes()) {
      latLng.first = latitudeProperty->getNodeValue(n);
      latLng.second = longitudeProperty->getNodeValue(n);
      nodeLatLng[n] = latLng;
    }
  }

  if (graph->existProperty(edgesPathsPropertyName)) {
    DoubleVectorProperty *edgesPathsProperty = graph->getProperty<DoubleVectorProperty>(edgesPathsPropertyName);
    edge e;
    forEach(e, graph->getEdges()) {
      const std::vector<double> &edgePath = edgesPathsProperty->getEdgeValue(e);
      std::vector<std::pair<double, double> > latLngs;

      for (size_t i = 0 ; i < edgePath.size() ; i+=2) {
        latLngs.push_back(make_pair(edgePath[i], edgePath[i+1]));
      }

      edgeBendsLatLng[e] = latLngs;
    }
  }

}

void GeographicViewGraphicsView::resizeEvent(QResizeEvent *event) {
  QGraphicsView::resizeEvent(event);
  scene()->setSceneRect(QRect(QPoint(0, 0), size()));
  googleMaps->resize(width(), height());
  glWidgetItem->resize(width(), height());

  if (progressWidget->isVisible()) {
    progressWidget->setPos(width() / 2 - progressWidget->sceneBoundingRect().width() / 2, height() / 2 - progressWidget->sceneBoundingRect().height() / 2);
  }

  if (noLayoutMsgBox && noLayoutMsgBox->isVisible()) {
    noLayoutMsgBox->setPos(width() / 2 - noLayoutMsgBox->sceneBoundingRect().width() / 2, height() / 2 - noLayoutMsgBox->sceneBoundingRect().height() / 2);
  }

  if (addresseSelectionProxy->isVisible()) {
    addresseSelectionProxy->setPos(width() / 2 - addresseSelectionProxy->sceneBoundingRect().width() / 2, height() / 2 - addresseSelectionProxy->sceneBoundingRect().height() / 2);
  }

  if (scene())
    scene()->update();

  // Hack : send a mouse event to force redraw of the scene (otherwise artifacts was displayed when maximizing or minimizing the graphics view)
  QMouseEvent *eventModif = new QMouseEvent(QEvent::MouseMove,QPoint(size().width()/2, size().height()/2), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
  QApplication::sendEvent(this, eventModif);
}

void GeographicViewGraphicsView::paintEvent (QPaintEvent * event) {

  Observable::holdObservers();

  if (graph && !geocodingActive) {

    if(googleMaps->isVisible() && (prevMapCenter != googleMaps->getCurrentMapCenter() || prevMapZoom != googleMaps->getCurrentMapZoom())) {

      prevMapCenter = googleMaps->getCurrentMapCenter();
      prevMapZoom = googleMaps->getCurrentMapZoom();

      float worldWidth = googleMaps->getWorldWidth();
      static const double maxLat = 85.05113f;
      Coord swPos = googleMaps->getPixelPosOnScreenForLatLng(-maxLat, 0.0);
      swPos[1]=height() - swPos[1];
      Coord nePos = googleMaps->getPixelPosOnScreenForLatLng(maxLat, 0.0);
      nePos[1]=height() - nePos[1];

      Coord mapCenterCoord = googleMaps->getPixelPosOnScreenForLatLng(prevMapCenter.first, prevMapCenter.second);
      float westToMapCenterDist = ((prevMapCenter.second + 180.0) * worldWidth) / 360.0;
      swPos.setX(mapCenterCoord.getX() - westToMapCenterDist);
      nePos.setX(swPos.getX() + worldWidth);

      currentMapCenter = prevMapCenter;
      lastSceneRect = sceneRect();
      currentMapZoom = prevMapZoom;

      BoundingBox bb;
      Coord rightCoord = googleMaps->getPixelPosOnScreenForLatLng(180,180);
      Coord leftCoord = googleMaps->getPixelPosOnScreenForLatLng(0,0);

      if (rightCoord[0] - leftCoord[0]) {
        float mapWidth=(width()/(rightCoord - leftCoord)[0])*180.;
        float middleLng=googleMaps->getLatLngForPixelPosOnScreen(width()/2.,height()/2.).second*2.;
        bb.expand(Coord(middleLng-mapWidth/2.,latitudeToMercator(googleMaps->getLatLngForPixelPosOnScreen(0,0).first*2.),0));
        bb.expand(Coord(middleLng+mapWidth/2.,latitudeToMercator(googleMaps->getLatLngForPixelPosOnScreen(width(),height()).first*2.),0));
        GlSceneZoomAndPan sceneZoomAndPan(glMainWidget->getScene(),bb,"Main",1);
        sceneZoomAndPan.zoomAndPanAnimationStep(1);
      }

      glWidgetItem->setRedrawNeeded(true);

    }
  }

  Observable::unholdObservers();

  QGraphicsView::paintEvent(event);
}

void GeographicViewGraphicsView::queueMapRefresh() {
  QTimer::singleShot(500, this, SLOT(refreshMap()));
}

void GeographicViewGraphicsView::refreshMap() {
  glWidgetItem->setRedrawNeeded(true);
  scene()->update();
}

void GeographicViewGraphicsView::setMapTranslationBlocked(const bool translationBlocked) {
  mapTranslationBlocked = translationBlocked;
}

void GeographicViewGraphicsView::centerView() {
  if(googleMaps->isVisible()) {
    googleMaps->setMapBounds(graph, nodeLatLng);
  }
  else {
    glMainWidget->centerScene();
  }
}
void GeographicViewGraphicsView::centerMapOnNode(const node n) {
  if (nodeLatLng.find(n) != nodeLatLng.end()) {
    googleMaps->setMapCenter(nodeLatLng[n].first, nodeLatLng[n].second);
  }
}

void GeographicViewGraphicsView::setGeoLayout(LayoutProperty *property) {
  *property=*geoLayout;
  geoLayout=property;
  glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementLayout(geoLayout);
}

void GeographicViewGraphicsView::setGeoSizes(SizeProperty *property) {
  *property=*geoViewSize;
  geoViewSize=property;
  glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementSize(geoViewSize);
}

void GeographicViewGraphicsView::setGeoShape(IntegerProperty *property) {
  *property=*geoViewShape;
  geoViewShape=property;
  glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementShape(geoViewShape);
}

void GeographicViewGraphicsView::afterSetNodeValue(PropertyInterface *prop, const node n) {
  if (geoViewSize != NULL) {
    SizeProperty *viewSize = static_cast<SizeProperty *>(prop);
    const Size &nodeSize = viewSize->getNodeValue(n);
    float sizeFactor = pow(1.3f, float(currentMapZoom));
    geoViewSize->setNodeValue(n, Size(sizeFactor * nodeSize.getW(), sizeFactor * nodeSize.getH(), sizeFactor * nodeSize.getD()));
  }
}

void GeographicViewGraphicsView::afterSetAllNodeValue(PropertyInterface *prop) {
  if (geoViewSize != NULL) {
    SizeProperty *viewSize = static_cast<SizeProperty *>(prop);
    const Size &nodeSize = viewSize->getNodeValue(graph->getOneNode());
    float sizeFactor = pow(1.3f, float(currentMapZoom));
    geoViewSize->setAllNodeValue(Size(sizeFactor * nodeSize.getW(), sizeFactor * nodeSize.getH(), sizeFactor * nodeSize.getD()));
  }
}

void GeographicViewGraphicsView::treatEvent(const Event& ev) {
  const PropertyEvent* propEvt = dynamic_cast<const PropertyEvent*>(&ev);

  if (propEvt) {
    PropertyInterface* prop = propEvt->getProperty();

    switch(propEvt->getType()) {
    case PropertyEvent::TLP_AFTER_SET_NODE_VALUE:
      afterSetNodeValue(prop, propEvt->getNode());
      break;

    case PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE:
      afterSetAllNodeValue(prop);
      break;

    default:
      break;
    }
  }
}

/*
static void getAngle(const Coord& coord,float &theta, float &phi) {
  Coord tmp(coord[1],coord[0],0);
  float lambda = tmp[1];

  if ( lambda <= M_PI)
    theta = lambda;
  else
    theta = lambda + 2. * M_PI;

  phi = M_PI / 2.0 - tmp[0];
}
*/

void GeographicViewGraphicsView::switchViewType() {
  GeographicView::ViewType viewType=_geoView->viewType();

  bool enableGoogleMap=false;
  bool enablePolygon=false;
  bool enablePlanisphere = false;

  switch(viewType) {
  case GeographicView::GoogleRoadMap: {
    enableGoogleMap=true;
    googleMaps->switchToRoadMapView();
    break;
  }

  case GeographicView::GoogleSatellite: {
    enableGoogleMap=true;
    googleMaps->switchToSatelliteView();
    break;
  }

  case GeographicView::GoogleTerrain: {
    enableGoogleMap=true;
    googleMaps->switchToTerrainView();
    break;
  }

  case GeographicView::GoogleHybrid: {
    enableGoogleMap=true;
    googleMaps->switchToHybridView();
    break;
  }

  case GeographicView::Polygon: {
    enablePolygon=true;
    glWidgetItem->setRedrawNeeded(true);
    break;
  }

  case GeographicView::Globe: {
    enablePlanisphere=true;
    break;
  }

  default:
    break;
  }

  if(planisphereEntity && planisphereEntity->isVisible()) {
    globeCameraBackup=glMainWidget->getScene()->getGraphCamera();
  }
  else {
    mapCameraBackup=glMainWidget->getScene()->getGraphCamera();
  }

  if(geoLayoutBackup!=NULL && geoLayoutComputed) {
    *geoLayout=*geoLayoutBackup;
    delete geoLayoutBackup;
    geoLayoutBackup=NULL;
  }

  GlLayer *layer=glMainWidget->getScene()->getLayer("Main");

  if (geoLayout == graph->getProperty<LayoutProperty>("viewLayout") && geoLayoutComputed)
    graph->push();

  Observable::holdObservers();

  googleMaps->setVisible(enableGoogleMap);

  if(polygonEntity)
    polygonEntity->setVisible(enablePolygon);

  layer->setCamera(new Camera(glMainWidget->getScene()));

  if(viewType!=GeographicView::Globe && geoLayoutComputed) {
    SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");
    node n;

    forEach(n, graph->getNodes()) {
      if (viewSize != geoViewSize) {
        const Size &nodeSize = viewSize->getNodeValue(n);
        geoViewSize->setNodeValue(n, nodeSize);
      }

      if (nodeLatLng.find(n) != nodeLatLng.end()) {
        geoLayout->setNodeValue(n, Coord(nodeLatLng[n].second*2., latitudeToMercator(nodeLatLng[n].first*2.),0));
      }
    }

    if (!edgeBendsLatLng.empty()) {
      edge e;
      forEach(e, graph->getEdges()) {
        vector<Coord> edgeBendsCoords;

        for (unsigned int i = 0 ; i < edgeBendsLatLng[e].size() ; ++i) {
          edgeBendsCoords.push_back(Coord(edgeBendsLatLng[e][i].second*2., latitudeToMercator(edgeBendsLatLng[e][i].first*2.),0));
        }

        geoLayout->setEdgeValue(e, edgeBendsCoords);
      }
    }



    if (firstMapSwitch) {
      BoundingBox bb;
      Coord rightCoord = googleMaps->getPixelPosOnScreenForLatLng(180,180);
      Coord leftCoord = googleMaps->getPixelPosOnScreenForLatLng(0,0);

      if (rightCoord[0] - leftCoord[0]) {
        float mapWidth=(width()/(rightCoord - leftCoord)[0])*180.;
        float middleLng=googleMaps->getLatLngForPixelPosOnScreen(width()/2.,height()/2.).second*2.;
        bb.expand(Coord(middleLng-mapWidth/2.,latitudeToMercator(googleMaps->getLatLngForPixelPosOnScreen(0,0).first*2.),0));
        bb.expand(Coord(middleLng+mapWidth/2.,latitudeToMercator(googleMaps->getLatLngForPixelPosOnScreen(width(),height()).first*2.),0));
        GlSceneZoomAndPan sceneZoomAndPan(glMainWidget->getScene(),bb,"Main",1);
        sceneZoomAndPan.zoomAndPanAnimationStep(1);
      }

      firstMapSwitch = false;
    }
    else {
      Camera &camera=glMainWidget->getScene()->getGraphCamera();
      camera.setEyes(mapCameraBackup.getEyes());
      camera.setCenter(mapCameraBackup.getCenter());
      camera.setUp(mapCameraBackup.getUp());
      camera.setZoomFactor(mapCameraBackup.getZoomFactor());
      camera.setSceneRadius(mapCameraBackup.getSceneRadius());
    }
  }

  else {

    if (!planisphereEntity) {
      if (planisphereTextureId == 0) {
        GlMainWidget::getFirstQGLWidget()->makeCurrent();
        planisphereTextureId = GlMainWidget::getFirstQGLWidget()->bindTexture(QPixmap(":/planisphere.jpg").transformed(QTransform().scale(1,-1)), GL_TEXTURE_2D, GL_RGBA, QGLContext::LinearFilteringBindOption);
        GlTextureManager::getInst().registerExternalTexture("Planisphere", planisphereTextureId);
      }

      planisphereEntity = new GlSphere(Coord(0.,0.,0.),50.,"Planisphere",255,0,0,90);
      glMainWidget->getScene()->getLayer("Main")->addGlEntity(planisphereEntity,"globeMap");
    }

    if (geoLayoutComputed) {

      SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");
      node n;
      edge e;

      assert(geoLayoutBackup==NULL);
      geoLayoutBackup = new LayoutProperty(graph);
      *geoLayoutBackup=*geoLayout;

      geoViewShape->setAllNodeValue(NodeShape::Sphere);
      geoViewShape->setAllEdgeValue(EdgeShape::CubicBSplineCurve);

      forEach(n, graph->getNodes()) {
        if (viewSize != geoViewSize) {
          const Size &nodeSize = viewSize->getNodeValue(n);
          geoViewSize->setNodeValue(n, nodeSize);
        }

        if (nodeLatLng.find(n) != nodeLatLng.end()) {
          Coord tmp(nodeLatLng[n].first*2./360.*M_PI,nodeLatLng[n].second*2./360.*M_PI,0);

          float lambda = tmp[1];
          float theta;

          if ( lambda <= M_PI)
            theta = lambda;
          else
            theta = lambda + 2. * M_PI;

          float phi = M_PI / 2.0 - tmp[0];

          tmp=Coord(50. * sin(phi) * cos(theta),
                    50. * sin(phi) * sin(theta),
                    50. * cos(phi));
          geoLayout->setNodeValue(n, tmp);
        }
      }

      forEach(e, graph->getEdges()) {
        const std::pair<node, node>& eEnds = graph->ends(e);
        node src = eEnds.first;
        node tgt = eEnds.second;
        Coord srcC(nodeLatLng[src].first*2./360.*M_PI,nodeLatLng[src].second*2./360.*M_PI,0);
        Coord tgtC(nodeLatLng[tgt].first*2./360.*M_PI,nodeLatLng[tgt].second*2./360.*M_PI,0);

        unsigned int bendsNumber=2;
        vector<Coord> bends;

        for(unsigned int i=0; i<bendsNumber; ++i) {
          Coord tmp=srcC+((tgtC-srcC)/(bendsNumber+1.f))*(i+1.f);
          float lambda = tmp[1];
          float theta;

          if ( lambda <= M_PI)
            theta = lambda;
          else
            theta = lambda + 2. * M_PI;

          float phi = M_PI / 2.0 - tmp[0];

          Coord tmp1(75. * sin(phi) * cos(theta),
                     75. * sin(phi) * sin(theta),
                     75. * cos(phi));


          bends.push_back(tmp1);
        }

        geoLayout->setEdgeValue(e, bends);
      }
    }

    if(firstGlobeSwitch) {
      firstGlobeSwitch=false;

      glMainWidget->getScene()->centerScene();
      Camera &camera=glMainWidget->getScene()->getGraphCamera();
      float centerEyeDistance=(camera.getEyes()-camera.getCenter()).norm();
      camera.setCenter(Coord(0,0,0));
      camera.setEyes(Coord(centerEyeDistance,0,0));
      camera.setUp(Coord(0,0,1));

      globeCameraBackup=camera;

    }
    else {
      Camera &camera=glMainWidget->getScene()->getGraphCamera();
      camera.setEyes(globeCameraBackup.getEyes());
      camera.setCenter(globeCameraBackup.getCenter());
      camera.setUp(globeCameraBackup.getUp());
      camera.setZoomFactor(globeCameraBackup.getZoomFactor());
      camera.setSceneRadius(globeCameraBackup.getSceneRadius());
    }
  }

  if (planisphereEntity)
    planisphereEntity->setVisible(enablePlanisphere);

  glMainWidget->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->setEdge3D(viewType==GeographicView::Globe);

  Observable::unholdObservers();

  graph->popIfNoUpdates();

  draw();
}

void GeographicViewGraphicsView::setGeoLayoutComputed() {
  geoLayoutComputed = true;
  noLayoutMsgBox->setVisible(false);
  glMainWidget->getScene()->getGlGraphComposite()->setVisible(true);
}

}
