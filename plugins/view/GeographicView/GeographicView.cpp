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
#include <tulip/SnapshotDialog.h>
#include <tulip/TlpQtTools.h>
#include <tulip/OpenGlConfigManager.h>

#include <QMenu>
#include <QThread>
#include <QComboBox>
#include <QTimeLine>
#include <QApplication>
#include <QGLFramebufferObject>
#include <QMessageBox>
#include <QTimer>

#include <iostream>

#include "GeographicView.h"

using namespace std;
using namespace tlp;

GeographicView::GeographicView(PluginContext *) : geoViewGraphicsView(NULL), geoViewConfigWidget(NULL),geolocalisationConfigWidget(NULL),sceneConfigurationWidget(NULL),sceneLayersConfigurationWidget(NULL),centerViewAction(NULL),showConfPanelAction(NULL),useSharedLayoutProperty(true),useSharedSizeProperty(true),useSharedShapeProperty(true),mapCenterLatitudeInit(0), mapCenterLongitudeInit(0),mapZoomInit(0) {
  _viewType=GoogleRoadMap;
}

GeographicView::~GeographicView() {
  delete geolocalisationConfigWidget;
  delete geoViewConfigWidget;
  delete sceneConfigurationWidget;
  delete sceneLayersConfigurationWidget;
}

void GeographicView::setupUi() {
  geoViewGraphicsView = new GeographicViewGraphicsView(this, new QGraphicsScene());

  geoViewConfigWidget = new GeographicViewConfigWidget();
  connect(geoViewConfigWidget,SIGNAL(mapToPolygonSignal()),this,SLOT(mapToPolygon()));

  geolocalisationConfigWidget = new GeolocalisationConfigWidget();
  connect(geolocalisationConfigWidget, SIGNAL(computeGeoLayout()), this, SLOT(computeGeoLayout()));

  sceneConfigurationWidget=new SceneConfigWidget();
  sceneConfigurationWidget->setGlMainWidget(geoViewGraphicsView->getGlMainWidget());

  sceneLayersConfigurationWidget=new SceneLayersConfigWidget();
  sceneLayersConfigurationWidget->setGlMainWidget(geoViewGraphicsView->getGlMainWidget());

  centerViewAction = new QAction("Center view", this);
  connect(centerViewAction,SIGNAL(triggered()),this,SLOT(centerView()));

}

void GeographicView::viewTypeChanged(QString viewTypeName) {
  QComboBox *comboBox=geoViewGraphicsView->getViewTypeComboBox();

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

  geoViewGraphicsView->switchViewType();

  comboBox->removeItem(0);
  comboBox->insertItem(0,viewTypeName);
  comboBox->setCurrentIndex(0);

  connect(comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(viewTypeChanged(QString)));
}

void GeographicView::fillContextMenu(QMenu *menu, const QPointF &) {
  menu->addAction(centerViewAction);
  centerViewAction->setToolTip(QString("Make the view to fully display and center its contents"));
  QAction* action = menu->addAction("Zoom +");
  action->setToolTip(QString("Increase zoom level"));
  connect(action,SIGNAL(triggered()), this, SLOT(zoomIn()));
  action = menu->addAction("Zoom -");
  action->setToolTip(QString("Increase zoom level"));
  connect(action,SIGNAL(triggered()), this, SLOT(zoomOut()));
  action = menu->addAction("Take a snapshot");
  action->setToolTip(QString("Show a dialog to save a snapshot of the current view display"));
  connect(action,SIGNAL(triggered()), this, SLOT(openSnapshotDialog()));
}

