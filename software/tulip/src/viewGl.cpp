//**********************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <string>
#include <map>
#include <vector>


#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qmessagebox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qapplication.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qfiledialog.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qdir.h>
#include <QtGui/qinputdialog.h>
#include <QtGui/qworkspace.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qdesktopwidget.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qimage.h>
#include <QtGui/qimagewriter.h>
#include <QtGui/qpainter.h>
#include <QtGui/qprogressdialog.h>
#include <QtGui/qlayout.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qcursor.h>
#include <QtGui/qaction.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qprinter.h>
#include <QtGui/qprintdialog.h>
#include <QtGui/qmenudata.h>
#include <QtGui/qtextedit.h>

#include <tulip/TlpTools.h>
#include <tulip/Reflect.h>
#include <tulip/GlMainWidget.h>
#include <tulip/ElementPropertiesWidget.h>
#include <tulip/AbstractProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/TlpQtTools.h>
#include <tulip/StableIterator.h>
#include <tulip/FindSelectionWidget.h>
#include <tulip/Morphing.h>
#include <tulip/ExtendedClusterOperation.h>
#include <tulip/ExportModule.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/ForEach.h>
#include <tulip/GWInteractor.h>
#include <tulip/LayerManagerWidget.h>
#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlRectTextured.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/GraphView.h>
#include <tulip/GlMainView.h>
#include <tulip/hash_string.h>
#include <tulip/TabWidget.h>

#include <PluginsManagerDialog.h>
#include <UpdatePlugin.h>

#include "viewGl.h"
#include "Application.h"
#include "QtProgress.h"
#include "ElementInfoToolTip.h"
#include "GridOptionsWidget.h"
#include "InfoDialog.h"
#include "AppStartUp.h"

using namespace std;
using namespace tlp;

//**********************************************************************
///Constructor of ViewGl
viewGl::viewGl(QWidget* parent): QMainWindow(parent)  {
  setupUi(this);
  //  cerr << __PRETTY_FUNCTION__ << endl;

  // remove strange scories from designer/Tulip.ui
  graphMenu->removeAction(Action);
  graphMenu->removeAction(menunew_itemAction);
  // set workspace background
  workspace->setBackground(QBrush(QPixmap(QString::fromUtf8(":/background_logo.png"))));

  Observable::holdObservers();
  //glWidget=0;
  gridOptionsWidget=0;
  aboutWidget=0;
  //copyCutPasteGraph = 0;
  elementsDisabled = false;

  //=======================================
  //MDI
  workspace->setScrollBarsEnabled( true );
  connect (workspace, SIGNAL(windowActivated(QWidget *)), this, SLOT(windowActivated(QWidget *)));

  //Create Data information editor (Hierarchy, Element info, Property Info)
  tabWidgetDock = new QDockWidget("Data manipulation", this);
  tabWidgetDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  tabWidgetDock->setWindowTitle("Info Editor");
  tabWidgetDock->setFeatures(QDockWidget::DockWidgetClosable |
			     QDockWidget::DockWidgetMovable |
			     QDockWidget::DockWidgetFloatable);
  //tabWidgetDock->setResizeEnabled(true);
  TabWidget *tabWidget = new TabWidget(tabWidgetDock);
#ifndef STATS_UI
  // remove Statistics tab if not needed
  tabWidget->tabWidget2->removeTab(tabWidget->tabWidget2->indexOf(tabWidget->StatsTab));
#endif
  tabWidgetDock->setWidget(tabWidget);
  this->addDockWidget(Qt::LeftDockWidgetArea, tabWidgetDock);
  tabWidget->show();
  tabWidgetDock->show();

  //Init hierarchy visualization widget
  clusterTreeWidget=tabWidget->clusterTree;
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

  //connect signals related to graph replacement
  connect(clusterTreeWidget, SIGNAL(graphChanged(Graph *)), 
	  this, SLOT(changeGraph(Graph *)));
  /*connect(clusterTreeWidget, SIGNAL(aboutToRemoveView(tlp::Graph *)), this, SLOT(graphAboutToBeRemoved(tlp::Graph *)));
    connect(clusterTreeWidget, SIGNAL(aboutToRemoveAllView(tlp::Graph *)), this, SLOT(graphAboutToBeRemoved(tlp::Graph *)));*/

  //+++++++++++++++++++++++++++
  //Connection of the menus
  connect(&stringMenu     , SIGNAL(triggered(QAction*)), SLOT(changeString(QAction*)));
  connect(&metricMenu     , SIGNAL(triggered(QAction*)), SLOT(changeMetric(QAction*)));
  connect(&layoutMenu     , SIGNAL(triggered(QAction*)), SLOT(changeLayout(QAction*)));
  connect(&selectMenu     , SIGNAL(triggered(QAction*)), SLOT(changeSelection(QAction*)));
  connect(&generalMenu  , SIGNAL(triggered(QAction*)), SLOT(applyAlgorithm(QAction*)));
  connect(&sizesMenu      , SIGNAL(triggered(QAction*)), SLOT(changeSizes(QAction*)));
  connect(&intMenu        , SIGNAL(triggered(QAction*)), SLOT(changeInt(QAction*)));
  connect(&colorsMenu     , SIGNAL(triggered(QAction*)), SLOT(changeColors(QAction*)));
  connect(&exportGraphMenu, SIGNAL(triggered(QAction*)), SLOT(exportGraph(QAction*)));
  connect(&importGraphMenu, SIGNAL(triggered(QAction*)), SLOT(importGraph(QAction*)));
  connect(&exportImageMenu, SIGNAL(triggered(QAction*)), SLOT(exportImage(QAction*)));
  connect(windowsMenu, SIGNAL( aboutToShow() ),
	  this, SLOT( windowsMenuAboutToShow() ) );
  connect(windowsMenu, SIGNAL(triggered(QAction*)),
	  this, SLOT( windowsMenuActivated(QAction*)));
  connect(toolsMenu, SIGNAL(triggered(QAction *)), SLOT(addView(QAction*)));
  connect(graphToolBar, SIGNAL(actionTriggered(QAction *)), SLOT(changeInteractor(QAction*)));
  enableQMenu(editMenu,false);

  Observable::unholdObservers();
  morph = new Morphing();

  // initialization of Qt Assistant, the path should be in $PATH
#if defined(__APPLE__)
  std::string assistantPath(tlp::TulipLibDir);
  assistantPath += string("../") + QT_ASSISTANT;
  assistant = new QAssistantClient(assistantPath.c_str(), this);
#else
  assistant = new QAssistantClient("", this);
#endif
  connect(assistant, SIGNAL(error(const QString&)), SLOT(helpAssistantError(const QString&)));

  currentGraph=NULL;
  currentView=NULL;
}
//**********************************************************************
void viewGl::enableQMenu(QMenu *popupMenu, bool enabled) {
  QList <QAction *> actions = popupMenu->actions();
  int nbElements = actions.size();
  for(int i = 0 ; i < nbElements ; i++) {
    actions.at(i)->setEnabled(enabled);
  }
}

//**********************************************************************
void viewGl::enableElements(bool enabled) {
  enableQMenu(&stringMenu, enabled);
  enableQMenu(&layoutMenu, enabled);
  enableQMenu(&metricMenu, enabled);
  enableQMenu(&selectMenu, enabled);
  enableQMenu(&intMenu, enabled);
  enableQMenu(&sizesMenu, enabled);
  enableQMenu(&colorsMenu, enabled);
  enableQMenu(&generalMenu, enabled);
  enableQMenu(&exportGraphMenu, enabled);
  enableQMenu(&exportImageMenu, enabled);
  fileSaveAction->setEnabled(enabled);
  fileSaveAsAction->setEnabled(enabled);
  filePrintAction->setEnabled(enabled);
  mouseActionGroup->setEnabled(enabled);
  grid_option->setEnabled(enabled);

  elementsDisabled = !enabled;
}
//**********************************************************************
void viewGl::observableDestroyed(Observable *) {
  //cerr << "[WARNING]" << __PRETTY_FUNCTION__ << endl;
}
//**********************************************************************
void viewGl::update ( ObserverIterator begin, ObserverIterator end) {
  /*Observable::holdObservers();
    clearObservers();*/
  eltProperties->updateTable();
  propertiesWidget->update();
  if (gridOptionsWidget !=0 )
    gridOptionsWidget->validateGrid();
  //redrawView();
  /*Observable::unholdObservers();
    initObservers();*/
}
//**********************************************************************
static const unsigned int NB_VIEWED_PROPERTIES=13;
static const string viewed_properties[NB_VIEWED_PROPERTIES]=
  {"viewLabel",
   "viewLabelColor",
   "viewLabelPosition",
   "viewBorderColor",
   "viewBorderWidth",
   "viewColor",
   "viewSelection",
   "viewMetaGraph",
   "viewShape",
   "viewSize",
   "viewTexture",
   "viewLayout",
   "viewRotation" };
