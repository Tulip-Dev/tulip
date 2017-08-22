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

#include <tulip/ForEach.h>
#include <tulip/GlQuantitativeAxis.h>
#include <tulip/TlpTools.h>
#include <tulip/Interactor.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlLabel.h>
#include <tulip/GlRect.h>
#include <tulip/GlTextureManager.h>
#include <tulip/TlpQtTools.h>
#include <tulip/QuickAccessBar.h>
#include <tulip/TulipViewSettings.h>

#include <QToolTip>
#include <QHelpEvent>
#include <QApplication>
#include <QMenu>

#include "HistogramView.h"
#include "HistogramInteractors.h"
#include "../utils/ViewGraphPropertiesSelectionWidget.h"
#include "HistoOptionsWidget.h"

using namespace std;

const unsigned int OVERVIEW_SIZE = 512;

const string propertiesTypes[] = {"double", "int"};
const unsigned int nbPropertiesTypes = sizeof(propertiesTypes) / sizeof(string);
const vector<string> propertiesTypesFilter(propertiesTypes, propertiesTypes + nbPropertiesTypes);

template <typename T>
std::string getStringFromNumber(T number, unsigned int precision = 5) {
  std::ostringstream oss;
  oss.precision(precision);
  oss << number;
  return oss.str();
}

namespace tlp {

PLUGIN(HistogramView)

GLuint HistogramView::binTextureId(0);
unsigned int HistogramView::histoViewInstancesCount(0);


HistogramView::HistogramView(const PluginContext *) : _bar(NULL),
  propertiesSelectionWidget(NULL), histoOptionsWidget(NULL), xAxisDetail(NULL), yAxisDetail(NULL), _histoGraph(NULL), emptyGraph(NULL),
  emptyGlGraphComposite(NULL), histogramsComposite(NULL), labelsComposite(NULL), axisComposite(NULL), smallMultiplesView(true), mainLayer(NULL), detailedHistogram(NULL),
  sceneRadiusBak(0), zoomFactorBak(0),
  noDimsLabel(NULL), noDimsLabel1(NULL), noDimsLabel2(NULL), emptyRect(NULL),
  emptyRect2(NULL), interactorsActivated(false), isConstruct(false), lastNbHistograms(0), dataLocation(NODE), needUpdateHistogram(false), edgeAsNodeGraph(NULL) {
  ++histoViewInstancesCount;
}

HistogramView::~HistogramView() {
  if(isConstruct) {
    if(currentInteractor()!=NULL)
      currentInteractor()->uninstall();

    --histoViewInstancesCount;

    if (histoViewInstancesCount == 0) {
      GlTextureManager::getInst().deleteTexture(BIN_RECT_TEXTURE);
      binTextureId = 0;
    }

    delete propertiesSelectionWidget;
    delete histoOptionsWidget;
    delete emptyGlGraphComposite;
    delete labelsComposite;
    delete emptyGraph;
    delete axisComposite;
    delete edgeAsNodeGraph;
  }
}

QList<QWidget*> HistogramView::configurationWidgets() const {
  return QList<QWidget *>() << propertiesSelectionWidget << histoOptionsWidget ;
}

void HistogramView::initGlWidget(Graph *) {
  GlLayer *layer = getGlMainWidget()->getScene()->getLayer("Main");

  if (layer == NULL) {
    layer = new GlLayer("Main");
    getGlMainWidget()->getScene()->addExistingLayer(layer);
  }

  mainLayer = layer;

  cleanupGlScene();

  if (emptyGlGraphComposite == NULL) {
    emptyGraph = newGraph();
    emptyGlGraphComposite = new GlGraphComposite(emptyGraph);
  }

  mainLayer->addGlEntity(emptyGlGraphComposite, "graph");

  if (histogramsComposite == NULL) {
    histogramsComposite = new GlComposite();
    mainLayer->addGlEntity(histogramsComposite, "overviews composite");
  }

  if (labelsComposite == NULL) {
    labelsComposite = new GlComposite();
    mainLayer->addGlEntity(labelsComposite, "labels composite");
  }

  if (axisComposite == NULL) {
    axisComposite = new GlComposite();
  }
}

void HistogramView::cleanupGlScene() {
  if (!smallMultiplesView && detailedHistogram != NULL) {
    mainLayer->deleteGlEntity(detailedHistogram->getBinsComposite());
  }

  if (axisComposite != NULL) {
    axisComposite->reset(false);
  }

  if (labelsComposite != NULL) {
    labelsComposite->reset(true);
  }

  if (histogramsComposite != NULL) {
    histogramsComposite->reset(true);
    histogramsMap.clear();
  }
}

QuickAccessBar *HistogramView::getQuickAccessBarImpl() {
  _bar = new QuickAccessBarImpl(NULL,QuickAccessBarImpl::QuickAccessButtons(QuickAccessBarImpl::SCREENSHOT|QuickAccessBarImpl::BACKGROUNDCOLOR|QuickAccessBarImpl::SHOWLABELS
                                |QuickAccessBarImpl::LABELSSCALED|QuickAccessBarImpl::SHOWEDGES
                                |QuickAccessBarImpl::NODECOLOR|QuickAccessBarImpl::EDGECOLOR|QuickAccessBarImpl::NODEBORDERCOLOR
                                |QuickAccessBarImpl::LABELCOLOR
                                                                           ));
  return _bar;
}

void HistogramView::setState(const DataSet &dataSet) {

  GlMainWidget *gl = getGlMainWidget();

  if(!isConstruct) {
    isConstruct=true;
    gl->installEventFilter(this);
    setOverviewVisible(true);

    propertiesSelectionWidget = new ViewGraphPropertiesSelectionWidget();
    histoOptionsWidget = new HistoOptionsWidget();
    propertiesSelectionWidget->setWidgetEnabled(true);
    histoOptionsWidget->setWidgetEnabled(false);
  }

  if (binTextureId == 0) {
    gl->getFirstQGLWidget()->makeCurrent();
    binTextureId = gl->getFirstQGLWidget()->bindTexture(QImage(":/histo_texture.png").transformed(QTransform().rotate(90)), GL_TEXTURE_2D);
    GlTextureManager::getInst().registerExternalTexture(BIN_RECT_TEXTURE, binTextureId);
  }

  GlMainView::setState(dataSet);

  Graph *lastGraph = _histoGraph;
  _histoGraph = graph();
  destroyHistogramsIfNeeded();

  if (lastGraph == NULL || lastGraph != _histoGraph) {
    if (lastGraph) {
      lastGraph->removeListener(this);
      lastGraph->getProperty("viewColor")->removeListener(this);
      lastGraph->getProperty("viewLabel")->removeListener(this);
      lastGraph->getProperty("viewSize")->removeListener(this);
      lastGraph->getProperty("viewShape")->removeListener(this);
      lastGraph->getProperty("viewSelection")->removeListener(this);
      lastGraph->getProperty("viewTexture")->removeListener(this);
    }

    initGlWidget(graph());
    detailedHistogram = NULL;

    if (edgeAsNodeGraph)
      delete edgeAsNodeGraph;

    if (_histoGraph) {
      edgeAsNodeGraph = tlp::newGraph();
      edgeToNode.clear();
      nodeToEdge.clear();
      edge e;
      forEach(e, _histoGraph->getEdges()) {
        nodeToEdge[edgeToNode[e] = edgeAsNodeGraph->addNode()] = e;
        edgeAsNodeGraph->getProperty<ColorProperty>("viewColor")->setNodeValue(edgeToNode[e],
            _histoGraph->getProperty<ColorProperty>("viewColor")->getEdgeValue(e));
        edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection")->setNodeValue(edgeToNode[e],
            _histoGraph->getProperty<BooleanProperty>("viewSelection")->getEdgeValue(e));
        edgeAsNodeGraph->getProperty<StringProperty>("viewLabel")->setNodeValue(edgeToNode[e],
            _histoGraph->getProperty<StringProperty>("viewLabel")->getEdgeValue(e));
      }
      edgeAsNodeGraph->getProperty<IntegerProperty>("viewShape")->setAllNodeValue(NodeShape::Circle);
      edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection")->addListener(this);
      _histoGraph->addListener(this);
      _histoGraph->getProperty("viewColor")->addListener(this);
      _histoGraph->getProperty("viewLabel")->addListener(this);
      _histoGraph->getProperty("viewSize")->addListener(this);
      _histoGraph->getProperty("viewShape")->addListener(this);
      _histoGraph->getProperty("viewSelection")->addListener(this);
      _histoGraph->getProperty("viewTexture")->addListener(this);
    }
  }

