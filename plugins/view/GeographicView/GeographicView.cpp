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

#include <tulip/GlMainWidget.h>
#include <tulip/Interactor.h>
#include <tulip/DataSet.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/TlpQtTools.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/TulipViewSettings.h>

#include <QMenu>
#include <QComboBox>
#include <QApplication>
#include <QTimer>

#include <iostream>

#include "GeographicView.h"

using namespace std;
using namespace tlp;

// define mapLayers constant
#define ADD_MAPLAYERS
#include "MapTypeAndLayers.defs"

GeographicView::GeographicView(PluginContext *)
    : geoViewGraphicsView(nullptr), geoViewConfigWidget(nullptr),
      geolocalisationConfigWidget(nullptr), sceneConfigurationWidget(nullptr),
      sceneLayersConfigurationWidget(nullptr), centerOnNodeAction(nullptr),
      showConfPanelAction(nullptr), useSharedLayoutProperty(true), useSharedSizeProperty(true),
      useSharedShapeProperty(true), mapCenterLatitudeInit(0), mapCenterLongitudeInit(0),
      mapZoomInit(0), _viewActionsManager(nullptr) {
  _mapType = OpenStreetMap;
}

GeographicView::~GeographicView() {
  delete geolocalisationConfigWidget;
  delete geoViewConfigWidget;
  delete sceneConfigurationWidget;
  delete sceneLayersConfigurationWidget;
  delete _viewActionsManager;
}

void GeographicView::setupUi() {
  geoViewGraphicsView = new GeographicViewGraphicsView(this, new QGraphicsScene());

  geoViewConfigWidget = new GeographicViewConfigWidget();
  connect(geoViewConfigWidget, SIGNAL(mapToPolygonSignal()), geoViewGraphicsView,
          SLOT(mapToPolygon()));

  geolocalisationConfigWidget = new GeolocalisationConfigWidget();
  connect(geolocalisationConfigWidget, SIGNAL(computeGeoLayout()), this, SLOT(computeGeoLayout()));

  sceneConfigurationWidget = new SceneConfigWidget();
  sceneConfigurationWidget->setGlMainWidget(geoViewGraphicsView->getGlMainWidget());

  sceneLayersConfigurationWidget = new SceneLayersConfigWidget();
  sceneLayersConfigurationWidget->setGlMainWidget(geoViewGraphicsView->getGlMainWidget());

  centerOnNodeAction = new QAction("Center on node", this);
  centerOnNodeAction->setToolTip("Center the view on the node under the menu top left corner");
  connect(centerOnNodeAction, SIGNAL(triggered()), this, SLOT(centerOnNode()));

  activateTooltipAndUrlManager(geoViewGraphicsView->getGlMainWidget());
  _viewActionsManager = new ViewActionsManager(this, geoViewGraphicsView->getGlMainWidget(), true);
}

void GeographicView::graphChanged(Graph *g) {
  setState(getState(g));

  if (g->isEmpty()) {
    // we perform an acceptable automatic configuration
    // in order to allow an interactive creation of the graph
    // over the geographic map, using the 'Add nodes/edges'
    // and 'Edit edge bends' view interactors
    auto prop = g->getProperty<SizeProperty>("viewSize");
    if (prop->getNodeDefaultValue() == TulipViewSettings::defaultSize(ElementType::NODE))
      prop->setNodeDefaultValue({0.0005, 0.0005, 0.0005});
    computeGeoLayout();
  }
}

void GeographicView::mapTypeChanged(QString mapTypeName) {
  QComboBox *comboBox = geoViewGraphicsView->getMapTypeComboBox();

  if (comboBox == nullptr)
    return;

  disconnect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(mapTypeChanged(QString)));

  _mapType = getMapType(mapTypeName);

  geoViewGraphicsView->switchMapType();

  comboBox->setCurrentIndex(0);
  comboBox->setItemText(0, mapTypeName);

  connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(mapTypeChanged(QString)));
}

