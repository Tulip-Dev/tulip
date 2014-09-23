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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlRect.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlLayer.h>
#include <tulip/GlProgressBar.h>
#include <tulip/GlLabel.h>
#include <tulip/LayoutProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/ForEach.h>
#include <tulip/tulipconf.h>

#include "PixelOrientedInteractors.h"
#include "PixelOrientedView.h"

#include "../utils/ViewGraphPropertiesSelectionWidget.h"

#include <QApplication>
#include <QMenu>
#include <QGraphicsView>

using namespace std;
using namespace pocore;
using namespace tlp;

static void setGraphView (tlp::GlGraphComposite *glGraph, bool displayNodes) {
  tlp::GlGraphRenderingParameters param = glGraph->getRenderingParameters ();
  param.setAntialiasing (true);
  param.setViewNodeLabel (true);
  param.setFontsType (2);
  param.setSelectedNodesStencil(1);
  param.setNodesStencil(0xFFFF);
  param.setNodesLabelStencil(0xFFFF);
  param.setDisplayEdges(false);
  param.setDisplayNodes(displayNodes);
  param.setDisplayMetaNodes(displayNodes);
  glGraph->setRenderingParameters (param);
}

const string propertiesTypes[] = {"double", "int"};
const unsigned int nbPropertiesTypes = sizeof(propertiesTypes) / sizeof(string);
const vector<string> propertiesTypesFilter(propertiesTypes, propertiesTypes + nbPropertiesTypes);