  propertiesSelectionWidget->setWidgetParameters(graph(), propertiesTypesFilter);

  dataSet.get("histo detailed name", detailedHistogramPropertyName);
  Color backgroundColor;

  if (dataSet.get("backgroundColor", backgroundColor)) {
    histoOptionsWidget->setBackgroundColor(backgroundColor);
  }

  map<string, DataSet> histogramParametersMap;
  DataSet histogramParameters;
  int i=0;
  stringstream ss;
  ss<<i;

  while (dataSet.get("histo"+ss.str(), histogramParameters)) {
    string propertyName;
    histogramParameters.get("property name", propertyName);
    selectedProperties.push_back(propertyName);
    histogramParametersMap[propertyName] = histogramParameters;

    ss.str("");
    ss << ++i;
  }

  propertiesSelectionWidget->setSelectedProperties(selectedProperties);

  if (!selectedProperties.empty()) {
    buildHistograms();

    for (size_t j = 0 ; j < selectedProperties.size() ; ++j) {
      unsigned int nbHistogramBins = 0;

      Histogram *histo = histogramsMap[selectedProperties[j]];

      if (histogramParametersMap[selectedProperties[j]].get("nb histogram bins", nbHistogramBins)) {
        histo->setLayoutUpdateNeeded();
        histo->setNbHistogramBins(nbHistogramBins);
      }

      unsigned int nbXGraduations = 0;

      if (histogramParametersMap[selectedProperties[j]].get("x axis nb graduations", nbXGraduations)) {
        histo->setLayoutUpdateNeeded();
        histo->setNbXGraduations(nbXGraduations);
      }

      unsigned int yAxisIncrementStep = 0;

      if (histogramParametersMap[selectedProperties[j]].get("y axis increment step", yAxisIncrementStep)) {
        histo->setLayoutUpdateNeeded();
        histo->setYAxisIncrementStep(yAxisIncrementStep);
      }

      bool cumulativeFrequenciesHisto = false;

      if (histogramParametersMap[selectedProperties[j]].get("cumulative frequencies histogram", cumulativeFrequenciesHisto)) {
        histo->setLayoutUpdateNeeded();
        histo->setCumulativeHistogram(cumulativeFrequenciesHisto);
        histo->setLastCumulativeHistogram(cumulativeFrequenciesHisto);
      }

      bool uniformQuantification = false;

      if (histogramParametersMap[selectedProperties[j]].get("uniform quantification", uniformQuantification)) {
        histo->setLayoutUpdateNeeded();
        histo->setUniformQuantification(uniformQuantification);
      }

      bool xAxisLogScale = false;

      if (histogramParametersMap[selectedProperties[j]].get("x axis logscale", xAxisLogScale)) {
        histo->setLayoutUpdateNeeded();
        histo->setXAxisLogScale(xAxisLogScale);
      }

      bool yAxisLogScale = false;

      if (histogramParametersMap[selectedProperties[j]].get("y axis logscale", yAxisLogScale)) {
        histo->setLayoutUpdateNeeded();
        histo->setYAxisLogScale(yAxisLogScale);
      }

      bool useCustomAxisScale = false;

      if (histogramParametersMap[selectedProperties[j]].get("x axis custom scale", useCustomAxisScale)) {
        histo->setLayoutUpdateNeeded();
        histo->setXAxisScaleDefined(useCustomAxisScale);

        if (useCustomAxisScale) {
          std::pair<double, double> axisScale(0, 0);
          histogramParametersMap[selectedProperties[j]].get("x axis scale min",
              axisScale.first);
          histogramParametersMap[selectedProperties[j]].get("x axis scale max",
              axisScale.second);
          histo->setXAxisScale(axisScale);
        }
      }

      if (histogramParametersMap[selectedProperties[j]].get("y axis custom scale", useCustomAxisScale)) {
        histo->setLayoutUpdateNeeded();
        histo->setYAxisScaleDefined(useCustomAxisScale);

        if (useCustomAxisScale) {
          std::pair<double, double> axisScale(0, 0);
          histogramParametersMap[selectedProperties[j]].get("y axis scale min",
              axisScale.first);
          histogramParametersMap[selectedProperties[j]].get("y axis scale max",
              axisScale.second);
          histo->setXAxisScale(axisScale);
        }
      }
    }
  }

