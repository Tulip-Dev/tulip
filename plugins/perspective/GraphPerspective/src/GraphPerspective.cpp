#include "GraphPerspective.h"

#include "ui_GraphPerspectiveMainWindow.h"

#include "GraphHierarchiesEditor.h"
#include "GraphHierarchiesModel.h"
#include <tulip/Graph.h>

using namespace tlp;

GraphPerspective::GraphPerspective(PerspectiveContext &c): Perspective(c), _ui(0), _graphs(new GraphHierarchiesModel(this)) {
}

void GraphPerspective::construct(tlp::PluginProgress *progress) {
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
  _ui->workspaceSplitter->setSizes(QList<int>() << 200 << 1000);
  _ui->docksSplitter->setSizes(QList<int>() << 500 << 300 << 300);

  _mainWindow->show();
  // Open project with model
  _graphs->readProject(_project,progress);

  if (_graphs->size() == 0)
    _graphs->addGraph(tlp::newGraph());

  _ui->graphHierarchiesEditor->setModel(_graphs);

  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>())
    connect(h,SIGNAL(expanded(bool)),this,SLOT(refreshDockExpandControls()));

  connect(_ui->graphsHierarchyManagerButton,SIGNAL(toggled(bool)),this,SLOT(refreshDockExpandControls()));
}

void GraphPerspective::refreshDockExpandControls() {
  QList<HeaderFrame *> expandedHeaders, collapsedHeaders;
  foreach(HeaderFrame *h, _ui->docksSplitter->findChildren<HeaderFrame *>()) {
    h->expandControl()->setEnabled(true);

    if (h->parentWidget() == _ui->graphHierarchiesEditor)
      continue;

    if (h->isExpanded())
      expandedHeaders.push_back(h);
    else
      collapsedHeaders.push_back(h);
  }

  if (expandedHeaders.size() == 1 && !_ui->graphHierarchiesEditor->isVisible())
    expandedHeaders[0]->expandControl()->setEnabled(false);

  if (expandedHeaders.size() == 0 && !_ui->graphHierarchiesEditor->isVisible()) {
    collapsedHeaders[0]->expand(true);
    collapsedHeaders[0]->expandControl()->setEnabled(false);
  }
}

PERSPECTIVEPLUGIN(GraphPerspective,"Graph hierarchy analysis", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0")