void GeographicView::setState(const DataSet &dataSet) {
  geolocalisationConfigWidget->setGraph(graph());
  geoViewGraphicsView->setGraph(graph());

  updatePoly(true);

  if(dataSet.exist("configurationWidget")) {
    DataSet conf;
    dataSet.get("configurationWidget",conf);
    geoViewConfigWidget->setState(conf);
    updatePoly();
    updateSharedProperties();
  }

  loadStoredPolyInformation(dataSet);

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

  sceneLayersConfigurationWidget->setGlMainWidget(geoViewGraphicsView->getGlMainWidget());
  sceneConfigurationWidget->setGlMainWidget(geoViewGraphicsView->getGlMainWidget());

  registerTriggers();

  string latitudePropName = "latitude";
  string longitudePropName = "longitude";

  dataSet.get("latitudePropertyName", latitudePropName);
  dataSet.get("longitudePropertyName", longitudePropName);

  if (graph()->existProperty(latitudePropName) && graph()->existProperty(longitudePropName)) {
    geolocalisationConfigWidget->setLatLngGeoLocMethod(latitudePropName, longitudePropName);

    string edgesPathsPropName;
    dataSet.get("edgesPathsPropertyName", edgesPathsPropName);
    geolocalisationConfigWidget->setEdgesPathsPropertyName(edgesPathsPropName);

    computeGeoLayout();
  }

  if (dataSet.exist("renderingParameters")) {
    GlGraphComposite *graphComposite = geoViewGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite();
    DataSet renderingParameters;
    dataSet.get("renderingParameters", renderingParameters);
    GlGraphRenderingParameters rp = graphComposite->getRenderingParameters();
    rp.setParameters(renderingParameters);
    string s;

    if (renderingParameters.get("elementsOrderingPropertyName", s) &&
        !s.empty()) {
      rp.setElementOrderingProperty(dynamic_cast<tlp::NumericProperty*>(graph()->getProperty(s)));
    }

    graphComposite->setRenderingParameters(rp);
    sceneConfigurationWidget->resetChanges();
  }

  if(dataSet.exist("mapCenterLatitude")) {

    dataSet.get("mapCenterLatitude", mapCenterLatitudeInit);
    dataSet.get("mapCenterLongitude", mapCenterLongitudeInit);
    dataSet.get("mapZoom", mapZoomInit);

    QTimer::singleShot(1500, this, SLOT(initMap()));
  }

}

void GeographicView::initMap() {
  geoViewGraphicsView->getGoogleMapsPage()->setMapCenter(mapCenterLatitudeInit, mapCenterLongitudeInit);
  geoViewGraphicsView->getGoogleMapsPage()->setCurrentZoom(mapZoomInit);
}

DataSet GeographicView::state() const {
  DataSet dataSet;
  DataSet configurationWidget = geoViewConfigWidget->state();
  dataSet.set("configurationWidget", configurationWidget);
  dataSet.set("viewType", int(_viewType));
  pair<double, double> mapCenter = geoViewGraphicsView->getGoogleMapsPage()->getCurrentMapCenter();
  dataSet.set("mapCenterLatitude", mapCenter.first);
  dataSet.set("mapCenterLongitude", mapCenter.second);
  dataSet.set("mapZoom", geoViewGraphicsView->getGoogleMapsPage()->getCurrentMapZoom());
  dataSet.set("renderingParameters", geoViewGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParameters().getParameters());

  saveStoredPolyInformation(dataSet);

  std::string latitudePropName = geolocalisationConfigWidget->getLatitudeGraphPropertyName();
  std::string longitudePropName = geolocalisationConfigWidget->getLongitudeGraphPropertyName();

  if (latitudePropName != longitudePropName &&
      graph()->existProperty(latitudePropName) && graph()->existProperty(longitudePropName)) {
    dataSet.set("latitudePropertyName", latitudePropName);
    dataSet.set("longitudePropertyName", longitudePropName);
  }

  std::string edgesPathsPropName = geolocalisationConfigWidget->getEdgesPathsPropertyName();

  if (graph()->existProperty(edgesPathsPropName)) {
    dataSet.set("edgesPathsPropertyName", edgesPathsPropName);
  }

  return dataSet;
}

void GeographicView::draw() {
  geoViewGraphicsView->draw();
}

void GeographicView::refresh() {
  geoViewGraphicsView->draw();
}