namespace tlp {

PLUGIN(PixelOrientedView)

PixelOrientedView::PixelOrientedView(const PluginContext *) :
  pixelOrientedGraph(NULL), graphComposite(NULL), mainLayer(NULL),
  overviewsComposite(NULL), optionsWidget(NULL), propertiesSelectionWidget(NULL),
  pixelOrientedMediator(NULL),
  lastNbNodes(0), overviewWidth(0), overviewHeight(0), minWidth(0), refSize(0), hilbertLayout(NULL), squareLayout(NULL), spiralLayout(new SpiralLayout()),
  zorderLayout(NULL), tulipNodeColorMapping(NULL), smallMultiplesView(true), sceneRadiusBak(0.0), zoomFactorBak(0.0), detailViewLabel(NULL), detailOverview(NULL),
  newGraphSet(false), smallMultiplesNeedUpdate(false), lastViewWindowWidth(0), lastViewWindowHeight(0), center(false), interactorsActivated(false),isConstruct(false)  {
}

PixelOrientedView::~PixelOrientedView() {
  if(isConstruct) {
    destroyData();

    if (mainLayer->findGlEntity("graph")) {
      GlGraphComposite *lastGraphComposite = (GlGraphComposite *)mainLayer->findGlEntity("graph");
      Graph *theGraph = lastGraphComposite->getInputData()->getGraph();
      theGraph->removeListener(lastGraphComposite);
    }

    delete spiralLayout;

    if (hilbertLayout != NULL) {
      delete hilbertLayout;
      delete squareLayout;
      delete zorderLayout;
    }

    if (tulipNodeColorMapping != NULL) {
      delete tulipNodeColorMapping;
    }

    delete pixelOrientedMediator;
    delete propertiesSelectionWidget;
    delete optionsWidget;
  }
}

QList<QWidget *> PixelOrientedView::configurationWidgets() const {
  return QList<QWidget *>() << propertiesSelectionWidget << optionsWidget ;
}

void PixelOrientedView::initGlWidget() {
  mainLayer = getGlMainWidget()->getScene()->getLayer("Main");

  if (mainLayer == NULL) {
    mainLayer = new GlLayer("Main");
    getGlMainWidget()->getScene()->addExistingLayer(mainLayer);
  }

  if (mainLayer->findGlEntity("graph")) {
    GlGraphComposite *lastGraphComposite = (GlGraphComposite *)mainLayer->findGlEntity("graph");
    Graph *theGraph = lastGraphComposite->getInputData()->getGraph();

    if(theGraph)
      theGraph->removeListener(lastGraphComposite);
  }

  if (overviewsComposite != NULL) {
    overviewsComposite->reset(true);
    detailOverview = NULL;
  }

  mainLayer->getComposite()->reset(true);

  overviewsComposite = new GlComposite();
  mainLayer->addGlEntity(overviewsComposite, "overview composite");

  detailViewLabel = NULL;

  graphComposite = new GlGraphComposite(pixelOrientedGraph);
  mainLayer->addGlEntity(graphComposite, "graph");
  setGraphView(graphComposite, false);
}

void PixelOrientedView::setColorFunction(pocore::ColorFunction *colorFunction) {
  pixelOrientedMediator->setColorFunction(colorFunction);
}

void PixelOrientedView::setLayoutFunction(pocore::LayoutFunction *layoutFunction) {
  pixelOrientedMediator->setLayoutFunction(layoutFunction);
}

void PixelOrientedView::setState(const DataSet &dataSet) {

  if(!isConstruct) {
    isConstruct=true;
    propertiesSelectionWidget = new ViewGraphPropertiesSelectionWidget();
    propertiesSelectionWidget->enableEdgesButton(false);
    connect(propertiesSelectionWidget, SIGNAL(applySettingsSignal()), this, SLOT(applySettings()));
    pixelOrientedMediator = new PixelOrientedMediator(spiralLayout, NULL);
    optionsWidget = new PixelOrientedOptionsWidget();
    connect(optionsWidget, SIGNAL(applySettingsSignal()), this, SLOT(applySettings()));
    layoutFunctionsMap["Spiral"] = spiralLayout;
    setOverviewVisible(true);
  }

  Graph *lastGraph = this->pixelOrientedGraph;

  if (graph() == NULL) {
    this->pixelOrientedGraph = graph();
    destroyData();
    initGlWidget();
    lastNbNodes = 0;

    if (lastGraph != NULL && lastGraph != pixelOrientedGraph) {
      overviewsComposite->reset(true);
      overviewsMap.clear();
      detailOverview = NULL;
      newGraphSet = true;
    }

    foreach (tlp::Observable* obs, triggers()) {
      removeRedrawTrigger(obs);
    }

    propertiesSelectionWidget->setWidgetParameters(NULL, propertiesTypesFilter);

    center = true;
    draw();
    center = false;

    return;
  }

  if (tulipNodeColorMapping != NULL && this->pixelOrientedGraph != graph()) {
    delete tulipNodeColorMapping;
    tulipNodeColorMapping = NULL;
  }

  if (tulipNodeColorMapping == NULL) {
    tulipNodeColorMapping = new TulipNodeColorMapping(pixelOrientedGraph);
    pixelOrientedMediator->setColorFunction(tulipNodeColorMapping);
  }

  if (lastGraph == NULL || lastGraph != graph()) {
    this->pixelOrientedGraph = graph();
    initGlWidget();
    destroyData();
    lastNbNodes = pixelOrientedGraph->numberOfNodes();
    initLayoutFunctions();

    if (lastGraph != NULL && lastGraph != pixelOrientedGraph) {
      overviewsComposite->reset(true);
      overviewsMap.clear();
      detailOverview = NULL;
      newGraphSet = true;
    }
  }

  if (lastGraph != NULL && lastGraph->getRoot() != pixelOrientedGraph->getRoot()) {
    overviewGenMap.clear();
    detailOverviewPropertyName = "";
  }

  dataSet.get("lastViewWindowWidth", lastViewWindowWidth);
  dataSet.get("lastViewWindowHeight", lastViewWindowHeight);

  propertiesSelectionWidget->setWidgetParameters(pixelOrientedGraph, propertiesTypesFilter);

  Color backgroundColor(255,255,255);
  dataSet.get("background color", backgroundColor);
  optionsWidget->setBackgroundColor(backgroundColor);

  bool haveSelectedGraphProperties=false;

  DataSet selectedGraphPropertiesDataSet;

  if (dataSet.get("selected graph properties", selectedGraphPropertiesDataSet)) {
    haveSelectedGraphProperties=true;
    int i = 0;
    ostringstream oss;
    oss << i;

    while (selectedGraphPropertiesDataSet.exist(oss.str())) {
      string propertyName;
      selectedGraphPropertiesDataSet.get(oss.str(), propertyName);
      selectedGraphProperties.push_back(propertyName);
      oss.str("");
      oss << ++i;
    }

    propertiesSelectionWidget->setSelectedProperties(selectedGraphProperties);

    for (size_t i = 0 ; i < selectedGraphProperties.size() ; ++i) {
      bool overviewGenerated = false;
      dataSet.get(selectedGraphProperties[i], overviewGenerated);
      overviewGenMap[selectedGraphProperties[i]] = overviewGenerated;
    }

    string layoutName = "";
    dataSet.get("layout", layoutName);

    if (layoutName != "") {
      optionsWidget->setLayoutType(layoutName);
    }
  }

  string detailOverviewName;
  dataSet.get("detail overview  name", detailOverviewName);

  if (detailOverviewName != "") {
    switchFromSmallMultiplesToDetailView(overviewsMap[detailOverviewName]);
  }

  center = true;

  if(haveSelectedGraphProperties) {
    updateOverviews(true);
    getGlMainWidget()->centerScene();
  }

  draw();
  center = false;

  drawOverview(true);
  registerTriggers();
}

DataSet PixelOrientedView::state() const {
  DataSet dataSet;
  DataSet selectedGraphPropertiesDataSet;

  for (size_t i = 0 ; i < selectedGraphProperties.size() ; ++i) {
    ostringstream oss;
    oss << i;
    selectedGraphPropertiesDataSet.set(oss.str(), selectedGraphProperties[i]);
  }

  dataSet.set("selected graph properties", selectedGraphPropertiesDataSet);
  dataSet.set("layout", optionsWidget->getLayoutType());

  map<string, bool> tmpOverviewGenMap=overviewGenMap;

  for (size_t i = 0 ; i < selectedGraphProperties.size() ; ++i) {
    dataSet.set(selectedGraphProperties[i], tmpOverviewGenMap[selectedGraphProperties[i]]);
  }

  dataSet.set("lastViewWindowWidth",  getGlMainWidget()->width());
  dataSet.set("lastViewWindowHeight",  getGlMainWidget()->height());
  dataSet.set("detail overview  name", detailOverviewPropertyName);
  dataSet.set("background color", optionsWidget->getBackgroundColor());
  return dataSet;
}

void PixelOrientedView::graphChanged(Graph *) {
  setState(DataSet());
}

void PixelOrientedView::initLayoutFunctions() {
  if (hilbertLayout != NULL) {
    delete hilbertLayout;
    delete squareLayout;
    delete zorderLayout;

  }

  hilbertLayout = new HilbertLayout((int) ceil(log(pixelOrientedGraph->numberOfNodes())/ log(4)));
  squareLayout = new SquareLayout((int) ceil(sqrt(pixelOrientedGraph->numberOfNodes())));
  zorderLayout = new ZorderLayout((int) ceil(log(pixelOrientedGraph->numberOfNodes())/ log(4)));
  layoutFunctionsMap["Zorder"] = zorderLayout;
  layoutFunctionsMap["Peano"] = hilbertLayout;
  layoutFunctionsMap["Square"] = squareLayout;
}

void PixelOrientedView::initPixelView() {

  if (lastNbNodes != pixelOrientedGraph->numberOfNodes()) {
    initLayoutFunctions();
  }

  Color backgroundColor = optionsWidget->getBackgroundColor();
  Color textColor = getTextColor();

  setLayoutFunction(layoutFunctionsMap[optionsWidget->getLayoutType()]);

  overviewWidth = MIN_IMAGE_WIDTH;
  overviewHeight = MIN_IMAGE_HEIGHT;

  minWidth = ((unsigned int) floor(sqrt((double)pixelOrientedGraph->numberOfNodes()))) + 1;

  while (minWidth > overviewWidth) {
    overviewWidth *= 2;
    overviewHeight *= 2;
  }

  unsigned int labelHeight = overviewHeight / 4;
  float spaceBetweenOverviews = overviewWidth / 10;

  pixelOrientedMediator->setImageSize(overviewWidth, overviewHeight);
  pixelOrientedMediator->setScreenFunctionsParameters(1, 0, 0, 0);

  overviewsComposite->reset(false);

  selectedGraphProperties = propertiesSelectionWidget->getSelectedGraphProperties();

  if (selectedGraphProperties.size()  == 0) return;

  float squareRoot = sqrt(double(selectedGraphProperties.size()));
  const unsigned int N =  (unsigned int) squareRoot + (fmod((float) selectedGraphProperties.size(), squareRoot) == 0. ? 0 : 1);

  for (size_t i = 0 ; i < selectedGraphProperties.size() ; ++i) {

    if (dataMap.find(selectedGraphProperties[i]) == dataMap.end()) {
      dataMap[selectedGraphProperties[i]] = new TulipGraphDimension(pixelOrientedGraph, selectedGraphProperties[i]);
    }
    else {
      dataMap[selectedGraphProperties[i]]->updateNodesRank();
    }

    unsigned int row = i / N;
    unsigned int col = i % N;

    Coord overviewBLCorner = Coord(col * (overviewWidth + spaceBetweenOverviews), -(labelHeight + row * (labelHeight + overviewHeight + spaceBetweenOverviews)), 0);
    ostringstream oss;
    oss << "pixel oriented overview for dimension " << selectedGraphProperties[i];

    if (overviewsMap.find(selectedGraphProperties[i]) == overviewsMap.end()) {
      PixelOrientedOverview *pixelOrientedOverview = new PixelOrientedOverview(dataMap[selectedGraphProperties[i]], pixelOrientedMediator, overviewBLCorner, selectedGraphProperties[i], backgroundColor, textColor);
      pixelOrientedOverview->computePixelView();
      overviewsMap[selectedGraphProperties[i]] = pixelOrientedOverview;

      if (overviewGenMap.find(selectedGraphProperties[i]) == overviewGenMap.end()) {
        overviewGenMap[selectedGraphProperties[i]] = false;
      }
    }
    else {
      overviewsMap[selectedGraphProperties[i]]->setBLCorner(overviewBLCorner);
      overviewsMap[selectedGraphProperties[i]]->setBackgroundColor(backgroundColor);
      overviewsMap[selectedGraphProperties[i]]->setTextColor(textColor);
    }

    overviewsComposite->addGlEntity(overviewsMap[selectedGraphProperties[i]], oss.str());
  }

  if (detailOverviewPropertyName != "") {
    detailOverview = overviewsMap[detailOverviewPropertyName];
  }

  lastNbNodes = pixelOrientedGraph->numberOfNodes();
}

void PixelOrientedView::destroyOverviewsIfNeeded() {
  vector<string> propertiesToRemove;

  for (size_t i = 0 ; i < selectedGraphProperties.size() ; ++i) {
    if (!pixelOrientedGraph->existProperty(selectedGraphProperties[i])) {
      if (overviewsMap[selectedGraphProperties[i]] == detailOverview) {
        detailOverview = NULL;
        detailOverviewPropertyName = "";
      }

      delete overviewsMap[selectedGraphProperties[i]];
      overviewsMap.erase(selectedGraphProperties[i]);
      delete dataMap[selectedGraphProperties[i]];
      dataMap.erase(selectedGraphProperties[i]);
      propertiesToRemove.push_back(selectedGraphProperties[i]);
    }
  }

  for (size_t i = 0 ; i < propertiesToRemove.size() ; ++i) {
    selectedGraphProperties.erase(remove(selectedGraphProperties.begin(), selectedGraphProperties.end(), propertiesToRemove[i]), selectedGraphProperties.end());
  }
}

void PixelOrientedView::destroyData() {
  for (map<string, TulipGraphDimension *>::iterator it = dataMap.begin() ; it != dataMap.end(); ++it) {
    delete it->second;
  }

  dataMap.clear();
}

void PixelOrientedView::addEmptyViewLabel() {
  Color textColor = getTextColor();
  GlLabel *noDimsLabel = new GlLabel(Coord(0,0,0), Size(200,200), textColor);
  noDimsLabel->setText(ViewName::PixelOrientedViewName);
  mainLayer->addGlEntity(noDimsLabel, "no dimensions label");
  GlLabel *noDimsLabel1 = new GlLabel(Coord(0,-50, 0), Size(400,200), textColor);
  noDimsLabel1->setText("No graph properties selected.");
  mainLayer->addGlEntity(noDimsLabel1, "no dimensions label 1");
  GlLabel *noDimsLabel2 = new GlLabel(Coord(0,-100,0), Size(700,200), textColor);
  noDimsLabel2->setText("Go to the \"Properties\" tab in top right corner.");
  mainLayer->addGlEntity(noDimsLabel2, "no dimensions label 2");
}

void PixelOrientedView::removeEmptyViewLabel() {
  GlSimpleEntity *noDimsLabel = mainLayer->findGlEntity("no dimensions label");
  GlSimpleEntity *noDimsLabel1 = mainLayer->findGlEntity("no dimensions label 1");
  GlSimpleEntity *noDimsLabel2 = mainLayer->findGlEntity("no dimensions label 2");

  if (noDimsLabel != NULL) {
    mainLayer->deleteGlEntity(noDimsLabel);
    delete noDimsLabel;
    mainLayer->deleteGlEntity(noDimsLabel1);
    delete noDimsLabel1;
    mainLayer->deleteGlEntity(noDimsLabel2);
    delete noDimsLabel2;
  }
}

void PixelOrientedView::generatePixelOverview(PixelOrientedOverview *pixelOverview, GlMainWidget *glWidget) {
  pixelOverview->computePixelView(glWidget);
  overviewGenMap[pixelOverview->getDimensionName()] = true;
}

void PixelOrientedView::draw() {
  if (pixelOrientedGraph != NULL) {

    getGlMainWidget()->getScene()->setBackgroundColor(optionsWidget->getBackgroundColor());

    unsigned int lastNbDimensionsSelected = selectedGraphProperties.size();

    destroyOverviewsIfNeeded();

    if (pixelOrientedGraph->numberOfNodes() >= 2) // We stop the creation of pixel overviews if we have less than 2 nodes as we need at least two to compute sizes for overview
      initPixelView();
    else
      selectedGraphProperties.clear();


    if (selectedGraphProperties.size() == 0) {
      if (!smallMultiplesView) {
        switchFromDetailViewToSmallMultiples();
      }

      removeEmptyViewLabel();
      addEmptyViewLabel();
      getGlMainWidget()->getScene()->centerScene();
      getGlMainWidget()->draw();
      return;
    }
    else {
      removeEmptyViewLabel();
    }

    if (lastNbDimensionsSelected != selectedGraphProperties.size()) {
      center = true;
    }

    if (!smallMultiplesView && lastNbDimensionsSelected == 1 && selectedGraphProperties.size() > 1) {
      switchFromDetailViewToSmallMultiples();
    }
    else if (selectedGraphProperties.size() == 1) {
      getGlMainWidget()->makeCurrent();
      overviewsMap[selectedGraphProperties[0]]->computePixelView();
      overviewGenMap[selectedGraphProperties[0]] = true;
      switchFromSmallMultiplesToDetailView(overviewsMap[selectedGraphProperties[0]]);
    }
    else if (!smallMultiplesView && detailOverview != NULL) {
      detailOverview->computePixelView();
      smallMultiplesNeedUpdate = true;

      if (newGraphSet) {
        switchFromSmallMultiplesToDetailView(detailOverview);
        newGraphSet = false;
      }
    }
    else if (!smallMultiplesView && detailOverview == NULL) {
      switchFromDetailViewToSmallMultiples();
    }
    else {
      getGlMainWidget()->makeCurrent();
      updateOverviews(true);
    }

    if (!smallMultiplesView && detailViewLabel != NULL) {
      detailViewLabel->setColor(getTextColor());
    }

    if (center) {
      centerView();
      center = false;
    }
    else {
      getGlMainWidget()->draw();
    }
  }
  else {
    getGlMainWidget()->draw();
  }
}

void PixelOrientedView::refresh() {
  getGlMainWidget()->redraw();
}

void PixelOrientedView::init() {
  emit drawNeeded();
}

Color PixelOrientedView::getTextColor() const {
  Color backgroundColor = optionsWidget->getBackgroundColor();

  Color textColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    textColor = Color(255,255,255);
  }
  else {
    textColor = Color(0,0,0);
  }

