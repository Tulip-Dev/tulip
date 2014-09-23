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
#include "ui_GridOptionsWidget.h"

#include <QGraphicsView>
#include <QActionGroup>
#include <QDialog>
#include <QMenu>
#include <QHelpEvent>
#include <QToolTip>
#include <QString>
#include <QEvent>

#include <tulip/GlMetaNodeRenderer.h>
#include <tulip/GlGrid.h>
#include <tulip/DrawingTools.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/ParameterListModel.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Gl2DRect.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlOverviewGraphicsItem.h>
#include <tulip/Interactor.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlCompositeHierarchyManager.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GraphModel.h>

using namespace tlp;
using namespace std;

const string NodeLinkDiagramComponent::viewName("Node Link Diagram view");

NodeLinkDiagramComponent::NodeLinkDiagramComponent(const tlp::PluginContext*): _grid(NULL), _gridOptions(NULL),_hasHulls(false),_tooltips(false),grid_ui(NULL) {
}

NodeLinkDiagramComponent::~NodeLinkDiagramComponent() {
  delete grid_ui;
}

void NodeLinkDiagramComponent::updateGrid() {
  delete _grid;
  _grid = NULL;

  if(_gridOptions==NULL)
    return;

  DataSet gridData = static_cast<ParameterListModel*>(_gridOptions->findChild<QTableView*>()->model())->parametersValues();
  StringCollection gridMode;
  gridData.get<StringCollection>("Grid mode",gridMode);
  int mode = gridMode.getCurrent();

  if (mode == 0)
    return;

  Coord margins;
  Size gridSize;
  Color gridColor;
  bool onX=true,onY=true,onZ=true;
  gridData.get<Coord>("Margins",margins);
  gridData.get<Size>("Grid size",gridSize);
  gridData.get<Color>("Grid color",gridColor);
  gridData.get<bool>("X grid",onX);
  gridData.get<bool>("Y grid",onY);
  gridData.get<bool>("Z grid",onZ);

  GlGraphInputData* inputData = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData();
  BoundingBox graphBB = computeBoundingBox(graph(),inputData->getElementLayout(),inputData->getElementSize(),inputData->getElementRotation());
  Coord bottomLeft = Coord(graphBB[0] - margins);
  Coord topRight = Coord(graphBB[1] + margins);

  if (mode == 1) {
    for (int i=0; i<3; ++i)
      gridSize[i] = abs(topRight[i] - bottomLeft[i]) / gridSize[i];
  }

  bool displays[3];
  displays[0] = onX;
  displays[1] = onY;
  displays[2] = onZ;

  _grid = new GlGrid(bottomLeft,
                     topRight,
                     gridSize,
                     gridColor,
                     displays);
  getGlMainWidget()->getScene()->getLayer("Main")->addGlEntity(_grid,"Node Link Diagram Component grid");
}

void NodeLinkDiagramComponent::draw() {
  updateGrid();
  GlMainView::draw();
}

void NodeLinkDiagramComponent::setupWidget() {
  GlMainView::setupWidget();
  graphicsView()->installEventFilter(this);//Handle tooltip events
}

bool NodeLinkDiagramComponent::eventFilter(QObject *obj,QEvent *event) {
  if(_tooltips==true && event->type()==QEvent::ToolTip) {
    SelectedEntity type;
    QHelpEvent *he = static_cast<QHelpEvent *>(event);
    GlMainWidget *gl = getGlMainWidget();

    if (gl->pickNodesEdges(he->x(), he->y(), type)) {
      // try to show the viewLabel if any
      StringProperty *labels = graph()->getProperty<StringProperty>("viewLabel");
      std::string label;
      QString ttip;
      node tmpNode=type.getNode();

      if(tmpNode.isValid()) {
        label = labels->getNodeValue(tmpNode);

        if (!label.empty())
          ttip = tlpStringToQString(label) + " (";

        ttip += QString("node #")+ QString::number(tmpNode.id);

        if (!label.empty())
          ttip += ")";

        QToolTip::showText(he->globalPos(), ttip, gl);
        return true;
      }
      else {
        edge tmpEdge=type.getEdge();

        if(tmpEdge.isValid()) {
          label = labels->getEdgeValue(tmpEdge);

          if (!label.empty())
            ttip = tlpStringToQString(label) + "(";

          ttip += QString("edge #")+QString::number(tmpEdge.id);

          if (!label.empty())
            ttip += ")";

          QToolTip::showText(he->globalPos(), ttip, gl);
          return true;
        }
      }
    }
    else { //be sure to hide the tooltip if the mouse cursor is not under a node or an edge
      QToolTip::hideText();
      event->ignore();
    }
  }

  // standard event processing
  return GlMainView::eventFilter(obj, event);
}