//**********************************************************************
/*void viewGl::initObservers() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (!glWidget) return;
  Graph *graph = glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  if (graph==0) return;
  for (unsigned int i=0; i<NB_VIEWED_PROPERTIES; ++i) {
    if (graph->existProperty(viewed_properties[i]))
      graph->getProperty(viewed_properties[i])->addObserver(this);
  }
  graph->addObserver(this);
  // initialize the number of nodes ans edges
  currentGraphNbNodes = graph->numberOfNodes();
  currentGraphNbEdges = graph->numberOfEdges();
  // show the infos
  updateCurrentGraphInfos();
  }*/
//**********************************************************************
/*void viewGl::clearObservers() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  if (glWidget == 0) return;
  Graph *graph = glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  if (graph == 0) return;
  for (unsigned int i=0; i<NB_VIEWED_PROPERTIES; ++i) {
    if (graph->existProperty(viewed_properties[i]))
      graph->getProperty(viewed_properties[i])->removeObserver(this);
  }
  graph->removeObserver(this);
  }
//**********************************************************************
// GraphObserver interface
void viewGl::addNode (Graph *graph, const node) {
  ++currentGraphNbNodes;
    updateCurrentGraphInfos();
}
void  viewGl::addEdge (Graph *graph, const edge) {
  ++currentGraphNbEdges;
    updateCurrentGraphInfos();
}
void  viewGl::delNode (Graph *graph, const node) {
  --currentGraphNbNodes;
    updateCurrentGraphInfos();
}
void  viewGl::delEdge (Graph *graph, const edge) {
  --currentGraphNbEdges;
    updateCurrentGraphInfos();
}
void  viewGl::reverseEdge (Graph *, const edge) {
}
void  viewGl::destroy (Graph *) {
}*/

//**********************************************************************
///Destructor of viewGl
viewGl::~viewGl() {
  delete morph;
  //  cerr << __PRETTY_FUNCTION__ << endl;
}
//**********************************************************************
// the dialog below is used to show plugins loading errors
// if needed it will be deleted when showing first graph
static QDialog *errorDlg = (QDialog *) NULL;

