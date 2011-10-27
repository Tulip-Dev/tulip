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
#include <QtGui/QApplication>
#include <QtCore/QPropertyAnimation>

#include <tulip/ProcessingAnimationItem.h>
#include <tulip/Interactor.h>
#include <tulip/ForEach.h>
#include <tulip/View.h>
#include <tulip/Graph.h>
#include "ui_WorkspacePanel.h"

using namespace tlp;

// helper class
class ProgressItem: public QGraphicsObject {
  ProcessingAnimationItem* _animation;
public:
  ProgressItem(QGraphicsScene* parentScene): QGraphicsObject() {
    _animation = new ProcessingAnimationItem(QPixmap(":/tulip/gui/ui/process-working.png"),QSize(64,64));
    _animation->setParentItem(this);
    parentScene->addItem(_animation);
  }

  QRectF boundingRect() const {
    return QRectF();
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(QColor(255,255,255));
    painter->setBrush(QColor(0,0,0,70));
    painter->drawRect(scene()->sceneRect());
    _animation->setOpacity(opacity());
    _animation->setPos(scene()->width()/2-16,scene()->height()/2-16);
  }
};

// ========================

WorkspacePanel::WorkspacePanel(tlp::Graph* graph, const QString& viewName, const tlp::DataSet& state, QWidget *parent)
  : QWidget(parent), _ui(new Ui::WorkspacePanel), _graph(graph), _view(NULL), _progressItem(NULL) {
  _ui->setupUi(this);

  QString selectedViewName = viewName;
  if (!ViewLister::pluginExists(selectedViewName))
    selectedViewName = "Node Link Diagram view"; // Always fall back to a Node link diagram view
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
  _view->graphicsView()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  layout()->addWidget(_view->graphicsView());
}

tlp::Graph* WorkspacePanel::graph() const {
  return _graph;
}
void WorkspacePanel::setGraph(tlp::Graph *graph) {
  assert(graph);
  _graph = graph;
}

void WorkspacePanel::setCurrentInteractor(tlp::Interactor *i) {
  assert(i);

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

  setCurrentInteractor(interactor);
}

bool WorkspacePanel::isProgressMode() const {
  return _progressItem != NULL;
}

void WorkspacePanel::toggleProgressMode(bool p) {
  assert(view() && view()->graphicsView() && view()->graphicsView()->scene());

  if (p && _progressItem == NULL)  {
    _progressItem = new ProgressItem(_view->graphicsView()->scene());
    _view->graphicsView()->scene()->addItem(_progressItem);
    _progressFadeIn = new QPropertyAnimation(_progressItem,"opacity",_progressItem);
    _progressFadeIn->setStartValue(0);
    _progressFadeIn->setEndValue(1);
    _progressFadeIn->setDuration(800);
    _progressFadeIn->start(QAbstractAnimation::DeleteWhenStopped);
  }
  else if (!p && _progressItem != NULL) {
    delete _progressItem;
    _progressItem = NULL;
  }
}

void WorkspacePanel::progress_handler(int,int) {
  if (!isProgressMode())
    toggleProgressMode(true);
  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void WorkspacePanel::refreshInteractorsToolbar() {
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
