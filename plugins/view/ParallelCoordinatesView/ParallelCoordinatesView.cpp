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

#include "ParallelCoordinatesView.h"
#include "AxisConfigDialogs.h"
#include "NominalParallelAxis.h"
#include "ParallelCoordinatesViewQuickAccessbar.h"
#include "ParallelTools.h"
#include "ParallelCoordinatesDrawing.h"
#include "ParallelCoordsDrawConfigWidget.h"
#include "QuantitativeParallelAxis.h"

#include <QMenu>
#include <QMenuBar>
#include <QToolTip>
#include <QImageWriter>
#include <QThread>
#include <QDir>
#include <QGraphicsView>
#include <QKeyEvent>

#include <tulip/GlTools.h>
#include <tulip/GlLabel.h>
#include <tulip/GlProgressBar.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Interactor.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/PropertyInterface.h>

#include "../utils/ViewGraphPropertiesSelectionWidget.h"

using namespace std;

const string propertiesTypes[] = {"double", "int", "string"};
const unsigned int nbPropertiesTypes = sizeof(propertiesTypes) / sizeof(string);
const vector<string> propertiesTypesFilter(propertiesTypes, propertiesTypes + nbPropertiesTypes);

namespace tlp {

GLuint ParallelCoordinatesView::linesTextureId(0);
GLuint ParallelCoordinatesView::slidersTextureId(0);
unsigned int ParallelCoordinatesView::parallelViewInstancesCount(0);

static void toggleGraphView(GlGraphComposite *glGraph, bool displayNodes) {
  GlGraphRenderingParameters param = glGraph->getRenderingParameters ();
  param.setAntialiasing (true);
  param.setNodesStencil(2);
  param.setNodesLabelStencil(2);
  param.setSelectedNodesStencil(2);
  param.setDisplayEdges(false);
  param.setDisplayNodes(displayNodes);
  param.setFontsType(2);
  glGraph->setRenderingParameters (param);
}

PLUGIN(ParallelCoordinatesView)

ParallelCoordinatesView::ParallelCoordinatesView(const PluginContext *) : _bar(NULL),
  showToolTips(NULL), mainLayer(NULL),axisSelectionLayer(NULL), glGraphComposite(NULL), axisPointsGraph(NULL), graphProxy(NULL), parallelCoordsDrawing(NULL) , dataConfigWidget(NULL), drawConfigWidget(NULL),
  firstSet(true), lastNbSelectedProperties(0), center(false), lastViewWindowWidth(0), lastViewWindowHeight(0),isConstruct(false),dontCenterViewAfterConfLoaded(false),needDraw(false) {
  ++parallelViewInstancesCount;
}

ParallelCoordinatesView::~ParallelCoordinatesView() {
  foreach (tlp::Observable* obs, triggers()) {
    removeRedrawTrigger(obs);
  }

  --parallelViewInstancesCount;

  if (parallelViewInstancesCount == 0) {
    GlTextureManager::getInst().deleteTexture(DEFAULT_TEXTURE_FILE);
    GlTextureManager::getInst().deleteTexture(SLIDER_TEXTURE_NAME);
    linesTextureId = 0;
    slidersTextureId = 0;
  }

  delete axisPointsGraph;
  delete graphProxy;
  graphProxy = NULL;
  delete dataConfigWidget;
  delete drawConfigWidget;
}

QuickAccessBar* ParallelCoordinatesView::getQuickAccessBarImpl() {
  _bar = new ParallelCoordinatesViewQuickAccessBar(drawConfigWidget);
  connect(_bar, SIGNAL(settingsChanged()), this, SLOT(applySettings()));
  return _bar;
}

void ParallelCoordinatesView::initGlWidget() {
  GlScene* scene = getGlMainWidget()->getScene();

  if(!mainLayer) {
    mainLayer = new GlLayer("Main");
    scene->addExistingLayer(mainLayer);
  }

  axisPointsGraph = tlp::newGraph();
  glGraphComposite = new GlGraphComposite(axisPointsGraph);
  mainLayer->addGlEntity(glGraphComposite, "graph");
  axisSelectionLayer = new GlLayer("Axis selection layer");
  GlGraphRenderingParameters param = scene->getGlGraphComposite ()->getRenderingParameters ();
  param.setAntialiasing (true);
  param.setNodesStencil(2);
  param.setNodesLabelStencil(1);
  param.setSelectedNodesStencil(1);
  param.setDisplayEdges(false);
  param.setDisplayNodes(true);
  param.setViewNodeLabel(false);
  param.setFontsType(2);
  scene->getGlGraphComposite ()->setRenderingParameters (param);
  getGlMainWidget()->setMouseTracking(true);
}

QList<QWidget *> ParallelCoordinatesView::configurationWidgets() const {
  return QList<QWidget *>() << dataConfigWidget << drawConfigWidget ;
}

void ParallelCoordinatesView::setState(const DataSet &dataSet) {

  GlMainWidget *mainWidget=getGlMainWidget();

  if(!isConstruct) {
    initGlWidget();
    buildContextMenu();
    setOverviewVisible(true);
    getGlMainWidget()->installEventFilter(this);

    dataConfigWidget = new ViewGraphPropertiesSelectionWidget();
    drawConfigWidget = new ParallelCoordsDrawConfigWidget();

    if (linesTextureId == 0) {
      GlMainWidget::getFirstQGLWidget()->makeCurrent();
      linesTextureId = mainWidget->bindTexture(QPixmap(":/parallel_texture.png"), GL_TEXTURE_2D, GL_RGBA, QGLContext::LinearFilteringBindOption);
      slidersTextureId = mainWidget->bindTexture(QPixmap(":/parallel_sliders_texture.png"), GL_TEXTURE_2D, GL_RGBA, QGLContext::LinearFilteringBindOption);
      GlTextureManager::getInst().registerExternalTexture(DEFAULT_TEXTURE_FILE, linesTextureId);
      GlTextureManager::getInst().registerExternalTexture(SLIDER_TEXTURE_NAME, slidersTextureId);
    }

    isConstruct=true;
  }

  GlMainView::setState(dataSet);

  removeTriggers();

  vector<string> selectedPropertiesBak;

  bool sameGraphRoot = false;

  if(graph()) {
    if (graphProxy != NULL && (graph()->getRoot() == graphProxy->getRoot())) {
      sameGraphRoot = true;
      selectedPropertiesBak = graphProxy->getSelectedProperties();
    }
  }

  if (parallelCoordsDrawing != NULL && graphProxy->getGraph() != graph()) {
    mainLayer->deleteGlEntity(parallelCoordsDrawing);
    graphProxy->removeListener(parallelCoordsDrawing);
    delete parallelCoordsDrawing;
    parallelCoordsDrawing = NULL;
  }

  if (graphProxy != NULL && graphProxy->getGraph() != graph()) {
    delete graphProxy;
    graphProxy = NULL;
  }

  if(graph()!=NULL) {
    if (graphProxy == NULL) {
      graphProxy = new ParallelCoordinatesGraphProxy(graph());
    }

    if (sameGraphRoot) {
      graphProxy->setSelectedProperties(selectedPropertiesBak);
    }

    if (dataSet.exist("selectedProperties")) {
      vector<string> selectedProperties;
      DataSet items;
      dataSet.get("selectedProperties", items);
      int i = 0;
      stringstream ss;
      ss<<i;

      while (items.exist(ss.str())) {
        string item;
        items.get(ss.str(), item);
        selectedProperties.push_back(item);
        ss.str("");
        ++i;
        ss << i;
      }

      graphProxy->setSelectedProperties(selectedProperties);
    }

    dataConfigWidget->setWidgetParameters(graph(), propertiesTypesFilter);
    dataConfigWidget->setSelectedProperties(graphProxy->getSelectedProperties());

    if (parallelCoordsDrawing == NULL) {
      parallelCoordsDrawing = new ParallelCoordinatesDrawing(graphProxy, axisPointsGraph);
      graphProxy->addListener(parallelCoordsDrawing);
      mainLayer->addGlEntity(parallelCoordsDrawing, "Parallel Coordinates");
    }

    //overviewWidget->setObservedView(mainWidget, parallelCoordsDrawing);

    unsigned int axisHeight = DEFAULT_AXIS_HEIGHT;
    unsigned int linesColorAlphaValue = DEFAULT_LINES_COLOR_ALPHA_VALUE;


    if (dataSet.exist("dataLocation")) {
      int dataLocation = 0;
      dataSet.get("dataLocation", dataLocation);
      dataConfigWidget->setDataLocation(static_cast<ElementType>(dataLocation));
    }

    if (dataSet.exist("backgroundColor")) {
      Color backgroundColor;
      dataSet.get("backgroundColor", backgroundColor);
      drawConfigWidget->setBackgroundColor(backgroundColor);
    }

    if (dataSet.exist("axisPointMinSize")) {
      unsigned int axisPointMinSize = 0;
      dataSet.get("axisPointMinSize", axisPointMinSize);
      drawConfigWidget->setAxisPointMinSize(axisPointMinSize);
    }

    if (dataSet.exist("axisPointMaxSize")) {
      unsigned int axisPointMaxSize = 0;
      dataSet.get("axisPointMaxSize", axisPointMaxSize);
      drawConfigWidget->setAxisPointMaxSize(axisPointMaxSize);
    }

    if (dataSet.exist("drawPointsOnAxis")) {
      bool drawPointsOnAxis = true;
      dataSet.get("drawPointsOnAxis", drawPointsOnAxis);
      drawConfigWidget->setDrawPointOnAxis(drawPointsOnAxis);
    }

    if (dataSet.exist("linesTextureFileName")) {
      string linesTextureFileName;
      dataSet.get("linesTextureFileName", linesTextureFileName);
      drawConfigWidget->setLinesTextureFilename(linesTextureFileName);
    }

    if (dataSet.exist("axisHeight")) {
      dataSet.get("axisHeight", axisHeight);
    }

    if (dataSet.exist("linesColorAlphaValue")) {
      dataSet.get("linesColorAlphaValue", linesColorAlphaValue);
    }

    if (dataSet.exist("non highlighted alpha value")) {
      unsigned int nonHighlightedAlphaValue = 0;
      dataSet.get("non highlighted alpha value", nonHighlightedAlphaValue);
      drawConfigWidget->setUnhighlightedEltsColorsAlphaValue(nonHighlightedAlphaValue);
    }

    if (dataSet.exist("linesType")) {
      int linesType = 0;
      dataSet.get("linesType", linesType);

      if (linesType == ParallelCoordinatesDrawing::STRAIGHT) {
        straightLinesType->setChecked(true);
        catmullRomSplineLinesType->setChecked(false);
        cubicBSplineInterpolationLinesType->setChecked(false);
      }
      else if (linesType == ParallelCoordinatesDrawing::CATMULL_ROM_SPLINE) {
        straightLinesType->setChecked(false);
        catmullRomSplineLinesType->setChecked(true);
        cubicBSplineInterpolationLinesType->setChecked(false);
      }
      else {
        straightLinesType->setChecked(false);
        catmullRomSplineLinesType->setChecked(false);
        cubicBSplineInterpolationLinesType->setChecked(true);
      }
    }

    if (dataSet.exist("layoutType")) {
      int layoutType = 0;
      dataSet.get("layoutType", layoutType);

      if (layoutType == ParallelCoordinatesDrawing::PARALLEL) {
        classicLayout->setChecked(true);
        circularLayout->setChecked(false);
      }
      else {
        classicLayout->setChecked(false);
        circularLayout->setChecked(true);
      }
    }

    drawConfigWidget->setAxisHeight(axisHeight);
    drawConfigWidget->setLinesColorAlphaValue(linesColorAlphaValue);

    dataSet.get("lastViewWindowWidth", lastViewWindowWidth);
    dataSet.get("lastViewWindowHeight", lastViewWindowHeight);

    if (dataSet.exist("scene")) {
      string sceneXML;
      dataSet.get("scene",sceneXML);
      getGlMainWidget()->getScene()->setWithXML(sceneXML,NULL);
      dontCenterViewAfterConfLoaded=true;
    }

  }
  else {
    dataConfigWidget->setWidgetParameters(NULL, propertiesTypesFilter);
  }

  bool quickAccessBarVisible=false;

  if (dataSet.get<bool>("quickAccessBarVisible", quickAccessBarVisible)) {
    needQuickAccessBar = true;
    setQuickAccessBarVisible(quickAccessBarVisible);
  }
  else
    setQuickAccessBarVisible(true);

  setupAndDrawView();
}

DataSet ParallelCoordinatesView::state() const {

  DataSet dataSet = GlMainView::state();

  string sceneOut;
  getGlMainWidget()->getScene()->getXMLOnlyForCameras(sceneOut);
  dataSet.set("scene",sceneOut);

  vector<string> selectedProperties=graphProxy->getSelectedProperties();
  DataSet selectedPropertiesData;
  int i=0;

  for (vector<string>::const_iterator it=selectedProperties.begin(); it != selectedProperties.end(); ++it) {
    std::stringstream s;
    s << i;
    selectedPropertiesData.set(s.str(), *it);
    i++;
  }

  dataSet.set("selectedProperties", selectedPropertiesData);
  dataSet.set("dataLocation", int(graphProxy->getDataLocation()));
  dataSet.set("backgroundColor", drawConfigWidget->getBackgroundColor());
  dataSet.set("axisHeight", drawConfigWidget->getAxisHeight());
  unsigned int axisPointMinSize = uint(drawConfigWidget->getAxisPointMinSize().getW());
  unsigned int axisPointMaxSize = uint(drawConfigWidget->getAxisPointMaxSize().getW());
  dataSet.set("axisPointMinSize", axisPointMinSize);
  dataSet.set("axisPointMaxSize", axisPointMaxSize);
  dataSet.set("drawPointsOnAxis", drawConfigWidget->drawPointOnAxis());
  dataSet.set("linesTextureFileName", drawConfigWidget->getLinesTextureFilename());
  dataSet.set("linesColorAlphaValue", drawConfigWidget->getLinesColorAlphaValue());
  dataSet.set("non highlighted alpha value", drawConfigWidget->getUnhighlightedEltsColorsAlphaValue());
  dataSet.set("layoutType", int(getLayoutType()));
  dataSet.set("linesType", int(getLinesType()));
  dataSet.set("lastViewWindowWidth",  getGlMainWidget()->width());
  dataSet.set("lastViewWindowHeight",  getGlMainWidget()->height());

  if (needQuickAccessBar)
    dataSet.set("quickAccessBarVisible", quickAccessBarVisible());


  return dataSet;
}

void ParallelCoordinatesView::graphChanged(tlp::Graph*) {
  if(isConstruct)
    setState(DataSet());
  else if(_bar!=NULL) {
    _bar->setEnabled(false);
  }
}

void ParallelCoordinatesView::updateWithoutProgressBar() {
  if (parallelCoordsDrawing) {
    parallelCoordsDrawing->update(getGlMainWidget(), true);
  }
}

void ParallelCoordinatesView::updateWithProgressBar() {
  if (parallelCoordsDrawing) {
    setOverviewVisible(false);
    toggleGraphView(glGraphComposite, false);
    parallelCoordsDrawing->update(getGlMainWidget());
    toggleGraphView(glGraphComposite, true);
    centerView();
    getGlMainWidget()->draw();
    setOverviewVisible(true);
  }
}

void ParallelCoordinatesView::addEmptyViewLabel() {
  Color backgroundColor(drawConfigWidget->getBackgroundColor());
  getGlMainWidget()->getScene()->setBackgroundColor(backgroundColor);

  Color foregroundColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    foregroundColor = Color(255,255,255);
  }
  else {
    foregroundColor = Color(0,0,0);
  }


