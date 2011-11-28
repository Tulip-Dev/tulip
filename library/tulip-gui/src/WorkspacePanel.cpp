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
#include <QtCore/QPropertyAnimation>
#include <QtGui/QGraphicsView>
#include <QtGui/QCloseEvent>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>

#include <tulip/TulipMetaTypes.h>
#include <tulip/ProcessingAnimationItem.h>
#include <tulip/Interactor.h>
#include <tulip/ForEach.h>
#include <tulip/View.h>
#include <tulip/Graph.h>
#include <tulip/GraphHierarchiesModel.h>
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

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(QColor(255,255,255));
    painter->setBrush(QColor(0,0,0,70));
    painter->drawRect(scene()->sceneRect());
    _animation->setOpacity(opacity());
    _animation->setPos(scene()->width()/2-16,scene()->height()/2-16);
  }
};

// ========================

WorkspacePanel::WorkspacePanel(tlp::View* view, const QString& viewName, QWidget *parent)
  : QWidget(parent), _ui(new Ui::WorkspacePanel), _view(NULL), _viewName(viewName), _progressItem(NULL) {
  _ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose,true);
  connect(_ui->closeButton,SIGNAL(clicked()),this,SLOT(close()));
  setView(view,viewName);
}

WorkspacePanel::~WorkspacePanel() {
  delete _ui;

  if (_view != NULL) {
    disconnect(_view,SIGNAL(destroyed()),this,SLOT(viewDestroyed()));
    delete _view;
  }

  emit closed(this);
}

View* WorkspacePanel::view() const {
  return _view;
}

QString WorkspacePanel::viewName() const {
  return _viewName;
}

void WorkspacePanel::setView(tlp::View* view, const QString& viewName) {
  assert(view != NULL);

  if (_view != NULL)
    disconnect(_view,SIGNAL(destroyed()),this,SLOT(viewDestroyed()));

  delete _view;

  _view = view;
  _viewName = viewName;

  QList<Interactor*> compatibleInteractors;
  QList<std::string> interactorNames = InteractorLister::compatibleInteractors(viewName.toStdString());
  foreach(std::string name,interactorNames) {
    compatibleInteractors << InteractorLister::getPluginObject(name,NULL);
  }
  _view->setInteractors(compatibleInteractors);
  _view->graphicsView()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  layout()->addWidget(_view->graphicsView());
  refreshInteractorsToolbar();

  if (compatibleInteractors.size()>0)
    setCurrentInteractor(compatibleInteractors[0]);

  connect(_view,SIGNAL(destroyed()),this,SLOT(viewDestroyed()));
  connect(_view,SIGNAL(graphSet(tlp::Graph*)),this,SLOT(viewGraphSet(tlp::Graph*)));
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
  QList<Interactor*> compatibleInteractors = _view->interactors();
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
    setCurrentInteractor(compatibleInteractors[0]);
  }
}

void WorkspacePanel::viewDestroyed() {
  _view = NULL;
  close();
}

void WorkspacePanel::setGraphsModel(tlp::GraphHierarchiesModel* model) {
  _ui->graphCombo->setModel(model);
  connect(_ui->graphCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(graphComboIndexChanged()));
}

void WorkspacePanel::viewGraphSet(tlp::Graph* g) {
  if (_ui->graphCombo->model() == NULL)
    return;
  tlp::GraphHierarchiesModel* model = static_cast<tlp::GraphHierarchiesModel*>(_ui->graphCombo->model());
  QModelIndex graphIndex = model->indexOf(g);
  if (graphIndex == _ui->graphCombo->selectedIndex())
    return;


  _ui->graphCombo->selectIndex(graphIndex);
}

void WorkspacePanel::graphComboIndexChanged() {
  tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
#ifndef NDEBUG
  if (g != NULL) {
    std::string name;
    g->getAttribute<std::string>("name",name);
    std::cerr << "selecting graph " << name << " in view" << std::endl;
  }
#endif /* NDEBUG */
  if (g != NULL && _view != NULL && g != _view->graph()) {
    _view->setGraph(g);
  }
}
