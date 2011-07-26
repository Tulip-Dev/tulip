#include "GraphPerspective.h"

#include "ui_GraphPerspectiveMainWindow.h"

#include "GraphHierarchiesEditor.h"
#include "GraphHierarchiesModel.h"
#include <tulip/SimplePluginProgressWidget.h>

//FIXME: remove me
#include <tulip/Graph.h>

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

  //FIXME: remove me
  Graph *g = tlp::newGraph();
  for (int i=0;i<200;++i)
    g->addNode();
  Graph *g1 = g->addSubGraph();
  for (int i=0;i<200;++i)
    g->addNode();
  _graphs->addGraph(g);
  _graphsEditor = new GraphHierarchiesEditor(_graphs,_mainWindow);
  connect(_ui->graphsHierarchyManagerButton,SIGNAL(clicked()),_graphsEditor,SLOT(show()));
}

PERSPECTIVEPLUGIN(GraphPerspective,"Graph hierarchy analysis", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0")