  GlLabel *noDimsLabel = new GlLabel(Coord(0,0,0), Size(200,200), foregroundColor);
  noDimsLabel->setText(ViewName::ParallelCoordinatesViewName);
  mainLayer->addGlEntity(noDimsLabel, "no dimensions label");
  GlLabel *noDimsLabel1 = new GlLabel(Coord(0.0f,-50.0f,0.0f), Size(400.0f,200.0f), foregroundColor);
  noDimsLabel1->setText("No graph properties selected.");
  mainLayer->addGlEntity(noDimsLabel1, "no dimensions label 1");
  GlLabel *noDimsLabel2 = new GlLabel(Coord(0.0f,-100.0f,0.0f), Size(700.0f,200.0f), foregroundColor);
  noDimsLabel2->setText("Go to the \"Properties\" tab in top right corner.");
  mainLayer->addGlEntity(noDimsLabel2, "no dimensions label 2");
  mainLayer->deleteGlEntity(parallelCoordsDrawing);
  mainLayer->deleteGlEntity(glGraphComposite);
}

void ParallelCoordinatesView::removeEmptyViewLabel() {
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

    if (parallelCoordsDrawing)
      mainLayer->addGlEntity(parallelCoordsDrawing, "Parallel Coordinates");

    mainLayer->addGlEntity(glGraphComposite, "graph");
  }
}

