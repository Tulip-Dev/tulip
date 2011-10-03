#include "GraphPerspective.h"

#include "ui_GraphPerspectiveMainWindow.h"

#include "GraphHierarchiesEditor.h"
#include "GraphHierarchiesModel.h"
#include <tulip/Graph.h>
#include <QtCore/QDebug>

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

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>())
    connect(h,SIGNAL(expanded(bool)),this,SLOT(refreshDockExpandControls()));
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