  unsigned nodes=NODE;
  dataSet.get("Nodes/Edges", nodes);
  dataLocation = static_cast<ElementType>(nodes);
  propertiesSelectionWidget->setDataLocation(dataLocation);
  viewConfigurationChanged();

  registerTriggers();

  if (!detailedHistogramPropertyName.empty()) {
    Histogram *histo = histogramsMap[detailedHistogramPropertyName];
    histo->update();
    switchFromSmallMultiplesToDetailedView(histo);
  }

  bool quickAccessBarVisible=false;

  if (dataSet.get<bool>("quickAccessBarVisible", quickAccessBarVisible)) {
    needQuickAccessBar = true;
    setQuickAccessBarVisible(quickAccessBarVisible);
  }
  else
    setQuickAccessBarVisible(true);
}

DataSet HistogramView::state() const {
  vector<string> selectedPropertiesTmp=vector<string>(selectedProperties);
  map<string, Histogram *> histogramsMapTmp=map<string, Histogram *>(histogramsMap);

  DataSet dataSet = GlMainView::state();
  dataSet.set("Nodes/Edges", static_cast<unsigned>(dataLocation));

  for (size_t i = 0 ; i < selectedPropertiesTmp.size() ; ++i) {
    std::stringstream ss;
    ss << i;
    DataSet histogramParameters;
    histogramParameters.set("property name", selectedPropertiesTmp[i]);
    histogramParameters.set("nb histogram bins", histogramsMapTmp[selectedPropertiesTmp[i]]->getNbHistogramBins());
    histogramParameters.set("x axis nb graduations", histogramsMapTmp[selectedPropertiesTmp[i]]->getNbXGraduations());
    histogramParameters.set("y axis increment step", histogramsMapTmp[selectedPropertiesTmp[i]]->getYAxisIncrementStep());
    histogramParameters.set("cumulative frequencies histogram", histogramsMapTmp[selectedPropertiesTmp[i]]->cumulativeFrequenciesHistogram());
    histogramParameters.set("uniform quantification", histogramsMapTmp[selectedPropertiesTmp[i]]->uniformQuantificationHistogram());
    histogramParameters.set("x axis logscale", histogramsMapTmp[selectedPropertiesTmp[i]]->xAxisLogScaleSet());
    histogramParameters.set("y axis logscale", histogramsMapTmp[selectedPropertiesTmp[i]]->yAxisLogScaleSet());
    bool customScale = histogramsMapTmp[selectedPropertiesTmp[i]]->getXAxisScaleDefined();
    histogramParameters.set("x axis custom scale", customScale);

    if (customScale) {
      const std::pair<double, double>& scale = histogramsMapTmp[selectedPropertiesTmp[i]]->getXAxisScale();
      histogramParameters.set("x axis scale min", scale.first);
      histogramParameters.set("x axis scale max", scale.second);
    }

    customScale = histogramsMapTmp[selectedPropertiesTmp[i]]->getYAxisScaleDefined();
    histogramParameters.set("y axis custom scale", customScale);

    if (customScale) {
      const std::pair<double, double>& scale = histogramsMapTmp[selectedPropertiesTmp[i]]->getYAxisScale();
      histogramParameters.set("y axis scale min", scale.first);
      histogramParameters.set("y axis scale max", scale.second);
    }

    dataSet.set("histo"+ss.str(), histogramParameters);
  }

  dataSet.set("backgroundColor", getGlMainWidget()->getScene()->getBackgroundColor());
  string histoDetailedNamed = "";

  if (detailedHistogram != NULL) {
    histoDetailedNamed = detailedHistogram->getPropertyName();
  }

  dataSet.set("histo detailed name", histoDetailedNamed);

  if (needQuickAccessBar)
    dataSet.set("quickAccessBarVisible", quickAccessBarVisible());

  return dataSet;
}

