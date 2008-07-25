#include "tulip/GlMainView.h"

#include <QtGui/QStatusBar>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

#include <tulip/ExtendedClusterOperation.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StableIterator.h>

#include "tulip/FindSelectionWidget.h"
#include "tulip/InteractorManager.h"
#include "tulip/TabWidget.h"
#include "tulip/GWOverviewWidget.h"
#include "tulip/RenderingParametersDialog.h"
#include "tulip/LayerManagerWidget.h"

#define UNNAMED "unnamed"

using namespace std;

namespace tlp {

  VIEWPLUGIN(GlMainViewCreator, "MainView", "Tulip Team", "16/04/2008", "Main view", "1.0", 0);  

  View* GlMainViewCreator::create(QWidget *parent){
    return new GlMainView(parent);
  }

  //==================================================
  GlMainView::GlMainView(QWidget *parent, const char *name):
    View(parent) {
    setupUi(this);
    copyCutPasteGraph=0;

    mainWidget=new GlMainWidget(this,"mainWidget");
    setCentralWidget(mainWidget);
    constructInteractorsMap();

    // Create overview widget after the tabWidgetDock
    // because of a bug with full docked GlMainWidget
    // In doing this the overviewDock will be the first
    // sibling candidate when the tabWidgetDock will loose the focus
    // and Qt will not try to give the focus to the first GlMainWidget
    overviewDock = new QDockWidget("Overview", this);
    overviewDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    overviewDock->setWindowTitle("3D Overview");
    overviewDock->setFeatures(QDockWidget::DockWidgetClosable |
			      QDockWidget::DockWidgetMovable |
			      QDockWidget::DockWidgetFloatable);
    //overviewDock->setResizeEnabled(true);
    overviewWidget = new GWOverviewWidget(overviewDock);
    overviewDock->setWidget(overviewWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, overviewDock);
    overviewWidget->show(); 
    overviewDock->show();

    //View Menu
    viewMenu->addAction("&Redraw View", this, SLOT(redrawView()), tr("Ctrl+Shift+R"));
    viewMenu->addAction("&Center View", this, SLOT(centerView()), tr("Ctrl+Shift+C"));
    //viewMenu->addAction("&New 3D View", this, SLOT(new3DView()), tr("Ctrl+Shift+N"));
    //Dialogs Menu
    connect(dialogMenu, SIGNAL(triggered(QAction*)), SLOT(showDialog(QAction*)));
    dialogMenu->addAction("3D &Overview");
    //dialogMenu->addAction("&Info Editor");
    dialogMenu->addAction("&Layer Manager");
    renderingParametersDialogAction = dialogMenu->addAction("&Rendering Parameters");
    renderingParametersDialogAction->setShortcut(tr("Ctrl+R"));

    renderingParametersDialog=new RenderingParametersDialog(this);
    layerWidget = new LayerManagerWidget(this);
    
  }
  //==================================================
  GlMainView::~GlMainView() {
    delete renderingParametersDialog;
  }

