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
#include "tulip/WorkspacePanel.h"

#include <QtCore/QDebug>
#include <QtGui/QGraphicsView>
#include <QtGui/QPushButton>

#include <tulip/Interactor.h>
#include <tulip/ForEach.h>
#include <tulip/View.h>
#include <tulip/Graph.h>
#include "ui_WorkspacePanel.h"

using namespace tlp;

WorkspacePanel::WorkspacePanel(tlp::Graph* graph, const QString& viewName, const tlp::DataSet& state, QWidget *parent)
  : QWidget(parent), _ui(new Ui::WorkspacePanel), _graph(graph), _view(NULL), _progressMode(false) {
  _ui->setupUi(this);

  QStringList installedViewNames;
  std::string name;
  forEach(name, ViewLister::availablePlugins()) {
    installedViewNames << name.c_str();
  }
  _ui->viewCombo->addItems(installedViewNames);

  QString selectedViewName = viewName;

  if (!installedViewNames.contains(selectedViewName))
    selectedViewName = "Node Link Diagram view";

  setView(selectedViewName,state);
}

WorkspacePanel::~WorkspacePanel() {
  delete _ui;
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
  _view->setupUi();
  _view->setGraph(graph());
  _view->setState(state);

  QList<Interactor*> compatibleInteractors;
  QList<std::string> interactorNames = InteractorLister::compatibleInteractors(name.toStdString());
  foreach(std::string name,interactorNames) {
    compatibleInteractors << InteractorLister::getPluginObject(name,NULL);
  }
  _view->setInteractors(compatibleInteractors);

  delete _ui->interactorsFrame->layout();
  bool interactorsUiShown = compatibleInteractors.size() > 0;
  _ui->currentInteractorButton->setVisible(interactorsUiShown);
  _ui->interactorsFrame->setVisible(interactorsUiShown);
  _ui->sep1->setVisible(interactorsUiShown);
  _ui->sep2->setVisible(interactorsUiShown);

  if (interactorsUiShown) {
    QHBoxLayout* interactorsLayout = new QHBoxLayout;
    interactorsLayout->setContentsMargins(0,0,0,0);
    interactorsLayout->setSpacing(4);
    foreach(Interactor* i, compatibleInteractors) {
      QPushButton* button = new QPushButton();
      button->setMinimumSize(22,22);
      button->setMaximumSize(22,22);
      button->setIcon(i->action()->icon());
      button->setToolTip(i->action()->text());
      interactorsLayout->addWidget(button);
      connect(button,SIGNAL(clicked()),i->action(),SLOT(trigger()));
      connect(i->action(),SIGNAL(triggered()),this,SLOT(interactorActionTriggered()));
    }
    _ui->interactorsFrame->setLayout(interactorsLayout);
    internalSetCurrentInteractor(compatibleInteractors[0]);
  }

  _view->graphicsView()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  layout()->addWidget(_view->graphicsView());
  connect(_view->graphicsView()->scene(),SIGNAL(sceneRectChanged(QRectF)),this,SLOT(viewSceneRectChanged(QRectF)));
}

void WorkspacePanel::internalSetCurrentInteractor(tlp::Interactor *i) {
  view()->setCurrentInteractor(i);
  _ui->currentInteractorButton->setText(i->action()->text());
  _ui->currentInteractorButton->setIcon(i->action()->icon());
  _ui->currentInteractorButton->setChecked(false);
}

void WorkspacePanel::interactorActionTriggered() {
  QAction* action = static_cast<QAction*>(sender());
  Interactor* interactor = static_cast<Interactor*>(action->parent());

  if (interactor == view()->currentInteractor())
    return;

  internalSetCurrentInteractor(interactor);
}

bool WorkspacePanel::isProgressMode() const {
  return _progressMode;
}

void WorkspacePanel::toggleProgressMode(bool p) {
  _progressMode = p;
}

void WorkspacePanel::progress_handler(int step, int max_step) {
  if (!isProgressMode())
    return;
}

void WorkspacePanel::viewSceneRectChanged(const QRectF &) {
}