void ParallelCoordinatesView::draw() {
  if(graph()) {
    if (graphProxy->selectedPropertiesisEmpty()) {
      removeEmptyViewLabel();
      addEmptyViewLabel();
      _bar->setEnabled(false);
      getGlMainWidget()->getScene()->centerScene();
      getGlMainWidget()->draw();
      return;
    }
    else {
      removeEmptyViewLabel();
      _bar->setEnabled(true);

      if (graphProxy->getDataCount() > PROGRESS_BAR_DISPLAY_NB_DATA_THRESHOLD) {
        updateWithProgressBar();
      }
      else {
        updateWithoutProgressBar();
      }
    }

    if (lastNbSelectedProperties != graphProxy->getNumberOfSelectedProperties() || center) {
      if(!dontCenterViewAfterConfLoaded) {
        centerView();
      }
      else {
        dontCenterViewAfterConfLoaded=false;
      }

      center = false;
    }
    else {
      getGlMainWidget()->draw();
    }

    lastNbSelectedProperties = graphProxy->getNumberOfSelectedProperties();
  }
  else {
    getGlMainWidget()->draw();
  }

  needDraw=false;
}

void ParallelCoordinatesView::refresh() {
  if(!needDraw)
    getGlMainWidget()->redraw();
  else
    draw();
}