bool HistogramView::eventFilter(QObject *object, QEvent *event) {
  if (xAxisDetail != NULL && event->type() == QEvent::ToolTip && !detailedHistogram->uniformQuantificationHistogram()) {
    GlMainWidget* glw = getGlMainWidget();
    QHelpEvent *he = static_cast<QHelpEvent *>(event);
    int x = glw->width() - he->x();
    int y = he->y();
    Coord screenCoords(x, y, 0);
    Coord sceneCoords(glw->getScene()->getLayer("Main")->getCamera().viewportTo3DWorld(glw->screenToViewport(screenCoords)));
    BoundingBox xAxisBB = xAxisDetail->getBoundingBox();

    if (sceneCoords.getX() > xAxisBB[0][0] && sceneCoords.getX() < xAxisBB[1][0] &&
        sceneCoords.getY() > xAxisBB[0][1] && sceneCoords.getY() < xAxisBB[1][1]) {
      double val = xAxisDetail->getValueForAxisPoint(sceneCoords);
      string valStr(getStringFromNumber(val));
      QToolTip::showText(he->globalPos(), tlp::tlpStringToQString(valStr));
    }

    return true;
  }

  return GlMainView::eventFilter(object,event);
}

void HistogramView::addEmptyViewLabel() {

  Color backgroundColor(histoOptionsWidget->getBackgroundColor());
  getGlMainWidget()->getScene()->setBackgroundColor(backgroundColor);

  Color foregroundColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    foregroundColor = Color(255,255,255);
  }
  else {
    foregroundColor = Color(0,0,0);
  }

  if (noDimsLabel == NULL) {
    noDimsLabel = new GlLabel(Coord(0,0,0), Size(200,200), foregroundColor);
    noDimsLabel->setText(ViewName::HistogramViewName);
    noDimsLabel1 = new GlLabel(Coord(0,-50,0), Size(400,200), foregroundColor);
    noDimsLabel1->setText("No graph properties selected.");
    noDimsLabel2 = new GlLabel(Coord(0,-100,0), Size(700,200), foregroundColor);
    noDimsLabel2->setText("Go to the \"Properties\" tab in top right corner.");
  }
  else {
    noDimsLabel->setColor(foregroundColor);
    noDimsLabel1->setColor(foregroundColor);
    noDimsLabel2->setColor(foregroundColor);
  }

  mainLayer->addGlEntity(noDimsLabel, "no dimensions label");
  mainLayer->addGlEntity(noDimsLabel1, "no dimensions label 1");
  mainLayer->addGlEntity(noDimsLabel2, "no dimensions label 2");
}

void HistogramView::removeEmptyViewLabel() {
  if (noDimsLabel != NULL) {
    mainLayer->deleteGlEntity(noDimsLabel);
    delete noDimsLabel;
    noDimsLabel = NULL;
    mainLayer->deleteGlEntity(noDimsLabel1);
    delete noDimsLabel1;
    noDimsLabel1 = NULL;
    mainLayer->deleteGlEntity(noDimsLabel2);
    delete noDimsLabel2;
    noDimsLabel2 = NULL;
  }
}

void HistogramView::viewConfigurationChanged() {
  getGlMainWidget()->getScene()->setBackgroundColor(histoOptionsWidget->getBackgroundColor());
  bool dataLocationChanged = propertiesSelectionWidget->getDataLocation() != dataLocation;

  if (dataLocationChanged) {
    histogramsComposite->reset(true);
    axisComposite->reset(false);
    histogramsMap.clear();
    detailedHistogram = NULL;
  }

  buildHistograms();

  if (detailedHistogram != NULL && lastNbHistograms != 0 && !dataLocationChanged) {

    detailedHistogram->setNbHistogramBins(histoOptionsWidget->getNbOfHistogramBins());
    detailedHistogram->setNbXGraduations(histoOptionsWidget->getNbXGraduations());
    detailedHistogram->setYAxisIncrementStep(histoOptionsWidget->getYAxisIncrementStep());
    detailedHistogram->setXAxisLogScale(histoOptionsWidget->xAxisLogScaleSet());
    detailedHistogram->setYAxisLogScale(histoOptionsWidget->yAxisLogScaleSet());
    detailedHistogram->setCumulativeHistogram(histoOptionsWidget->cumulativeFrequenciesHisto());
    detailedHistogram->setUniformQuantification(histoOptionsWidget->uniformQuantification());
    detailedHistogram->setDisplayGraphEdges(histoOptionsWidget->showGraphEdges());
    detailedHistogram->setXAxisScaleDefined(histoOptionsWidget->useCustomXAxisScale());
    detailedHistogram->setXAxisScale(histoOptionsWidget->getXAxisScale());
    detailedHistogram->setYAxisScaleDefined(histoOptionsWidget->useCustomYAxisScale());
    detailedHistogram->setYAxisScale(histoOptionsWidget->getYAxisScale());
    detailedHistogram->setLayoutUpdateNeeded();
    detailedHistogram->update();
    histoOptionsWidget->setBinWidth(detailedHistogram->getHistogramBinsWidth());
    histoOptionsWidget->setYAxisIncrementStep(detailedHistogram->getYAxisIncrementStep());

  }

  updateHistograms(detailedHistogram);
  draw();
  drawOverview(true);
}