  //==================================================
  // Accessor and set 
  //==================================================
  void GlMainView::setData(Graph *graph,string *in) {
    mainWidget->setData(graph,in);
    overviewWidget->setObservedView(mainWidget);
    mainWidget->getScene()->centerScene();
    layerWidget->attachMainWidget(mainWidget);
    redrawView();
  }
  //==================================================
  Graph *GlMainView::getGraph() {
    return mainWidget->getGraph();
  }
  //==================================================
  QWidget *GlMainView::getWidget() {
    return this;
  }
  //==================================================
  GlMainWidget *GlMainView::getGlMainWidget() {
    return mainWidget;
  }
  //==================================================
  void GlMainView::changeGraph(Graph *graph) {
    if(mainWidget->getGraph()!=graph)
      setData(graph);
    
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
    cout << "Size: " << interactorsMap["addNode"].size() << endl;
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
  
  void GlMainView::getEditMenuFlags(Vector<bool, 10> &flags) {
    flags.fill(true);
  }

  //==================================================
  // GUI functions
  //==================================================
  void  GlMainView::redrawView() {
    mainWidget->draw();
  }
  //==================================================
  void GlMainView::centerView() {
    mainWidget->getScene()->centerScene();
    overviewWidget->setObservedView(mainWidget);
    redrawView();
  }
  //==================================================
  void GlMainView::showDialog(QAction* action){
    string name(action->text().toStdString());

    /*if (name=="&Info Editor") {
      tabWidgetDock->show();
      tabWidgetDock->raise();
      }*/

    if (name=="3D &Overview") {
      overviewDock->show();
      overviewDock->raise();
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
  namespace {
    typedef std::vector<node> NodeA;
    typedef std::vector<edge> EdgeA;
    
    void GetSelection(NodeA & outNodeA, EdgeA & outEdgeA,
		      Graph *inG, BooleanProperty * inSel ) {
      assert( inSel );
      assert( inG );
      outNodeA.clear();
      outEdgeA.clear();
      // Get edges
      Iterator<edge> * edgeIt = inG->getEdges();
      while( edgeIt->hasNext() ) {
	edge e = edgeIt->next();
	if( inSel->getEdgeValue(e) )
	  outEdgeA.push_back( e );
      } delete edgeIt;
      // Get nodes
      Iterator<node> * nodeIt = inG->getNodes();
      while( nodeIt->hasNext() ) {
	node n = nodeIt->next();
	if( inSel->getNodeValue(n) )
	  outNodeA.push_back( n );
      } delete nodeIt;
    }

    void SetSelection(BooleanProperty * outSel, NodeA & inNodeA,
		      EdgeA & inEdgeA, Graph * inG) {
      assert( outSel );
      assert( inG );
      outSel->setAllNodeValue( false );
      outSel->setAllEdgeValue( false );
      // Set edges
      for( unsigned int e = 0 ; e < inEdgeA.size() ; e++ )
	outSel->setEdgeValue( inEdgeA[e], true );
      // Set nodes
      for( unsigned int n = 0 ; n < inNodeA.size() ; n++ )
	outSel->setNodeValue( inNodeA[n], true );
    }
  }
  //==================================================
  void GlMainView::selectAll() {
    Graph *graph=mainWidget->getGraph();
    if (graph==0) return;
    Observable::holdObservers();
    graph->getLocalProperty<BooleanProperty>("viewSelection")->setAllNodeValue(true);
    graph->getLocalProperty<BooleanProperty>("viewSelection")->setAllEdgeValue(true);
    mainWidget->getScene()->getGlGraphComposite()->getInputData()->reloadSelectionProperty();
    Observable::unholdObservers();
  }
  //==================================================
  void GlMainView::deselectAll() {
    Graph *graph=mainWidget->getGraph();
    if (graph==0) return;
    Observable::holdObservers();
    graph->getProperty<BooleanProperty>("viewSelection")->setAllNodeValue(false);
    graph->getProperty<BooleanProperty>("viewSelection")->setAllEdgeValue(false);
    Observable::unholdObservers();
  }
  //==================================================
  void GlMainView::delSelection() {
    Graph *graph=mainWidget->getGraph();
    if (graph==0) return;
    Observable::holdObservers();
    BooleanProperty *elementSelected=graph->getProperty<BooleanProperty>("viewSelection");
    StableIterator<node> itN(graph->getNodes());
    while(itN.hasNext()) {
      node itv = itN.next();
      if (elementSelected->getNodeValue(itv)==true)
	graph->delNode(itv);
    }
    StableIterator<edge> itE(graph->getEdges());
    while(itE.hasNext()) {
      edge ite=itE.next();
      if (elementSelected->getEdgeValue(ite)==true)   
	graph->delEdge(ite);
    }
    Observable::unholdObservers();
  }
  //==================================================
  void GlMainView::reverseSelection() {
    Graph *graph=mainWidget->getGraph();
    if (graph==0) return;
    Observable::holdObservers();
    graph->getLocalProperty<BooleanProperty>("viewSelection")->reverse();
    mainWidget->getScene()->getGlGraphComposite()->getInputData()->reloadSelectionProperty();
    Observable::unholdObservers();
  }
  //=================================================
  void GlMainView::newSubgraph() {
    //if (!glWidget) return;
    Graph *graph=mainWidget->getGraph();
    if (graph==0) return;
    bool ok = FALSE;
    string tmp;
    bool verifGraph = true;
    BooleanProperty *sel1 = graph->getProperty<BooleanProperty>("viewSelection");  
    Observable::holdObservers();
    Iterator<edge>*itE = graph->getEdges();
    while (itE->hasNext()) {
      edge ite= itE->next();
      if (sel1->getEdgeValue(ite)) {
	if (!sel1->getNodeValue(graph->source(ite))) {sel1->setNodeValue(graph->source(ite),true); verifGraph=false;}
	if (!sel1->getNodeValue(graph->target(ite))) {sel1->setNodeValue(graph->target(ite),true); verifGraph=false;}
      }
    } delete itE;
    Observable::unholdObservers();
    
    if(!verifGraph) 
      QMessageBox::critical( 0, "Tulip Warning" ,"The selection wasn't a graph, missing nodes have been added");
    QString text = QInputDialog::getText(this,
					 "Creation of subgraph" ,
					 "Please enter the subgraph name" ,
					 QLineEdit::Normal, QString::null, &ok);
    if (ok && !text.isEmpty()) {
      sel1 = graph->getProperty<BooleanProperty>("viewSelection");
      Graph *tmp = graph->addSubGraph(sel1);
      tmp->setAttribute("name",string(text.toAscii().data()));
      //clusterTreeWidget->update();
    }
    else if (ok) {
      sel1 = graph->getProperty<BooleanProperty>("viewSelection");
      Graph *tmp=graph->addSubGraph(sel1);
      tmp->setAttribute("name", newName());
      //clusterTreeWidget->update();
    }
  }
  //================================================
  void GlMainView::reverseSelectedEdgeDirection() {
    //if (!glWidget) return;
    Graph *graph=mainWidget->getGraph();
    if (graph==0) return;
    Observable::holdObservers();
    graph->getProperty<BooleanProperty>("viewSelection")->reverseEdgeDirection();  
    Observable::unholdObservers();
  }
  //==================================================
  void GlMainView::editCut() {
    Graph * g = mainWidget->getGraph();
    if( !g ) return;
    // free the previous ccpGraph
    if( copyCutPasteGraph ) {
      delete copyCutPasteGraph;
      copyCutPasteGraph = 0;
    }
    BooleanProperty * selP = g->getProperty<BooleanProperty>("viewSelection");
    if( !selP ) return;
    // Save selection
    NodeA nodeA;
    EdgeA edgeA;
    GetSelection( nodeA, edgeA, g, selP );
    Observable::holdObservers();
    copyCutPasteGraph = tlp::newGraph();
    tlp::copyToGraph( copyCutPasteGraph, g, selP );
    // Restore selection
    SetSelection( selP, nodeA, edgeA, g );
    tlp::removeFromGraph( g, selP );
    Observable::unholdObservers();
  }
  //==================================================
  void GlMainView::editCopy() {
    Graph * g = mainWidget->getGraph();
    if( !g ) return;
    // free the previous ccpGraph
    if( copyCutPasteGraph ) {
      delete copyCutPasteGraph;
      copyCutPasteGraph = 0;
    }
    BooleanProperty * selP = g->getProperty<BooleanProperty>("viewSelection");
    if( !selP ) return;
    Observable::holdObservers();
    copyCutPasteGraph = tlp::newGraph();
    tlp::copyToGraph( copyCutPasteGraph, g, selP );
    Observable::unholdObservers();
  }
  //==================================================
  void GlMainView::editPaste() {
    //if( !glWidget ) return;
    Graph * g = mainWidget->getGraph();
    if( !g ) return;
    if( !copyCutPasteGraph ) return;
    Observable::holdObservers();
    BooleanProperty * selP = g->getProperty<BooleanProperty>("viewSelection");
    tlp::copyToGraph( g, copyCutPasteGraph, 0, selP );
    Observable::unholdObservers();
  }
  //==================================================
  void GlMainView::editFind() {
    //if(!glWidget) return;
    Graph * g = mainWidget->getGraph();
    if( !g ) return;
    
    static string currentProperty;
    FindSelectionWidget *sel = new FindSelectionWidget(g, currentProperty, this);
    Observable::holdObservers();
    int nbItemsFound = sel->exec();
    Observable::unholdObservers();
    if (nbItemsFound > - 1)
      currentProperty = sel->getCurrentProperty();
    delete sel;
    switch(nbItemsFound) {
    case -1: break;
    case 0: statusBar()->showMessage("No item found."); break;
    default:
      stringstream sstr;
      sstr << nbItemsFound << " item(s) found.";
      statusBar()->showMessage(sstr.str().c_str());
    }
  }
  //==================================================
  void GlMainView::group() {
    set<node> tmp;
    Graph *graph=mainWidget->getGraph();
    Iterator<node> *it=graph->getNodes();
    BooleanProperty *select = graph->getProperty<BooleanProperty>("viewSelection");
    while (it->hasNext()) {
      node itn = it->next();
      if (select->getNodeValue(itn))
	tmp.insert(itn);
    }delete it;
    if (tmp.empty()) return;
    if (graph == graph->getRoot()) {
      QMessageBox::critical( 0, "Warning" ,"Grouping can't be done on the root graph, a subgraph will be created");    
      graph = tlp::newCloneSubGraph(graph, "groups");
    }
    node metaNode = tlp::createMetaNode(graph, tmp);
    // set metanode viewColor to glWidget background color
    Color metaNodeColor = graph->getProperty<ColorProperty>("viewColor")->getNodeValue(metaNode);
    metaNodeColor[3]=127;
    graph->getProperty<ColorProperty>("viewColor")->setNodeValue(metaNode,metaNodeColor);
    //clusterTreeWidget->update();
    //changeGraph(graph);
  }
  //==================================================
  std::string GlMainView::newName() {
    static int idx = 0;
    
    if (idx++ == 0)
      return std::string(UNNAMED);
    
    stringstream ss;
    ss << UNNAMED << '_' << idx - 1;
    return ss.str();
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