void viewGl::startTulip() {
  // adjust size if needed
  QRect sRect = QApplication::desktop()->availableGeometry();
  QRect wRect(this->geometry());
  QRect fRect(this->frameGeometry());
  int deltaWidth = fRect.width() - wRect.width();
  int deltaHeight = fRect.height() - wRect.height();
  // adjust width
  if (fRect.width() > sRect.width()) {
    wRect.setWidth(sRect.width() - deltaWidth);
  }
  // screen width centering
  wRect.moveLeft(sRect.left() + 
		 (sRect.width() - wRect.width())/2);
  // adjust height
  if (fRect.height() > sRect.height()) {
    wRect.setHeight(sRect.height() - deltaHeight);
  }
  // screen height centering
  wRect.moveTop(sRect.top() + (deltaHeight - deltaWidth)/2 + 
		(sRect.height() - wRect.height())/2);
  // adjust geometry
  this->setGeometry(wRect.x(), wRect.y(),
		    wRect.width(), wRect.height());

  UpdatePlugin::installWhenRestartTulip();

  AppStartUp *appStart=new AppStartUp(this);
  QDialog *errorDlg;
  std::string errors;
  appStart->show();
  appStart->initTulip(&pluginLoader,errors);
  delete appStart;
  buildMenus();
  this->show();
  if (errors.size() > 0) {
    errorDlg = new QDialog(this);
    errorDlg->setWindowTitle("Errors when loading Tulip plugins !!!");
    QVBoxLayout* errorDlgLayout = new QVBoxLayout(errorDlg);
    errorDlgLayout->setMargin(11);
    errorDlgLayout->setSpacing(6);
    QFrame *frame = new QFrame(errorDlg);
    QHBoxLayout* frameLayout = new QHBoxLayout(frame);
    frameLayout->setMargin(0);
    frameLayout->setSpacing(0);
    QSpacerItem* spacer  = new QSpacerItem( 180, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    frameLayout->addItem( spacer );
    QTextEdit* textWidget = new QTextEdit(QString(""),errorDlg);
    textWidget->setReadOnly(true);
    textWidget->setLineWrapMode(QTextEdit::NoWrap);
    errorDlgLayout->addWidget( textWidget );
    QPushButton * closeB = new QPushButton( "Close", frame);
    frameLayout->addWidget( closeB );
    errorDlgLayout->addWidget( frame );
    QWidget::connect(closeB, SIGNAL(clicked()), errorDlg, SLOT(hide()));
    errorDlg->resize( QSize(400, 250).expandedTo(errorDlg->minimumSizeHint()) );
    textWidget->setText(QString(errors.c_str()));
    errorDlg->show();
  }
  enableElements(false);
  // this call force initialization of status bar
  updateCurrentGraphInfos();
  int argc = qApp->argc();
  if (argc>1) {
    char ** argv = qApp->argv();
    for (int i=1;i<argc;++i) {
      QFileInfo info(argv[i]);
      QString s = info.absoluteFilePath();
      fileOpen(0, s);
    }
  }

  pluginsUpdateChecker = new PluginsUpdateChecker(pluginLoader.pluginsList,this);
  connect(pluginsUpdateChecker, SIGNAL(checkFinished()), this,SLOT(deletePluginsUpdateChecker()));
  multiServerManager = pluginsUpdateChecker->getMultiServerManager();
}
//**********************************************************************
void viewGl::changeGraph(Graph *graph) {
  //cerr << __PRETTY_FUNCTION__ << " (Graph = " << (int)graph << ")" << endl;
  /*clearObservers();
  QFileInfo tmp(openFiles[(unsigned long)glWidget].name.c_str());
  GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
  param.setTexturePath(string(tmp.dir().path().toAscii().data()) + "/");
  glWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);
  QDir::setCurrent(tmp.dir().path() + "/");
  clusterTreeWidget->setGraph(graph);
  eltProperties->setGraph(graph);
  layerWidget->attachMainWidget(glWidget);
#ifdef STATS_UI
  statsWidget->setGlMainWidget(glWidget);
#endif
  redrawView();
  // this line has been moved after the call to redrawView to ensure
  // that a new created graph has all its view... properties created
  // (call to initProxies())
  propertiesWidget->setGraph(graph);
  // avoid too much notification when importing a graph
  // see fileOpen
  if (importedGraph != graph)
  initObservers();*/
  clusterTreeWidget->setGraph(graph);
  eltProperties->setGraph(graph);
  //layerWidget->attachMainWidget(glWidget);
#ifdef STATS_UI
  //statsWidget->setGlMainWidget(glWidget);
#endif
  currentGraph=graph;
  initMainView();
  propertiesWidget->setGraph(graph);
}
//**********************************************************************
void viewGl::hierarchyChangeGraph(Graph *graph) {
  /*#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << " (Graph = " << (int)graph << ")" << endl;
#endif
  if( glWidget == 0 ) return;
  if (glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph() == graph)  return;
  //clearObservers();
  GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
  delete glWidget->getScene()->getGlGraphComposite();
  glWidget->getScene()->getLayer("Main")->deleteGlEntity("graph");
  GlGraphComposite* graphComposite=new GlGraphComposite(graph);
  glWidget->getScene()->addGlGraphCompositeInfo(glWidget->getScene()->getLayer("Main"),graphComposite);
  glWidget->getScene()->getLayer("Main")->addGlEntity(graphComposite,"graph");
  glWidget->getScene()->centerScene();
  //glWidget->getScene()->getGlGraphComposite()->getInputData()->setGraph(graph);
  glWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);*/
  changeGraph(graph);
  //initObservers();
}
//**********************************************************************
void viewGl::windowActivated(QWidget *w) {
  //cerr << __PRETTY_FUNCTION__ << " (QWidget = " << (int)w << ")" << endl;
  View *view = dynamic_cast<View*>(w);
  if(view){
    currentView=view;
    installInteractors(view);
    installEditMenu(view);
    //view->getWidget()->installEventFilter(this);
  }
  /*if (w==0)  {
    glWidget = 0;
    return;
  }
  if (typeid(*w)==typeid(GlMainWidget)) {
    glWidget=((GlMainWidget *)w);
    glWidget->resetInteractors(*currentInteractors);
    changeGraph(glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph());
    }*/
}
//**********************************************************************
void viewGl::installInteractors(View *view) {
  graphToolBar->clear();

  list<QAction *> interactorsList;
  view->getInteractorsActionList(interactorsList);

  for(list<QAction *>::iterator it=interactorsList.begin();it!=interactorsList.end();++it) {
    cout << "Install action : " << (*it)->text().toStdString() << endl;
    graphToolBar->addAction(*it);
  }

  if(interactorsList.size()!=0)
    changeInteractor(interactorsList.front());
}
//**********************************************************************
void viewGl::installEditMenu(View *view) {
  bool activate;
  QList <QAction *> actions = editMenu->actions();

  for(int i=0;i<actions.size();i++) {
    if(!actions.at(i)->isSeparator()) {
      
      string actionName=actions.at(i)->text().toStdString();
      if(actionName=="&Cut")
	activate=view->cutIsEnable();
      else if(actionName=="C&opy")
	activate=view->copyIsEnable();
      else if(actionName=="&Paste") 
	activate=view->pasteIsEnable();
      else if(actionName=="&Find...")
	activate=view->findIsEnable();
      else if(actionName=="Select All")
	activate=view->selectAllIsEnable();
      else if(actionName=="Delete selection")
	activate=view->delSelectionIsEnable();
      else if(actionName=="Deselect All")
	activate=view->deselectAllIsEnable();
      else if(actionName=="Invert selection")
	activate=view->invertSelectionIsEnable();
      else if(actionName=="Create group")
	activate=view->createGroupIsEnable();
      else if(actionName=="Create subgraph")
	activate=view->createSubgraphIsEnable();
      else{
	activate=false;
	cout << "ERROR : edit action " << actionName << " unknow" << endl;
      }

      actions.at(i)->setEnabled(activate);
    }
  }
}
//**********************************************************************
GlMainView* viewGl::initMainView(string *in) {
  workspace->closeAllWindows();

  View* newView=createView("MainView",currentGraph);
  
  return (GlMainView*)newView;
  
}
//==================================================
void viewGl::showElementProperties(unsigned int eltId, bool isNode) {
  cout << "show element property" << endl;
  if (isNode)
    eltProperties->setCurrentNode(currentGraph,  tlp::node(eltId));
  else
    eltProperties->setCurrentEdge(currentGraph,  tlp::edge(eltId));
  // show 'Element' tab in 'Info Editor'
  QWidget *tab = eltProperties->parentWidget();
  QTabWidget *tabWidget = (QTabWidget *) tab->parentWidget()->parentWidget();
  tabWidget->setCurrentIndex(tabWidget->indexOf(tab));
}
//**********************************************************************
/*GlMainWidget * viewGl::newOpenGlView(Graph *graph, const QString &name) {
  assert(graph != 0);
  // delete plugins loading errors dialog if needed
  if (errorDlg) {
    delete errorDlg;
    errorDlg = (QDialog *) NULL;
  }
  //Create 3D graph view
  GlMainWidget *glWidget = new GlMainWidget(workspace, name.toAscii().data());
  glWidget->getScene()->addObserver(this);
  layerWidget->attachMainWidget(glWidget);
  //GlMainWidget *glWidget = new GlMainWidget();
  workspace->addWindow(glWidget);
  //GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
  //assert(glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph() == 0);
  //Camera *camera=new Camera(glWidget->getScene());
  
  glWidget->move(0,0);
  glWidget->setWindowTitle(name);
  glWidget->setMinimumSize(0, 0);
  glWidget->resize(500,500);
  glWidget->setMaximumSize(32767, 32767);
  //glWidget->setBackgroundMode(Qt::PaletteBackground);  
  glWidget->installEventFilter(this);
  glWidget->resetInteractors(*currentInteractors);
  connect(glWidget, SIGNAL(closing(GlMainWidget *, QCloseEvent *)), this, SLOT(glMainWidgetClosing(GlMainWidget *, QCloseEvent *)));

  if(elementsDisabled)
    enableElements(true);

  //cerr << __PRETTY_FUNCTION__ << "...END" << endl;
  return glWidget;
  }*/
//**********************************************************************
/*void viewGl::constructDefaultScene(GlMainWidget *glWidget) {
  GlLayer* layer=new GlLayer("Main");
  GlLayer *backgroundLayer=new GlLayer("Background");
  backgroundLayer->setVisible(false);
  GlLayer *foregroundLayer=new GlLayer("Foreground");
  foregroundLayer->setVisible(false);

  backgroundLayer->set2DMode();
  foregroundLayer->set2DMode();
  string dir=TulipLibDir;
  dir += "tlp/bitmaps/";
  GlRectTextured *background=new GlRectTextured(0,1.,0,1.,dir + "tex_back.png",true);
  backgroundLayer->addGlEntity(background,"background");

  GlRectTextured *labri=new GlRectTextured(5.,55.,5.,55.,dir + "logolabri.jpg");
  foregroundLayer->addGlEntity(labri,"labrilogo");

  GlComposite *hulls=new GlComposite;
  hulls->setVisible(false);
  layer->addGlEntity(hulls,"Hulls");

  glWidget->getScene()->addLayer(backgroundLayer);
  glWidget->getScene()->addLayer(layer);
  glWidget->getScene()->addLayer(foregroundLayer);
  }*/
//**********************************************************************
void viewGl::new3DView() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  /*if (!glWidget) return;
  GlMainWidget *newGlWidget =
    newOpenGlView(glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph(), 
		  glWidget->parentWidget()->windowTitle());
  constructDefaultScene(newGlWidget);
  newGlWidget->getScene()->addGlGraphCompositeInfo(glWidget->getScene()->getLayer("Main"),glWidget->getScene()->getGlGraphComposite());
  newGlWidget->getScene()->getLayer("Main")->addGlEntity(glWidget->getScene()->getGlGraphComposite(),"graph");
  newGlWidget->getScene()->getGlGraphComposite()->setRenderingParameters(glWidget->getScene()->getGlGraphComposite()->getRenderingParameters());
  newGlWidget->getScene()->setBackgroundColor(glWidget->getScene()->getBackgroundColor());
  newGlWidget->getScene()->centerScene();
  newGlWidget->show();
  //  cerr << __PRETTY_FUNCTION__ << "...END" << endl;*/
}
//**********************************************************************
void viewGl::fileNew() {
  Observable::holdObservers();
  Graph *newGraph=tlp::newGraph();
  initializeGraph(newGraph);
  /*GlMainWidget *glW =
    newOpenGlView(newGraph,
		  newGraph->getAttribute<string>(std::string("name")).c_str());
  constructDefaultScene(glW);
  GlGraphComposite* glGraphComposite=new GlGraphComposite(newGraph);
  glW->getScene()->getLayer("Main")->addGlEntity(glGraphComposite,"graph");
  glW->getScene()->addGlGraphCompositeInfo(glW->getScene()->getLayer("Main"),glGraphComposite);
  initializeGlScene(glW->getScene());
  Observable::unholdObservers();
  glW->show();*/
  currentGraph=newGraph;
  changeGraph(currentGraph);
  initMainView();
}
//**********************************************************************
void viewGl::setNavigateCaption(string newCaption) {
   QWidgetList windows = workspace->windowList();
   for( int i = 0; i < int(windows.count()); ++i ) {
     QWidget *win = windows.at(i);
     /*if (typeid(*win)==typeid(GlMainWidget)) {
       GlMainWidget *tmpNavigate = dynamic_cast<GlMainWidget *>(win);
       if(tmpNavigate == glWidget) {
	 tmpNavigate->setWindowTitle(newCaption.c_str());
	 return;
       }
       }*/
   }
}
//**********************************************************************
bool viewGl::doFileSave() {
  /*if (!glWidget) return false;
  if (openFiles.find((unsigned long)glWidget)==openFiles.end() || 
      (openFiles[(unsigned long)glWidget].name == "")) {
    return doFileSaveAs();
  }
  viewGlFile &vFile = openFiles[(unsigned long)glWidget];
  return doFileSave("tlp", vFile.name, vFile.author, vFile.comments);*/
}
//**********************************************************************
void viewGl::fileSave() {
  doFileSave();
}
//**********************************************************************
static void setGraphName(Graph *g, QString s) {
  QString cleanName=s.section('/',-1);
  QStringList fields = cleanName.split('.');
  cleanName=cleanName.section('.', -fields.count(), -2);
  g->setAttribute("name", string(cleanName.toAscii().data()));
}
//**********************************************************************
bool viewGl::doFileSave(string plugin, string filename, string author, string comments) {
  /*if (!glWidget) return false;
  DataSet dataSet;
  StructDef parameter = ExportModuleFactory::factory->getPluginParameters(plugin);
  parameter.buildDefaultDataSet(dataSet);
  if (author.length() > 0)
    dataSet.set<string>("author", author);
  if (comments.length() > 0)
    dataSet.set<string>("text::comments", comments);
  if (!tlp::openDataSetDialog(dataSet, 0, &parameter,
			      &dataSet, "Enter Export parameters", NULL,
			      this)) //, glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph())
    return false;
  dataSet.set("displaying", glWidget->getScene()->getGlGraphComposite()->getRenderingParameters().getParameters());
  string sceneOut;
  glWidget->getScene()->getXML(sceneOut);
  string dir=TulipLibDir;
  while(sceneOut.find(dir) != std::string::npos) {
    int pos=sceneOut.find(dir);
    sceneOut.replace(pos,dir.length()-1,"TulipLibDir");
  }
  dataSet.set<string>("scene", sceneOut);
  if (filename.length() == 0) {
    QString name;
    if (plugin == "tlp")
      name =
	QFileDialog::getSaveFileName(this, tr("Choose a file to save" ),
				     QString(),
				     tr("Tulip graph (*.tlp *.tlp.gz)"));
    else
      name =
	QFileDialog::getSaveFileName(this,
				     this->windowTitle().toAscii().data());
    if (name == QString::null) return false;
    filename = name.toAscii().data();
  }
  bool result;
  ostream *os;
  if (filename.rfind(".gz") == (filename.length() - 3)) 
    os = tlp::getOgzstream(filename.c_str());
  else {
    if ((plugin == "tlp") && (filename.rfind(".tlp") == std::string::npos))
      filename += ".tlp";
    os = new ofstream(filename.c_str());
  }

  // keep trace of file infos
  viewGlFile &vFile = openFiles[(unsigned long)glWidget];
  vFile.name = filename;
  dataSet.get<string>("author", vFile.author);
  dataSet.get<string>("text::comments", vFile.comments);

  if (!(result=tlp::exportGraph(glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph(), *os, plugin, dataSet, NULL))) {
    QMessageBox::critical( 0, "Tulip export Failed",
			   "The file has not been saved"
			   );
  } else {
    statusBar()->showMessage((filename + " saved.").c_str());
  }
  setNavigateCaption(filename);
  setGraphName(glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph(), QString(filename.c_str()));
  delete os;
  return result;*/
}
//**********************************************************************
bool viewGl::doFileSaveAs() {
  //if (!glWidget || !glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph())
    return false;
    //return doFileSave("tlp", "", "", "");
}
//**********************************************************************
void viewGl::fileSaveAs() {
  doFileSaveAs();
}
//**********************************************************************
void viewGl::fileOpen() {
  QString s;
  fileOpen(0,s);
}
//**********************************************************************
void viewGl::initializeGraph(Graph *graph) {
  // graph->setAttribute("name", newName());
  graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  graph->getProperty<SizeProperty>("viewSize")->setAllEdgeValue(Size(0.125,0.125,0.5));
  graph->getProperty<ColorProperty>("viewColor")->setAllNodeValue(Color(255,0,0));
  graph->getProperty<ColorProperty>("viewColor")->setAllEdgeValue(Color(0,0,0));
  graph->getProperty<IntegerProperty>("viewShape")->setAllNodeValue(1);
  graph->getProperty<IntegerProperty>("viewShape")->setAllEdgeValue(0);
}
//**********************************************************************
/*void viewGl::initializeGlScene(GlScene *scene) {
  GlGraphRenderingParameters param = scene->getGlGraphComposite()->getRenderingParameters();
  param.setViewArrow(true);
  param.setDisplayEdges(true);
  param.setFontsType(1);
  param.setFontsPath(((Application *)qApp)->bitmapPath);
  param.setViewNodeLabel(true);
  //scene->setBackgroundColor(Color(255,255,255));
  scene->setViewOrtho(true);
  param.setElementOrdered(false);
  param.setEdgeColorInterpolate(false);
  Camera cam = *scene->getCamera(); //default value for drawing small graph in the window
  cam.setCenter(Coord(0, 0,  0));
  cam.setEyes(Coord(0, 0, 10));
  cam.setUp(Coord(0, 1,  0));
  cam.setZoomFactor(0.5);
  cam.setSceneRadius(10);
  scene->setCamera(&cam);
  //scene->setRenderingParameters(param);
  }*/
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
// we use a hash_map to store plugin parameters
static StructDef *getPluginParameters(TemplateFactoryInterface *factory, std::string name) {
  static stdext::hash_map<unsigned long, stdext::hash_map<std::string, StructDef * > > paramMaps;
  stdext::hash_map<std::string, StructDef *>::const_iterator it;
  it = paramMaps[(unsigned long) factory].find(name);
  if (it == paramMaps[(unsigned long) factory].end())
    paramMaps[(unsigned long) factory][name] = new StructDef(factory->getPluginParameters(name));
  return paramMaps[(unsigned long) factory][name];
}
//**********************************************************************
void viewGl::fileOpen(string *plugin, QString &s) {
  Observable::holdObservers();
  DataSet dataSet;
  string tmpStr="tlp";
  bool cancel=false, noPlugin = true;
  if (s == QString::null) {
    if (plugin==0) {
      plugin = &tmpStr;
      s = QFileDialog::getOpenFileName(this, tr("Choose a file to open" ),
				       QString(),
				       tr("Tulip graph (*.tlp *.tlp.gz)"));

      if (s == QString::null)
	cancel=true;
      else
	dataSet.set("file::filename", string(s.toAscii().data()));
    }
    else {
      noPlugin = false;
      s = QString::null;
      StructDef sysDef = ImportModuleFactory::factory->getPluginParameters(*plugin);
      StructDef *params = getPluginParameters(ImportModuleFactory::factory, *plugin);
      params->buildDefaultDataSet( dataSet );
      cancel = !tlp::openDataSetDialog(dataSet, &sysDef, params, &dataSet,
				       "Enter plugin parameter(s)", NULL, this);
    }
  } else {
    plugin = &tmpStr;
    dataSet.set("file::filename", string(s.toAscii().data()));
    noPlugin = true;
  }
  if (!cancel) {
    if(noPlugin) {
      QWidgetList windows = workspace->windowList();
      for( int i = 0; i < int(windows.count()); ++i ) {
	QWidget *win = windows.at(i);
	if (typeid(*win)==typeid(GlMainWidget)) {
	  GlMainWidget *tmpNavigate = dynamic_cast<GlMainWidget *>(win);
	  if(openFiles[((unsigned long)tmpNavigate)].name == s.toAscii().data()) {
	    int answer = QMessageBox::question(this, "Open", "This file is already opened. Do you want to load it anyway?",  
					       QMessageBox::Yes,  QMessageBox::No);
	    if(answer == QMessageBox::No)
	      return;
	    break;
	  }
	}
      }
    }
    
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    Graph *newGraph = tlp::newGraph();
    initializeGraph(newGraph);
    if (s == QString::null)
      s = newGraph->getAttribute<string>("name").c_str();
    bool result=true;
    QFileInfo tmp(s);
    QDir::setCurrent(tmp.dir().path() + "/");
    currentGraph=newGraph;
    //GlMainWidget *glW = newOpenGlView(newGraph, s);
    //    changeGraph(0);
    QtProgress *progressBar = new QtProgress(this, string("Loading : ")+ s.section('/',-1).toAscii().data());
    // this will avoid too much notification when
    // importing a graph (see changeGraph)
    importedGraph = newGraph;
    result = tlp::importGraph(*plugin, dataSet, progressBar ,newGraph);
    importedGraph = 0;
    // now ensure notification
    //initObservers();
    if (progressBar->state()==TLP_CANCEL || !result ) {
      //      changeGraph(0);
      //delete glW;
      delete newGraph;
      QApplication::restoreOverrideCursor();
      // qWarning("Canceled import/Open");
      std::string errorTitle("Canceled import/Open: ");
      errorTitle += s.section('/',-1).toAscii().data();
      std::string errorMsg = progressBar->getError();
      delete progressBar;
      Observable::unholdObservers();
      QMessageBox::critical(this, errorTitle.c_str(), errorMsg.c_str());
      return;
    }
    delete progressBar;
    if(noPlugin)
      setGraphName(newGraph, s);

    string sceneData;
    dataSet.get<std::string>("scene", sceneData);

    if(!sceneData.empty()) {
      string dir=TulipLibDir;
      string name="TulipLibDir";
      while(sceneData.find(name)!= string::npos) {
	int pos=sceneData.find(name);
	sceneData.replace(pos,name.length(),dir);
      }
      initMainView(&sceneData);
    } else {
      GlMainView *mainView=initMainView();

      DataSet displaying;
      Color color;
      if(dataSet.get<DataSet>("displaying", displaying)) {
	if(displaying.get<Color>("backgroundColor", color)) {
	  mainView->getGlMainWidget()->getScene()->setBackgroundColor(color);
	}
      }
    }

    changeGraph(newGraph);
    //initializeGlScene(glW->getScene());
    

    /*if(noPlugin) {
      viewGlFile vFile;
      vFile.name = s.toAscii().data();
      dataSet.get<std::string>("author", vFile.author);
      dataSet.get<std::string>("text::comments", vFile.comments);
      openFiles[((unsigned long)glW)] = vFile;
      }*/
    QApplication::restoreOverrideCursor();
    //    changeGraph(0);
    bool displayingInfoFound = false;


    /*GlGraphRenderingParameters param = glW->getScene()->getGlGraphComposite()->getRenderingParameters();
    DataSet glGraphData;
    if (dataSet.get<DataSet>("displaying", glGraphData)) {
      param.setParameters(glGraphData);
      glW->getScene()->getGlGraphComposite()->setRenderingParameters(param);
      displayingInfoFound = true;
    }
    //if (!displayingInfoFound)
      glW->getScene()->centerScene();

    // glWidget will be bind to that new GlMainWidget
    // in the windowActivated method,
    // so let it be activated
    glW->show();*/
    qApp->processEvents();

    // show current subgraph if any
    /*int id = 0;
    if (glGraphData.get<int>("SupergraphId", id) && id) {
      Graph *subGraph = getCurrentSubGraph(newGraph, id);
      if (subGraph)
	hierarchyChangeGraph(subGraph);
	}*/
    
    // Ugly hack to handle old Tulip 2 file
    // to remove in future version
    /*Coord sr;
    if (glGraphData.get<Coord>("sceneRotation", sr)) {
      // only recenter view
      // because setRenderingParameters
      // no longer deals with sceneRotation and sceneTranslation
      centerView();
      // update viewColor alpha channel
      // which was not managed in Tulip 2
      ColorProperty *colors =
	newGraph->getProperty<ColorProperty>("viewColor");
      node n;
      forEach(n, newGraph->getNodes()) {
	Color color = colors->getNodeValue(n);
	if (!color.getA())
	  color.setA(200);
	colors->setNodeValue(n, color);
      }
      edge e;
      forEach(e, newGraph->getEdges()) {
	Color color = colors->getEdgeValue(e);
	if (!color.getA())
	  color.setA(200);
	colors->setEdgeValue(e, color);
      }
      }*/

    // synchronize overview display parameters
  }
  /* else {
    qWarning("Canceled  Open/import");
  } */
  Observable::unholdObservers();
}
//**********************************************************************
void viewGl::importGraph(QAction* action) {
  string name = action->text().toStdString();
  QString s;
  fileOpen(&name,s);
}