void HistogramView::draw() {
  GlMainWidget *gl = getGlMainWidget();

  if (selectedProperties.empty()) {
    if (!interactors().empty()) {
      setCurrentInteractor(interactors().front());
    }

    if (!smallMultiplesView) {
      switchFromDetailedViewToSmallMultiples();
    }

    removeEmptyViewLabel();
    addEmptyViewLabel();
    gl->centerScene();

    if(_bar)
      _bar->setEnabled(false);

    return;
  }

  if(_bar)
    _bar->setEnabled(true);

  if (detailedHistogram != NULL) {
    needUpdateHistogram=true;
    detailedHistogram->update();
    updateDetailedHistogramAxis();
  }
  else {
    updateHistograms();
  }

  if (!smallMultiplesView && detailedHistogram != NULL) {
    switchFromSmallMultiplesToDetailedView(detailedHistogram);
  }

  if (!selectedProperties.empty()) {
    removeEmptyViewLabel();
  }

  if (!smallMultiplesView && (detailedHistogram == NULL || (selectedProperties.size() > 1 && lastNbHistograms == 1))) {
    switchFromDetailedViewToSmallMultiples();
  }

  if (selectedProperties.size() == 1) {
    switchFromSmallMultiplesToDetailedView(histogramsMap[selectedProperties[0]]);
    propertiesSelectionWidget->setWidgetEnabled(true);
  }

  if (lastNbHistograms != selectedProperties.size()) {
    centerView();
    lastNbHistograms = selectedProperties.size();
    return;
  }

  gl->draw();
  lastNbHistograms = selectedProperties.size();
}

void HistogramView::refresh() {
  getGlMainWidget()->redraw();
}

void HistogramView::graphChanged(Graph *) {
  setState(DataSet());
  drawOverview();
}

void HistogramView::buildHistograms() {

  GlMainWidget::getFirstQGLWidget()->makeCurrent();

  histogramsComposite->reset(false);
  labelsComposite->reset(true);

  selectedProperties = propertiesSelectionWidget->getSelectedGraphProperties();
  dataLocation = propertiesSelectionWidget->getDataLocation();

  if (selectedProperties.empty()) {
    return;
  }

  float spaceBetweenOverviews = OVERVIEW_SIZE / 10.f;
  float labelHeight = OVERVIEW_SIZE / 6.0f;

  float squareRoot = sqrt(float(selectedProperties.size()));
  const unsigned int N =  uint(squareRoot) + (fmod(float(selectedProperties.size()), squareRoot) == 0.f ? 0u : 1u);

  Color backgroundColor(histoOptionsWidget->getBackgroundColor());
  getGlMainWidget()->getScene()->setBackgroundColor(backgroundColor);

  Color foregroundColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    foregroundColor = Color(255,255,255);
  }
  else {
    foregroundColor = Color(0,0,0);
  }

  vector<GlLabel *> propertiesLabels;
  float minSize = FLT_MIN;

  // disable user input
  // before allowing some display feedback
  tlp::disableQtUserInput();

  for (size_t i = 0 ; i < selectedProperties.size() ; ++i) {

    unsigned int row = i / N;
    unsigned int col = i % N;

    Coord overviewBLCorner(col * (OVERVIEW_SIZE + spaceBetweenOverviews), -(labelHeight + row * (OVERVIEW_SIZE + spaceBetweenOverviews + labelHeight)), 0);
    ostringstream oss;
    oss << "histogram overview for property " << selectedProperties[i];

    if (histogramsMap.find(selectedProperties[i]) == histogramsMap.end()) {
      Histogram *histoOverview = new Histogram(_histoGraph, edgeAsNodeGraph, edgeToNode, selectedProperties[i], dataLocation, overviewBLCorner, OVERVIEW_SIZE, backgroundColor, foregroundColor);
      histogramsMap[selectedProperties[i]] = histoOverview;
    }
    else {
      histogramsMap[selectedProperties[i]]->setDataLocation(dataLocation);
      histogramsMap[selectedProperties[i]]->setBLCorner(overviewBLCorner);
      histogramsMap[selectedProperties[i]]->setBackgroundColor(backgroundColor);
      histogramsMap[selectedProperties[i]]->setTextColor(foregroundColor);
    }

    histogramsComposite->addGlEntity(histogramsMap[selectedProperties[i]], oss.str());

    GlLabel *propertyLabel = new GlLabel(Coord(overviewBLCorner.getX() + OVERVIEW_SIZE / 2, overviewBLCorner.getY() - labelHeight / 2, 0),
                                         Size((8.f/10.f) * OVERVIEW_SIZE, labelHeight), foregroundColor);
    propertyLabel->setText(selectedProperties[i]);
    propertiesLabels.push_back(propertyLabel);

    if(i==0)
      minSize=propertyLabel->getHeightAfterScale();
    else {
      if(minSize>propertyLabel->getHeightAfterScale())
        minSize=propertyLabel->getHeightAfterScale();
    }

    labelsComposite->addGlEntity(propertyLabel, selectedProperties[i] + " label");

    if (selectedProperties.size() == 1 || (detailedHistogramPropertyName == selectedProperties[i])) {
      detailedHistogram = histogramsMap[selectedProperties[i]];
    }

    // add some feedback
    if (i % 10 == 0)
      QApplication::processEvents();
  }

  // reenable user input
  tlp::enableQtUserInput();


  for(vector<GlLabel *>::iterator it=propertiesLabels.begin(); it!=propertiesLabels.end(); ++it) {
    (*it)->setSize(Size((*it)->getSize()[0],minSize));
  }

}

void HistogramView::updateHistograms(Histogram *detailOverview) {
  needUpdateHistogram=false;
  getGlMainWidget()->makeCurrent();
  map<string, Histogram *>::iterator it;

  for (it = histogramsMap.begin() ; it != histogramsMap.end() ; ++it) {
    if (std::find(selectedProperties.begin(), selectedProperties.end(), it->first) != selectedProperties.end()) {
      if (it->second != detailOverview) {
        (it->second)->update();
      }
    }
  }
}

vector<Histogram *> HistogramView::getHistograms() const {
  vector<Histogram *> ret;
  map<string, Histogram *>::const_iterator it;

  for (it = histogramsMap.begin() ; it != histogramsMap.end() ; ++it) {
    if (std::find(selectedProperties.begin(), selectedProperties.end(), it->first) != selectedProperties.end()) {
      ret.push_back(it->second);
    }
  }

  return ret;
}

