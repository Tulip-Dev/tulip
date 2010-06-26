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
#include <QtCore/QObject>
#include <QtTest/QtTest>
#include <QtGui/QMainWindow>
#include <QtGui/QWorkspace>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>

#include <tulip/Graph.h>
#include <tulip/GlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/Controller.h>
#include <tulip/MainController.h>
#include <tulip/ControllerPluginsManager.h>
#include <tulip/NodeLinkDiagramComponent.h>

using namespace std;
using namespace tlp;

class OpenGraphTest : public QObject {

  Q_OBJECT
private slots:
  void initTestCase();
  void testLoadGraph();
  void testOpenController();
  void testViewIsOpen();
  void testInteractorActionsAreInstalled();
  void testNodesEdgesDisplay();
  void testAugmentedDisplay();

private :
  DataSet *dataSet;
  Graph *graph;
  QMainWindow *mainWindow;
  QToolBar *graphToolBar;
  QWorkspace *workspace;
  MainController *controller;
  NodeLinkDiagramComponent *nldc;
  GlMainWidget *mainWidget;
};

void OpenGraphTest::initTestCase() {
  initTulipLib();
  loadPlugins();
  GlyphManager::getInst().loadPlugins();   // software side plugins, i.e. glyphs
  InteractorManager::getInst().loadPlugins(); // interactors plugins
  ViewPluginsManager::getInst().loadPlugins(); // view plugins
  ControllerPluginsManager::getInst().loadPlugins(); // controller plugins


  dataSet = new DataSet();
  graph = tlp::newGraph();

  mainWindow =new QMainWindow();
  mainWindow->menuBar()->addAction("&Windows");
  graphToolBar=mainWindow->addToolBar("graphToolBar");
  workspace=new QWorkspace();
  mainWindow->setCentralWidget(workspace);
}
//==========================================================
void OpenGraphTest::testLoadGraph() {
  dataSet->set("file::filename",string("../DATA/OpenGraphTest.tlp"));
  tlp::importGraph("tlp", *dataSet, NULL ,graph);

  QVERIFY(graph->numberOfNodes()==3);
}
//==========================================================
void OpenGraphTest::testOpenController() {
  QVERIFY(dataSet->exist("controller")==true);

  DataSet controller;
  dataSet->get<DataSet>("controller", controller);
  Iterator< std::pair<std::string, DataType*> > *it=controller.getValues();

  QVERIFY(it->hasNext()==true);

  pair<string, DataType*> p;
  p = it->next();
  DataSet *controllerData=(DataSet*)p.second->value;
  string controllerName=p.first;

  this->controller=(MainController*)ControllerPluginsManager::getInst().createController(controllerName);
  QVERIFY(this->controller!=NULL);
  this->controller->attachMainWindow(MainWindowFacade(mainWindow,graphToolBar,workspace));
  this->controller->setData(graph,*controllerData);
}
//==========================================================
void OpenGraphTest::testViewIsOpen() {
  QWidgetList widgetList=workspace->windowList();
  QVERIFY(widgetList.size()==1);
  nldc=dynamic_cast<NodeLinkDiagramComponent*>(controller->getView(widgetList[0]));
  QVERIFY(nldc!=NULL);
}
//==========================================================
void OpenGraphTest::testInteractorActionsAreInstalled() {
  QList<QAction*> actions=graphToolBar->actions();
  QVERIFY(actions.size()==10);
}
//==========================================================
void OpenGraphTest::testNodesEdgesDisplay() {
  QWidgetList widgetList=workspace->windowList();
  QRect rect=widgetList[0]->contentsRect();
  mainWidget=nldc->getGlMainWidget();
  vector<node> node;
  vector<edge> edge;
  mainWidget->doSelect(0,0,rect.width(),rect.height(),node,edge);
  QVERIFY(node.size()==3);
  QVERIFY(edge.size()==3);
}
//==========================================================
void OpenGraphTest::testAugmentedDisplay() {
  QWidgetList widgetList=workspace->windowList();
  QRect rect=widgetList[0]->contentsRect();
  GlLayer *foreground=mainWidget->getScene()->getLayer("Foreground");
  vector<GlEntity *> entities;
  mainWidget->selectGlEntities(0, 0,rect.width(), rect.height(),entities,foreground);
  QVERIFY(entities.size()==1);
}
//==========================================================
QTEST_MAIN(OpenGraphTest)
#include "OpenGraphTest.moc"