  return textColor;
}

void PixelOrientedView::centerView(bool) {
  if (!getGlMainWidget()->isVisible()) {
    if (lastViewWindowWidth != 0 && lastViewWindowHeight != 0) {
      getGlMainWidget()->getScene()->ajustSceneToSize(lastViewWindowWidth, lastViewWindowHeight);
    }
    else {
      getGlMainWidget()->getScene()->centerScene();
    }
  }
  else {
    getGlMainWidget()->getScene()->ajustSceneToSize(getGlMainWidget()->width(), getGlMainWidget()->height());
  }

  // we apply a zoom factor to preserve a 50 px margin width
  // to ensure the scene will not be drawn under the configuration tabs title
  float glWidth = (float) graphicsView()->width();
  getGlMainWidget()->getScene()->zoomFactor((glWidth - 50)/ glWidth);
  getGlMainWidget()->draw();
}

void PixelOrientedView::updateOverviews(const bool updateAll) {

  if (smallMultiplesView) {
    mainLayer->deleteGlEntity(overviewsComposite);
  }
  else {
    setGraphView(graphComposite, false);
    mainLayer->deleteGlEntity(detailViewLabel);
  }

  unsigned int nbOverviews = selectedGraphProperties.size();
  unsigned currentStep = 0;

  double sceneRadiusBak = getGlMainWidget()->getScene()->getGraphCamera().getSceneRadius();
  double zoomFactorBak = getGlMainWidget()->getScene()->getGraphCamera().getZoomFactor();
  Coord eyesBak = getGlMainWidget()->getScene()->getGraphCamera().getEyes();
  Coord centerBak = getGlMainWidget()->getScene()->getGraphCamera().getCenter();
  Coord upBak = getGlMainWidget()->getScene()->getGraphCamera().getUp();

  GlProgressBar *progressBar =
    new GlProgressBar(Coord(0, 0, 0), 600, 100,
                      // use same green color as the highlighting one
                      // in workspace panel
                      Color(0xCB, 0xDE, 0x5D));
  progressBar->setComment("Updating pixel oriented view ...");
  progressBar->progress(currentStep, nbOverviews);
  mainLayer->addGlEntity(progressBar, "progress bar");
  getGlMainWidget()->draw();
  // needed to display progressBar
  QApplication::processEvents();

  for (map<string, PixelOrientedOverview *>::iterator it = overviewsMap.begin() ; it != overviewsMap.end() ; ++it) {
    if (std::find(selectedGraphProperties.begin(), selectedGraphProperties.end(), it->first) != selectedGraphProperties.end()) {
      if (updateAll || overviewGenMap[it->first]) {
        (it->second)->computePixelView();
        overviewGenMap[it->first] = true;
      }

      progressBar->progress(++currentStep, nbOverviews);
      getGlMainWidget()->draw();
      // needed to display progressBar
      QApplication::processEvents();
    }
  }

  mainLayer->deleteGlEntity(progressBar);
  delete progressBar;

  if (smallMultiplesView) {
    mainLayer->addGlEntity(overviewsComposite, "overviews composite");
  }
  else {
    setGraphView(graphComposite, true);
    mainLayer->addGlEntity(detailViewLabel, "dimension label");
  }

  getGlMainWidget()->getScene()->getGraphCamera().setSceneRadius(sceneRadiusBak);
  getGlMainWidget()->getScene()->getGraphCamera().setZoomFactor(zoomFactorBak);
  getGlMainWidget()->getScene()->getGraphCamera().setEyes(eyesBak);
  getGlMainWidget()->getScene()->getGraphCamera().setCenter(centerBak);
  getGlMainWidget()->getScene()->getGraphCamera().setUp(upBak);

  getGlMainWidget()->draw();
}