void HistogramView::destroyHistogramsIfNeeded() {
  vector<string> propertiesToRemove;

  for (size_t i = 0 ; i  < selectedProperties.size() ; ++i) {
    if (!_histoGraph || !_histoGraph->existProperty(selectedProperties[i])) {
      if (histogramsMap[selectedProperties[i]] == detailedHistogram) {
        if (!smallMultiplesView) {
          mainLayer->deleteGlEntity(detailedHistogram->getBinsComposite());
        }

        detailedHistogram = NULL;
      }

      propertiesToRemove.push_back(selectedProperties[i]);
      delete histogramsMap[selectedProperties[i]];
      histogramsMap.erase(selectedProperties[i]);
    }
  }

  for (size_t i = 0 ; i < propertiesToRemove.size() ; ++i) {
    selectedProperties.erase(remove(selectedProperties.begin(), selectedProperties.end(), propertiesToRemove[i]), selectedProperties.end());
  }
}

void HistogramView::switchFromSmallMultiplesToDetailedView(Histogram *histogramToDetail) {
  if(!histogramToDetail)
    return;

  if (smallMultiplesView) {
    sceneRadiusBak = getGlMainWidget()->getScene()->getGraphCamera().getSceneRadius();
    zoomFactorBak = getGlMainWidget()->getScene()->getGraphCamera().getZoomFactor();
    eyesBak = getGlMainWidget()->getScene()->getGraphCamera().getEyes();
    centerBak = getGlMainWidget()->getScene()->getGraphCamera().getCenter();
    upBak = getGlMainWidget()->getScene()->getGraphCamera().getUp();
  }

  mainLayer->deleteGlEntity(histogramsComposite);
  mainLayer->deleteGlEntity(labelsComposite);

  if (detailedHistogram)
    _histoGraph->getProperty(detailedHistogram->getPropertyName())->removeListener(this);

  detailedHistogram = histogramToDetail;
  detailedHistogramPropertyName = detailedHistogram->getPropertyName();
  _histoGraph->getProperty(detailedHistogramPropertyName)->addListener(this);

  updateDetailedHistogramAxis();

  mainLayer->addGlEntity(axisComposite, "axis composite");
  mainLayer->addGlEntity(histogramToDetail->getBinsComposite(), "bins composite");

  float offset = detailedHistogram->getYAxis()->getMaxLabelWidth() + 90;
  Coord brCoord(detailedHistogram->getYAxis()->getAxisBaseCoord() - Coord(offset,0,0));
  Coord tlCoord(detailedHistogram->getYAxis()->getAxisBaseCoord() - Coord(offset + 65,0,0) + Coord(0, detailedHistogram->getYAxis()->getAxisLength()));
  emptyRect = new GlRect(tlCoord, brCoord, Color(0,0,0,0), Color(0,0,0,0));

  float offset2 = (detailedHistogram->getXAxis()->getAxisGradsWidth() / 2.) + detailedHistogram->getXAxis()->getLabelHeight();
  Coord tlCoord2(detailedHistogram->getXAxis()->getAxisBaseCoord() - Coord(0, offset2, 0));
  Coord brCoord2(detailedHistogram->getXAxis()->getAxisBaseCoord() + Coord(detailedHistogram->getXAxis()->getAxisLength(), 0, 0) - Coord(0, offset2 + 60, 0));
  emptyRect2 = new GlRect(tlCoord2, brCoord2, Color(0,0,0,0), Color(0,0,0,0));

  mainLayer->addGlEntity(emptyRect, "emptyRect");
  mainLayer->addGlEntity(emptyRect2, "emptyRect2");

  mainLayer->addGlEntity(histogramToDetail->getGlGraphComposite(), "graph");

  toggleInteractors(true);

  if (smallMultiplesView)
    centerView();

  smallMultiplesView = false;

  if (selectedProperties.size() > 1) {
    propertiesSelectionWidget->setWidgetEnabled(false);
  }

  histoOptionsWidget->setWidgetEnabled(true);

  histoOptionsWidget->enableShowGraphEdgesCB(dataLocation == NODE);
  histoOptionsWidget->setUniformQuantification(detailedHistogram->uniformQuantificationHistogram());
  histoOptionsWidget->setNbOfHistogramBins(detailedHistogram->getNbHistogramBins());
  histoOptionsWidget->setBinWidth(detailedHistogram->getHistogramBinsWidth());
  histoOptionsWidget->setYAxisIncrementStep(detailedHistogram->getYAxisIncrementStep());
  histoOptionsWidget->setYAxisLogScale(detailedHistogram->yAxisLogScaleSet());
  histoOptionsWidget->setNbXGraduations(detailedHistogram->getNbXGraduations());
  histoOptionsWidget->setXAxisLogScale(detailedHistogram->xAxisLogScaleSet());
  histoOptionsWidget->setCumulativeFrequenciesHistogram(detailedHistogram->cumulativeFrequenciesHistogram());
  histoOptionsWidget->setShowGraphEdges(detailedHistogram->displayGraphEdges());
  histoOptionsWidget->useCustomXAxisScale(detailedHistogram->getXAxisScaleDefined());
  histoOptionsWidget->setXAxisScale(detailedHistogram->getXAxisScale());
  histoOptionsWidget->useCustomYAxisScale(detailedHistogram->getYAxisScaleDefined());
  histoOptionsWidget->setYAxisScale(detailedHistogram->getYAxisScale());
  histoOptionsWidget->setInitXAxisScale(detailedHistogram->getInitXAxisScale());
  histoOptionsWidget->setInitYAxisScale(detailedHistogram->getInitYAxisScale());

  getGlMainWidget()->draw();
}

