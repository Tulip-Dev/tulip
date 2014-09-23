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

#include <typeinfo>

#include <QGraphicsView>
#include <QAction>
#include <QMenu>

#include "MatrixView.h"
#include "PropertyValuesDispatcher.h"
#include "GlMatrixBackgroundGrid.h"

#include <tulip/Graph.h>
#include <tulip/IntegerProperty.h>
#include <tulip/GlMainWidget.h>
#include <tulip/ForEach.h>
#include <tulip/TlpQtTools.h>
#include <tulip/GlLabel.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlyphManager.h>
#include <tulip/ParametricCurves.h>
#include <tulip/TulipViewSettings.h>

using namespace tlp;
using namespace std;

MatrixView::MatrixView(const PluginContext *):
  NodeLinkDiagramComponent(),
  _matrixGraph(NULL), _graphEntitiesToDisplayedNodes(NULL), _displayedNodesToGraphEntities(NULL), _displayedEdgesToGraphEdges(NULL), _displayedNodesAreNodes(NULL), _dispatcher(NULL),
  _configurationWidget(NULL), _mustUpdateSizes(false), _mustUpdateLayout(false), _orderingMetricName("") {
}

MatrixView::~MatrixView() {
  deleteDisplayedGraph();
}

void MatrixView::setState(const DataSet &ds) {

  clearRedrawTriggers();

  setOverviewVisible(true);

  if (!_configurationWidget) {
    _configurationWidget = new MatrixViewConfigurationWidget(getGlMainWidget()->parentWidget());
    connect(_configurationWidget, SIGNAL(changeBackgroundColor(QColor)), this, SLOT(setBackgroundColor(QColor)));
    connect(_configurationWidget, SIGNAL(metricSelected(std::string)), this, SLOT(setOrderingMetric(std::string)));
    connect(_configurationWidget, SIGNAL(setGridDisplayMode()), this, SLOT(setGridDisplayMode()));
    connect(_configurationWidget, SIGNAL(showEdges(bool)), this, SLOT(showEdges(bool)));

    QAction* centerAction = new QAction(trUtf8("Center"),this);
    centerAction->setShortcut(trUtf8("Ctrl+Shift+C"));
    connect(centerAction,SIGNAL(triggered()),getGlMainWidget(),SLOT(centerScene()));
    graphicsView()->addAction(centerAction);
  }

  _configurationWidget->setGraph(graph());

  initDisplayedGraph();
  registerTriggers();

  bool status=true;
  ds.get("show Edges", status);
  showEdges(status);
  _configurationWidget->setDisplayEdges(status);

  ds.get("ascending order", status);
  _configurationWidget->setAscendingOrder(status);

  Color c=getGlMainWidget()->getScene()->getBackgroundColor();
  ds.get("Background Color", c);
  _configurationWidget->setBackgroundColor(tlp::colorToQColor(c));

  unsigned grid=0;
  ds.get("Grid mode", grid);
  _configurationWidget->setgridmode(grid);

  int orderingindex=0;
  ds.get("ordering", orderingindex);
  _configurationWidget->setOrderingProperty(orderingindex);

}

void MatrixView::showEdges(bool show) {
  getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->setDisplayEdges(show);
  emit drawNeeded();
}

void MatrixView::graphChanged(Graph *) {
  setState(DataSet());
}

DataSet MatrixView::state() const {
  DataSet ds;
  ds.set("show Edges", getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->isDisplayEdges());
  ds.set("ascending order", _configurationWidget->ascendingOrder());
  ds.set("Grid mode", _configurationWidget->gridDisplayMode());
  ds.set("Background Color", getGlMainWidget()->getScene()->getBackgroundColor());
  ds.set("ordering", _configurationWidget->orderingProperty());
  return ds;
}

QList<QWidget *> MatrixView::configurationWidgets() const {
  return QList<QWidget *>() << _configurationWidget ;
}

