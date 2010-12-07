/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/NodeLinkDiagramComponent.h"

#include <QtGui/QMenuBar>
#include <QtGui/QImageWriter>
#include <QtGui/QStatusBar>
#include <QtGui/QInputDialog>
#include <QtGui/QMouseEvent>
#include <QtGui/QToolTip>

#include <tulip/ExtendedClusterOperation.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StableIterator.h>
#include <tulip/StringProperty.h>

#include <tulip/GlMetaNodeTrueRenderer.h>

#include "tulip/GWOverviewWidget.h"
#include "tulip/RenderingParametersDialog.h"
#include "tulip/LayerManagerWidget.h"
#include "tulip/AugmentedDisplayDialog.h"
#include "tulip/GridOptionsWidget.h"
#include "tulip/InteractorManager.h"
#include "tulip/QtMetaNodeRenderer.h"
#include "tulip/TlpQtTools.h"
#include "tulip/BaseGraphicsViewComponentMacro.h"

namespace tlp {

  VIEWPLUGIN(NodeLinkDiagramComponent, "Node Link Diagram view", "Tulip Team", "16/04/2008", "Node link diagram", "1.0");
  //GRAPHICSVIEWEXTENSION(NodeLinkDiagramComponentGraphicsView, "Node Link Diagram graphics view","Node Link Diagram view", "Tulip Team", "16/04/2008", "Node link diagram GV", "1.0");

  //==================================================
  NodeLinkDiagramComponent::NodeLinkDiagramComponent():GlMainView(),qtMetaNode(true),currentMetaNodeRenderer(NULL) {
  }
  //==================================================
  NodeLinkDiagramComponent::~NodeLinkDiagramComponent() {
  }
  //==================================================
  QWidget *NodeLinkDiagramComponent::construct(QWidget *parent) {
  	QWidget *widget=GlMainView::construct(parent);

  	//View Menu
  	viewMenu=new QMenu("View");
  	viewMenu->addAction("&Redraw View", this, SLOT(draw()), tr("Ctrl+Shift+R"));
  	viewMenu->addAction("&Center View", this, SLOT(centerView()), tr("Ctrl+Shift+C"));
  	//Dialogs Menu
  	renderingParametersDialog=new RenderingParametersDialog();
  	connect(renderingParametersDialog,SIGNAL(viewNeedDraw()),this, SLOT(drawAfterRenderingParametersChange()));
  	layerManagerWidget=new LayerManagerWidget();
  	augmentedDisplayDialogAction = dialogMenu->addAction("Augmented Display");
  	//Options Menu
  	optionsMenu=new QMenu("Options");
  	actionTooltips=optionsMenu->addAction("Tooltips");
  	actionTooltips->setCheckable(true);
  	actionTooltips->setChecked(false);
  	actionsGridOptions=optionsMenu->addAction("Grid");
  	connect(actionsGridOptions, SIGNAL(triggered()), SLOT(gridOptions()));
  	gridOptionsWidget=NULL;
  	actionZOrderingOptions=optionsMenu->addAction("Z Ordering");
  	actionZOrderingOptions->setCheckable(true);
  	actionZOrderingOptions->setChecked(true);
  	actionAntialiasingOptions=optionsMenu->addAction("Antialiasing");
  	actionAntialiasingOptions->setCheckable(true);
  	actionAntialiasingOptions->setChecked(true);

  	return widget;
  }
  //==================================================
  // Accessor and set
  //==================================================
  void NodeLinkDiagramComponent::setData(Graph *graph,DataSet dataSet) {
    DataSet data;
    if(dataSet.exist("data")){
      dataSet.get("data",data);
    }else{
      data=dataSet;
    }
    mainWidget->setData(graph,data);

		mainWidget->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(new GlMetaNodeTrueRenderer(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()));
		
		getGlMainWidget()->useHulls(true);
		
    renderingParametersDialog->setGlMainView(this);
    layerManagerWidget->setGlMainView(this);
    overviewWidget->setObservedView(mainWidget,mainWidget->getScene()->getGlGraphComposite());
    draw();
  }
  //==================================================
  void NodeLinkDiagramComponent::getData(Graph **graph,DataSet *dataSet) {
    dataSet->set<DataSet>("data",mainWidget->getData());
    *graph=mainWidget->getGraph();
  }
  //==================================================
  void NodeLinkDiagramComponent::setGraph(Graph *graph,bool initView) {
    mainWidget->setGraph(graph);
    overviewWidget->setObservedView(mainWidget,mainWidget->getScene()->getGlGraphComposite());

    mainWidget->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(new GlMetaNodeTrueRenderer(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()));
    if(initView)
      init();
  }
  //==================================================
  void NodeLinkDiagramComponent::setGraph(Graph *graph) {
    if(mainWidget->getScene()->getGlGraphComposite()!=NULL && mainWidget->getGraph()==graph)
      setGraph(graph,false);
    else
      setGraph(graph,true);
  }
  //==================================================
  std::list<std::pair<QWidget *,std::string> > NodeLinkDiagramComponent::getConfigurationWidget() {
    std::list<std::pair<QWidget *,std::string> > widgetList;
    widgetList.push_back(std::pair<QWidget*,std::string>(renderingParametersDialog,"Rendering Parameters"));
    widgetList.push_back(std::pair<QWidget*,std::string>(layerManagerWidget,"Layer Manager"));
    return widgetList;
  }