void HistogramView::switchFromDetailedViewToSmallMultiples() {

  if(needUpdateHistogram)
    updateHistograms();

  mainLayer->addGlEntity(emptyGlGraphComposite, "graph");

  mainLayer->deleteGlEntity(axisComposite);
  mainLayer->deleteGlEntity(emptyRect);
  mainLayer->deleteGlEntity(emptyRect2);
  delete emptyRect;
  delete emptyRect2;

  if (detailedHistogram != NULL) {
    mainLayer->deleteGlEntity(detailedHistogram->getBinsComposite());
  }

  detailedHistogram = NULL;
  detailedHistogramPropertyName = "";
  GlMainWidget *gl = getGlMainWidget();
  xAxisDetail = NULL;
  yAxisDetail = NULL;
  mainLayer->addGlEntity(histogramsComposite, "overviews composite");
  mainLayer->addGlEntity(labelsComposite, "labels composite");
  Camera &cam = gl->getScene()->getGraphCamera();
  cam.setSceneRadius(sceneRadiusBak);
  cam.setZoomFactor(zoomFactorBak);
  cam.setEyes(eyesBak);
  cam.setCenter(centerBak);
  cam.setUp(upBak);

  smallMultiplesView = true;

  toggleInteractors(false);
  propertiesSelectionWidget->setWidgetEnabled(true);
  histoOptionsWidget->setWidgetEnabled(false);
  histoOptionsWidget->resetAxisScale();

  gl->draw();
}

void HistogramView::updateDetailedHistogramAxis() {
  GlQuantitativeAxis *xAxis = detailedHistogram->getXAxis();
  GlQuantitativeAxis *yAxis = detailedHistogram->getYAxis();
  xAxis->addCaption(GlAxis::BELOW, 100, false, 300, 155, detailedHistogram->getPropertyName());
  yAxis->addCaption(GlAxis::LEFT, 100, false, 300, 155, (dataLocation == NODE ? "number of nodes" : "number of edges"));

  if(xAxis->getCaptionHeight()>yAxis->getCaptionHeight())
    xAxis->setCaptionHeight(yAxis->getCaptionHeight(),false);
  else
    yAxis->setCaptionHeight(xAxis->getCaptionHeight(),false);

  axisComposite->reset(false);
  axisComposite->addGlEntity(xAxis, "x axis");
  axisComposite->addGlEntity(yAxis, "y axis");

  if(xAxis->getSpaceBetweenAxisGrads()>yAxis->getSpaceBetweenAxisGrads())
    xAxis->setGradsLabelsHeight(yAxis->getSpaceBetweenAxisGrads()/2.);
  else
    yAxis->setGradsLabelsHeight(xAxis->getSpaceBetweenAxisGrads()/2.);

  xAxisDetail = xAxis;
  yAxisDetail = yAxis;
}

BoundingBox HistogramView::getSmallMultiplesBoundingBox() const {
  GlBoundingBoxSceneVisitor glBBSV(NULL);
  histogramsComposite->acceptVisitor(&glBBSV);
  labelsComposite->acceptVisitor(&glBBSV);
  return glBBSV.getBoundingBox();
}

void HistogramView::registerTriggers() {
  foreach (tlp::Observable* obs, triggers()) {
    removeRedrawTrigger(obs);
  }

  if(graph()) {
    addRedrawTrigger(graph());
    Iterator<string> *it = graph()->getProperties();

    while(it->hasNext()) {
      PropertyInterface *property=graph()->getProperty(it->next());
      addRedrawTrigger(property);
    }

    delete it;
  }
}

void HistogramView::interactorsInstalled(const QList<tlp::Interactor*>&) {
  toggleInteractors(false);
}