void MatrixView::fillContextMenu(QMenu *menu, const QPointF &point) {
  GlMainView::fillContextMenu(menu,point);
  //Check if a node/edge is under the mouse pointer
  SelectedEntity entity;

  if (getGlMainWidget()->pickNodesEdges(point.x(), point.y(), entity)) {
    menu->addSeparator();
    isNode = entity.getEntityType() == SelectedEntity::NODE_SELECTED;
    itemId = entity.getComplexEntityId();

    if (isNode) {
      if (!_displayedNodesAreNodes->getNodeValue(node(itemId)))
        isNode = false;

      itemId = _displayedNodesToGraphEntities->getNodeValue(node(itemId));
    }
    else
      itemId = _displayedEdgesToGraphEdges->getEdgeValue(edge(itemId));

    menu->addAction((isNode ? trUtf8("Node #") : trUtf8("Edge #"))
                    + QString::number(itemId))->setEnabled(false);

    menu->addSeparator();

    menu->addAction(tr("Toggle selection"),this,SLOT(addRemoveItemToSelection()));
    menu->addAction(tr("Select"),this,SLOT(selectItem()));
    menu->addAction(tr("Delete"),this,SLOT(deleteItem()));
  }
}

void MatrixView::draw() {
  if (_mustUpdateSizes) {
    normalizeSizes();
    _mustUpdateSizes = false;
  }

  if (_mustUpdateLayout) {
    updateLayout();
    _mustUpdateLayout = false;
  }

  getGlMainWidget()->draw();
}

void MatrixView::refresh() {
  getGlMainWidget()->redraw();
}

void MatrixView::deleteDisplayedGraph() {
  foreach (tlp::Observable* obs, triggers()) {
    removeRedrawTrigger(obs);
  }
  delete _matrixGraph;
  _matrixGraph=NULL;
  delete _graphEntitiesToDisplayedNodes;
  _graphEntitiesToDisplayedNodes=NULL;
  delete _displayedNodesToGraphEntities;
  _displayedNodesToGraphEntities=NULL;
  delete _displayedEdgesToGraphEdges;
  _displayedEdgesToGraphEdges=NULL;
  delete _displayedNodesAreNodes;
  _displayedNodesAreNodes=NULL;
  delete _dispatcher;
  _dispatcher=NULL;
}

void MatrixView::initDisplayedGraph() {
  _mustUpdateLayout = true;
  _mustUpdateSizes = true;

  deleteDisplayedGraph();

  _matrixGraph = newGraph();

  if(!graph()) {
    return;
  }

  _graphEntitiesToDisplayedNodes = new IntegerVectorProperty(graph());
  _displayedNodesAreNodes = new BooleanProperty(_matrixGraph);
  _displayedNodesToGraphEntities = new IntegerProperty(_matrixGraph);
  _displayedEdgesToGraphEdges = new IntegerProperty(_matrixGraph);
  createScene(_matrixGraph,DataSet());

  Observable::holdObservers();
  node n;
  forEach(n, graph()->getNodes())
  addNode(graph(), n);

  edge e;
  forEach(e, graph()->getEdges())
  addEdge(graph(), e);
  Observable::unholdObservers();

  GlGraphInputData *inputData = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();
  _sourceToTargetProperties.clear();
  _sourceToTargetProperties.insert(inputData->getElementColor()->getName());
  _sourceToTargetProperties.insert(inputData->getElementShape()->getName());
  _sourceToTargetProperties.insert(inputData->getElementLabel()->getName());
  _sourceToTargetProperties.insert(inputData->getElementFont()->getName());
  _sourceToTargetProperties.insert(inputData->getElementFontSize()->getName());
  _sourceToTargetProperties.insert(inputData->getElementBorderWidth()->getName());
  _sourceToTargetProperties.insert(inputData->getElementBorderColor()->getName());
  _sourceToTargetProperties.insert(inputData->getElementLabelColor()->getName());
  _sourceToTargetProperties.insert(inputData->getElementSelected()->getName());
  _sourceToTargetProperties.insert(inputData->getElementTexture()->getName());
  set<string> targetToSourceProperties;
  targetToSourceProperties.insert(inputData->getElementSelected()->getName());
  _dispatcher = new PropertyValuesDispatcher(graph(), _matrixGraph, _sourceToTargetProperties, targetToSourceProperties, _graphEntitiesToDisplayedNodes, _displayedNodesAreNodes, _displayedNodesToGraphEntities, _displayedEdgesToGraphEdges, _edgesMap);

  GlGraphRenderingParameters *renderingParameters = getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParametersPointer();
  renderingParameters->setLabelScaled(true);
  renderingParameters->setLabelsDensity(100);
  renderingParameters->setAntialiasing(false);

  _configurationWidget->setBackgroundColor(colorToQColor(getGlMainWidget()->getScene()->getBackgroundColor()));
  addGridBackground();

  if (_mustUpdateSizes) {
    normalizeSizes();
    _mustUpdateSizes = false;
  }

  if (_mustUpdateLayout) {
    updateLayout();
    _mustUpdateLayout = false;
  }

  centerView();
}