  void NodeLinkDiagramComponent::specificEventFilter(QObject *,QEvent *event) {
    if (event->type() == QEvent::KeyPress){
      QKeyEvent *keyEvent=(QKeyEvent*)event;
      if((keyEvent->key()==Qt::Key_R) && (keyEvent->modifiers() & Qt::ControlModifier)!=0 && (keyEvent->modifiers() & Qt::ShiftModifier)!=0)
        draw();
      if((keyEvent->key()==Qt::Key_C) && (keyEvent->modifiers() & Qt::ControlModifier)!=0 && (keyEvent->modifiers() & Qt::ShiftModifier)!=0)
        centerView();
    }
    if (event->type() == QEvent::ToolTip && actionTooltips->isChecked()) {
      node tmpNode;
      edge tmpEdge;
      ElementType type;
      QString tmp;
      QHelpEvent *he = static_cast<QHelpEvent *>(event);
      QRect rect=mainWidget->frameGeometry();
      if (mainWidget->doSelect(he->pos().x()-rect.x(), he->pos().y()-rect.y(), type, tmpNode, tmpEdge)) {
        // try to show the viewLabel if any
        StringProperty *labels = mainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph()->getProperty<StringProperty>("viewLabel");
        std::string label;
        QString ttip;
        switch(type) {
        case NODE:
          label = labels->getNodeValue(tmpNode);
          if (!label.empty())
            ttip += (label + " (").c_str();
          ttip += QString("node: ")+ tmp.setNum(tmpNode.id);
          if (!label.empty())
            ttip += ")";
          QToolTip::showText(he->globalPos(), ttip);
          break;
        case EDGE:
          label = labels->getEdgeValue(tmpEdge);
          if (!label.empty())
            ttip += (label + "(").c_str();
          ttip += QString("edge: ")+ tmp.setNum(tmpEdge.id);
          if (!label.empty())
            ttip += ")";
          QToolTip::showText(he->globalPos(), ttip);
          break;
        }
      }
    }
  }

