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
#include <iostream>
#include <fstream>
#include <cmath>

#include <QTimer>

#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlRect.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlLayer.h>
#include <tulip/GlLabel.h>
#include <tulip/LayoutProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/tulipconf.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>

#include "PixelOrientedInteractors.h"
#include "PixelOrientedView.h"
#include "PixelOrientedViewQuickAccessBar.h"

#include "../utils/ViewGraphPropertiesSelectionWidget.h"

#include <QApplication>
#include <QGraphicsView>
#include <QMainWindow>
#include <QProgressDialog>
#include <QTimer>

using namespace std;
using namespace pocore;

static void setGraphView(tlp::GlGraphComposite *glGraph, bool displayNodes) {
  tlp::GlGraphRenderingParameters param = glGraph->getRenderingParameters();
  param.setAntialiasing(true);
  param.setViewNodeLabel(true);
  param.setFontsType(2);
  param.setSelectedNodesStencil(1);
  param.setNodesStencil(0xFFFF);
  param.setNodesLabelStencil(0xFFFF);
  param.setDisplayEdges(false);
  param.setDisplayNodes(displayNodes);
  param.setDisplayMetaNodes(displayNodes);
  glGraph->setRenderingParameters(param);
}

const string propertiesTypes[] = {"double", "int"};
const unsigned int nbPropertiesTypes = sizeof(propertiesTypes) / sizeof(string);
const vector<string> propertiesTypesFilter(propertiesTypes, propertiesTypes + nbPropertiesTypes);