void NodeLinkDiagramComponent::setState(const tlp::DataSet& data) {
  ParameterDescriptionList gridParameters;
  gridParameters.add<StringCollection>("Grid mode","","No grid;Space divisions;Fixed size",true);
  gridParameters.add<Size>("Grid size","","(1,1,1)",false);
  gridParameters.add<Size>("Margins","","(0.5,0.5,0.5)",false);
  gridParameters.add<Color>("Grid color","","(0,0,0,255)",false);
  gridParameters.add<bool>("X grid","","true",false);
  gridParameters.add<bool>("Y grid","","true",false);
  gridParameters.add<bool>("Z grid","","true",false);
  ParameterListModel* model = new ParameterListModel(gridParameters,NULL,this);

  grid_ui = new Ui::GridOptionsWidget;
  _gridOptions = new QDialog(graphicsView());
  grid_ui->setupUi(_gridOptions);
  grid_ui->tableView->setModel(model);
  grid_ui->tableView->setItemDelegate(new TulipItemDelegate);
  connect(grid_ui->tableView, SIGNAL(destroyed()), grid_ui->tableView->itemDelegate(), SLOT(deleteLater()));

  bool overviewVisible=true;

  if(data.exist("overviewVisible")) {
    data.get<bool>("overviewVisible",overviewVisible);
  }

  bool quickAccessBarVisible = true;

  if(data.exist("quickAccessBarVisible")) {
    data.get<bool>("quickAccessBarVisible",quickAccessBarVisible);
  }

  bool keepSPOV = false;
  data.get<bool>("keepScenePointOfViewOnSubgraphChanging", keepSPOV);
  getGlMainWidget()->setKeepScenePointOfViewOnSubgraphChanging(keepSPOV);

  createScene(graph(), data);
  registerTriggers();
  setOverviewVisible(overviewVisible);

  if(overviewItem())
    overviewItem()->setLayerVisible("Foreground",false);

  setQuickAccessBarVisible(quickAccessBarVisible);
}

void NodeLinkDiagramComponent::graphChanged(tlp::Graph* graph) {
  GlGraphComposite* composite =
    getGlMainWidget()->getScene()->getGlGraphComposite();
  Graph* oldGraph = composite ? composite->getGraph() : NULL;
  loadGraphOnScene(graph);
  registerTriggers();

  if (oldGraph == NULL || graph == NULL ||
      (oldGraph->getRoot() != graph->getRoot()) ||
      getGlMainWidget()->keepScenePointOfViewOnSubgraphChanging() == false)
    centerView();

  emit drawNeeded();
  drawOverview();
}

tlp::DataSet NodeLinkDiagramComponent::state() const {
  DataSet data=sceneData();
  data.set("overviewVisible",overviewVisible());
  data.set("quickAccessBarVisible",quickAccessBarVisible());
  data.set("keepScenePointOfViewOnSubgraphChanging",
           getGlMainWidget()->keepScenePointOfViewOnSubgraphChanging());
  return data;
}

