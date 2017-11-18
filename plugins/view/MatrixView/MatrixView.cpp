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
#include <QGraphicsView>
#include <QAction>
#include <QMenu>

#include "MatrixView.h"
#include "PropertyValuesDispatcher.h"
#include "GlMatrixBackgroundGrid.h"
#include "MatrixViewQuickAccessBar.h"

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

using namespace std;

namespace tlp {

MatrixView::MatrixView(const PluginContext *)
    : NodeLinkDiagramComponent(), _bar(NULL), _matrixGraph(NULL),
      _graphEntitiesToDisplayedNodes(NULL), _displayedNodesToGraphEntities(NULL),
      _displayedEdgesToGraphEdges(NULL), _displayedNodesAreNodes(NULL), _dispatcher(NULL),
      _configurationWidget(NULL), _mustUpdateSizes(false), _mustUpdateLayout(false),
      _isOriented(false), _orderingMetricName("") {}

MatrixView::~MatrixView() {
  deleteDisplayedGraph();
}

QuickAccessBar *MatrixView::getQuickAccessBarImpl() {
  _bar = new MatrixViewQuickAccessBar(_configurationWidget);
  connect(_bar, SIGNAL(settingsChanged()), this, SLOT(applySettings()));
  return _bar;
}

void MatrixView::setState(const DataSet &ds) {

  clearRedrawTriggers();

  setOverviewVisible(true);

  if (!_configurationWidget) {
    _configurationWidget = new MatrixViewConfigurationWidget();
    connect(_configurationWidget, SIGNAL(changeBackgroundColor(QColor)), this,
            SLOT(setBackgroundColor(QColor)));
    connect(_configurationWidget, SIGNAL(metricSelected(std::string)), this,
            SLOT(setOrderingMetric(std::string)));
    connect(_configurationWidget, SIGNAL(setGridDisplayMode()), this, SLOT(setGridDisplayMode()));
    connect(_configurationWidget, SIGNAL(showEdges(bool)), this, SLOT(showEdges(bool)));
    connect(_configurationWidget, SIGNAL(enableEdgeColorInterpolation(bool)), this,
            SLOT(enableEdgeColorInterpolation(bool)));
    connect(_configurationWidget, SIGNAL(updateOriented(bool)), this, SLOT(setOriented(bool)));
  }

  _configurationWidget->setGraph(graph());

  initDisplayedGraph();
  registerTriggers();

  bool status = true;
  ds.get("show Edges", status);
  showEdges(status);
  _configurationWidget->setDisplayEdges(status);

  ds.get("ascending order", status);
  _configurationWidget->setAscendingOrder(status);

  Color c = getGlMainWidget()->getScene()->getBackgroundColor();
  ds.get("Background Color", c);
  _configurationWidget->setBackgroundColor(tlp::colorToQColor(c));

  unsigned grid = 0;
  ds.get("Grid mode", grid);
  _configurationWidget->setgridmode(grid);

  int orderingindex = 0;
  ds.get("ordering", orderingindex);
  _configurationWidget->setOrderingProperty(orderingindex);

  status = false;
  ds.get("oriented", status);
  _isOriented = status;
  _configurationWidget->setOriented(status);
  status = false;

  ds.get("edge color interpolation", status);
  enableEdgeColorInterpolation(status);
  _configurationWidget->setEdgeColorInterpolation(status);

  bool quickAccessBarVisible = false;

  if (ds.get<bool>("quickAccessBarVisible", quickAccessBarVisible)) {
    needQuickAccessBar = true;
    setQuickAccessBarVisible(quickAccessBarVisible);
  } else // display quickaccessbar
    setQuickAccessBarVisible(true);
}

void MatrixView::showEdges(bool show) {
  getGlMainWidget()
      ->getScene()
      ->getGlGraphComposite()
      ->getRenderingParametersPointer()
      ->setDisplayEdges(show);
  emit drawNeeded();
}

void MatrixView::enableEdgeColorInterpolation(bool flag) {
  getGlMainWidget()
      ->getScene()
      ->getGlGraphComposite()
      ->getRenderingParametersPointer()
      ->setEdgeColorInterpolate(flag);
  emit drawNeeded();
}

void MatrixView::setOriented(bool flag) {
  if (flag != _isOriented) {
    _isOriented = flag;
    Observable::holdObservers();

    if (_isOriented) {
      edge e;
      forEach(e, graph()->getEdges()) {
        // delete the second node mapping the current edge
        vector<int> edgeNodes = _graphEntitiesToDisplayedNodes->getEdgeValue(e);
        _matrixGraph->delNode(node(edgeNodes[1]));
        edgeNodes.resize(1);
        _graphEntitiesToDisplayedNodes->setEdgeValue(e, edgeNodes);
      }
    } else {
      edge e;
      forEach(e, graph()->getEdges()) {
        // must add the symetric node
        vector<int> edgeNodes = _graphEntitiesToDisplayedNodes->getEdgeValue(e);
        edgeNodes.push_back(_matrixGraph->addNode().id);
        _graphEntitiesToDisplayedNodes->setEdgeValue(e, edgeNodes);

        // layout and shape will be updated in updataLayout method
        // but other view properties must be set now
        for (set<string>::iterator it = _sourceToTargetProperties.begin();
             it != _sourceToTargetProperties.end(); ++it) {
          PropertyInterface *prop = _matrixGraph->getProperty(*it);
          prop->setNodeStringValue(node(edgeNodes[1]),
                                   prop->getNodeStringValue(node(edgeNodes[0])));
        }
      }
    }

    Observable::unholdObservers();
    emit drawNeeded();
  }
}

void MatrixView::graphChanged(Graph *) {
  setState(DataSet());
}

DataSet MatrixView::state() const {
  DataSet ds;
  ds.set("show Edges", getGlMainWidget()
                           ->getScene()
                           ->getGlGraphComposite()
                           ->getRenderingParametersPointer()
                           ->isDisplayEdges());
  ds.set("edge color interpolation", getGlMainWidget()
                                         ->getScene()
                                         ->getGlGraphComposite()
                                         ->getRenderingParametersPointer()
                                         ->isEdgeColorInterpolate());
  ds.set("ascending order", _configurationWidget->ascendingOrder());
  ds.set("Grid mode", _configurationWidget->gridDisplayMode());
  ds.set("Background Color", getGlMainWidget()->getScene()->getBackgroundColor());
  ds.set("ordering", _configurationWidget->orderingProperty());
  ds.set("oriented", _isOriented);

  if (needQuickAccessBar)
    ds.set("quickAccessBarVisible", quickAccessBarVisible());

  return ds;
}

QList<QWidget *> MatrixView::configurationWidgets() const {
  return QList<QWidget *>() << _configurationWidget;
}

void MatrixView::fillContextMenu(QMenu *menu, const QPointF &point) {
  GlMainView::fillContextMenu(menu, point);
  // Check if a node/edge is under the mouse pointer
  SelectedEntity entity;

  if (getGlMainWidget()->pickNodesEdges(point.x(), point.y(), entity)) {
    menu->addSeparator();
    isNode = entity.getEntityType() == SelectedEntity::NODE_SELECTED;
    itemId = entity.getComplexEntityId();
    QString sId = QString::number(itemId);

    if (isNode) {
      if (!_displayedNodesAreNodes->getNodeValue(node(itemId)))
        isNode = false;

      itemId = _displayedNodesToGraphEntities->getNodeValue(node(itemId));
    } else
      itemId = _displayedEdgesToGraphEdges->getEdgeValue(edge(itemId));

    menu->addAction((isNode ? trUtf8("Node #") : trUtf8("Edge #")) + sId)->setEnabled(false);

    menu->addSeparator();

    QAction *action =
        menu->addAction(tr("Toggle selection"), this, SLOT(addRemoveItemToSelection()));
    action->setToolTip(QString("Invert the selection of the ") + (isNode ? "node #" : "edge #") +
                       sId);
    action = menu->addAction(tr("Select"), this, SLOT(selectItem()));
    action->setToolTip(QString("Select the ") + (isNode ? "node #" : "edge #") + sId);
    action = menu->addAction(tr("Delete"), this, SLOT(deleteItem()));
    action->setToolTip(QString("Delete the ") + (isNode ? "node #" : "edge #") + sId);
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
  foreach (tlp::Observable *obs, triggers()) { removeRedrawTrigger(obs); }

  delete _matrixGraph;
  _matrixGraph = NULL;
  delete _graphEntitiesToDisplayedNodes;
  _graphEntitiesToDisplayedNodes = NULL;
  delete _displayedNodesToGraphEntities;
  _displayedNodesToGraphEntities = NULL;
  delete _displayedEdgesToGraphEdges;
  _displayedEdgesToGraphEdges = NULL;
  delete _displayedNodesAreNodes;
  _displayedNodesAreNodes = NULL;
  delete _dispatcher;
  _dispatcher = NULL;
}

void MatrixView::initDisplayedGraph() {
  _mustUpdateLayout = true;
  _mustUpdateSizes = true;

  deleteDisplayedGraph();

  if (graph() == NULL) {
    return;
  }

  _matrixGraph = newGraph();
  _matrixGraph->reserveNodes(2 * (graph()->numberOfNodes() + graph()->numberOfEdges()));
  _matrixGraph->reserveEdges(graph()->numberOfEdges());

  _graphEntitiesToDisplayedNodes = new IntegerVectorProperty(graph());
  _displayedNodesAreNodes = new BooleanProperty(_matrixGraph);
  _displayedNodesToGraphEntities = new IntegerProperty(_matrixGraph);
  _displayedEdgesToGraphEdges = new IntegerProperty(_matrixGraph);
  createScene(_matrixGraph, DataSet());

  Observable::holdObservers();
  node n;
  forEach(n, graph()->getNodes()) addNode(graph(), n);

  edge e;
  forEach(e, graph()->getEdges()) addEdge(graph(), e);
  Observable::unholdObservers();

  GlGraphInputData *inputData =
      getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();
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
  _dispatcher = new PropertyValuesDispatcher(
      graph(), _matrixGraph, _sourceToTargetProperties, targetToSourceProperties,
      _graphEntitiesToDisplayedNodes, _displayedNodesAreNodes, _displayedNodesToGraphEntities,
      _displayedEdgesToGraphEdges, _edgesMap);

  GlGraphRenderingParameters *renderingParameters =
      getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParametersPointer();
  renderingParameters->setLabelScaled(true);
  renderingParameters->setLabelsDensity(100);

  _configurationWidget->setBackgroundColor(
      colorToQColor(getGlMainWidget()->getScene()->getBackgroundColor()));
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
  if (graph() == NULL)
    return;

  float maxWidth = FLT_MIN, maxHeight = FLT_MIN;
  SizeProperty *originalSizes =
      getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementSize();
  SizeProperty *matrixSizes =
      getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementSize();

  node n;
  forEach(n, graph()->getNodes()) {
    Size s(originalSizes->getNodeValue(n));
    maxWidth = max<float>(maxWidth, s[0]);
    maxHeight = max<float>(maxHeight, s[1]);
  }

  Observable::holdObservers();
  forEach(n, _matrixGraph->getNodes()) {
    if (!_displayedNodesAreNodes->getNodeValue(n))
      continue;

    Size s(originalSizes->getNodeValue(node(_displayedNodesToGraphEntities->getNodeValue(n))));
    matrixSizes->setNodeValue(n, Size(s[0] * maxVal / maxWidth, s[1] * maxVal / maxHeight, 1));
  }
  Observable::unholdObservers();
}

void MatrixView::addNode(tlp::Graph *, const tlp::node n) {
  _mustUpdateLayout = true;
  _mustUpdateSizes = true;

  vector<int> nodeToDisplayedNodes;
  nodeToDisplayedNodes.reserve(2);

  for (int i = 0; i < 2; ++i) {
    node dispNode = _matrixGraph->addNode();
    nodeToDisplayedNodes.push_back(dispNode);
    _displayedNodesToGraphEntities->setNodeValue(dispNode, n.id);
    _displayedNodesAreNodes->setNodeValue(dispNode, true);
  }

  _graphEntitiesToDisplayedNodes->setNodeValue(n, nodeToDisplayedNodes);
}

void MatrixView::addEdge(tlp::Graph *g, const tlp::edge e) {
  _mustUpdateLayout = true;
  _mustUpdateSizes = true;

  vector<int> edgeToDisplayedNodes;
  edgeToDisplayedNodes.reserve(2);

  for (int i = 0; i < 2; ++i) {
    node dispEdge = _matrixGraph->addNode();
    edgeToDisplayedNodes.push_back(dispEdge);
    _displayedNodesToGraphEntities->setNodeValue(dispEdge, e.id);
    _displayedNodesAreNodes->setNodeValue(dispEdge, false);
  }

  _graphEntitiesToDisplayedNodes->setEdgeValue(e, edgeToDisplayedNodes);

  const std::pair<node, node> &eEnds = g->ends(e);

  node dispSrc = node(_graphEntitiesToDisplayedNodes->getNodeValue(eEnds.first)[0]);

  node dispTgt = node(_graphEntitiesToDisplayedNodes->getNodeValue(eEnds.second)[0]);

  edge dispEdge = _matrixGraph->addEdge(dispSrc, dispTgt);

  _edgesMap[e] = dispEdge;

  _displayedEdgesToGraphEdges->setEdgeValue(dispEdge, e.id);

  ColorProperty *originalColors = graph()->getProperty<ColorProperty>("viewColor");
  ColorProperty *colors =
      getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementColor();

  colors->setEdgeValue(dispEdge, originalColors->getEdgeValue(e));
}

void MatrixView::treatEvent(const Event &message) {
  const GraphEvent *graphEvent = dynamic_cast<const GraphEvent *>(&message);

  if (graphEvent) {
    if (graphEvent->getType() == GraphEvent::TLP_ADD_EDGE)
      addEdge(graphEvent->getGraph(), graphEvent->getEdge());

    if (graphEvent->getType() == GraphEvent::TLP_DEL_NODE)
      delNode(graphEvent->getGraph(), graphEvent->getNode());

    if (graphEvent->getType() == GraphEvent::TLP_DEL_EDGE)
      delEdge(graphEvent->getGraph(), graphEvent->getEdge());
  }
}

void MatrixView::delNode(tlp::Graph *, const tlp::node n) {
  _mustUpdateLayout = true;
  _mustUpdateSizes = true;

  vector<int> vect = _graphEntitiesToDisplayedNodes->getNodeValue(n);

  for (vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
    _matrixGraph->delNode(node(*it));
}

void MatrixView::delEdge(tlp::Graph *, const tlp::edge e) {
  _mustUpdateLayout = true;
  _mustUpdateSizes = true;

  vector<int> vect = _graphEntitiesToDisplayedNodes->getEdgeValue(e);

  for (vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
    _matrixGraph->delNode(node(*it));

  _matrixGraph->delEdge(_edgesMap[e]);
  _edgesMap.remove(e);
}

template <typename PROPTYPE>
struct AscendingPropertySorter {
  PROPTYPE *prop;
  AscendingPropertySorter(PropertyInterface *_prop) : prop(static_cast<PROPTYPE *>(_prop)) {}
  bool operator()(node a, node b) {
    return prop->getNodeValue(a) < prop->getNodeValue(b);
  }
};

template <typename PROPTYPE>
struct DescendingPropertySorter {
  PROPTYPE *prop;
  DescendingPropertySorter(PropertyInterface *_prop) : prop(static_cast<PROPTYPE *>(_prop)) {}
  bool operator()(node a, node b) {
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
  int i = 0;
  node n;
  forEach(n, graph()->getNodes()) _orderedNodes[i++] = n;

  if (graph()->existProperty(_orderingMetricName)) {
    PropertyInterface *pi = graph()->getProperty(_orderingMetricName);

    if (pi->getTypename() == "double") {
      if (_configurationWidget->ascendingOrder())
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             AscendingPropertySorter<DoubleProperty>(pi));
      else
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             DescendingPropertySorter<DoubleProperty>(pi));

    } else if (pi->getTypename() == "int") {
      if (_configurationWidget->ascendingOrder())
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             AscendingPropertySorter<IntegerProperty>(pi));
      else
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             DescendingPropertySorter<IntegerProperty>(pi));
    } else if (pi->getTypename() == "string") {
      if (_configurationWidget->ascendingOrder())
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             AscendingPropertySorter<StringProperty>(pi));
      else
        sort(_orderedNodes.begin(), _orderedNodes.end(),
             DescendingPropertySorter<StringProperty>(pi));
    }
  } else if (!_configurationWidget->ascendingOrder())
    sort(_orderedNodes.begin(), _orderedNodes.end(), DescendingIdSorter());
}