void ParallelCoordinatesView::init() {
  emit drawNeeded();
}

bool ParallelCoordinatesView::eventFilter(QObject *obj,QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);

    if((keyEvent->key()==Qt::Key_R) && (keyEvent->modifiers() & Qt::ControlModifier)!=0 && (keyEvent->modifiers() & Qt::ShiftModifier)!=0)
      emit drawNeeded();

    if((keyEvent->key()==Qt::Key_C) && (keyEvent->modifiers() & Qt::ControlModifier)!=0 && (keyEvent->modifiers() & Qt::ShiftModifier)!=0)
      centerView();
  }

  if (event->type() == QEvent::ToolTip && showToolTips->isChecked()) {
    QHelpEvent *he = dynamic_cast<QHelpEvent *>(event);

    if (parallelCoordsDrawing != NULL) {
      const set<unsigned int> &dataUnderPointer(mapGlEntitiesInRegionToData(he->x(), he->y(), 1, 1));

      if (!dataUnderPointer.empty()) {
        QString ttip(graphProxy->getToolTipTextforData(*(dataUnderPointer.begin())).c_str());
        QToolTip::showText(he->globalPos(), ttip);
      }
    }
  }

  if (graphProxy != NULL && graphProxy->graphColorsModified()) {
    Observable::holdObservers();
    graphProxy->colorDataAccordingToHighlightedElts();
    Observable::unholdObservers();
  }

  return GlMainView::eventFilter(obj, event);
}