void GeographicView::fillContextMenu(QMenu *menu, const QPointF &pf) {
  _viewActionsManager->fillContextMenu(menu);
  // Check if a node is under the mouse pointer
  bool result;
  SelectedEntity entity;
  result = geoViewGraphicsView->getGlMainWidget()->pickNodesEdges(pf.x(), pf.y(), entity);

  if (result && entity.getEntityType() == SelectedEntity::NODE_SELECTED) {
    auto id = entity.getComplexEntityId();
    _nodeUnderMouse = node(id);
    QString sId = QString::number(id);
    centerOnNodeAction->setText("Center view on node #" + sId);
    // insert after center view
    QAction *centerViewAction = nullptr;
    for (auto *action : menu->actions()) {
      if (centerViewAction) {
        menu->insertAction(action, centerOnNodeAction);
        break;
      }
      if (action->text() == "Center view")
        centerViewAction = action;
    }
  }

  QAction *action = menu->addAction("Zoom +");
  action->setToolTip(QString("Increase zoom level"));
  connect(action, SIGNAL(triggered()), this, SLOT(zoomIn()));
  action = menu->addAction("Zoom -");
  action->setToolTip(QString("Increase zoom level"));
  connect(action, SIGNAL(triggered()), this, SLOT(zoomOut()));
  menu->addSeparator();
  menu->addAction("Augmented display")->setEnabled(false);
  menu->addSeparator();
  View::fillContextMenu(menu, pf);
}

void GeographicView::setState(const DataSet &dataSet) {
  geolocalisationConfigWidget->setGraph(graph());
  geoViewGraphicsView->setGraph(graph());

  updatePoly(true);

  if (dataSet.exists("configurationWidget")) {
    DataSet conf;
    dataSet.get("configurationWidget", conf);
    geoViewConfigWidget->setState(conf);
    updatePoly();
    updateSharedProperties();
  }

  loadStoredPolyInformation(dataSet);

  int mapType = 0;
  if (dataSet.get("viewType", mapType)) {
    // ensure compatibility with previous version
    switch (mapType) {
    case OpenTopoMap: // 1
      mapType = EsriSatellite;
      break;

    case EsriStreetMap: // 2
      mapType = EsriTopoMap;
      break;

    case EsriTopoMap: // 3
      mapType = EsriLightGrayCanvas;
      break;

    case EsriNatGeoMap: // 4
      mapType = LeafletCustomTileLayer;
      break;

    case EsriSatellite: // 5
      mapType = Polygon;
      break;

    case EsriLightGrayCanvas: // 6
      mapType = Globe;

    default:
      break;
    }
  } else
    dataSet.get("mapType", mapType);

  _mapType = MapType(mapType);

  string mapTypeName = QStringToTlpString(getViewName(_mapType));

  mapTypeChanged(mapTypeName.c_str());

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
  } else {
    geolocalisationConfigWidget->setAddressGeoLocMethod();
    geoViewGraphicsView->createLayoutWithAddresses();
  }

  GlGraphComposite *graphComposite =
      geoViewGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite();
  GlGraphRenderingParameters rp = graphComposite->getRenderingParameters();

  if (dataSet.exists("renderingParameters")) {
    DataSet renderingParameters;
    dataSet.get("renderingParameters", renderingParameters);
    rp.setParameters(renderingParameters);
    string s;

    if (renderingParameters.get("elementsOrderingPropertyName", s) && !s.empty()) {
      rp.setElementOrderingProperty(dynamic_cast<tlp::NumericProperty *>(graph()->getProperty(s)));
    }
  } else
    // same default initialization as NodeLinkDiagramComponent
    NodeLinkDiagramComponent::initRenderingParameters(&rp);

  graphComposite->setRenderingParameters(rp);
  sceneConfigurationWidget->resetChanges();

  View::setState(dataSet);

  mapCenterLatitudeInit = 44.8084;
  mapCenterLongitudeInit = -40;
  mapZoomInit = 3;
  dataSet.get("mapCenterLatitude", mapCenterLatitudeInit);
  dataSet.get("mapCenterLongitude", mapCenterLongitudeInit);
  dataSet.get("mapZoom", mapZoomInit);

  QTimer::singleShot(1500, this, SLOT(initMap()));
}

void GeographicView::initMap() {
  geoViewGraphicsView->getLeafletMapsPage()->setMapCenter(mapCenterLatitudeInit,
                                                          mapCenterLongitudeInit);
  geoViewGraphicsView->getLeafletMapsPage()->setCurrentZoom(mapZoomInit);
}

