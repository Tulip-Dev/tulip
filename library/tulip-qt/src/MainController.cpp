#include "tulip/MainController.h"

#include <stdio.h>

#include <QtGui/QDockWidget>
#include <QtGui/QWorkspace>
#include <QtGui/QToolBar>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QStatusBar>
#include <QtGui/QInputDialog>
#include <QtGui/QClipboard>
#include <QtGui/QTabWidget>

#include <tulip/tuliphash.h>
#include <tulip/Graph.h>
#include <tulip/Algorithm.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/TlpQtTools.h>
#include <tulip/ExtendedClusterOperation.h>
#include <tulip/StableIterator.h>
#include <tulip/ForEach.h>

#include <tulip/AcyclicTest.h>
#include <tulip/SimpleTest.h>
#include <tulip/ConnectedTest.h>
#include <tulip/BiconnectedTest.h>
#include <tulip/TriconnectedTest.h>
#include <tulip/TreeTest.h>
#include <tulip/GraphTools.h>
#include <tulip/PlanarityTest.h>
#include <tulip/OuterPlanarTest.h>

#include "tulip/TabWidget.h"
#include "tulip/ViewPluginsManager.h"
#include "tulip/QtProgress.h"
#include "tulip/Morphing.h"
#include "tulip/FindSelectionWidget.h"
#include "tulip/NodeLinkDiagramComponent.h"
#include "tulip/GlMainWidget.h"
#include "tulip/InteractorManager.h"

using namespace std;

namespace tlp {

  //**********************************************************************
  static Graph* getCurrentSubGraph(Graph *graph, int id) {
    if (graph->getId() == id) {
      return graph;
    }
    Graph *sg;
    forEach(sg, graph->getSubGraphs()) {
      Graph *csg = getCurrentSubGraph(sg, id);
      if (csg)
        returnForEach(csg);
    }
    return (Graph *) 0;
  }
  //**********************************************************************
  static Graph* getCurrentSubGraph(Graph *graph,const string &name) {
    if(graph->getAttribute<string>("name")==name)
      return graph;

    Graph *sg;
    forEach(sg, graph->getSubGraphs()) {
      Graph *csg = getCurrentSubGraph(sg, name);
      if (csg)
        returnForEach(csg);
    }
    return (Graph *) 0;
  }
  //**********************************************************************
  // we use a hash_map to store plugin parameters
  static StructDef *getPluginParameters(TemplateFactoryInterface *factory, std::string name) {
    static TLP_HASH_MAP<unsigned long, TLP_HASH_MAP<std::string, StructDef * > > paramMaps;
    TLP_HASH_MAP<std::string, StructDef *>::const_iterator it;
    it = paramMaps[(unsigned long) factory].find(name);
    if (it == paramMaps[(unsigned long) factory].end())
      paramMaps[(unsigned long) factory][name] = new StructDef(factory->getPluginParameters(name));
    return paramMaps[(unsigned long) factory][name];
  }
  //*********************************************************************
  static std::vector<std::string> getItemGroupNames(std::string itemGroup) {
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    std::vector<std::string> groupNames;
    const char * separator = "::";

    while(true) {
      start = itemGroup.find_first_not_of(separator, end);
      if (start == std::string::npos) {
	return groupNames;
      }
      end = itemGroup.find_first_of(separator, start);
      if (end == std::string::npos)
	end = itemGroup.length();
      groupNames.push_back(itemGroup.substr(start, end - start));
    }
  }
  //**********************************************************************
  static void insertInMenu(QMenu &menu, string itemName, string itemGroup,
			   std::vector<QMenu*> &groupMenus, std::string::size_type &nGroups) {
    std::vector<std::string> itemGroupNames = getItemGroupNames(itemGroup);
    QMenu *subMenu = &menu;
    std::string::size_type nGroupNames = itemGroupNames.size();
    for (std::string::size_type i = 0; i < nGroupNames; i++) {
      QMenu *groupMenu = (QMenu *) 0;
      for (std::string::size_type j = 0; j < nGroups; j++) {
	if (itemGroupNames[i] == groupMenus[j]->objectName().toAscii().data()) {
	  subMenu = groupMenu = groupMenus[j];
	  break;
	}
      }
      if (!groupMenu) {
	groupMenu = new QMenu(itemGroupNames[i].c_str(), subMenu);
	groupMenu->setObjectName(QString(itemGroupNames[i].c_str()));
	subMenu->addMenu(groupMenu);
	groupMenus.push_back(groupMenu);
	nGroups++;
	subMenu = groupMenu;
      }
    }
    subMenu->addAction(itemName.c_str());
  }
  //**********************************************************************
  template <typename TYPEN, typename TYPEE, typename TPROPERTY>
  void buildPropertyMenu(QMenu &menu, QObject *receiver, const char *slot) {
    typename TemplateFactory<PropertyFactory<TPROPERTY>, TPROPERTY, PropertyContext>::ObjectCreator::const_iterator it;
    std::vector<QMenu*> groupMenus;
    std::string::size_type nGroups = 0;
    it=AbstractProperty<TYPEN, TYPEE, TPROPERTY>::factory->objMap.begin();
    for (;it!=AbstractProperty<TYPEN,TYPEE, TPROPERTY>::factory->objMap.end();++it)
      insertInMenu(menu, it->first.c_str(), it->second->getGroup(), groupMenus, nGroups);
  }
  template <typename TFACTORY, typename TMODULE>
  void buildMenuWithContext(QMenu &menu, QObject *receiver, const char *slot) {
    typename TemplateFactory<TFACTORY, TMODULE, AlgorithmContext>::ObjectCreator::const_iterator it;
    std::vector<QMenu*> groupMenus;
    std::string::size_type nGroups = 0;
    for (it=TFACTORY::factory->objMap.begin();it != TFACTORY::factory->objMap.end();++it)
      insertInMenu(menu, it->first.c_str(), it->second->getGroup(), groupMenus, nGroups);
  }
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
  //==================================================
  #define UNNAMED "unnamed"
  std::string newName() {
    static int idx = 0;

    if (idx++ == 0)
      return std::string(UNNAMED);
    stringstream ss;
    ss << UNNAMED << '_' << idx - 1;
    return ss.str();
  }


  //**********************************************************************
  //**********************************************************************
  //**********************************************************************
  //**********************************************************************
  //**********************************************************************
  CONTROLLERPLUGIN(MainController, "MainController", "Tulip Team", "16/04/2008", "Main controller", "1.0");

