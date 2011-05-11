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
#include "tulip/MainController.h"

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
#include <tulip/DrawingTools.h>

#include "tulip/ControllerAlgorithmTools.h"
#include "tulip/ControllerViewsTools.h"
#include "tulip/TabWidget.h"
#include "tulip/Morphing.h"
#include "tulip/FindSelectionWidget.h"
#include "tulip/NodeLinkDiagramComponent.h"
#include "tulip/GlMainWidget.h"
#include "tulip/SnapshotDialog.h"

using namespace std;

namespace tlp {

  //**********************************************************************
  static Graph* getCurrentSubGraph(Graph *graph,const string &name) {
    if(graph->getAttribute<string>("name")==name)
      return graph;

    Graph *sg;
    forEach(sg, graph->getSubGraphs()) {
      Graph *csg = getCurrentSubGraph(sg, name);
      if (csg)
        return csg;
    }
    return (Graph *) 0;
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
			   std::vector<QMenu*> &groupMenus, std::string::size_type &nGroups,
			   QObject *receiver, const char *slot) {
    std::vector<std::string> itemGroupNames = getItemGroupNames(itemGroup);
    QMenu *subMenu = &menu;
    std::string::size_type nGroupNames = itemGroupNames.size();
    for (std::string::size_type i = 0; i < nGroupNames; i++) {
      QMenu *groupMenu = (QMenu *) 0;
      for (std::string::size_type j = 0; j < nGroups; j++) {
	if (itemGroupNames[i] == groupMenus[j]->objectName().toUtf8().data()) {
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
    QAction *action=subMenu->addAction(itemName.c_str());
    QObject::connect(action,SIGNAL(triggered()),receiver,slot);
  }
  //**********************************************************************
  template <typename TPROPERTY>
  void buildPropertyMenu(QMenu &menu, QObject *receiver, const char *slot) {
    typename PluginManager<TemplateAlgorithm<TPROPERTY>, PropertyContext >::ObjectCreator::const_iterator it;
    std::vector<QMenu*> groupMenus;
    std::string::size_type nGroups = 0;
    it= PropertyPluginManager<TemplateAlgorithm<TPROPERTY> >::objMap().begin();
    for (;it!=PropertyPluginManager<TemplateAlgorithm<TPROPERTY> >::objMap().end();++it)
      insertInMenu(menu, it->first.c_str(), it->second.factory->getGroup(), groupMenus, nGroups,receiver,slot);
  }
  
  void buildMenuWithContext(QMenu &menu, QObject *receiver, const char *slot) {
    PluginManager<Algorithm, AlgorithmContext>::ObjectCreator::const_iterator it;
    std::vector<QMenu*> groupMenus;
    std::string::size_type nGroups = 0;
    for (it= AlgorithmManager::objMap().begin();it != AlgorithmManager::objMap().end();++it)
      insertInMenu(menu, it->first.c_str(), it->second.factory->getGroup(), groupMenus, nGroups,receiver,slot);
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
    (void) inG; //to remove unused param warning
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
#ifndef WITHOUT_MAIN_CONTROLLER
  CONTROLLERPLUGIN(MainController, "Tulip Classic", "Tulip Team", "16/04/2008", "Tulip Classic", "1.0");
#endif

  //**********************************************************************
  MainController::MainController():
    currentGraphNbNodes(0),currentGraphNbEdges(0),graphToReload(NULL),blockUpdate(false),inAlgorithm(false),clusterTreeWidget(NULL),
    editMenu(NULL), algorithmMenu(NULL), viewMenu(NULL), optionsMenu(NULL),
    graphMenu(NULL), intMenu(NULL), stringMenu(NULL), sizesMenu(NULL),
    colorsMenu(NULL), layoutMenu(NULL), metricMenu(NULL), selectMenu(NULL),
    generalMenu(NULL) {
    morph = new Morphing();
  }
  //**********************************************************************
  MainController::~MainController() {
    clearObservers();
    Graph *currentGraph=getGraph();
    if (currentGraph) {
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
      delete tabWidgetDock;
      delete configWidgetDock;
      delete morph;
      delete currentGraph;
    }
  }
  //**********************************************************************
  void MainController::attachMainWindow(MainWindowFacade facade){
    ControllerViewsManager::attachMainWindow(facade);
    loadGUI();
  }
  //**********************************************************************
  // define some specific MetaValueCalculator classes
  // viewColor
  class ViewColorCalculator :public AbstractColorProperty::MetaValueCalculator {
  public:
    virtual void computeMetaValue(AbstractColorProperty* color, node mN,
				  Graph*, Graph*) {
      // meta node color is half opaque white
      color->setNodeValue(mN, Color(255, 255, 255, 127));
    }

    virtual void computeMetaValue(AbstractColorProperty* color, edge mE,
				  Iterator<edge>*itE, Graph*) {
      // meta edge color is the color of the first underlying edge
      color->setEdgeValue(mE, color->getEdgeValue(itE->next()));
    }
  };

  // viewLabel
  class ViewLabelCalculator :public AbstractStringProperty::MetaValueCalculator {
  public:
    // set the meta node label to label of viewMetric max corresponding node
    void computeMetaValue(AbstractStringProperty* label,
			  node mN, Graph* sg, Graph*) {
      // nothing to do if viewMetric does not exist
      if (!sg->existProperty("viewMetric"))
	return;
      node viewMetricMaxNode;
      double vMax = -DBL_MAX;
      DoubleProperty *metric = sg->getProperty<DoubleProperty>("viewMetric");
      Iterator<node> *itN= sg->getNodes();
      while (itN->hasNext()){
	node itn = itN->next();
	const double& value = metric->getNodeValue(itn);
	if (value > vMax) {
	  vMax = value;
	  viewMetricMaxNode = itn;
	}
      } delete itN;
      label->setNodeValue(mN, label->getNodeValue(viewMetricMaxNode));
    }
  };

  // viewLayout
  class ViewLayoutCalculator :public AbstractLayoutProperty::MetaValueCalculator {
  public:
    void computeMetaValue(AbstractLayoutProperty* layout,
			  node mN, Graph* sg, Graph* mg) {
      SizeProperty* size = mg->getProperty<SizeProperty>("viewSize");
      DoubleProperty* rot = mg->getProperty<DoubleProperty>("viewRotation");
      BoundingBox box =
      tlp::computeBoundingBox(sg, (LayoutProperty *) layout, size, rot);
      Coord maxL(box[1]);
      Coord minL(box[0]);
      layout->setNodeValue(mN, (maxL + minL) / 2.0 );
      Coord v(maxL - minL);
      if (v[2] < 0.0001) v[2] = 0.1f;
      mg->getProperty<SizeProperty>("viewSize")->
	setNodeValue(mN, Size(v[0],v[1],v[2]));
    }
  };

  class ViewSizeCalculator
    :public AbstractSizeProperty::MetaValueCalculator {
  public:
    void computeMetaValue(AbstractSizeProperty*, node, Graph*, Graph*) {
      // do nothing
    }
  };

  // corresponding static instances
  static ViewColorCalculator vColorCalc;
  static ViewLabelCalculator vLabelCalc;
  static ViewLayoutCalculator vLayoutCalc;
  static ViewSizeCalculator vSizeCalc;
  //**********************************************************************
  void MainController::setData(Graph *graph,DataSet dataSet) {
    editMenu->setEnabled(true);
    algorithmMenu->setEnabled(true);
    viewMenu->setEnabled(true);
    optionsMenu->setEnabled(true);
    graphMenu->setEnabled(true);
    snapshotAction->setEnabled(true);

    unsigned int holdCount=Observable::observersHoldCounter();
    Observable::holdObservers();
    Graph *newGraph=graph;

    // If graph don't have valid layoutProperty : we apply random layout
    Iterator<node>* itn = newGraph->getProperty<LayoutProperty>("viewLayout")->getNonDefaultValuatedNodes();
    if (!itn->hasNext() && newGraph->numberOfNodes()>1) {
      DataSet dataSet;
      dataSet.set<bool>("3D layout",false);
      ControllerAlgorithmTools::changeProperty<LayoutProperty>(newGraph,mainWindowFacade.getParentWidget(),"Random","viewLayout",dataSet,NULL,false,false,false);
    } delete itn;

    Graph *lastViewedGraph=newGraph;
    Observable::unholdObservers();
    setCurrentGraph(newGraph);
    // install predefined meta value calculators
    newGraph->getProperty<ColorProperty>("viewColor")->
      setMetaValueCalculator(&vColorCalc);
    newGraph->getProperty<StringProperty>("viewLabel")->
      setMetaValueCalculator(&vLabelCalc);
    newGraph->getProperty<LayoutProperty>("viewLayout")->
      setMetaValueCalculator(&vLayoutCalc);
    newGraph->getProperty<SizeProperty>("viewSize")->
      setMetaValueCalculator(&vSizeCalc);
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
          int x = 0,y = 0,width = 0,height  = 0;

          if((*(DataSet*)p.second->value).exist("id")){
            int id = 0;
            (*(DataSet*)p.second->value).get("id",id);
            if(id!=0){
	      lastViewedGraph=newGraph->getDescendantGraph(id);
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

          createView(v.first,lastViewedGraph,*(DataSet*)v.second->value,true,QRect(x,y,width,height),maximized);

        }
      }
    }else{
      View *view=initMainView(dataSet);
      lastViewedGraph=view->getGraph();
    }

    newGraph->addObserver(this);
    newGraph->addGraphObserver(this);

    clusterTreeWidget->setGraph(lastViewedGraph);
    eltProperties->setGraph(lastViewedGraph);
    propertiesWidget->setGraph(lastViewedGraph);
    updateCurrentGraphInfos();

    assert(holdCount==Observable::observersHoldCounter());
    if(holdCount!=Observable::observersHoldCounter()){
      cerr << "hold/unhold observers error in MainController setData" << endl;
    }

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
      View *view = getViewOfWidget(widgetList[i]);
      if(view){
        view->getData(&graph,&viewData);
        tmp.set<DataSet>(getNameOfView(view),viewData);
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

    *graph=getCurrentGraph();
  }
  //**********************************************************************
  void MainController::drawViews(bool init) {
    Observable::holdObservers();

    ControllerViewsManager::drawViews(init);

    eltProperties->updateTable();
    propertiesWidget->update();

    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::observableDestroyed(Observable *) {
    //cerr << "[WARNING]" << __PRETTY_FUNCTION__ << endl;
  }
  //**********************************************************************
  void MainController::update (ObserverIterator, ObserverIterator) {
    // block update when we do an undo/redo
    if(blockUpdate)
      return;

    blockUpdate=true;

    if(graphToReload){
      // enter here if a property is add/delete on the graph
      Graph *graph=graphToReload;

      updateViewsOfGraph(graph);
      updateViewsOfSubGraphs(graph);

      graphToReload=NULL;
    }else{
      drawViews();
    }

    blockUpdate=false;

    updateCurrentGraphInfos();

    updateUndoRedoInfos();
  }
  //**********************************************************************
  void MainController::initObservers() {
    if (!getCurrentGraph())
      return;

    // Observe properties of the graph
    Iterator<PropertyInterface*> *it = getCurrentGraph()->getObjectProperties();
    while (it->hasNext()) {
      PropertyInterface* tmp = it->next();
      tmp->addObserver(this);
    } delete it;
  }
  //**********************************************************************
  void MainController::clearObservers() {
    if (!getCurrentGraph())
      return;

    // Remove observation of properties
    Iterator<PropertyInterface*> *it =getCurrentGraph()->getObjectProperties();
    while (it->hasNext()) {
      (it->next())->removeObserver(this);
    } delete it;
  }
  //**********************************************************************
  void MainController::addSubGraph(Graph*g, Graph *sg){
    if(getCurrentGraph()!=g)
	  	return;
	if(!inAlgorithm){
		sg->addObserver(this);
	  	clusterTreeWidget->update();
	}
  }
  //**********************************************************************
  void MainController::delSubGraph(Graph *g, Graph *sg){
    if(inAlgorithm){
      return;
    }

    // BFS on deleted subgraph to remove observer and to update views that were on sg or a sub-graph of sg
    vector<Graph *> toCompute;
    toCompute.push_back(sg);

    while(toCompute.size()!=0){
      vector<Graph *> newToCompute;
      for(vector<Graph*>::iterator it=toCompute.begin();it!=toCompute.end();++it){
        // Remove observers
        (*it)->removeObserver(this);

        // if the current graph is *it, set current graph at g
        if(getCurrentGraph()==(*it)){
          setCurrentGraph(g);
          clusterTreeWidget->setGraph(g);
        }

        // update views that were on *it
        changeGraphOfViews(*it,g);

        Iterator<Graph *> *itS=(*it)->getSubGraphs();
        while(itS->hasNext()) {
          newToCompute.push_back(itS->next());
        }
      }
      toCompute=newToCompute;
    }

    if(graphToReload==sg)
      graphToReload=NULL;
  }
  //**********************************************************************
  void  MainController::addLocalProperty(Graph *graph, const std::string &propertyName){
    graphToReload=graph;

    if(graph==getCurrentGraph()){
      eltProperties->setGraph(graph);
      propertiesWidget->setGraph(graph);
    }

    graph->getProperty(propertyName)->addObserver(this);
  }
  //**********************************************************************
  void  MainController::delLocalProperty(Graph *graph, const std::string &propertyName){
    graphToReload=graph;

    if(graph==getCurrentGraph()){
      eltProperties->setGraph(graph);
      propertiesWidget->setGraph(graph);
    }

    graph->getProperty(propertyName)->removeObserver(this);
  }
  //**********************************************************************
  void MainController::afterSetAttribute(Graph *graph, const std::string &name){
    // In this function we only do threatment if graph name is changed (attribute "name" is changed)
    if(name=="name"){
      clusterTreeWidget->update();
      changeWindowTitle(graph);
    }
  }
  //**********************************************************************
  void MainController::treatEvent(const Event& evt) {
    GraphObserver::treatEvent(evt);
  }
  //**********************************************************************
  void MainController::loadGUI() {

  	mainWindowFacade.getWorkspace()->setScrollBarsEnabled( true );

    //+++++++++++++++++++++++++++
    //Create Data information editor (Hierarchy, Element info, Property Info)
    tabWidgetDock = new QDockWidget("Data manipulation", mainWindowFacade.getParentWidget());
    tabWidgetDock->setObjectName(QString("dataManipulationDockWidget"));
    tabWidgetDock->hide();
    tabWidgetDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tabWidgetDock->setWindowTitle("Graph Editor");
    tabWidgetDock->setFeatures(QDockWidget::DockWidgetClosable |
			    QDockWidget::DockWidgetMovable |
			    QDockWidget::DockWidgetFloatable);
    TabWidget *tabWidget = new TabWidget(tabWidgetDock);
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

    configWidgetDock = new QDockWidget("Data manipulation", mainWindowFacade.getParentWidget());
    configWidgetDock->setObjectName(QString("configDockWidget"));
    configWidgetTab = new QTabWidget(configWidgetDock);
    configWidgetTab->setFocusPolicy(Qt::StrongFocus);

    configWidgetTab->addTab(ControllerViewsTools::getNoInteractorConfigurationWidget(),"Interactor");
    configWidgetTab->setTabPosition(QTabWidget::West);
    configWidgetDock->setWidget(configWidgetTab);
    configWidgetDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    configWidgetDock->setWindowTitle("View Editor");
    configWidgetDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mainWindowFacade.addDockWidget(Qt::LeftDockWidgetArea, configWidgetDock);

    mainWindowFacade.tabifyDockWidget(tabWidgetDock,configWidgetDock);

    buildMenu();

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

    if (editMenu == NULL) {
    	editMenu = new QMenu("&Edit", mainWindowFacade.getMenuBar());
    	editMenu->setEnabled(false);
    	mainWindowFacade.getMenuBar()->insertMenu(windowAction,editMenu);
    } else {
    	editMenu->clear();
    }

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
    if (algorithmMenu == NULL) {
    	algorithmMenu = new QMenu("&Algorithm", mainWindowFacade.getMenuBar());
    	algorithmMenu->setEnabled(false);
    } else {
    	algorithmMenu->clear();
    }


    if (intMenu == NULL)
    	intMenu=new QMenu("&Integer", algorithmMenu);
    else
    	intMenu->clear();

    if (stringMenu == NULL)
    	stringMenu=new QMenu("L&abel", algorithmMenu);
    else
    	stringMenu->clear();

    if (sizesMenu == NULL)
    	sizesMenu=new QMenu("S&ize", algorithmMenu);
    else
    	sizesMenu->clear();

    if (colorsMenu == NULL)
    	colorsMenu=new QMenu("&Color", algorithmMenu);
    else
    	colorsMenu->clear();

    if (layoutMenu == NULL)
    	layoutMenu=new QMenu("&Layout", algorithmMenu);
    else
    	layoutMenu->clear();

    if (metricMenu == NULL)
    	metricMenu=new QMenu("&Measure", algorithmMenu);
    else
    	metricMenu->clear();

    if (selectMenu == NULL)
    	selectMenu=new QMenu("&Selection", algorithmMenu);
    else
    	selectMenu->clear();

    if (generalMenu == NULL)
    	generalMenu=new QMenu("&General", algorithmMenu);
    else
    	generalMenu->clear();

    buildPropertyMenu<IntegerProperty>(*intMenu, this, SLOT(changeInt()));
    buildPropertyMenu<StringProperty>(*stringMenu, this, SLOT(changeString()));
    buildPropertyMenu<SizeProperty>(*sizesMenu, this, SLOT(changeSizes()));
    buildPropertyMenu<ColorProperty>(*colorsMenu, this, SLOT(changeColors()));
    buildPropertyMenu<LayoutProperty>(*layoutMenu, this, SLOT(changeLayout()));
    buildPropertyMenu<DoubleProperty>(*metricMenu, this, SLOT(changeMetric()));
    buildPropertyMenu<BooleanProperty>(*selectMenu, this, SLOT(changeSelection()));
    buildMenuWithContext(*generalMenu, this, SLOT(applyAlgorithm()));

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
    if (graphMenu == NULL) {
    	graphMenu = new QMenu("&Graph", mainWindowFacade.getMenuBar());
    	graphMenu->setEnabled(false);
    } else {
    	graphMenu->clear();
    }

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
    if (viewMenu == NULL)  {
    	viewMenu = new QMenu("&View", mainWindowFacade.getMenuBar());
    	viewMenu->setEnabled(false);
    } else {
    	viewMenu->clear();
    }

    connect(viewMenu, SIGNAL(triggered(QAction *)), SLOT(createView(QAction*)));
    PluginManager<View, ViewContext*>::ObjectCreator::const_iterator it;
    for (it=ViewPluginManager::objMap().begin();it != ViewPluginManager::objMap().end();++it) {
      viewMenu->addAction(it->first.c_str());
    }
    mainWindowFacade.getMenuBar()->insertMenu(windowAction,viewMenu);

    //Options menu
    if (optionsMenu == NULL) {
    	optionsMenu = new QMenu("&Options", mainWindowFacade.getMenuBar());
    	optionsMenu->setEnabled(false);
    } else {
    	optionsMenu->clear();
    }

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
    connect(propertiesDockAction,SIGNAL(triggered()), this, SLOT(showGraphEditor()));
    QAction *configurationDockAction = optionsMenu->addAction("Show view editor");
    connect(configurationDockAction,SIGNAL(triggered()), this, SLOT(showViewEditor()));
    mainWindowFacade.getMenuBar()->insertMenu(windowAction,optionsMenu);

    redoAction=new QAction(QIcon(":/tulip/qt/icons/i_redo.png"),"redo",mainWindowFacade.getParentWidget());
    undoAction=new QAction(QIcon(":/tulip/qt/icons/i_undo.png"),"undo",mainWindowFacade.getParentWidget());
    snapshotAction = new QAction(QIcon(":/tulip/qt/icons/i_snapshot.png"),"snapshot",mainWindowFacade.getParentWidget());
    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    snapshotAction->setEnabled(false);
    mainWindowFacade.getToolBar()->addAction(undoAction);
    mainWindowFacade.getToolBar()->addAction(redoAction);
    mainWindowFacade.getToolBar()->addAction(snapshotAction);
    connect(undoAction,SIGNAL(triggered()),this,SLOT(undo()));
    connect(redoAction,SIGNAL(triggered()),this,SLOT(redo()));
    connect(snapshotAction,SIGNAL(triggered()),this,SLOT(snapshot()));
  }
  //**********************************************************************
  View* MainController::initMainView(DataSet dataSet) {
    View* newView=createView("Node Link Diagram view",getCurrentGraph(),dataSet);
    return newView;
  }
  //**********************************************************************
  View* MainController::createView(const string &name,Graph *graph,DataSet dataSet,bool forceWidgetSize,const QRect &rect,bool maximized){
    QRect newRect=rect;
    forceWidgetSize=true;
    unsigned int viewsNumber=getViewsNumber();
    if(newRect.width()==0 && newRect.height()==0){
      forceWidgetSize=false;
      newRect=QRect(QPoint((viewsNumber)*20,(viewsNumber)*20),QSize(0,0));
    }

    unsigned int holdCount=Observable::observersHoldCounter();

    View *createdView=ControllerViewsManager::createView(name,graph,dataSet,forceWidgetSize,newRect,maximized);

    assert(holdCount==Observable::observersHoldCounter());
    if(holdCount!=Observable::observersHoldCounter()){
      cerr << "hold/unhold observers error when the view " << name << " is created" << endl;
    }

    connect(createdView, SIGNAL(elementSelected(unsigned int, bool)),this,SLOT(showElementProperties(unsigned int, bool)));
    connect(createdView, SIGNAL(requestChangeGraph(tlp::View*,tlp::Graph*)), this, SLOT(viewRequestChangeGraph(tlp::View*,tlp::Graph*)));

    return createdView;
  }
  //**********************************************************************
  bool MainController::windowActivated(QWidget *widget) {

    lastConfigTabIndexOnView[getCurrentView()]=configWidgetTab->currentIndex();

    if(!ControllerViewsManager::windowActivated(widget))
      return false;

    // Remove tabs of View Editor
    while(configWidgetTab->count()>0){
      configWidgetTab->removeTab(0);
    }

    // Find view and graph of this widget
    View *view=getViewOfWidget(widget);
    Graph *graph=getGraphOfView(view);

    // Update left part of tulip
    clusterTreeWidget->setGraph(graph);
    eltProperties->setGraph(graph);
    propertiesWidget->setGraph(graph);

    // Load interactor configuration widget
    QWidget *interactorConfigurationWidget=ControllerViewsManager::getInteractorConfigurationWidgetOfView(view);
    if(interactorConfigurationWidget)
      configWidgetTab->addTab(interactorConfigurationWidget,"Interactor");
    else
      configWidgetTab->addTab(ControllerViewsTools::getNoInteractorConfigurationWidget(),"Interactor");

    // Load view configuration widget
    list<pair<QWidget *,string> > configWidgetsList=view->getConfigurationWidget();
    for(list<pair<QWidget *,string> >::iterator it=configWidgetsList.begin();it!=configWidgetsList.end();++it){
      configWidgetTab->addTab((*it).first,(*it).second.c_str());
    }
    if(lastConfigTabIndexOnView.count(view)!=0)
      configWidgetTab->setCurrentIndex(lastConfigTabIndexOnView[view]);

    //Remove observer (nothing if this not observe)
    graph->removeGraphObserver(this);
    graph->removeObserver(this);
    //Add observer
    graph->addGraphObserver(this);
    graph->addObserver(this);

    return true;
  }
  //**********************************************************************
  bool MainController::changeGraph(Graph *graph) {
    if(getCurrentGraph()==graph)
      return false;
    if(!getCurrentView())
      return false;

    clearObservers();

    unsigned int holdCount=Observable::observersHoldCounter();

    ControllerViewsManager::changeGraph(graph);

    assert(holdCount==Observable::observersHoldCounter());
    if(holdCount!=Observable::observersHoldCounter()){
      cerr << "hold/unhold observers error when changeGraph " << endl;
    }

    clusterTreeWidget->setGraph(graph);
    eltProperties->setGraph(graph);
    propertiesWidget->setGraph(graph);

    updateCurrentGraphInfos();
    updateUndoRedoInfos();

    initObservers();
    //Remove observer (nothing if this not observe)
    graph->removeGraphObserver(this);
    graph->removeObserver(this);
    //Add observer
    graph->addGraphObserver(this);
    graph->addObserver(this);
    // install predefined meta value calculators
    graph->getProperty<ColorProperty>("viewColor")->
      setMetaValueCalculator(&vColorCalc);
    graph->getProperty<StringProperty>("viewLabel")->
      setMetaValueCalculator(&vLabelCalc);
    graph->getProperty<LayoutProperty>("viewLayout")->
      setMetaValueCalculator(&vLayoutCalc);
    graph->getProperty<SizeProperty>("viewSize")->
      setMetaValueCalculator(&vSizeCalc);

    return true;
  }
  //**********************************************************************
   void MainController::graphAboutToBeRemove(Graph*){
     setCurrentGraph(NULL);
   }
  //**********************************************************************
  bool MainController::changeInteractor(QAction* action) {
    QWidget *configurationWidget;
    if(!ControllerViewsManager::changeInteractor(action,&configurationWidget))
      return false;

    bool onInteractorConfigTab=configWidgetTab->currentIndex()==0;
    configWidgetTab->removeTab(0);
    configWidgetTab->insertTab(0,configurationWidget,"Interactor");

    if(onInteractorConfigTab)
      configWidgetTab->setCurrentIndex(0);

    return true;
  }
  //**********************************************************************
  void MainController::widgetWillBeClosed(QObject *object) {
    ControllerViewsManager::widgetWillBeClosed(object);

    //If after close this widget we have no widget open : clear
    if(getViewsNumber()==0){
      while(configWidgetTab->count()>0)
        configWidgetTab->removeTab(0);
      configWidgetTab->addTab(ControllerViewsTools::getNoInteractorConfigurationWidget(),"Interactor");
    }
  }
  //==================================================
  void MainController::showElementProperties(unsigned int eltId, bool isNode) {
    if (isNode)
      eltProperties->setCurrentNode(getCurrentGraph(),  tlp::node(eltId));
    else
      eltProperties->setCurrentEdge(getCurrentGraph(),  tlp::edge(eltId));
    // show 'Element' tab in 'Info Editor'
    QWidget *tab = eltProperties->parentWidget();
    QTabWidget *tabWidget = (QTabWidget *) tab->parentWidget()->parentWidget();
    tabWidget->setCurrentIndex(tabWidget->indexOf(tab));
    showGraphEditor();
  }
  //==================================================
  void MainController::showGraphEditor() {
    if (configWidgetDock->isVisible()) {
      QMainWindow* mWindow = (QMainWindow *) mainWindowFacade.getParentWidget();
      configWidgetDock->hide();
      tabWidgetDock->show();
      mWindow->tabifyDockWidget(tabWidgetDock,configWidgetDock);
      configWidgetDock->show();
    }else{
      tabWidgetDock->show();
    }
  }
  //==================================================
  void MainController::showViewEditor() {
    if (tabWidgetDock->isVisible()) {
      QMainWindow* mWindow = (QMainWindow *) mainWindowFacade.getParentWidget();
      tabWidgetDock->hide();
      configWidgetDock->show();
      tabWidgetDock->show();
      mWindow->tabifyDockWidget(tabWidgetDock,configWidgetDock);
    }else{
      configWidgetDock->show();
    }
  }
  //==================================================
  void MainController::viewRequestChangeGraph(View *view,Graph *graph) {
    assert(view==getCurrentView());
    (void) view; //to remove unused param warning
    changeGraph(graph);
  }
  //**********************************************************************
  void MainController::updateCurrentGraphInfos() {
    if(!getCurrentGraph())
      return;

    static QLabel *currentGraphInfosLabel = 0;
    if (!currentGraphInfosLabel) {
      currentGraphInfosLabel = new QLabel(mainWindowFacade.getStatusBar());
      mainWindowFacade.getStatusBar()->addPermanentWidget(currentGraphInfosLabel);
    }

    currentGraphNbNodes=getCurrentGraph()->numberOfNodes();
    currentGraphNbEdges=getCurrentGraph()->numberOfEdges();

    stringstream sstr;
    sstr << "nodes: " << currentGraphNbNodes << ", edges: " << currentGraphNbEdges;
    currentGraphInfosLabel->setText(sstr.str().c_str());
    // Update nb nodes/edges for current graph and subgraphs
    clusterTreeWidget->updateCurrentGraphInfos(getCurrentGraph());
  }
  //==============================================================
  void MainController::editCut() {
    Graph *graph=getCurrentGraph();
    if(!graph)
    	return;

    BooleanProperty * selP = graph->getProperty<BooleanProperty>("viewSelection");
    if( !selP ) return;
    // Save selection
    NodeA nodeA;
    EdgeA edgeA;
    GetSelection( nodeA, edgeA, graph, selP );
    Observable::holdObservers();
    Graph* newGraph = tlp::newGraph();
    tlp::copyToGraph( newGraph, graph, selP );
    stringstream tmpss;
    DataSet dataSet;
    tlp::exportGraph(newGraph, tmpss, "tlp", dataSet, NULL);
    delete newGraph;
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(tmpss.str().c_str());
    graph->push();
    // Restore selection
    SetSelection( selP, nodeA, edgeA, graph );
    tlp::removeFromGraph( graph, selP );
    Observable::unholdObservers();
    drawViews();
  }
  //==============================================================
  void MainController::editCopy() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    BooleanProperty * selP = graph->getProperty<BooleanProperty>("viewSelection");
    if( !selP ) return;
    Observable::holdObservers();
    Graph* newGraph = tlp::newGraph();
    tlp::copyToGraph( newGraph, graph, selP );
    stringstream tmpss;
    DataSet dataSet;
    tlp::exportGraph(newGraph, tmpss, "tlp", dataSet, NULL);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString::fromUtf8(tmpss.str().c_str()));
    delete newGraph;
    Observable::unholdObservers();
  }
  //==============================================================
  void MainController::editPaste() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    graph->removeObserver(this);
    Observable::holdObservers();
    BooleanProperty * selP = graph->getProperty<BooleanProperty>("viewSelection");

    graph->push();
    Graph *newGraph=tlp::newGraph();
    DataSet dataSet;
    QClipboard *clipboard = QApplication::clipboard();
    dataSet.set<string>("file::data", clipboard->text().toUtf8().data());
    tlp::importGraph("tlp", dataSet, NULL ,newGraph);
    tlp::copyToGraph( graph, newGraph, 0, selP );
    delete newGraph;
    Observable::unholdObservers();
    graph->addObserver(this);

    updateCurrentGraphInfos();

    drawViews(true);
  }
  //==============================================================
  void MainController::editFind() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    static string currentProperty;
    FindSelectionWidget *sel = new FindSelectionWidget(graph, currentProperty, mainWindowFacade.getParentWidget());
    Observable::holdObservers();
    // allow to undo
    graph->push();
    int nbItemsFound = sel->exec();
    if (nbItemsFound > -1)
      currentProperty = sel->getCurrentProperty();
    delete sel;
    switch(nbItemsFound) {
    case 0:
      mainWindowFacade.getStatusBar()->showMessage("No item found.");
      //no item found does not mean we want to undo.
      break;
    case -1:
      // forget the current graph state
      graph->pop(false);
      break;
    default:
      stringstream sstr;
      sstr << nbItemsFound << " item(s) found.";
      mainWindowFacade.getStatusBar()->showMessage(sstr.str().c_str());
    }
    // unhold at last to ensure that in case of cancellation or
    // no item found the current graph state has been pop before
    // the call to updateUndoRedoInfos
    Observable::unholdObservers();
  }
  //==============================================================
  void MainController::editCreateGroup() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    set<node> tmp;
    Iterator<node> *it=graph->getNodes();
    BooleanProperty *select = graph->getProperty<BooleanProperty>("viewSelection");
    while (it->hasNext()) {
      node itn = it->next();
      if (select->getNodeValue(itn))
        tmp.insert(itn);
    }delete it;
    if (tmp.empty()) return;
    graph->push();
    Observable::holdObservers();
    bool haveToChangeGraph=false;
    Graph *graphToAddTo=graph;
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
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

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
    QString text = QInputDialog::getText(mainWindowFacade.getParentWidget(),
        "Creation of subgraph" ,
        "Please enter the subgraph name" ,
        QLineEdit::Normal, QString::null, &ok);
    if (ok && !text.isEmpty()) {
      sel1 = graph->getProperty<BooleanProperty>("viewSelection");
      graph->push();
      Graph *tmp = graph->addSubGraph(sel1);
      tmp->setAttribute("name", string(text.toUtf8().data()));
      clusterTreeWidget->update();
      //emit clusterTreeNeedUpdate();
    }
    else if (ok) {
      sel1 = graph->getProperty<BooleanProperty>("viewSelection");
      graph->push();
      Graph *tmp=graph->addSubGraph(sel1);
      tmp->setAttribute("name", newName());
      clusterTreeWidget->update();
      //emit clusterTreeNeedUpdate();
    }
  }
  //==============================================================
  void MainController::editDelSelection() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    graph->push();
    graph->removeObserver(this);
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
    graph->addObserver(this);
    updateCurrentGraphInfos();

