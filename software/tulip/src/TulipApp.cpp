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
//**********************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <limits.h>
#include <string>
#include <map>
#include <vector>


#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qpushbutton.h>
#include <QtGui/qapplication.h>
#include <QtGui/qfiledialog.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qlibraryinfo.h>
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
#include <QtGui/qtoolbar.h>

#include <tulip/tuliphash.h>
#include <tulip/TlpTools.h>
#include <tulip/Reflect.h>
#include <tulip/GlMainWidget.h>
#include <tulip/ElementPropertiesWidget.h>
#include <tulip/AbstractProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
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
#include <tulip/Interactor.h>
#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/ControllerPluginsManager.h>
#include <tulip/GraphView.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/TabWidget.h>
#include <tulip/MainController.h>
#include <tulip/QtProgress.h>
#include <tulip/PreferenceManager.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/QtOpenGlErrorViewer.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/tulipconf.h>

#include <PluginsHelp.h>
#include <PluginsManagerDialog.h>
#include <UpdatePlugin.h>

#include "TulipApp.h"
#include "Application.h"
#include "ElementInfoToolTip.h"
#include "InfoDialog.h"
#include "AppStartUp.h"
#include "PreferenceDialog.h"
#include "ChooseControllerDialog.h"

#define UNNAMED "unnamed"

using namespace std;
using namespace tlp;

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

//**********************************************************************
///Constructor of ViewGl
TulipApp::TulipApp(QWidget* parent): QMainWindow(parent),currentTabIndex(-1)  {
  // initialize needRestart setting
  disableRestart();
  setupUi(this);

  tabWidget=centralTabWidget;

#if QT_MINOR_REL >= 5
  tabWidget->setTabsClosable(true);
  connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
#endif

}

//**********************************************************************
void TulipApp::enableElements(bool enabled) {
  fileSaveAction->setEnabled(enabled);
  fileSaveAsAction->setEnabled(enabled);
  filePrintAction->setEnabled(enabled);
  mouseActionGroup->setEnabled(enabled);
  exportGraphMenu.menuAction()->setEnabled(enabled);
}
//**********************************************************************
///Destructor of viewGl
TulipApp::~TulipApp() {
}
//**********************************************************************
void TulipApp::startTulip() {
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
  //delete appStart;
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
      errorDlg->exec();
    }
  if(ControllerFactory::factory->objMap.empty()){
    QMessageBox::critical(this,tr("No controller found"),tr("No controller where found in Tulip plug-ins.\n Tulip can't work without controller."));
    exit(-1);
  }
  buildMenus();
  this->show();
  enableElements(false);

  /*Load preference*/
  PreferenceDialog::loadPreference();

  OpenGlErrorViewer *oldErrorViewer=OpenGlConfigManager::getInst().setErrorViewer(new QtOpenGlErrorViewer);
  delete oldErrorViewer;

  if(PreferenceManager::getInst().getNetworkConnection()){
    pluginsUpdateChecker = new PluginsUpdateChecker(pluginLoader.pluginsList,this);
    connect(pluginsUpdateChecker,SIGNAL(updateFinished()),this,SLOT(displayRestartForPlugins()));
    multiServerManager = pluginsUpdateChecker->getMultiServerManager();
  }else{
    pluginsUpdateChecker=NULL;
  }

  /*QWidget *centralwidget = new QWidget(this);
  QGridLayout *gridLayout = new QGridLayout(centralwidget);
  tabWidget=new QTabWidget(centralwidget);
  centralwidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
  tabWidget->setGeometry(0,0,800,600);
  //tabWidget->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));*/
  tabWidget->removeTab(0);
  connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
  // set workspace background
  //newWorkspace->setBackground(QBrush(QPixmap(QString::fromUtf8(":/background_logo.png"))));

  aboutWidget=NULL;

  //=======================================
  //MDI
  //newWorkspace->setScrollBarsEnabled( true );

  // Window Menu
  connect(windowsMenu, SIGNAL( aboutToShow() ),
	  this, SLOT( windowsMenuAboutToShow() ) );
  connect(windowsMenu, SIGNAL(triggered(QAction*)),
	  this, SLOT( windowsMenuActivated(QAction*)));

  std::string assistantPath(tlp::TulipLibDir);
  assistantPath += string("../");
  assistantPath += string(STRINGIFY(QT_ASSISTANT));
  assistantProcess= new QProcess(this);
#if defined(__linux__)
  assistantProcessApp = QLibraryInfo::location(QLibraryInfo::BinariesPath)+ '/' + QLatin1String(QT_ASSISTANT);
#else
  assistantProcessApp = assistantPath.c_str();
#if defined(__APPLE__)
  assistantProcessApp.append(".app/Contents/MacOS/Assistant");
