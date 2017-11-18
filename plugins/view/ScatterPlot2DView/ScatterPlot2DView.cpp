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

#include <tulip/GlTextureManager.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlQuantitativeAxis.h>
#include <tulip/GlLabel.h>
#include <tulip/GlLine.h>
#include <tulip/GlProgressBar.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GraphProperty.h>
#include <tulip/TulipViewSettings.h>

#include <QTime>
#include <QMenu>
#include <QGraphicsView>
#include <QApplication>

#include "ScatterPlot2DView.h"
#include "ScatterPlot2DOptionsWidget.h"
#include "ScatterPlot2D.h"
#include "ScatterPlot2DInteractors.h"
#include "ScatterPlotQuickAccessBar.h"

#include "../utils/ViewGraphPropertiesSelectionWidget.h"

using namespace std;

const unsigned int OVERVIEWS_SIZE = 512;
const float OFFSET_BETWEEN_PREVIEWS = 16;

namespace tlp {

GLuint ScatterPlot2DView::backgroundTextureId(0);
unsigned int ScatterPlot2DView::scatterplotViewInstancesCount(0);

PLUGIN(ScatterPlot2DView)

class map_pair_string_key_contains
    : public unary_function<pair<pair<string, string>, ScatterPlot2D *>, bool> {

public:
  map_pair_string_key_contains(const string &pairValueToFind) : pairValueToFind(pairValueToFind) {}

  bool operator()(const pair<pair<string, string>, ScatterPlot2D *> &elem) const {
    string pairStringKeyFirst = elem.first.first;
    string pairStringKeySecond = elem.first.second;
    return (pairStringKeyFirst == pairValueToFind) || (pairStringKeySecond == pairValueToFind);
  }

private:
  string pairValueToFind;
};

const string propertiesTypes[] = {"double", "int"};
const unsigned int nbPropertiesTypes = sizeof(propertiesTypes) / sizeof(string);
const vector<string> propertiesTypesFilter(propertiesTypes, propertiesTypes + nbPropertiesTypes);

ScatterPlot2DView::ScatterPlot2DView(const PluginContext *)
    : _bar(NULL), propertiesSelectionWidget(NULL), optionsWidget(NULL), scatterPlotGraph(NULL),
      emptyGraph(NULL), mainLayer(NULL), glGraphComposite(NULL), scatterPlotSize(NULL),
      matrixComposite(NULL), axisComposite(NULL), labelsComposite(NULL), detailedScatterPlot(NULL),
      detailedScatterPlotPropertyName(make_pair("", "")), center(false), matrixView(true),
      sceneRadiusBak(0.0), zoomFactorBak(0.0), matrixUpdateNeeded(false), newGraphSet(false),
      lastViewWindowWidth(0), lastViewWindowHeight(0), interactorsActivated(false),
      initialized(false), edgeAsNodeGraph(NULL) {}

ScatterPlot2DView::~ScatterPlot2DView() {

  if (initialized)
    --scatterplotViewInstancesCount;

  if (scatterplotViewInstancesCount == 0) {
    GlTextureManager::getInst().deleteTexture("gaussian_text_back");
    backgroundTextureId = 0;
  }

  delete propertiesSelectionWidget;
  delete optionsWidget;
  delete glGraphComposite;
  delete matrixComposite;
  delete axisComposite;
  delete emptyGraph;
  delete edgeAsNodeGraph;
}

void ScatterPlot2DView::initGlWidget(Graph *) {
  GlLayer *layer = getGlMainWidget()->getScene()->getLayer("Main");

  if (layer == NULL) {
    layer = new GlLayer("Main");
    getGlMainWidget()->getScene()->addExistingLayer(layer);
  }

  mainLayer = layer;

  cleanupGlScene();

  if (emptyGraph == NULL) {
    emptyGraph = newGraph();
    glGraphComposite = new GlGraphComposite(emptyGraph);
    mainLayer->addGlEntity(glGraphComposite, "graph");
  }

  if (matrixComposite == NULL) {
    matrixComposite = new GlComposite();
    mainLayer->addGlEntity(matrixComposite, "matrix composite");
  }

  if (axisComposite == NULL) {
    axisComposite = new GlComposite();
    mainLayer->addGlEntity(axisComposite, "axis composite");
  }

  if (labelsComposite == NULL) {
    labelsComposite = new GlComposite();
  }
}

void ScatterPlot2DView::cleanupGlScene() {

  if (axisComposite != NULL) {
    axisComposite->reset(false);
  }

  if (labelsComposite != NULL) {
    labelsComposite->reset(true);
  }

  if (matrixComposite != NULL) {
    matrixComposite->reset(true);
    // labelsComposite was added as a GlEntity of matrixComposite
    // in buildScatterPlotsMatrix() (see below)
    // so it has been deleted by the previous call and then
    // we must ensure to use a new one if needed
    labelsComposite = NULL;
    scatterPlotsMap.clear();
  }
}

QList<QWidget *> ScatterPlot2DView::configurationWidgets() const {
  return QList<QWidget *>() << propertiesSelectionWidget << optionsWidget;
}

void ScatterPlot2DView::setState(const DataSet &dataSet) {
  if (!initialized) {
    propertiesSelectionWidget = new ViewGraphPropertiesSelectionWidget();
    optionsWidget = new ScatterPlot2DOptionsWidget();
    optionsWidget->setWidgetEnabled(false);
    ++scatterplotViewInstancesCount;
    initialized = true;
    setOverviewVisible(true);
    needQuickAccessBar = true;
  }

  if (backgroundTextureId == 0) {
    getGlMainWidget()->getFirstQGLWidget()->makeCurrent();
    backgroundTextureId = getGlMainWidget()->getFirstQGLWidget()->bindTexture(
        QPixmap(":/background_texture.png").transformed(QTransform().rotate(90)), GL_TEXTURE_2D);
    GlTextureManager::getInst().registerExternalTexture("gaussian_tex_back", backgroundTextureId);
  }

  Graph *lastGraph = scatterPlotGraph;
  scatterPlotGraph = graph();
  propertiesSelectionWidget->setWidgetParameters(scatterPlotGraph, propertiesTypesFilter);

  if (lastGraph == NULL || lastGraph != scatterPlotGraph) {
    newGraphSet = true;

    if (lastGraph) {
      lastGraph->removeListener(this);
      lastGraph->getProperty("viewColor")->removeListener(this);
      lastGraph->getProperty("viewLabel")->removeListener(this);
      lastGraph->getProperty("viewSelection")->removeListener(this);
      lastGraph->getProperty("viewSize")->removeListener(this);
      lastGraph->getProperty("viewShape")->removeListener(this);
      lastGraph->getProperty("viewTexture")->removeListener(this);
    }

    delete edgeAsNodeGraph;

    if (scatterPlotGraph) {
      edgeAsNodeGraph = tlp::newGraph();
      ColorProperty *edgeAsNodeGraphColor =
          edgeAsNodeGraph->getProperty<ColorProperty>("viewColor");
      ColorProperty *graphColor = scatterPlotGraph->getProperty<ColorProperty>("viewColor");
      BooleanProperty *edgeAsNodeGraphSelection =
          edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection");
      BooleanProperty *graphSelection =
          scatterPlotGraph->getProperty<BooleanProperty>("viewSelection");
      StringProperty *edgeAsNodeGraphLabel =
          edgeAsNodeGraph->getProperty<StringProperty>("viewLabel");
      StringProperty *graphLabel = scatterPlotGraph->getProperty<StringProperty>("viewLabel");
      edgeToNode.clear();
      nodeToEdge.clear();
      edge e;
      forEach(e, scatterPlotGraph->getEdges()) {
        node n = edgeToNode[e] = edgeAsNodeGraph->addNode();
        nodeToEdge[n] = e;
        edgeAsNodeGraphColor->setNodeValue(n, graphColor->getEdgeValue(e));
        edgeAsNodeGraphSelection->setNodeValue(n, graphSelection->getEdgeValue(e));
        edgeAsNodeGraphLabel->setNodeValue(n, graphLabel->getEdgeValue(e));
      }
      // This is quite ugly but before listening to the graph we must
      // ensure that its viewMetaGraph property already exist to avoid
      // an event loop when building the Scatterplot2D
      scatterPlotGraph->getRoot()->getProperty<GraphProperty>("viewMetaGraph");
      scatterPlotGraph->addListener(this);
      graphColor->addListener(this);
      graphLabel->addListener(this);
      graphSelection->addListener(this);
      scatterPlotGraph->getProperty("viewSize")->addListener(this);
      scatterPlotGraph->getProperty("viewShape")->addListener(this);
      scatterPlotGraph->getProperty("viewTexture")->addListener(this);

      edgeAsNodeGraphSelection->addListener(this);
      edgeAsNodeGraph->getProperty<IntegerProperty>("viewShape")
          ->setAllNodeValue(NodeShape::Circle);
    } else
      edgeAsNodeGraph = NULL;

    initGlWidget(scatterPlotGraph);
    detailedScatterPlot = NULL;
    destroyOverviews();
  }

  if (!scatterPlotGraph) {
    scatterPlotsGenMap.clear();
  } else {
    if (lastGraph != NULL && scatterPlotGraph != NULL &&
        (lastGraph->getRoot() != scatterPlotGraph->getRoot())) {
      scatterPlotsGenMap.clear();
    }
  }

  center = lastGraph ? false : true;

  dataSet.get("lastViewWindowWidth", lastViewWindowWidth);
  dataSet.get("lastViewWindowHeight", lastViewWindowHeight);

  bool showedges = false, showlabels = false, scalelabels = false;

  if (dataSet.get("display graph edges", showedges))
    optionsWidget->setDisplayGraphEdges(showedges);

  if (dataSet.get("display node labels", showlabels))
    optionsWidget->setDisplayNodeLabels(showlabels);

  if (dataSet.get("scale labels", scalelabels))
    optionsWidget->setDisplayScaleLabels(scalelabels);

  Color backgroundColor;

  if (dataSet.get("background color", backgroundColor))
    optionsWidget->setBackgroundColor(backgroundColor);

  int minSizeMap = 0;

  if (dataSet.get("min Size Mapping", minSizeMap))
    optionsWidget->setMinSizeMapping(float(minSizeMap));

  int maxSizeMap = 0;

  if (dataSet.get("max Size Mapping", maxSizeMap))
    optionsWidget->setMaxSizeMapping(float(maxSizeMap));

  optionsWidget->configurationChanged();

  DataSet selectedGraphPropertiesDataSet;

  if (dataSet.get("selected graph properties", selectedGraphPropertiesDataSet)) {
    selectedGraphProperties.clear();
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
    DataSet generatedScatterPlotDataSet;
    dataSet.get("generated scatter plots", generatedScatterPlotDataSet);

    for (size_t j = 0; j < selectedGraphProperties.size(); ++j) {
      for (size_t k = 0; k < selectedGraphProperties.size(); ++k) {
        if (j != k) {
          bool scatterPlotGenerated = false;
          generatedScatterPlotDataSet.get(
              selectedGraphProperties[j] + "_" + selectedGraphProperties[k], scatterPlotGenerated);
          scatterPlotsGenMap[make_pair(selectedGraphProperties[j], selectedGraphProperties[k])] =
              scatterPlotGenerated;
        }
      }
    }
  }

  unsigned nodes = NODE;
  dataSet.get("Nodes/Edges", nodes);
  dataLocation = static_cast<ElementType>(nodes);
  propertiesSelectionWidget->setDataLocation(dataLocation);
  viewConfigurationChanged();

  if (overviewVisible())
    drawOverview(true);

  string detailScatterPlotX = "";
  string detailScatterPlotY = "";
  dataSet.get("detailed scatterplot x dim", detailScatterPlotX);
  dataSet.get("detailed scatterplot y dim", detailScatterPlotY);

  if (!detailScatterPlotX.empty() && !detailScatterPlotY.empty()) {
    pair<string, string> x_y = make_pair(detailScatterPlotX, detailScatterPlotY);

    if (!scatterPlotsGenMap[x_y]) {
      scatterPlotsMap[x_y]->generateOverview();
      scatterPlotsGenMap[x_y] = true;
    }

    switchFromMatrixToDetailView(scatterPlotsMap[make_pair(detailScatterPlotX, detailScatterPlotY)],
                                 true);
  }

  registerTriggers();

  bool quickAccessBarVisible = false;

  if (dataSet.get<bool>("quickAccessBarVisible", quickAccessBarVisible)) {
    needQuickAccessBar = true;
    setQuickAccessBarVisible(quickAccessBarVisible);
  } else // display quickaccessbar
    setQuickAccessBarVisible(true);

  GlMainView::setState(dataSet);
}

DataSet ScatterPlot2DView::state() const {
  DataSet dataSet = GlMainView::state();
  DataSet selectedGraphPropertiesDataSet;

  for (size_t i = 0; i < selectedGraphProperties.size(); ++i) {
    ostringstream oss;
    oss << i;
    selectedGraphPropertiesDataSet.set(oss.str(), selectedGraphProperties[i]);
  }

  dataSet.set("selected graph properties", selectedGraphPropertiesDataSet);
  DataSet generatedScatterPlotDataSet;
  map<pair<string, string>, bool>::const_iterator it;

  for (it = scatterPlotsGenMap.begin(); it != scatterPlotsGenMap.end(); ++it) {
    generatedScatterPlotDataSet.set((*it).first.first + "_" + (*it).first.second, (*it).second);
  }

  dataSet.set("generated scatter plots", generatedScatterPlotDataSet);
  dataSet.set("min Size Mapping", int(optionsWidget->getMinSizeMapping().getW()));
  dataSet.set("max Size Mapping", int(optionsWidget->getMaxSizeMapping().getW()));
  dataSet.set("background color", optionsWidget->getBackgroundColor());
  dataSet.set("display graph edges", optionsWidget->displayGraphEdges());
  dataSet.set("display node labels", optionsWidget->displayNodeLabels());
  dataSet.set("scale labels", optionsWidget->displayScaleLabels());
  dataSet.set("lastViewWindowWidth", getGlMainWidget()->width());
  dataSet.set("lastViewWindowHeight", getGlMainWidget()->height());
  dataSet.set("detailed scatterplot x dim", detailedScatterPlotPropertyName.first);
  dataSet.set("detailed scatterplot y dim", detailedScatterPlotPropertyName.second);
  dataSet.set("Nodes/Edges", static_cast<unsigned>(dataLocation));

  if (needQuickAccessBar)
    dataSet.set("quickAccessBarVisible", quickAccessBarVisible());

  return dataSet;
}

Graph *ScatterPlot2DView::getScatterPlotGraph() {
  return scatterPlotGraph;
}

void ScatterPlot2DView::graphChanged(Graph *) {
  setState(DataSet());
}

void ScatterPlot2DView::toggleInteractors(const bool activate) {
  QList<Interactor *> interactorsList = interactors();

  for (QList<Interactor *>::iterator it = interactorsList.begin(); it != interactorsList.end();
       ++it) {
    if (!(dynamic_cast<ScatterPlot2DInteractorNavigation *>(*it))) {
      (*it)->action()->setEnabled(activate);

      if (!activate) {
        (*it)->action()->setChecked(false);
      }
    } else if (!activate) {
      (*it)->action()->setChecked(true);
    }

    interactorsActivated = activate;
  }
}

void ScatterPlot2DView::computeNodeSizes() {
  if (!scatterPlotSize) {
    scatterPlotSize = new SizeProperty(scatterPlotGraph);
  } else {
    scatterPlotSize->setAllNodeValue(Size(0, 0, 0));
    scatterPlotSize->setAllEdgeValue(Size(0, 0, 0));
  }

  SizeProperty *viewSize = scatterPlotGraph->getProperty<SizeProperty>("viewSize");

  Size eltMinSize(viewSize->getMin());
  Size eltMaxSize(viewSize->getMax());
  Size pointMinSize(optionsWidget->getMinSizeMapping());
  Size pointMaxSize(optionsWidget->getMaxSizeMapping());

  Size resizeFactor;
  Size deltaSize(eltMaxSize - eltMinSize);

  for (unsigned int i = 0; i < 3; ++i) {
    if (deltaSize[i] != 0) {
      resizeFactor[i] = (pointMaxSize[i] - pointMinSize[i]) / deltaSize[i];
    } else {
      resizeFactor[i] = 0;
    }
  }

  node n;
  forEach(n, scatterPlotGraph->getNodes()) {
    const Size &nodeSize = viewSize->getNodeValue(n);
    Size adjustedNodeSize(pointMinSize + resizeFactor * (nodeSize + Size(-1.0f, -1.0f, -1.0f)));
    scatterPlotSize->setNodeValue(n, adjustedNodeSize);
  }

  GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
  glGraphInputData->setElementSize(scatterPlotSize);
}

QuickAccessBar *ScatterPlot2DView::getQuickAccessBarImpl() {
  _bar = new ScatterPlotQuickAccessBar(optionsWidget);
  connect(_bar, SIGNAL(settingsChanged()), this, SLOT(applySettings()));
  return _bar;
}

void ScatterPlot2DView::buildScatterPlotsMatrix() {

  dataLocation = propertiesSelectionWidget->getDataLocation();
  Color backgroundColor(optionsWidget->getBackgroundColor());
  getGlMainWidget()->getScene()->setBackgroundColor(backgroundColor);

  Color foregroundColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    foregroundColor = Color(255, 255, 255);
  } else {
    foregroundColor = Color(0, 0, 0);
  }

