#include "tulip/WorkspacePanel.h"

#include <QtGui/QGraphicsView>
#include <QtCore/QDebug>

#include <tulip/Interactor.h>
#include <tulip/ForEach.h>
#include <tulip/View.h>
#include <tulip/Graph.h>
#include "ui_WorkspacePanel.h"

using namespace tlp;

WorkspacePanel::WorkspacePanel(tlp::Graph* graph, const QString& viewName, const tlp::DataSet& state, QWidget *parent)
  : QWidget(parent), _ui(new Ui::WorkspacePanel), _graph(graph), _view(NULL) {
  assert(graph);

  _ui->setupUi(this);

  QStringList installedViewNames;
  std::string name;
  forEach(name, ViewLister::availablePlugins())
  installedViewNames << name.c_str();
  _ui->viewCombo->addItems(installedViewNames);

  QString selectedViewName = viewName;

  if (!installedViewNames.contains(selectedViewName))
    selectedViewName = "Node Link Diagram view";

  setView(selectedViewName,state);
}

WorkspacePanel::~WorkspacePanel() {
}

View* WorkspacePanel::view() const {
  return _view;
}
void WorkspacePanel::setView(const QString &viewName,const tlp::DataSet& state) {
  if (!ViewLister::pluginExists(viewName.toStdString()))
    return;

  _ui->viewCombo->setCurrentIndex(_ui->viewCombo->findText(viewName));
  internalSetView(viewName,state);
}

tlp::Graph* WorkspacePanel::graph() const {
  return _graph;
}
void WorkspacePanel::setGraph(tlp::Graph *graph) {
  assert(graph);
  _graph = graph;
}

void WorkspacePanel::internalSetView(const QString &name,const DataSet& state) {
  if (_view != NULL) {
    _view->graphicsView()->deleteLater();
    delete _view;
  }

  _view = ViewLister::getPluginObject(name.toStdString(),NULL);
  assert(_view);
  _view->setGraph(graph());
  _view->setState(state);

  QList<Interactor*> interactors;
  QList<std::string> interactorNames = InteractorLister::compatibleInteractors(name.toStdString());
  foreach(std::string name,interactorNames)
  interactors << InteractorLister::getPluginObject(name,NULL);
  _view->setInteractors(interactors);

  _view->setupUi();



//  _ui->viewFrame->layout()->addWidget(_view->graphicsView());
  _view->graphicsView()->show();
}