#endif
#endif
  /*saveActions(menuBar(),NULL,controllerToMenu);
  saveActions(toolBar,NULL,controllerToToolBar);*/
  tabIndexToController[-1]=NULL;
  saveInterface(-1);

  OpenGlErrorViewer *oldViewer=GlTextureManager::getInst().setErrorViewer(new QtOpenGlErrorViewer(this));
  delete oldViewer;

  // if we have only one controller : auto load it
  MutableContainer<Controller *> controllers;
  ControllerPluginsManager::getInst().initControllerPluginsList(controllers);
  TemplateFactory<ControllerFactory, Controller, ControllerContext>::ObjectCreator::const_iterator it;
  it=ControllerFactory::factory->objMap.begin();
  string name=(*it).first;
  ++it;
  if(it==ControllerFactory::factory->objMap.end()){
    controllerAutoLoad=false;
    createController(name,newName());
    currentTabIndex=0;
    Controller::currentActiveController(tabIndexToController[currentTabIndex]);
    controllerAutoLoad=true;
  }else{
    controllerAutoLoad=false;
  }

  int argc = qApp->argc();
  if (argc>1) {
    char ** argv = qApp->argv();
    for (int i=1;i<argc;++i) {
      if(string(argv[i])=="--width") {
        int width = atoi(argv[i+1]);
        wRect.setWidth(width);
        ++i;
        continue;
      }
      if(string(argv[i])=="--height") {
        int height = atoi(argv[i+1]);
        wRect.setHeight(height);
        ++i;
        continue;
      }
      if(string(argv[i])=="--x") {
        int x = atoi(argv[i+1]);
        wRect.setX(x);
        ++i;
        continue;
      }
      if(string(argv[i])=="--y") {
        int y = atoi(argv[i+1]);
        wRect.setY(y);
        ++i;
        continue;
      }
      QFileInfo info(argv[i]);
      QString s = info.absoluteFilePath();
      fileOpen(0, s);
    }
  }
  this->setGeometry(wRect.x(), wRect.y(),wRect.width(), wRect.height());

}
//**********************************************************************
std::string TulipApp::newName() {
  static int idx = 0;

  if (idx++ == 0)
    return std::string(UNNAMED);

  stringstream ss;
  ss << UNNAMED << '_' << idx - 1;
  return ss.str();
}
//**********************************************************************
void TulipApp::initializeGraph(Graph *graph) {
  graph->setAttribute("name", newName());
  graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  graph->getProperty<SizeProperty>("viewSize")->setAllEdgeValue(Size(0.125,0.125,0.5));
  graph->getProperty<ColorProperty>("viewColor")->setAllNodeValue(Color(255,0,0));
  graph->getProperty<ColorProperty>("viewColor")->setAllEdgeValue(Color(0,0,0));
  graph->getProperty<IntegerProperty>("viewShape")->setAllNodeValue(4);
  graph->getProperty<IntegerProperty>("viewShape")->setAllEdgeValue(0);
  graph->getProperty<StringProperty>("viewFont")->setAllNodeValue(tlp::TulipLibDir + "tlp/bitmaps/font.ttf");
  graph->getProperty<StringProperty>("viewFont")->setAllEdgeValue(tlp::TulipLibDir + "tlp/bitmaps/font.ttf");
  graph->getProperty<IntegerProperty>("viewFontSize")->setAllNodeValue(18);
  graph->getProperty<IntegerProperty>("viewFontSize")->setAllEdgeValue(18);
  graph->getProperty<IntegerProperty>("viewSrcAnchorShape")->setAllEdgeValue(EdgeExtremityGlyphManager::NoEdgeExtremetiesId);
  graph->getProperty<SizeProperty>("viewSrcAnchorSize")->setAllEdgeValue(Size(0.25,0.25,0.25));
  graph->getProperty<IntegerProperty>("viewTgtAnchorShape")->setAllEdgeValue(50);
  graph->getProperty<SizeProperty>("viewTgtAnchorSize")->setAllEdgeValue(Size(0.25,0.25,0.25));
}
//**********************************************************************
void TulipApp::fileNew(QAction *action) {
  string name=action->text().toStdString();
  Graph *graph = newGraph();
  initializeGraph(graph);
  string graphName;
  graph->getAttribute("name",graphName);
  createController(name,graphName);
  tabIndexToController[tabWidget->currentIndex()]->setData(graph);
  GraphNeedsSavingObserver* observer = new GraphNeedsSavingObserver(tabWidget, tabWidget->currentIndex(), graph);
  observer->doNeedSaving();
  graph->addObserver(observer);
  controllerToGraphObserver[tabIndexToController[tabWidget->currentIndex()]] = observer;
  enableElements(true);
}
//**********************************************************************
bool TulipApp::fileNew(bool) {
  MutableContainer<Controller *> controllers;
  ControllerPluginsManager::getInst().initControllerPluginsList(controllers);
  TemplateFactory<ControllerFactory, Controller, ControllerContext>::ObjectCreator::const_iterator it;
  it=ControllerFactory::factory->objMap.begin();
  string name=(*it).first;
  Graph *graph = newGraph();
  initializeGraph(graph);
  string graphName;
  graph->getAttribute("name",graphName);
  if(createController(name,graphName)) {
    tabIndexToController[tabWidget->currentIndex()]->setData(graph);
    GraphNeedsSavingObserver* observer = new GraphNeedsSavingObserver(tabWidget, tabWidget->currentIndex(), graph);
    observer->doNeedSaving();
    graph->addObserver(observer);
    controllerToGraphObserver[tabIndexToController[tabWidget->currentIndex()]] = observer;    
    enableElements(true);
  } else
    return false;
  return true;
}
//**********************************************************************
void TulipApp::fileCloseTab(){
  if(controllerAutoLoad)
    return;
  int index=tabWidget->currentIndex();
  closeTab(index);
}
//**********************************************************************
void TulipApp::closeTab(int index){
  if(controllerAutoLoad)
    return;

  if(index!=tabWidget->currentIndex())
    tabWidget->setCurrentIndex(index);

  Graph *graph=tabIndexToController[index]->getGraph();
  while(graph->getRoot()!=graph)
    graph=graph->getRoot();
  QMessageBox::StandardButton answer =askSaveGraph(graph->getAttribute<string>("name"),index);
    if(answer != QMessageBox::Cancel){
    Controller *controller=tabIndexToController[index];
    tabWidget->setCurrentIndex(index-1);

    map<int,Controller *> newTabIndexToController;
    for(map<int,Controller *>::iterator it=tabIndexToController.begin();it!=tabIndexToController.end();++it){
      if((*it).first<index)
        newTabIndexToController[(*it).first]=(*it).second;
      else if((*it).first>index)
        newTabIndexToController[(*it).first-1]=(*it).second;
    }
    tabIndexToController=newTabIndexToController;

    controllerToWorkspace.erase(controller);
    controllerToMenu.erase(controller);
    controllerToToolBar.erase(controller);
    controllerToGraphToolBar.erase(controller);
    controllerToDockWidget.erase(controller);
    controllerToCustomToolBar.erase(controller);
    controllerToWidgetVisible.erase(controller);

    currentTabIndex=-1;

    int newIndex=tabWidget->currentIndex();
    if(index<tabWidget->currentIndex())
      newIndex--;

    tabWidget->removeTab(index);
    //tabChanged(newIndex);

    if(tabWidget->count()==0)
      tabWidget->setCurrentIndex(-1);


    delete controller;
  }
}
//**********************************************************************
bool TulipApp::createController(const string &name,const string &graphName) {
  if(!controllerAutoLoad){
    /*if(currentController) {
      Graph *graph=currentController->getGraph();
      if(askSaveGraph(graph->getAttribute<string>("name")))
        return false;
    }
    delete currentController;
    newWorkspace->closeAllWindows();*/
    QWidget *tab=new QWidget();
    QGridLayout *gridLayout = new QGridLayout(tab);
    QWorkspace *newWorkspace=new QWorkspace(tab);
    
    newWorkspace->setBackground(QBrush(QPixmap(QString::fromUtf8(":/background_logo.png"))));

    gridLayout->addWidget(newWorkspace, 0, 0, 1, 1);
    int index=tabWidget->addTab(tab, QString::fromUtf8(graphName.c_str()));
    tabWidget->setCurrentIndex(index);

    loadInterface(-1);

    Controller *newController=ControllerPluginsManager::getInst().createController(name);
    toolBar->setWindowTitle("Tool Bar");
    graphToolBar->setWindowTitle("Interactors Tool Bar");
    newController->attachMainWindow(MainWindowFacade(this,toolBar,graphToolBar,newWorkspace));
    tabIndexToController[index]=newController;
    controllerToControllerName[newController]=name;
    controllerToWorkspace[newController]=newWorkspace;
    
    //Set Qt object names
    tab->setObjectName(QString("ControllerMainWidget ")+QString::number(index));
    newWorkspace->setObjectName(QString("ControllerWorkspace ")+QString::number(index));

  }else{
    controllerAutoLoad=false;
    tabWidget->setTabText(0, QString::fromUtf8(graphName.c_str()));
  }
  Controller::currentActiveController(tabIndexToController[tabWidget->currentIndex()]);
  return true;
}
//**********************************************************************
bool TulipApp::doFileSave(int index) {
  Controller *controller=tabIndexToController[index];
  if (openFiles.find(controller)==openFiles.end() ||
      (openFiles[controller].name == "")) {
    return doFileSaveAs();
  }
  FileInfo &vFile = openFiles[controller];
  return doFileSave(controller,"tlp", vFile.name, vFile.author, vFile.comments);
}
//**********************************************************************
void TulipApp::fileSave() {
  doFileSave(tabWidget->currentIndex());
}
//**********************************************************************
bool TulipApp::doFileSave(Controller *controllerToSave,string plugin, string filename,
			  string author, string comments) {

  DataSet dataSet;
  StructDef parameter = ExportModuleFactory::factory->getPluginParameters(plugin);
  parameter.buildDefaultDataSet(dataSet);
  DataSet controller;
  DataSet controllerData;
  Graph *graph;
  controllerToSave->getData(&graph,&controllerData);
  if (graph == NULL)
    return false;
  controller.set<DataSet>(controllerToControllerName[controllerToSave],controllerData);
  dataSet.set<DataSet>("controller",controller);

  string graphName;
  
  if (graph->getRoot()->getAttribute("name", graphName) && graphName.find("unnamed") != 0)
    dataSet.set<string>("name", graphName);
  else
    dataSet.set<string>("name", filename);
  if (!author.empty())
    dataSet.set<string>("author", author);
  if (!comments.empty())
    dataSet.set<string>("text::comments", comments);
  string title = string("Enter Export parameters: ") + plugin;
  if (!tlp::openDataSetDialog(dataSet, 0, &parameter,
			      &dataSet, title.c_str(), NULL,
			      this))
    return false;

  if (filename.empty()) {
    QString name;
    if (plugin == "tlp")
      name =
	QFileDialog::getSaveFileName(this, tr("Choose a file to save" ),
				     QString(),
				     tr("Tulip graph (*.tlp *.tlp.gz)"));
    else
      name =
	QFileDialog::getSaveFileName(this,
				     this->windowTitle().toUtf8().data());
    if (name == QString::null) return false;
    filename = name.toUtf8().data();
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
  FileInfo &vFile = openFiles[controllerToSave];
  vFile.name = filename;
  dataSet.get<string>("author", vFile.author);
  dataSet.get<string>("text::comments", vFile.comments);
  dataSet.get<string>("name", graphName);

  PluginProgress* progress = new QtProgress(NULL, "saving Graph");
  if (!(result=tlp::exportGraph(graph, *os, plugin, dataSet, progress))) {
    QMessageBox::critical( 0, "Tulip export Failed",
			   "The file has not been saved"
			   );
  } else {
    statusBar()->showMessage(QString::fromUtf8((filename + " saved.").c_str()));
  }
  delete progress;
  
  tabWidget->setTabText(tabWidget->currentIndex(), QString::fromUtf8(filename.c_str()));
  controllerToGraphObserver[tabIndexToController[tabWidget->currentIndex()]]->saved();
  //setNavigateCaption(filename);
  //setGraphName(graph, QString(filename.c_str()));
  delete os;
  return result;
}
//**********************************************************************
bool TulipApp::doFileSaveAs() {
  return doFileSave(tabIndexToController[tabWidget->currentIndex()],"tlp", "", "", "");
}
//**********************************************************************
void TulipApp::fileSaveAs() {
  doFileSaveAs();
}
//**********************************************************************
void TulipApp::fileOpen() {
  QString s;
  fileOpen(0,s);
}
//**********************************************************************
void TulipApp::fileOpen(string *plugin, QString &s) {
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
	dataSet.set("file::filename", string(s.toUtf8().data()));
    }else {
      noPlugin = false;
      s = QString::null;
      StructDef sysDef = ImportModuleFactory::factory->getPluginParameters(*plugin);
      StructDef *params = getPluginParameters(ImportModuleFactory::factory, *plugin);
      params->buildDefaultDataSet( dataSet );
      string title = string("Enter Import parameters: ") + plugin->c_str();
      cancel = !tlp::openDataSetDialog(dataSet, &sysDef, params, &dataSet,
				       title.c_str(), NULL, this);
    }
  } else {
    plugin = &tmpStr;
    dataSet.set("file::filename", string(s.toUtf8().data()));
    noPlugin = true;
  }
  if (!cancel) {

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    Graph *newGraph = tlp::newGraph();
    initializeGraph(newGraph);
    if (s == QString::null)
      s = newGraph->getAttribute<string>("name").c_str();
    bool result=true;
    QFileInfo tmp(s);
    QDir::setCurrent(tmp.dir().path() + "/");

    QtProgress *progressBar = new QtProgress(this, string("Loading : ")+ s.section('/',-1).toUtf8().data());

    result = tlp::importGraph(*plugin, dataSet, progressBar ,newGraph);

    string errorMessage;
    DataSet controllerData;
    bool haveControllerData=false;
    string controllerName;
    // now ensure notification
    if (progressBar->state()==TLP_CANCEL || !result ) {
      errorMessage = progressBar->getError();
      delete progressBar;
      QApplication::restoreOverrideCursor();
    }else{
      delete progressBar;
      QApplication::restoreOverrideCursor();

      MutableContainer<Controller *> controllers;
      ControllerPluginsManager::getInst().initControllerPluginsList(controllers);
      TemplateFactory<ControllerFactory, Controller, ControllerContext>::ObjectCreator::const_iterator it;
      vector<string> controllersName;
      for (it=ControllerFactory::factory->objMap.begin();it != ControllerFactory::factory->objMap.end();++it) {
        controllersName.push_back(it->first);
      }

      ChooseControllerDialog chooseControllerDialog;

      //check if we have controller information in file and set to default controller in ChooseControllerDialog
      string defaultControllerName="MainController";
      if(dataSet.exist("controller")) {
        DataSet controller;
        dataSet.get<DataSet>("controller", controller);
        Iterator< std::pair<std::string, DataType*> > *it=controller.getValues();
        assert(it->hasNext());
        pair<string, DataType*> p;
        p = it->next();
        controllerData=*((DataSet*)p.second->value);
        haveControllerData=true;
        defaultControllerName=p.first;
      }

      if(controllersName.size()==1){
        // if we have only one controller : auto load it
        controllerName = controllersName[0];
      }else{
        bool displayDialog=true;

        // if auto load controller setting is set
        if(haveControllerData && PreferenceManager::getInst().getAutoLoadController()){
          // check if this controller is in available controllers
          for(vector<string>::iterator it=controllersName.begin();it!=controllersName.end();++it){
            if((*it)==defaultControllerName){
              displayDialog=false;
              controllerName=defaultControllerName;
              break;
            }
          }
        }

        if(displayDialog){
          // If we find controller name : display this name is ChooseControllerDialog (replace None)
          if(haveControllerData)
            chooseControllerDialog.setDefaultControllerName(defaultControllerName);

          chooseControllerDialog.setDefaultCheckedControllerName(defaultControllerName);
          chooseControllerDialog.setControllersAvailable(controllersName);
          if(chooseControllerDialog.exec()==QDialog::Rejected){
            errorMessage="Cancel by user";
          }else{
            controllerName = chooseControllerDialog.getCheckedControllerName();
          }
        }
      }
    }

    if(errorMessage!=""){
      delete newGraph;
      QApplication::restoreOverrideCursor();
      std::string errorTitle("Canceled import/Open: ");
      errorTitle += s.section('/',-1).toUtf8().data();
      QMessageBox::critical(this, errorTitle.c_str(), errorMessage.c_str());
      QApplication::restoreOverrideCursor();
      return;
    }

    //We have no controller data, so controller is not save in the file
    if(!haveControllerData) {
      DataSet newDataSet;

      if(dataSet.exist("scene")) {
        // Tlp file with scene system
        string sceneDataStr;
        dataSet.get<string>("scene", sceneDataStr);
        newDataSet.set<string>("scene",sceneDataStr);
      }
      if(dataSet.exist("displaying")) {
        // Tlp file with old system
        DataSet glGraphData;
        dataSet.get<DataSet>("displaying", glGraphData);
        newDataSet.set<DataSet>("displaying",glGraphData);
      }

      controllerData=newDataSet;
    }

    createController(controllerName,s.toUtf8().data());
    tabIndexToController[tabWidget->currentIndex()]->setData(newGraph,controllerData);
    GraphNeedsSavingObserver* observer = new GraphNeedsSavingObserver(tabWidget, tabWidget->currentIndex(), newGraph);
    newGraph->addGraphObserver(observer);
    controllerToGraphObserver[tabIndexToController[tabWidget->currentIndex()]] = observer;    
    if (!noPlugin) {
      observer->doNeedSaving();
    }
    enableElements(true);

    if(noPlugin) {
      FileInfo vFile;
      vFile.name = s.toUtf8().data();
      dataSet.get<std::string>("author", vFile.author);
      dataSet.get<std::string>("text::comments", vFile.comments);
      openFiles[tabIndexToController[tabWidget->currentIndex()]] = vFile;
    }

    enableElements(true);

    // synchronize overview display parameters
  } /*else {
    qWarning("Canceled  Open/import");
    }*/
}
//**********************************************************************
void TulipApp::importGraph() {
  QAction *action=(QAction*)(sender());
  importGraph(action);
}
//**********************************************************************
void TulipApp::importGraph(QAction* action) {
  string name = action->text().toStdString();
  QString s;
  fileOpen(&name,s);
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
			   std::vector<QMenu*> &groupMenus, std::string::size_type &nGroups,QObject *receiver, const char *slot) {
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
    QAction *action=subMenu->addAction(itemName.c_str());
    QObject::connect(action,SIGNAL(triggered()),receiver,slot);
  }
