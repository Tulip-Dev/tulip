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

using namespace std;

namespace tlp {

  VIEWPLUGIN(NodeLinkDiagramComponent, "Node Link Diagram view", "Tulip Team", "16/04/2008", "Node link diagram", "1.0");

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
  	dialogMenu=new QMenu("Dialog");
  	connect(dialogMenu, SIGNAL(triggered(QAction*)), SLOT(showDialog(QAction*)));
  	overviewAction=dialogMenu->addAction("3D &Overview");
  	overviewAction->setCheckable(true);
  	overviewAction->setChecked(true);
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
  	actionZOrderingOptions->setChecked(false);
  	actionAntialiasingOptions=optionsMenu->addAction("Antialiasing");
  	actionAntialiasingOptions->setCheckable(true);
  	actionAntialiasingOptions->setChecked(true);
  	actionTrueMetaNodeOptions=optionsMenu->addAction("Textured meta node");
  	actionTrueMetaNodeOptions->setCheckable(true);
  	if(canUseQGLPixelBuffer()){
  	  actionTrueMetaNodeOptions->setChecked(true);
  	}else{
  	  actionTrueMetaNodeOptions->setEnabled(false);
  	  actionTrueMetaNodeOptions->setChecked(false);
  	  qtMetaNode=false;
  	}