void ParallelCoordinatesView::buildContextMenu() {

  viewSetupMenu = new QMenu(tr("View setup"));
  viewSetupMenu->addAction(tr("Layout type"))->setEnabled(false);
  QActionGroup *layoutActionGroup = new QActionGroup(this);
  classicLayout = viewSetupMenu->addAction(tr("Classic layout"),this, SLOT(centerSetupAndDrawView()));
  classicLayout->setToolTip(QString("Use parallel axis layout"));
  classicLayout->setCheckable(true);
  classicLayout->setChecked(true);
  layoutActionGroup->addAction(classicLayout);
  circularLayout = viewSetupMenu->addAction(tr("Circular layout"),this, SLOT(centerSetupAndDrawView()));
  circularLayout->setToolTip(QString("In the circular layout, the axis are laid regularly as the radius of a circle"));
  circularLayout->setCheckable(true);
  layoutActionGroup->addAction(circularLayout);
  viewSetupMenu->addSeparator();

  viewSetupMenu->addAction(tr("Lines type"))->setEnabled(false);
  QActionGroup *lineTypeActionGroup = new QActionGroup(this);
  straightLinesType = viewSetupMenu->addAction(tr("Polyline"),this, SLOT(setupAndDrawView()));
  straightLinesType->setToolTip(QString("Draw a polyline joining the consecutive coordinates belonging to the same graph element"));
  straightLinesType->setCheckable(true);
  straightLinesType->setChecked(true);
  lineTypeActionGroup->addAction(straightLinesType);
  catmullRomSplineLinesType = viewSetupMenu->addAction(tr("Catmull-Rom spline"),this, SLOT(setupAndDrawView()));
  catmullRomSplineLinesType->setToolTip(QString("Draw a Catmull-Rom spline joining the consecutive coordinates belonging to the same graph element"));
  catmullRomSplineLinesType->setCheckable(true);
  lineTypeActionGroup->addAction(catmullRomSplineLinesType);
  cubicBSplineInterpolationLinesType = viewSetupMenu->addAction(tr("Cubic B-spline interpolation"),this, SLOT(setupAndDrawView()));
  cubicBSplineInterpolationLinesType->setToolTip(QString("Draw a cubic B-spline joining the consecutive coordinates belonging to the same graph element"));
  cubicBSplineInterpolationLinesType->setCheckable(true);
  lineTypeActionGroup->addAction(catmullRomSplineLinesType);
  viewSetupMenu->addSeparator();

  viewSetupMenu->addAction(tr("Lines thickness"))->setEnabled(false);
  QActionGroup *lineActionGroup = new QActionGroup(this);
  thickLines = viewSetupMenu->addAction(tr("Map to viewSize"),this, SLOT(setupAndDrawView()));
  thickLines->setToolTip(QString("The lines thickness is computed according the viewSize property values"));
  thickLines->setCheckable(true);
  thickLines->setChecked(true);
  lineActionGroup->addAction(thickLines);
  thinLines = viewSetupMenu->addAction(tr("Thin lines"),this, SLOT(setupAndDrawView()));
  thinLines->setToolTip(QString("The thickness is thin and the same for all the  curves representing the graph elements"));
  thinLines->setCheckable(true);
  lineActionGroup->addAction(thinLines);

  showToolTips = new QAction(QString("Tooltips"), NULL);
  showToolTips->setToolTip(QString("Enable to display a tooltip indicating the graph element located under the mouse pointer"));
  showToolTips->setCheckable(true);
  showToolTips->setChecked(false);


  axisMenuSeparator=new QAction(NULL);
  axisMenuSeparator->setSeparator(true);
  axisConfiguration=new QAction(tr("Axis configuration"),NULL);
  connect(axisConfiguration,SIGNAL(triggered()),this,SLOT(axisConfigurationSlot()));
  removeAxisAction=new QAction(tr("Remove axis"),NULL);
  connect(removeAxisAction,SIGNAL(triggered()),this,SLOT(removeAxisSlot()));
  highlightMenuSeparator=new QAction(NULL);
  highlightMenuSeparator->setSeparator(true);
  selectHighlightedElements=new QAction(tr("Select highlighted elements"),NULL);
  selectHighlightedElements->setToolTip(QString("Select the graph elements corresponding to the currently highlighted curves"));
  connect(selectHighlightedElements,SIGNAL(triggered()),this,SLOT(selectHighlightedElementsSlot()));
  addSelectHighlightedElements=new QAction(tr("Add highlighted elements to selection"),NULL);
  addSelectHighlightedElements->setToolTip(QString("Add the graph elements corresponding to the currently highlighted curves to the current selection"));
  connect(addSelectHighlightedElements,SIGNAL(triggered()),this,SLOT(addSelectHighlightedElementsSlot()));
  removeSelectHighlightedElements=new QAction(tr("Remove highlighted elements to selection"),NULL);
  removeSelectHighlightedElements->setToolTip(QString("Remove the graph elements corresponding to the currently highlighted curves from the current selection"));
  connect(removeSelectHighlightedElements,SIGNAL(triggered()),this,SLOT(removeSelectHighlightedElementsSlot()));
  resetHightlightedElements=new QAction(tr("Reset highlighting of elements"),NULL);
  resetHightlightedElements->setToolTip(QString("Unhighlight all the elements"));
  connect(resetHightlightedElements,SIGNAL(triggered()),this,SLOT(resetHightlightedElementsSlot()));
}

