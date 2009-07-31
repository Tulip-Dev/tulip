//-*-c++-*-

#ifndef viewGl_included
#define viewGl_included
#include <map>

#include <vector>
#include <QtGui/qdockwidget.h>
#include <QtGui/qsplitter.h>
#include <QtCore/qstring.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qmenu.h>
#include <QtGui/QTabWidget>
#include <QtAssistant/qassistantclient.h>
#include <string>
#include <tulip/Reflect.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/Observable.h>
#include <tulip/GlSceneObserver.h>
#include <PluginsUpdateChecker.h>
#include "TulipPluginLoader.h"
#include "TulipData.h"

namespace tlp {
  class Controller;
}

//class QWorkspace;
class QProgressDialog;
class QEvent;
class View3DSetup;
class Cluster;

// minimal structure to keep open files infos
struct FileInfo {
  std::string name;
  std::string author;
  std::string comments;
};

///Widget for manipulation and visualization of a graph
class viewGl : public QMainWindow, public Ui::TulipData {
  Q_OBJECT;

public:
  viewGl(QWidget *parent=NULL);
  virtual ~viewGl();
  void setParameters(const tlp::DataSet &);

protected:
  QWidget *aboutWidget;
  //tlp::GlMainWidget *glWidget;
  tlp::TulipPluginLoader pluginLoader;
  tlp::PluginsUpdateChecker *pluginsUpdateChecker;
  tlp::MultiServerManager *multiServerManager;
  bool elementsDisabled;

  QMenu importGraphMenu;
  QMenu exportGraphMenu;

  void enableQMenu(QMenu *, bool);
  void enableElements(bool);
  std::string newName();
  void initializeGraph(tlp::Graph *);

public slots:
  void startTulip();
  void fileOpen(std::string *,QString &);
  void closeEvent(QCloseEvent *e);

protected slots:
  void helpIndex();
  void helpContents();
  void helpAbout();
  void plugins();
  void helpAssistantError(const QString &msg);
  void fileExit();
  void fileSave();
  void fileSaveAs();
  void fileNew(QAction *action);
  bool fileNew(bool);
  void fileCloseTab();
  bool createController(const std::string &name,const std::string &graphName);
  void fileOpen();
  void filePrint();
  void importGraph(QAction*);
  void exportGraph(QAction*);
  void windowsMenuAboutToShow();
  void windowsMenuActivated(QAction*);
  void deletePluginsUpdateChecker();
  void tabChanged(int index);

private:
  void buildMenus();
  bool doFileSave(int);
  bool doFileSaveAs();
  bool doFileSave(tlp::Controller *controllerToSave,std::string plugin, std::string filename, std::string author, std::string comments);
  bool askSaveGraph(const std::string name,int index);
  bool closeWin();
  void saveActions(QWidget *widget,tlp::Controller *controller,std::map<tlp::Controller *,std::vector<QAction *> > &mapToSave);
  void clearInterface();
  void loadInterface(int index);
  void saveInterface(int index);

  QMenu *newMenu;
  QAction *newAction;
  unsigned int mouseClicX,mouseClicY;

  QAssistantClient* assistant;
	/*std::string currentControllerName;
  tlp::Controller *currentController;*/
  bool controllerAutoLoad;
  int currentTabIndex;
  std::map<int,tlp::Controller *> tabIndexToController;
  std::map<tlp::Controller *, std::string> controllerToControllerName;
  std::map<tlp::Controller *,QWorkspace *> controllerToWorkspace;
  std::map<tlp::Controller *,std::vector<QAction *> > controllerToMenu;
  std::map<tlp::Controller *,std::vector<QAction *> > controllerToToolBar;
  std::map<tlp::Controller *,std::vector<QAction *> > controllerToGraphToolBar;
  std::map<tlp::Controller *,std::vector<std::pair<Qt::DockWidgetArea,QDockWidget *> > > controllerToDockWidget;
  std::map<tlp::Controller *,std::pair<std::string,std::string > > controllerToStatusBar;
  std::map<tlp::Controller *,FileInfo> openFiles;
 };

#endif // viewGl_included

