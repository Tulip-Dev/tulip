#include "tulip/GlMainView.h"

#include <QtGui/QStatusBar>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMouseEvent>
#include <QtGui/QToolTip>
#include <QtGui/QMenuBar>
#include <QtGui/QImageWriter>
#include <QtGui/QFileDialog>

#include <tulip/ExtendedClusterOperation.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/StableIterator.h>

#include "tulip/FindSelectionWidget.h"
#include "tulip/InteractorManager.h"
#include "tulip/TabWidget.h"
#include "tulip/GWOverviewWidget.h"
#include "tulip/RenderingParametersDialog.h"
#include "tulip/LayerManagerWidget.h"
#include "tulip/GridOptionsWidget.h"

using namespace std;

namespace tlp {

  VIEWPLUGIN(GlMainViewCreator, "MainView", "Tulip Team", "16/04/2008", "Main view", "1.0", 0);  

  View* GlMainViewCreator::create(const string &pluginName,QWidget *parent){
    return new GlMainView(pluginName,parent);
  }

  //==================================================
  GlMainView::GlMainView(const string &pluginName,QWidget *parent, const char *name):
    View(pluginName,parent) {

    mainWidget=new GlMainWidget(this,"mainWidget");

    setCentralWidget(mainWidget);

    constructInteractorsMap();

    // Create overview widget
    overviewFrame = new QFrame(mainWidget);
    overviewFrame->setContentsMargins(0,0,0,0);
    overviewFrame->setGeometry(QRect(0, 0, 100, 100));
    overviewFrame->setFrameShape(QFrame::StyledPanel);
    overviewFrame->setFrameShadow(QFrame::Raised);
    QGridLayout *gridLayout_2 = new QGridLayout(overviewFrame);
    gridLayout_2->setMargin(0);
    overviewWidget = new GWOverviewWidget(overviewFrame);
    gridLayout_2->addWidget(overviewWidget, 0, 0, 1, 1);
    connect(overviewWidget,SIGNAL(hideOverview(bool)),this,SLOT(hideOverview(bool)));
    
    //View Menu
    viewMenu=new QMenu("View");
    viewMenu->addAction("&Redraw View", this, SLOT(redrawView()), tr("Ctrl+Shift+R"));
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
  GlMainView::~GlMainView() {
    delete renderingParametersDialog;
  }

  //==================================================
  // Accessor and set 
  //==================================================
  void GlMainView::setData(Graph *graph,DataSet dataSet) {
    DataSet data;
    if(dataSet.exist("data")){
      dataSet.get("data",data);
    }
    mainWidget->setData(graph,data);
    overviewWidget->setObservedView(mainWidget);
    mainWidget->getScene()->centerScene();
    layerWidget->attachMainWidget(mainWidget);
    redrawView();
  }
  //==================================================
  DataSet GlMainView::getData() {
    DataSet dataSet;
    dataSet.set<DataSet>("data",mainWidget->getData());
    return dataSet;
  }
  //==================================================
  Graph *GlMainView::getGraph() {
    return mainWidget->getGraph();
  }
  //==================================================
  GlMainWidget *GlMainView::getGlMainWidget() {
    return mainWidget;
  }
  //==================================================
  void GlMainView::changeGraph(Graph *graph) {
    if(mainWidget->getGraph()!=graph)
      setData(graph,DataSet());
  }

  //==================================================
  // Interactor functions
  //==================================================
  void GlMainView::constructInteractorsMap() {
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
  void GlMainView::getInteractorsActionList(list<QAction*> &interactorsList) {
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
  Iterator<Interactor *> *GlMainView::installInteractor(const string &name) {
    mainWidget->resetInteractors(interactorsMap[name]);
    return mainWidget->getInteractors();
  }

  void GlMainView::addNode(Graph *graph,const node n) {
    mainWidget->addNode(graph,n);
    redrawView();
  }
  void GlMainView::addEdge(Graph *graph,const edge e) {
    mainWidget->addEdge(graph,e);
    redrawView();
  }
  void GlMainView::delNode(Graph *graph,const node n) {
    mainWidget->delNode(graph,n);
    redrawView();
  }
  void GlMainView::delEdge(Graph *graph,const edge e) {
    mainWidget->delEdge(graph,e);
    redrawView();
  }

  void GlMainView::specificEventFilter(QObject *object,QEvent *event) {
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

  void GlMainView::buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu) {
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
  
  void GlMainView::computeContextMenuAction(QAction *action) {
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
	  changeGraph(meta->getNodeValue(tmpNode));
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
  void GlMainView::progressUpdate() {
    centerView();
    redrawView();
  }
  void GlMainView::exportImage(QAction* action) {
    string name = action->text().toStdString();
    QString s(QFileDialog::getSaveFileName());
    if (s.isNull()) return;    
    if (name=="EPS") {
      if (!mainWidget->outputEPS(64000000,true,s.toAscii().data()))
	QMessageBox::critical( 0, "Save Picture Failed",
			       "The file has not been saved.");
      return;
    } else if (name=="SVG") {
      if (!mainWidget->outputSVG(64000000,s.toAscii().data()))
	QMessageBox::critical( 0, "Save Picture Failed",
			       "The file has not been saved.");
      return;
    } 
    int width,height;
    width = mainWidget->width();
    height = mainWidget->height();
    unsigned char* image= mainWidget->getImage();
    QPixmap pm(width,height);
    QPainter painter;
    painter.begin(&pm);
    for (int y=0; y<height; y++)
      for (int x=0; x<width; x++) {
	painter.setPen(QColor(image[(height-y-1)*width*3+(x)*3],
			      image[(height-y-1)*width*3+(x)*3+1],
			      image[(height-y-1)*width*3+(x)*3+2]));
	painter.drawPoint(x,y);
      }
    painter.end();
    free(image);
    pm.save( s, name.c_str());
  }
  //==================================================
  // GUI functions
  //==================================================
  void  GlMainView::redrawView() {
    if (gridOptionsWidget !=0) 
      gridOptionsWidget->validateGrid();
    mainWidget->draw();
    overviewWidget->updateView();
  }
  //==================================================
  void GlMainView::centerView() {
    mainWidget->getScene()->centerScene();
    overviewWidget->setObservedView(mainWidget);
    redrawView();
  }
  //==================================================
  void GlMainView::hideOverview(bool hide) {
    if(hide) {
      overviewFrame->hide();
    }else{
      overviewFrame->show();
    }
    overviewAction->setChecked(!hide);
  }
  //==================================================
  void GlMainView::showDialog(QAction* action){
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
  void GlMainView::gridOptions() {
    if (gridOptionsWidget == 0)
      gridOptionsWidget = new GridOptionsWidget(this);
    gridOptionsWidget->setCurrentMainWidget(mainWidget);
    gridOptionsWidget->setCurrentRenderingParametersDialog(renderingParametersDialog);
    gridOptionsWidget->show();
  }
  //==================================================
  /*void GlMainView::showElementProperties(unsigned int eltId, bool isNode) {
    cout << "GlMainView::showElementProperties" << endl;
    emit showElementPropertiesSignal(eltId, isNode);
    }*/
  //==================================================
  // GlSceneObserver interface
  void GlMainView::addLayer(GlScene* scene, const std::string& name, GlLayer* layer) {
    layerWidget->addLayer(scene,name,layer);
  }
  void GlMainView::modifyLayer(GlScene* scene, const std::string& name, GlLayer* layer){
    layerWidget->updateLayer(name,layer);
  }
}