void ParallelCoordinatesView::fillContextMenu(QMenu *menu, const QPointF &point) {
  GlMainView::fillContextMenu(menu,point);
  menu->addAction(viewSetupMenu->menuAction());
  viewSetupMenu->setStyleSheet(menu->styleSheet());
  menu->addAction(showToolTips);

  axisUnderPointer = getAxisUnderPointer(point.x(), point.y());

  if (axisUnderPointer != NULL) {
    menu->addAction(axisMenuSeparator);
    menu->addAction(axisConfiguration);
    axisConfiguration->setToolTip(QString("Configure the axis '") + axisUnderPointer->getAxisName().c_str() + "'");
    menu->addAction(removeAxisAction);
    removeAxisAction->setToolTip(QString("Remove the axis '") + axisUnderPointer->getAxisName().c_str() + "': the property is then deselected in the Properties configuration panel");
  }

  if (graphProxy->highlightedEltsSet()) {
    menu->addAction(highlightMenuSeparator);
    menu->addAction(selectHighlightedElements);
    menu->addAction(addSelectHighlightedElements);
    menu->addAction(removeSelectHighlightedElements);
    menu->addAction(resetHightlightedElements);
  }
}

void ParallelCoordinatesView::axisConfigurationSlot() {
  axisUnderPointer->showConfigDialog();
  emit drawNeeded();
}

void ParallelCoordinatesView::removeAxisSlot() {
  graphProxy->removePropertyFromSelection(axisUnderPointer->getAxisName());
  dataConfigWidget->setSelectedProperties(graphProxy->getSelectedProperties());
  emit drawNeeded();
}

void ParallelCoordinatesView::selectHighlightedElementsSlot() {
  Observable::holdObservers();
  graphProxy->selectHighlightedElements();
  Observable::unholdObservers();
}

void ParallelCoordinatesView::addSelectHighlightedElementsSlot() {
  Observable::holdObservers();
  graphProxy->setSelectHighlightedElements(true);
  Observable::unholdObservers();
}

void ParallelCoordinatesView::removeSelectHighlightedElementsSlot() {
  Observable::holdObservers();
  graphProxy->setSelectHighlightedElements(false);
  Observable::unholdObservers();
}

void ParallelCoordinatesView::resetHightlightedElementsSlot() {
  Observable::holdObservers();
  graphProxy->unsetHighlightedElts();
  parallelCoordsDrawing->resetAxisSlidersPosition();
  graphProxy->colorDataAccordingToHighlightedElts();
  Observable::unholdObservers();
}

void ParallelCoordinatesView::centerSetupAndDrawView() {
  center=true;
  setupAndDrawView();
}


void ParallelCoordinatesView::setupAndDrawView() {
  if (!graphProxy)
    return;

  if(graph()) {
    GlScene* scene = getGlMainWidget()->getScene();
    graphProxy->setSelectedProperties(dataConfigWidget->getSelectedGraphProperties());
    graphProxy->setDataLocation(dataConfigWidget->getDataLocation());
    scene->setBackgroundColor(drawConfigWidget->getBackgroundColor());
    parallelCoordsDrawing->setAxisHeight(drawConfigWidget->getAxisHeight());
    parallelCoordsDrawing->setAxisPointMinSize(drawConfigWidget->getAxisPointMinSize());
    parallelCoordsDrawing->setAxisPointMaxSize(drawConfigWidget->getAxisPointMaxSize());
    parallelCoordsDrawing->setBackgroundColor(drawConfigWidget->getBackgroundColor());
    parallelCoordsDrawing->setDrawPointsOnAxis(drawConfigWidget->drawPointOnAxis());
    parallelCoordsDrawing->setLineTextureFilename(drawConfigWidget->getLinesTextureFilename());
    parallelCoordsDrawing->setLinesColorAlphaValue(drawConfigWidget->getLinesColorAlphaValue());
    parallelCoordsDrawing->setLayoutType(getLayoutType());
    parallelCoordsDrawing->setLinesType(getLinesType());
    parallelCoordsDrawing->setLinesThickness(getLinesThickness());
    scene->getGlGraphComposite()->getRenderingParametersPointer()->setViewNodeLabel(drawConfigWidget->displayNodeLabels());

    if (graphProxy->getUnhighlightedEltsColorAlphaValue() != drawConfigWidget->getUnhighlightedEltsColorsAlphaValue()) {
      graphProxy->setUnhighlightedEltsColorAlphaValue(drawConfigWidget->getUnhighlightedEltsColorsAlphaValue());
      Observable::holdObservers();
      graphProxy->colorDataAccordingToHighlightedElts();
      Observable::unholdObservers();
    }
  }

  registerTriggers();
  needDraw=true;
  draw();
  drawOverview(true);
}