void GeographicView::computeGeoLayout() {
  if (geolocalisationConfigWidget->geolocateByAddress()) {
    geoViewGraphicsView->createLayoutWithAddresses(geolocalisationConfigWidget->getAddressGraphPropertyName(), geolocalisationConfigWidget->createLatAndLngProperties());

    if (geolocalisationConfigWidget->createLatAndLngProperties()) {
      geolocalisationConfigWidget->setGraph(graph());
      geolocalisationConfigWidget->setLatLngGeoLocMethod("latitude", "longitude");
    }
  }
  else {
    string latProp = geolocalisationConfigWidget->getLatitudeGraphPropertyName();
    string lngProp = geolocalisationConfigWidget->getLongitudeGraphPropertyName();
    string edgesPathsProp = "";

    if (geolocalisationConfigWidget->useEdgesPaths()) {
      edgesPathsProp = geolocalisationConfigWidget->getEdgesPathsPropertyName();
    }

    if (latProp != lngProp) {
      geoViewGraphicsView->createLayoutWithLatLngs(latProp, lngProp, edgesPathsProp);
    }
  }

  geoViewGraphicsView->centerView();
  // check for shared properties
  // before computing view layout
  updateSharedProperties();
  geoViewGraphicsView->setGeoLayoutComputed();
  // compute view layout
  geoViewGraphicsView->switchViewType();
}

void GeographicView::centerView() {
  geoViewGraphicsView->centerView();
}

void GeographicView::zoomIn() {
  geoViewGraphicsView->zoomIn();
}

void GeographicView::zoomOut() {
  geoViewGraphicsView->zoomOut();
}

void GeographicView::currentZoomChanged() {
  geoViewGraphicsView->currentZoomChanged();
}

QList<QWidget*> GeographicView::configurationWidgets() const {
  return QList<QWidget*>() << geolocalisationConfigWidget << geoViewConfigWidget << sceneConfigurationWidget << sceneLayersConfigurationWidget;
}

void GeographicView::applySettings() {
  updateSharedProperties();
  updatePoly();
}

void GeographicView::updateSharedProperties() {
  GlGraphInputData *inputData=geoViewGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();

  if(useSharedLayoutProperty!=geoViewConfigWidget->useSharedLayoutProperty()) {
    useSharedLayoutProperty=geoViewConfigWidget->useSharedLayoutProperty();

    if(useSharedLayoutProperty)
      geoViewGraphicsView->setGeoLayout(graph()->getProperty<LayoutProperty>("viewLayout"));
    else
      geoViewGraphicsView->setGeoLayout(new LayoutProperty(graph()));
  }

  if(useSharedShapeProperty!=geoViewConfigWidget->useSharedShapeProperty()) {
    useSharedShapeProperty=geoViewConfigWidget->useSharedShapeProperty();

    if(useSharedShapeProperty)
      geoViewGraphicsView->setGeoShape(graph()->getProperty<IntegerProperty>("viewShape"));
    else
      geoViewGraphicsView->setGeoShape(new IntegerProperty(graph()));
  }

  if(useSharedSizeProperty!=geoViewConfigWidget->useSharedSizeProperty()) {
    useSharedSizeProperty=geoViewConfigWidget->useSharedSizeProperty();

    if(useSharedSizeProperty)
      geoViewGraphicsView->setGeoSizes(graph()->getProperty<SizeProperty>("viewSize"));
    else
      geoViewGraphicsView->setGeoSizes(new SizeProperty(graph()));
  }

  inputData->getGlVertexArrayManager()->setHaveToComputeAll(true);
}

void GeographicView::updatePoly(bool force) {
  if(geoViewConfigWidget->polyOptionsChanged() || force) {
    switch(geoViewConfigWidget->polyFileType()) {
    case GeographicViewConfigWidget::CsvFile: {
      geoViewGraphicsView->loadCsvFile(geoViewConfigWidget->getCsvFile());
      break;
    }

    case GeographicViewConfigWidget::PolyFile: {
      geoViewGraphicsView->loadPolyFile(geoViewConfigWidget->getPolyFile());
      break;
    }

    default : {
      geoViewGraphicsView->loadDefaultMap();
      break;
    }
    }
  }
}

