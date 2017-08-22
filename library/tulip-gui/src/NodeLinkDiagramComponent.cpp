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
#include <tulip/NumericProperty.h>

using namespace tlp;
using namespace std;

const string NodeLinkDiagramComponent::viewName("Node Link Diagram view");

NodeLinkDiagramComponent::NodeLinkDiagramComponent(const tlp::PluginContext*): _grid(NULL), _gridOptions(NULL),_hasHulls(false),_tooltips(false),grid_ui(NULL) {
}

NodeLinkDiagramComponent::~NodeLinkDiagramComponent() {
  if(grid_ui)
    delete grid_ui->tableView->itemDelegate();

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
      QString ttip;
      node tmpNode=type.getNode();

      if(tmpNode.isValid()) {
        ttip = NodesGraphModel::getNodeTooltip(graph(), tmpNode);
        QToolTip::showText(he->globalPos(), ttip, gl);
        return true;
      }
      else {
        edge tmpEdge=type.getEdge();

        if(tmpEdge.isValid()) {
          ttip = EdgesGraphModel::getEdgeTooltip(graph(), tmpEdge);
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
  grid_ui->tableView->setItemDelegate(new TulipItemDelegate(grid_ui->tableView));

  setOverviewVisible(true);
  setQuickAccessBarVisible(true);
  GlMainView::setState(data);

  bool keepSPOV = false;
  data.get<bool>("keepScenePointOfViewOnSubgraphChanging", keepSPOV);
  getGlMainWidget()->setKeepScenePointOfViewOnSubgraphChanging(keepSPOV);

  data.get("Tooltips", _tooltips);

  createScene(graph(), data);
  registerTriggers();

  if(overviewItem())
    overviewItem()->setLayerVisible("Foreground",false);
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
  data.set("keepScenePointOfViewOnSubgraphChanging",
           getGlMainWidget()->keepScenePointOfViewOnSubgraphChanging());
  data.set("Tooltips", _tooltips);
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

    string s;

    if (renderingParameters.get("elementsOrderingPropertyName", s) &&
        !s.empty()) {
      rp.setElementOrderingProperty(dynamic_cast<tlp::NumericProperty*>(graph->getProperty(s)));
    }

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
  DataSet outDataSet = GlMainView::state();
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

  GlGraphComposite* oldGraphComposite=static_cast<GlGraphComposite *>(scene->getLayer("Main")->findGlEntity("graph"));

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

  //Check if a node/edge is under the mouse pointer
  bool result;
  SelectedEntity entity;
  result = getGlMainWidget()->pickNodesEdges(point.x(), point.y(), entity);

  if (result) {
    menu->addSeparator();
    isNode = entity.getEntityType() == SelectedEntity::NODE_SELECTED;
    itemId = entity.getComplexEntityId();
    QString sId = QString::number(itemId);

    menu->addAction((isNode ? trUtf8("Node #") : trUtf8("Edge #"))
                    + sId)->setEnabled(false);

    menu->addSeparator();

    QMenu* selectMenu = menu->addMenu("Select");
    QAction* action;

    if (isNode) {
      action = selectMenu->addAction(tr("node"),this,SLOT(selectItem()));
      action->setToolTip(QString("Select the node #") + sId);
      action = selectMenu->addAction(tr("predecessor nodes"),
                                     this,SLOT(selectInNodes()));
      action->setToolTip(QString("Select the predecessors of node #") + sId);
      action = selectMenu->addAction(tr("successor nodes"),
                                     this,SLOT(selectOutNodes()));
      action->setToolTip(QString("Select the sucessors of node #") + sId);
      action = selectMenu->addAction(tr("input edges"),
                                     this,SLOT(selectInEdges()));
      action->setToolTip(QString("Select the input edges of node #") + sId);
      action = selectMenu->addAction(tr("output edges"),
                                     this,SLOT(selectOutEdges()));
      action->setToolTip(QString("Select the output edges of node #") + sId);
      action = selectMenu->addAction(tr("node and all its neighbour nodes (including edges)"),this,SLOT(selectNodeAndAllNeighbourNodesAndEdges()));
      action->setToolTip(QString("Select the node #") + sId + " and its neighbours and adjacent edges");
    }
    else {
      action = selectMenu->addAction(tr("edge"),this,SLOT(selectItem()));
      action->setToolTip(QString("Select the edge #") + sId);
      action = selectMenu->addAction(tr("edge extremities"),
                                     this,SLOT(selectExtremities()));
      action->setToolTip(QString("Select the source and target nodes of edge #") + sId);
      action = selectMenu->addAction(tr("edge and its extremities"),
                                     this,SLOT(selectEdgeAndExtremities()));
      action->setToolTip(QString("Select the edge #") + sId + " and its source and target nodes");
    }

    QMenu* addToSelectionMenu = menu->addMenu("Add to selection");

    if (isNode) {
      action = addToSelectionMenu->addAction(tr("node"), this,
                                             SLOT(addItemToSelection()));
      action->setToolTip(QString("Add the node #") + sId + " to the current selection");
      action = addToSelectionMenu->addAction(tr("predecessor nodes"), this,
                                             SLOT(addInNodesToSelection()));
      action->setToolTip(QString("Add the predecessors of node #") + sId + " to the current selection");
      action = addToSelectionMenu->addAction(tr("successor nodes"), this,
                                             SLOT(addOutNodesToSelection()));
      action->setToolTip(QString("Add the sucessors of node #") + sId + " to the current selection");
      action = addToSelectionMenu->addAction(tr("input edges"), this,
                                             SLOT(addInEdgesToSelection()));
      action->setToolTip(QString("Add the input edges of node #") + sId + " to the current selection");
      action = addToSelectionMenu->addAction(tr("output edges"), this,
                                             SLOT(addOutEdgesToSelection()));
      action->setToolTip(QString("Add the output edges of node #") + sId + " to the current selection");
      action = addToSelectionMenu->addAction(tr("node and all its neighbour nodes (including edges)"),this,SLOT(addNodeAndAllNeighbourNodesAndEdgesToSelection()));
      action->setToolTip(QString("Add the node #") + sId + "and its neighbours and adjacent edges to the current selection");
    }
    else {
      action = addToSelectionMenu->addAction(tr("edge"), this,
                                             SLOT(addItemToSelection()));
      action->setToolTip(QString("Add the edge #") + sId + " to the current selection");
      action = addToSelectionMenu->addAction(tr("edge extremities"), this,
                                             SLOT(addExtremitiesToSelection()));
      action->setToolTip(QString("Add the source and target nodes of edge #") + sId + " to the current selection");
      action = addToSelectionMenu->addAction(tr("edge and its extremities"), this, SLOT(addEdgeAndExtremitiesToSelection()));
      action->setToolTip(QString("Add the edge #") + sId + "and its  source and target nodes to the current selection");
    }

    QMenu* removeFromSelectionMenu = menu->addMenu("Remove from selection");

    if (isNode) {
      action = removeFromSelectionMenu->addAction(tr("node"), this,
               SLOT(removeItemFromSelection()));
      action->setToolTip(QString("Remove the node #") + sId + " from the current selection");
      action = removeFromSelectionMenu->addAction(tr("predecessor nodes"), this,
               SLOT(removeInNodesFromSelection()));
      action->setToolTip(QString("Remove the predecessors of node #") + sId + " from the current selection");
      action = removeFromSelectionMenu->addAction(tr("successor nodes"), this,
               SLOT(removeOutNodesFromSelection()));
      action->setToolTip(QString("Remove the sucessors of node #") + sId + " from the current selection");
      action = removeFromSelectionMenu->addAction(tr("input edges"), this,
               SLOT(removeInEdgesFromSelection()));
      action->setToolTip(QString("Remove the input edges of node #") + sId + " from the current selection");
      action = removeFromSelectionMenu->addAction(tr("output edges"), this,
               SLOT(removeOutEdgesFromSelection()));
      action->setToolTip(QString("Remove the output edges of node #") + sId + " from the current selection");
      action = removeFromSelectionMenu->addAction(tr("node and all its neighbour nodes (including edges)"),this,SLOT(removeNodeAndAllNeighbourNodesAndEdgesFromSelection()));
    }
    else {
      action = removeFromSelectionMenu->addAction(tr("edge"), this,
               SLOT(removeItemFromSelection()));
      action = removeFromSelectionMenu->addAction(tr("edge extremities"), this,
               SLOT(removeExtremitiesFromSelection()));
      action = removeFromSelectionMenu->addAction(tr("edge and its extremities"),this,SLOT(removeEdgeAndExtremitiesFromSelection()));
      action->setToolTip(QString("Remove the node #") + sId + "and its neighbours and adjacent edges from the current selection");
    }

    QMenu* toggleMenu = menu->addMenu("Toggle selection of");

    if (isNode) {
      action = toggleMenu->addAction(tr("node"), this, SLOT(addRemoveItemToSelection()));
      action->setToolTip(QString("Invert the selection of the node #") + sId);
      action = toggleMenu->addAction(tr("predecessor nodes"), this,
                                     SLOT(addRemoveInNodesToSelection()));
      action->setToolTip(QString("Invert the selection of the predecessors of the node #") + sId);
      action = toggleMenu->addAction(tr("successor nodes"), this,SLOT(addRemoveOutNodesToSelection()));
      action->setToolTip(QString("Invert the selection of the successors of the node #") + sId);
      action = toggleMenu->addAction(tr("input edges"), this,
                                     SLOT(addRemoveInEdgesToSelection()));
      action->setToolTip(QString("Invert the selection of the input edges of the node #") + sId);
      action = toggleMenu->addAction(tr("output edges"), this,
                                     SLOT(addRemoveOutEdgesToSelection()));
      action->setToolTip(QString("Invert the selection of the output edges of the node #") + sId);
      action = toggleMenu->addAction(tr("node and all its neighbour nodes (including edges)"),this,SLOT(addRemoveNodeAndAllNeighbourNodesAndEdges()));
      action->setToolTip(QString("Invertthe selection of the node #") + sId + "and its neighbours and adjacent edges");
    }
    else {
      action = toggleMenu->addAction(tr("edge"), this,
                                     SLOT(addRemoveItemToSelection()));
      action->setToolTip(QString("Invert the selection of the edge #") + sId);
      action = toggleMenu->addAction(tr("edge extremities"), this,
                                     SLOT(addRemoveExtremitiesToSelection()));
      action->setToolTip(QString("Invert the selection of the source and target nodes of the edge #") + sId);
      action = toggleMenu->addAction(tr("edge and its extremities"),this,SLOT(addRemoveEdgeAndExtremitiesToSelection()));
      action->setToolTip(QString("Invert the selection of the edge #") + sId + " and its source and target nodes");
    }

    action = menu->addAction(tr("Delete"),this,SLOT(deleteItem()));
    action->setToolTip(QString("Delete the ") + (isNode ? "node #" : "edge #") + sId);

    QMenu* updateMenu = menu->addMenu("Edit");
    action = updateMenu->addAction("Color", this, SLOT(editColor()));
    action->setToolTip(QString("Display a dialog to update the color of the ") + (isNode ? "node #" : "edge #") + sId);
    action = updateMenu->addAction("Label", this, SLOT(editLabel()));
    action->setToolTip(QString("Display a dialog to update the label of the ") + (isNode ? "node #" : "edge #") + sId);
    action = updateMenu->addAction("Shape", this, SLOT(editShape()));
    action->setToolTip(QString("Display a dialog to update the shape of the ") + (isNode ? "node #" : "edge #") + sId);
    action = updateMenu->addAction("Size", this, SLOT(editSize()));
    action->setToolTip(QString("Display a dialog to update the size of the ") + (isNode ? "node #" : "edge #") + sId);

    if (isNode) {
      Graph *metaGraph=graph()->getNodeMetaInfo(node(entity.getComplexEntityId()));

      if (metaGraph) {
        action = menu->addAction(tr("Go inside"),this,SLOT(goInsideItem()));
        action->setToolTip(QString("Display the sub-graph represented by the meta-node #") + sId);
        menu->addAction(tr("Ungroup"),this,SLOT(ungroupItem()));
        action->setToolTip(QString("Replace the meta-node #") + sId + " by the sub-graph it represents");
      }
    }
  }
  else {
    GlMainView::fillContextMenu(menu,point);

    QAction* action = menu->addAction("Tooltips");
    action->setToolTip(QString("When moving the mouse pointer, a tooltip is displayed with some information about the graph element located under the pointer"));
    action->setCheckable(true);
    action->setChecked(_tooltips);
    connect(action,SIGNAL(triggered(bool)),this,SLOT(displayToolTips(bool)));

    action = menu->addAction(trUtf8("Use Z ordering"));
    action->setToolTip(QString("The graph elements are displayed according the ordering of their z coordinate"));
    action->setCheckable(true);
    action->setChecked(getGlMainWidget()->getScene()->getGlGraphComposite()->getRenderingParametersPointer()->isElementZOrdered());
    connect(action,SIGNAL(triggered(bool)),this,SLOT(setZOrdering(bool)));
    action = menu->addAction(trUtf8("Grid display parameters"),this,SLOT(showGridControl()));
    action->setToolTip(QString("Display the grid setup wizard"));
  }
}

void NodeLinkDiagramComponent::addRemoveItemToSelection(bool pushGraph, bool toggleSelection, bool selectValue, bool resetSelection) {
  BooleanProperty *elementSelected = graph()->getProperty<BooleanProperty>("viewSelection");

  if (pushGraph) {
    graph()->push();
  }

  if (resetSelection) {
    elementSelected->setAllNodeValue(false);
    elementSelected->setAllEdgeValue(false);
  }

  // selection add/remove graph item
  if (isNode)
    elementSelected->setNodeValue(node(itemId), toggleSelection ? !elementSelected->getNodeValue(node(itemId)) : selectValue);
  else
    elementSelected->setEdgeValue(edge(itemId), toggleSelection ? !elementSelected->getEdgeValue(edge(itemId)) : selectValue);
}

void NodeLinkDiagramComponent::addRemoveInNodesToSelection(bool pushGraph, bool toggleSelection, bool selectValue, bool resetSelection) {
  BooleanProperty *elementSelected = graph()->getProperty<BooleanProperty>("viewSelection");

  if (pushGraph) {
    graph()->push();
  }

  if (resetSelection) {
    elementSelected->setAllNodeValue(false);
    elementSelected->setAllEdgeValue(false);
  }

  node neigh;
  MutableContainer<bool> inNodes;
  forEach(neigh, graph()->getInNodes(node(itemId))) {
    if (inNodes.get(neigh.id) == false) {
      elementSelected->setNodeValue(neigh, toggleSelection ? !elementSelected->getNodeValue(neigh) : selectValue);
      inNodes.set(neigh.id, true);
    }
  }
}

void NodeLinkDiagramComponent::addRemoveOutNodesToSelection(bool pushGraph, bool toggleSelection, bool selectValue, bool resetSelection) {
  BooleanProperty *elementSelected = graph()->getProperty<BooleanProperty>("viewSelection");

  if (pushGraph) {
    graph()->push();
  }

  if (resetSelection) {
    elementSelected->setAllNodeValue(false);
    elementSelected->setAllEdgeValue(false);
  }

  node neigh;
  MutableContainer<bool> outNodes;
  forEach(neigh, graph()->getOutNodes(node(itemId))) {
    if (outNodes.get(neigh.id) == false) {
      elementSelected->setNodeValue(neigh, toggleSelection ? !elementSelected->getNodeValue(neigh) : selectValue);
      outNodes.set(neigh.id, true);
    }
  }
}

void NodeLinkDiagramComponent::addRemoveInEdgesToSelection(bool pushGraph, bool toggleSelection, bool selectValue, bool resetSelection) {
  BooleanProperty *elementSelected = graph()->getProperty<BooleanProperty>("viewSelection");

  if (pushGraph) {
    graph()->push();
  }

  if (resetSelection) {
    elementSelected->setAllNodeValue(false);
    elementSelected->setAllEdgeValue(false);
  }

  edge e;
  forEach(e, graph()->getInEdges(node(itemId))) {
    elementSelected->setEdgeValue(e, toggleSelection ? !elementSelected->getEdgeValue(e) : selectValue);
  }
}

void NodeLinkDiagramComponent::addRemoveOutEdgesToSelection(bool pushGraph, bool toggleSelection, bool selectValue, bool resetSelection) {
  BooleanProperty *elementSelected = graph()->getProperty<BooleanProperty>("viewSelection");

  if (pushGraph) {
    graph()->push();
  }

  if (resetSelection) {
    elementSelected->setAllNodeValue(false);
    elementSelected->setAllEdgeValue(false);
  }

  edge e;
  forEach(e, graph()->getOutEdges(node(itemId))) {
    elementSelected->setEdgeValue(e, toggleSelection ? !elementSelected->getEdgeValue(e) : selectValue);
  }
}

void NodeLinkDiagramComponent::addRemoveNodeAndAllNeighbourNodesAndEdges(bool toggleSelection, bool selectValue, bool resetSelection) {
  graph()->push();
  addRemoveItemToSelection(false, toggleSelection, selectValue, resetSelection);
  addRemoveInEdgesToSelection(false, toggleSelection, selectValue);
  addRemoveOutEdgesToSelection(false, toggleSelection, selectValue);
  addRemoveInNodesToSelection(false, toggleSelection, selectValue);
  addRemoveOutNodesToSelection(false, toggleSelection, selectValue);
}

void NodeLinkDiagramComponent::addRemoveExtremitiesToSelection(bool pushGraph, bool toggleSelection, bool selectValue, bool resetSelection) {
  BooleanProperty *elementSelected = graph()->getProperty<BooleanProperty>("viewSelection");

  if (pushGraph) {
    graph()->push();
  }

  if (resetSelection) {
    elementSelected->setAllNodeValue(false);
    elementSelected->setAllEdgeValue(false);
  }

  node src = graph()->source(edge(itemId));
  node tgt = graph()->target(edge(itemId));
  elementSelected->setNodeValue(src, toggleSelection ? !elementSelected->getNodeValue(src) : selectValue);

  if (src != tgt) {
    elementSelected->setNodeValue(tgt, toggleSelection ? !elementSelected->getNodeValue(tgt) : selectValue);
  }
}

void NodeLinkDiagramComponent::addRemoveEdgeAndExtremitiesToSelection(bool toggleSelection, bool selectValue, bool resetSelection) {
  graph()->push();
  addRemoveItemToSelection(false, toggleSelection, selectValue, resetSelection);
  addRemoveExtremitiesToSelection(false, toggleSelection, selectValue);
}

void NodeLinkDiagramComponent::selectItem() {
  addRemoveItemToSelection(true, false, true, true);
}

void NodeLinkDiagramComponent::selectInNodes(bool pushGraph) {
  addRemoveInNodesToSelection(pushGraph, false, true, true);
}

void NodeLinkDiagramComponent::selectOutNodes(bool pushGraph) {
  addRemoveOutNodesToSelection(pushGraph, false, true, true);
}

void NodeLinkDiagramComponent::selectInEdges(bool pushGraph) {
  addRemoveInEdgesToSelection(pushGraph, false, true, true);
}

void NodeLinkDiagramComponent::selectOutEdges(bool pushGraph) {
  addRemoveOutEdgesToSelection(pushGraph, false, true, true);
}

void NodeLinkDiagramComponent::selectNodeAndAllNeighbourNodesAndEdges() {
  addRemoveNodeAndAllNeighbourNodesAndEdges(false, true, true);
}

void NodeLinkDiagramComponent::selectExtremities(bool pushGraph) {
  addRemoveExtremitiesToSelection(pushGraph, false, true, true);
}

void NodeLinkDiagramComponent::selectEdgeAndExtremities() {
  addRemoveEdgeAndExtremitiesToSelection(false, true, true);
}

void NodeLinkDiagramComponent::addItemToSelection() {
  addRemoveItemToSelection(true, false, true);
}

void NodeLinkDiagramComponent::addInNodesToSelection(bool pushGraph) {
  addRemoveInNodesToSelection(pushGraph, false, true);
}

void NodeLinkDiagramComponent::addOutNodesToSelection(bool pushGraph) {
  addRemoveOutNodesToSelection(pushGraph, false, true);
}

void NodeLinkDiagramComponent::addInEdgesToSelection(bool pushGraph) {
  addRemoveInEdgesToSelection(pushGraph, false, true);
}

void NodeLinkDiagramComponent::addOutEdgesToSelection(bool pushGraph) {
  addRemoveOutEdgesToSelection(pushGraph, false, true);
}

void NodeLinkDiagramComponent::addNodeAndAllNeighbourNodesAndEdgesToSelection() {
  addRemoveNodeAndAllNeighbourNodesAndEdges(false, true);
}

void NodeLinkDiagramComponent::addExtremitiesToSelection(bool pushGraph) {
  addRemoveExtremitiesToSelection(pushGraph, false, true);
}

void NodeLinkDiagramComponent::addEdgeAndExtremitiesToSelection() {
  addRemoveEdgeAndExtremitiesToSelection(false, true);
}

void NodeLinkDiagramComponent::removeItemFromSelection() {
  addRemoveItemToSelection(true, false, false);
}

void NodeLinkDiagramComponent::removeInNodesFromSelection(bool pushGraph) {
  addRemoveInNodesToSelection(pushGraph, false, false);
}

void NodeLinkDiagramComponent::removeOutNodesFromSelection(bool pushGraph) {
  addRemoveOutNodesToSelection(pushGraph, false, false);
}

void NodeLinkDiagramComponent::removeInEdgesFromSelection(bool pushGraph) {
  addRemoveInEdgesToSelection(pushGraph, false, false);
}

void NodeLinkDiagramComponent::removeOutEdgesFromSelection(bool pushGraph) {
  addRemoveOutEdgesToSelection(pushGraph, false, false);
}

void NodeLinkDiagramComponent::removeNodeAndAllNeighbourNodesAndEdgesFromSelection() {
  addRemoveNodeAndAllNeighbourNodesAndEdges(false, false);
}

void NodeLinkDiagramComponent::removeExtremitiesFromSelection(bool pushGraph) {
  addRemoveExtremitiesToSelection(pushGraph, false, false);
}

void NodeLinkDiagramComponent::removeEdgeAndExtremitiesFromSelection() {
  addRemoveEdgeAndExtremitiesToSelection(false, false);
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

  graph()->popIfNoUpdates();
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