ParallelCoordinatesDrawing::LayoutType ParallelCoordinatesView::getLayoutType() const {
  return (classicLayout->isChecked())?ParallelCoordinatesDrawing::PARALLEL:ParallelCoordinatesDrawing::CIRCULAR;
}

ParallelCoordinatesDrawing::LinesType ParallelCoordinatesView::getLinesType() const {
  if (straightLinesType->isChecked()) {
    return ParallelCoordinatesDrawing::STRAIGHT;
  }
  else if (catmullRomSplineLinesType->isChecked()) {
    return ParallelCoordinatesDrawing::CATMULL_ROM_SPLINE;
  }
  else {
    return ParallelCoordinatesDrawing::CUBIC_BSPLINE_INTERPOLATION;
  }
}

ParallelCoordinatesDrawing::LinesThickness ParallelCoordinatesView::getLinesThickness() const {
  return (thickLines->isChecked())?ParallelCoordinatesDrawing::THICK:ParallelCoordinatesDrawing::THIN;
}

const set<unsigned int> &ParallelCoordinatesView::mapGlEntitiesInRegionToData(const int x, const int y, const unsigned int width, const unsigned int height) {

  vector<SelectedEntity> selectedEntities;
  vector<SelectedEntity> selectedAxisPoints;
  vector<SelectedEntity> dummy;

  mappedData.clear();

  bool result = getGlMainWidget()->pickGlEntities(x, y, width, height, selectedEntities, mainLayer);

  if (result) {
    for (vector<SelectedEntity>::const_iterator it = selectedEntities.begin(); it != selectedEntities.end(); ++it) {
      GlEntity *entity = (*it).getSimpleEntity();
      unsigned int selectedEltId;

      if (parallelCoordsDrawing->getDataIdFromGlEntity(entity, selectedEltId)) {
        mappedData.insert(selectedEltId);
      }
    }
  }

  getGlMainWidget()->pickNodesEdges(x, y, width , height, selectedAxisPoints, dummy, mainLayer);

  for (vector<SelectedEntity>::const_iterator it = selectedAxisPoints.begin(); it != selectedAxisPoints.end(); ++it) {
    node n((*it).getComplexEntityId());
    unsigned int selectedEltId;

    if (parallelCoordsDrawing->getDataIdFromAxisPoint(n, selectedEltId)) {
      mappedData.insert(selectedEltId);
    }
  }

  return mappedData;
}

void ParallelCoordinatesView::setDataUnderPointerSelectFlag(const int x, const int y, const bool selectFlag) {
  const set<unsigned int> &dataUnderPointer(mapGlEntitiesInRegionToData(x, y, 1, 1));

  for (set<unsigned int>::const_iterator it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
    if (!graphProxy->highlightedEltsSet() || graphProxy->isDataHighlighted(*it))
      graphProxy->setDataSelected(*it, selectFlag);
  }
}

void ParallelCoordinatesView::setDataInRegionSelectFlag(const int x, const int y, const unsigned int width, const unsigned int height, const bool selectFlag) {
  const set<unsigned int> &dataUnderPointer(mapGlEntitiesInRegionToData(x, y, width, height));

  for (set<unsigned int>::const_iterator it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
    if (!graphProxy->highlightedEltsSet() || graphProxy->isDataHighlighted(*it))
      graphProxy->setDataSelected(*it, selectFlag);
  }
}

void ParallelCoordinatesView::resetSelection() {
  graphProxy->resetSelection();
}

void ParallelCoordinatesView::deleteDataUnderPointer(const int x, const int y) {
  const set<unsigned int> &dataUnderPointer(mapGlEntitiesInRegionToData(x, y, 1, 1));

  for (set<unsigned int>::const_iterator it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
    if (!graphProxy->highlightedEltsSet() || graphProxy->isDataHighlighted(*it))
      graphProxy->deleteData(*it);
  }
}

bool ParallelCoordinatesView::getDataUnderPointerProperties(const int x, const int y,SelectedEntity &selectedEntity) {
  const set<unsigned int> &dataUnderPointer(mapGlEntitiesInRegionToData(x, y, 1, 1));

  if (!dataUnderPointer.empty()) {
    unsigned int dataId;

    if (!graphProxy->highlightedEltsSet()) {
      dataId = *(dataUnderPointer.begin());
    }
    else {
      set<unsigned int>::const_iterator it = dataUnderPointer.begin();

      while (it != dataUnderPointer.end() && !graphProxy->isDataHighlighted(*it)) {
        ++it;
      }

      if (it == dataUnderPointer.end()) {
        return false;
      }
      else {
        dataId = *it;
      }
    }

    if (graphProxy->getDataLocation() == NODE) {
      selectedEntity=SelectedEntity(graph(),dataId,SelectedEntity::NODE_SELECTED);
    }
    else {
      selectedEntity=SelectedEntity(graph(),dataId,SelectedEntity::EDGE_SELECTED);
    }

    return true;
  }
  else {
    return false;
  }
}