namespace tlp {

PLUGIN(PixelOrientedView)

PixelOrientedView::PixelOrientedView(const PluginContext *)
    : GlMainView(true), pixelOrientedGraph(nullptr), graphComposite(nullptr), mainLayer(nullptr),
      overviewsComposite(nullptr), optionsWidget(nullptr), propertiesSelectionWidget(nullptr),
      pixelOrientedMediator(nullptr), lastNbNodes(0), overviewWidth(0), overviewHeight(0),
      minWidth(0), refSize(0), hilbertLayout(nullptr), squareLayout(nullptr),
      spiralLayout(new SpiralLayout()), zorderLayout(nullptr), tulipNodeColorMapping(nullptr),
      smallMultiplesView(true), sceneRadiusBak(0.0), zoomFactorBak(0.0), detailViewLabel(nullptr),
      detailOverview(nullptr), newGraphSet(false), smallMultiplesNeedUpdate(false),
      lastViewWindowWidth(0), lastViewWindowHeight(0), center(false), isConstruct(false) {}

PixelOrientedView::~PixelOrientedView() {
  if (isConstruct) {
    destroyData();
  }

  delete spiralLayout;
  delete hilbertLayout;
  delete squareLayout;
  delete zorderLayout;
  delete tulipNodeColorMapping;
  delete pixelOrientedMediator;
  delete propertiesSelectionWidget;
  delete optionsWidget;
  delete graphComposite;
}

QList<QWidget *> PixelOrientedView::configurationWidgets() const {
  return QList<QWidget *>() << propertiesSelectionWidget << optionsWidget;
}

void PixelOrientedView::initGlWidget() {
  mainLayer = getGlMainWidget()->getScene()->getLayer("Main");

  if (mainLayer == nullptr) {
    mainLayer = new GlLayer("Main");
    getGlMainWidget()->getScene()->addExistingLayer(mainLayer);
  } else {
    overviewsComposite->reset(true);
    detailOverview = nullptr;
    mainLayer->getComposite()->reset(true);
  }
  overviewsComposite = new GlComposite();
  mainLayer->addGlEntity(overviewsComposite, "overview composite");

  detailViewLabel = nullptr;

  GlGraphComposite *lastGraphComposite = graphComposite;
  graphComposite = new GlGraphComposite(pixelOrientedGraph);
  mainLayer->addGlEntity(graphComposite, "graph");
  delete lastGraphComposite;

  setGraphView(graphComposite, false);
}

void PixelOrientedView::setColorFunction(pocore::ColorFunction *colorFunction) {
  pixelOrientedMediator->setColorFunction(colorFunction);
}

void PixelOrientedView::setLayoutFunction(pocore::LayoutFunction *layoutFunction) {
  pixelOrientedMediator->setLayoutFunction(layoutFunction);
}

void PixelOrientedView::setState(const DataSet &dataSet) {
  if (!isConstruct) {
    isConstruct = true;
    propertiesSelectionWidget = new ViewGraphPropertiesSelectionWidget();
    propertiesSelectionWidget->enableEdgesButton(false);
    pixelOrientedMediator = new PixelOrientedMediator(spiralLayout, nullptr);
    optionsWidget = new PixelOrientedOptionsWidget();
    layoutFunctionsMap["Spiral"] = spiralLayout;
  }

  GlMainView::setState(dataSet);

  Graph *lastGraph = pixelOrientedGraph;

  if (tulipNodeColorMapping != nullptr && pixelOrientedGraph != graph()) {
    delete tulipNodeColorMapping;
    tulipNodeColorMapping = nullptr;
  }

  if (tulipNodeColorMapping == nullptr) {
    tulipNodeColorMapping = new TulipNodeColorMapping(pixelOrientedGraph);
    pixelOrientedMediator->setColorFunction(tulipNodeColorMapping);
  }

  if (lastGraph == nullptr || lastGraph != graph()) {
    pixelOrientedGraph = graph();
    initGlWidget();
    destroyData();
    lastNbNodes = pixelOrientedGraph->numberOfNodes();
    initLayoutFunctions();

    if (lastGraph != nullptr && lastGraph != pixelOrientedGraph) {
      overviewsComposite->reset(true);
      overviewsMap.clear();
      detailOverview = nullptr;
      newGraphSet = true;
    }
  }

  if (lastGraph != nullptr && lastGraph->getRoot() != pixelOrientedGraph->getRoot()) {
    overviewGenMap.clear();
    detailOverviewPropertyName = "";
  }

  dataSet.get("lastViewWindowWidth", lastViewWindowWidth);
  dataSet.get("lastViewWindowHeight", lastViewWindowHeight);

  propertiesSelectionWidget->setWidgetParameters(pixelOrientedGraph, propertiesTypesFilter);

  Color backgroundColor(255, 255, 255);
  dataSet.get("background color", backgroundColor);
  optionsWidget->setBackgroundColor(backgroundColor);

  bool haveSelectedGraphProperties = false;

  DataSet selectedGraphPropertiesDataSet;

  if (dataSet.get("selected graph properties", selectedGraphPropertiesDataSet)) {
    haveSelectedGraphProperties = true;
    int i = 0;
    ostringstream oss;
    oss << i;

    while (selectedGraphPropertiesDataSet.exists(oss.str())) {
      string propertyName;
      selectedGraphPropertiesDataSet.get(oss.str(), propertyName);
      selectedGraphProperties.push_back(propertyName);
      oss.str("");
      oss << ++i;
    }

    propertiesSelectionWidget->setSelectedProperties(selectedGraphProperties);

    for (size_t i = 0; i < selectedGraphProperties.size(); ++i) {
      bool overviewGenerated = false;
      dataSet.get(selectedGraphProperties[i], overviewGenerated);
      overviewGenMap[selectedGraphProperties[i]] = overviewGenerated;
    }

    string layoutName("");
    dataSet.get("layout", layoutName);

    if (!layoutName.empty()) {
      optionsWidget->setLayoutType(layoutName);
    }
  }

  if (haveSelectedGraphProperties) {
    updateOverviews(true);
    getGlMainWidget()->centerScene();
  }
  draw();

  registerTriggers();

  string detailOverviewName;
  dataSet.get("detail overview  name", detailOverviewName);
  if (!detailOverviewName.empty()) {
    switchFromSmallMultiplesToDetailView(overviewsMap[detailOverviewName]);
  }

  bool quickAccessBarVisible = false;
  if (dataSet.get<bool>("quickAccessBarVisible", quickAccessBarVisible)) {
    needQuickAccessBar = true;
    setQuickAccessBarVisible(quickAccessBarVisible);
  } else // display quickaccessbar
    setQuickAccessBarVisible(true);
}

DataSet PixelOrientedView::state() const {
  DataSet dataSet = GlMainView::state();
  DataSet selectedGraphPropertiesDataSet;

  unsigned int i = 0;
  for (auto &prop : selectedGraphProperties) {
    ostringstream oss;
    oss << i++;
    selectedGraphPropertiesDataSet.set(oss.str(), prop);
    dataSet.set(prop, overviewGenMap.find(prop)->second);
  }

  dataSet.set("selected graph properties", selectedGraphPropertiesDataSet);
  dataSet.set("layout", optionsWidget->getLayoutType());
  dataSet.set("lastViewWindowWidth", getGlMainWidget()->width());
  dataSet.set("lastViewWindowHeight", getGlMainWidget()->height());
  dataSet.set("detail overview  name", detailOverviewPropertyName);
  dataSet.set("background color", optionsWidget->getBackgroundColor());

  if (needQuickAccessBar)
    dataSet.set("quickAccessBarVisible", quickAccessBarVisible());

  return dataSet;
}

void PixelOrientedView::graphChanged(Graph *) {
  setState(DataSet());
}

void PixelOrientedView::initLayoutFunctions() {
  delete hilbertLayout;
  delete squareLayout;
  delete zorderLayout;

  hilbertLayout = new HilbertLayout(int(ceil(log(pixelOrientedGraph->numberOfNodes()) / log(4))));
  squareLayout = new SquareLayout(int(ceil(sqrt(pixelOrientedGraph->numberOfNodes()))));
  zorderLayout = new ZorderLayout(int(ceil(log(pixelOrientedGraph->numberOfNodes()) / log(4))));
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

  minWidth = uint(floor(sqrt(double(pixelOrientedGraph->numberOfNodes())))) + 1;

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

  if (selectedGraphProperties.empty())
    return;

  float squareRoot = sqrt(float(selectedGraphProperties.size()));
  const unsigned int N =
      uint(squareRoot) + (fmod(float(selectedGraphProperties.size()), squareRoot) == 0.f ? 0u : 1u);

  for (size_t i = 0; i < selectedGraphProperties.size(); ++i) {
    const string &prop = selectedGraphProperties[i];

    auto itd = dataMap.find(prop);
    if (itd == dataMap.end())
      dataMap.emplace(prop, new TulipGraphDimension(pixelOrientedGraph, prop));
    else
      itd->second->updateNodesRank();

    unsigned int row = i / N;
    unsigned int col = i % N;

    Coord overviewBLCorner(
        col * (overviewWidth + spaceBetweenOverviews),
        -(labelHeight + row * (labelHeight + overviewHeight + spaceBetweenOverviews)), 0);
    ostringstream oss;
    oss << "pixel oriented overview for dimension " << prop;

    PixelOrientedOverview *pixelOrientedOverview;
    auto ito = overviewsMap.find(prop);
    if (ito == overviewsMap.end()) {
      pixelOrientedOverview = new PixelOrientedOverview(
          dataMap[prop], pixelOrientedMediator, overviewBLCorner, prop, backgroundColor, textColor);
      pixelOrientedOverview->computePixelView();
      overviewsMap.emplace(prop, pixelOrientedOverview);

      auto itog = overviewGenMap.find(prop);
      if (itog == overviewGenMap.end()) {
        overviewGenMap.emplace(prop, false);
      }
    } else {
      pixelOrientedOverview = ito->second;
      pixelOrientedOverview->setBLCorner(overviewBLCorner);
      pixelOrientedOverview->setBackgroundColor(backgroundColor);
      pixelOrientedOverview->setTextColor(textColor);
    }

    overviewsComposite->addGlEntity(pixelOrientedOverview, oss.str());
  }

  if (!detailOverviewPropertyName.empty()) {
    detailOverview = overviewsMap[detailOverviewPropertyName];
  }

  lastNbNodes = pixelOrientedGraph->numberOfNodes();
}

void PixelOrientedView::destroyOverviewsIfNeeded() {
  vector<string> propertiesToRemove;

  for (auto &prop : selectedGraphProperties) {
    if (!pixelOrientedGraph->existProperty(prop)) {
      if (overviewsMap[prop] == detailOverview) {
        detailOverview = nullptr;
        detailOverviewPropertyName = "";
      }

      delete overviewsMap[prop];
      overviewsMap.erase(prop);
      delete dataMap[prop];
      dataMap.erase(prop);
      propertiesToRemove.push_back(prop);
    }
  }

  for (auto &prop : propertiesToRemove) {
    selectedGraphProperties.erase(
        remove(selectedGraphProperties.begin(), selectedGraphProperties.end(), prop),
        selectedGraphProperties.end());
  }
}

void PixelOrientedView::destroyData() {
  for (auto &it : dataMap) {
    delete it.second;
  }

  dataMap.clear();
}

void PixelOrientedView::addEmptyViewLabel() {
  Color backgroundColor(optionsWidget->getBackgroundColor());
  getGlMainWidget()->getScene()->setBackgroundColor(backgroundColor);

  Color textColor = getTextColor();

  GlLabel *noDimsLabel = new GlLabel(Coord(0.0f, 0.0f, 0.0f), Size(200.0f, 200.0f), textColor);
  noDimsLabel->setText(ViewName::PixelOrientedViewName);
  mainLayer->addGlEntity(noDimsLabel, "no dimensions label");
  GlLabel *noDimsLabel1 = new GlLabel(Coord(0.0f, -50.0f, 0.0f), Size(400.0f, 200.0f), textColor);
  noDimsLabel1->setText("No graph properties selected.");
  mainLayer->addGlEntity(noDimsLabel1, "no dimensions label 1");
  GlLabel *noDimsLabel2 = new GlLabel(Coord(0.0f, -100.0f, 0.0f), Size(700.0f, 200.0f), textColor);
  noDimsLabel2->setText("Go to the \"Properties\" tab in top right corner.");
  mainLayer->addGlEntity(noDimsLabel2, "no dimensions label 2");

  mainLayer->deleteGlEntity(graphComposite);
}

void PixelOrientedView::removeEmptyViewLabel() {
  GlSimpleEntity *noDimsLabel = mainLayer->findGlEntity("no dimensions label");
  GlSimpleEntity *noDimsLabel1 = mainLayer->findGlEntity("no dimensions label 1");
  GlSimpleEntity *noDimsLabel2 = mainLayer->findGlEntity("no dimensions label 2");

  if (noDimsLabel != nullptr) {
    mainLayer->deleteGlEntity(noDimsLabel);
    delete noDimsLabel;
    mainLayer->deleteGlEntity(noDimsLabel1);
    delete noDimsLabel1;
    mainLayer->deleteGlEntity(noDimsLabel2);
    delete noDimsLabel2;

    mainLayer->addGlEntity(graphComposite, "graph");
  }
}

void PixelOrientedView::generatePixelOverview(PixelOrientedOverview *pixelOverview) {
  pixelOverview->computePixelView();
  overviewGenMap[pixelOverview->getDimensionName()] = true;
}

void PixelOrientedView::draw() {
  GlMainWidget *glw = getGlMainWidget();
  GlScene *scene = glw->getScene();

  scene->setBackgroundColor(optionsWidget->getBackgroundColor());
  unsigned int lastNbDimensionsSelected = selectedGraphProperties.size();
  destroyOverviewsIfNeeded();

  if (pixelOrientedGraph->numberOfNodes() >= 2) // We stop the creation of pixel overviews if we
    // have less than 2 nodes as we need at least two
    // to compute sizes for overview
    initPixelView();
  else
    selectedGraphProperties.clear();

  if (selectedGraphProperties.empty()) {
    if (!smallMultiplesView) {
      switchFromDetailViewToSmallMultiples();
    }

    removeEmptyViewLabel();
    addEmptyViewLabel();
    if (quickAccessBarVisible())
      _quickAccessBar->setEnabled(false);
    setOverviewVisible(false);
    glw->centerScene();
    return;
  }

  removeEmptyViewLabel();
  if (quickAccessBarVisible())
    _quickAccessBar->setEnabled(true);
  setOverviewVisible(true);

  if (lastNbDimensionsSelected != selectedGraphProperties.size()) {
    center = true;
  }

  if (!smallMultiplesView && lastNbDimensionsSelected == 1 && selectedGraphProperties.size() > 1) {
    switchFromDetailViewToSmallMultiples();
  } else if (selectedGraphProperties.size() == 1) {
    glw->makeCurrent();
    overviewsMap[selectedGraphProperties[0]]->computePixelView();
    overviewGenMap[selectedGraphProperties[0]] = true;
    switchFromSmallMultiplesToDetailView(overviewsMap[selectedGraphProperties[0]]);
  } else if (!smallMultiplesView && detailOverview != nullptr) {
    detailOverview->computePixelView();
    smallMultiplesNeedUpdate = true;

    if (newGraphSet) {
      switchFromSmallMultiplesToDetailView(detailOverview);
      newGraphSet = false;
    }
  } else if (!smallMultiplesView && detailOverview == nullptr) {
    switchFromDetailViewToSmallMultiples();
  } else {
    glw->makeCurrent();
    updateOverviews(true);
  }

  if (!smallMultiplesView && detailViewLabel != nullptr) {
    detailViewLabel->setColor(getTextColor());
  }

  if (center) {
    centerView();
    center = false;
  } else {
    glw->draw();
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
    textColor = Color(255, 255, 255);
  } else {
    textColor = Color(0, 0, 0);
  }

  return textColor;
}

void PixelOrientedView::centerView(bool) {
  if (!getGlMainWidget()->isVisible()) {
    if (lastViewWindowWidth != 0 && lastViewWindowHeight != 0) {
      getGlMainWidget()->getScene()->adjustSceneToSize(lastViewWindowWidth, lastViewWindowHeight);
    } else {
      getGlMainWidget()->getScene()->centerScene();
    }
  } else {
    getGlMainWidget()->getScene()->adjustSceneToSize(getGlMainWidget()->width(),
                                                     getGlMainWidget()->height());
  }

  // we apply a zoom factor to preserve a 50 px margin height
  // to ensure the scene will not be drawn under the configuration tabs title
  float glHeight = graphicsView()->width();
  getGlMainWidget()->getScene()->zoomFactor((glHeight - 50) / glHeight);
  getGlMainWidget()->draw();
}

void PixelOrientedView::updateOverviews(const bool updateAll) {
  unsigned int nbOverviews = selectedGraphProperties.size();

  if (nbOverviews) {
    if (smallMultiplesView) {
      mainLayer->deleteGlEntity(overviewsComposite);
    } else {
      setGraphView(graphComposite, false);
      mainLayer->deleteGlEntity(detailViewLabel);
    }

    unsigned currentStep = 0;
    Camera &cam = getGlMainWidget()->getScene()->getGraphCamera();
    double sceneRadiusBak = cam.getSceneRadius();
    double zoomFactorBak = cam.getZoomFactor();
    Coord eyesBak = cam.getEyes();
    Coord centerBak = cam.getCenter();
    Coord upBak = cam.getUp();

    QProgressDialog progress(Perspective::instance()->mainWindow());
    progress.setCancelButton(nullptr);
    progress.setWindowTitle("Computing pixel oriented view for: ");
    progress.setRange(0, nbOverviews);
    progress.setMinimumWidth(400);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);

    for (auto &it : overviewsMap) {
      if (std::find(selectedGraphProperties.begin(), selectedGraphProperties.end(), it.first) !=
          selectedGraphProperties.end()) {
        if (updateAll || overviewGenMap[it.first]) {
          (it.second)->computePixelView();
          overviewGenMap[it.first] = true;
        }
        progress.setLabelText((it.second)->getDimensionName().c_str());
        progress.setValue(++currentStep);
      }
    }

    if (smallMultiplesView) {
      mainLayer->addGlEntity(overviewsComposite, "overviews composite");
    } else {
      setGraphView(graphComposite, true);
      mainLayer->addGlEntity(detailViewLabel, "dimension label");
    }

    cam.setSceneRadius(sceneRadiusBak);
    cam.setZoomFactor(zoomFactorBak);
    cam.setEyes(eyesBak);
    cam.setCenter(centerBak);
    cam.setUp(upBak);

    getGlMainWidget()->draw();
  }
}

