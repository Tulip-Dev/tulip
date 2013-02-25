/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include "GoogleMapsGraphicsView.h"

#include "GoogleMapsView.h"

#include <tulip/GlCPULODCalculator.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlSphere.h>
#include <tulip/GlLine.h>
#include <tulip/GlSceneZoomAndPan.h>
#include <tulip/GlyphManager.h>

#include <QtCore/QTextStream>
#include <QtCore/QTimeLine>
#include <QtGui/QMessageBox>

using namespace std;

namespace tlp {

static QGLFormat GlInit() {
  QGLFormat tmpFormat;
  tmpFormat.setDirectRendering(true);
  tmpFormat.setDoubleBuffer(true);
  tmpFormat.setAccum(false);
  tmpFormat.setStencil(true);
  tmpFormat.setOverlay(false);
  tmpFormat.setDepth(true);
  tmpFormat.setRgba(true);
  tmpFormat.setAlpha(true);
  tmpFormat.setStereo(false);
  tmpFormat.setSampleBuffers(true);
  return tmpFormat;
}

GlComposite *readPolyFile(QString fileName) {

  map<string,vector<vector<Coord> > > clearPolygons;

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
      if(currentVector.size()!=0)
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

        if(currentVector.size()!=0)
          datas.push_back(currentVector);

        if(datas.size()!=0) {

          clearPolygons[polygonName]=datas;
          composite->addGlEntity(new GlComplexPolygon(datas,Color(0,0,0,50),Color(0,0,0,255)),polygonName);
          datas.clear();
          currentVector.clear();
        }
      }

      polygonName=line.toStdString();
      continue;
    }

    if(lat==90.)
      lat=89.999;

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
    if(currentVector.size()!=0)
      datas.push_back(currentVector);

    clearPolygons[polygonName]=datas;
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
      if(currentVector.size()!=0)
        datas.push_back(currentVector);

      currentVector=vector<Coord>();
      continue;
    }

    if(strList[0].toInt()!=lastIndex) {
      if(currentVector.size()!=0)
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

  if(datas.size()==0)
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
      if(currentVector.size()!=0)
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

        if(currentVector.size()!=0)
          datas.push_back(currentVector);

        if(datas.size()!=0) {

          clearPolygons[polygonName]=datas;
          datas.clear();
          currentVector.clear();
        }
      }

      polygonName=line.toStdString();
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
    if(currentVector.size()!=0)
      datas.push_back(currentVector);

    clearPolygons[polygonName]=datas;
  }

  map<Coord,Coord> simplifiedCoord;

  QString newName(fileName);
  newName.replace(".poly",QString("_")+QString::number(definition)+".poly");
  cout << "create : " << newName.toStdString() << endl;
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