  void NodeLinkDiagramComponent::buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu) {
  	contextMenu->addMenu(viewMenu);
  	contextMenu->addMenu(optionsMenu);

  	GlMainView::buildContextMenu(object,event,contextMenu);

    //set default value with rendering parameters values
    GlGraphRenderingParameters param=mainWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
    actionZOrderingOptions->setChecked(param.isElementZOrdered());
    actionAntialiasingOptions->setChecked(param.isAntialiased());

    //Check if a node/edge is under the mouse pointer
  	node tmpNode;
  	edge tmpEdge;
  	Graph *graph=mainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  	bool result;
  	ElementType type;
  	// look if the mouse pointer is over a node or edge
  	QRect rect=mainWidget->frameGeometry();
  	result = mainWidget->doSelect(event->x()-rect.x(), event->y()-rect.y(), type, tmpNode, tmpEdge);
  	if (!result)
  		return;
  	contextMenu->addSeparator();
  	contextMenu->addSeparator();
  	// Display a context menu
  	isNode = type == NODE;
  	itemId = isNode ? tmpNode.id : tmpEdge.id;
  	std::stringstream sstr;
  	sstr << (isNode ? "Node " : "Edge ") << itemId;
  	contextMenu->addAction(tr(sstr.str().c_str()))->setEnabled(false);
  	contextMenu->addSeparator();
  	addRemoveAction = contextMenu->addAction(tr("Add to/Remove from selection"));
  	selectAction = contextMenu->addAction(tr("Select"));
  	deleteAction = contextMenu->addAction(tr("Delete"));
  	contextMenu->addSeparator();
  	goAction = NULL;
  	ungroupAction = NULL;
  	if (isNode) {
  	  Graph *metaGraph=graph->getNodeMetaInfo(tmpNode);
  		if (metaGraph) {
  			goAction = contextMenu->addAction(tr("Go inside"));
  			ungroupAction = contextMenu->addAction(tr("Ungroup"));
  		}
  	}
  	if (goAction != NULL)
  		contextMenu->addSeparator();
  	propAction = contextMenu->addAction(tr("Properties"));
  }

  void NodeLinkDiagramComponent::computeContextMenuAction(QAction *action) {
    Graph *graph=mainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();

    Observable::holdObservers();
    if (action == deleteAction) { // Delete
      graph->push();
      // delete graph item
      if (isNode)
        graph->delNode(node(itemId));
      else
        graph->delEdge(edge(itemId));
    } else if (action == propAction) { // Properties
      emit elementSelected(itemId, isNode);
    }else if (action == goAction) { // Go inside
      Graph *metaGraph=graph->getNodeMetaInfo(node(itemId));
      emit requestChangeGraph(this,metaGraph);
    } else if (action == ungroupAction) { // Ungroup
      graph->push();
      graph->openMetaNode(node(itemId));
    } else if(action == selectAction || action == addRemoveAction) {
      BooleanProperty *elementSelected = graph->getProperty<BooleanProperty>("viewSelection");
      graph->push();
      if (action == selectAction) { // Select
        // empty selection
        elementSelected->setAllNodeValue(false);
        elementSelected->setAllEdgeValue(false);
      }
      // selection add/remove graph item
      if (isNode)
        elementSelected->setNodeValue(node(itemId), !elementSelected->getNodeValue(node(itemId)));
      else
        elementSelected->setEdgeValue(edge(itemId), !elementSelected->getEdgeValue(edge(itemId)));
    }
    GlGraphRenderingParameters param=mainWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
    param.setElementZOrdered(actionZOrderingOptions->isChecked());
    param.setAntialiasing(actionAntialiasingOptions->isChecked());
    mainWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);
    Observable::unholdObservers();
  }
  void NodeLinkDiagramComponent::checkAlgorithmResult() {
    if(!mainWidget->getScene()->getGlGraphComposite())
      return;
    Graph *graph=mainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();

    if(graph->attributeExist("NodeLinkDiagramComponent")){
      DataSet nodeLinkDiagramComponentDataSet;
      graph->getAttribute("NodeLinkDiagramComponent",nodeLinkDiagramComponentDataSet);

      //remove old info
      std::list<std::string> toRemove;
      for(std::map<std::string,DataSet>::iterator it=algorithmInfoDataSet.begin();it!=algorithmInfoDataSet.end();++it){
        if(!nodeLinkDiagramComponentDataSet.exist((*it).first)){
          DataSet layerAndCompositeDataSet=(*it).second;
          toRemove.push_back((*it).first);
          std::string layerName;
          long compositeLong;
          layerAndCompositeDataSet.get("layer",layerName);
          layerAndCompositeDataSet.get("composite",compositeLong);
          mainWidget->getScene()->getLayer(layerName)->deleteGlEntity((GlSimpleEntity*)compositeLong);
        }
      }
      for(std::list<std::string>::iterator it=toRemove.begin();it!=toRemove.end();++it)
        algorithmInfoDataSet.erase(*it);

      Iterator< std::pair<std::string, DataType*> > *infoDataSetIt=nodeLinkDiagramComponentDataSet.getValues();
      while(infoDataSetIt->hasNext()) {
        std::pair<std::string, DataType*> infoData;
        infoData = infoDataSetIt->next();

        DataSet newLayerAndCompositeDataSet=*((DataSet*)(infoData.second->value));
        std::string newLayerName;
        long newCompositeLong;
        newLayerAndCompositeDataSet.get("layer",newLayerName);
        newLayerAndCompositeDataSet.get("composite",newCompositeLong);

        std::map<std::string,DataSet>::iterator it=algorithmInfoDataSet.find(infoData.first);
        if(it==algorithmInfoDataSet.end()){
          //add new info
          algorithmInfoDataSet[infoData.first]=newLayerAndCompositeDataSet;

          GlComposite *composite;
          composite=(GlComposite*)newCompositeLong;
          mainWidget->getScene()->getLayer(newLayerName)->addGlEntity(composite,infoData.first);
        }else{
          //check integrity
          DataSet oldLayerAndCompositeDataSet=(*it).second;
          std::string oldLayerName;
          long oldCompositeLong;
          oldLayerAndCompositeDataSet.get("layer",oldLayerName);
          oldLayerAndCompositeDataSet.get("composite",oldCompositeLong);

          if(oldCompositeLong!=newCompositeLong){
            mainWidget->getScene()->getLayer(oldLayerName)->deleteGlEntity((GlSimpleEntity*)oldCompositeLong);
            algorithmInfoDataSet.erase(it);

            algorithmInfoDataSet[infoData.first]=newLayerAndCompositeDataSet;
            GlComposite *composite;
            composite=(GlComposite*)newCompositeLong;
            mainWidget->getScene()->getLayer(newLayerName)->addGlEntity(composite,infoData.first);
          }
        }
      }
      for(std::map<std::string,DataSet>::iterator it=algorithmInfoDataSet.begin();it!=algorithmInfoDataSet.end();++it){
        DataSet oldLayerAndCompositeDataSet=(*it).second;
        std::string oldLayerName;
        long oldCompositeLong;
        oldLayerAndCompositeDataSet.get("layer",oldLayerName);
        oldLayerAndCompositeDataSet.get("composite",oldCompositeLong);


      }
      /*Iterator< std::pair<std::string, DataType*> > *infoDataSetIt=nodeLinkDiagramComponentDataSet.getValues();
      while(infoDataSetIt->hasNext()) {
        pair<string, DataType*> infoData;
        infoData = infoDataSetIt->next();

        map<string,DataSet>::iterator it=algorithmInfoDataSet.find(infoData.first);
        if(it==algorithmInfoDataSet.end()){
          DataSet layerAndCompositeDataSet=*((DataSet*)(infoData.second->value));
          algorithmInfoDataSet[infoData.first]=layerAndCompositeDataSet;
          string layerName;
          long compositeLong;
          GlComposite *composite;
          layerAndCompositeDataSet.get("layer",layerName);
          layerAndCompositeDataSet.get("composite",compositeLong);
          composite=(GlComposite*)compositeLong;

          mainWidget->getScene()->getLayer(layerName)->addGlEntity(composite,infoData.first);
        }else{
          if(infoData.second->typeName==typeid(string).name()){
            if(*((string*)infoData.second->value) =="toRemove"){
              DataSet layerAndCompositeDataSet=algorithmInfoDataSet[infoData.first];
              algorithmInfoDataSet.erase(infoData.first);
              string layerName;
              long compositeLong;
              layerAndCompositeDataSet.get("layer",layerName);
              layerAndCompositeDataSet.get("composite",compositeLong);
              mainWidget->getScene()->getLayer(layerName)->deleteGlEntity((GlSimpleEntity*)compositeLong);
              toRemove.push_back(infoData.first);
            }
          }
        }
      }

      for(vector<string>::iterator it=toRemove.begin();it!=toRemove.end();++it){
        nodeLinkDiagramComponentDataSet.remove(*it);
      }
      graph->setAttribute("NodeLinkDiagramComponent",nodeLinkDiagramComponentDataSet);*/
    }else{
      for(std::map<std::string,DataSet>::iterator it=algorithmInfoDataSet.begin();it!=algorithmInfoDataSet.end();++it){
        DataSet layerAndCompositeDataSet=(*it).second;
        std::string layerName;
        long compositeLong;
        layerAndCompositeDataSet.get("layer",layerName);
        layerAndCompositeDataSet.get("composite",compositeLong);
        mainWidget->getScene()->getLayer(layerName)->deleteGlEntity((GlSimpleEntity*)compositeLong);
      }
      algorithmInfoDataSet.clear();
    }
  }
  //==================================================
  // GUI functions
  //==================================================
  void NodeLinkDiagramComponent::drawAfterRenderingParametersChange(){
    draw();
  }
  //==================================================
  void NodeLinkDiagramComponent::draw() {
    checkAlgorithmResult();

    GlMainView::draw();
    if (gridOptionsWidget !=0)
      gridOptionsWidget->validateGrid();
    layerManagerWidget->setGlMainView(this);
    //overviewWidget->updateView();
  }
  //==================================================
  void NodeLinkDiagramComponent::refresh() {
    draw();
  }
  //==================================================
  void NodeLinkDiagramComponent::init() {
    for(std::map<std::string,DataSet>::iterator it=algorithmInfoDataSet.begin();it!=algorithmInfoDataSet.end();++it){
      std::string layerName;
      (*it).second.get("layer",layerName);
      mainWidget->getScene()->getLayer(layerName)->deleteGlEntity((*it).first);
    }
    algorithmInfoDataSet.clear();
    centerView();
  }
  //==================================================
  void NodeLinkDiagramComponent::centerView() {
    mainWidget->getScene()->centerScene();
    overviewWidget->setObservedView(mainWidget,mainWidget->getScene()->getGlGraphComposite());
    draw();
  }
  //==================================================
  void NodeLinkDiagramComponent::showDialog(QAction* action){
    GlMainView::showDialog(action);

    std::string name(action->text().toStdString());

    if(name =="Augmented Display") {
      Graph *graph=mainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      AugmentedDisplayDialog dialog(widget,graph,"NodeLinkDiagramComponent");
      if(dialog.exec()==QDialog::Accepted){
        std::vector<std::string> toRemove;
        dialog.getRemovedList(toRemove);
        DataSet nldcDataSet;
        graph->getAttribute("NodeLinkDiagramComponent",nldcDataSet);
        for(std::vector<std::string>::iterator it=toRemove.begin();it!=toRemove.end();++it){
          DataSet data=algorithmInfoDataSet[*it];
          std::string layerName;
          data.get("layer",layerName);
          mainWidget->getScene()->getLayer(layerName)->deleteGlEntity(*it);
          algorithmInfoDataSet.erase(*it);
          nldcDataSet.remove(*it);
        }
        graph->setAttribute("NodeLinkDiagramComponent",nldcDataSet);
        mainWidget->draw();
      }
    }
  }
  //==================================================
  void NodeLinkDiagramComponent::gridOptions() {
    if (gridOptionsWidget == 0)
      gridOptionsWidget = new GridOptionsWidget(getWidget());
    gridOptionsWidget->setCurrentMainWidget(mainWidget);
    gridOptionsWidget->setCurrentRenderingParametersDialog(renderingParametersDialog);
    gridOptionsWidget->show();
  }
  //==================================================
  // GlSceneObserver interface
  void NodeLinkDiagramComponent::addLayer(GlScene*, const std::string&, GlLayer*) {
    //renderingParametersDialog->addLayer(scene,name,layer);
  }
  void NodeLinkDiagramComponent::modifyLayer(GlScene*, const std::string&, GlLayer*){
    //renderingParametersDialog->updateLayer(name,layer);
  }
}