vector<PixelOrientedOverview *> PixelOrientedView::getOverviews() {
  vector<PixelOrientedOverview *> ret;

  for (auto &it : overviewsMap) {
    if (std::find(selectedGraphProperties.begin(), selectedGraphProperties.end(), it.first) !=
        selectedGraphProperties.end()) {
      ret.push_back(it.second);
    }
  }

  return ret;
}

QuickAccessBar *PixelOrientedView::getQuickAccessBarImpl() {
  auto _bar = new PixelOrientedViewQuickAccessBar(optionsWidget);

  connect(_bar, SIGNAL(settingsChanged()), this, SLOT(applySettings()));
  return _bar;
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

  delete detailViewLabel;
  detailViewLabel = new GlLabel(
      Coord((graphBoundingBox[0][0] + graphBoundingBox[1][0]) / 2.f,
            graphBoundingBox[0][1] - (graphBoundingBox[1][1] - graphBoundingBox[0][1]) / 8.f),
      Size((graphBoundingBox[1][0] - graphBoundingBox[0][0]),
           (graphBoundingBox[1][1] - graphBoundingBox[0][1]) / 4.f),
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

  if (tlp::inGuiTestingMode())
    // sometimes we must wait a bit to ensure an effective centerView
    QTimer::singleShot(200, this, SLOT(centerView()));
  else
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
  detailOverview = nullptr;
  detailOverviewPropertyName = "";
  propertiesSelectionWidget->setEnabled(true);
  getGlMainWidget()->draw();
}

BoundingBox PixelOrientedView::getSmallMultiplesViewBoundingBox() {
  GlBoundingBoxSceneVisitor glBBSV(nullptr);
  overviewsComposite->acceptVisitor(&glBBSV);
  return glBBSV.getBoundingBox();
}

void PixelOrientedView::interactorsInstalled(const QList<tlp::Interactor *> &) {
  toggleInteractors(detailOverview != nullptr);
}

void PixelOrientedView::toggleInteractors(const bool activate) {
  View::toggleInteractors(activate, {InteractorName::PixelOrientedInteractorNavigation});
}

void PixelOrientedView::registerTriggers() {
  for (auto obs : triggers()) {
    removeRedrawTrigger(obs);
  }

  addRedrawTrigger(graph());

  for (auto prop : graph()->getObjectProperties()) {
    addRedrawTrigger(prop);
  }
}

void PixelOrientedView::applySettings() {
  if (propertiesSelectionWidget->configurationChanged() || optionsWidget->configurationChanged()) {
    updateOverviews(true);
    draw();

    if (!smallMultiplesView) {
      mainLayer->deleteGlEntity(detailViewLabel);
      GlGraphInputData *inputData = graphComposite->getInputData();
      inputData->setElementLayout(detailOverview->getPixelViewLayout());
      inputData->setElementSize(detailOverview->getPixelViewSize());
      GlBoundingBoxSceneVisitor glBBSV(inputData);
      graphComposite->acceptVisitor(&glBBSV);
      BoundingBox graphBoundingBox = glBBSV.getBoundingBox();

      detailViewLabel->setPosition(
          Coord((graphBoundingBox[0][0] + graphBoundingBox[1][0]) / 2.f,
                graphBoundingBox[0][1] - (graphBoundingBox[1][1] - graphBoundingBox[0][1]) / 8.f));

      detailViewLabel->setSize(Size((graphBoundingBox[1][0] - graphBoundingBox[0][0]),
                                    (graphBoundingBox[1][1] - graphBoundingBox[0][1]) / 4.f));

      mainLayer->addGlEntity(detailViewLabel, "dimension label");
      centerView();
    }
    drawOverview(true);
  }
}
} // namespace tlp
