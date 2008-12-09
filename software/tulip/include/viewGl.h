//-*-c++-*-

#ifndef viewGl_included
#define viewGl_included
#include <map>

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <vector>
#include <QtGui/qdockwidget.h>
#include <QtGui/qsplitter.h>
#include <QtCore/qstring.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qmenu.h>
#include <QtAssistant/qassistantclient.h>
#include <string>
#include <tulip/Reflect.h>
#include <tulip/Graph.h>
#include <tulip/GlScene.h>
#include <tulip/Observable.h>
#include <tulip/GlSceneObserver.h>
#include <tulip/GWInteractor.h>
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
struct viewGlFile {
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
  void fileNew(bool);
  void createController(const std::string &name);
  void fileOpen();
  void filePrint();
  void importGraph(QAction*);
  void exportGraph(QAction*);
  void windowsMenuAboutToShow();
  void windowsMenuActivated(QAction*);
  void deletePluginsUpdateChecker();

private:
  void buildMenus();
  stdext::hash_map<unsigned long, viewGlFile> openFiles;
  bool doFileSave();
  bool doFileSaveAs();
  bool doFileSave(std::string plugin, std::string filename, std::string author, std::string comments);
  bool askSaveGraph(const std::string name);
  bool closeWin();
  int alreadyTreated(std::set<unsigned long>, tlp::Graph *);

  QMenu *newMenu;
  QAction *newAction;
  unsigned int mouseClicX,mouseClicY;

  QAssistantClient* assistant;
	std::string currentControllerName;
  tlp::Controller *currentController;
  bool controllerAutoLoad;
};

#endif // viewGl_included

