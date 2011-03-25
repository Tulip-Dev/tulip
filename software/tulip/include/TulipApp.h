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

#ifndef TULIPAPP_H
#define TULIPAPP_H

#include <QtGui/qmessagebox.h>
#include <QtGui/qmainwindow.h>
#include <QtCore/QSettings>
#include <QtCore/QProcess>

#include <PluginsUpdateChecker.h>
#include "TulipPluginLoader.h"
#include "TulipData.h"
#include "GraphNeedsSavingObserver.h"

namespace tlp {
  class Controller;
  class Graph;
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
class TulipApp : public QMainWindow, public Ui::TulipData {
  Q_OBJECT;

public:
  TulipApp(QWidget *parent=NULL);
  virtual ~TulipApp();
  void setParameters(const tlp::DataSet &);

  static bool needRestart() {
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    bool needStart = settings.value("needRestart",false).toBool();
    settings.endGroup();
    return needStart;
  }

  static void enableRestart() {
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    settings.setValue("needRestart", true);
    settings.endGroup();
    settings.sync();
  }

  static void disableRestart() {
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("PluginsManager");
    settings.setValue("needRestart", false);
    settings.endGroup();
    settings.sync();
  }

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
  void helpDocumentation();
  void helpAbout();
  void preference();
  void plugins();
  void displayRestartForPlugins();
  void helpAssistantError(const QString &msg);
  void fileExit();
  void fileSave();
  void fileSaveAs();
  void fileNew(QAction *action);
  bool fileNew(bool);
  void fileCloseTab();
  void closeTab(int index);
  bool createController(const std::string &name,const std::string &graphName);
  void fileOpen();
  void filePrint();
  void importGraph();
  void exportGraph();
  void importGraph(QAction*);
  void exportGraph(QAction*);
  void windowsMenuAboutToShow();
  void windowsMenuActivated(QAction*);
  void deletePluginsUpdateChecker();
  void tabChanged(int index);
  void cascade();
  void closeAll();

private:
  void buildMenus();
  bool doFileSave(int);
  bool doFileSaveAs();
  bool doFileSave(tlp::Controller *controllerToSave,std::string plugin, std::string filename, std::string author, std::string comments);
  /**
    * @brief Ask user to save a graph. If user select "Yes" automatically perform the saving of the graph.
    * @param name The name of the graph to save.
    * @param index The index of the controller containing the graph to save.
    * @param activateNoToAll Allow user to select the "no to all" button.
    * @return The button clicked by the user.
    */
   QMessageBox::StandardButton askSaveGraph(const std::string &name,int index, bool activateNoToAll=false );
  bool closeWin();
  void saveActions(QWidget *widget,tlp::Controller *controller,std::map<tlp::Controller *,std::vector<QAction *> > &mapToSave);
  void clearInterface();
  void loadInterface(int index);
  void saveInterface(int index);

  QMenu *newMenu;
  QAction *newAction;
  QTabWidget *tabWidget;
  unsigned int mouseClicX,mouseClicY;

  QProcess *assistantProcess;
  QString assistantProcessApp;

	/*std::string currentControllerName;
  tlp::Controller *currentController;*/
  bool controllerAutoLoad;
  int currentTabIndex;
  std::map<int,tlp::Controller *> tabIndexToController;
  std::map<tlp::Controller *, GraphNeedsSavingObserver*>controllerToGraphObserver;
  std::map<tlp::Controller *, std::string> controllerToControllerName;
  std::map<tlp::Controller *,QWorkspace *> controllerToWorkspace;
  std::map<tlp::Controller *,std::vector<QAction *> > controllerToMenu;
  std::map<tlp::Controller *,std::vector<QAction *> > controllerToToolBar;
  std::map<tlp::Controller *,std::vector<QAction *> > controllerToGraphToolBar;
  std::map<tlp::Controller *,std::vector<std::pair<Qt::DockWidgetArea,QDockWidget *> > > controllerToDockWidget;
  std::map<tlp::Controller *,std::vector<std::pair<Qt::ToolBarArea, QToolBar *> > > controllerToCustomToolBar;
  std::map<tlp::Controller *,std::vector<std::pair<QWidget *, bool> > > controllerToWidgetVisible;
  std::map<tlp::Controller *,std::pair<std::string,std::string > > controllerToStatusBar;
  std::map<tlp::Controller *,FileInfo> openFiles;
 };

#endif // viewGl_included