void MatrixView::updateLayout() {
  if (graph() == NULL)
    return;

  holdObservers();
  updateNodesOrder();

  LayoutProperty *layout =
      getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementLayout();
  Coord horiz(1, 0, 0), vert(0, -1, 0);
  IntegerProperty *position = getGlMainWidget()
                                  ->getScene()
                                  ->getGlGraphComposite()
                                  ->getInputData()
                                  ->getElementLabelPosition();

  for (vector<node>::iterator it = _orderedNodes.begin(); it != _orderedNodes.end(); ++it) {
    vector<int> dispNodes = _graphEntitiesToDisplayedNodes->getNodeValue(node(*it));
    layout->setNodeValue(node(dispNodes[0]), horiz);
    position->setNodeValue(node(dispNodes[0]), LabelPosition::Top);
    layout->setNodeValue(node(dispNodes[1]), vert);
    position->setNodeValue(node(dispNodes[1]), LabelPosition::Left);
    horiz[0] += 1;
    vert[1] -= 1;
  }

  IntegerProperty *shapes =
      getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementShape();
  int shape = GlyphManager::getInst().glyphId("2D - Square");
  edge e;
  forEach(e, graph()->getEdges()) {
    const std::pair<node, node> eEnds = graph()->ends(e);
    vector<int> srcNodes = _graphEntitiesToDisplayedNodes->getNodeValue(eEnds.first),
                tgtNodes = _graphEntitiesToDisplayedNodes->getNodeValue(eEnds.second),
                edgeNodes = _graphEntitiesToDisplayedNodes->getEdgeValue(e);

    // 0 => horizontal line, 1 => vertical line
    Coord src0 = layout->getNodeValue(node(srcNodes[0])),
          tgt0 = layout->getNodeValue(node(tgtNodes[0])),
          src1 = layout->getNodeValue(node(srcNodes[1])),
          tgt1 = layout->getNodeValue(node(tgtNodes[1]));

    layout->setNodeValue(node(edgeNodes[0]), Coord(tgt0[0], src1[1], 0));
    shapes->setNodeValue(node(edgeNodes[0]), shape);

    if (_isOriented == false) {
      layout->setNodeValue(node(edgeNodes[1]), Coord(src0[0], tgt1[1], 0));
      shapes->setNodeValue(node(edgeNodes[1]), shape);
    }
  }

  forEach(e, _matrixGraph->getEdges()) {
    const std::pair<node, node> eEnds = _matrixGraph->ends(e);
    node src = eEnds.first;
    node tgt = eEnds.second;

    Coord srcPos = layout->getNodeValue(src);
    Coord tgtPos = layout->getNodeValue(tgt);
    float xMax = max(srcPos[0], tgtPos[0]);
    float xMin = min(srcPos[0], tgtPos[0]);
    float dist = (xMax - xMin);
    std::vector<Coord> bends(4);
    bends[0] = srcPos;
    bends[1] = srcPos;
    bends[1][1] += dist / 3. + 1.;
    bends[2] = tgtPos;
    bends[2][1] += dist / 3. + 1.;
    bends[3] = tgtPos;
    vector<Coord> curvePoints;
    computeBezierPoints(bends, curvePoints, 20);
    layout->setEdgeValue(e, curvePoints);
  }

  unholdObservers();
}

void MatrixView::setBackgroundColor(QColor c) {
  getGlMainWidget()->getScene()->setBackgroundColor(QColorToColor(c));
  emit drawNeeded();
}

void MatrixView::setOrderingMetric(const std::string &name) {
  if (!name.empty() && !graph()->existProperty(name))
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
  foreach (tlp::Observable *obs, triggers()) { removeRedrawTrigger(obs); }

  if (graph()) {
    addRedrawTrigger(graph());
    Iterator<string> *it = _matrixGraph->getProperties();

    while (it->hasNext()) {
      PropertyInterface *property = _matrixGraph->getProperty(it->next());
      addRedrawTrigger(property);
    }

    delete it;
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
    backgroundLayer =
        new GlLayer("MatrixView_Background",
                    &(getGlMainWidget()->getScene()->getLayer("Main")->getCamera()), true);
    backgroundLayer->clear();
    getGlMainWidget()->getScene()->addExistingLayerBefore(backgroundLayer, "Main");
  } else {
    GlEntity *entity = backgroundLayer->findGlEntity("MatrixView_backgroundGrid");
    delete entity;
  }
}

void MatrixView::applySettings() {
  _mustUpdateLayout = true;
  emit drawNeeded();
}

PLUGIN(MatrixView)
}
