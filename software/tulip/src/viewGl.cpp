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
#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlRectTextured.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/ControllerPluginsManager.h>
#include <tulip/GraphView.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/hash_string.h>
#include <tulip/TabWidget.h>

#include <PluginsManagerDialog.h>
#include <UpdatePlugin.h>

#include "viewGl.h"
#include "Application.h"
#include "QtProgress.h"
#include "ElementInfoToolTip.h"
#include "InfoDialog.h"
#include "AppStartUp.h"

#define UNNAMED "unnamed"

using namespace std;
using namespace tlp;

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
///Constructor of ViewGl
viewGl::viewGl(QWidget* parent): QMainWindow(parent),currentController(NULL)  {
  setupUi(this);

}

//**********************************************************************
void viewGl::enableElements(bool enabled) {
  fileSaveAction->setEnabled(enabled);
  fileSaveAsAction->setEnabled(enabled);
  filePrintAction->setEnabled(enabled);
  mouseActionGroup->setEnabled(enabled);
}
//**********************************************************************
///Destructor of viewGl
viewGl::~viewGl() {
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

  // set workspace background
  workspace->setBackground(QBrush(QPixmap(QString::fromUtf8(":/background_logo.png"))));

  aboutWidget=NULL;

  //=======================================
  //MDI
  workspace->setScrollBarsEnabled( true );

  // Window Menu
  connect(windowsMenu, SIGNAL( aboutToShow() ),
	  this, SLOT( windowsMenuAboutToShow() ) );
  connect(windowsMenu, SIGNAL(triggered(QAction*)),
	  this, SLOT( windowsMenuActivated(QAction*)));

  // initialization of Qt Assistant, the path should be in $PATH
#if defined(__APPLE__)
  std::string assistantPath(tlp::TulipLibDir);
  assistantPath += string("../") + QT_ASSISTANT;
  assistant = new QAssistantClient(assistantPath.c_str(), this);
#else
  assistant = new QAssistantClient("", this);
#endif
  connect(assistant, SIGNAL(error(const QString&)), SLOT(helpAssistantError(const QString&)));

  // if we have only one controller : auto load it
  MutableContainer<Controller *> controllers;
  ControllerPluginsManager::getInst().initControllerPluginsList(controllers);
  TemplateFactory<ControllerFactory, Controller, ControllerContext>::ObjectCreator::const_iterator it;
  it=ControllerFactory::factory->objMap.begin();
  string name=(*it).first;
  ++it;
  if(it==ControllerFactory::factory->objMap.end()){
    controllerAutoLoad=false;
    createController(name);
    controllerAutoLoad=true;
  }else{
    controllerAutoLoad=false;
  }

}
//**********************************************************************
std::string viewGl::newName() {
  static int idx = 0;

  if (idx++ == 0)
    return std::string(UNNAMED);

  stringstream ss;
  ss << UNNAMED << '_' << idx - 1;
  return ss.str();
}
//**********************************************************************
void viewGl::initializeGraph(Graph *graph) {
  graph->setAttribute("name", newName());
  graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  graph->getProperty<SizeProperty>("viewSize")->setAllEdgeValue(Size(0.125,0.125,0.5));
  graph->getProperty<ColorProperty>("viewColor")->setAllNodeValue(Color(255,0,0));
  graph->getProperty<ColorProperty>("viewColor")->setAllEdgeValue(Color(0,0,0));
  graph->getProperty<IntegerProperty>("viewShape")->setAllNodeValue(1);
  graph->getProperty<IntegerProperty>("viewShape")->setAllEdgeValue(0);
}
//**********************************************************************
void viewGl::fileNew(QAction *action) {
  string name=action->text().toStdString();
  createController(name);
  currentController->setData();
  enableElements(true);
}
//**********************************************************************
void viewGl::fileNew(bool checked) {
  MutableContainer<Controller *> controllers;
  ControllerPluginsManager::getInst().initControllerPluginsList(controllers);
  TemplateFactory<ControllerFactory, Controller, ControllerContext>::ObjectCreator::const_iterator it;
  it=ControllerFactory::factory->objMap.begin();
  string name=(*it).first;
  createController(name);
  currentController->setData();
  enableElements(true);
}
//**********************************************************************
void viewGl::createController(const string &name) {
  if(!controllerAutoLoad){
    if(currentController) {
      Graph *graph=currentController->getGraph();
      if(askSaveGraph(graph->getAttribute<string>("name")))
        return;
    }
    delete currentController;
    workspace->closeAllWindows();
    Controller *newController=ControllerPluginsManager::getInst().createController(name);
    newController->attachMainWindow(MainWindowFacade(this,toolBar,graphToolBar,workspace));
    currentController=newController;
    currentControllerName=name;
  }else{
    controllerAutoLoad=false;
  }
}
//**********************************************************************
bool viewGl::doFileSave() {
  if(!currentController) return false;
  if (openFiles.find((unsigned long)currentController)==openFiles.end() ||
      (openFiles[(unsigned long)currentController].name == "")) {
    return doFileSaveAs();
  }
  viewGlFile &vFile = openFiles[(unsigned long)currentController];
  return doFileSave("tlp", vFile.name, vFile.author, vFile.comments);
}
//**********************************************************************
void viewGl::fileSave() {
  doFileSave();
}
//**********************************************************************
bool viewGl::doFileSave(string plugin, string filename, string author, string comments) {
  if(!currentController) return false;

  DataSet dataSet;
  StructDef parameter = ExportModuleFactory::factory->getPluginParameters(plugin);
  parameter.buildDefaultDataSet(dataSet);
  if (author.length() > 0)
    dataSet.set<string>("author", author);
  if (comments.length() > 0)
    dataSet.set<string>("text::comments", comments);
  if (!tlp::openDataSetDialog(dataSet, 0, &parameter,
			      &dataSet, "Enter Export parameters", NULL,
			      this))
    return false;

  DataSet controller;
  DataSet controllerData;
  Graph *graph;
  currentController->getData(&graph,&controllerData);
  controller.set<DataSet>(currentControllerName,controllerData);
  dataSet.set<DataSet>("controller",controller);

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
  viewGlFile &vFile = openFiles[(unsigned long)currentController];
  vFile.name = filename;
  dataSet.get<string>("author", vFile.author);
  dataSet.get<string>("text::comments", vFile.comments);

  if (!(result=tlp::exportGraph(graph, *os, plugin, dataSet, NULL))) {
    QMessageBox::critical( 0, "Tulip export Failed",
			   "The file has not been saved"
			   );
  } else {
    statusBar()->showMessage((filename + " saved.").c_str());
  }
  //setNavigateCaption(filename);
  //setGraphName(graph, QString(filename.c_str()));
  delete os;
  return result;
}
//**********************************************************************
bool viewGl::doFileSaveAs() {
  return doFileSave("tlp", "", "", "");
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
void viewGl::fileOpen(string *plugin, QString &s) {
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
    }else {
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
    if(currentController && !controllerAutoLoad) {
      Graph *graph=currentController->getGraph();
      if(askSaveGraph(graph->getAttribute<string>("name")))
        return;
      delete currentController;
      currentController=NULL;
      workspace->closeAllWindows();
    }
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

    //GlMainWidget *glW = newOpenGlView(newGraph, s);
    //    changeGraph(0);
    QtProgress *progressBar = new QtProgress(this, string("Loading : ")+ s.section('/',-1).toAscii().data());
    // this will avoid too much notification when
    // importing a graph (see changeGraph)
    //importedGraph = newGraph;
    result = tlp::importGraph(*plugin, dataSet, progressBar ,newGraph);
    //importedGraph = 0;
    // now ensure notification
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
      QMessageBox::critical(this, errorTitle.c_str(), errorMsg.c_str());
      QApplication::restoreOverrideCursor();
      return;
    }

    delete progressBar;
    /*if(noPlugin)
      setGraphName(newGraph, s);*/
    QApplication::restoreOverrideCursor();

    if(dataSet.exist("controller")) {
      // last version of tlp file
      DataSet controller;
      dataSet.get<DataSet>("controller", controller);
      Iterator< std::pair<std::string, DataType*> > *it=controller.getValues();
      assert(it->hasNext());

      pair<string, DataType*> p;
      p = it->next();
      DataSet *controllerData=(DataSet*)p.second->value;
      string controllerName=p.first;

      createController(controllerName);
      currentController->setData(newGraph,*controllerData);
      enableElements(true);
    }else if(dataSet.exist("scene")) {
      // Tlp file with scene system
      string sceneDataStr;
      DataSet sceneData;
      dataSet.get<string>("scene", sceneDataStr);
      sceneData.set<string>("scene",sceneDataStr);
      createController("MainController");
      currentController->setData(newGraph,sceneData);
    }else if(dataSet.exist("displaying")) {
      // Tlp file with old system
      DataSet glGraphData;
      dataSet.get<DataSet>("displaying", glGraphData);
      DataSet displayingData;
      displayingData.set<DataSet>("displaying",glGraphData);
      createController("MainController");
      currentController->setData(newGraph,displayingData);
    }else{
      createController("MainController");
      currentController->setData(newGraph);
    }

    if(noPlugin) {
      viewGlFile vFile;
      vFile.name = s.toAscii().data();
      dataSet.get<std::string>("author", vFile.author);
      dataSet.get<std::string>("text::comments", vFile.comments);
      openFiles[((unsigned long)currentController)] = vFile;
    }

    enableElements(true);

    // synchronize overview display parameters
  } else {
    qWarning("Canceled  Open/import");
    }
}
//**********************************************************************
void viewGl::importGraph(QAction* action) {
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
    connect(newAction,SIGNAL(triggered(bool)), SLOT(fileNew(bool)));
    fileMenu->insertAction(fileOpenAction,newAction);
  }

  buildMenuWithContext<ExportModuleFactory, ExportModule>(exportGraphMenu, this, SLOT(exportGraph(QAction*)));
  buildMenuWithContext<ImportModuleFactory, ImportModule>(importGraphMenu, this, SLOT(importGraph(QAction*)));
  connect(&exportGraphMenu, SIGNAL(triggered(QAction*)), SLOT(exportGraph(QAction*)));
  connect(&importGraphMenu, SIGNAL(triggered(QAction*)), SLOT(importGraph(QAction*)));
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
  }else{
    windowsMenu->addSeparator();
    QWidgetList windows = workspace->windowList();
    for ( int i = 0; i < int(windows.count()); ++i ) {
      QAction* action = windowsMenu->addAction(windows.at(i)->windowTitle());
      action->setChecked(workspace->activeWindow() == windows.at(i));
      action->setData(QVariant(i));
    }
  }
}
//**********************************************************************
/* returns true if user canceled */
bool viewGl::askSaveGraph(const std::string name) {
  string message = "Do you want to save this graph : " + name + " ?";
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
  if(currentController) {
    Graph *graph=currentController->getGraph();
    bool canceled = askSaveGraph(graph->getAttribute<string>("name"));
    if (canceled)
      return false;
  }
  return true;
}
//**********************************************************************
int viewGl::alreadyTreated(set<unsigned long> treatedGraph, Graph *graph) {
  /*set<unsigned int>::iterator iterator = treatedGraph.begin();
  while(iterator != treatedGraph.end()) {
    unsigned int currentGraph = *iterator;
    if(currentGraph == (unsigned long)graph)
      return true;
    iterator++;
  }
  return false;*/
}
//**********************************************************************
void viewGl::closeEvent(QCloseEvent *e) {
  if (closeWin())
    e->accept();
  else
    e->ignore();
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
