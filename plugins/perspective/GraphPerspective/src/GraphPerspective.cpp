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
#include "GraphPerspective.h"

#include "ui_GraphPerspectiveMainWindow.h"

#include "GraphHierarchiesEditor.h"
#include "GraphHierarchiesModel.h"
#include <tulip/Graph.h>

//FIXME: remove me
#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/TlpTools.h>

using namespace tlp;

GraphPerspective::GraphPerspective(PerspectiveContext &c): Perspective(c), _ui(0), _graphs(new GraphHierarchiesModel(this)) {
}

void GraphPerspective::construct(tlp::PluginProgress *progress) {
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
  connect(_ui->actionFull_screen,SIGNAL(toggled(bool)),this,SLOT(showFullScreen(bool)));
  _ui->workspaceSplitter->setSizes(QList<int>() << 200 << 1000);
  _ui->docksSplitter->setSizes(QList<int>() << 500 << 800);

  _mainWindow->show();
  // Open project with model
  _graphs->readProject(_project,progress);

  if (_graphs->size() == 0)
    _graphs->addGraph(tlp::newGraph());

  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->algorithmRunner->setModel(_graphs);

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    connect(h,SIGNAL(expanded(bool)),this,SLOT(refreshDockExpandControls()));
  }

  Observable::holdObservers();
  QVector<View*> views;

  for (int i=0; i<50; ++i) {
    Graph* g = newGraph();
    node n = g->addNode();
    g->getProperty<ColorProperty>("viewColor")->setNodeValue(n,Color(255,255,255));
    g->getProperty<StringProperty>("viewLabel")->setNodeValue(n,("#" + QString::number(i)).toStdString());
    g->getProperty<IntegerProperty>("viewFontSize")->setAllNodeValue(18);
    g->getProperty<StringProperty>("viewFont")->setAllNodeValue(TulipBitmapDir + "font.ttf");
    views.push_back(_ui->workspace->addView("Node Link Diagram view",g));
  }

  Observable::unholdObservers();
//  _ui->workspace->switchToGridMode();

}

void GraphPerspective::refreshDockExpandControls() {
  QList<HeaderFrame *> expandedHeaders, collapsedHeaders;
  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    h->expandControl()->setEnabled(true);

    if (h->isExpanded())
      expandedHeaders.push_back(h);
    else
      collapsedHeaders.push_back(h);
  }

  if (expandedHeaders.size() == 1)
    expandedHeaders[0]->expandControl()->setEnabled(false);
}

void GraphPerspective::showFullScreen(bool f) {
  if (f) {
    _maximised = _mainWindow->isMaximized();
    _mainWindow->showFullScreen();
  }
  else {
    _mainWindow->showNormal();

    if (_maximised)
      _mainWindow->showMaximized();
  }
}

PERSPECTIVEPLUGIN(GraphPerspective,"Graph hierarchy analysis", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0")