void MatrixView::normalizeSizes(double maxVal) {
  float maxWidth=FLT_MIN, maxHeight=FLT_MIN;
  SizeProperty *originalSizes = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementSize();
  SizeProperty *matrixSizes = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementSize();

  node n;
  forEach(n, graph()->getNodes()) {
    Size s(originalSizes->getNodeValue(n));
    maxWidth = max<float>(maxWidth,s[0]);
    maxHeight = max<float>(maxHeight,s[1]);
  }

  Observable::holdObservers();
  forEach(n, _matrixGraph->getNodes()) {
    if (!_displayedNodesAreNodes->getNodeValue(n))
      continue;

    Size s(originalSizes->getNodeValue(node(_displayedNodesToGraphEntities->getNodeValue(n))));
    matrixSizes->setNodeValue(n, Size(s[0]*maxVal/maxWidth, s[1]*maxVal/maxHeight, 1));
  }
  Observable::unholdObservers();
}

void MatrixView::addNode(tlp::Graph *, const tlp::node n) {
  _mustUpdateLayout = true;
  _mustUpdateSizes=true;

  for (int i=0; i <= 1; ++i) {
    node dispNode = _matrixGraph->addNode();
    {
      vector<int> vect = _graphEntitiesToDisplayedNodes->getNodeValue(n);
      vect.push_back(dispNode.id);
      _graphEntitiesToDisplayedNodes->setNodeValue(n, vect);
    }
    _displayedNodesToGraphEntities->setNodeValue(dispNode, n.id);
    _displayedNodesAreNodes->setNodeValue(dispNode, true);
    getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementLabelPosition()->setNodeValue(dispNode, (i == 1 ? LabelPosition::Center : LabelPosition::Top));

    for (set<string>::iterator it = _sourceToTargetProperties.begin(); it != _sourceToTargetProperties.end(); ++it)
      _matrixGraph->getProperty(*it)->setNodeStringValue(dispNode, graph()->getProperty(*it)->getNodeStringValue(n));

  }
}

void MatrixView::addEdge(tlp::Graph *g, const tlp::edge e) {
  _mustUpdateLayout = true;
  _mustUpdateSizes=true;

  for (int i=0; i <= 1; ++i) {
    node dispEdge = _matrixGraph->addNode();
    {
      vector<int> vect = _graphEntitiesToDisplayedNodes->getEdgeValue(e);
      vect.push_back(dispEdge.id);
      _graphEntitiesToDisplayedNodes->setEdgeValue(e, vect);
    }
    _displayedNodesToGraphEntities->setNodeValue(dispEdge, e.id);
    _displayedNodesAreNodes->setNodeValue(dispEdge, false);

    for (set<string>::iterator it = _sourceToTargetProperties.begin(); it != _sourceToTargetProperties.end(); ++it)
      _matrixGraph->getProperty(*it)->setNodeStringValue(dispEdge, graph()->getProperty(*it)->getEdgeStringValue(e));
  }

  const std::pair<node, node>& eEnds = g->ends(e);

  node dispSrc = node(_graphEntitiesToDisplayedNodes->getNodeValue(eEnds.first)[0]);

  node dispTgt = node(_graphEntitiesToDisplayedNodes->getNodeValue(eEnds.second)[0]);

  edge dispEdge = _matrixGraph->addEdge(dispSrc,dispTgt);

  _edgesMap[e] = dispEdge;

  _displayedEdgesToGraphEdges->setEdgeValue(dispEdge, e.id);

  ColorProperty* originalColors = graph()->getProperty<ColorProperty>("viewColor");

  ColorProperty* colors = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementColor();

  colors->setEdgeValue(dispEdge,originalColors->getEdgeValue(e));
}