//==================================================
void NodeLinkDiagramComponent::createScene(Graph *graph,DataSet dataSet) {
  GlScene *scene=getGlMainWidget()->getScene();
  scene->clearLayersList();

  std::string sceneInput="";

  if(dataSet.exist("scene")) {
    dataSet.get("scene",sceneInput);
  }

  if(sceneInput=="") {
    //Default scene
    GlLayer* layer=new GlLayer("Main");
    GlLayer *backgroundLayer=new GlLayer("Background");
    backgroundLayer->setVisible(false);
    GlLayer *foregroundLayer=new GlLayer("Foreground");
    foregroundLayer->setVisible(true);

    backgroundLayer->set2DMode();
    foregroundLayer->set2DMode();
    std::string dir=TulipBitmapDir;
    Gl2DRect *labri=new Gl2DRect(35.,5.,50.,50.,dir + "logolabri.jpg",false,false);
    labri->setStencil(1);
    labri->setVisible(false);
    foregroundLayer->addGlEntity(labri,"labrilogo");

    scene->addExistingLayer(backgroundLayer);
    scene->addExistingLayer(layer);
    scene->addExistingLayer(foregroundLayer);
    GlGraphComposite* graphComposite=new GlGraphComposite(graph,scene);
    scene->getLayer("Main")->addGlEntity(graphComposite,"graph");
    graphComposite->getRenderingParametersPointer()->setViewNodeLabel(true);
    graphComposite->getRenderingParametersPointer()->setEdgeColorInterpolate(false);
    graphComposite->getRenderingParametersPointer()->setNodesStencil(0x0002);
    graphComposite->getRenderingParametersPointer()->setNodesLabelStencil(0x0001);
    scene->centerScene();
  }
  else {
    size_t pos=sceneInput.find("TulipBitmapDir/");

    while(pos!=std::string::npos) {
      sceneInput.replace(pos,15,TulipBitmapDir);
      pos=sceneInput.find("TulipBitmapDir/");
    }

    pos=sceneInput.find("TulipLibDir/");

    while(pos!=std::string::npos) {
      sceneInput.replace(pos,12,TulipLibDir);
      pos=sceneInput.find("TulipLibDir/");
    }

    scene->setWithXML(sceneInput,graph);
  }

  if(dataSet.exist("Display")) {
    DataSet renderingParameters;
    dataSet.get("Display",renderingParameters);
    GlGraphRenderingParameters rp=scene->getGlGraphComposite()->getRenderingParameters();
    rp.setParameters(renderingParameters);
    scene->getGlGraphComposite()->setRenderingParameters(rp);
  }

  if(dataSet.exist("Hulls")) {
    useHulls(true);
    DataSet hullsSet;
    dataSet.get<DataSet>("Hulls", hullsSet);
    manager->setVisible(true);
    manager->setData(hullsSet);
  }

  getGlMainWidget()->emitGraphChanged();
}
//==================================================
DataSet NodeLinkDiagramComponent::sceneData() const {
  GlScene *scene=getGlMainWidget()->getScene();
  DataSet outDataSet;
  outDataSet.set("Display",scene->getGlGraphComposite()->getRenderingParameters().getParameters());
  std::string out;
  scene->getXML(out);
  size_t pos=out.find(TulipBitmapDir);

  while(pos!=std::string::npos) {
    out.replace(pos,TulipBitmapDir.size(),"TulipBitmapDir/");
    pos=out.find(TulipBitmapDir);
  }

  outDataSet.set("scene",out);

  if(_hasHulls && manager->isVisible()) {
    outDataSet.set("Hulls", manager->getData());
  }

  return outDataSet;
}
//==================================================
void NodeLinkDiagramComponent::loadGraphOnScene(Graph *graph) {
  GlScene *scene=getGlMainWidget()->getScene();

  if(!scene->getLayer("Main")) {
    createScene(graph,DataSet());
    return;
  }

  if(_hasHulls)
    manager->setGraph(graph);

  GlGraphComposite* oldGraphComposite=(GlGraphComposite *)(scene->getLayer("Main")->findGlEntity("graph"));

  if(!oldGraphComposite) {
    createScene(graph,DataSet());
    return;
  }

  GlGraphRenderingParameters param=oldGraphComposite->getRenderingParameters();
  GlMetaNodeRenderer *metaNodeRenderer=oldGraphComposite->getInputData()->getMetaNodeRenderer();
  // prevent deletion of MetaNodeRenderer when deleting oldGraphComposite
  oldGraphComposite->getInputData()->setMetaNodeRenderer(NULL,false);
  GlGraphComposite* graphComposite=new GlGraphComposite(graph);
  graphComposite->setRenderingParameters(param);

  metaNodeRenderer->setInputData(graphComposite->getInputData());

  graphComposite->getInputData()->setMetaNodeRenderer(metaNodeRenderer);

  if(oldGraphComposite->getInputData()->graph==graph) {
    delete graphComposite->getInputData()->getGlVertexArrayManager();
    graphComposite->getInputData()->setGlVertexArrayManager(oldGraphComposite->getInputData()->getGlVertexArrayManager());
    // prevent deletion of GlVertexArrayManager when deleting oldGraphComposite
    oldGraphComposite->getInputData()->setGlVertexArrayManager(NULL);
    graphComposite->getInputData()->getGlVertexArrayManager()->setInputData(graphComposite->getInputData());
  }

  scene->getLayer("Main")->addGlEntity(graphComposite,"graph");

  delete oldGraphComposite;
  getGlMainWidget()->emitGraphChanged();
}

