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

#include <GL/glew.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlMainWidget.h>
#include <tulip/ForEach.h>
#include <tulip/Camera.h>
#include <tulip/GlTools.h>
#include <tulip/Interactor.h>
#include <tulip/DataSet.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlComplexPolygon.h>

#include <QMenu>
#include <QWebFrame>
#include <QThread>
#include <QComboBox>
#include <QDomDocument>
#include <QXmlDefaultHandler>
#include <QTimeLine>
#include <QApplication>

#include <iostream>

#include "GoogleMapsView.h"

using namespace std;
using namespace tlp;

GoogleMapsView::GoogleMapsView(PluginContext *) : googleMapsViewConfigWidget(NULL),geolocalisationConfigWidget(NULL),sceneConfigurationWidget(NULL),
  sceneLayersConfigurationWidget(NULL),useSharedLayoutProperty(true),useSharedSizeProperty(true),useSharedShapeProperty(true) {
  _viewType=GoogleRoadMap;
}

GoogleMapsView::~GoogleMapsView() {
  delete geolocalisationConfigWidget;
  delete googleMapsViewConfigWidget;
  delete sceneConfigurationWidget;
  delete sceneLayersConfigurationWidget;
}

void GoogleMapsView::setupUi() {
  googleMapsGraphicsView = new GoogleMapsGraphicsView(this, new QGraphicsScene());

  googleMapsViewConfigWidget = new GoogleMapsViewConfigWidget();
  connect(googleMapsViewConfigWidget,SIGNAL(mapToPolygonSignal()),this,SLOT(mapToPolygon()));

  geolocalisationConfigWidget = new GeolocalisationConfigWidget();
  connect(geolocalisationConfigWidget, SIGNAL(computeGeoLayout()), this, SLOT(computeGeoLayout()));

  sceneConfigurationWidget=new SceneConfigWidget();
  sceneConfigurationWidget->setGlMainWidget(googleMapsGraphicsView->getGlMainWidget());

  sceneLayersConfigurationWidget=new SceneLayersConfigWidget();
  sceneLayersConfigurationWidget->setGlMainWidget(googleMapsGraphicsView->getGlMainWidget());

  centerViewAction = new QAction("Center view", this);
  connect(centerViewAction,SIGNAL(triggered()),this,SLOT(centerView()));

  _placeholderItem = new QGraphicsRectItem(0,0,1,1);
  _placeholderItem->setBrush(Qt::transparent);
  _placeholderItem->setPen(QPen(Qt::transparent));
  googleMapsGraphicsView->scene()->addItem(_placeholderItem);
}

void GoogleMapsView::viewTypeChanged(QString viewTypeName) {
  QComboBox *comboBox=googleMapsGraphicsView->getViewTypeComboBox();

  if (comboBox == NULL)
    return;

  disconnect(comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(viewTypeChanged(QString)));

  if(viewTypeName=="RoadMap") {
    _viewType=GoogleRoadMap;
  }
  else if(viewTypeName=="Satellite") {
    _viewType=GoogleSatellite;
  }
  else if(viewTypeName=="Terrain") {
    _viewType=GoogleTerrain;
  }
  else if(viewTypeName=="Hybrid") {
    _viewType=GoogleHybrid;
  }
  else if(viewTypeName=="Polygon") {
    _viewType=Polygon;
  }
  else if(viewTypeName=="Globe") {
    _viewType=Globe;
  }

  googleMapsGraphicsView->switchViewType();

  comboBox->removeItem(0);
  comboBox->insertItem(0,viewTypeName);
  comboBox->setCurrentIndex(0);

  connect(comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(viewTypeChanged(QString)));
}

void GoogleMapsView::fillContextMenu(QMenu *menu, const QPointF &) {
  menu->addAction(centerViewAction);
  QAction* action = new QAction("Zoom +", this);
  connect(action,SIGNAL(triggered()), this, SLOT(zoomOut()));
  menu->addAction(action);
  action = new QAction("Zoom -", this);
  connect(action,SIGNAL(triggered()), this, SLOT(zoomIn()));
  menu->addAction(action);
}