void GeographicView::loadStoredPolyInformation(const DataSet &dataset) {
  if(dataset.exist("polygons")) {
    DataSet polyConf;
    dataset.get("polygons",polyConf);
    GlComposite *composite=geoViewGraphicsView->getPolygon();
    const map<string, GlSimpleEntity*> &entities=composite->getGlEntities();

    for(map<string,GlSimpleEntity*>::const_iterator it=entities.begin(); it!=entities.end(); ++it) {
      DataSet entityData;

      if(polyConf.exist((*it).first)) {
        polyConf.get((*it).first,entityData);
        Color color;
        entityData.get("color",color);
        static_cast<GlComplexPolygon*>((*it).second)->setFillColor(color);
        entityData.get("outlineColor",color);
        static_cast<GlComplexPolygon*>((*it).second)->setOutlineColor(color);
      }
    }
  }
}

void GeographicView::saveStoredPolyInformation(DataSet &dataset) const {
  GlComposite *composite=geoViewGraphicsView->getPolygon();
  DataSet polyConf;
  const map<string, GlSimpleEntity*> &entities=composite->getGlEntities();

  for(map<string,GlSimpleEntity*>::const_iterator it=entities.begin(); it!=entities.end(); ++it) {
    DataSet entityData;
    entityData.set("color", static_cast<GlComplexPolygon*>((*it).second)->getFillColor());
    entityData.set("outlineColor", static_cast<GlComplexPolygon*>((*it).second)->getOutlineColor());
    polyConf.set((*it).first,entityData);
  }

  dataset.set("polygons",polyConf);
}

QGraphicsItem *GeographicView::centralItem() const {
  return geoViewGraphicsView->getPlaceHolderItem();
}

void GeographicView::registerTriggers() {
  clearRedrawTriggers();

  if (graph() == NULL)
    return;

  addRedrawTrigger(geoViewGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getGraph());
  std::set<tlp::PropertyInterface*> properties = geoViewGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->properties();

  for(std::set<tlp::PropertyInterface*>::iterator it = properties.begin(); it != properties.end(); ++it) {
    addRedrawTrigger(*it);
  }
}

QPixmap GeographicView::snapshot(const QSize &size) const {

  // hide the graphics widget used to configure the view
  // before taking a snapshot
  QList<QGraphicsProxyWidget*> gWidgetsToRestore;
  QList<QGraphicsItem*> sceneItems = geoViewGraphicsView->scene()->items();

  for (int i = 0 ; i < sceneItems.size() ; ++i) {
    QGraphicsProxyWidget *gWidget = dynamic_cast<QGraphicsProxyWidget*>(sceneItems.at(i));

    if (gWidget && gWidget->isVisible()) {
      gWidget->hide();
      gWidgetsToRestore.push_back(gWidget);
    }
  }

  QGLFramebufferObjectFormat fboFormat;
  fboFormat.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
  fboFormat.setSamples(OpenGlConfigManager::getInst().maxNumberOfSamples());

  int width = geoViewGraphicsView->width();
  int height = geoViewGraphicsView->height();

  QGLFramebufferObject renderFbo(width, height, fboFormat);
  QGLFramebufferObject renderFbo2(width, height);

  QPainter fboPainter(&renderFbo);
  fboPainter.setRenderHint(QPainter::Antialiasing);
  fboPainter.setRenderHint(QPainter::HighQualityAntialiasing);
  geoViewGraphicsView->scene()->render(&fboPainter);
  fboPainter.end();

  QGLFramebufferObject::blitFramebuffer(&renderFbo2, QRect(0,0,width, height), &renderFbo, QRect(0,0,width, height));

  // restore the graphics widgets previously hidden
  for (int i = 0 ; i < gWidgetsToRestore.size() ; ++i) {
    gWidgetsToRestore.at(i)->show();
  }

  QImage snapshotImage = renderFbo2.toImage();
  snapshotImage = QImage(snapshotImage.bits(),snapshotImage.width(),snapshotImage.height(),QImage::Format_ARGB32).convertToFormat(QImage::Format_RGB32);

  return QPixmap::fromImage(snapshotImage).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

}

void GeographicView::openSnapshotDialog() {
  SnapshotDialog dlg(this);
  dlg.setSnapshotHasViewSizeRatio(true);
  dlg.exec();
}

PLUGIN(GeographicView)