//**********************************************************************
void viewGl::setParameters(const DataSet& data) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  /*GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
  param.setParameters(data);
  glWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);*/
  clusterTreeWidget->setGraph(currentGraph);
  eltProperties->setGraph(currentGraph);
  propertiesWidget->setGraph(currentGraph);
  }
//**********************************************************************
void viewGl::updateCurrentGraphInfos() {
  static QLabel *currentGraphInfosLabel = 0;
  if (!currentGraphInfosLabel) {
    statusBar()->addWidget(new QLabel(statusBar()), true);
    currentGraphInfosLabel = new QLabel(statusBar());
    statusBar()->addWidget(currentGraphInfosLabel);
  }
  char tmp[255];
  sprintf(tmp,"nodes:%d, edges:%d", currentGraphNbNodes, currentGraphNbEdges);
  currentGraphInfosLabel->setText(tmp);
  clusterTreeWidget->updateCurrentGraphInfos(currentGraphNbNodes, currentGraphNbEdges);
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
  //cout << subMenu->name() << "->" << itemName << endl;
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
//**********************************************************************
void viewGl::buildMenus() {
  //Properties PopMenus
  buildPropertyMenu<IntegerType, IntegerType, IntegerAlgorithm>(intMenu, this, SLOT(changeInt(QAction*)));
  buildPropertyMenu<StringType, StringType, StringAlgorithm>(stringMenu, this, SLOT(changeString(QAction*)));
  buildPropertyMenu<SizeType, SizeType, SizeAlgorithm>(sizesMenu, this, SLOT(changeSize(QAction*)));
  buildPropertyMenu<ColorType, ColorType, ColorAlgorithm>(colorsMenu, this, SLOT(changeColor(QAction*)));
  buildPropertyMenu<PointType, LineType, LayoutAlgorithm>(layoutMenu, this, SLOT(changeLayout(QAction*)));
  buildPropertyMenu<DoubleType, DoubleType, DoubleAlgorithm>(metricMenu, this, SLOT(changeMetric(QAction*)));
  buildPropertyMenu<BooleanType, BooleanType, BooleanAlgorithm>(selectMenu, this, SLOT(changeSelection(QAction*)));

  buildMenuWithContext<AlgorithmFactory, Algorithm>(generalMenu, this, SLOT(applyAlgorithm(QAction*)));
  buildMenuWithContext<ExportModuleFactory, ExportModule>(exportGraphMenu, this, SLOT(exportGraph(QAction*)));
  buildMenuWithContext<ImportModuleFactory, ImportModule>(importGraphMenu, this, SLOT(importGraph(QAction*)));
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
    exportImageMenu.addAction(str);
  //==============================================================
  //File Menu 
  fileMenu->addSeparator();
  if (importGraphMenu.actions().count()>0) {
    importGraphMenu.setTitle("&Import");
    fileMenu->addMenu(&importGraphMenu);
  }
  if (exportGraphMenu.actions().count()>0) {
    exportGraphMenu.setTitle("&Export");
    fileMenu->addMenu(&exportGraphMenu);
  }
  if (exportImageMenu.actions().count()>0) {
    exportImageMenu.setTitle("&Save Picture as ");
    fileMenu->addMenu(&exportImageMenu); //this , SLOT( outputImage() ));
  }
  //Property Menu
  if (selectMenu.actions().count()>0) {
    selectMenu.setTitle("&Selection");
    propertyMenu->addMenu(&selectMenu);
  }
  if (colorsMenu.actions().count()>0) {
    colorsMenu.setTitle("&Color");
    propertyMenu->addMenu(&colorsMenu);
  }
  if (metricMenu.actions().count()>0) {
    metricMenu.setTitle("&Measure");
    propertyMenu->addMenu(&metricMenu);
  }
  if (intMenu.actions().count()>0) {
    intMenu.setTitle("&Integer");
    propertyMenu->addMenu(&intMenu);
  }
  if (layoutMenu.actions().count()>0) {
    layoutMenu.setTitle("&Layout");
    propertyMenu->addMenu(&layoutMenu);
  }
  if (sizesMenu.actions().count()>0) {
    sizesMenu.setTitle("S&ize");
    propertyMenu->addMenu(&sizesMenu);
  }
  if (stringMenu.actions().count()>0) {
    stringMenu.setTitle("L&abel");
    propertyMenu->addMenu(&stringMenu);
  }
  if (generalMenu.actions().count()>0) {
    generalMenu.setTitle("&General");
    propertyMenu->addMenu(&generalMenu);
  }
  //Tools menu
  MutableContainer<ViewCreator *> views;
  ViewPluginsManager::getInst().initViewPluginsList(views);
  TemplateFactory<ViewCreatorFactory, ViewCreator, ViewCreatorContext>::ObjectCreator::const_iterator it;
  for (it=ViewCreatorFactory::factory->objMap.begin();it != ViewCreatorFactory::factory->objMap.end();++it) {
    toolsMenu->addAction(it->first.c_str());/*QMenu *newTool = new QMenu(it->first.c_str(), toolsMenu);
    newTool->setObjectName(QString(it->first.c_str()));
    toolsMenu->addMenu(newTool);*/
  }
}
//**********************************************************************
void viewGl::outputEPS() {
  /*if (!glWidget) return;
  QString s( QFileDialog::getSaveFileName());
  if (!s.isNull()) {
    if (glWidget->outputEPS(64000000,true,s.toAscii().data()))
      statusBar()->showMessage(s + " saved.");
    else
      QMessageBox::critical( 0, "Save Picture Failed",
			     "The file has not been saved."
			     );
			     }*/
}
//**********************************************************************
void viewGl::outputSVG() {
  /*if (!glWidget) return;
  QString s( QFileDialog::getSaveFileName());
  if (!s.isNull()) {
    if (glWidget->outputSVG(64000000,s.toAscii().data()))
      statusBar()->showMessage(s + " saved.");
    else
      QMessageBox::critical( 0, "Save Picture Failed",
			     "The file has not been saved."
			     );
			     }*/
}
//**********************************************************************
void viewGl::exportImage(QAction* action) {
  /*if (!glWidget) return;
  string name = action->text().toStdString();
  if (name=="EPS") {
    outputEPS();
    return;
  } else if (name=="SVG") {
    outputSVG();
    return;
  }
  QString s(QFileDialog::getSaveFileName());
  if (s.isNull()) return;    
  int width,height;
  width = glWidget->width();
  height = glWidget->height();
  unsigned char* image= glWidget->getImage();
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
  statusBar()->showMessage(s + " saved.");*/
}
//**********************************************************************
void viewGl::exportGraph(QAction* action) {
  /*if (!glWidget) return;*/
  doFileSave(action->text().toStdString(), "", "", "");
}
//**********************************************************************
void viewGl::windowsMenuActivated(QAction* action) {
  int id = action->data().toInt();
  QWidget* w = workspace->windowList().at(id);
  if ( w ) {
    w->setFocus();
    w->show();
  }
}
//**********************************************************************
void viewGl::windowsMenuAboutToShow() {
  windowsMenu->clear();
  QAction* cascadeAction = windowsMenu->addAction("&Cascade", workspace, SLOT(cascade() ) );
  QAction* tileAction = windowsMenu->addAction("&Tile", workspace, SLOT(tile() ) );
  if ( workspace->windowList().isEmpty() ) {
    cascadeAction->setEnabled(false);
    tileAction->setEnabled(false);
  }
  windowsMenu->addSeparator();
  QWidgetList windows = workspace->windowList();
  for ( int i = 0; i < int(windows.count()); ++i ) {
    QAction* action = windowsMenu->addAction(windows.at(i)->windowTitle());
    action->setChecked(workspace->activeWindow() == windows.at(i));
    action->setData(QVariant(i));
  }
}
//**********************************************************************
/* returns true if user canceled */
bool viewGl::askSaveGraph(const std::string name) {
  string message = "Do you want to save this graph: " + name + " ?";
  int answer = QMessageBox::question(this, "Save", message.c_str(),
    QMessageBox::Yes | QMessageBox::Default,
    QMessageBox::No,
    QMessageBox::Cancel | QMessageBox::Escape);
  switch(answer) {
    case QMessageBox::Cancel : return true;
    case QMessageBox::Yes: return !doFileSave();
    default: return false;
  }
}
//**********************************************************************
/* returns true if window agrees to be closed */ 
bool viewGl::closeWin() {
  /*set<unsigned int> treatedGraph;
  QWidgetList windows = workspace->windowList();
  for(int i = 0; i < int(windows.count()); ++i ) {
    QWidget *win = windows.at(i);
    if (typeid(*win)==typeid(GlMainWidget)) {
      GlMainWidget *tmpNavigate = dynamic_cast<GlMainWidget *>(win);
      Graph *graph = tmpNavigate->getScene()->getGlGraphComposite()->getInputData()->getGraph()->getRoot();
      if(!alreadyTreated(treatedGraph, graph)) {
        glWidget = tmpNavigate;
        bool canceled = askSaveGraph(graph->getAttribute<string>("name"));
        if (canceled)
          return false;
	treatedGraph.insert((unsigned long)graph);
      }
    }
    } */
  return true;
}
//**********************************************************************
int viewGl::alreadyTreated(set<unsigned int> treatedGraph, Graph *graph) {
  set<unsigned int>::iterator iterator = treatedGraph.begin();
  while(iterator != treatedGraph.end()) {
    unsigned int currentGraph = *iterator;
    if(currentGraph == (unsigned long)graph)
      return true;
    iterator++;
  }
  return false;
}
//**********************************************************************
void viewGl::closeEvent(QCloseEvent *e) {
  if (closeWin())
    e->accept();
  else
    e->ignore();
}
//**********************************************************************
bool viewGl::eventFilter(QObject *obj, QEvent *e) {
  // With Qt4 software/src/tulip/ElementTooltipInfo.cpp
  // is no longer needed; the tooltip implementation must take place
  // in the event() method inherited from QWidget.
  /*if (obj->inherits("GlMainWidget") &&
      e->type() == QEvent::ToolTip && tooltips->isChecked()) {
    node tmpNode;
    edge tmpEdge;
    ElementType type;
    QString tmp;
    QHelpEvent *he = static_cast<QHelpEvent *>(e);
    if (((GlMainWidget *) obj)->doSelect(he->pos().x(), he->pos().y(), type, tmpNode, tmpEdge)) {
      // try to show the viewLabel if any
      StringProperty *labels = ((GlMainWidget *) obj)->getScene()->getGlGraphComposite()->getInputData()->getGraph()->getProperty<StringProperty>("viewLabel");
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
      return true;
    }
  }
  if ( obj->inherits("GlMainWidget") &&
       (e->type() == QEvent::MouseButtonRelease)) {
    QMouseEvent *me = (QMouseEvent *) e;
    if (me->button()==Qt::RightButton) {
      bool result;
      ElementType type;
      node tmpNode;
      edge tmpEdge;
      // look if the mouse pointer is over a node or edge
      result = glWidget->doSelect(me->x(), me->y(), type, tmpNode, tmpEdge);
      if (!result)
      return false;
      // Display a context menu
      bool isNode = type == NODE;
      int itemId = isNode ? tmpNode.id : tmpEdge.id;
      QMenu contextMenu(this);
      stringstream sstr;
      sstr << (isNode ? "Node " : "Edge ") << itemId;
      contextMenu.addAction(tr(sstr.str().c_str()))->setEnabled(false);
      contextMenu.addSeparator();
      contextMenu.addAction(tr("Add to/Remove from selection"));
      QAction* selectAction = contextMenu.addAction(tr("Select"));
      QAction* deleteAction = contextMenu.addAction(tr("Delete"));
      contextMenu.addSeparator();
      Graph *graph=glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
      QAction* goAction = NULL;
      QAction* ungroupAction = NULL;
      if (isNode) {
	GraphProperty *meta = graph->getProperty<GraphProperty>("viewMetaGraph");
	if (meta->getNodeValue(tmpNode)!=0) {
	  goAction = contextMenu.addAction(tr("Go inside"));
	  ungroupAction = contextMenu.addAction(tr("Ungroup"));
	}
      }
      if (goAction != NULL)
	contextMenu.addSeparator();
      QAction* propAction = contextMenu.addAction(tr("Properties"));
      QAction* menuAction = contextMenu.exec(me->globalPos(), selectAction);
      if (menuAction == NULL)
	return true;
      Observable::holdObservers();
      if (menuAction == deleteAction) { // Delete
	// delete graph item
	if (isNode)
	  graph->delNode(node(itemId));
	else
	  graph->delEdge(edge(itemId));
      } else {
	if (menuAction == propAction) // Properties
	  showElementProperties(itemId, isNode);
	else  {
	  if (menuAction == goAction) { // Go inside
	    GraphProperty *meta = graph->getProperty<GraphProperty>("viewMetaGraph");
	    changeGraph(meta->getNodeValue(tmpNode));
	  }
	  else  {
	    if (menuAction == ungroupAction) { // Ungroup
	      tlp::openMetaNode(graph, tmpNode);
	      clusterTreeWidget->update();
	    } else {
	      BooleanProperty *elementSelected = graph->getProperty<BooleanProperty>("viewSelection");
	      if (menuAction == selectAction) { // Select
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
      return true;
    }
    return false;
    }*/
  return false;
}
//**********************************************************************
void viewGl::focusInEvent ( QFocusEvent * ) {
}
//**********************************************************************
View* viewGl::createView(const string &name,Graph *graph,string *xmlData){
  
  MutableContainer<ViewCreator *> views;
  ViewPluginsManager::getInst().initViewPluginsList(views);
  View *newView=views.get(ViewPluginsManager::getInst().viewPluginId(name))->create(workspace);
  connect(newView, SIGNAL(showElementPropertiesSignal(unsigned int, bool)),this,SLOT(showElementProperties(unsigned int, bool)));


  newView->setData(graph,xmlData);
  graph->addObserver(newView);

  if(elementsDisabled)
    enableElements(true);
 
  displayView(newView,name);
  return newView;
}
//**********************************************************************
void viewGl::addView(QAction *action) {
  createView(action->text().toStdString(),currentGraph);
}
//**********************************************************************
void viewGl::displayView(View *view,const string &name){
  workspace->addWindow(view->getWidget());
  view->getWidget()->move(0,0);
  view->getWidget()->setWindowTitle(name.c_str());
  view->getWidget()->setMinimumSize(0, 0);
  view->getWidget()->resize(500,500);
  view->getWidget()->setMaximumSize(32767, 32767);
  view->getWidget()->show();
}
//**********************************************************************
void viewGl::changeInteractor(QAction* action){
  if(currentView){
    QList<QAction*> actions=graphToolBar->actions();
    for(QList<QAction*>::iterator it=actions.begin();it!=actions.end();++it) {
      (*it)->setChecked(false);
    }
    action->setCheckable(true);
    action->setChecked(true);
    Iterator<Interactor *> *interactors=currentView->installInteractor(action->text().toStdString());
    while(interactors->hasNext()) {
      Interactor *interactor=interactors->next();
      interactor->setView(currentView);
    }
  }
}

//==============================================================
void viewGl::graphAboutToBeRemoved(Graph *sg) {
  //  cerr << __PRETTY_FUNCTION__ <<  "Possible bug" << endl;
  /*GlGraphRenderingParameters param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
  //param.setGraph(0);
  glWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);
  overviewWidget->setObservedView(glWidget);*/
}
//==============================================================
void viewGl::plugins() {
  PluginsManagerDialog *pluginsManager=new PluginsManagerDialog(multiServerManager,this);
  
  /*QDialog dialog;
  QVBoxLayout layout;
  layout.setContentsMargins(0,0,0,0);
  PluginsManagerMainWindow pluginsManager(pluginLoader.pluginsList);
  
  layout.addWidget(&pluginsManager);

  dialog.setLayout(&layout);*/
  pluginsManager->exec();
  if (pluginsManager->pluginUpdatesPending()) {
    int result = QMessageBox::warning(this, 
				      tr("Update plugins"), 
				      tr("To finish installing/removing plugins \nTulip must be restart.\nDo you want to exit Tulip now ?"),
				      QMessageBox::Yes | QMessageBox::Default,
				      QMessageBox::No);
    if(result == QMessageBox::Yes)
      fileExit();
  }
}
//==============================================================
void viewGl::deletePluginsUpdateChecker(){
  disconnect(pluginsUpdateChecker, SIGNAL(checkFinished()), this,SLOT(deletePluginsUpdateChecker()));
  delete pluginsUpdateChecker;
}
//==============================================================
void viewGl::editCut() {
  if(currentView)
    currentView->cut();
}
//==============================================================
void viewGl::editCopy() {
  if(currentView)
    currentView->copy();
}
//==============================================================
void viewGl::editPaste() {
  if(currentView)
    currentView->paste();
}
//==============================================================
void viewGl::editFind() {
  if(currentView)
    currentView->find();
}
//==============================================================
void viewGl::editCreateGroup() {
  if(currentView)
    currentView->createGroup();
}
//==============================================================
void viewGl::editCreateSubGraph() {
  if(currentView)
    currentView->createSubgraph();
}
//==============================================================
void viewGl::editDelSelection() {
  if(currentView)
    currentView->delSelection();
}
//==============================================================
void viewGl::editReverseSelection() {
  if(currentView)
    currentView->invertSelection();
}
//==============================================================
void viewGl::editSelectAll() {
  if(currentView)
    currentView->selectAll();
}
//==============================================================
void viewGl::editDeselectAll() {
  if(currentView)
    currentView->deselectAll();
}
//==============================================================
void viewGl::helpAbout() {
  if (aboutWidget==0)
    aboutWidget = new InfoDialog(this);
  aboutWidget->show();
}
//==============================================================
void viewGl::helpIndex() {
  QStringList cmdList;
  cmdList << "-profile" 
	  << QString( (tlp::TulipDocProfile).c_str());
  
  assistant->setArguments(cmdList);
  if ( !assistant->isOpen() ){
    assistant->showPage(QString( (tlp::TulipUserHandBookIndex).c_str()));
    assistant->openAssistant();
  }
  else	
    assistant->showPage(QString( (tlp::TulipUserHandBookIndex).c_str()));
}
//==============================================================
void viewGl::helpContents() {
  QStringList cmdList;
  cmdList << "-profile" 
	  << QString( (tlp::TulipDocProfile).c_str());
  
  assistant->setArguments(cmdList);
  if ( !assistant->isOpen() ){ 
    assistant->showPage(QString( (tlp::TulipUserHandBookIndex).c_str()));
    assistant->openAssistant();
  }
  else	
    assistant->showPage(QString( (tlp::TulipUserHandBookIndex).c_str()));
}
//==============================================================
void viewGl::helpAssistantError(const QString &msg) {
  cerr << msg.toAscii().data() << endl;
}
//==============================================================
void viewGl::fileExit() {
  if (closeWin())
    exit(EXIT_SUCCESS);
}
//==============================================================
void viewGl::filePrint() {
  /*if (!glWidget) return;
  Graph *graph=
    glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  if (graph==0) return;
  
  QPrinter printer;
  QPrintDialog dialog(&printer, this);
  if (!dialog.exec())
    return;
  int width,height;
  width = glWidget->width();
  height = glWidget->height();
  unsigned char* image= glWidget->getImage();
  QPainter painter(&printer);
  for (int y=0; y<height; y++)
    for (int x=0; x<width; x++) {
      painter.setPen(QColor(image[(height-y-1)*width*3+(x)*3],
			    image[(height-y-1)*width*3+(x)*3+1],
			    image[(height-y-1)*width*3+(x)*3+2]));
      painter.drawPoint(x,y);
    }
  painter.end();
  delete image;*/
}
//==============================================================
/*void viewGl::glMainWidgetClosing(GlMainWidget *glgw, QCloseEvent *event) {
  Graph *root = glgw->getScene()->getGlGraphComposite()->getInputData()->getGraph()->getRoot();
  QWidgetList windows = workspace->windowList();
  int i;
  for( i = 0; i < int(windows.count()); ++i ) {
    QWidget *win = windows.at(i);
    if (typeid(*win)==typeid(GlMainWidget)) {
      GlMainWidget *tmpNavigate = dynamic_cast<GlMainWidget *>(win);
      int graph1 = root->getId();
      int graph2 = tmpNavigate->getScene()->getGlGraphComposite()->getInputData()->getGraph()->getRoot()->getId();
      if((tmpNavigate != glgw) && (graph1 == graph2))
	break;
    }
  }
  if(i == int(windows.count())) {
    bool canceled = askSaveGraph(glgw->getScene()->getGlGraphComposite()->getInputData()->getGraph()->getAttribute<string>("name"));
    if (canceled) {
      event->ignore();
      return;
    }
    clusterTreeWidget->setGraph(0);
    propertiesWidget->setGlMainWidget(NULL);
    eltProperties->setGraph(0);
#ifdef STATS_UI
    statsWidget->setGlMainWidget(0);
#endif
    GlGraphRenderingParameters param = glgw->getScene()->getGlGraphComposite()->getRenderingParameters();
    //param.setGraph(0);
    glgw->getScene()->getGlGraphComposite()->setRenderingParameters(param);
  } else
    // no graph to delete
    root = (Graph *) 0;
  
  if (openFiles.find((unsigned long)glgw) != openFiles.end())   
    openFiles.erase((unsigned long)glgw);
  
  if(glgw == glWidget) {
    GlGraphRenderingParameters param = glgw->getScene()->getGlGraphComposite()->getRenderingParameters();
    //param.setGraph(0);
    glgw->getScene()->getGlGraphComposite()->setRenderingParameters(param);
    glWidget = 0;
    updateCurrentGraphInfos();
  }
  delete glgw;

  // if needed the graph must be deleted after
  // the GlGraphWidget because this one has to remove itself
  // from the graph observers list
  if (root)
    delete root;
  
  if(windows.count() == 1)
    enableElements(false);
    }*/
//**********************************************************************
/// Apply a general algorithm
void viewGl::applyAlgorithm(QAction* action) {
  /*clearObservers();
  if (glWidget==0) return;
  Observable::holdObservers();*/
  string name = action->text().toStdString();
  string erreurMsg;
  DataSet dataSet;
  Graph *graph=currentGraph;
  StructDef *params = getPluginParameters(AlgorithmFactory::factory, name);
  StructDef sysDef = AlgorithmFactory::factory->getPluginParameters(name);
  params->buildDefaultDataSet(dataSet, graph );
  bool ok = tlp::openDataSetDialog(dataSet, &sysDef, params, &dataSet,
				   "Tulip Parameter Editor", graph, this);
  if (ok) {
    QtProgress myProgress(this,name);
    myProgress.hide();
    if (!tlp::applyAlgorithm(graph, erreurMsg, &dataSet, name, &myProgress  )) {
      QMessageBox::critical( 0, "Tulip Algorithm Check Failed",QString((name + ":\n" + erreurMsg).c_str()));
    }
    clusterTreeWidget->update();
    clusterTreeWidget->setGraph(graph);
  }
  /*Observable::unholdObservers();
  initObservers();*/
}
//**********************************************************************
//Management of properties
//**********************************************************************
template<typename PROPERTY>
bool viewGl::changeProperty(string name, string destination, bool query, bool redraw) {
  /*if( !glWidget ) return false;*/
  Graph *graph = currentGraph;
  if(graph == 0) return false;
  Observable::holdObservers();
  /*overviewWidget->setObservedView(0);*/
  GlGraphRenderingParameters param;
  /*QtProgress myProgress(this, name, redraw ? glWidget : 0);*/
  QtProgress myProgress(this, name);
  string erreurMsg;
  bool   resultBool=true;  
  DataSet *dataSet =0;
  if (query) {
    dataSet = new DataSet();
    StructDef *params = getPluginParameters(PROPERTY::factory, name);
    StructDef sysDef = PROPERTY::factory->getPluginParameters(name);
    params->buildDefaultDataSet( *dataSet, graph );
    resultBool = tlp::openDataSetDialog(*dataSet, &sysDef, params, dataSet,
					"Tulip Parameter Editor", graph, this);
  }

  if (resultBool) {
    /*if (typeid(PROPERTY) == typeid(LayoutProperty)) {
      param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
      //cam = param.getCamera();
      param.setInputLayout(name);
      glWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);
      glWidget->getScene()->getGlGraphComposite()->getInputData()->reloadLayoutProperty();
      }*/

    PROPERTY *dest = graph->template getLocalProperty<PROPERTY>(name);
    resultBool = graph->computeProperty(name, dest, erreurMsg, &myProgress, dataSet);
    if (!resultBool) {
      QMessageBox::critical(this, "Tulip Algorithm Check Failed", QString((name + ":\n" + erreurMsg).c_str()) );
    }
    else 
      switch(myProgress.state()){
      case TLP_CONTINUE:
      case TLP_STOP:
	*graph->template getLocalProperty<PROPERTY>(destination) = *dest;
	break;
      case TLP_CANCEL:
	resultBool=false;
      };
    graph->delLocalProperty(name);
    /*if (typeid(PROPERTY) == typeid(LayoutProperty)) {
      param = glWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
      param.setInputLayout("viewLayout");
      //param.setCamera(cam);
      glWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);
      glWidget->getScene()->getGlGraphComposite()->getInputData()->reloadLayoutProperty();
      }*/
    //glWidget->getScene()->getGlGraphComposite()->getInputData()->loadProperties();
  }
  if (dataSet!=0) delete dataSet;

  propertiesWidget->setGraph(graph);
  /*overviewWidget->setObservedView(glWidget);*/
  Observable::unholdObservers();
  return resultBool;
}
//**********************************************************************
void viewGl::changeString(QAction* action) {
  //clearObservers();
  string name = action->text().toStdString();
  /*if (changeProperty<StringProperty>(name,"viewLabel"))
    redrawView();*/
  changeProperty<StringProperty>(name,"viewLabel");
  //initObservers();
}
//**********************************************************************
void viewGl::changeSelection(QAction* action) {
  //clearObservers();
  string name = action->text().toStdString();
  changeProperty<BooleanProperty>(name, "viewSelection");
  /*if (changeProperty<BooleanProperty>(name, "viewSelection")) {
    //glWidget->getScene()->getGlGraphComposite()->getInputData()->reloadSelectionProperty();
    redrawView();
    }*/
  //initObservers();
}
//**********************************************************************
void viewGl::changeMetric(QAction* action) {
  //clearObservers();
  string name = action->text().toStdString();
  bool result = changeProperty<DoubleProperty>(name,"viewMetric", true);
  if (result && map_metric->isChecked()) {
    changeProperty<ColorProperty>("Metric Mapping","viewColor", false);
    /*if (changeProperty<ColorProperty>("Metric Mapping","viewColor", false))
      redrawView();*/
  }
  //initObservers();
}
//**********************************************************************
void viewGl::changeLayout(QAction* action) {
  //clearObservers();
  string name = action->text().toStdString();
  GraphState * g0 = 0;
  /*if( enable_morphing->isChecked() ) 
    g0 = new GraphState(glWidget);*/

  bool result = changeProperty<LayoutProperty>(name, "viewLayout", true, true);
  if (result) {
    if( force_ratio->isChecked() )
      currentGraph->getLocalProperty<LayoutProperty>("viewLayout")->perfectAspectRatio();
    //Graph *graph=glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
    /*Observable::holdObservers();
    glWidget->getScene()->centerScene();
    overviewWidget->setObservedView(glWidget);
    Observable::unholdObservers();*/
    /*if( enable_morphing->isChecked() ) {
      GraphState * g1 = new GraphState( glWidget );
      bool morphable = morph->init( glWidget, g0, g1);
      if( !morphable ) {
	delete g1;
	g1 = 0;
      } else {
	morph->start(glWidget);
	g0 = 0;	// state remains in morph data ...
      }
      }*/
  }
  //redrawView();
  if( g0 )
    delete g0;
  //initObservers();
}
  //**********************************************************************
void viewGl::changeInt(QAction* action) {
  //clearObservers();
  string name = action->text().toStdString();
  changeProperty<IntegerProperty>(name, "viewInt");
  //initObservers();
}
  //**********************************************************************
void viewGl::changeColors(QAction* action) {
  //clearObservers();
  GraphState * g0 = 0;
  /*if( enable_morphing->isChecked() )
  g0 = new GraphState( glWidget );*/
  string name = action->text().toStdString();
  bool result = changeProperty<ColorProperty>(name,"viewColor");
  if( result ) {
    /*if( enable_morphing->isChecked() ) {
      GraphState * g1 = new GraphState( glWidget );
      bool morphable = morph->init( glWidget, g0, g1);
      if( !morphable ) {
	delete g1;
	g1 = 0;
      } else {
	morph->start(glWidget);
	g0 = 0;	// state remains in morph data ...
      }
      }*/
    //redrawView();
  }
  if( g0 )
    delete g0;
  //initObservers();
}
//**********************************************************************
void viewGl::changeSizes(QAction* action) {
  //clearObservers();
  GraphState * g0 = 0;
  /*if( enable_morphing->isChecked() )
    g0 = new GraphState( glWidget );*/
  string name = action->text().toStdString();
  bool result = changeProperty<SizeProperty>(name,"viewSize");
  if( result ) {
    /*if( enable_morphing->isChecked() ) {
      GraphState * g1 = new GraphState( glWidget );
      bool morphable = morph->init( glWidget, g0, g1);
      if( !morphable ) {
	delete g1;
	g1 = 0;
      } else {
	morph->start(glWidget);
	g0 = 0;	// state remains in morph data ...
      }
      }*/
    //redrawView();
  }
  if( g0 )
    delete g0;
  //initObservers();
}
//**********************************************************************
void viewGl::gridOptions() {
  /*if (gridOptionsWidget == 0)
    gridOptionsWidget = new GridOptionsWidget(this);
  gridOptionsWidget->setCurrentMainWidget(glWidget);
  gridOptionsWidget->setCurrentLayerManagerWidget(layerWidget);
  gridOptionsWidget->show();*/
}
//**********************************************************************
#include <tulip/AcyclicTest.h>
void viewGl::isAcyclic() {
  if (AcyclicTest::isAcyclic(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is acyclic"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not acyclic"
			   );
}
void viewGl::makeAcyclic() {
  Observable::holdObservers();
  vector<tlp::SelfLoops> tmpSelf;
  vector<edge> tmpReversed;
  AcyclicTest::makeAcyclic(currentGraph, tmpReversed, tmpSelf);
  Observable::unholdObservers();
}
//**********************************************************************
#include <tulip/SimpleTest.h>
void viewGl::isSimple() {
  //if (glWidget == 0) return;
  if (SimpleTest::isSimple(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is simple"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not simple"
			   );
}
void viewGl::makeSimple() {
  //if (glWidget == 0) return;
  Observable::holdObservers();
  vector<edge> removed;
  SimpleTest::makeSimple(currentGraph, removed);
  Observable::unholdObservers();
}
//**********************************************************************
#include <tulip/ConnectedTest.h>
void viewGl::isConnected() {
  //if (glWidget == 0) return;
  if (ConnectedTest::isConnected(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is connected"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not connected"
			   );
}
void viewGl::makeConnected() {
  //if (glWidget == 0) return;
  Observable::holdObservers();
  vector<edge> tmp;
  ConnectedTest::makeConnected(currentGraph, tmp);
  Observable::unholdObservers();
}
//**********************************************************************
#include <tulip/BiconnectedTest.h>
void viewGl::isBiconnected() {
  //if (glWidget == 0) return;
  if (BiconnectedTest::isBiconnected(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is biconnected"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not biconnected"
			   );
}
void viewGl::makeBiconnected() {
  //if (glWidget == 0) return;
  Observable::holdObservers();
  vector<edge> tmp;
  BiconnectedTest::makeBiconnected(currentGraph, tmp);
  Observable::unholdObservers();
}
//**********************************************************************
#include <tulip/TriconnectedTest.h>
void viewGl::isTriconnected() {
  //if (glWidget == 0) return;
  if (TriconnectedTest::isTriconnected(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is triconnected"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not triconnected"
			   );
}
//**********************************************************************
#include <tulip/TreeTest.h>
void viewGl::isTree() {
  //if (glWidget == 0) return;
  if (TreeTest::isTree(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is a directed tree"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not a directed tree"
			   );
}
//**********************************************************************
void viewGl::isFreeTree() {
  //if (glWidget == 0) return;
  if (TreeTest::isFreeTree(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is a free tree"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not a free tree"
			   );
}
#include <tulip/GraphTools.h>
void viewGl::makeDirected() {
  //if (glWidget == 0) return;
  if (!TreeTest::isFreeTree(currentGraph))
    QMessageBox::information( this, "Tulip test",
			      "The graph is not a free tree"
			      );
  Graph *graph = currentGraph;
  node n, root;
  forEach(n, graph->getProperty<BooleanProperty>("viewSelection")->getNodesEqualTo(true)) {
    if (root.isValid()) {
      QMessageBox::critical( this, "Make Rooted",
			     "Only one root node must be selected.");
      breakForEach;
    }
    root = n;
  }
  if (!root.isValid())
    root = graphCenterHeuristic(graph);
      
  Observable::holdObservers();
  TreeTest::makeRootedTree(currentGraph, root);
  Observable::unholdObservers();
}
//**********************************************************************
#include <tulip/PlanarityTest.h>
void viewGl::isPlanar() {
  //if (glWidget == 0) return;
  Observable::holdObservers();
  if (PlanarityTest::isPlanar(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is planar"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not planar"
			   );
  Observable::unholdObservers();
}
//**********************************************************************
#include <tulip/OuterPlanarTest.h>
void viewGl::isOuterPlanar() {
  //if (glWidget == 0) return;
  Observable::holdObservers();
  if (OuterPlanarTest::isOuterPlanar(currentGraph))
    QMessageBox::information( this, "Tulip test",
			   "The graph is outer planar"
			   );
  else
    QMessageBox::information( this, "Tulip test",
			   "The graph is not outer planar"
			   );
  Observable::unholdObservers();
}