//**********************************************************************
template <typename TFACTORY, typename TMODULE>
  void buildMenuWithContext(QMenu &menu, QObject *receiver, const char *slot) {
    typename TemplateFactory<TFACTORY, TMODULE, AlgorithmContext>::ObjectCreator::const_iterator it;
    std::vector<QMenu*> groupMenus;
    std::string::size_type nGroups = 0;
    for (it=TFACTORY::factory->objMap.begin();it != TFACTORY::factory->objMap.end();++it)
      insertInMenu(menu, it->first.c_str(), it->second->getGroup(), groupMenus, nGroups,receiver,slot);
  }
//**********************************************************************
void TulipApp::buildMenus() {
  MutableContainer<Controller *> controllers;
  ControllerPluginsManager::getInst().initControllerPluginsList(controllers);
  TemplateFactory<ControllerFactory, Controller, ControllerContext>::ObjectCreator::const_iterator it;

  // if we have only one controller : auto load it
  // In this case doesn't add sub menu in new menu
  it=ControllerFactory::factory->objMap.begin();
  ++it;
  if(it!=ControllerFactory::factory->objMap.end()) {
    //Add new menu in File menu
    newMenu=new QMenu("New");
    connect(newMenu, SIGNAL(triggered(QAction *)), SLOT(fileNew(QAction*)));
    fileMenu->insertMenu(fileOpenAction,newMenu);
    for (it=ControllerFactory::factory->objMap.begin();it != ControllerFactory::factory->objMap.end();++it) {
      newMenu->addAction(it->first.c_str());
    }
  }else{
    newAction = new QAction("New",fileMenu);
    newAction->setShortcut(QKeySequence(tr("Ctrl+N")));
    connect(newAction,SIGNAL(triggered(bool)), SLOT(fileNew(bool)));
    fileMenu->insertAction(fileOpenAction,newAction);
  }

  buildMenuWithContext<ExportModuleFactory, ExportModule>(exportGraphMenu, this, SLOT(exportGraph()));
  buildMenuWithContext<ImportModuleFactory, ImportModule>(importGraphMenu, this, SLOT(importGraph()));
  //connect(&exportGraphMenu, SIGNAL(triggered(QAction*)), SLOT(exportGraph(QAction*)));
  //connect(&importGraphMenu, SIGNAL(triggered(QAction*)), SLOT(importGraph(QAction*)));
  if (importGraphMenu.actions().count()>0) {
    importGraphMenu.setTitle("&Import");
    fileMenu->insertMenu(filePrintAction,&importGraphMenu);
  }
  if (exportGraphMenu.actions().count()>0) {
    exportGraphMenu.setTitle("&Export");
    fileMenu->insertMenu(filePrintAction,&exportGraphMenu);
  }
  fileMenu->insertSeparator(filePrintAction);
}
//**********************************************************************
void TulipApp::exportGraph() {
  QAction *action=(QAction*)(sender());
  exportGraph(action);
}
//**********************************************************************
void TulipApp::exportGraph(QAction* action) {
  /*if (!glWidget) return;*/
  doFileSave(tabIndexToController[tabWidget->currentIndex()],action->text().toStdString(), "", "", "");
}
//**********************************************************************
void TulipApp::windowsMenuActivated(QAction* action) {
  int id = action->data().toInt();
  QWidget* w = controllerToWorkspace[tabIndexToController[tabWidget->currentIndex()]]->windowList().at(id);
  if ( w ) {
    w->setFocus();
    w->show();
  }
}
//**********************************************************************
void TulipApp::windowsMenuAboutToShow() {
  windowsMenu->clear();
  if(tabWidget->currentIndex()==-1)
    return;
  QWorkspace *currentWorkspace=controllerToWorkspace[tabIndexToController[tabWidget->currentIndex()]];
  QAction* cascadeAction = windowsMenu->addAction("&Cascade", this, SLOT(cascade() ) );
  QAction* tileAction = windowsMenu->addAction("&Tile", currentWorkspace, SLOT(tile() ) );
  if ( currentWorkspace->windowList().isEmpty() ) {
    cascadeAction->setEnabled(false);
    tileAction->setEnabled(false);
  }else{
    windowsMenu->addSeparator();
    QWidgetList windows = currentWorkspace->windowList();
    for ( int i = 0; i < int(windows.count()); ++i ) {
      QAction* action = windowsMenu->addAction(windows.at(i)->windowTitle());
      action->setChecked(currentWorkspace->activeWindow() == windows.at(i));
      action->setData(QVariant(i));
    }
  }
}
//**********************************************************************
QMessageBox::StandardButton TulipApp::askSaveGraph(const std::string &name,int index,bool activateNoToAll) {
  
  if(!controllerToGraphObserver[tabIndexToController[index]]->needSaving()) {
    return QMessageBox::No;
  }
  string message = "Do you want to save this graph : " + name + " ?";
  QMessageBox::StandardButton answer;
  if(activateNoToAll) {
    answer = QMessageBox::question(this, "Save", QString::fromUtf8(message.c_str()),
    QMessageBox::Yes | QMessageBox::No | QMessageBox::NoToAll | QMessageBox::Cancel | QMessageBox::Escape
    ,QMessageBox::Yes);
  }else{
    answer = QMessageBox::question(this, "Save", QString::fromUtf8(message.c_str()),
    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel | QMessageBox::Escape
    ,QMessageBox::Yes);
  }
  //If the register step fail send the cancel button to stop the process.
  if(answer == QMessageBox::Yes){
    return doFileSave(index)?QMessageBox::Yes:QMessageBox::Cancel;
  }
  return answer;
}
//**********************************************************************
/* returns true if window agrees to be closed */
bool TulipApp::closeWin() {
  QMessageBox::StandardButton answer = QMessageBox::No;
  if(!controllerAutoLoad){
    for(map<int,Controller *>::iterator it=tabIndexToController.begin();it!=tabIndexToController.end() && answer != QMessageBox::NoToAll;++it){
      if((*it).second){
        Graph *graph=((*it).second)->getGraph();
        assert(graph);
        while(graph->getRoot()!=graph){
          graph=graph->getRoot();
        }
        answer = askSaveGraph(graph->getAttribute<string>("name"),(*it).first,true);
        //If user cancel or click on cancel stop the process
        if(answer == QMessageBox::Cancel){
          return false;
        }
      }
    }

    for(map<int,Controller *>::iterator it=tabIndexToController.begin();it!=tabIndexToController.end();++it){
      if((*it).second){
        delete (*it).second;
      }
    }
  }
  return true;
}
//**********************************************************************
void TulipApp::closeEvent(QCloseEvent *e) {
  if (closeWin())
    e->accept();
  else
    e->ignore();
}
//==============================================================
void TulipApp::plugins() {
  if(!pluginsUpdateChecker){
    pluginsUpdateChecker = new PluginsUpdateChecker(pluginLoader.pluginsList,this);
    connect(pluginsUpdateChecker,SIGNAL(updateFinished()),this,SLOT(displayRestartForPlugins()));
    multiServerManager = pluginsUpdateChecker->getMultiServerManager();
  }

  PluginsHelp::checkViewHelp();

  PluginsManagerDialog *pluginsManager=new PluginsManagerDialog(multiServerManager,this);

  pluginsManager->exec();
  if (pluginsManager->pluginUpdatesPending()) {
    displayRestartForPlugins();
  }
}
//==============================================================
void TulipApp::displayRestartForPlugins() {
  int result = QMessageBox::warning(this,
      tr("Update plugins"),
      tr("To finish installing/removing plugins, \nTulip must be restarted.\nDo you want to restart Tulip now ?"),
      QMessageBox::Yes | QMessageBox::Default,
      QMessageBox::No);
  if(result == QMessageBox::Yes){
    // Store that tulip must be auto restarted
    enableRestart();

    fileExit();
  }
}
//==============================================================
void TulipApp::deletePluginsUpdateChecker(){
  disconnect(pluginsUpdateChecker,SIGNAL(updateFinished()),this,SLOT(displayRestartForPlugins()));
  delete pluginsUpdateChecker;
  plugins();
}
//==============================================================
void TulipApp::saveActions(QWidget *widget,Controller *controller,map<Controller *,vector<QAction *> > &mapToSave){
  mapToSave[controller].clear();
  QList<QAction *> actions=widget->actions();
  for(QList<QAction *>::iterator it=actions.begin();it!=actions.end();++it){
    mapToSave[controller].push_back(*it);
  }
}
//==============================================================
void TulipApp::clearInterface() {
  //Dock widgets
  QObjectList objectList=this->children();
  for(QObjectList::iterator it=objectList.begin();it!=objectList.end();++it){
    QDockWidget *widget=dynamic_cast<QDockWidget *>(*it);
    if(widget){
      removeDockWidget(widget);
      widget->hide();
    }

    QToolBar *tb = dynamic_cast<QToolBar *>(*it);
    if (tb && tb != toolBar && tb != graphToolBar) {
      removeToolBar(tb);
      tb->hide();
      tb->setParent(0);
    }
  }

  menuBar()->clear();
  toolBar->clear();
  graphToolBar->clear();
}
//==============================================================
void TulipApp::saveInterface(int index) {
  Controller *controller=tabIndexToController[index];
  saveActions(menuBar(),controller,controllerToMenu);
  saveActions(graphToolBar,controller,controllerToGraphToolBar);
  saveActions(toolBar,controller,controllerToToolBar);

  controllerToDockWidget[controller].clear();
  controllerToCustomToolBar[controller].clear();
  controllerToWidgetVisible[controller].clear();
  QObjectList objectList=this->children();
  for(QObjectList::iterator it=objectList.begin();it!=objectList.end();++it){
    QDockWidget *widget=dynamic_cast<QDockWidget *>(*it);
    if(widget){
      Qt::DockWidgetArea area=dockWidgetArea(widget);
      if(area!=Qt::NoDockWidgetArea){
        controllerToDockWidget[controller].push_back(pair<Qt::DockWidgetArea,QDockWidget *>(area,widget));
        controllerToWidgetVisible[controller].push_back(pair<QWidget *, bool>(widget, widget->isVisible()));
      }
    }

    QToolBar *tb = dynamic_cast<QToolBar *>(*it);
    if (tb && tb != toolBar && tb != graphToolBar) {
      Qt::ToolBarArea area = toolBarArea(tb);
      controllerToCustomToolBar[controller].push_back(pair<Qt::ToolBarArea, QToolBar *>(area, tb));
      controllerToWidgetVisible[controller].push_back(pair<QWidget *, bool>(tb, tb->isVisible()));
    }
  }
  controllerToWidgetVisible[controller].push_back(pair<QWidget *, bool>(graphToolBar, graphToolBar->isVisible()));
  controllerToWidgetVisible[controller].push_back(pair<QWidget *, bool>(toolBar, toolBar->isVisible()));

  pair<string,string > tmp(statusBar()->currentMessage().toStdString(),string());
  objectList=statusBar()->children();
  for(QObjectList::iterator it=objectList.begin();it!=objectList.end();++it){
    QLabel *widget=dynamic_cast<QLabel*>(*it);
    if(widget)
      tmp.second=widget->text().toStdString();
  }
  controllerToStatusBar[tabIndexToController[index]]=tmp;
}
//==============================================================
void TulipApp::loadInterface(int index){
  if(tabIndexToController.count(index)==0)
      return;

  Controller *controller=tabIndexToController[index];

  clearInterface();

  if(controllerToWorkspace.count(controller)!=0){
    controllerToWorkspace[controller]->setActiveWindow(controllerToWorkspace[controller]->activeWindow());
  }

  if(controllerToDockWidget.count(controller)!=0){
    vector<pair<Qt::DockWidgetArea,QDockWidget*> > tmp=controllerToDockWidget[controller];
    vector<pair<Qt::DockWidgetArea,QDockWidget*> >::iterator it=tmp.end();

#if QT_MINOR_REL > 3
    it=tmp.begin();
    while(it!=tmp.end()){
      restoreDockWidget((*it).second);
      ++it;
    }
#else
    while(it!=tmp.begin()){
      --it;
      addDockWidget((*it).first,(*it).second);
      (*it).second->show();
    }

    if(controller) {
      vector<pair<QDockWidget *,QDockWidget*> > tabifiedDockWidget=controller->getMainWindowFacade()->getTabifiedDockWidget();
      for(vector<pair<QDockWidget *,QDockWidget*> >::iterator it=tabifiedDockWidget.begin();it!=tabifiedDockWidget.end();++it) {
	tabifyDockWidget((*it).first,(*it).second);
      }
    }
#endif
  }

  if(controllerToCustomToolBar.count(controller)!=0){
    vector<pair<Qt::ToolBarArea, QToolBar *> > toolBarsToAdd = controllerToCustomToolBar[controller];
    if (!toolBarsToAdd.empty()) {
      for (vector<pair<Qt::ToolBarArea, QToolBar *> >::iterator it = toolBarsToAdd.begin(); it != toolBarsToAdd.end(); ++it) {
        addToolBar(it->first, it->second);
        it->second->show();
      }
    }
  }

  if(controllerToMenu.count(controller)!=0){
    vector<QAction *> actionsToAdd=controllerToMenu[controller];
    if(!actionsToAdd.empty()){
      for(vector<QAction *>::iterator it=actionsToAdd.begin();it!=actionsToAdd.end();++it){
        menuBar()->addAction(*it);
      }
    }
  }

  if(controllerToToolBar.count(controller)!=0){
    vector<QAction *> actionsToAdd=controllerToToolBar[controller];
    if(!actionsToAdd.empty()){
      for(vector<QAction *>::iterator it=actionsToAdd.begin();it!=actionsToAdd.end();++it){
        toolBar->addAction(*it);
      }
    }
  }

  if(controllerToGraphToolBar.count(controller)!=0){
    vector<QAction *> actionsToAdd=controllerToGraphToolBar[controller];
    if(!actionsToAdd.empty()){
      for(vector<QAction *>::iterator it=actionsToAdd.begin();it!=actionsToAdd.end();++it){
        graphToolBar->addAction(*it);
        if((*it)->isChecked()){
          (*it)->activate(QAction::Trigger);
        }
      }
    }
  }

  if(controllerToStatusBar.count(controller)!=0){
    statusBar()->showMessage(controllerToStatusBar[controller].first.c_str());
    QObjectList objectList=statusBar()->children();
    for(QObjectList::iterator it=objectList.begin();it!=objectList.end();++it){
      QLabel *widget=dynamic_cast<QLabel*>(*it);
      if(widget)
        widget->setText(controllerToStatusBar[controller].second.c_str());
    }
  }

  if (controllerToWidgetVisible.count(controller) != 0) {
    vector<pair<QWidget *,bool> > tmp = controllerToWidgetVisible[controller];
    for(vector<pair<QWidget *,bool> >::iterator it = tmp.begin(); it != tmp.end(); ++it)
      it->first->setVisible(it->second);
  }
}
//==============================================================
void TulipApp::tabChanged(int index){
  if(index==-1){
    fileNew(false);
    return;
  }

  if(currentTabIndex!=-1)
    saveInterface(currentTabIndex);

  loadInterface(index);

  currentTabIndex=index;
  Controller::currentActiveController(tabIndexToController[currentTabIndex]);
}
//==============================================================
void TulipApp::helpAbout() {
  if (aboutWidget==0)
    aboutWidget = new InfoDialog(this);
  aboutWidget->show();
}
//==============================================================
void TulipApp::preference() {
  PreferenceDialog pref(this);
  if(pref.exec()!=QDialog::Accepted)
    return;

  if(!pref.preferencesAreModified())
    return;
  
  int result = QMessageBox::warning(this,
				    tr(""),
                    tr("To apply preferences modifications, \nTulip must be restarted.\nDo you want to restart Tulip now ?"),
				    QMessageBox::Yes | QMessageBox::Default,
				    QMessageBox::No);
  if(result == QMessageBox::Yes) {

    // Store that tulip must be auto restarted
    enableRestart();

    fileExit();
  }
}
//==============================================================
void TulipApp::helpDocumentation() {
  QStringList args;
  args << "-collectionFile" << tlp::TulipDocProfile.c_str();
  assistantProcess->start(assistantProcessApp, args);
  if (!assistantProcess->waitForStarted()) {
    QMessageBox::critical(this, tr("Remote Control"),
                          tr("Could not start Qt Assistant from %1.").arg(assistantProcessApp));
    return;
  }
}
//==============================================================
void TulipApp::helpAssistantError(const QString &msg) {
  cerr << msg.toUtf8().data() << endl;
}
//==============================================================
void TulipApp::fileExit() {
  if (closeWin())
    exit(EXIT_SUCCESS);
}
//==============================================================
void TulipApp::filePrint() {
  Graph *graph= tabIndexToController[tabWidget->currentIndex()]->getGraph();
  if (graph==0) return;

  NodeLinkDiagramComponent *nldc = NULL;
  QWidget *widget=tabWidget->widget(tabWidget->currentIndex());
  QObjectList tmp=widget->children();
  for(QObjectList::iterator it=tmp.begin();it!=tmp.end();++it){
    QWorkspace *workspace=dynamic_cast<QWorkspace*>(*it);
    if(workspace){
      QWidget *viewWidget=workspace->activeWindow();
      MainController *mainController=(MainController*)tabIndexToController[tabWidget->currentIndex()];
      View *view=mainController->getViewOfWidget(viewWidget);
      nldc=dynamic_cast<NodeLinkDiagramComponent*>(view);
    }
  }

  if(nldc){
    GlMainWidget *glWidget=nldc->getGlMainWidget();
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
    delete image;
  }
}
//==============================================================
void TulipApp::cascade(){
  QWorkspace *currentWorkspace=controllerToWorkspace[tabIndexToController[tabWidget->currentIndex()]];
  currentWorkspace->cascade();
  QWidgetList widgetList=currentWorkspace->windowList();
  for(QWidgetList::iterator it=widgetList.begin();it!=widgetList.end();++it){
    (*it)->resize(QSize(512,512));
  }
}
