/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <QtGui/qapplication.h>
#include <QtCore/QTimer>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QWorkspace>

#include <tulip/Graph.h>
#include <tulip/SizeProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/MouseInteractors.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/MainController.h>
#include <tulip/ViewPluginsManager.h>

#include "MainControllerProfile.h"

using namespace std;
using namespace tlp;

MainWindowFacade initMainWindow(QMainWindow &mainWindow){
  QMenuBar *menuBar=mainWindow.menuBar();
  menuBar->addMenu("&Windows");
  QToolBar *mainToolBar=new QToolBar(&mainWindow);
  mainWindow.addToolBar(mainToolBar);
  QToolBar *graphToolBar=new QToolBar(&mainWindow);
  mainWindow.addToolBar(graphToolBar);
  QWorkspace *workspace = new QWorkspace(&mainWindow);
  mainWindow.setCentralWidget(workspace);
  return MainWindowFacade(&mainWindow,mainToolBar,graphToolBar,workspace);
}

/*******************************************************************/
int main(int argc,char ** argv ){

  if (argc<2) {
    cerr << "usage :" << endl;
    cerr << "\t " << argv[0] << " <filename>" << endl;
    cerr << "file must be in tlp format" << endl;
    exit(1);
  }

  QApplication MainApp(argc,argv);
  QMainWindow mainWindow;

  MainWindowFacade mainWindowFacade=initMainWindow(mainWindow);
  /****************************************************/
  tlp::initTulipLib();

  PluginLoaderTxt txtPlug;
  tlp::loadPlugins(&txtPlug);   // library side plugins
  GlyphManager::getInst().loadPlugins(&txtPlug);   // software side plugins, i.e. glyphs
  ViewPluginsManager::getInst().loadPlugins(&txtPlug);
  //  GlGraph::loadPlugins(); //Glyoh plugins */
  /****************************************************/
  MainController *mainController=new MainController;
  mainController->attachMainWindow(mainWindowFacade);

  DataSet dataSet;
  cout << argv[1] << endl;
  string filename=argv[1];
  dataSet.set("file::filename", filename);
  Graph *newGraph = tlp::newGraph();
  importGraph("tlp", dataSet, NULL,newGraph);

  DataSet controller;
  dataSet.get<DataSet>("controller", controller);
  Iterator< std::pair<std::string, DataType*> > *it=controller.getValues();
  assert(it->hasNext());

  pair<string, DataType*> p;
  p = it->next();
  DataSet *controllerData=(DataSet*)p.second->value;
  string controllerName=p.first;

  mainController->setData(newGraph,*controllerData);

  MainControllerProfile profile(newGraph,mainController);

  QTimer::singleShot(0, &profile, SLOT(delNodes()));

  mainWindow.show();

  return MainApp.exec();
}