vector<PixelOrientedOverview *> PixelOrientedView::getOverviews() {
  vector<PixelOrientedOverview *> ret;

  for (map<string, PixelOrientedOverview *>::iterator it = overviewsMap.begin() ; it != overviewsMap.end() ; ++it) {
    if (std::find(selectedGraphProperties.begin(), selectedGraphProperties.end(), it->first) != selectedGraphProperties.end()) {
      ret.push_back(it->second);
    }
  }

  return ret;
}

void PixelOrientedView::switchFromSmallMultiplesToDetailView(PixelOrientedOverview *pixelOverview) {

  if (smallMultiplesView) {
    sceneRadiusBak = getGlMainWidget()->getScene()->getGraphCamera().getSceneRadius();
    zoomFactorBak = getGlMainWidget()->getScene()->getGraphCamera().getZoomFactor();
    eyesBak = getGlMainWidget()->getScene()->getGraphCamera().getEyes();
    centerBak = getGlMainWidget()->getScene()->getGraphCamera().getCenter();
    upBak = getGlMainWidget()->getScene()->getGraphCamera().getUp();
  }

  mainLayer->deleteGlEntity(overviewsComposite);
  GlGraphInputData *inputData = graphComposite->getInputData();
  inputData->setElementLayout(pixelOverview->getPixelViewLayout());
  inputData->setElementSize(pixelOverview->getPixelViewSize());

  setGraphView(graphComposite, true);

  GlBoundingBoxSceneVisitor glBBSV(inputData);
  graphComposite->acceptVisitor(&glBBSV);
  BoundingBox graphBoundingBox = glBBSV.getBoundingBox();


  if (detailViewLabel != NULL) {
    delete detailViewLabel;
  }

  detailViewLabel = new GlLabel(Coord((graphBoundingBox[0][0] + graphBoundingBox[1][0])/ 2.f, graphBoundingBox[0][1] - (graphBoundingBox[1][1] - graphBoundingBox[0][1]) / 8.f),
                                Size((graphBoundingBox[1][0] - graphBoundingBox[0][0]), (graphBoundingBox[1][1] - graphBoundingBox[0][1]) / 4.f),
                                getTextColor());
  detailViewLabel->setText(pixelOverview->getData()->getDimensionName());
  mainLayer->addGlEntity(detailViewLabel, "dimension label");
  smallMultiplesView = false;
  toggleInteractors(true);
  detailOverview = pixelOverview;
  detailOverviewPropertyName = pixelOverview->getDimensionName();

  if (selectedGraphProperties.size() > 1) {
    propertiesSelectionWidget->setEnabled(false);
  }

  centerView();
}