void ParallelCoordinatesView::highlightDataUnderPointer(const int x, const int y, const bool addEltToMagnifyFlag) {
  if (!addEltToMagnifyFlag) {
    graphProxy->unsetHighlightedElts();
  }

  const set<unsigned int> &dataUnderPointer(mapGlEntitiesInRegionToData(x, y, 1, 1));

  for (set<unsigned int>::const_iterator it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
    graphProxy->addOrRemoveEltToHighlight(*it);
  }

  graphProxy->colorDataAccordingToHighlightedElts();
}

void ParallelCoordinatesView::highlightDataInRegion(const int x, const int y, const int width, const int height, const bool addEltToMagnifyFlag) {
  if (!addEltToMagnifyFlag) {
    graphProxy->unsetHighlightedElts();
  }

  const set<unsigned int> &dataUnderPointer = mapGlEntitiesInRegionToData(x, y, width, height);

  for (set<unsigned int>::const_iterator it = dataUnderPointer.begin() ; it != dataUnderPointer.end() ; ++it) {
    graphProxy->addOrRemoveEltToHighlight(*it);
  }

  graphProxy->colorDataAccordingToHighlightedElts();
}

void ParallelCoordinatesView::resetHighlightedElements() {
  graphProxy->unsetHighlightedElts();
  graphProxy->colorDataAccordingToHighlightedElts();
}

ParallelAxis *ParallelCoordinatesView::getAxisUnderPointer(const int x, const int y) const {
  vector<ParallelAxis *> allAxis(parallelCoordsDrawing->getAllAxis());
  axisSelectionLayer->setSharedCamera(&getGlMainWidget()->getScene()->getLayer("Main")->getCamera());
  axisSelectionLayer->getComposite()->reset(false);

  for (size_t i = 0 ; i < allAxis.size() ; ++i) {
    axisSelectionLayer->addGlEntity(allAxis[i], getStringFromNumber(allAxis[i]));
  }

  vector<SelectedEntity> pickedEntities;

  if (getGlMainWidget()->pickGlEntities(x, y, pickedEntities, axisSelectionLayer)) {
    return dynamic_cast<ParallelAxis *>(pickedEntities[0].getSimpleEntity());
  }

  axisSelectionLayer->getComposite()->reset(false);
  return NULL;
}

void ParallelCoordinatesView::swapAxis(ParallelAxis *firstAxis, ParallelAxis *secondAxis) {
  parallelCoordsDrawing->swapAxis(firstAxis, secondAxis);
  dataConfigWidget->setSelectedProperties(graphProxy->getSelectedProperties());
}

void ParallelCoordinatesView::removeAxis(ParallelAxis *axis) {
  parallelCoordsDrawing->removeAxis(axis);
}

void ParallelCoordinatesView::addAxis(ParallelAxis *axis) {
  parallelCoordsDrawing->addAxis(axis);
}

vector<ParallelAxis *> ParallelCoordinatesView::getAllAxis() {
  return parallelCoordsDrawing->getAllAxis();
}

void ParallelCoordinatesView::updateAxisSlidersPosition() {
  if (!graphProxy->highlightedEltsSet()) {
    parallelCoordsDrawing->resetAxisSlidersPosition();
  }
  else {
    const set<unsigned int> &highlightedElts(graphProxy->getHighlightedElts());
    vector<ParallelAxis *> axis(getAllAxis());
    vector<ParallelAxis *>::iterator it;

    for (it = axis.begin() ; it != axis.end() ; ++it) {
      (*it)->updateSlidersWithDataSubset(highlightedElts);
    }
  }
}

void ParallelCoordinatesView::updateWithAxisSlidersRange(ParallelAxis *axis, ParallelCoordinatesDrawing::HighlightedEltsSetOp highlightedEltsSetOp) {
  parallelCoordsDrawing->updateWithAxisSlidersRange(axis, highlightedEltsSetOp);
  graphProxy->colorDataAccordingToHighlightedElts();
}

bool ParallelCoordinatesView::highlightedElementsSet() const {
  return graphProxy->highlightedEltsSet();
}

void ParallelCoordinatesView::highlightDataInAxisBoxPlotRange(QuantitativeParallelAxis *axis) {
  const set<unsigned int> &eltToHighlight(axis->getDataBetweenBoxPlotBounds());

  if (!eltToHighlight.empty()) {
    graphProxy->resetHighlightedElts(eltToHighlight);
    graphProxy->colorDataAccordingToHighlightedElts();
    updateAxisSlidersPosition();
  }
}

void ParallelCoordinatesView::removeTriggers() {
  foreach (tlp::Observable* obs, triggers()) {
    removeRedrawTrigger(obs);
  }
}

void ParallelCoordinatesView::registerTriggers() {
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

void ParallelCoordinatesView::applySettings() {
  if(dataConfigWidget->configurationChanged() || drawConfigWidget->configurationChanged())
    setupAndDrawView();
}

}