void MatrixView::delNode(tlp::Graph *,const tlp::node n) {
  _mustUpdateLayout = true;
  _mustUpdateSizes=true;

  vector<int> vect = _graphEntitiesToDisplayedNodes->getNodeValue(n);

  for (vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
    _matrixGraph->delNode(node(*it));
}

void MatrixView::delEdge(tlp::Graph *,const tlp::edge e) {
  _mustUpdateLayout = true;
  _mustUpdateSizes=true;

  vector<int> vect = _graphEntitiesToDisplayedNodes->getEdgeValue(e);

  for (vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
    _matrixGraph->delNode(node(*it));

  _matrixGraph->delEdge(_edgesMap[e]);
  _edgesMap.remove(e);
}

template<typename PROPTYPE>
struct AscendingPropertySorter {
  PROPTYPE *prop;
  AscendingPropertySorter(PropertyInterface *_prop) :
    prop(static_cast<PROPTYPE*>(_prop)) {}
  bool operator()(node a,node b) {
    return prop->getNodeValue(a) < prop->getNodeValue(b);
  }
};

template<typename PROPTYPE>
struct DescendingPropertySorter {
  PROPTYPE *prop;
  DescendingPropertySorter(PropertyInterface *_prop) :
    prop(static_cast<PROPTYPE*>(_prop)) {}
  bool operator()(node a,node b) {
    return prop->getNodeValue(a) > prop->getNodeValue(b);
  }
};

struct DescendingIdSorter {
  bool operator()(node a, node b) {
    return a.id > b.id;
  }
};

void MatrixView::updateNodesOrder() {
  _orderedNodes.clear();
  _orderedNodes.resize(graph()->numberOfNodes());
  int i=0;
  node n;
  forEach(n, graph()->getNodes())
  _orderedNodes[i++] = n;

  if (graph()->existProperty(_orderingMetricName)) {
    PropertyInterface *pi = graph()->getProperty(_orderingMetricName);

    if (pi->getTypename() == "double") {
      if (_configurationWidget->ascendingOrder())
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             AscendingPropertySorter<DoubleProperty>(pi));
      else
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             DescendingPropertySorter<DoubleProperty>(pi));

    }
    else if (pi->getTypename() == "int") {
      if (_configurationWidget->ascendingOrder())
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             AscendingPropertySorter<IntegerProperty>(pi));
      else
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             DescendingPropertySorter<IntegerProperty>(pi));
    }
    else if (pi->getTypename() == "string") {
      if (_configurationWidget->ascendingOrder())
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             AscendingPropertySorter<StringProperty>(pi));
      else
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             DescendingPropertySorter<StringProperty>(pi));
    }
  }
  else if (!_configurationWidget->ascendingOrder())
    sort(_orderedNodes.begin(), _orderedNodes.end(),
         DescendingIdSorter());
}