void GoogleMapsView::setState(const DataSet &dataSet) {
  geolocalisationConfigWidget->setGraph(graph());
  googleMapsGraphicsView->setGraph(graph());

  updatePoly(true);

  if (graph()->existProperty("latitude") && graph()->existProperty("longitude")) {
    geolocalisationConfigWidget->setLatLngGeoLocMethod();
    computeGeoLayout();
  }

  QTimeLine timeLine(500);
  timeLine.start();

  while (timeLine.state() == QTimeLine::Running) {
    QApplication::processEvents();
  }

  if(dataSet.exist("configurationWidget")) {
    DataSet conf;
    dataSet.get("configurationWidget",conf);
    googleMapsViewConfigWidget->setState(conf);
    updatePoly();
    updateSharedProperties();
  }

  loadStoredPolyInformations(dataSet);

  if(dataSet.exist("viewType")) {
    int viewType = 0;
    dataSet.get("viewType", viewType);
    _viewType = static_cast<ViewType>(viewType);
  }

  string viewTypeName="RoadMap";

  if(_viewType==GoogleSatellite)
    viewTypeName="Satellite";
  else if(_viewType==GoogleTerrain)
    viewTypeName="Terrain";
  else if(_viewType==GoogleHybrid)
    viewTypeName="Hybrid";
  else if(_viewType==Polygon)
    viewTypeName="Polygon";
  else if(_viewType==Globe)
    viewTypeName="Globe";

  viewTypeChanged(viewTypeName.c_str());

  if(dataSet.exist("cameras")) {
    string cameras;
    dataSet.get("cameras",cameras);
    googleMapsGraphicsView->getGlMainWidget()->getScene()->setWithXML(cameras,graph());
  }

  sceneLayersConfigurationWidget->setGlMainWidget(googleMapsGraphicsView->getGlMainWidget());
  sceneConfigurationWidget->setGlMainWidget(googleMapsGraphicsView->getGlMainWidget());

  registerTriggers();

}

DataSet GoogleMapsView::state() const {
  DataSet dataSet;
  DataSet configurationWidget=googleMapsViewConfigWidget->state();
  dataSet.set("configurationWidget",configurationWidget);
  dataSet.set("viewType",(int)_viewType);
  saveStoredPolyInformations(dataSet);
  string cameras;
  googleMapsGraphicsView->getGlMainWidget()->getScene()->getXMLOnlyForCameras(cameras);
  dataSet.set("cameras",cameras);
  return dataSet;
}

void GoogleMapsView::draw() {
  googleMapsGraphicsView->draw();
}

void GoogleMapsView::refresh() {
  googleMapsGraphicsView->draw();
}

void GoogleMapsView::computeGeoLayout() {
  if (geolocalisationConfigWidget->geolocateByAddress()) {
    googleMapsGraphicsView->createLayoutWithAddresses(geolocalisationConfigWidget->getAddressGraphPropertyName(), geolocalisationConfigWidget->createLatAndLngProperties());
  }
  else {
    string latProp = geolocalisationConfigWidget->getLatitudeGraphPropertyName();
    string lngProp = geolocalisationConfigWidget->getLongitudeGraphPropertyName();

    if (latProp != lngProp) {
      googleMapsGraphicsView->createLayoutWithLatLngs(latProp, lngProp);
    }
  }

  googleMapsGraphicsView->centerView();
  // check for shared properties
  // before computing view layout
  updateSharedProperties();
  // compute view layout
  googleMapsGraphicsView->switchViewType();
}

void GoogleMapsView::centerView() {
  googleMapsGraphicsView->centerView();
}

void GoogleMapsView::zoomIn() {
  googleMapsGraphicsView->zoomIn();
}

void GoogleMapsView::zoomOut() {
  googleMapsGraphicsView->zoomOut();
}

void GoogleMapsView::currentZoomChanged() {
  googleMapsGraphicsView->currentZoomChanged();
}

QList<QWidget*> GoogleMapsView::configurationWidgets() const {
  return QList<QWidget*>() << geolocalisationConfigWidget << googleMapsViewConfigWidget << sceneConfigurationWidget << sceneLayersConfigurationWidget;
}

void GoogleMapsView::applySettings() {
  updateSharedProperties();
  updatePoly();
}

