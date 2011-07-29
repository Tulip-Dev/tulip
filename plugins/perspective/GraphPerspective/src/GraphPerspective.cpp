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
  _ui->docksSplitter->setSizes(QList<int>() << 500 << 300);

  _mainWindow->show();
  // Open project with model
  _graphs->readProject(_project,progress);
  if (_graphs->size() == 0)
    _graphs->addGraph(tlp::newGraph());

  _ui->graphHierarchiesEditor->setModel(_graphs);
  _ui->graphHierarchiesEditor->hide();
  connect(_ui->graphsHierarchyManagerButton,SIGNAL(toggled(bool)),this,SLOT(graphHierarchiesEditorDisplayed(bool)));
}

void GraphPerspective::graphHierarchiesEditorDisplayed(bool f) {
  _ui->algorithmRunner->setVisible(!f);
  _ui->filtersManager->setVisible(!f);
}



PERSPECTIVEPLUGIN(GraphPerspective,"Graph hierarchy analysis", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0")