void NodeLinkDiagramComponent::registerTriggers() {
  clearRedrawTriggers();

  if (graph() == NULL)
    return;

  addRedrawTrigger(getGlMainWidget()->getScene()->getGlGraphComposite()->getGraph());
  std::set<tlp::PropertyInterface*> properties = getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->properties();

  for(std::set<tlp::PropertyInterface*>::iterator it = properties.begin(); it != properties.end(); ++it) {
    addRedrawTrigger(*it);
  }
}

void NodeLinkDiagramComponent::setZOrdering(bool f) {
  getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->setElementZOrdered(f);
  centerView();
}

void NodeLinkDiagramComponent::showGridControl() {
  if (_gridOptions->exec() == QDialog::Rejected)
    return;

  updateGrid();
  emit drawNeeded();
}

void NodeLinkDiagramComponent::requestChangeGraph(Graph *graph) {
  this->loadGraphOnScene(graph);
  registerTriggers();
  emit graphSet(graph);
  centerView();
  draw();
}

void NodeLinkDiagramComponent::displayToolTips(bool display) {
  _tooltips = display;
}

void NodeLinkDiagramComponent::fillContextMenu(QMenu *menu, const QPointF &point) {
  GlMainView::fillContextMenu(menu,point);

  QAction *actionTooltips=menu->addAction("Tooltips");
  actionTooltips->setCheckable(true);
  actionTooltips->setChecked(_tooltips);
  connect(actionTooltips,SIGNAL(triggered(bool)),this,SLOT(displayToolTips(bool)));

  QAction* zOrdering = menu->addAction(trUtf8("Use Z ordering"));
  zOrdering->setCheckable(true);
  zOrdering->setChecked(getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->isElementZOrdered());
  connect(zOrdering,SIGNAL(triggered(bool)),this,SLOT(setZOrdering(bool)));
  menu->addAction(trUtf8("Grid display parameters"),this,SLOT(showGridControl()));

  //Check if a node/edge is under the mouse pointer
  bool result;
  SelectedEntity entity;
  result = getGlMainWidget()->pickNodesEdges(point.x(), point.y(), entity);

  if (result) {
    menu->addSeparator();
    isNode = entity.getEntityType() == SelectedEntity::NODE_SELECTED;
    itemId = entity.getComplexEntityId();

    menu->addAction((isNode ? trUtf8("Node #") : trUtf8("Edge #"))
                    + QString::number(itemId))->setEnabled(false);

    menu->addSeparator();

    menu->addAction(tr("Toggle selection"),this,SLOT(addRemoveItemToSelection()));
    menu->addAction(tr("Select"),this,SLOT(selectItem()));
    menu->addAction(tr("Delete"),this,SLOT(deleteItem()));

    QMenu* updateMenu = menu->addMenu("Edit");
    updateMenu->addAction("Color", this, SLOT(editColor()));
    updateMenu->addAction("Label", this, SLOT(editLabel()));
    updateMenu->addAction("Shape", this, SLOT(editShape()));
    updateMenu->addAction("Size", this, SLOT(editSize()));

    if (isNode) {
      Graph *metaGraph=graph()->getNodeMetaInfo(node(entity.getComplexEntityId()));

      if (metaGraph) {
        menu->addAction(tr("Go inside"),this,SLOT(goInsideItem()));
        menu->addAction(tr("Ungroup"),this,SLOT(ungroupItem()));
      }
    }
  }
}