  float gridLeft = -(OFFSET_BETWEEN_PREVIEWS / 2.0f);
  float gridBottom = gridLeft;
  float gridRight = selectedGraphProperties.size() * (OVERVIEWS_SIZE) +
                    (selectedGraphProperties.size() - 1.0f) * OFFSET_BETWEEN_PREVIEWS +
                    (OFFSET_BETWEEN_PREVIEWS / 2.0f);
  float gridTop = gridRight;
  float cellSize = OVERVIEWS_SIZE + OFFSET_BETWEEN_PREVIEWS;

  GlSimpleEntity *lastGrid = matrixComposite->findGlEntity("grid");
  matrixComposite->reset(false);
  delete lastGrid;
  labelsComposite->reset(true);

  if (selectedGraphProperties.size() >= 2) {

    GlComposite *grid = new GlComposite(true);
    GlLine *lineV0 = new GlLine();
    lineV0->addPoint(Coord(gridLeft, gridBottom, -1.0f), Color(0, 0, 0, 255));
    lineV0->addPoint(Coord(gridLeft, gridTop - cellSize, -1.0f), Color(0, 0, 0, 255));
    grid->addGlEntity(lineV0, "lineV0");
    GlLine *lineH0 = new GlLine();
    lineH0->addPoint(Coord(gridLeft, gridBottom, -1.0f), Color(0, 0, 0, 255));
    lineH0->addPoint(Coord(gridRight - cellSize, gridBottom, -1.0f), Color(0, 0, 0, 255));
    grid->addGlEntity(lineH0, "lineH0");

    for (unsigned int i = 0; i < selectedGraphProperties.size(); ++i) {
      GlLine *lineV = new GlLine();
      lineV->addPoint(Coord(gridLeft + cellSize * (i + 1), gridBottom, -1.0f), Color(0, 0, 0, 255));
      lineV->addPoint(Coord(gridLeft + cellSize * (i + 1), gridTop - cellSize * (i + 1), -1.0f),
                      Color(0, 0, 0, 255));
      GlLine *lineH = new GlLine();
      lineH->addPoint(Coord(gridLeft, gridBottom + cellSize * (i + 1), -1.0f), Color(0, 0, 0, 255));
      lineH->addPoint(Coord(gridRight - cellSize * (i + 1), gridBottom + cellSize * (i + 1), -1.0f),
                      Color(0, 0, 0, 255));
      stringstream strstr;
      strstr << i + 1;
      grid->addGlEntity(lineV, "lineV" + strstr.str());
      grid->addGlEntity(lineH, "lineH" + strstr.str());
    }

    matrixComposite->addGlEntity(grid, "grid");
    matrixComposite->addGlEntity(labelsComposite, "labels composite");

    for (size_t i = 0; i < selectedGraphProperties.size(); ++i) {

      if (i != selectedGraphProperties.size() - 1) {
        GlLabel *xLabel = new GlLabel(
            Coord(gridLeft + i * cellSize + cellSize / 2.0f, gridBottom - cellSize / 4.0f),
            Size(8.0f * (cellSize / 10.0f), cellSize / 2.0f), foregroundColor);
        xLabel->setText(selectedGraphProperties[i]);
        labelsComposite->addGlEntity(xLabel, selectedGraphProperties[i] + "x label");
      }

      if (i != 0) {
        GlLabel *yLabel =
            new GlLabel(Coord(gridLeft - cellSize / 2.0f, gridTop - i * cellSize - cellSize / 2.0f),
                        Size(8.0f * (cellSize / 10.0f), cellSize / 2.0f), foregroundColor);
        yLabel->setText(selectedGraphProperties[i]);
        labelsComposite->addGlEntity(yLabel, selectedGraphProperties[i] + "y label");
      }

      for (size_t j = i + 1; j < selectedGraphProperties.size(); ++j) {
        pair<string, string> overviewsMapKey =
            make_pair(selectedGraphProperties[i], selectedGraphProperties[j]);
        ScatterPlot2D *scatterOverview = NULL;
        Coord overviewBlCorner(i * (OVERVIEWS_SIZE + OFFSET_BETWEEN_PREVIEWS),
                               (selectedGraphProperties.size() - j - 1.0f) *
                                   (OVERVIEWS_SIZE + OFFSET_BETWEEN_PREVIEWS));
        map<pair<string, string>, ScatterPlot2D *>::iterator it =
            scatterPlotsMap.find(overviewsMapKey);

        if (it != scatterPlotsMap.end() && it->second) {
          scatterOverview = (it->second);

          if (!scatterOverview)
            continue;

          scatterOverview->setDataLocation(dataLocation);
          scatterOverview->setBLCorner(overviewBlCorner);
          scatterOverview->setUniformBackgroundColor(backgroundColor);
          scatterOverview->setForegroundColor(foregroundColor);
        } else {
          scatterOverview = new ScatterPlot2D(
              scatterPlotGraph, edgeAsNodeGraph, nodeToEdge, selectedGraphProperties[i],
              selectedGraphProperties[j], dataLocation, overviewBlCorner, OVERVIEWS_SIZE,
              backgroundColor, foregroundColor);
          scatterPlotsMap[overviewsMapKey] = scatterOverview;

          if (scatterPlotsGenMap.find(overviewsMapKey) == scatterPlotsGenMap.end()) {
            scatterPlotsGenMap[overviewsMapKey] = false;
          }
        }

        scatterOverview->setDisplayGraphEdges(optionsWidget->displayGraphEdges());
        scatterOverview->setDisplayNodeLabels(optionsWidget->displayNodeLabels());
        scatterOverview->setLabelsScaled(optionsWidget->displayScaleLabels());

        if (!optionsWidget->uniformBackground()) {
          scatterOverview->mapBackgroundColorToCorrelCoeff(true, optionsWidget->getMinusOneColor(),
                                                           optionsWidget->getZeroColor(),
                                                           optionsWidget->getOneColor());
        }

        matrixComposite->addGlEntity(scatterOverview,
                                     selectedGraphProperties[i] + "_" + selectedGraphProperties[j]);

        // add some feedback
        /*if ((i + 1) * (j + 1) % 10 == 0)
          QApplication::processEvents();*/
      }
    }
  }