void MatrixView::updateLayout() {
  holdObservers();
  updateNodesOrder();

  LayoutProperty *layout = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementLayout();
  Coord horiz(1,0,0), vert(0,-1,0);

  for (vector<node>::iterator it = _orderedNodes.begin(); it != _orderedNodes.end(); ++it) {
    vector<int> dispNodes = _graphEntitiesToDisplayedNodes->getNodeValue(node(*it));
    layout->setNodeValue(node(dispNodes[0]), horiz);
    layout->setNodeValue(node(dispNodes[1]), vert);
    horiz[0]+=1;
    vert[1]-=1;
  }

  IntegerProperty* shapes = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementShape();
  int shape = GlyphManager::getInst().glyphId("2D - Square");
  edge e;
  forEach (e, graph()->getEdges()) {
    const std::pair<node, node>& eEnds = graph()->ends(e);
    vector<int> srcNodes = _graphEntitiesToDisplayedNodes->getNodeValue(eEnds.first),
                tgtNodes = _graphEntitiesToDisplayedNodes->getNodeValue(eEnds.second),
                edgeNodes = _graphEntitiesToDisplayedNodes->getEdgeValue(e);

    // 0 => horizontal line, 1 => vertical line
    Coord src0 = layout->getNodeValue(node(srcNodes[0])), tgt0 = layout->getNodeValue(node(tgtNodes[0])),
          src1 = layout->getNodeValue(node(srcNodes[1])), tgt1 = layout->getNodeValue(node(tgtNodes[1]));

    layout->setNodeValue(node(edgeNodes[0]), Coord(src0[0], tgt1[1], 0));
    layout->setNodeValue(node(edgeNodes[1]), Coord(tgt0[0], src1[1], 0));
    shapes->setNodeValue(node(edgeNodes[0]),shape);
    shapes->setNodeValue(node(edgeNodes[1]),shape);
  }

  forEach(e, _matrixGraph->getEdges()) {
    const std::pair<node, node>& eEnds = _matrixGraph->ends(e);
    node src = eEnds.first;
    node tgt = eEnds.second;

    Coord srcPos = layout->getNodeValue(src);
    Coord tgtPos = layout->getNodeValue(tgt);
    float xMax = max(srcPos[0],tgtPos[0]);
    float xMin = min(srcPos[0],tgtPos[0]);
    float dist = (xMax - xMin);
    Coord b(xMin+dist,srcPos[1]+dist/2.,0);
    std::vector<Coord> bends(4);
    bends[0] = srcPos;
    bends[1] = srcPos;
    bends[1][1] += dist/3. + 1.;
    bends[2] = tgtPos;
    bends[2][1] += dist/3. + 1.;
    bends[3] = tgtPos;
    vector<Coord> curvePoints;
    computeBezierPoints(bends,curvePoints,20);
    layout->setEdgeValue(e,curvePoints);
  }

  unholdObservers();
}

void MatrixView::setBackgroundColor(QColor c) {
  getGlMainWidget()->getScene()->setBackgroundColor(QColorToColor(c));
  emit drawNeeded();
}

void MatrixView::setOrderingMetric(const std::string& name) {
  if (name != "" && !graph()->existProperty(name))
    return;

  if (graph()->existProperty(_orderingMetricName))
    graph()->getProperty(_orderingMetricName)->removeObserver(this);

  _orderingMetricName = name;

  if (graph()->existProperty(name))
    graph()->getProperty(name)->addObserver(this);

  _mustUpdateLayout = true;
  emit drawNeeded();
}

void MatrixView::setGridDisplayMode() {
  emit drawNeeded();
}

void MatrixView::registerTriggers() {
  foreach (tlp::Observable* obs, triggers()) {
    removeRedrawTrigger(obs);
  }

  if(_matrixGraph) {
    addRedrawTrigger(_matrixGraph);
    Iterator<string> *it = _matrixGraph->getProperties();

    while(it->hasNext()) {
      PropertyInterface *property=_matrixGraph->getProperty(it->next());
      addRedrawTrigger(property);
    }
  }
}

void MatrixView::addGridBackground() {
  removeGridBackground();
  GlLayer *backgroundLayer = getGlMainWidget()->getScene()->getLayer("MatrixView_Background");
  backgroundLayer->addGlEntity(new GlMatrixBackgroundGrid(this), "MatrixView_backgroundGrid");
}

void MatrixView::removeGridBackground() {
  GlLayer *backgroundLayer = getGlMainWidget()->getScene()->getLayer("MatrixView_Background");

  if (!backgroundLayer) {
    backgroundLayer = new GlLayer("MatrixView_Background", &(getGlMainWidget()->getScene()->getLayer("Main")->getCamera()), true);
    backgroundLayer->clear();
    getGlMainWidget()->getScene()->addExistingLayerBefore(backgroundLayer, "Main");
  }
  else {
    GlEntity *entity = backgroundLayer->findGlEntity("MatrixView_backgroundGrid");
    delete entity;
  }
}

void MatrixView::applySettings() {
  _mustUpdateLayout = true;
  emit drawNeeded();
}

PLUGIN(MatrixView)