void PixelOrientedView::switchFromDetailViewToSmallMultiples() {
  if (smallMultiplesNeedUpdate) {
    updateOverviews(true);
    smallMultiplesNeedUpdate = false;
  }

  setGraphView(graphComposite, false);
  mainLayer->deleteGlEntity(detailViewLabel);
  mainLayer->addGlEntity(overviewsComposite, "overviews composite");
  getGlMainWidget()->getScene()->getGraphCamera().setSceneRadius(sceneRadiusBak);
  getGlMainWidget()->getScene()->getGraphCamera().setZoomFactor(zoomFactorBak);
  getGlMainWidget()->getScene()->getGraphCamera().setEyes(eyesBak);
  getGlMainWidget()->getScene()->getGraphCamera().setCenter(centerBak);
  getGlMainWidget()->getScene()->getGraphCamera().setUp(upBak);
  smallMultiplesView = true;
  toggleInteractors(false);
  detailOverview = NULL;
  detailOverviewPropertyName = "";
  propertiesSelectionWidget->setEnabled(true);
  getGlMainWidget()->draw();
}

BoundingBox PixelOrientedView::getSmallMultiplesViewBoundingBox() {
  GlBoundingBoxSceneVisitor glBBSV(NULL);
  overviewsComposite->acceptVisitor(&glBBSV);
  return glBBSV.getBoundingBox();
}

