#include "GraphPerspective.h"

#include "ui_GraphPerspectiveMainWindow.h"

#include "GraphHierarchiesEditor.h"
#include "GraphHierarchiesModel.h"
#include <tulip/Graph.h>

//FIXME: remove me
#include <QtCore/QTimer>

using namespace tlp;

GraphPerspective::GraphPerspective(PerspectiveContext &c): Perspective(c), _ui(0), _graphs(new GraphHierarchiesModel(this)), _graphsEditor(0) {
}

void GraphPerspective::construct(tlp::PluginProgress *progress) {
  _ui = new Ui::GraphPerspectiveMainWindowData;
  _ui->setupUi(_mainWindow);
  _ui->workspaceSplitter->setSizes(QList<int>() << 200 << 1000);
  _ui->docksSplitter->setSizes(QList<int>() << 500 << 300);
  _mainWindow->show();

  // Open project with model

  _graphsEditor = new GraphHierarchiesEditor(_graphs,_mainWindow);
  connect(_ui->graphsHierarchyManagerButton,SIGNAL(clicked()),this,SLOT(showGraphHierarchyEditor()));
}

void GraphPerspective::showGraphHierarchyEditor() {
  _graphsEditor->move(_mainWindow->geometry().x() + _ui->centralWidget->x(),
                      _mainWindow->geometry().y() + _ui->mainWidget->geometry().y());
  _graphsEditor->resize(_ui->docksWidget->width()+20,_ui->docksWidget->height()-20);
  _graphsEditor->show();
}

PERSPECTIVEPLUGIN(GraphPerspective,"Graph hierarchy analysis", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0")
