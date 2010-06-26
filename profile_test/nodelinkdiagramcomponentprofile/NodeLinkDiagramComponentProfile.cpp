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
#include <QtGui/qapplication.h>
#include <QtCore/QTimer>

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


#include "ViewProfile.h"

using namespace std;
using namespace tlp;

/*******************************************************************/
Graph * importGraph(GlMainWidget *glw) {
  DataSet dataSet;
  Graph *graph = newGraph();
  graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  graph->getProperty<SizeProperty>("viewSize")->setAllEdgeValue(Size(0.125,0.125,0.5));
  graph->getProperty<ColorProperty>("viewColor")->setAllNodeValue(Color(255,0,0));
  graph->getProperty<ColorProperty>("viewColor")->setAllEdgeValue(Color(0,0,0));
  graph->getProperty<IntegerProperty>("viewShape")->setAllNodeValue(1);
  graph->getProperty<IntegerProperty>("viewShape")->setAllEdgeValue(0);
  graph->getProperty<LayoutProperty>("viewLayout")->setAllNodeValue(Coord(0,0,0));
  if (graph != 0) {
    openGraphOnGlMainWidget(graph,&dataSet,glw);
  }
  return graph;
}
/*******************************************************************/
int main(int argc,char ** argv ){

  QApplication MainApp(argc,argv);

  /****************************************************/
  tlp::initTulipLib();

  PluginLoaderTxt txtPlug;
  tlp::loadPlugins(&txtPlug);   // library side plugins
  GlyphManager::getInst().loadPlugins(&txtPlug);   // software side plugins, i.e. glyphs
  //  GlGraph::loadPlugins(); //Glyoh plugins */
  /****************************************************/
  NodeLinkDiagramComponent MainWin;
  QWidget *widget=MainWin.construct(NULL);
  //MainApp.setMainWidget(&MainWin);
  widget->resize(640,480);
  widget->show();

  Graph *graph;

  graph=importGraph(MainWin.getGlMainWidget());
  ViewProfile *profile=new ViewProfile(graph,&MainWin);

  MainWin.init();
  MouseNKeysNavigator m;
  MainWin.pushInteractor(&m);
  QTimer::singleShot(0, profile, SLOT(addNodes()));
  return MainApp.exec();
}