DataSet GeographicView::state() const {
  DataSet dataSet = View::state();
  DataSet configurationWidget = geoViewConfigWidget->state();
  dataSet.set("configurationWidget", configurationWidget);
  dataSet.set("mapType", int(_mapType));
  pair<double, double> mapCenter = geoViewGraphicsView->getLeafletMapsPage()->getCurrentMapCenter();
  dataSet.set("mapCenterLatitude", mapCenter.first);
  dataSet.set("mapCenterLongitude", mapCenter.second);
  dataSet.set("mapZoom", geoViewGraphicsView->getLeafletMapsPage()->getCurrentMapZoom());
  dataSet.set("renderingParameters", geoViewGraphicsView->getGlMainWidget()
                                         ->getScene()
                                         ->getGlGraphComposite()
                                         ->getRenderingParameters()
                                         .getParameters());

  saveStoredPolyInformation(dataSet);

  std::string latitudePropName = geolocalisationConfigWidget->getLatitudeGraphPropertyName();
  std::string longitudePropName = geolocalisationConfigWidget->getLongitudeGraphPropertyName();

  if (latitudePropName != longitudePropName && graph()->existProperty(latitudePropName) &&
      graph()->existProperty(longitudePropName)) {
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
    geoViewGraphicsView->createLayoutWithAddresses(
        geolocalisationConfigWidget->getAddressGraphPropertyName(),
        geolocalisationConfigWidget->createLatAndLngProperties(),
        geolocalisationConfigWidget->resetLatAndLngValues(),
        geolocalisationConfigWidget->automaticChoice());

    if (geolocalisationConfigWidget->createLatAndLngProperties()) {
      geolocalisationConfigWidget->setGraph(graph());
      geolocalisationConfigWidget->setLatLngGeoLocMethod("latitude", "longitude");
    }
  } else {
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
  geoViewGraphicsView->switchMapType();
}

void GeographicView::centerView() {
  geoViewGraphicsView->centerView();
}

void GeographicView::centerOnNode() {
  geoViewGraphicsView->centerMapOnNode(_nodeUnderMouse);
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

QList<QWidget *> GeographicView::configurationWidgets() const {
  return QList<QWidget *>() << geolocalisationConfigWidget << geoViewConfigWidget
                            << sceneConfigurationWidget << sceneLayersConfigurationWidget;
}

void GeographicView::applySettings() {
  updateSharedProperties();
  updatePoly();
}

void GeographicView::updateSharedProperties() {
  GlGraphInputData *inputData =
      geoViewGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();

  if (useSharedLayoutProperty != geoViewConfigWidget->useSharedLayoutProperty()) {
    useSharedLayoutProperty = geoViewConfigWidget->useSharedLayoutProperty();

    if (useSharedLayoutProperty)
      geoViewGraphicsView->setGeoLayout(graph()->getProperty<LayoutProperty>("viewLayout"));
    else
      geoViewGraphicsView->setGeoLayout(new LayoutProperty(graph()));
  }

  if (useSharedShapeProperty != geoViewConfigWidget->useSharedShapeProperty()) {
    useSharedShapeProperty = geoViewConfigWidget->useSharedShapeProperty();

    if (useSharedShapeProperty)
      geoViewGraphicsView->setGeoShape(graph()->getProperty<IntegerProperty>("viewShape"));
    else
      geoViewGraphicsView->setGeoShape(new IntegerProperty(graph()));
  }

  if (useSharedSizeProperty != geoViewConfigWidget->useSharedSizeProperty()) {
    useSharedSizeProperty = geoViewConfigWidget->useSharedSizeProperty();

    if (useSharedSizeProperty)
      geoViewGraphicsView->setGeoSizes(graph()->getProperty<SizeProperty>("viewSize"));
    else
      geoViewGraphicsView->setGeoSizes(new SizeProperty(graph()));
  }

  inputData->getGlVertexArrayManager()->setHaveToComputeAll(true);
}

void GeographicView::updatePoly(bool force) {
  if (geoViewConfigWidget->polyOptionsChanged() || force) {
    switch (geoViewConfigWidget->polyFileType()) {
    case GeographicViewConfigWidget::CsvFile: {
      geoViewGraphicsView->loadCsvFile(geoViewConfigWidget->getCsvFile());
      break;
    }

    case GeographicViewConfigWidget::PolyFile: {
      geoViewGraphicsView->loadPolyFile(geoViewConfigWidget->getPolyFile());
      break;
    }

    default: {
      geoViewGraphicsView->loadDefaultMap();
      break;
    }
    }
  }
}

void GeographicView::loadStoredPolyInformation(const DataSet &dataset) {
  if (dataset.exists("polygons")) {
    DataSet polyConf;
    dataset.get("polygons", polyConf);
    GlComposite *composite = geoViewGraphicsView->getPolygon();
    const map<string, GlSimpleEntity *> &entities = composite->getGlEntities();

    for (map<string, GlSimpleEntity *>::const_iterator it = entities.begin(); it != entities.end();
         ++it) {
      DataSet entityData;

      if (polyConf.exists((*it).first)) {
        polyConf.get((*it).first, entityData);
        Color color;
        entityData.get("color", color);
        static_cast<GlComplexPolygon *>((*it).second)->setFillColor(color);
        entityData.get("outlineColor", color);
        static_cast<GlComplexPolygon *>((*it).second)->setOutlineColor(color);
      }
    }
  }
}

void GeographicView::saveStoredPolyInformation(DataSet &dataset) const {
  GlComposite *composite = geoViewGraphicsView->getPolygon();
  DataSet polyConf;
  const map<string, GlSimpleEntity *> &entities = composite->getGlEntities();

  for (map<string, GlSimpleEntity *>::const_iterator it = entities.begin(); it != entities.end();
       ++it) {
    DataSet entityData;
    entityData.set("color", static_cast<GlComplexPolygon *>((*it).second)->getFillColor());
    entityData.set("outlineColor",
                   static_cast<GlComplexPolygon *>((*it).second)->getOutlineColor());
    polyConf.set((*it).first, entityData);
  }

  dataset.set("polygons", polyConf);
}

QGraphicsItem *GeographicView::centralItem() const {
  return geoViewGraphicsView->getPlaceHolderItem();
}

void GeographicView::registerTriggers() {
  clearRedrawTriggers();

  if (graph() == nullptr)
    return;

  addRedrawTrigger(
      geoViewGraphicsView->getGlMainWidget()->getScene()->getGlGraphComposite()->getGraph());
  auto &properties = geoViewGraphicsView->getGlMainWidget()
                         ->getScene()
                         ->getGlGraphComposite()
                         ->getInputData()
                         ->properties();

  for (auto prop : properties) {
    addRedrawTrigger(prop);
  }
}

QPixmap GeographicView::snapshot(const QSize &size) const {

  // hide the graphics widget used to configure the view
  // before taking a snapshot
  QList<QGraphicsProxyWidget *> gWidgetsToRestore;

  for (auto item : geoViewGraphicsView->scene()->items()) {
    QGraphicsProxyWidget *gWidget = dynamic_cast<QGraphicsProxyWidget *>(item);

    if (gWidget && gWidget->isVisible()) {
      gWidget->hide();
      gWidgetsToRestore.push_back(gWidget);
    }
  }

  int width = geoViewGraphicsView->width();
  int height = geoViewGraphicsView->height();

  QImage snapshotImage(width, height, QImage::Format_RGB32);
  QPainter painter(&snapshotImage);
  geoViewGraphicsView->scene()->render(&painter);
  painter.end();

  // restore the graphics widgets previously hidden
  for (auto gWidget : gWidgetsToRestore) {
    gWidget->show();
  }

  return QPixmap::fromImage(snapshotImage)
      .scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

GeographicView::MapType GeographicView::getMapType(const QString &name) {
  for (unsigned int i = 0; i < mapLayers.size(); ++i) {
    if (name == mapLayers[i].name)
      return MapType(i);
  }
  return OpenStreetMap;
}

const char *GeographicView::getViewName(GeographicView::MapType mapType) {
  if (mapType < 0 || mapType >= mapLayers.size())
    return mapLayers[OpenStreetMap].name;
  return mapLayers[mapType].name;
}

const vector<LeafletMaps::MapLayer> &GeographicView::getMapLayers() {
  return mapLayers;
}

PLUGIN(GeographicView)
