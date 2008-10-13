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

#include "tulip/GWOverviewWidget.h"
#include "tulip/RenderingParametersDialog.h"
#include "tulip/LayerManagerWidget.h"
#include "tulip/GridOptionsWidget.h"
#include "tulip/InteractorManager.h"

using namespace std;

namespace tlp {

  VIEWPLUGIN(NodeLinkDiagramComponentCreator, "NodeLinkDiagramComponent", "Tulip Team", "16/04/2008", "Node link diagram", "1.0", 0);  

  View* NodeLinkDiagramComponentCreator::create(const string &pluginName,QWidget *parent){
    return new NodeLinkDiagramComponent(pluginName,parent);
  }

  //==================================================
  NodeLinkDiagramComponent::NodeLinkDiagramComponent(const string &pluginName,QWidget *parent, const char *name):
    GlMainView(pluginName,parent,name) {
    
    constructInteractorsMap();

    //View Menu
    viewMenu=new QMenu("View");
    viewMenu->addAction("&Redraw View", this, SLOT(draw()), tr("Ctrl+Shift+R"));
    viewMenu->addAction("&Center View", this, SLOT(centerView()), tr("Ctrl+Shift+C"));
    viewMenu->addAction("&New 3D View", this, SLOT(new3DView()), tr("Ctrl+Shift+N"));
    //Dialogs Menu
    dialogMenu=new QMenu("Dialog");
    connect(dialogMenu, SIGNAL(triggered(QAction*)), SLOT(showDialog(QAction*)));
    overviewAction=dialogMenu->addAction("3D &Overview");
    overviewAction->setCheckable(true);
    overviewAction->setChecked(true);
    dialogMenu->addAction("&Info Editor");
    renderingParametersDialogAction = dialogMenu->addAction("&Rendering Parameters");
    renderingParametersDialogAction->setShortcut(tr("Ctrl+R"));
    renderingParametersDialog=new RenderingParametersDialog(this);
    layerWidget = new LayerManagerWidget(this);
    //Options Menu
    optionsMenu=new QMenu("Options");
    actionTooltips=optionsMenu->addAction("Tooltips");
    actionTooltips->setCheckable(true);
    actionTooltips->setChecked(false);
    actionsGridOptions=optionsMenu->addAction("Grid");
    connect(actionsGridOptions, SIGNAL(triggered()), SLOT(gridOptions()));
    gridOptionsWidget=NULL;
    
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
  }
  //==================================================
  NodeLinkDiagramComponent::~NodeLinkDiagramComponent() {
    delete renderingParametersDialog;
  }
  //==================================================
  // Accessor and set 
  //==================================================
  void NodeLinkDiagramComponent::setData(Graph *graph,DataSet dataSet) {
    DataSet data;
    if(dataSet.exist("data")){
      dataSet.get("data",data);
    }
    mainWidget->setData(graph,data);
    overviewWidget->setObservedView(mainWidget);
    layerWidget->attachMainWidget(mainWidget);
    init();
  }
  //==================================================
  void NodeLinkDiagramComponent::getData(Graph **graph,DataSet *dataSet) {
    dataSet->set<DataSet>("data",mainWidget->getData());
    *graph=mainWidget->getGraph();
  }
  //==================================================
  // Interactor functions
  //==================================================
  void NodeLinkDiagramComponent::constructInteractorsMap() {
    MutableContainer<Interactor *> interactors;
    InteractorManager::getInst().initInteractorList(interactors);
    interactorsMap["addEdge"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["addEdge"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseNodeBuilder")));
    interactorsMap["addEdge"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseEdgeBuilder")));
    interactorsMap["addNode"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["addNode"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseNodeBuilder")));
    interactorsMap["deleteElt"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["deleteElt"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseElementDeleter")));
    interactorsMap["graphNavigate"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseNKeysNavigator")));
    interactorsMap["magicSelection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["magicSelection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseMagicSelector")));
    interactorsMap["editSelection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["editSelection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseSelector")));
    interactorsMap["editSelection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseSelectionEditor")));
    interactorsMap["editEdgeBend"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["editEdgeBend"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseEdgeSelector")));
    interactorsMap["editEdgeBend"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseEdgeBendEditor")));
    interactorsMap["select"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["select"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseShowElementInfos")));
    interactorsMap["selection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["selection"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseSelector")));
    interactorsMap["zoomBox"].push_back(interactors.get(InteractorManager::getInst().interactorId("MousePanNZoomNavigator")));
    interactorsMap["zoomBox"].push_back(interactors.get(InteractorManager::getInst().interactorId("MouseBoxZoomer")));
  }
  //==================================================
  void NodeLinkDiagramComponent::getInteractorsActionList(list<QAction*> &interactorsList) {
    interactorsList.push_back(new QAction(QIcon(":/i_navigation.png"),"graphNavigate",this));
    interactorsList.push_back(new QAction(QIcon(":/i_select.png"),"select",this));
    interactorsList.push_back(new QAction(QIcon(":/i_selection.png"),"selection",this));
    interactorsList.push_back(new QAction(QIcon(":/i_move.png"),"editSelection",this));
    interactorsList.push_back(new QAction(QIcon(":/i_magic.png"),"magicSelection",this));
    interactorsList.push_back(new QAction(QIcon(":/i_zoom.png"),"zoomBox",this));
    interactorsList.push_back(new QAction(QIcon(":/i_del.png"),"deleteElt",this));
    interactorsList.push_back(new QAction(QIcon(":/i_addnode.png"),"addNode",this));
    interactorsList.push_back(new QAction(QIcon(":/i_addedge.png"),"addEdge",this));
    interactorsList.push_back(new QAction(QIcon(":/i_bends.png"),"editEdgeBend",this));
  }
  void NodeLinkDiagramComponent::installInteractor(QAction *action) {
    resetInteractors(interactorsMap[action->text().toStdString()]);
  }

  void NodeLinkDiagramComponent::specificEventFilter(QObject *object,QEvent *event) {
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
    if ( object->inherits("tlp::GlMainView")) {
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
      contextMenu->addAction(tr("Add to/Remove from selection"));
      selectAction = contextMenu->addAction(tr("Select"));
      deleteAction = contextMenu->addAction(tr("Delete"));
      contextMenu->addSeparator();
      goAction = NULL;
      ungroupAction = NULL;
      if (isNode) {
	GraphProperty *meta = graph->getProperty<GraphProperty>("viewMetaGraph");
	if (meta->getNodeValue(tmpNode)!=0) {
	  goAction = contextMenu->addAction(tr("Go inside"));
	  ungroupAction = contextMenu->addAction(tr("Ungroup"));
	}
      }
      if (goAction != NULL)
	contextMenu->addSeparator();
      propAction = contextMenu->addAction(tr("Properties"));
    }
  }
  
  void NodeLinkDiagramComponent::computeContextMenuAction(QAction *action) {
    Graph *graph=mainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
    node tmpNode;
    edge tmpEdge;

    Observable::holdObservers();
    if (action == deleteAction) { // Delete
      // delete graph item
      if (isNode)
	graph->delNode(node(itemId));
      else
	graph->delEdge(edge(itemId));
    } else {
      if (action == propAction) // Properties
	showElementProperties(itemId, isNode);
      else  {
	if (action == goAction) { // Go inside
	  GraphProperty *meta = graph->getProperty<GraphProperty>("viewMetaGraph");
	  setGraph(meta->getNodeValue(tmpNode));
	}
	else  {
	  if (action == ungroupAction) { // Ungroup
	    tlp::openMetaNode(graph, tmpNode);
	  } else {
	    BooleanProperty *elementSelected = graph->getProperty<BooleanProperty>("viewSelection");
	    if (action == selectAction) { // Select
	      // empty selection
	      elementSelected->setAllNodeValue(false);
	      elementSelected->setAllEdgeValue(false);
	    }
	    // selection add/remove graph item
	    if (isNode)
	      elementSelected->setNodeValue(tmpNode, !elementSelected->getNodeValue(tmpNode));
	    else
	      elementSelected->setEdgeValue(tmpEdge, !elementSelected->getEdgeValue(tmpEdge));
	  }
	}
      }
    }
    Observable::unholdObservers();
  }
  //==================================================
  // GUI functions
  //==================================================
  void NodeLinkDiagramComponent::draw() {
    GlMainView::draw();
    if (gridOptionsWidget !=0) 
      gridOptionsWidget->validateGrid();
    overviewWidget->updateView();
  }
  //==================================================
  void NodeLinkDiagramComponent::refresh() {
    draw();
  }
  //==================================================
  void NodeLinkDiagramComponent::init() {
    centerView();
  }
  //==================================================
  void NodeLinkDiagramComponent::centerView() {
    mainWidget->getScene()->centerScene();
    overviewWidget->setObservedView(mainWidget);
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

    if (name=="&Layer Manager") {
      layerWidget->show();
    }

    if (name=="&Rendering Parameters") {
      renderingParametersDialog->setGlMainView(this);
      renderingParametersDialog->exec();
    }
  }
  //==================================================
  void NodeLinkDiagramComponent::gridOptions() {
    if (gridOptionsWidget == 0)
      gridOptionsWidget = new GridOptionsWidget(this);
    gridOptionsWidget->setCurrentMainWidget(mainWidget);
    gridOptionsWidget->setCurrentRenderingParametersDialog(renderingParametersDialog);
    gridOptionsWidget->show();
  }
  //==================================================
  // GlSceneObserver interface
  void NodeLinkDiagramComponent::addLayer(GlScene* scene, const std::string& name, GlLayer* layer) {
    layerWidget->addLayer(scene,name,layer);
  }
  void NodeLinkDiagramComponent::modifyLayer(GlScene* scene, const std::string& name, GlLayer* layer){
    layerWidget->updateLayer(name,layer);
  }
}