  	//Export Menu
  	exportImageMenu=new QMenu("&Save Picture as ");
  	// Tulip known formats (see GlGraph)
  	// formats are sorted, "~" is just an end marker
  	char *tlpFormats[] = {"EPS", "SVG", "~"};
  	unsigned int i = 0;
  	//Image PopuMenu
  	// int Qt 4, output formats are not yet sorted and uppercased
  	list<QString> formats;
  	// first add Tulip known formats
  	while (strcmp(tlpFormats[i], "~") != 0)
  		formats.push_back(tlpFormats[i++]);
  	// uppercase and insert all Qt formats
  	foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
  		char* tmp = format.data();
  		for (int i = strlen(tmp) - 1; i >= 0; --i)
  			tmp[i] = toupper(tmp[i]);
  		formats.push_back(QString(tmp));
  	}
  	// sort before inserting in exportImageMenu
  	formats.sort();
  	foreach(QString str, formats)
  	exportImageMenu->addAction(str);

  	connect(exportImageMenu, SIGNAL(triggered(QAction*)), SLOT(exportImage(QAction*)));
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
    if(currentMetaNodeRenderer)
      delete currentMetaNodeRenderer;
    if(qtMetaNode)
      currentMetaNodeRenderer = new QtMetaNodeRenderer(NULL,getGlMainWidget(),getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData());
    else
      currentMetaNodeRenderer = new GlMetaNodeTrueRenderer(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData());
    mainWidget->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(currentMetaNodeRenderer);
    renderingParametersDialog->setGlMainView(this);
    layerManagerWidget->setGlMainView(this);
    overviewWidget->setObservedView(mainWidget,mainWidget->getScene()->getGlGraphComposite());
    init();
  }
  //==================================================
  void NodeLinkDiagramComponent::getData(Graph **graph,DataSet *dataSet) {
    dataSet->set<DataSet>("data",mainWidget->getData());
    *graph=mainWidget->getGraph();
  }
  //==================================================
  void NodeLinkDiagramComponent::setGraph(Graph *graph) {
    mainWidget->setGraph(graph);
    overviewWidget->setObservedView(mainWidget,mainWidget->getScene()->getGlGraphComposite());
    if(currentMetaNodeRenderer)
      delete currentMetaNodeRenderer;
    if(qtMetaNode)
      currentMetaNodeRenderer = new QtMetaNodeRenderer(NULL,getGlMainWidget(),getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData());
    else
      currentMetaNodeRenderer = new GlMetaNodeTrueRenderer(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData());

    mainWidget->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(currentMetaNodeRenderer);
    init();
  }
  //==================================================
  list<pair<QWidget *,string> > NodeLinkDiagramComponent::getConfigurationWidget() {
    list<pair<QWidget *,string> > widgetList;
    widgetList.push_back(pair<QWidget*,string>(renderingParametersDialog,"Rendering Parameters"));
    widgetList.push_back(pair<QWidget*,string>(layerManagerWidget,"Layer Manager"));
    return widgetList;
  }

  void NodeLinkDiagramComponent::specificEventFilter(QObject *object,QEvent *event) {
    if (event->type() == QEvent::KeyPress){
      QKeyEvent *keyEvent=(QKeyEvent*)event;
      if((keyEvent->key()==Qt::Key_R) && (keyEvent->modifiers() & Qt::ControlModifier)!=0 && (keyEvent->modifiers() & Qt::ShiftModifier)!=0)
        draw();
      if((keyEvent->key()==Qt::Key_C) && (keyEvent->modifiers() & Qt::ControlModifier)!=0 && (keyEvent->modifiers() & Qt::ShiftModifier)!=0)
        centerView();
    }
    if (object->inherits("tlp::GlMainView") &&
        event->type() == QEvent::ToolTip && actionTooltips->isChecked()) {
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
  	contextMenu->addMenu(dialogMenu);
  	contextMenu->addMenu(optionsMenu);
  	contextMenu->addMenu(exportImageMenu);

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
  	stringstream sstr;
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
      tlp::openMetaNode(graph, node(itemId));
    } else if(action == selectAction || action == addRemoveAction) {
      BooleanProperty *elementSelected = graph->getProperty<BooleanProperty>("viewSelection");
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
    if(!actionTrueMetaNodeOptions->isChecked() && qtMetaNode){
      qtMetaNode=false;
      if(currentMetaNodeRenderer)
        delete currentMetaNodeRenderer;
      currentMetaNodeRenderer = new GlMetaNodeTrueRenderer(getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData());
      mainWidget->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(currentMetaNodeRenderer);
      draw();
    }
    if(actionTrueMetaNodeOptions->isChecked() && !qtMetaNode){
      qtMetaNode=true;
      if(currentMetaNodeRenderer)
        delete currentMetaNodeRenderer;
      currentMetaNodeRenderer = new QtMetaNodeRenderer(NULL,getGlMainWidget(),getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData());
      mainWidget->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(currentMetaNodeRenderer);
      draw();
    }
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
      list<string> toRemove;
      for(map<string,DataSet>::iterator it=algorithmInfoDataSet.begin();it!=algorithmInfoDataSet.end();++it){
        if(!nodeLinkDiagramComponentDataSet.exist((*it).first)){
          DataSet layerAndCompositeDataSet=(*it).second;
          toRemove.push_back((*it).first);
          string layerName;
          long compositeLong;
          layerAndCompositeDataSet.get("layer",layerName);
          layerAndCompositeDataSet.get("composite",compositeLong);
          mainWidget->getScene()->getLayer(layerName)->deleteGlEntity((GlSimpleEntity*)compositeLong);
        }
      }
      for(list<string>::iterator it=toRemove.begin();it!=toRemove.end();++it)
        algorithmInfoDataSet.erase(*it);

      Iterator< std::pair<std::string, DataType*> > *infoDataSetIt=nodeLinkDiagramComponentDataSet.getValues();
      while(infoDataSetIt->hasNext()) {
        pair<string, DataType*> infoData;
        infoData = infoDataSetIt->next();

        DataSet newLayerAndCompositeDataSet=*((DataSet*)(infoData.second->value));
        string newLayerName;
        long newCompositeLong;
        newLayerAndCompositeDataSet.get("layer",newLayerName);
        newLayerAndCompositeDataSet.get("composite",newCompositeLong);

        map<string,DataSet>::iterator it=algorithmInfoDataSet.find(infoData.first);
        if(it==algorithmInfoDataSet.end()){
          //add new info
          algorithmInfoDataSet[infoData.first]=newLayerAndCompositeDataSet;

          GlComposite *composite;
          composite=(GlComposite*)newCompositeLong;
          mainWidget->getScene()->getLayer(newLayerName)->addGlEntity(composite,infoData.first);
        }else{
          //check integrity
          DataSet oldLayerAndCompositeDataSet=(*it).second;
          string oldLayerName;
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
      for(map<string,DataSet>::iterator it=algorithmInfoDataSet.begin();it!=algorithmInfoDataSet.end();++it){
        DataSet oldLayerAndCompositeDataSet=(*it).second;
        string oldLayerName;
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
      for(map<string,DataSet>::iterator it=algorithmInfoDataSet.begin();it!=algorithmInfoDataSet.end();++it){
        DataSet layerAndCompositeDataSet=(*it).second;
        string layerName;
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
    if(currentMetaNodeRenderer) {
      delete currentMetaNodeRenderer;
      currentMetaNodeRenderer = NULL;
    }
    if(qtMetaNode){
      currentMetaNodeRenderer = new QtMetaNodeRenderer(NULL,getGlMainWidget(),getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData());
      mainWidget->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(currentMetaNodeRenderer);
    }
    draw();
  }
  //==================================================
  void NodeLinkDiagramComponent::draw() {
    checkAlgorithmResult();

    GlMainView::draw();
    if (gridOptionsWidget !=0)
      gridOptionsWidget->validateGrid();
    //overviewWidget->updateView();
  }
  //==================================================
  void NodeLinkDiagramComponent::refresh() {
    draw();
  }
  //==================================================
  void NodeLinkDiagramComponent::init() {
    for(map<string,DataSet>::iterator it=algorithmInfoDataSet.begin();it!=algorithmInfoDataSet.end();++it){
      string layerName;
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
    string name(action->text().toStdString());

    if (name=="3D &Overview") {
      if(overviewFrame->isVisible())
	overviewFrame->hide();
      else
	overviewFrame->show();
    }

    if(name =="Augmented Display") {
      Graph *graph=mainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      AugmentedDisplayDialog dialog(widget,graph,"NodeLinkDiagramComponent");
      if(dialog.exec()==QDialog::Accepted){
        vector<string> toRemove;
        dialog.getRemovedList(toRemove);
        DataSet nldcDataSet;
        graph->getAttribute("NodeLinkDiagramComponent",nldcDataSet);
        for(vector<string>::iterator it=toRemove.begin();it!=toRemove.end();++it){
          DataSet data=algorithmInfoDataSet[*it];
          string layerName;
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
  void NodeLinkDiagramComponent::addLayer(GlScene* scene, const std::string& name, GlLayer* layer) {
    //renderingParametersDialog->addLayer(scene,name,layer);
  }
  void NodeLinkDiagramComponent::modifyLayer(GlScene* scene, const std::string& name, GlLayer* layer){
    //renderingParametersDialog->updateLayer(name,layer);
  }
}