  if (!detailedScatterPlotPropertyName.first.empty() &&
      !detailedScatterPlotPropertyName.second.empty()) {
    detailedScatterPlot = scatterPlotsMap[detailedScatterPlotPropertyName];
  }

  if (center)
    centerView();
}

void ScatterPlot2DView::addEmptyViewLabel() {
  Color backgroundColor(optionsWidget->getBackgroundColor());
  getGlMainWidget()->getScene()->setBackgroundColor(backgroundColor);

  Color foregroundColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    foregroundColor = Color(255, 255, 255);
  } else {
    foregroundColor = Color(0, 0, 0);
  }

  GlLabel *noDimsLabel =
      new GlLabel(Coord(0.0f, 0.0f, 0.0f), Size(200.0f, 200.0f), foregroundColor);
  noDimsLabel->setText(ViewName::ScatterPlot2DViewName);
  mainLayer->addGlEntity(noDimsLabel, "no dimensions label");
  GlLabel *noDimsLabel1 =
      new GlLabel(Coord(0.0f, -50.0f, 0.0f), Size(400.0f, 200.0f), foregroundColor);
  noDimsLabel1->setText("Select at least two graph properties.");
  mainLayer->addGlEntity(noDimsLabel1, "no dimensions label 1");
  GlLabel *noDimsLabel2 =
      new GlLabel(Coord(0.0f, -100.0f, 0.0f), Size(700.0f, 200.0f), foregroundColor);
  noDimsLabel2->setText("Go to the \"Properties\" tab in top right corner.");
  mainLayer->addGlEntity(noDimsLabel2, "no dimensions label 2");
}