void GoogleMapsView::updateSharedProperties() {
  GlGraphInputData *inputData=googleMapsGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();

  if(useSharedLayoutProperty!=googleMapsViewConfigWidget->useSharedLayoutProperty()) {
    useSharedLayoutProperty=googleMapsViewConfigWidget->useSharedLayoutProperty();

    if(useSharedLayoutProperty)
      googleMapsGraphicsView->setGeoLayout(graph()->getProperty<LayoutProperty>("viewLayout"));
    else
      googleMapsGraphicsView->setGeoLayout(new LayoutProperty(graph()));
  }

  if(useSharedShapeProperty!=googleMapsViewConfigWidget->useSharedShapeProperty()) {
    useSharedShapeProperty=googleMapsViewConfigWidget->useSharedShapeProperty();

    if(useSharedShapeProperty)
      googleMapsGraphicsView->setGeoShape(graph()->getProperty<IntegerProperty>("viewShape"));
    else
      googleMapsGraphicsView->setGeoShape(new IntegerProperty(graph()));
  }

  if(useSharedSizeProperty!=googleMapsViewConfigWidget->useSharedSizeProperty()) {
    useSharedSizeProperty=googleMapsViewConfigWidget->useSharedSizeProperty();

    if(useSharedSizeProperty)
      googleMapsGraphicsView->setGeoSizes(graph()->getProperty<SizeProperty>("viewSize"));
    else
      googleMapsGraphicsView->setGeoSizes(new SizeProperty(graph()));
  }

  inputData->getGlVertexArrayManager()->setHaveToComputeAll(true);
}

void GoogleMapsView::updatePoly(bool force) {
  if(googleMapsViewConfigWidget->polyOptionsChanged() || force) {
    switch(googleMapsViewConfigWidget->polyFileType()) {
    case GoogleMapsViewConfigWidget::CsvFile: {
      googleMapsGraphicsView->loadCsvFile(googleMapsViewConfigWidget->getCsvFile());
      break;
    }

    case GoogleMapsViewConfigWidget::PolyFile: {
      googleMapsGraphicsView->loadPolyFile(googleMapsViewConfigWidget->getPolyFile());
      break;
    }

    default : {
      googleMapsGraphicsView->loadDefaultMap();
      break;
    }
    }
  }
}

void GoogleMapsView::loadStoredPolyInformations(const DataSet &dataset) {
  if(dataset.exist("polygons")) {
    DataSet polyConf;
    dataset.get("polygons",polyConf);
    GlComposite *composite=googleMapsGraphicsView->getPolygon();
    const map<string, GlSimpleEntity*> &entities=composite->getGlEntities();

    for(map<string,GlSimpleEntity*>::const_iterator it=entities.begin(); it!=entities.end(); ++it) {
      DataSet entityData;

      if(polyConf.exist((*it).first)) {
        polyConf.get((*it).first,entityData);
        Color color;
        entityData.get("color",color);
        ((GlComplexPolygon*)(*it).second)->setFillColor(color);
        entityData.get("outlineColor",color);
        ((GlComplexPolygon*)(*it).second)->setOutlineColor(color);
      }
    }
  }
}

void GoogleMapsView::saveStoredPolyInformations(DataSet &dataset) const {
  GlComposite *composite=googleMapsGraphicsView->getPolygon();
  DataSet polyConf;
  const map<string, GlSimpleEntity*> &entities=composite->getGlEntities();

  for(map<string,GlSimpleEntity*>::const_iterator it=entities.begin(); it!=entities.end(); ++it) {
    DataSet entityData;
    entityData.set("color",((GlComplexPolygon*)(*it).second)->getFillColor());
    entityData.set("outlineColor",((GlComplexPolygon*)(*it).second)->getOutlineColor());
    polyConf.set((*it).first,entityData);
  }

  dataset.set("polygons",polyConf);
}

QGraphicsItem *GoogleMapsView::centralItem() const {
  return _placeholderItem;
}

void GoogleMapsView::registerTriggers() {
  clearRedrawTriggers();

  if (graph() == NULL)
    return;

  addRedrawTrigger(googleMapsGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getGraph());
  std::set<tlp::PropertyInterface*> properties = googleMapsGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->properties();

  for(std::set<tlp::PropertyInterface*>::iterator it = properties.begin(); it != properties.end(); ++it) {
    addRedrawTrigger(*it);
  }
}

PLUGIN(GoogleMapsView)