  //**********************************************************************
  MainController::MainController():
    currentGraph(NULL),currentView(NULL),copyCutPasteGraph(NULL),currentGraphNbNodes(0),currentGraphNbEdges(0),graphToReload(NULL),blockUpdate(false),clusterTreeWidget(NULL) {
    morph = new Morphing();
  }
  //**********************************************************************
  MainController::~MainController() {
    clearObservers();
    if (currentGraph) {
      currentGraph->removeObserver(this);
      currentGraph->removeGraphObserver(this);
      delete editMenu;
      delete algorithmMenu;
      delete viewMenu;
      delete optionsMenu;
      delete graphMenu;
      delete undoAction;
      delete redoAction;

      delete clusterTreeWidget;
      delete propertiesWidget;
      delete eltProperties;
#ifdef STATS_UI
      delete statsWidget;
#endif

      delete tabWidgetDock;
    }
  }
  //**********************************************************************
  void MainController::attachMainWindow(MainWindowFacade facade){
    Controller::attachMainWindow(facade);
    loadGUI();
  }
  //**********************************************************************
  void MainController::setData(Graph *graph,DataSet dataSet) {
    editMenu->setEnabled(true);
    algorithmMenu->setEnabled(true);
    viewMenu->setEnabled(true);
    optionsMenu->setEnabled(true);
    graphMenu->setEnabled(true);

    Observable::holdObservers();
    Graph *newGraph=graph;
    newGraph->addObserver(this);
    newGraph->addGraphObserver(this);
    Graph *lastViewedGraph=newGraph;
    Observable::unholdObservers();
    currentGraph=newGraph;
    if(dataSet.exist("views")) {
      DataSet views;
      dataSet.get<DataSet>("views", views);
      Iterator< std::pair<std::string, DataType*> > *it=views.getValues();
      if(!it->hasNext()){
        initMainView(DataSet());
      }else{
        while(it->hasNext()) {
          pair<string, DataType*> p;
          p = it->next();
          Iterator< std::pair<std::string, DataType*> > *it2=(*(DataSet*)p.second->value).getValues();
          pair<string, DataType*> v=it2->next();
          int x,y,width,height;

          if((*(DataSet*)p.second->value).exist("id")){
            int id;
            (*(DataSet*)p.second->value).get("id",id);
            if(id!=0){
              lastViewedGraph=getCurrentSubGraph(newGraph, id);
              if(!lastViewedGraph)
                lastViewedGraph=newGraph;
            }
          }

          if((*(DataSet*)p.second->value).exist("graphName")){
            string graphName;
            (*(DataSet*)p.second->value).get("graphName",graphName);
            lastViewedGraph=getCurrentSubGraph(newGraph, graphName);
            if(!lastViewedGraph)
              lastViewedGraph=newGraph;
          }

          (*(DataSet*)p.second->value).get("x",x);
          (*(DataSet*)p.second->value).get("y",y);
          (*(DataSet*)p.second->value).get("width",width);
          (*(DataSet*)p.second->value).get("height",height);

          bool maximized=false;
          if((*(DataSet*)p.second->value).exist("maximized")){
            (*(DataSet*)p.second->value).get("maximized",maximized);
          }

          createView(v.first,lastViewedGraph,*(DataSet*)v.second->value,QRect(x,y,width,height),maximized);

        }
      }
    }else{
      NodeLinkDiagramComponent *view;
      if(dataSet.exist("scene")) {
        view=(NodeLinkDiagramComponent*)initMainView(dataSet);
      }else{
        view=(NodeLinkDiagramComponent*)initMainView(DataSet());
      }

      if(dataSet.exist("displaying")) {
        GlMainWidget *glW=view->getGlMainWidget();
        GlGraphRenderingParameters param = glW->getScene()->getGlGraphComposite()->getRenderingParameters();
        DataSet displayingData;
        dataSet.get<DataSet>("displaying", displayingData);

        param.setParameters(displayingData);
        glW->getScene()->getGlGraphComposite()->setRenderingParameters(param);
        if(displayingData.exist("backgroundColor")){
          Color backgroundColor;
          displayingData.get<Color>("backgroundColor",backgroundColor);
          glW->getScene()->setBackgroundColor(backgroundColor);
        }
        if(displayingData.exist("cameraEyes") && displayingData.exist("cameraCenter") && displayingData.exist("cameraUp") && displayingData.exist("cameraZoomFactor") && displayingData.exist("distCam")){
          Coord cameraEyes, cameraCenter, cameraUp;
          double cameraZoomFactor, distCam;
          displayingData.get<Coord>("cameraEyes",cameraEyes);
          displayingData.get<Coord>("cameraCenter",cameraCenter);
          displayingData.get<Coord>("cameraUp",cameraUp);
          displayingData.get<double>("cameraZoomFactor",cameraZoomFactor);
          displayingData.get<double>("distCam",distCam);
          Camera *camera=glW->getScene()->getLayer("Main")->getCamera();
          camera->setEyes(cameraEyes);
          camera->setCenter(cameraCenter);
          camera->setUp(cameraUp);
          camera->setZoomFactor(cameraZoomFactor);
          camera->setSceneRadius(distCam);
        }
        // show current subgraph if any
        int id = 0;
        if (displayingData.get<int>("SupergraphId", id) && id) {
          Graph *subGraph = getCurrentSubGraph(newGraph, id);
          if (subGraph){
            view->setGraph(subGraph);
            viewGraph[view]=subGraph;
          }
        }
      }

    }

    clusterTreeWidget->setGraph(lastViewedGraph);
    eltProperties->setGraph(lastViewedGraph);
    propertiesWidget->setGraph(lastViewedGraph);
    updateCurrentGraphInfos();
    initObservers();
  }
  //**********************************************************************
  void MainController::getData(Graph **graph,DataSet *dataSet) {
    DataSet views;
    QWidgetList widgetList;

    widgetList=mainWindowFacade.getWorkspace()->windowList();


    for(int i=0;i<widgetList.size();++i) {
      QRect rect=((QWidget *)(widgetList[i]->parent()))->geometry();
      DataSet tmp;
      stringstream str;
      str << "view" << i ;
      DataSet viewData;
      Graph *graph;
      View *view =viewWidget[widgetList[i]];
      if(view){
        view->getData(&graph,&viewData);
        tmp.set<DataSet>(viewNames[view],viewData);
        tmp.set<unsigned int>("id",graph->getId());
        tmp.set<int>("x",rect.left());
        tmp.set<int>("y",rect.top());
        tmp.set<int>("width",rect.width());
        tmp.set<int>("height",rect.height());
        tmp.set<bool>("maximized", ((QWidget *)(widgetList[i]->parent()))->isMaximized());
        views.set<DataSet>(str.str(),tmp);
      }
    }
    dataSet->set<DataSet>("views",views);

    *graph=currentGraph;
  }
  //**********************************************************************
  Graph *MainController::getGraph() {
    return currentGraph;
  }
  //**********************************************************************
  void MainController::redrawViews(bool init) {
    Observable::holdObservers();
    eltProperties->updateTable();
    propertiesWidget->update();

    QList<QWidget *> widgetList=mainWindowFacade.getWorkspace()->windowList();
    for(QList<QWidget *>::iterator it=widgetList.begin();it!=widgetList.end();++it) {
      if(!init)
        viewWidget[*it]->draw();
      else
        viewWidget[*it]->init();
    }

    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::observableDestroyed(Observable *) {
    //cerr << "[WARNING]" << __PRETTY_FUNCTION__ << endl;
  }
  //**********************************************************************
  void MainController::update ( ObserverIterator begin, ObserverIterator end) {
    if(blockUpdate)
      return;

    blockUpdate=true;

    if(graphToReload){
      Graph *graph=graphToReload;
      graphToReload=NULL;
      //Update view of this graph
      for(map<View *,Graph* >::iterator it=viewGraph.begin();it!=viewGraph.end();++it){
        if((*it).second==graph){
          (*it).first->setGraph(graph);
        }
      }
      //Update children of this graph
      for(map<View *,Graph* >::iterator it=viewGraph.begin();it!=viewGraph.end();++it){
        Graph *parentGraph=(*it).second;
        while(parentGraph!=parentGraph->getRoot() && parentGraph!=graph){
          parentGraph=parentGraph->getSuperGraph();
          if(parentGraph==graph){
            (*it).first->setGraph((*it).second);
          }
        }
      }
    }else{
      redrawViews();
    }

    blockUpdate=false;

    if(currentGraph){
      currentGraphNbNodes=currentGraph->numberOfNodes();
      currentGraphNbEdges=currentGraph->numberOfEdges();
      updateCurrentGraphInfos();
    }

    updateUndoRedoInfos();
  }
  //**********************************************************************
  void MainController::initObservers() {
    if (currentGraph==0) return;
    Iterator<PropertyInterface*> *it =
      currentGraph->getObjectProperties();
    while (it->hasNext()) {
      PropertyInterface* tmp = it->next();
      tmp->addObserver(this);
    } delete it;
    currentGraphNbNodes=currentGraph->numberOfNodes();
    currentGraphNbEdges=currentGraph->numberOfEdges();
    updateCurrentGraphInfos();
  }
  //**********************************************************************
  void MainController::clearObservers() {
    if (currentGraph == 0) return;
    Iterator<PropertyInterface*> *it =
      currentGraph->getObjectProperties();
    while (it->hasNext()) {
      (it->next())->removeObserver(this);
    } delete it;
  }
  //**********************************************************************
  void MainController::addSubGraph(Graph *g, Graph *sg){
    if(currentGraph!=g)
      return;
    sg->addObserver(this);
  }
  //**********************************************************************
  void MainController::delSubGraph(Graph *g, Graph *sg){
    Iterator<Graph *> *itS=sg->getSubGraphs();
    while(itS->hasNext()) {
      Graph *subgraph = itS->next();
      delSubGraph(sg,subgraph);
    }
    currentGraph=g;
    for(map<View *,Graph *>::iterator it=viewGraph.begin();it!=viewGraph.end();++it){
      if((*it).second==sg){
        (*it).first->setGraph(g);
        (*it).second=g;
      }
    }
  }
  //**********************************************************************
  void  MainController::addLocalProperty(Graph *graph, const std::string&){
    graphToReload=graph;

    if(graph==currentGraph){
      eltProperties->setGraph(graph);
      propertiesWidget->setGraph(graph);
    }
  }
  //**********************************************************************
  void  MainController::delLocalProperty(Graph *graph, const std::string&){
    graphToReload=graph;

    if(graph==currentGraph){
      eltProperties->setGraph(graph);
      propertiesWidget->setGraph(graph);
    }
  }
  //**********************************************************************
  void MainController::loadGUI() {

  	mainWindowFacade.getWorkspace()->setScrollBarsEnabled( true );
    connect (mainWindowFacade.getWorkspace(), SIGNAL(windowActivated(QWidget *)), this, SLOT(windowActivated(QWidget *)));

    //+++++++++++++++++++++++++++
    //Create Data information editor (Hierarchy, Element info, Property Info)
    tabWidgetDock = new QDockWidget("Data manipulation", mainWindowFacade.getParentWidget());
    tabWidgetDock->hide();
    tabWidgetDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tabWidgetDock->setWindowTitle("Graph Editor");
    tabWidgetDock->setFeatures(QDockWidget::DockWidgetClosable |
			    QDockWidget::DockWidgetMovable |
			    QDockWidget::DockWidgetFloatable);
    TabWidget *tabWidget = new TabWidget(tabWidgetDock);
#ifndef STATS_UI
    // remove Statistics tab if not needed
    tabWidget->tabWidget2->removeTab(tabWidget->tabWidget2->indexOf(tabWidget->StatsTab));
#endif
    tabWidgetDock->setWidget(tabWidget);
    mainWindowFacade.addDockWidget(Qt::LeftDockWidgetArea, tabWidgetDock);
    tabWidget->show();
    tabWidgetDock->show();

    //+++++++++++++++++++++++++++
    //Init hierarchy visualization widget
    clusterTreeWidget=tabWidget->clusterTree;
      //connect signals related to graph replacement
    connect(clusterTreeWidget, SIGNAL(graphChanged(Graph *)),this, SLOT(changeGraph(Graph *)));
    connect(clusterTreeWidget, SIGNAL(aboutToRemoveView(Graph *)), this, SLOT(graphAboutToBeRemove(Graph *)));
    //Init Property Editor Widget
    propertiesWidget=tabWidget->propertyDialog;
    propertiesWidget->setGraph(NULL);
    connect(propertiesWidget->tableNodes, SIGNAL(showElementProperties(unsigned int,bool)),
	    this, SLOT(showElementProperties(unsigned int,bool)));
    connect(propertiesWidget->tableEdges, SIGNAL(showElementProperties(unsigned int,bool)),
	    this, SLOT(showElementProperties(unsigned int,bool)));
    //Init Element info widget
    eltProperties = tabWidget->elementInfo;
#ifdef STATS_UI
    //Init Statistics panel
    statsWidget = tabWidget->tulipStats;
    statsWidget->setSGHierarchyWidgetWidget(clusterTreeWidget);
#endif

    configWidgetDock = new QDockWidget("Data manipulation", mainWindowFacade.getParentWidget());
    configWidgetTab = new QTabWidget(configWidgetDock);

    //create no plugins configuration widget
    noInteractorConfigWidget = new QWidget(configWidgetTab);
    QGridLayout *gridLayout = new QGridLayout(noInteractorConfigWidget);
    QLabel *label = new QLabel(noInteractorConfigWidget);
    label->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(label, 0, 0, 1, 1);
    label->setText("No interactor configuration");

    configWidgetTab->addTab(noInteractorConfigWidget,"Interactor");
    configWidgetTab->setTabPosition(QTabWidget::West);
    configWidgetDock->setWidget(configWidgetTab);
    configWidgetDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    configWidgetDock->setWindowTitle("View Editor");
    configWidgetDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mainWindowFacade.addDockWidget(Qt::LeftDockWidgetArea, configWidgetDock);

    mainWindowFacade.tabifyDockWidget(tabWidgetDock,configWidgetDock);

    buildMenu();

    //+++++++++++++++++++++++++++
    //Connection of the menus
    //connect(mainWindowFacade.getInteractorsToolBar(), SIGNAL(actionTriggered(QAction *)), SLOT(changeInteractor(QAction*)));

  }
  //**********************************************************************
  void MainController::buildMenu() {
    QAction *tmpAction;

    //Search the Windows menu to add others menu before this menu
    QAction *windowAction=NULL;
    QList<QAction *> menuBarActions=mainWindowFacade.getMenuBar()->actions();
    for(QList<QAction *>::iterator it=menuBarActions.begin();it!=menuBarActions.end();++it) {
      if((*it)->text()=="&Windows")
        windowAction=*it;
    }
    assert(windowAction);

    editMenu = new QMenu("&Edit");
    editMenu->setEnabled(false);
    mainWindowFacade.getMenuBar()->insertMenu(windowAction,editMenu);

    tmpAction=editMenu->addAction("&Cut",this,SLOT(editCut()),QKeySequence(tr("Ctrl+X")));
    editMenu->addAction("C&opy",this,SLOT(editCopy()),QKeySequence(tr("Ctrl+C")));
    editMenu->addAction("&Paste",this,SLOT(editPaste()),QKeySequence(tr("Ctrl+V")));
    editMenu->addSeparator();
    editMenu->addAction("&Find...",this,SLOT(editFind()),QKeySequence(tr("Ctrl+F")));
    editMenu->addSeparator();
    editMenu->addAction("Select all",this,SLOT(editSelectAll()),QKeySequence(tr("Ctrl+A")));
    editMenu->addAction("Delete selection",this,SLOT(editDelSelection()),QKeySequence(tr("Del")));
    editMenu->addAction("Deselect all",this,SLOT(editDeselectAll()),QKeySequence(tr("Ctrl+Shift+A")));
    editMenu->addAction("Invert selection",this,SLOT(editReverseSelection()),QKeySequence(tr("Ctrl+I")));
    editMenu->addSeparator();
    editMenu->addAction("Create group",this,SLOT(editCreateGroup()),QKeySequence(tr("Ctrl+G")));
    editMenu->addAction("Create subgraph",this,SLOT(editCreateSubgraph()),QKeySequence(tr("Ctrl+Shift+G")));
    editMenu->addSeparator();
    editUndoAction=editMenu->addAction("Undo",this,SLOT(undo()),QKeySequence(tr("Ctrl+Z")));
    editUndoAction->setEnabled(false);
    editRedoAction=editMenu->addAction("Redo",this,SLOT(redo()),QKeySequence(tr("Ctrl+Y")));
    editRedoAction->setEnabled(false);

     //Algorithm Menu
    algorithmMenu = new QMenu("Algorithm");
    algorithmMenu->setEnabled(false);
    intMenu=new QMenu("&Integer");
    stringMenu=new QMenu("L&abel");
    sizesMenu=new QMenu("S&ize");
    colorsMenu=new QMenu("&Color");
    layoutMenu=new QMenu("&Layout");
    metricMenu=new QMenu("&Measure");
    selectMenu=new QMenu("&Selection");
    generalMenu=new QMenu("&General");

    buildPropertyMenu<IntegerType, IntegerType, IntegerAlgorithm>(*intMenu, this, SLOT(changeInt(QAction*)));
    buildPropertyMenu<StringType, StringType, StringAlgorithm>(*stringMenu, this, SLOT(changeString(QAction*)));
    buildPropertyMenu<SizeType, SizeType, SizeAlgorithm>(*sizesMenu, this, SLOT(changeSize(QAction*)));
    buildPropertyMenu<ColorType, ColorType, ColorAlgorithm>(*colorsMenu, this, SLOT(changeColor(QAction*)));
    buildPropertyMenu<PointType, LineType, LayoutAlgorithm>(*layoutMenu, this, SLOT(changeLayout(QAction*)));
    buildPropertyMenu<DoubleType, DoubleType, DoubleAlgorithm>(*metricMenu, this, SLOT(changeMetric(QAction*)));
    buildPropertyMenu<BooleanType, BooleanType, BooleanAlgorithm>(*selectMenu, this, SLOT(changeSelection(QAction*)));
    buildMenuWithContext<AlgorithmFactory, Algorithm>(*generalMenu, this, SLOT(applyAlgorithm(QAction*)));

    connect(stringMenu, SIGNAL(triggered(QAction*)), this, SLOT(changeString(QAction*)));
    connect(metricMenu, SIGNAL(triggered(QAction*)), this, SLOT(changeMetric(QAction*)));
    connect(layoutMenu, SIGNAL(triggered(QAction*)), this, SLOT(changeLayout(QAction*)));
    connect(selectMenu, SIGNAL(triggered(QAction*)), this, SLOT(changeSelection(QAction*)));
    connect(generalMenu, SIGNAL(triggered(QAction*)), this, SLOT(applyAlgorithm(QAction*)));
    connect(sizesMenu, SIGNAL(triggered(QAction*)), this, SLOT(changeSizes(QAction*)));
    connect(intMenu, SIGNAL(triggered(QAction*)), this, SLOT(changeInt(QAction*)));
    connect(colorsMenu, SIGNAL(triggered(QAction*)), this, SLOT(changeColors(QAction*)));

    if (selectMenu->actions().count()>0)
      algorithmMenu->addMenu(selectMenu);
    if (colorsMenu->actions().count()>0)
      algorithmMenu->addMenu(colorsMenu);
    if (metricMenu->actions().count()>0)
      algorithmMenu->addMenu(metricMenu);
    if (intMenu->actions().count()>0)
      algorithmMenu->addMenu(intMenu);
    if (layoutMenu->actions().count()>0)
      algorithmMenu->addMenu(layoutMenu);
    if (sizesMenu->actions().count()>0)
      algorithmMenu->addMenu(sizesMenu);
    if (stringMenu->actions().count()>0)
      algorithmMenu->addMenu(stringMenu);
    if (generalMenu->actions().count()>0)
      algorithmMenu->addMenu(generalMenu);
    mainWindowFacade.getMenuBar()->insertMenu(windowAction,algorithmMenu);


    //Graph menu
    graphMenu = new QMenu("&Graph");
    graphMenu->setEnabled(false);
    QMenu *testGraphMenu=graphMenu->addMenu("Test");
    tmpAction=testGraphMenu->addAction("Simple");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isSimple()));
    tmpAction=testGraphMenu->addAction("Directed Tree");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isTree()));
    tmpAction=testGraphMenu->addAction("Free Tree");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isFreeTree()));
    tmpAction=testGraphMenu->addAction("Acyclic");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isAcyclic()));
    tmpAction=testGraphMenu->addAction("Connected");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isConnected()));
    tmpAction=testGraphMenu->addAction("Biconnected");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isBiconnected()));
    tmpAction=testGraphMenu->addAction("Triconnected");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isTriconnected()));
    tmpAction=testGraphMenu->addAction("Planar");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isPlanar()));
    tmpAction=testGraphMenu->addAction("Outer Planar");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(isOuterPlanar()));

    QMenu *modifyGraphMenu=graphMenu->addMenu("&Modify");
    tmpAction=modifyGraphMenu->addAction("Make simple");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(makeSimple()));
    tmpAction=modifyGraphMenu->addAction("Make acyclic");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(makeAcyclic()));
    tmpAction=modifyGraphMenu->addAction("Make connected");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(makeConnected()));
    tmpAction=modifyGraphMenu->addAction("Make biconnected");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(makeBiconnected()));
    tmpAction=modifyGraphMenu->addAction("Make directed");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(makeDirected()));
    modifyGraphMenu->addSeparator();
    tmpAction=modifyGraphMenu->addAction("Reverse selected edges");
    connect(tmpAction,SIGNAL(triggered()),this,SLOT(reverseSelectedEdgeDirection()));

    mainWindowFacade.getMenuBar()->insertMenu(windowAction,graphMenu);


    //View menu
    viewMenu = new QMenu("View");
    viewMenu->setEnabled(false);
    connect(viewMenu, SIGNAL(triggered(QAction *)), SLOT(addView(QAction*)));
    TemplateFactory<ViewFactory, View, ViewContext>::ObjectCreator::const_iterator it;
    for (it=ViewFactory::factory->objMap.begin();it != ViewFactory::factory->objMap.end();++it) {
      viewMenu->addAction(it->first.c_str());
    }
    mainWindowFacade.getMenuBar()->insertMenu(windowAction,viewMenu);

    //Options menu
    optionsMenu = new QMenu("&Options");
    optionsMenu->setEnabled(false);
    forceRatioAction = optionsMenu->addAction("Force ratio");
    forceRatioAction->setCheckable(true);
    forceRatioAction->setChecked(false);
    mapMetricAction = optionsMenu->addAction("Map metric");
    mapMetricAction->setCheckable(true);
    mapMetricAction->setChecked(true);
    morphingAction = optionsMenu->addAction("Morphing");
    morphingAction->setCheckable(true);
    morphingAction->setChecked(false);
    optionsMenu->addSeparator();
    QAction *propertiesDockAction = optionsMenu->addAction("Show graph editor");
    connect(propertiesDockAction,SIGNAL(triggered()),tabWidgetDock,SLOT(show()));
    QAction *configurationDockAction = optionsMenu->addAction("Show view editor");
    connect(configurationDockAction,SIGNAL(triggered()),configWidgetDock,SLOT(show()));
    mainWindowFacade.getMenuBar()->insertMenu(windowAction,optionsMenu);

    redoAction=new QAction(QIcon(":/i_redo.png"),"redo",mainWindowFacade.getParentWidget());
    undoAction=new QAction(QIcon(":/i_undo.png"),"undo",mainWindowFacade.getParentWidget());
    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    mainWindowFacade.getToolBar()->addAction(undoAction);
    mainWindowFacade.getToolBar()->addAction(redoAction);
    connect(undoAction,SIGNAL(triggered()),this,SLOT(undo()));
    connect(redoAction,SIGNAL(triggered()),this,SLOT(redo()));
  }
  //**********************************************************************
  View* MainController::initMainView(DataSet dataSet) {
    View* newView=createView("Node Link Diagram view",currentGraph,dataSet);
    return newView;
  }
  //**********************************************************************
  View* MainController::createView(const string &name,Graph *graph,DataSet dataSet,const QRect &rect,bool maximized){
    string verifiedName=name;
    View *newView=ViewPluginsManager::getInst().createView(name);

    if(!newView){
      verifiedName="Node Link Diagram view";
      newView=ViewPluginsManager::getInst().createView("Node Link Diagram view");
    }

    multimap<int,string> interactorsNamesAndPriorityMap;

    if(newView->getRealViewName()=="")
      interactorsNamesAndPriorityMap=InteractorManager::getInst().getSortedCompatibleInteractors(verifiedName);
    else
      interactorsNamesAndPriorityMap=InteractorManager::getInst().getSortedCompatibleInteractors(newView->getRealViewName());

    list<Interactor *> interactorsList;
    for(multimap<int,string>::reverse_iterator it=interactorsNamesAndPriorityMap.rbegin();it!=interactorsNamesAndPriorityMap.rend();++it){
      interactorsList.push_back(InteractorManager::getInst().getInteractor((*it).second));
    }
    newView->setInteractors(interactorsList);

    QWidget *widget;
    widget=newView->construct(mainWindowFacade.getWorkspace());
    newView->setData(graph,dataSet);
    widget->setObjectName(QString("ViewMainWidget p:")+QString::number((long)widget));

    viewGraph[newView]=graph;
    viewNames[newView]=verifiedName;
    viewWidget[widget]=newView;



    widget->setAttribute(Qt::WA_DeleteOnClose,true);
    mainWindowFacade.getWorkspace()->addWindow(widget);

    connect(newView, SIGNAL(elementSelected(unsigned int, bool)),this,SLOT(showElementProperties(unsigned int, bool)));
    connect(newView, SIGNAL(requestChangeGraph(View *,Graph *)), this, SLOT(viewRequestChangeGraph(View *,Graph *)));
    connect(widget, SIGNAL(destroyed(QObject *)),this, SLOT(widgetWillBeClosed(QObject *)));

    string windowTitle=verifiedName +" : " + graph->getAttribute<string>("name");
    widget->setWindowTitle(windowTitle.c_str());
    if(rect.width()==0 && rect.height()==0){
      QRect newRect;
      if(widget->size().height()<10 || widget->size().width()<10){
        newRect = QRect(QPoint((viewWidget.size()-1)*20,(viewWidget.size()-1)*20),QSize(500,500));
      }else{
        newRect = QRect(QPoint((viewWidget.size()-1)*20,(viewWidget.size()-1)*20),widget->size());
      }
      ((QWidget*)(widget->parent()))->setGeometry(newRect);
    }else{
      ((QWidget*)(widget->parent()))->setGeometry(rect);
    }

    if(maximized)
      ((QWidget*)(widget->parent()))->showMaximized();

    widget->setMaximumSize(32767, 32767);
    widget->show();

    windowActivated(widget);

    return newView;
  }
  //**********************************************************************
  void MainController::windowActivated(QWidget *w) {
    //check if a view is close
    QWidgetList widgets=mainWindowFacade.getWorkspace()->windowList();

    std::map<QWidget *,View*>::iterator it=viewWidget.find(w);
    if(it!=viewWidget.end()) {

      lastConfigTabIndexOnView[currentView]=configWidgetTab->currentIndex();
      while(configWidgetTab->count()>1){
	configWidgetTab->removeTab(1);
      }
      if(configWidgetTab->widget(0)!=noInteractorConfigWidget) {
	configWidgetTab->removeTab(0);
	configWidgetTab->addTab(noInteractorConfigWidget,"Interactor");
      }

      View *view=(*it).second;

      currentView=view;
      currentGraph=currentView->getGraph();
      installInteractors(view);
      //installEditMenu(view);
      clusterTreeWidget->setGraph(currentGraph);
      eltProperties->setGraph(currentGraph);
      propertiesWidget->setGraph(currentGraph);

      list<pair<QWidget *,string> > configWidgetsList=view->getConfigurationWidget();
      for(list<pair<QWidget *,string> >::iterator it=configWidgetsList.begin();it!=configWidgetsList.end();++it){
	configWidgetTab->addTab((*it).first,(*it).second.c_str());
      }
      if(lastConfigTabIndexOnView.count(currentView)!=0)
	configWidgetTab->setCurrentIndex(lastConfigTabIndexOnView[currentView]);

      //Remove observer (nothing if this not observe)
      currentGraph->removeGraphObserver(this);
      currentGraph->removeObserver(this);
      //Add observer
      currentGraph->addGraphObserver(this);
      currentGraph->addObserver(this);
    }
  }
  //**********************************************************************
  void MainController::changeGraph(Graph *graph) {
    if(currentGraph==graph)
      return;
    if(!currentView)
      return;

    clearObservers();

    currentGraph=graph;
    viewGraph[currentView]=graph;

    clusterTreeWidget->setGraph(graph);
    eltProperties->setGraph(graph);
    propertiesWidget->setGraph(graph);
    currentView->setGraph(graph);

    QWidget *widget;
    for(map<QWidget *,View *>::iterator it=viewWidget.begin();it!=viewWidget.end();++it){
      if((*it).second==currentView)
        widget=(*it).first;
    }
    string windowTitle=viewNames[currentView] +" : "+graph->getAttribute<string>("name");
    widget->setWindowTitle(windowTitle.c_str());

    updateUndoRedoInfos();

    initObservers();
    //Remove observer (nothing if this not observe)
    currentGraph->removeGraphObserver(this);
    currentGraph->removeObserver(this);
    //Add observer
    currentGraph->addGraphObserver(this);
    currentGraph->addObserver(this);
  }
  //**********************************************************************
   void MainController::graphAboutToBeRemove(Graph *graph){
     currentGraph=NULL;
   }
  //**********************************************************************
  void MainController::installInteractors(View *view) {
    //remove connection of old actions
    QList<QAction *> oldActions=mainWindowFacade.getInteractorsToolBar()->actions();
    for(QList<QAction *>::iterator it=oldActions.begin();it!=oldActions.end();++it){
      disconnect((*it),SIGNAL(triggered()),this,SLOT(changeInteractor()));
    }

    mainWindowFacade.getInteractorsToolBar()->clear();


    list<Interactor *> interactorsList=view->getInteractors();
    list<QAction *> interactorsActionList;
    for(list<Interactor *>::iterator it=interactorsList.begin();it!=interactorsList.end();++it)
      interactorsActionList.push_back((*it)->getAction());

    for(list<QAction *>::iterator it=interactorsActionList.begin();it!=interactorsActionList.end();++it) {
      mainWindowFacade.getInteractorsToolBar()->addAction(*it);
      //connect action with change interactor slot
      connect((*it),SIGNAL(triggered()),this,SLOT(changeInteractor()));
    }

    if(!interactorsActionList.empty()) {
      map<View*,QAction *>::iterator it=lastInteractorOnView.find(view);
      if(it!=lastInteractorOnView.end()){
        if(mainWindowFacade.getInteractorsToolBar()->actions().contains((*it).second)){
          changeInteractor((*it).second);
          return;
        }
      }

      changeInteractor(interactorsActionList.front());
    }
  }
  //**********************************************************************
  void MainController::changeInteractor() {
    //Get QAction who emit the signal (sett QObject protected functions)
    QAction *action=(QAction*)sender();
    changeInteractor(action);
  }
  //**********************************************************************
  void MainController::changeInteractor(QAction* action) {
    if(currentView){
      QList<QAction*> actions=mainWindowFacade.getInteractorsToolBar()->actions();
      for(QList<QAction*>::iterator it=actions.begin();it!=actions.end();++it) {
        (*it)->setChecked(false);
      }
      action->setCheckable(true);
      action->setChecked(true);
      InteractorAction *interactorAction=(InteractorAction *)action;
      currentView->setActiveInteractor(interactorAction->getInteractor());
      lastInteractorOnView[currentView]=action;
      QWidget *interactorWidget=interactorAction->getInteractor()->getConfigurationWidget();
      bool onInteractorConfigTab=configWidgetTab->currentIndex()==0;
      configWidgetTab->removeTab(0);
      QWidget *containerWidget=new QWidget();
      QGridLayout *gridLayout = new QGridLayout(containerWidget);
      gridLayout->setSpacing(0);
      gridLayout->setMargin(0);
      if(interactorWidget){
        gridLayout->addWidget(interactorWidget,0,0);
      }else{
        gridLayout->addWidget(noInteractorConfigWidget,0,0);
      }

      configWidgetTab->insertTab(0,containerWidget,"Interactor");

      if(onInteractorConfigTab)
        configWidgetTab->setCurrentIndex(0);

      currentView->refresh();
    }
  }
  //**********************************************************************
  void MainController::addView(QAction *action) {
    createView(action->text().toStdString(),currentGraph,DataSet());
  }
  //==================================================
  void MainController::showElementProperties(unsigned int eltId, bool isNode) {
    if (isNode)
      eltProperties->setCurrentNode(currentGraph,  tlp::node(eltId));
    else
      eltProperties->setCurrentEdge(currentGraph,  tlp::edge(eltId));
    // show 'Element' tab in 'Info Editor'
    QWidget *tab = eltProperties->parentWidget();
    QTabWidget *tabWidget = (QTabWidget *) tab->parentWidget()->parentWidget();
    tabWidget->setCurrentIndex(tabWidget->indexOf(tab));
  }
  //==================================================
  void MainController::viewRequestChangeGraph(View *view,Graph *graph) {
    assert(view==currentView);
    changeGraph(graph);
  }
  //==================================================
  void MainController::widgetWillBeClosed(QObject *object) {
    QWidget *widget=(QWidget*)object;
    View *view=viewWidget[widget];
    delete viewWidget[widget];
    viewWidget.erase(widget);
    viewNames.erase(view);
    lastInteractorOnView.erase(view);
    viewGraph.erase(view);
    if(viewWidget.size()==0){
      mainWindowFacade.getInteractorsToolBar()->clear();
      currentView=NULL;
      emit willBeClosed();
    }
  }
  //**********************************************************************
  void MainController::updateCurrentGraphInfos() {
    static QLabel *currentGraphInfosLabel = 0;
    if (!currentGraphInfosLabel) {
      //mainWindowFacade.getStatusBar()->addWidget(new QLabel(mainWindowFacade.getStatusBar()), true);
      currentGraphInfosLabel = new QLabel(mainWindowFacade.getStatusBar());
      mainWindowFacade.getStatusBar()->addPermanentWidget(currentGraphInfosLabel);
    }

    char tmp[255];
    sprintf(tmp,"nodes:%d, edges:%d", currentGraphNbNodes, currentGraphNbEdges);
    currentGraphInfosLabel->setText(tmp);
    clusterTreeWidget->updateCurrentGraphInfos(currentGraphNbNodes, currentGraphNbEdges);
  }
  //==============================================================
  void MainController::editCut() {
    if( !currentGraph )
    	return;
    // free the previous ccpGraph
    if( copyCutPasteGraph ) {
      delete copyCutPasteGraph;
      copyCutPasteGraph = 0;
    }
    BooleanProperty * selP = currentGraph->getProperty<BooleanProperty>("viewSelection");
    if( !selP ) return;
    // Save selection
    NodeA nodeA;
    EdgeA edgeA;
    GetSelection( nodeA, edgeA, currentGraph, selP );
    Observable::holdObservers();
    Graph* newGraph = tlp::newGraph();
    tlp::copyToGraph( newGraph, currentGraph, selP );
    stringstream tmpss;
    DataSet dataSet;
    tlp::exportGraph(newGraph, tmpss, "tlp", dataSet, NULL);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmpss.str().c_str());
    currentGraph->push();
    // Restore selection
    SetSelection( selP, nodeA, edgeA, currentGraph );
    tlp::removeFromGraph( currentGraph, selP );
    Observable::unholdObservers();
    redrawViews(true);
  }
  //==============================================================
  void MainController::editCopy() {
    if( !currentGraph ) return;
    // free the previous ccpGraph
    if( copyCutPasteGraph ) {
      delete copyCutPasteGraph;
      copyCutPasteGraph = 0;
    }
    BooleanProperty * selP = currentGraph->getProperty<BooleanProperty>("viewSelection");
    if( !selP ) return;
    Observable::holdObservers();
    Graph* newGraph = tlp::newGraph();
    tlp::copyToGraph( newGraph, currentGraph, selP );
    stringstream tmpss;
    DataSet dataSet;
    tlp::exportGraph(newGraph, tmpss, "tlp", dataSet, NULL);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmpss.str().c_str());
    Observable::unholdObservers();
  }
  //==============================================================
  void MainController::editPaste() {
    if( !currentGraph ) return;

    currentGraph->removeObserver(this);
    Observable::holdObservers();
    BooleanProperty * selP = currentGraph->getProperty<BooleanProperty>("viewSelection");

    currentGraph->push();
    Graph *newGraph=tlp::newGraph();
    DataSet dataSet;
    QClipboard *clipboard = QApplication::clipboard();
    dataSet.set<string>("file::data",clipboard->text().toStdString());
    tlp::importGraph("tlp", dataSet, NULL ,newGraph);
    tlp::copyToGraph( currentGraph, newGraph, 0, selP );
    Observable::unholdObservers();
    currentGraph->addObserver(this);
    currentGraphNbNodes=currentGraph->numberOfNodes();
    currentGraphNbEdges=currentGraph->numberOfEdges();
    updateCurrentGraphInfos();

    redrawViews(true);
  }
  //==============================================================
  void MainController::editFind() {
    if( !currentGraph ) return;

    static string currentProperty;
    FindSelectionWidget *sel = new FindSelectionWidget(currentGraph, currentProperty, mainWindowFacade.getParentWidget());
    Observable::holdObservers();
    int nbItemsFound = sel->exec();
    Observable::unholdObservers();
    if (nbItemsFound > - 1)
      currentProperty = sel->getCurrentProperty();
    delete sel;
    switch(nbItemsFound) {
    case -1: break;
    case 0: mainWindowFacade.getStatusBar()->showMessage("No item found."); break;
    default:
      stringstream sstr;
      sstr << nbItemsFound << " item(s) found.";
      mainWindowFacade.getStatusBar()->showMessage(sstr.str().c_str());
    }
  }
  //==============================================================
  void MainController::editCreateGroup() {
    set<node> tmp;
    Iterator<node> *it=currentGraph->getNodes();
    BooleanProperty *select = currentGraph->getProperty<BooleanProperty>("viewSelection");
    while (it->hasNext()) {
      node itn = it->next();
      if (select->getNodeValue(itn))
        tmp.insert(itn);
    }delete it;
    if (tmp.empty()) return;
    currentGraph->push();
    Observable::holdObservers();
    bool haveToChangeGraph=false;
    Graph *graphToAddTo=currentGraph;
    if (graphToAddTo == graphToAddTo->getRoot()) {
      QMessageBox::critical( 0, "Warning" ,"Grouping can't be done on the root graph, a subgraph will be created");
      graphToAddTo = tlp::newCloneSubGraph(graphToAddTo, "groups");
      haveToChangeGraph=true;
    }
    node metaNode = graphToAddTo->createMetaNode(tmp);
    if(haveToChangeGraph)
      changeGraph(graphToAddTo);
    Observable::unholdObservers();
    clusterTreeWidget->update();
  }
  //==============================================================
  void MainController::editCreateSubgraph() {
    if (currentGraph==0) return;
    bool ok = FALSE;
    string tmp;
    bool verifGraph = true;
    BooleanProperty *sel1 = currentGraph->getProperty<BooleanProperty>("viewSelection");
    Observable::holdObservers();
    Iterator<edge>*itE = currentGraph->getEdges();
    while (itE->hasNext()) {
      edge ite= itE->next();
      if (sel1->getEdgeValue(ite)) {
        if (!sel1->getNodeValue(currentGraph->source(ite))) {sel1->setNodeValue(currentGraph->source(ite),true); verifGraph=false;}
        if (!sel1->getNodeValue(currentGraph->target(ite))) {sel1->setNodeValue(currentGraph->target(ite),true); verifGraph=false;}
      }
    } delete itE;
    Observable::unholdObservers();

    if(!verifGraph)
      QMessageBox::critical( 0, "Tulip Warning" ,"The selection wasn't a graph, missing nodes have been added");
    QString text = QInputDialog::getText(mainWindowFacade.getParentWidget(),
        "Creation of subgraph" ,
        "Please enter the subgraph name" ,
        QLineEdit::Normal, QString::null, &ok);
    if (ok && !text.isEmpty()) {
      sel1 = currentGraph->getProperty<BooleanProperty>("viewSelection");
      currentGraph->push();
      Graph *tmp = currentGraph->addSubGraph(sel1);
      tmp->setAttribute("name",string(text.toAscii().data()));
      clusterTreeWidget->update();
      //emit clusterTreeNeedUpdate();
    }
    else if (ok) {
      sel1 = currentGraph->getProperty<BooleanProperty>("viewSelection");
      currentGraph->push();
      Graph *tmp=currentGraph->addSubGraph(sel1);
      tmp->setAttribute("name", newName());
      clusterTreeWidget->update();
      //emit clusterTreeNeedUpdate();
    }
  }
  //==============================================================
  void MainController::editDelSelection() {
    if (currentGraph==0) return;
    currentGraph->push();
    currentGraph->removeObserver(this);
    Observable::holdObservers();
    BooleanProperty *elementSelected=currentGraph->getProperty<BooleanProperty>("viewSelection");
    StableIterator<node> itN(currentGraph->getNodes());
    while(itN.hasNext()) {
      node itv = itN.next();
      if (elementSelected->getNodeValue(itv)==true)
        currentGraph->delNode(itv);
    }
    StableIterator<edge> itE(currentGraph->getEdges());
    while(itE.hasNext()) {
      edge ite=itE.next();
      if (elementSelected->getEdgeValue(ite)==true)
        currentGraph->delEdge(ite);
    }
    Observable::unholdObservers();
    currentGraph->addObserver(this);
    currentGraphNbNodes=currentGraph->numberOfNodes();
    currentGraphNbEdges=currentGraph->numberOfEdges();
    updateCurrentGraphInfos();

    redrawViews(true);
  }
  //==============================================================
  void MainController::editReverseSelection() {
    if (currentGraph==0) return;
    currentGraph->push();
    Observable::holdObservers();
    currentGraph->getProperty<BooleanProperty>("viewSelection")->reverse();
    Observable::unholdObservers();
  }
  //==============================================================
  void MainController::editSelectAll() {
    if (currentGraph==0) return;
    currentGraph->push();
    Observable::holdObservers();
    currentGraph->getProperty<BooleanProperty>("viewSelection")->setAllNodeValue(true);
    currentGraph->getProperty<BooleanProperty>("viewSelection")->setAllEdgeValue(true);
    Observable::unholdObservers();
  }
  //==============================================================
  void MainController::editDeselectAll() {
    if (currentGraph==0) return;
    currentGraph->push();
    Observable::holdObservers();
    currentGraph->getProperty<BooleanProperty>("viewSelection")->setAllNodeValue(false);
    currentGraph->getProperty<BooleanProperty>("viewSelection")->setAllEdgeValue(false);
    Observable::unholdObservers();
  }
  //**********************************************************************
  /// Apply a general algorithm
  void MainController::applyAlgorithm(QAction* action) {
    Observable::holdObservers();
    string name = action->text().toStdString();
    string erreurMsg;
    DataSet dataSet;
    Graph *graph=currentGraph;
    StructDef *params = getPluginParameters(AlgorithmFactory::factory, name);
    StructDef sysDef = AlgorithmFactory::factory->getPluginParameters(name);
    params->buildDefaultDataSet(dataSet, graph );
    bool ok = tlp::openDataSetDialog(dataSet, &sysDef, params, &dataSet,
				     "Tulip Parameter Editor", graph, mainWindowFacade.getParentWidget());
    if (ok) {
      QtProgress myProgress(mainWindowFacade.getParentWidget(),name);
      myProgress.hide();
      graph->push();
      if (!tlp::applyAlgorithm(graph, erreurMsg, &dataSet, name, &myProgress  )) {
        QMessageBox::critical( 0, "Tulip Algorithm Check Failed",QString((name + ":\n" + erreurMsg).c_str()));
        graph->pop();
      }
      undoAction->setEnabled(graph->canPop());
      editUndoAction->setEnabled(graph->canPop());
      clusterTreeWidget->update();
      clusterTreeWidget->setGraph(graph);
    }
    Observable::unholdObservers();
    redrawViews(true);
  }
  //**********************************************************************
  //Management of properties
  //**********************************************************************
  template<typename PROPERTY>
  bool MainController::changeProperty(string name, string destination, bool query, bool redraw, bool push) {
    /*if( !glWidget ) return false;*/
    Graph *graph = currentGraph;
    if(graph == 0) return false;
    Observable::holdObservers();
    /*overviewWidget->setObservedView(0);*/
    GlGraphRenderingParameters param;
    QtProgress *myProgress=new QtProgress(mainWindowFacade.getParentWidget(), name,redraw ? currentView : 0);

    string erreurMsg;
    bool   resultBool=true;
    DataSet dataSet;
    if (query) {
      StructDef *params = getPluginParameters(PROPERTY::factory, name);
      StructDef sysDef = PROPERTY::factory->getPluginParameters(name);
      params->buildDefaultDataSet(dataSet, graph );
      resultBool = tlp::openDataSetDialog(dataSet, &sysDef, params, &dataSet,
					  "Tulip Parameter Editor", graph, mainWindowFacade.getParentWidget());
    }

    if (resultBool) {
      PROPERTY* tmp = new PROPERTY(graph);
      if (push)
	graph->push();
      // must be done after push because destination property
      // may not exist and thus the getLocalProperty call may create it
      // and so it must be deleted at pop time
      PROPERTY* dest = graph->template getLocalProperty<PROPERTY>(destination);
      tmp->setAllNodeValue(dest->getNodeDefaultValue());
      tmp->setAllEdgeValue(dest->getEdgeDefaultValue());
      graph->push(false);
      bool updateLayout = (typeid(PROPERTY) == typeid(LayoutProperty) &&
			   viewNames[currentView]=="Node Link Diagram view");
      if (updateLayout) {
	graph->setAttribute("viewLayout", tmp);
	((NodeLinkDiagramComponent*)currentView)->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->reloadLayoutProperty();
      }
      resultBool = currentGraph->computeProperty(name, tmp, erreurMsg, myProgress, &dataSet);
      bool hasNdldc = false;
      DataSet nodeLinkDiagramComponentDataSet;
      if ((typeid(PROPERTY) == typeid(ColorProperty) &&
	   viewNames[currentView]=="Node Link Diagram view"))
	hasNdldc = graph->getAttribute("NodeLinkDiagramComponent", nodeLinkDiagramComponentDataSet);
      graph->pop();
      if (updateLayout) {
	graph->removeAttribute("viewLayout");
	((NodeLinkDiagramComponent*)currentView)->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->reloadLayoutProperty();
      }
      if (!resultBool) {
        QMessageBox::critical(mainWindowFacade.getParentWidget(), "Tulip Algorithm Check Failed", QString((name + ":\n" + erreurMsg).c_str()) );
	graph->pop();
      }
      else
        switch(myProgress->state()){
        case TLP_CONTINUE:
        case TLP_STOP:
          if (push) {
	    undoAction->setEnabled(true);
            editUndoAction->setEnabled(true);
          }
	  if (hasNdldc)
	    graph->setAttribute("NodeLinkDiagramComponent", nodeLinkDiagramComponentDataSet);
	  *dest = *tmp;

          break;
        case TLP_CANCEL:
          resultBool=false;
        };
      delete tmp;
    }

    propertiesWidget->setGraph(graph);
    /*overviewWidget->setObservedView(glWidget);*/
    Observable::unholdObservers();
    delete myProgress;
    return resultBool;
  }
  //**********************************************************************
  void MainController::changeString(QAction* action) {
    string name = action->text().toStdString();
    if (changeProperty<StringProperty>(name,"viewLabel"))
      redrawViews(true);
  }
  //**********************************************************************
  void MainController::changeSelection(QAction* action) {
    string name = action->text().toStdString();
    if (changeProperty<BooleanProperty>(name, "viewSelection")) {
      redrawViews();
    }
  }
  //**********************************************************************
  void MainController::changeMetric(QAction* action) {
    string name = action->text().toStdString();
    bool result = changeProperty<DoubleProperty>(name,"viewMetric", true);
    if (result && mapMetricAction->isChecked()) {
      if (changeProperty<ColorProperty>("Metric Mapping","viewColor", false,true, false))
        redrawViews(true);
    }
  }
  //**********************************************************************
  void MainController::changeLayout(QAction* action) {
    string name = action->text().toStdString();
    GraphState * g0 = 0;
    GlMainView *mainView=dynamic_cast<GlMainView *>(currentView);
    if( morphingAction->isChecked() && mainView!=NULL)
      g0 = new GraphState(mainView->getGlMainWidget());

    bool result = changeProperty<LayoutProperty>(name, "viewLayout", true, true);
    if (result) {
      if( forceRatioAction->isChecked() )
        currentGraph->getLocalProperty<LayoutProperty>("viewLayout")->perfectAspectRatio();

      if( morphingAction->isChecked() && mainView!=NULL) {
        clearObservers();
        mainView->getGlMainWidget()->getScene()->centerScene();
        GraphState * g1 = new GraphState(mainView->getGlMainWidget());
        bool morphable = morph->init(mainView->getGlMainWidget(), g0, g1);
        if( !morphable ) {
          delete g1;
          g1 = 0;
        } else {
          morph->start(mainView->getGlMainWidget());
          g0 = 0;	// state remains in morph data ...
        }
        initObservers();
      }
    }
    redrawViews(true);
    if( g0 )
      delete g0;
  }
  //**********************************************************************
  void MainController::changeInt(QAction* action) {
    string name = action->text().toStdString();
    changeProperty<IntegerProperty>(name, "viewInt");
    redrawViews(true);
  }
  //**********************************************************************
  void MainController::changeColors(QAction* action) {
    GraphState * g0 = 0;
    GlMainView *mainView=dynamic_cast<GlMainView *>(currentView);
    if( morphingAction->isChecked() && mainView!=NULL)
      g0 = new GraphState(mainView->getGlMainWidget());
    string name = action->text().toStdString();
    bool result = changeProperty<ColorProperty>(name,"viewColor");
    if( result ) {
      if( morphingAction->isChecked() && mainView!=NULL) {
        clearObservers();
        mainView->getGlMainWidget()->getScene()->centerScene();
        GraphState * g1 = new GraphState( mainView->getGlMainWidget() );
        bool morphable = morph->init( mainView->getGlMainWidget(), g0, g1);
        if( !morphable ) {
          delete g1;
          g1 = 0;
        } else {
          morph->start(mainView->getGlMainWidget());
          g0 = 0;	// state remains in morph data ...
        }
        initObservers();
      }
      redrawViews(true);
    }
    if( g0 )
      delete g0;
  }
  //**********************************************************************
  void MainController::changeSizes(QAction* action) {
    GraphState * g0 = 0;
    GlMainView *mainView=dynamic_cast<GlMainView *>(currentView);
    if( morphingAction->isChecked() && mainView!=NULL)
      g0 = new GraphState(mainView->getGlMainWidget());
    string name = action->text().toStdString();
    bool result = changeProperty<SizeProperty>(name,"viewSize");
    if( result ) {
      if( morphingAction->isChecked() && mainView!=NULL) {
        clearObservers();
        mainView->getGlMainWidget()->getScene()->centerScene();
        GraphState * g1 = new GraphState( mainView->getGlMainWidget() );
        bool morphable = morph->init( mainView->getGlMainWidget(), g0, g1);
        if( !morphable ) {
          delete g1;
          g1 = 0;
        } else {
          morph->start(mainView->getGlMainWidget());
          g0 = 0; // state remains in morph data ...
        }
        initObservers();
      }
      redrawViews(true);
    }
    if( g0 )
      delete g0;
  }
  //**********************************************************************
  void MainController::isAcyclic() {
    if (AcyclicTest::isAcyclic(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is acyclic"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not acyclic"
				);
  }
  void MainController::makeAcyclic() {
    Observable::holdObservers();
    vector<tlp::SelfLoops> tmpSelf;
    vector<edge> tmpReversed;
    currentGraph->push();
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);

    AcyclicTest::makeAcyclic(currentGraph, tmpReversed, tmpSelf);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::isSimple() {
    //if (glWidget == 0) return;
    if (SimpleTest::isSimple(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is simple"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not simple"
				);
  }
  void MainController::makeSimple() {
    Observable::holdObservers();
    vector<edge> removed;
    currentGraph->push();
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);

    SimpleTest::makeSimple(currentGraph, removed);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::isConnected() {
    //if (glWidget == 0) return;
    if (ConnectedTest::isConnected(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is connected"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not connected"
				);
  }
  void MainController::makeConnected() {
    Observable::holdObservers();
    vector<edge> tmp;

    currentGraph->push();
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);

    ConnectedTest::makeConnected(currentGraph, tmp);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::isBiconnected() {
    //if (glWidget == 0) return;
    if (BiconnectedTest::isBiconnected(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is biconnected"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not biconnected"
				);
  }
  void MainController::makeBiconnected() {
    Observable::holdObservers();
    vector<edge> tmp;
    currentGraph->push();
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);

    BiconnectedTest::makeBiconnected(currentGraph, tmp);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::isTriconnected() {
    //if (glWidget == 0) return;
    if (TriconnectedTest::isTriconnected(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is triconnected"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not triconnected"
				);
  }
  //**********************************************************************
  void MainController::isTree() {
    //if (glWidget == 0) return;
    if (TreeTest::isTree(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is a directed tree"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not a directed tree"
				);
  }
  //**********************************************************************
  void MainController::isFreeTree() {
    //if (glWidget == 0) return;
    if (TreeTest::isFreeTree(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is a free tree"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not a free tree"
				);
  }
  void MainController::makeDirected() {
    if (!TreeTest::isFreeTree(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test","The graph is not a free tree");
    node n, root;
    forEach(n, currentGraph->getProperty<BooleanProperty>("viewSelection")->getNodesEqualTo(true)) {
      if (root.isValid()) {
        QMessageBox::critical( mainWindowFacade.getParentWidget(), "Make Rooted","Only one root node must be selected.");
        breakForEach;
      }
      root = n;
    }
    if (!root.isValid())
      root = graphCenterHeuristic(currentGraph);

    Observable::holdObservers();

    currentGraph->push();
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);

    TreeTest::makeRootedTree(currentGraph, root);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::isPlanar() {
    //if (glWidget == 0) return;
    Observable::holdObservers();
    if (PlanarityTest::isPlanar(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is planar"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not planar"
				);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::isOuterPlanar() {
    //if (glWidget == 0) return;
    Observable::holdObservers();
    if (OuterPlanarTest::isOuterPlanar(currentGraph))
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is outer planar"
				);
    else
      QMessageBox::information( mainWindowFacade.getParentWidget(), "Tulip test",
				"The graph is not outer planar"
				);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::reverseSelectedEdgeDirection() {
    Observable::holdObservers();
    currentGraph->push();
    currentGraph->getProperty<BooleanProperty>("viewSelection")->reverseEdgeDirection();
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::updateUndoRedoInfos() {
    if(!currentGraph)
      return;

    undoAction->setEnabled(currentGraph->canPop());
    redoAction->setEnabled(currentGraph->canUnpop());
    editUndoAction->setEnabled(currentGraph->canPop());
    editRedoAction->setEnabled(currentGraph->canUnpop());
  }
  //**********************************************************************
  map<View *,list<int> > MainController::saveViewsHierarchiesBeforePop() {
    map<View *,list<int> > hierarchies;
    for(map<View *,Graph* >::iterator itView=viewGraph.begin();itView!=viewGraph.end();++itView){
      hierarchies[(*itView).first]=list<int>();
      Graph *father=(*itView).second;
      while(father!=father->getSuperGraph()){
        hierarchies[(*itView).first].push_back(father->getId());
        father=father->getSuperGraph();
      }
      hierarchies[(*itView).first].push_back(father->getId());
    }
    return hierarchies;
  }
  //**********************************************************************
  void MainController::checkViewsHierarchiesAfterPop(map<View *,list<int> > &hierarchies){
    for(map<View *,Graph* >::iterator itView=viewGraph.begin();itView!=viewGraph.end();++itView){
      Graph *newGraph;
      for(list<int>::iterator it=hierarchies[(*itView).first].begin();it!=hierarchies[(*itView).first].end();++it){
        newGraph=currentGraph->getRoot()->getDescendantGraph(*it);
        if(!newGraph && (currentGraph->getRoot()->getId()==(*it)))
          newGraph=currentGraph->getRoot();
        if(newGraph)
          break;
      }

      if(newGraph!=(*itView).second){
        (*itView).first->setGraph(newGraph);
        viewGraph[(*itView).first]=newGraph;
      }
    }
  }
  //**********************************************************************
  void MainController::undo() {
    map<View *,list<int> > hierarchies=saveViewsHierarchiesBeforePop();

    Graph *root=currentGraph->getRoot();
    blockUpdate=true;
    root->pop();
    blockUpdate=false;

    checkViewsHierarchiesAfterPop(hierarchies);
    Graph *newGraph=viewGraph[currentView];

    changeGraph(newGraph);
    // force clusterTreeWidget to update
    clusterTreeWidget->update();
    propertiesWidget->setGraph(newGraph);
    eltProperties->setGraph(newGraph,false);

    redrawViews();
    updateUndoRedoInfos();

  }
  //**********************************************************************
  void MainController::redo() {
    map<View *,list<int> > hierarchies=saveViewsHierarchiesBeforePop();

    Graph* root = currentGraph->getRoot();
    blockUpdate=true;
    root->unpop();
    blockUpdate=false;

    checkViewsHierarchiesAfterPop(hierarchies);
    Graph *newGraph=viewGraph[currentView];
    changeGraph(newGraph);
    // force clusterTreeWidget to update
    clusterTreeWidget->update();
    propertiesWidget->setGraph(newGraph);
    eltProperties->setGraph(newGraph,false);

    redrawViews();
    updateUndoRedoInfos();
  }
  //**********************************************************************
  View *MainController::getView(QWidget *widget) {
    return viewWidget[widget];
  }
  //**********************************************************************
  View *MainController::getCurrentView() {
    return currentView;
  }

}