void PixelOrientedView::interactorsInstalled(const QList<tlp::Interactor*>&) {
  toggleInteractors(false);
}

void PixelOrientedView::toggleInteractors(const bool activate) {
  QList<Interactor *> interactorsList = interactors();

  for (QList<Interactor *>::iterator it = interactorsList.begin() ; it != interactorsList.end() ; ++it) {
    if (!(dynamic_cast<PixelOrientedInteractorNavigation *>(*it))) {
      (*it)->action()->setEnabled(activate);

      if (!activate) {
        (*it)->action()->setChecked(false);
      }
    }
    else if (!activate) {
      (*it)->action()->setChecked(true);
    }

    interactorsActivated = activate;
  }
}

void PixelOrientedView::registerTriggers() {
  foreach (tlp::Observable* obs, triggers()) {
    removeRedrawTrigger(obs);
  }
  addRedrawTrigger(graph());
  Iterator<string> *it = graph()->getProperties();

  while(it->hasNext()) {
    PropertyInterface *property=graph()->getProperty(it->next());
    addRedrawTrigger(property);
  }
}

void PixelOrientedView::applySettings() {
  if(propertiesSelectionWidget->configurationChanged() || optionsWidget->configurationChanged()) {
    updateOverviews(true);
    getGlMainWidget()->centerScene();
    draw();
  }
}

}