GlSimpleEntity *buildPlanisphereEntity(GlMainWidget *mainWidget) {
  GlMainWidget::getFirstQGLWidget()->makeCurrent();
  GLuint planisphereTextureId = mainWidget->bindTexture(QPixmap(":/planisphere.jpg").transformed(QTransform().scale(1,-1)), GL_TEXTURE_2D, GL_RGBA, QGLContext::LinearFilteringBindOption);
  GlTextureManager::getInst().registerExternalTexture("Planisphere", planisphereTextureId);

  return new GlSphere(Coord(0.,0.,0.),50.,"Planisphere",255,0,0,90);
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


GoogleMapsGraphicsView::GoogleMapsGraphicsView(GoogleMapsView *googleMapsView, QGraphicsScene *graphicsScene, QWidget *parent) : QGraphicsView(graphicsScene, parent),
  _googleMapsView(googleMapsView), graph(NULL),  googleMaps(NULL), globeCameraBackup(NULL,true),geoLayout(NULL), geoViewSize(NULL), geoLayoutBackup(NULL), mapTranslationBlocked(false), mouseDragging(false),
  geocodingActive(false), cancelGeocoding(false),firstGlobeSwitch(true) {
  setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
  glWidget = new QGLWidget(GlInit(), this, GlMainWidget::getFirstQGLWidget());
  setViewport(glWidget);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setFrameStyle(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  googleMaps = new GoogleMaps();
  googleMaps->setMouseTracking(false);
  googleMaps->resize(512,512);
  progressWidget = new ProgressWidgetGraphicsProxy();
  progressWidget->hide();
  progressWidget->setZValue(2);
  addressSelectionDialog = new AddressSelectionDialog(googleMaps);
  scene()->addItem(progressWidget);
  addresseSelectionProxy = scene()->addWidget(addressSelectionDialog, Qt::Dialog);
  addresseSelectionProxy->hide();
  addresseSelectionProxy->setZValue(3);

  googleMaps->setProgressWidget(progressWidget);
  googleMaps->setAdresseSelectionDialog(addressSelectionDialog,addresseSelectionProxy);

  scene()->addWidget(googleMaps);

  while (!googleMaps->pageInit()) {
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
  }

  glMainWidget = new GlMainWidget(0, googleMapsView);
  glMainWidget->getScene()->setCalculator(new GlCPULODCalculator());
  glMainWidget->getScene()->setBackgroundColor(Color(255,255,255,0));

  glWidgetItem = new GlMainWidgetGraphicsItem(glMainWidget, 512, 512);
  glWidgetItem->setPos(0,0);

  scene()->addItem(glWidgetItem);

  viewTypeComboBox=new QComboBox;
  viewTypeComboBox->addItems(QStringList()<<"RoadMap"<<"RoadMap"<<"Satellite" << "Terrain" << "Hybrid" <<"Polygon" << "Globe");
  viewTypeComboBox->insertSeparator(1);
  QGraphicsProxyWidget *viewTypeProxyWidget=new QGraphicsProxyWidget;
  viewTypeProxyWidget->setWidget(viewTypeComboBox);
  viewTypeProxyWidget->setPos(20,20);
  scene()->addItem(viewTypeProxyWidget);
  connect(viewTypeComboBox,SIGNAL(currentIndexChanged(QString)),_googleMapsView,SLOT(viewTypeChanged(QString)));

  polygonEntity=NULL;
}



GoogleMapsGraphicsView::~GoogleMapsGraphicsView() {
  if (geocodingActive) {
    if (addressSelectionDialog->isVisible()) {
      addressSelectionDialog->accept();
    }

    cancelGeocoding = true;

    while (geocodingActive) {
      QApplication::processEvents();
    }
  }

  cleanup();
  delete glMainWidget;
}

void GoogleMapsGraphicsView::cleanup() {
  if(graph) {

    GlScene *scene=glMainWidget->getScene();
    scene->clearLayersList();

    if(geoLayout!=graph->getProperty<LayoutProperty>("viewLayout"))
      delete geoLayout;

    if(geoViewSize!=graph->getProperty<SizeProperty>("viewSize"))
      delete geoViewSize;

    if(geoViewShape!=graph->getProperty<IntegerProperty>("viewShape"))
      delete geoViewShape;
  }
}

void GoogleMapsGraphicsView::setGraph(Graph *graph) {
  if (this->graph != graph) {
    cleanup();
    this->graph = graph;

    GlScene *scene=glMainWidget->getScene();
    GlGraphComposite *graphComposite=new GlGraphComposite(graph);
    GlLayer *layer=scene->createLayer("Main");
    planisphereEntity = buildPlanisphereEntity(glMainWidget);
    layer->addGlEntity(planisphereEntity,"globeMap");

    layer->addGlEntity(graphComposite,"Graph");

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

void GoogleMapsGraphicsView::loadDefaultMap() {
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

void GoogleMapsGraphicsView::loadCsvFile(QString fileName) {
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

void GoogleMapsGraphicsView::loadPolyFile(QString fileName) {
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

void GoogleMapsGraphicsView::mapToPolygon() {
  GlComposite *composite=dynamic_cast<GlComposite*>(polygonEntity);

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
        GlComplexPolygon *polygon=dynamic_cast<GlComplexPolygon*>((*it).second);

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

GlGraphComposite *GoogleMapsGraphicsView::getGlGraphComposite() const {
  return glMainWidget->getScene()->getGlGraphComposite();

}

void GoogleMapsGraphicsView::createLayoutWithAddresses(string addressPropertyName, bool createLatAndLngProps) {
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

    while (nodesIt->hasNext() && !progressWidget->cancelRequested() && !cancelGeocoding) {
      if (grabNextNode) {
        n = nodesIt->next();
        progressWidget->setProgress(++nbNodesProcessed, nbNodes);
      }

      grabNextNode = true;
      string addr = removeQuotesIfAny(addressProperty->getNodeValue(n));

      if (addr == "") continue;

      progressWidget->setComment("Retrieving latitude and longitude for address : \n" + QString(addr.c_str()));

      if (nodeLatLng.find(n) == nodeLatLng.end()) {
        string geocodingRequestStatus = googleMaps->getLatLngForAddress(addr.c_str(), latLng, true);

        if (geocodingRequestStatus == "OK") {
          nodeLatLng[n] = latLng;

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

          while (timeLine.state() != QTimeLine::NotRunning) {
            QApplication::processEvents();
          }

          progressWidget->setFrameColor(Qt::green);
          grabNextNode = false;
        }
        else {
          progressWidget->hide();
          QMessageBox::warning(NULL, "Geolocalisation failed", "No results were found for address : \n" + QString(addr.c_str()));
          progressWidget->show();
        }

      }

      QApplication::processEvents();
    }

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
        string geocodingRequestStatus = googleMaps->getLatLngForAddress(addr.c_str(), latLng);

        if (geocodingRequestStatus != "OK") {
          QTimeLine timeLine(3500);
          timeLine.start();

          while (timeLine.state() != QTimeLine::NotRunning) {
            QApplication::processEvents();
          }

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

void GoogleMapsGraphicsView::createLayoutWithLatLngs(std::string latitudePropertyName, std::string longitudePropertyName) {
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
}

void GoogleMapsGraphicsView::resizeEvent(QResizeEvent *event) {
  QGraphicsView::resizeEvent(event);
  scene()->setSceneRect(QRect(QPoint(0, 0), size()));
  googleMaps->resize(width(), height());
  glWidgetItem->resize(width(), height());

  if (progressWidget->isVisible()) {
    progressWidget->setPos(width() / 2 - progressWidget->sceneBoundingRect().width() / 2, height() / 2 - progressWidget->sceneBoundingRect().height() / 2);
  }

  if (scene())
    scene()->update();

  // Hack : send a mouse event to force redraw of the scene (otherwise artifacts was displayed when maximizing or minimizing the graphics view)
  QMouseEvent *eventModif = new QMouseEvent(QEvent::MouseMove,QPoint(size().width()/2, size().height()/2), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
  QApplication::sendEvent(this, eventModif);
}

void GoogleMapsGraphicsView::paintEvent (QPaintEvent * event) {
  pair<double, double> mapCenter = googleMaps->getCurrentMapCenter();
  unsigned int mapZoom = googleMaps->getCurrentMapZoom();

  Observable::holdObservers();

  if (graph && !geocodingActive) {

    if(googleMaps->isVisible()) {
      float worldWidth = googleMaps->getWorldWidth();
      static const double maxLat = 85.05113f;
      Coord swPos = googleMaps->getPixelPosOnScreenForLatLng(-maxLat, 0.0);
      swPos[1]=height() - swPos[1];
      Coord nePos = googleMaps->getPixelPosOnScreenForLatLng(maxLat, 0.0);
      nePos[1]=height() - nePos[1];

      Coord mapCenterCoord = googleMaps->getPixelPosOnScreenForLatLng(mapCenter.first, mapCenter.second);
      float westToMapCenterDist = ((mapCenter.second + 180.0) * worldWidth) / 360.0;
      swPos.setX(mapCenterCoord.getX() - westToMapCenterDist);
      nePos.setX(swPos.getX() + worldWidth);

      currentMapCenter = mapCenter;
      lastSceneRect = sceneRect();
      currentMapZoom = mapZoom;

      BoundingBox bb;
      float mapWidth=(width()/(googleMaps->getPixelPosOnScreenForLatLng(180,180)-googleMaps->getPixelPosOnScreenForLatLng(0,0))[0])*180.;
      float middleLng=googleMaps->getLatLngForPixelPosOnScreen(width()/2.,height()/2.).second*2.;
      bb.expand(Coord(middleLng-mapWidth/2.,latitudeToMercator(googleMaps->getLatLngForPixelPosOnScreen(0,0).first*2.),0));
      bb.expand(Coord(middleLng+mapWidth/2.,latitudeToMercator(googleMaps->getLatLngForPixelPosOnScreen(width(),height()).first*2.),0));
      GlSceneZoomAndPan sceneZoomAndPan(glMainWidget->getScene(),bb,"Main",1);
      sceneZoomAndPan.zoomAndPanAnimationStep(1);
    }

    glWidgetItem->setRedrawNeeded(true);
    glMainWidget->getScene()->setBackgroundColor(Color(255,255,255,0));
    glMainWidget->getScene()->setClearBufferAtDraw(false);
  }

  Observable::unholdObservers();

  QGraphicsView::paintEvent(event);

}

void GoogleMapsGraphicsView::setMapTranslationBlocked(const bool translationBlocked) {
  mapTranslationBlocked = translationBlocked;
}

void GoogleMapsGraphicsView::centerView() {
  if(googleMaps->isVisible()) {
    googleMaps->setMapBounds(graph, nodeLatLng);
  }
  else {
    glMainWidget->centerScene();
  }
}
void GoogleMapsGraphicsView::centerMapOnNode(const node n) {
  if (nodeLatLng.find(n) != nodeLatLng.end()) {
    googleMaps->setMapCenter(nodeLatLng[n].first, nodeLatLng[n].second);
  }
}

void GoogleMapsGraphicsView::setGeoLayout(LayoutProperty *property) {
  *property=*geoLayout;
  geoLayout=property;
  glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementLayout(geoLayout);
}

void GoogleMapsGraphicsView::setGeoSizes(SizeProperty *property) {
  *property=*geoViewSize;
  geoViewSize=property;
  glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementSize(geoViewSize);
}

void GoogleMapsGraphicsView::setGeoShape(IntegerProperty *property) {
  *property=*geoViewShape;
  geoViewShape=property;
  glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setElementShape(geoViewShape);
}

void GoogleMapsGraphicsView::afterSetNodeValue(PropertyInterface *prop, const node n) {
  if (geoViewSize != NULL) {
    SizeProperty *viewSize = (SizeProperty *) prop;
    const Size &nodeSize = viewSize->getNodeValue(n);
    float sizeFactor = pow((float) 1.3, (int) currentMapZoom);
    geoViewSize->setNodeValue(n, Size(sizeFactor * nodeSize.getW(), sizeFactor * nodeSize.getH(), sizeFactor * nodeSize.getD()));
  }
}

void GoogleMapsGraphicsView::afterSetAllNodeValue(PropertyInterface *prop) {
  if (geoViewSize != NULL) {
    SizeProperty *viewSize = (SizeProperty *) prop;
    const Size &nodeSize = viewSize->getNodeValue(graph->getOneNode());
    float sizeFactor = pow((float) 1.3, (int) currentMapZoom);
    geoViewSize->setAllNodeValue(Size(sizeFactor * nodeSize.getW(), sizeFactor * nodeSize.getH(), sizeFactor * nodeSize.getD()));
  }
}

void getAngle(const Coord& coord,float &theta, float &phi) {
  Coord tmp(coord[1],coord[0],0);
  float lambda = tmp[1];

  if ( lambda <= M_PI)
    theta = lambda;
  else
    theta = lambda + 2. * M_PI;

  phi = M_PI / 2.0 - tmp[0];
}

void GoogleMapsGraphicsView::switchViewType() {
  GoogleMapsView::ViewType viewType=_googleMapsView->viewType();

  bool enableGoogleMap=false;
  bool enablePolygon=false;
  bool enablePlanisphere = false;

  switch(viewType) {
  case GoogleMapsView::GoogleRoadMap: {
    enableGoogleMap=true;
    googleMaps->switchToRoadMapView();
    break;
  }

  case GoogleMapsView::GoogleSatellite: {
    enableGoogleMap=true;
    googleMaps->switchToSatelliteView();
    break;
  }

  case GoogleMapsView::GoogleTerrain: {
    enableGoogleMap=true;
    googleMaps->switchToTerrainView();
    break;
  }

  case GoogleMapsView::GoogleHybrid: {
    enableGoogleMap=true;
    googleMaps->switchToHybridView();
    break;
  }

  case GoogleMapsView::Polygon: {
    enablePolygon=true;
    break;
  }

  case GoogleMapsView::Globe: {
    enablePlanisphere=true;
    break;
  }

  default:
    break;
  }

  bool switchToGoogleMap=false;

  if(googleMaps->isVisible()!=enableGoogleMap && viewType!=GoogleMapsView::Polygon && viewType!=GoogleMapsView::Globe)
    switchToGoogleMap=true;

  if(planisphereEntity->isVisible()) {
    globeCameraBackup=glMainWidget->getScene()->getGraphCamera();
  }

  if(geoLayoutBackup!=NULL) {
    *geoLayout=*geoLayoutBackup;
    delete geoLayoutBackup;
    geoLayoutBackup=NULL;
  }

  GlLayer *layer=glMainWidget->getScene()->getLayer("Main");

  Observable::holdObservers();

  if(switchToGoogleMap) {
    BoundingBox bb;
    bb.expand(layer->getCamera().screenTo3DWorld(Coord(0,0,0)));
    bb.expand(layer->getCamera().screenTo3DWorld(Coord(width(),height(),0)));

    bb[0][1]=mercatorToLatitude(bb[0][1]);
    bb[1][1]=mercatorToLatitude(bb[1][1]);

    bb[0]=bb[0]/2.f;
    bb[1]=bb[1]/2.f;

    googleMaps->setMapBounds(bb[0],bb[1]);
    googleMaps->setCurrentZoom(googleMaps->getCurrentMapZoom()+1);
  }

  googleMaps->setVisible(enableGoogleMap);

  if(polygonEntity)
    polygonEntity->setVisible(enablePolygon);

  planisphereEntity->setVisible(enablePlanisphere);

  if(enableGoogleMap)
    layer->setCamera(new Camera(glMainWidget->getScene()));
  else
    layer->setCamera(new Camera(glMainWidget->getScene()));

  if(viewType!=GoogleMapsView::Globe) {
    SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");
    node n;

    forEach(n, graph->getNodes()) {
      const Size &nodeSize = viewSize->getNodeValue(n);
      geoViewSize->setNodeValue(n, nodeSize);

      if (nodeLatLng.find(n) != nodeLatLng.end()) {
        geoLayout->setNodeValue(n, Coord(nodeLatLng[n].second*2., latitudeToMercator(nodeLatLng[n].first*2.),0));
      }
      else {
        geoLayout->setNodeValue(n, Coord(0,0,0));
      }
    }

    if (edgeBendsLatLng.size() > 0) {
      edge e;
      forEach(e, graph->getEdges()) {
        vector<Coord> edgeBendsCoords;

        for (unsigned int i = 0 ; i < edgeBendsLatLng[e].size() ; ++i) {
          edgeBendsCoords.push_back(Coord(edgeBendsLatLng[e][i].second*2., latitudeToMercator(edgeBendsLatLng[e][i].first*2.),0));
        }

        geoLayout->setEdgeValue(e, edgeBendsCoords);
      }
    }

    BoundingBox bb;
    float mapWidth=(width()/(googleMaps->getPixelPosOnScreenForLatLng(180,180)-googleMaps->getPixelPosOnScreenForLatLng(0,0))[0])*180.;
    float middleLng=googleMaps->getLatLngForPixelPosOnScreen(width()/2.,height()/2.).second*2.;
    bb.expand(Coord(middleLng-mapWidth/2.,latitudeToMercator(googleMaps->getLatLngForPixelPosOnScreen(0,0).first*2.),0));
    bb.expand(Coord(middleLng+mapWidth/2.,latitudeToMercator(googleMaps->getLatLngForPixelPosOnScreen(width(),height()).first*2.),0));
    GlSceneZoomAndPan sceneZoomAndPan(glMainWidget->getScene(),bb,"Main",1);
    sceneZoomAndPan.zoomAndPanAnimationStep(1);
  }

  if(viewType==GoogleMapsView::Globe) {
    SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");
    node n;
    edge e;

    assert(geoLayoutBackup==NULL);
    geoLayoutBackup = new LayoutProperty(graph);
    *geoLayoutBackup=*geoLayout;

    geoViewShape->setAllNodeValue(GlyphManager::getInst().glyphId("3D - Sphere"));
    geoViewShape->setAllEdgeValue(16);

    forEach(n, graph->getNodes()) {
      const Size &nodeSize = viewSize->getNodeValue(n);
      geoViewSize->setNodeValue(n, nodeSize);

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
      else {
        geoLayout->setNodeValue(n, Coord(0,0,0));
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
        Coord tmp=srcC+((tgtC-srcC)/(bendsNumber+1.f))*((float)i+1);
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

    if (edgeBendsLatLng.size() > 0) {
      edge e;
      forEach(e, graph->getEdges()) {
        vector<Coord> edgeBendsCoords;

        for (unsigned int i = 0 ; i < edgeBendsLatLng[e].size() ; ++i) {
          edgeBendsCoords.push_back(Coord(edgeBendsLatLng[e][i].second*2., latitudeToMercator(edgeBendsLatLng[e][i].first*2.),0));
        }

        geoLayout->setEdgeValue(e, edgeBendsCoords);
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

      draw();
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

  Observable::unholdObservers();

}

}