void NodeLinkDiagramComponent::addRemoveItemToSelection() {
  BooleanProperty *elementSelected = graph()->getProperty<BooleanProperty>("viewSelection");
  graph()->push();

  // selection add/remove graph item
  if (isNode)
    elementSelected->setNodeValue(node(itemId), !elementSelected->getNodeValue(node(itemId)));
  else
    elementSelected->setEdgeValue(edge(itemId), !elementSelected->getEdgeValue(edge(itemId)));
}

void NodeLinkDiagramComponent::selectItem() {
  BooleanProperty *elementSelected = graph()->getProperty<BooleanProperty>("viewSelection");
  graph()->push();

  // empty selection
  elementSelected->setAllNodeValue(false);
  elementSelected->setAllEdgeValue(false);

  // selection add/remove graph item
  if (isNode)
    elementSelected->setNodeValue(node(itemId), true);
  else
    elementSelected->setEdgeValue(edge(itemId), true);
}

void NodeLinkDiagramComponent::deleteItem() {
  graph()->push();

  if (isNode)
    graph()->delNode(node(itemId));
  else
    graph()->delEdge(edge(itemId));
}

void NodeLinkDiagramComponent::editValue(PropertyInterface* pi) {
  TulipItemDelegate tid(getGlMainWidget());
  QVariant val =
    TulipItemDelegate::showEditorDialog(isNode ? NODE : EDGE,
                                        pi, graph(), &tid,
                                        getGlMainWidget(), itemId);

  // Check if edition has been cancelled
  if (!val.isValid())
    return;

  graph()->push();

  if (isNode)
    GraphModel::setNodeValue(itemId, pi, val);
  else
    GraphModel::setEdgeValue(itemId, pi, val);
}

void NodeLinkDiagramComponent::editColor() {
  editValue(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementColor());
}

void NodeLinkDiagramComponent::editLabel() {
  editValue(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementLabel());
}

void NodeLinkDiagramComponent::editShape() {
  editValue(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementShape());
}

void NodeLinkDiagramComponent::editSize() {
  editValue(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementSize());
}

const Camera& NodeLinkDiagramComponent::goInsideItem(node meta) {
  Graph *metaGraph=graph()->getNodeMetaInfo(meta);
  Size size=getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementSize()->getNodeValue(meta);
  Coord coord=getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementLayout()->getNodeValue(meta);
  BoundingBox bb;
  bb.expand(coord-size/2.f);
  bb.expand(coord+size/2.f);
  QtGlSceneZoomAndPanAnimator zoomAnPan(getGlMainWidget(), bb);
  zoomAnPan.animateZoomAndPan();
  loadGraphOnScene(metaGraph);
  registerTriggers();
  emit graphSet(metaGraph);
  centerView();
  draw();
  return getGlMainWidget()->getScene()->getLayer("Main")->getCamera();
}

void NodeLinkDiagramComponent::goInsideItem() {
  goInsideItem(node(itemId));
}

void NodeLinkDiagramComponent::ungroupItem() {
  graph()->push();
  graph()->openMetaNode(node(itemId));
}

void NodeLinkDiagramComponent::useHulls(bool hasHulls) {
  GlScene *scene=getGlMainWidget()->getScene();

  if(hasHulls == _hasHulls)
    return;

  _hasHulls = hasHulls;

  if(_hasHulls) {
    manager = new GlCompositeHierarchyManager(scene->getGlGraphComposite()->getInputData()->getGraph(),
        scene->getLayer("Main"),
        "Hulls",
        scene->getGlGraphComposite()->getInputData()->getElementLayout(),
        scene->getGlGraphComposite()->getInputData()->getElementSize(),
        scene->getGlGraphComposite()->getInputData()->getElementRotation());
    // Now we remove and add GlGraphComposite to be sure of the order (first Hulls and after GraphComposite)
    // This code doesn't affect the behavior of tulip but the tlp file is modified
    scene->getLayer("Main")->deleteGlEntity(scene->getGlGraphComposite());
    scene->getLayer("Main")->addGlEntity(scene->getGlGraphComposite(),"graph");
  }
}

bool NodeLinkDiagramComponent::hasHulls() const {
  return _hasHulls;
}

PLUGIN(NodeLinkDiagramComponent)