    drawViews();
  }
  //==============================================================
  void MainController::editReverseSelection() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    graph->push();
    Observable::holdObservers();
    if(graph->existLocalProperty("viewSelection")){
      graph->getProperty<BooleanProperty>("viewSelection")->reverse();
    }else{
      BooleanProperty *selectionProperty=graph->getProperty<BooleanProperty>("viewSelection");
      StableIterator<node> itN(graph->getNodes());
      while(itN.hasNext()) {
        node itv = itN.next();
        selectionProperty->setNodeValue(itv,!selectionProperty->getNodeValue(itv));
      }
      StableIterator<edge> itE(graph->getEdges());
      while(itE.hasNext()) {
        edge ite=itE.next();
        selectionProperty->setEdgeValue(ite,!selectionProperty->getEdgeValue(ite));
      }
    }
    Observable::unholdObservers();
  }
  //==============================================================
  void MainController::editSelectAll() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    graph->push();
    Observable::holdObservers();
    BooleanProperty *selectionProperty =
      graph->getProperty<BooleanProperty>("viewSelection");
    node n;
    forEach(n, graph->getNodes()) {
      selectionProperty->setNodeValue(n, true);
     }
    edge e;
    forEach(e, graph->getEdges()) {
      selectionProperty->setEdgeValue(e, true);
    }
    Observable::unholdObservers();
  }
  //==============================================================
  void MainController::editDeselectAll() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    graph->push();
    Observable::holdObservers();
    if(graph->existLocalProperty("viewSelection")){
      graph->getProperty<BooleanProperty>("viewSelection")->setAllNodeValue(false);
      graph->getProperty<BooleanProperty>("viewSelection")->setAllEdgeValue(false);
    }else {
      BooleanProperty *selectionProperty =
	graph->getProperty<BooleanProperty>("viewSelection");
      node n;
      forEach(n, graph->getNodes()) {
	selectionProperty->setNodeValue(n, false);
      }
      edge e;
      forEach(e, graph->getEdges()) {
	selectionProperty->setEdgeValue(e, false);
      }
    }
    Observable::unholdObservers();
  }
  //**********************************************************************
  /// Apply a general algorithm
  void MainController::applyAlgorithm() {
    QAction *action=(QAction*)(sender());
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    inAlgorithm=true;
    bool result=ControllerAlgorithmTools::applyAlgorithm(graph,mainWindowFacade.getParentWidget(),action->text().toStdString());
    inAlgorithm=false;
    if(result){
      undoAction->setEnabled(graph->canPop());
      editUndoAction->setEnabled(graph->canPop());
      clusterTreeWidget->update();
      clusterTreeWidget->setGraph(graph);
      drawViews(true);
    }
  }
  //**********************************************************************
  void MainController::afterChangeProperty() {
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);
    propertiesWidget->setGraph(getCurrentGraph());
    drawViews(false);
  }
  //**********************************************************************
  GraphState *MainController::constructGraphState() {
    GlMainView *mainView=dynamic_cast<GlMainView *>(getCurrentView());
    if(mainView)
      return new GraphState(mainView->getGlMainWidget());

    return NULL;
  }
  //**********************************************************************
  void MainController::applyMorphing(GraphState *graphState){
    GlMainView *mainView=dynamic_cast<GlMainView *>(getCurrentView());
    clearObservers();
    mainView->getGlMainWidget()->getScene()->centerScene();
    GraphState * g1 = constructGraphState();
    bool morphable = morph->init(mainView->getGlMainWidget(), graphState, g1);
    if( !morphable ) {
      delete g1;
      g1 = 0;
    } else {
      morph->start(mainView->getGlMainWidget());
      graphState = 0;	// state remains in morph data ...
    }
    initObservers();
  }
  //**********************************************************************
  void MainController::changeString() {
    QAction *action=(QAction*)(sender());
    inAlgorithm=true;
    bool result = ControllerAlgorithmTools::changeString(getCurrentGraph(),mainWindowFacade.getParentWidget(),action->text().toStdString(),"viewLabel",getCurrentView());
    inAlgorithm=false;
    if (result)
      afterChangeProperty();
  }
  //**********************************************************************
  void MainController::changeSelection() {
    QAction *action=(QAction*)(sender());

    inAlgorithm=true;
    bool result = ControllerAlgorithmTools::changeBoolean(getCurrentGraph(),mainWindowFacade.getParentWidget(),action->text().toStdString(),"viewSelection",getCurrentView());
    inAlgorithm=false;
    if (result)
      afterChangeProperty();
  }
  //**********************************************************************
  void MainController::changeMetric() {
    QAction *action=(QAction*)(sender());
    inAlgorithm=true;
    bool result = ControllerAlgorithmTools::changeMetric(getCurrentGraph(),mainWindowFacade.getParentWidget(),action->text().toStdString(),"viewMetric",getCurrentView(),mapMetricAction->isChecked(),"Color Mapping","viewColor");
    inAlgorithm=false;
    if (result)
      afterChangeProperty();
  }
  //**********************************************************************
  void MainController::changeLayout() {
    QAction *action=(QAction*)(sender());
    GraphState * g0=NULL;
    if(morphingAction->isChecked())
      g0=constructGraphState();

    inAlgorithm=true;
    bool result = ControllerAlgorithmTools::changeLayout(getCurrentGraph(),mainWindowFacade.getParentWidget(),action->text().toStdString(), "viewLayout",getCurrentView());
    inAlgorithm=false;
    if (result) {
      if( forceRatioAction->isChecked() )
        getCurrentGraph()->getLocalProperty<LayoutProperty>("viewLayout")->perfectAspectRatio();

      if( morphingAction->isChecked() && g0) {
        applyMorphing(g0);
      }
    }

    drawViews(true);
  }
  //**********************************************************************
  void MainController::changeInt() {
    QAction *action=(QAction*)(sender());
    inAlgorithm=true;
    bool result = ControllerAlgorithmTools::changeInt(getCurrentGraph(),mainWindowFacade.getParentWidget(),action->text().toStdString(), "viewInt",getCurrentView());
    inAlgorithm=false;
    if (result)
      afterChangeProperty();
  }
  //**********************************************************************
  void MainController::changeColors() {
    QAction *action=(QAction*)(sender());
    GraphState * g0=NULL;
    if(morphingAction->isChecked())
      g0=constructGraphState();

    inAlgorithm=true;
    bool result = ControllerAlgorithmTools::changeColors(getCurrentGraph(),mainWindowFacade.getParentWidget(),action->text().toStdString(),"viewColor",getCurrentView());
    inAlgorithm=false;
    if( result ) {
      if( morphingAction->isChecked() && g0) {
        applyMorphing(g0);
      }
    }

    delete g0;
  }
  //**********************************************************************
  void MainController::changeSizes() {
    QAction *action=(QAction*)(sender());
    GraphState * g0 = NULL;
    if(morphingAction->isChecked())
      g0=constructGraphState();

    inAlgorithm=true;
    bool result = ControllerAlgorithmTools::changeSizes(getCurrentGraph(),mainWindowFacade.getParentWidget(),action->text().toStdString(),"viewSize",getCurrentView());
    inAlgorithm=false;
    if( result ) {
      if( morphingAction->isChecked() && g0) {
        applyMorphing(g0);
      }
    }

    delete g0;
  }
  //**********************************************************************
  void MainController::isAcyclic() {
    ControllerAlgorithmTools::isAcyclic(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  void MainController::makeAcyclic() {
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);
    ControllerAlgorithmTools::makeAcyclic(getCurrentGraph());
  }
  //**********************************************************************
  void MainController::isSimple() {
    ControllerAlgorithmTools::isSimple(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  void MainController::makeSimple() {
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);
    ControllerAlgorithmTools::makeSimple(getCurrentGraph());
  }
  //**********************************************************************
  void MainController::isConnected() {
    ControllerAlgorithmTools::isConnected(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  void MainController::makeConnected() {
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);
    ControllerAlgorithmTools::makeConnected(getCurrentGraph());
  }
  //**********************************************************************
  void MainController::isBiconnected() {
    ControllerAlgorithmTools::isBiconnected(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  void MainController::makeBiconnected() {
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);
    ControllerAlgorithmTools::makeBiconnected(getCurrentGraph());
  }
  //**********************************************************************
  void MainController::isTriconnected() {
    ControllerAlgorithmTools::isTriconnected(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  //**********************************************************************
  void MainController::isTree() {
    ControllerAlgorithmTools::isTree(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  //**********************************************************************
  void MainController::isFreeTree() {
    ControllerAlgorithmTools::isFreeTree(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  void MainController::makeDirected() {
    undoAction->setEnabled(true);
    editUndoAction->setEnabled(true);
    ControllerAlgorithmTools::makeDirected(mainWindowFacade.getParentWidget(),getCurrentGraph());
  }
  //**********************************************************************
  void MainController::isPlanar() {
    ControllerAlgorithmTools::isPlanar(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  //**********************************************************************
  void MainController::isOuterPlanar() {
    ControllerAlgorithmTools::isOuterPlanar(getCurrentGraph(),mainWindowFacade.getParentWidget());
  }
  //**********************************************************************
  void MainController::reverseSelectedEdgeDirection() {
    Observable::holdObservers();
    getCurrentGraph()->push();
    getCurrentGraph()->getProperty<BooleanProperty>("viewSelection")->reverseEdgeDirection();
    Observable::unholdObservers();
  }
  //**********************************************************************
  void MainController::updateUndoRedoInfos() {
    Graph *graph=getCurrentGraph();
    if(!graph)
      return;

    undoAction->setEnabled(graph->canPop());
    redoAction->setEnabled(graph->canUnpop());
    editUndoAction->setEnabled(graph->canPop());
    editRedoAction->setEnabled(graph->canUnpop());
  }
  //**********************************************************************
  void MainController::undo() {
    saveViewsGraphsHierarchies();

    Graph *root=getCurrentGraph()->getRoot();
    blockUpdate=true;
    root->pop();
    blockUpdate=false;

    checkViewsGraphsHierarchy();
    Graph *newGraph=getGraphOfView(getCurrentView());

    updateViewsOfGraph(newGraph);
    updateViewsOfSubGraphs(newGraph->getRoot());
    // force clusterTreeWidget to update
    clusterTreeWidget->update();
    propertiesWidget->setGraph(newGraph);
    eltProperties->setGraph(newGraph,false);

    // views and interactors informed that undo has been made
    vector<View *> views;
    getViews(views);
    for(vector<View*>::iterator it=views.begin();it!=views.end();++it){
      (*it)->undoIsDone();
      Interactor *interactor=(*it)->getActiveInteractor();
      if(interactor)
        interactor->undoIsDone();
    }

    drawViews(false);
    updateCurrentGraphInfos();
    updateUndoRedoInfos();
  }
  //**********************************************************************
  void MainController::redo() {
    saveViewsGraphsHierarchies();

    Graph* root = getCurrentGraph()->getRoot();
    blockUpdate=true;
    root->unpop();
    blockUpdate=false;

    checkViewsGraphsHierarchy();
    Graph *newGraph=getGraphOfView(getCurrentView());
    updateViewsOfSubGraphs(newGraph->getRoot());
    // force clusterTreeWidget to update
    clusterTreeWidget->update();
    propertiesWidget->setGraph(newGraph);
    eltProperties->setGraph(newGraph,false);

    drawViews(false);
    updateCurrentGraphInfos();
    updateUndoRedoInfos();
  }
  //**********************************************************************
  void MainController::snapshot(){
    QImage image=currentView->createPicture(16,16,false);
    if(image.isNull()){
      QMessageBox::critical(NULL,"can't create snapshot","Sorry but you can't create snapshot with this view");
    }else{
      SnapshotDialog snapshotDialog(*currentView);
      snapshotDialog.setModal(true);
      snapshotDialog.exec();
    }
  }

}