void HistogramView::toggleInteractors(const bool activate) {
  QList<Interactor *> interactorsList = interactors();

  for (QList<Interactor *>::iterator it = interactorsList.begin() ; it != interactorsList.end() ; ++it) {
    if (!(dynamic_cast<HistogramInteractorNavigation *>(*it))) {
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

void HistogramView::applySettings() {
  if(propertiesSelectionWidget->configurationChanged() || histoOptionsWidget->configurationChanged())
    viewConfigurationChanged();
}

void HistogramView::treatEvent(const Event &message) {
  if (typeid(message) == typeid(GraphEvent)) {
    const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&message);

    if(graphEvent) {
      if(graphEvent->getType()==GraphEvent::TLP_ADD_NODE)
        addNode(graphEvent->getGraph(),graphEvent->getNode());

      if(graphEvent->getType()==GraphEvent::TLP_ADD_EDGE)
        addEdge(graphEvent->getGraph(),graphEvent->getEdge());

      if(graphEvent->getType()==GraphEvent::TLP_DEL_NODE)
        delNode(graphEvent->getGraph(),graphEvent->getNode());

      if(graphEvent->getType()==GraphEvent::TLP_DEL_EDGE)
        delEdge(graphEvent->getGraph(),graphEvent->getEdge());
    }
  }

  if(typeid(message) == typeid(PropertyEvent)) {
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&message);

    if(propertyEvent) {
      if(propertyEvent->getType()==PropertyEvent::TLP_AFTER_SET_NODE_VALUE)
        afterSetNodeValue(propertyEvent->getProperty(),propertyEvent->getNode());

      if(propertyEvent->getType()==PropertyEvent::TLP_AFTER_SET_EDGE_VALUE)
        afterSetEdgeValue(propertyEvent->getProperty(),propertyEvent->getEdge());

      if(propertyEvent->getType()==PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE)
        afterSetAllNodeValue(propertyEvent->getProperty());

      if(propertyEvent->getType()==PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE)
        afterSetAllEdgeValue(propertyEvent->getProperty());

    }
  }
}

void HistogramView::afterSetNodeValue(PropertyInterface *p, const node n) {
  if (p->getGraph() == edgeAsNodeGraph && p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection =
      static_cast<BooleanProperty*>(p);
    BooleanProperty *viewSelection =
      _histoGraph->getProperty<BooleanProperty>("viewSelection");
    viewSelection->removeListener(this);
    viewSelection->setEdgeValue(nodeToEdge[n], edgeAsNodeGraphSelection->getNodeValue(n));
    viewSelection->addListener(this);
    setTextureUpdateNeeded();
    return;
  }

  afterSetAllNodeValue(p);
}

void HistogramView::afterSetEdgeValue(PropertyInterface *p, const edge e) {
  if (edgeToNode.find(e) == edgeToNode.end())
    return;

  if (p->getName() == "viewColor") {
    ColorProperty *edgeAsNodeGraphColors =
      edgeAsNodeGraph->getProperty<ColorProperty>("viewColor");
    ColorProperty *viewColor = static_cast<ColorProperty*>(p);
    edgeAsNodeGraphColors->setNodeValue(edgeToNode[e], viewColor->getEdgeValue(e));
    setTextureUpdateNeeded();
  }
  else if (p->getName() == "viewLabel") {
    StringProperty *edgeAsNodeGraphLabels =
      edgeAsNodeGraph->getProperty<StringProperty>("viewLabel");
    StringProperty *viewLabel = static_cast<StringProperty*>(p);
    edgeAsNodeGraphLabels->setNodeValue(edgeToNode[e], viewLabel->getEdgeValue(e));
  }
  else if (p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection = edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection");
    BooleanProperty *viewSelection = static_cast<BooleanProperty*>(p);
    edgeAsNodeGraphSelection->removeListener(this);

    if (edgeAsNodeGraphSelection->getNodeValue(edgeToNode[e]) != viewSelection->getEdgeValue(e))
      edgeAsNodeGraphSelection->setNodeValue(edgeToNode[e], viewSelection->getEdgeValue(e));

    edgeAsNodeGraphSelection->addListener(this);
    setTextureUpdateNeeded();
  }
}

void HistogramView::afterSetAllNodeValue(PropertyInterface *p) {
  if (detailedHistogram &&
      p->getName() == detailedHistogram->getPropertyName()) {
    setLayoutUpdateNeeded();
  }
  else if (p->getName() == "viewSize") {
    setSizesUpdateNeeded();
  }
  else if (p->getName() == "viewSelection") {
    if (p->getGraph() == edgeAsNodeGraph) {
      BooleanProperty *edgeAsNodeGraphSelection =
        static_cast<BooleanProperty*>(p);
      BooleanProperty *viewSelection =
        _histoGraph->getProperty<BooleanProperty>("viewSelection");
      viewSelection->setAllEdgeValue(edgeAsNodeGraphSelection->getNodeValue(edgeAsNodeGraph->getOneNode()));
    }

    setTextureUpdateNeeded();
  }
  else if (p->getName() == "viewColor"|| p->getName() == "viewShape" ||
           p->getName() == "viewTexture") {
    setTextureUpdateNeeded();
  }
}

void HistogramView::afterSetAllEdgeValue(PropertyInterface *p) {

  if (detailedHistogram &&
      p->getName() == detailedHistogram->getPropertyName()) {
    setLayoutUpdateNeeded();
  }

  if (p->getName() == "viewColor") {
    ColorProperty *edgeAsNodeGraphColors =
      edgeAsNodeGraph->getProperty<ColorProperty>("viewColor");
    ColorProperty *viewColor = static_cast<ColorProperty*>(p);
    edgeAsNodeGraphColors->setAllNodeValue(viewColor->getEdgeDefaultValue());
    setTextureUpdateNeeded();
  }
  else if (p->getName() == "viewLabel") {
    StringProperty *edgeAsNodeGraphLabels =
      edgeAsNodeGraph->getProperty<StringProperty>("viewLabel");
    StringProperty *viewLabel = static_cast<StringProperty*>(p);
    edgeAsNodeGraphLabels->setAllNodeValue(viewLabel->getEdgeDefaultValue());
  }
  else if (p->getName() == "viewSelection") {
    BooleanProperty *edgeAsNodeGraphSelection = edgeAsNodeGraph->getProperty<BooleanProperty>("viewSelection");
    BooleanProperty *viewSelection = static_cast<BooleanProperty*>(p);
    edge e;
    forEach(e, _histoGraph->getEdges()) {
      if (edgeAsNodeGraphSelection->getNodeValue(edgeToNode[e]) != viewSelection->getEdgeValue(e)) {
        edgeAsNodeGraphSelection->setNodeValue(edgeToNode[e], viewSelection->getEdgeValue(e));
      }
    }

    setTextureUpdateNeeded();
  }
}

void HistogramView::addNode(Graph *, const node) {
  setLayoutUpdateNeeded();
  setSizesUpdateNeeded();
}

void HistogramView::addEdge(Graph *, const edge e) {
  edgeToNode[e] = edgeAsNodeGraph->addNode();
  setLayoutUpdateNeeded();
  setSizesUpdateNeeded();
}

void HistogramView::delNode(Graph *, const node) {
  setLayoutUpdateNeeded();
  setSizesUpdateNeeded();
}

void HistogramView::delEdge(Graph *, const edge e) {
  edgeAsNodeGraph->delNode(edgeToNode[e]);
  edgeToNode.erase(e);
  setLayoutUpdateNeeded();
  setSizesUpdateNeeded();
}

unsigned int HistogramView::getMappedId(unsigned int id) {
  if (dataLocation == EDGE)
    return nodeToEdge[node(id)].id;

  return id;
}

}
