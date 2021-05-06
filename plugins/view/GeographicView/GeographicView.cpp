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

static const std::vector<LeafletMaps::MapLayer> mapLayers =
{
 // list map layers according the ViewType enum ordering
 // OpenStreetMap is the default one
 LeafletMaps::MapLayer("OpenStreetMap",
		       "https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png",
		       "&copy; <a href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a> contributors",
		       19),
 // OpenTopoMap
 LeafletMaps::MapLayer("OpenTopoMap",
		       "https://{s}.tile.opentopomap.org/{z}/{x}/{y}.png",
		       "Map data: {attribution.OpenStreetMap}, <a href=\"http://viewfinderpanoramas.org\">SRTM</a> | Map style: &copy; <a href=\"https://opentopomap.org\">OpenTopoMap</a> (<a href=\"https://creativecommons.org/licenses/by-sa/3.0/\">CC-BY-SA</a>)",
		       17),
 // Esri World Street Map
 LeafletMaps::MapLayer("Esri World Street Map",
		       "https://server.arcgisonline.com/ArcGIS/rest/services/World_Street_Map/MapServer/tile/{z}/{y}/{x}",
		       "Tiles &copy; Esri &mdash; Source: Esri, DeLorme, NAVTEQ, USGS, Intermap, iPC, NRCAN, Esri Japan, METI, Esri China (Hong Kong), Esri (Thailand), TomTom, 2012"),
 // Esri Topographic Map
 LeafletMaps::MapLayer("Esri Topographic Map",
		       "https://server.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/{z}/{y}/{x}",
		       "Tiles &copy; Esri &mdash; Esri, DeLorme, NAVTEQ, TomTom, Intermap, iPC, USGS, FAO, NPS, NRCAN, GeoBase, Kadaster NL, Ordnance Survey, Esri Japan, METI, Esri China (Hong Kong), and the GIS User Community"),
 // Esri National Geographic Map
 LeafletMaps::MapLayer("Esri National Geographic Map",
		       "https://server.arcgisonline.com/ArcGIS/rest/services/NatGeo_World_Map/MapServer/tile/{z}/{y}/{x}",
		       "Tiles &copy; Esri &mdash; National Geographic, Esri, DeLorme, NAVTEQ, UNEP-WCMC, USGS, NASA, ESA, METI, NRCAN, GEBCO, NOAA, iPC",
		       16),
 // Esri World Imagery
 LeafletMaps::MapLayer("Esri World Imagery",
		       "https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}",
		       "Tiles &copy; Esri &mdash; Source: Esri, i-cubed, USDA, USGS, AEX, GeoEye, Getmapping, Aerogrid, IGN, IGP, UPR-EGP, and the GIS User Community"),
 // Esri Light Gray Canvas
 LeafletMaps::MapLayer("Esri Light Gray Canvas",
		       "https://server.arcgisonline.com/ArcGIS/rest/services/Canvas/World_Light_Gray_Base/MapServer/tile/{z}/{y}/{x}",
		       "Tiles &copy; Esri &mdash; Esri, DeLorme, NAVTEQ",
		       16),
 // Esri Dark Gray Canvas
 LeafletMaps::MapLayer("Esri Dark Gray Canvas",
		       "https://server.arcgisonline.com/ArcGIS/rest/services/Canvas/World_Dark_Gray_Base/MapServer/tile/{z}/{y}/{x}",
		       "Tiles &copy; Esri &mdash; Esri, DeLorme, NAVTEQ",
		       16),
 // CartoDB Voyager
 LeafletMaps::MapLayer("CartoDB Map",
		       "https://{s}.basemaps.cartocdn.com/rastertiles/voyager/{z}/{x}/{y}{r}.png",
		       "&copy; <a href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a> contributors &copy; <a href=\"https://carto.com/attributions\">CARTO</a>",
		       19),
 // Carto DB Light Gray Map
 LeafletMaps::MapLayer("CartoDB Light Map",
		       "https://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}{r}.png",
		       "&copy; <a href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a> contributors &copy; <a href=\"https://carto.com/attributions\">CARTO</a>",
		       19),
 // Carto DB Dark Gray Map
 LeafletMaps::MapLayer("CartoDB Dark Map",
		       "https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png",
		       "&copy; <a href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a> contributors &copy; <a href=\"https://carto.com/attributions\">CARTO</a>",
		       19),
 // Wikimedia
 LeafletMaps::MapLayer("Wikimedia Map",
		       "https://maps.wikimedia.org/osm-intl/{z}/{x}/{y}{r}.png",
		       "<a href=\"https://wikimediafoundation.org/wiki/Maps_Terms_of_Use\">Wikimedia</a>"),
 // the one below are not really map layer
 LeafletMaps::MapLayer("Leaflet Custom Tile Layer"),
 LeafletMaps::MapLayer("Polygon"),
 LeafletMaps::MapLayer("Globe")
};

GeographicView::GeographicView(PluginContext *)
    : geoViewGraphicsView(nullptr), geoViewConfigWidget(nullptr),
      geolocalisationConfigWidget(nullptr), sceneConfigurationWidget(nullptr),
      sceneLayersConfigurationWidget(nullptr), centerOnNodeAction(nullptr),
      showConfPanelAction(nullptr), useSharedLayoutProperty(true), useSharedSizeProperty(true),
      useSharedShapeProperty(true), mapCenterLatitudeInit(0), mapCenterLongitudeInit(0),
      mapZoomInit(0), _viewActionsManager(nullptr) {
  _viewType = OpenStreetMap;
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
  connect(geoViewConfigWidget, SIGNAL(mapToPolygonSignal()), this, SLOT(mapToPolygon()));

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
  setState(DataSet());

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

void GeographicView::viewTypeChanged(QString viewTypeName) {
  QComboBox *comboBox = geoViewGraphicsView->getViewTypeComboBox();

  if (comboBox == nullptr)
    return;

  disconnect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(viewTypeChanged(QString)));

  _viewType = ViewType(comboBox->currentIndex() - 2);

  geoViewGraphicsView->switchViewType();

  comboBox->setCurrentIndex(0);
  comboBox->setItemText(0, viewTypeName);

  connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(viewTypeChanged(QString)));
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

  if (dataSet.exists("viewType")) {
    int viewType = 0;
    dataSet.get("viewType", viewType);
    _viewType = ViewType(viewType);
  }

  string viewTypeName = QStringToTlpString(getViewNameFromType(_viewType));

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

  if (dataSet.exists("mapCenterLatitude")) {

    dataSet.get("mapCenterLatitude", mapCenterLatitudeInit);
    dataSet.get("mapCenterLongitude", mapCenterLongitudeInit);
    dataSet.get("mapZoom", mapZoomInit);

    QTimer::singleShot(1500, this, SLOT(initMap()));
  }
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
  dataSet.set("viewType", int(_viewType));
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
  geoViewGraphicsView->switchViewType();
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

GeographicView::ViewType GeographicView::getViewTypeFromName(const QString &name) const {
  for (unsigned int i = 0; i < mapLayers.size(); ++i) {
    if (name == mapLayers[i].name)
      return  ViewType(i);
  }
  return OpenStreetMap;
}

const char *GeographicView::getViewNameFromType(GeographicView::ViewType viewType) const {
  if (viewType < 0 || viewType >= mapLayers.size())
    return mapLayers[OpenStreetMap].name;
  return mapLayers[viewType].name;
}

const vector<LeafletMaps::MapLayer> &GeographicView::getMapLayers() {
  return mapLayers;
}

PLUGIN(GeographicView)