void ScatterPlot2DView::removeEmptyViewLabel() {
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

void ScatterPlot2DView::viewConfigurationChanged() {
  getGlMainWidget()->getScene()->setBackgroundColor(optionsWidget->getBackgroundColor());
  bool dataLocationChanged = propertiesSelectionWidget->getDataLocation() != dataLocation;

  if (dataLocationChanged) {
    detailedScatterPlot = NULL;
    buildScatterPlotsMatrix();
  }

  if (detailedScatterPlot != NULL) {

    detailedScatterPlot->setXAxisScaleDefined(optionsWidget->useCustomXAxisScale());
    detailedScatterPlot->setXAxisScale(optionsWidget->getXAxisScale());
    detailedScatterPlot->setYAxisScaleDefined(optionsWidget->useCustomYAxisScale());
    detailedScatterPlot->setYAxisScale(optionsWidget->getYAxisScale());
  }

  draw();
}

void ScatterPlot2DView::draw() {

  destroyOverviewsIfNeeded();

  if (selectedGraphProperties.size() !=
      propertiesSelectionWidget->getSelectedGraphProperties().size()) {
    center = true;
  }

  selectedGraphProperties = propertiesSelectionWidget->getSelectedGraphProperties();

  if (selectedGraphProperties.size() < 2) {
    destroyOverviews();
    removeEmptyViewLabel();
    matrixUpdateNeeded = false;
    switchFromDetailViewToMatrixView();
    addEmptyViewLabel();
    getGlMainWidget()->getScene()->centerScene();
    getGlMainWidget()->draw();

    if (_bar)
      _bar->setEnabled(false);

    return;
  } else {
    removeEmptyViewLabel();
  }

  if (_bar)
    _bar->setEnabled(true);

  computeNodeSizes();
  buildScatterPlotsMatrix();

  if (!matrixView && detailedScatterPlot != NULL) {
    getGlMainWidget()->makeCurrent();
    detailedScatterPlot->generateOverview();
    axisComposite->reset(false);
    axisComposite->addGlEntity(detailedScatterPlot->getXAxis(), "x axis");
    axisComposite->addGlEntity(detailedScatterPlot->getYAxis(), "y axis");
    matrixUpdateNeeded = true;

    if (newGraphSet) {
      switchFromMatrixToDetailView(detailedScatterPlot, center);
      newGraphSet = false;
    }
  } else if (matrixView) {
    getGlMainWidget()->makeCurrent();
    generateScatterPlots();
  } else if (!matrixView && detailedScatterPlot == NULL) {
    switchFromDetailViewToMatrixView();
    center = true;
  }

  if (center) {
    centerView();
  } else {
    getGlMainWidget()->draw();
  }
}

void ScatterPlot2DView::centerView(bool) {
  if (!getGlMainWidget()->isVisible()) {
    if (lastViewWindowWidth != 0 && lastViewWindowHeight != 0) {
      getGlMainWidget()->getScene()->ajustSceneToSize(lastViewWindowWidth, lastViewWindowHeight);
    } else {
      getGlMainWidget()->getScene()->centerScene();
    }
  } else {
    getGlMainWidget()->getScene()->ajustSceneToSize(getGlMainWidget()->width(),
                                                    getGlMainWidget()->height());
  }

  // we apply a zoom factor to preserve a 50 px margin width
  // to ensure the scene will not be drawn under the configuration tabs title
  float glWidth = graphicsView()->width();
  getGlMainWidget()->getScene()->zoomFactor((glWidth - 50) / glWidth);
  getGlMainWidget()->draw();
  center = false;
}

void ScatterPlot2DView::applySettings() {
  if (propertiesSelectionWidget->configurationChanged() || optionsWidget->configurationChanged()) {
    viewConfigurationChanged();
    _bar->reset();
  }
}

void ScatterPlot2DView::destroyOverviewsIfNeeded() {

  vector<string> propertiesToRemove;

  for (size_t i = 0; i < selectedGraphProperties.size(); ++i) {

    if (!scatterPlotGraph || !scatterPlotGraph->existProperty(selectedGraphProperties[i])) {
      propertiesToRemove.push_back(selectedGraphProperties[i]);

      if (detailedScatterPlotPropertyName.first == selectedGraphProperties[i] ||
          detailedScatterPlotPropertyName.second == selectedGraphProperties[i]) {
        detailedScatterPlotPropertyName = make_pair("", "");
      }

      map<pair<string, string>, ScatterPlot2D *>::iterator overviewToDestroyIt;
      overviewToDestroyIt = find_if(scatterPlotsMap.begin(), scatterPlotsMap.end(),
                                    map_pair_string_key_contains(selectedGraphProperties[i]));

      while (overviewToDestroyIt != scatterPlotsMap.end()) {
        if (overviewToDestroyIt->second == detailedScatterPlot) {
          detailedScatterPlot = NULL;

          if (!matrixView) {
            GlGraphInputData *glGraphInputData = glGraphComposite->getInputData();
            glGraphInputData->setElementLayout(
                scatterPlotGraph->getProperty<LayoutProperty>("viewLayout"));
          }
        }

        delete overviewToDestroyIt->second;
        scatterPlotsGenMap.erase(overviewToDestroyIt->first);
        scatterPlotsMap.erase(overviewToDestroyIt);
        overviewToDestroyIt = find_if(scatterPlotsMap.begin(), scatterPlotsMap.end(),
                                      map_pair_string_key_contains(selectedGraphProperties[i]));
      }
    }
  }

  for (size_t i = 0; i < propertiesToRemove.size(); ++i) {
    selectedGraphProperties.erase(remove(selectedGraphProperties.begin(),
                                         selectedGraphProperties.end(), propertiesToRemove[i]),
                                  selectedGraphProperties.end());
  }

  if (!propertiesToRemove.empty()) {
    propertiesSelectionWidget->setSelectedProperties(selectedGraphProperties);
  }
}

void ScatterPlot2DView::destroyOverviews() {
  for (map<pair<string, string>, ScatterPlot2D *>::iterator it = scatterPlotsMap.begin();
       it != scatterPlotsMap.end(); ++it) {
    matrixComposite->deleteGlEntity(it->second);
    delete it->second;
  }

  scatterPlotsMap.clear();
  detailedScatterPlot = NULL;
  GlSimpleEntity *grid = matrixComposite->findGlEntity("grid");
  matrixComposite->deleteGlEntity(grid);
  delete grid;
  labelsComposite->reset(true);
  mainLayer->addGlEntity(glGraphComposite, "graph");
}

void ScatterPlot2DView::generateScatterPlots() {

  if (selectedGraphProperties.empty())
    return;

  GlLabel *coeffLabel = NULL;

  if (matrixView) {
    mainLayer->deleteGlEntity(matrixComposite);
  } else {
    mainLayer->deleteGlEntity(axisComposite);
    mainLayer->addGlEntity(glGraphComposite, "graph");
    coeffLabel = dynamic_cast<GlLabel *>(mainLayer->findGlEntity("coeffLabel"));
    mainLayer->deleteGlEntity("coeffLabel");
  }

  unsigned int nbOverviews =
      (selectedGraphProperties.size() - 1) * selectedGraphProperties.size() / 2;
  unsigned currentStep = 0;

  double sceneRadiusBak = getGlMainWidget()->getScene()->getGraphCamera().getSceneRadius();
  double zoomFactorBak = getGlMainWidget()->getScene()->getGraphCamera().getZoomFactor();
  Coord eyesBak = getGlMainWidget()->getScene()->getGraphCamera().getEyes();
  Coord centerBak = getGlMainWidget()->getScene()->getGraphCamera().getCenter();
  Coord upBak = getGlMainWidget()->getScene()->getGraphCamera().getUp();

  GlProgressBar *progressBar = new GlProgressBar(Coord(0.0f, 0.0f, 0.0f), 600.0f, 100.0f,
                                                 // use same green color as the highlighting one
                                                 // in workspace panel
                                                 Color(0xCB, 0xDE, 0x5D));
  progressBar->setComment("Updating scatter plot matrix...");
  progressBar->progress(currentStep, nbOverviews);
  mainLayer->addGlEntity(progressBar, "progress bar");
  centerView();
  getGlMainWidget()->draw();

  // disable user input
  tlp::disableQtUserInput();

  for (size_t i = 0; i < selectedGraphProperties.size() - 1; ++i) {
    for (size_t j = 0; j < selectedGraphProperties.size(); ++j) {
      ScatterPlot2D *overview =
          scatterPlotsMap[make_pair(selectedGraphProperties[i], selectedGraphProperties[j])];

      if (!overview)
        continue;

      overview->generateOverview();
      scatterPlotsGenMap[make_pair(selectedGraphProperties[i], selectedGraphProperties[j])] = true;

      currentStep += 1;
      progressBar->progress(currentStep, nbOverviews);

      // needed to display progressBar
      if ((i + 1) * (j + 1) % 10 == 0)
        getGlMainWidget()->draw();

      QApplication::processEvents();
    }
  }

  tlp::enableQtUserInput();

  mainLayer->deleteGlEntity(progressBar);
  delete progressBar;

  if (matrixView) {
    mainLayer->addGlEntity(matrixComposite, "matrix composite");
  } else {
    mainLayer->addGlEntity(axisComposite, "axis composite");

    if (coeffLabel != NULL) {
      mainLayer->addGlEntity(coeffLabel, "coeffLabel");
    }

    mainLayer->addGlEntity(detailedScatterPlot->getGlGraphComposite(), "graph");
  }

  getGlMainWidget()->getScene()->getGraphCamera().setSceneRadius(sceneRadiusBak);
  getGlMainWidget()->getScene()->getGraphCamera().setZoomFactor(zoomFactorBak);
  getGlMainWidget()->getScene()->getGraphCamera().setEyes(eyesBak);
  getGlMainWidget()->getScene()->getGraphCamera().setCenter(centerBak);
  getGlMainWidget()->getScene()->getGraphCamera().setUp(upBak);

  getGlMainWidget()->draw();
}

void ScatterPlot2DView::generateScatterPlot(ScatterPlot2D *scatterPlot, GlMainWidget *glWidget) {
  scatterPlot->generateOverview(glWidget);
  scatterPlotsGenMap[make_pair(scatterPlot->getXDim(), scatterPlot->getYDim())] = true;
}

void ScatterPlot2DView::switchFromMatrixToDetailView(ScatterPlot2D *scatterPlot, bool recenter) {

  sceneRadiusBak = getGlMainWidget()->getScene()->getGraphCamera().getSceneRadius();
  zoomFactorBak = getGlMainWidget()->getScene()->getGraphCamera().getZoomFactor();
  eyesBak = getGlMainWidget()->getScene()->getGraphCamera().getEyes();
  centerBak = getGlMainWidget()->getScene()->getGraphCamera().getCenter();
  upBak = getGlMainWidget()->getScene()->getGraphCamera().getUp();

  mainLayer->deleteGlEntity(matrixComposite);
  GlQuantitativeAxis *xAxis = scatterPlot->getXAxis();
  GlQuantitativeAxis *yAxis = scatterPlot->getYAxis();
  axisComposite->addGlEntity(xAxis, "x axis");
  axisComposite->addGlEntity(yAxis, "y axis");
  mainLayer->addGlEntity(axisComposite, "axis composite");
  GlLabel *coeffLabel = new GlLabel(
      Coord(xAxis->getAxisBaseCoord().getX() + (1.0f / 2.0f) * xAxis->getAxisLength(),
            yAxis->getAxisBaseCoord().getY() - 260),
      Size(xAxis->getAxisLength() / 2.0f, yAxis->getLabelHeight()), xAxis->getAxisColor());
  ostringstream oss;
  oss << "correlation coefficient = " << scatterPlot->getCorrelationCoefficient();
  coeffLabel->setText(oss.str());
  mainLayer->addGlEntity(coeffLabel, "coeffLabel");
  mainLayer->addGlEntity(scatterPlot->getGlGraphComposite(), "graph");
  toggleInteractors(true);
  matrixView = false;
  detailedScatterPlot = scatterPlot;
  detailedScatterPlotPropertyName = make_pair(scatterPlot->getXDim(), scatterPlot->getYDim());
  propertiesSelectionWidget->setWidgetEnabled(false);
  optionsWidget->setWidgetEnabled(true);
  optionsWidget->useCustomXAxisScale(detailedScatterPlot->getXAxisScaleDefined());
  optionsWidget->setXAxisScale(detailedScatterPlot->getXAxisScale());
  optionsWidget->useCustomYAxisScale(detailedScatterPlot->getYAxisScaleDefined());
  optionsWidget->setYAxisScale(detailedScatterPlot->getYAxisScale());
  optionsWidget->setInitXAxisScale(detailedScatterPlot->getInitXAxisScale());
  optionsWidget->setInitYAxisScale(detailedScatterPlot->getInitYAxisScale());
  optionsWidget->configurationChanged();

  if (recenter)
    centerView();
}

void ScatterPlot2DView::switchFromDetailViewToMatrixView() {

  axisComposite->reset(false);
  mainLayer->deleteGlEntity("coeffLabel");

  if (matrixUpdateNeeded) {
    generateScatterPlots();
    matrixUpdateNeeded = false;
  }

  mainLayer->addGlEntity(glGraphComposite, "graph");
  mainLayer->addGlEntity(matrixComposite, "matrix composite");
  GlScene *scene = getGlMainWidget()->getScene();
  Camera &cam = scene->getGraphCamera();
  cam.setSceneRadius(sceneRadiusBak);
  cam.setZoomFactor(zoomFactorBak);
  cam.setEyes(eyesBak);
  cam.setCenter(centerBak);
  cam.setUp(upBak);
  scene->setBackgroundColor(optionsWidget->getBackgroundColor());
  matrixView = true;
  detailedScatterPlot = NULL;
  detailedScatterPlotPropertyName = make_pair("", "");
  propertiesSelectionWidget->setWidgetEnabled(true);
  optionsWidget->setWidgetEnabled(false);
  optionsWidget->resetAxisScale();
  toggleInteractors(false);
  getGlMainWidget()->draw();
}

void ScatterPlot2DView::refresh() {
  getGlMainWidget()->redraw();
}

void ScatterPlot2DView::init() {
  emit drawNeeded();
}

BoundingBox ScatterPlot2DView::getMatrixBoundingBox() {
  GlBoundingBoxSceneVisitor glBBSV(NULL);
  matrixComposite->acceptVisitor(&glBBSV);
  return glBBSV.getBoundingBox();
}

std::vector<ScatterPlot2D *> ScatterPlot2DView::getSelectedScatterPlots() const {
  vector<ScatterPlot2D *> ret;
  map<pair<string, string>, ScatterPlot2D *>::const_iterator it;

  for (it = scatterPlotsMap.begin(); it != scatterPlotsMap.end(); ++it) {
    // a scatter plot is selected if non null
    // and if the property on the x axis is before the property on the y axis
    // in the selectedGraphProperties vector
    if (!it->second)
      continue;

    // properties on x and y axis
    const string &xProp = (it->first).first;
    const string &yProp = (it->first).second;
    // position in the selectedGraphProperties of the property on the x axis
    int xPos = -1;
    bool valid = false;

    for (unsigned int i = 0; i < selectedGraphProperties.size(); ++i) {
      const string &prop = selectedGraphProperties[i];

      if (prop == xProp) {
        xPos = i;
        continue;
      }

      if (prop == yProp) {
        if (xPos != -1)
          valid = true;

        break;
      }
    }

    if (valid)
      ret.push_back(it->second);
  }

  return ret;
}

void ScatterPlot2DView::interactorsInstalled(const QList<tlp::Interactor *> &) {
  toggleInteractors(false);
}

void ScatterPlot2DView::registerTriggers() {
  foreach (tlp::Observable *obs, triggers()) { removeRedrawTrigger(obs); }

  if (graph()) {
    addRedrawTrigger(graph());
    Iterator<string> *it = getScatterPlotGraph()->getProperties();

    while (it->hasNext()) {
      PropertyInterface *property = getScatterPlotGraph()->getProperty(it->next());
      addRedrawTrigger(property);
    }

    delete it;
  }
}

void ScatterPlot2DView::treatEvent(const Event &message) {
  const GraphEvent *graphEvent = dynamic_cast<const GraphEvent *>(&message);

  if (graphEvent) {
    if (graphEvent->getType() == GraphEvent::TLP_ADD_EDGE)
      addEdge(graphEvent->getGraph(), graphEvent->getEdge());

    if (graphEvent->getType() == GraphEvent::TLP_DEL_NODE)
      delNode(graphEvent->getGraph(), graphEvent->getNode());

    if (graphEvent->getType() == GraphEvent::TLP_DEL_EDGE)
      delEdge(graphEvent->getGraph(), graphEvent->getEdge());
  }

  const PropertyEvent *propertyEvent = dynamic_cast<const PropertyEvent *>(&message);

  if (propertyEvent) {
    if (propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_NODE_VALUE)
      afterSetNodeValue(propertyEvent->getProperty(), propertyEvent->getNode());

    if (propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_EDGE_VALUE)
      afterSetEdgeValue(propertyEvent->getProperty(), propertyEvent->getEdge());

    if (propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE)
      afterSetAllNodeValue(propertyEvent->getProperty());

    if (propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE)
      afterSetAllEdgeValue(propertyEvent->getProperty());
  }
}

void ScatterPlot2DView::afterSetNodeValue(PropertyInterface *p, const node n) {
  if (p->getGraph() == edgeAsNodeGraph && p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection = static_cast<BooleanProperty *>(p);
    BooleanProperty *viewSelection =
        scatterPlotGraph->getProperty<BooleanProperty>("viewSelection");
    viewSelection->removeListener(this);
    viewSelection->setEdgeValue(nodeToEdge[n], edgeAsNodeGraphSelection->getNodeValue(n));
    viewSelection->addListener(this);
    return;
  }
}

void ScatterPlot2DView::afterSetEdgeValue(PropertyInterface *p, const edge e) {
  if (edgeToNode.find(e) == edgeToNode.end())
    return;

  if (p->getName() == "viewColor") {
    ColorProperty *edgeAsNodeGraphColors = edgeAsNodeGraph->getProperty<ColorProperty>("viewColor");
    ColorProperty *viewColor = static_cast<ColorProperty *>(p);
    edgeAsNodeGraphColors->setNodeValue(edgeToNode[e], viewColor->getEdgeValue(e));
  } else if (p->getName() == "viewLabel") {
    StringProperty *edgeAsNodeGraphLabels =
        edgeAsNodeGraph->getProperty<StringProperty>("viewLabel");
    StringProperty *viewLabel = static_cast<StringProperty *>(p);
    edgeAsNodeGraphLabels->setNodeValue(edgeToNode[e], viewLabel->getEdgeValue(e));
  } else if (p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection =
        edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection");
    BooleanProperty *viewSelection = static_cast<BooleanProperty *>(p);
    edgeAsNodeGraphSelection->removeListener(this);

    if (edgeAsNodeGraphSelection->getNodeValue(edgeToNode[e]) != viewSelection->getEdgeValue(e))
      edgeAsNodeGraphSelection->setNodeValue(edgeToNode[e], viewSelection->getEdgeValue(e));

    edgeAsNodeGraphSelection->addListener(this);
  }
}

void ScatterPlot2DView::afterSetAllNodeValue(PropertyInterface *p) {
  if (p->getName() == "viewSelection") {
    if (p->getGraph() == edgeAsNodeGraph) {
      BooleanProperty *edgeAsNodeGraphSelection = static_cast<BooleanProperty *>(p);
      BooleanProperty *viewSelection =
          scatterPlotGraph->getProperty<BooleanProperty>("viewSelection");
      viewSelection->setAllEdgeValue(
          edgeAsNodeGraphSelection->getNodeValue(edgeAsNodeGraph->getOneNode()));
    }
  }
}

void ScatterPlot2DView::afterSetAllEdgeValue(PropertyInterface *p) {

  if (p->getName() == "viewColor") {
    ColorProperty *edgeAsNodeGraphColors = edgeAsNodeGraph->getProperty<ColorProperty>("viewColor");
    ColorProperty *viewColor = static_cast<ColorProperty *>(p);
    edgeAsNodeGraphColors->setAllNodeValue(viewColor->getEdgeDefaultValue());
  } else if (p->getName() == "viewLabel") {
    StringProperty *edgeAsNodeGraphLabels =
        edgeAsNodeGraph->getProperty<StringProperty>("viewLabel");
    StringProperty *viewLabel = static_cast<StringProperty *>(p);
    edgeAsNodeGraphLabels->setAllNodeValue(viewLabel->getEdgeDefaultValue());
  } else if (p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection =
        edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection");
    BooleanProperty *viewSelection = static_cast<BooleanProperty *>(p);
    edge e;
    forEach(e, scatterPlotGraph->getEdges()) {
      if (edgeAsNodeGraphSelection->getNodeValue(edgeToNode[e]) != viewSelection->getEdgeValue(e)) {
        edgeAsNodeGraphSelection->setNodeValue(edgeToNode[e], viewSelection->getEdgeValue(e));
      }
    }
  }
}

void ScatterPlot2DView::addEdge(Graph *, const edge e) {
  edgeToNode[e] = edgeAsNodeGraph->addNode();
}

void ScatterPlot2DView::delNode(Graph *, const node) {}

void ScatterPlot2DView::delEdge(Graph *, const edge e) {
  edgeAsNodeGraph->delNode(edgeToNode[e]);
  edgeToNode.erase(e);
}

unsigned int ScatterPlot2DView::getMappedId(unsigned int id) {
  if (dataLocation == EDGE)
    return nodeToEdge[node(id)].id;

  return id;
}
}
