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
#include "tulip/Workspace.h"

#include <math.h>

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsEffect>
#include <QtGui/QGraphicsSceneDragDropEvent>

#include <tulip/TulipMetaTypes.h>
#include <tulip/View.h>
#include <tulip/WorkspacePanel.h>

#include <QtCore/QDebug>

#ifndef NDEBUG
#include <modeltest.h>
#endif /* NDEBUG */

#include "ui_Workspace.h"

using namespace tlp;

/*
  Helper storage class to ensure synchronization between panels list and model passed down to opened panels
  */
Workspace::PanelsStorage::PanelsStorage(QObject* parent): QAbstractItemModel(parent) {
}
Workspace::PanelsStorage::PanelsStorage(const PanelsStorage& cpy): QAbstractItemModel(cpy.QObject::parent()) {
  _storage = cpy._storage;
}
void Workspace::PanelsStorage::push_back(tlp::WorkspacePanel* panel) {
  _storage.push_back(panel);
}
int Workspace::PanelsStorage::removeAll(tlp::WorkspacePanel* panel) {
  return _storage.removeAll(panel);
}
tlp::WorkspacePanel* Workspace::PanelsStorage::operator[](int i) const {
  return _storage[i];
}
tlp::WorkspacePanel* Workspace::PanelsStorage::operator[](int i) {
  return _storage[i];
}
int Workspace::PanelsStorage::size() const {
  return _storage.size();
}
Workspace::PanelsStorage::iterator Workspace::PanelsStorage::begin() {
  return _storage.begin();
}
Workspace::PanelsStorage::iterator Workspace::PanelsStorage::end() {
  return _storage.end();
}
Workspace::PanelsStorage::const_iterator Workspace::PanelsStorage::begin() const {
  return _storage.begin();
}
Workspace::PanelsStorage::const_iterator Workspace::PanelsStorage::end() const {
  return _storage.end();
}
QModelIndex Workspace::PanelsStorage::index(int row, int column,const QModelIndex &parent) const {
  if (!hasIndex(row,column,parent))
    return QModelIndex();

  return createIndex(row,column);
}
QModelIndex Workspace::PanelsStorage::parent(const QModelIndex &child) const {
  return QModelIndex();
}
int Workspace::PanelsStorage::rowCount(const QModelIndex &parent) const {
  if (parent != QModelIndex())
    return 0;

  return _storage.size();
}
int Workspace::PanelsStorage::columnCount(const QModelIndex&) const {
  return 1;
}
QVariant Workspace::PanelsStorage::data(const QModelIndex &index, int role) const {
  if (!hasIndex(index.row(),index.column(),index.parent()))
    return QVariant();

  WorkspacePanel* panel = _storage[index.row()];

  if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
    return panel->windowTitle();

  return QVariant();
}
// ***********************************************

Workspace::Workspace(QWidget *parent)
  : QWidget(parent), _ui(new Ui::Workspace), _currentPanelIndex(0), _model(NULL) {
  _ui->setupUi(this);
  connect(_ui->startupButton,SIGNAL(clicked()),this,SIGNAL(addPanelRequest()));

#ifndef NDEBUG
  new ModelTest(&_panels,this);
#endif /* NDEBUG */

  // This map allows us to know how much slots we have for each mode and which widget corresponds to those slots
  QVector<PlaceHolderWidget*> startupVector(0);
  QVector<PlaceHolderWidget*> singleVector(1);
  singleVector[0] = _ui->singlePage;
  QVector<PlaceHolderWidget*> splitVector(2);
  splitVector[0] = _ui->splitPagePanel1;
  splitVector[1] = _ui->splitPagePanel2;
  QVector<PlaceHolderWidget*> gridVector(4);
  gridVector[0] = _ui->gridPagePanel1;
  gridVector[1] = _ui->gridPagePanel2;
  gridVector[2] = _ui->gridPagePanel3;
  gridVector[3] = _ui->gridPagePanel4;
  _modeToSlots[_ui->startupPage] = startupVector;
  _modeToSlots[_ui->singlePage] = singleVector;
  _modeToSlots[_ui->splitPage] = splitVector;
  _modeToSlots[_ui->gridPage] = gridVector;

  // This map allows us to know wich widget can toggle a mode
  _modeSwitches[_ui->singlePage] = _ui->singleModeButton;
  _modeSwitches[_ui->splitPage] = _ui->splitModeButton;
  _modeSwitches[_ui->gridPage] = _ui->gridModeButton;
  updateAvailableModes();
}

Workspace::~Workspace() {
  delete _ui;
}

void Workspace::setModel(tlp::GraphHierarchiesModel* model) {
  _model = model;

  if (_model != NULL) {
    foreach(WorkspacePanel* panel,_panels)
    panel->setGraphsModel(_model);
  }
}

QList<tlp::View*> Workspace::panels() const {
  QList<tlp::View*> result;
  foreach(WorkspacePanel* panel, _panels) {
    result.push_back(panel->view());
  }
  return result;
}

QString Workspace::panelTitle(tlp::WorkspacePanel* panel) const {
  int digit = 0;

  QRegExp regExp("^.*(?:<([^>])*>){1}$");
  foreach(WorkspacePanel* other, _panels) {
    if (other == panel)
      continue;

    if (other->viewName() == panel->viewName()) {
      if (regExp.exactMatch(other->windowTitle()))
        digit = std::max<int>(digit,regExp.cap(1).toInt());
      else
        digit = std::max<int>(digit,1);
    }
  }

  if (digit == 0) {
    return panel->viewName();
  }

  return panel->viewName() + " <" + QString::number(digit+1) + ">";
}

void Workspace::addPanel(tlp::View* view, const QString& viewName) {
  WorkspacePanel* panel = new WorkspacePanel(view,viewName);

  if (_model != NULL)
    panel->setGraphsModel(_model);

  panel->installEventFilter(this);
  panel->setWindowTitle(panelTitle(panel));
  connect(panel,SIGNAL(drawNeeded()),this,SLOT(viewNeedsDraw()));
  connect(panel,SIGNAL(destroyed(QObject*)),this,SLOT(panelDestroyed(QObject*)));

  // Add it to the list
  _panels.push_back(panel);

  // activate available modes
  updateAvailableModes();

  // If on startup mode, switch to single
  if (currentModeWidget() == _ui->startupPage) {
    switchToSingleMode();
  }
  else {
    updatePanels();
  }

  // Force the first panel's graph combo box update when underleying model has been updated.
  panel->viewGraphSet(view->graph());
}


void Workspace::delView(tlp::View* view) {
  WorkspacePanel* panel = NULL;
  foreach(WorkspacePanel* it, _panels) {
    if (it->view() == view) {
      panel = it;
      break;
    }
  }
  delete panel;
}

void Workspace::panelDestroyed(QObject* obj) {
  WorkspacePanel* panel = static_cast<WorkspacePanel*>(obj);
  int removeCount = _panels.removeAll(panel);
  assert(removeCount>0);
  if (removeCount==0)
    return;
  updateAvailableModes();

  if (!_modeSwitches[currentModeWidget()]->isEnabled()) {
    int maxSize = 0;
    QWidget* fallbackMode = _ui->startupPage;

    // Current mode is not available, fallback to the largest available mode
    foreach(QWidget* it, _modeToSlots.keys()) {
      if (_panels.size() >= _modeToSlots[it].size() && _modeToSlots[it].size() > maxSize) {
        maxSize = _modeToSlots[it].size();
        fallbackMode = it;
      }
    }
    switchWorkspaceMode(fallbackMode);
  }

  if (currentModeWidget() == _ui->startupPage) {
    return;
  }

  updatePanels();
}

void Workspace::viewNeedsDraw() {
  WorkspacePanel* panel = static_cast<WorkspacePanel*>(sender());
  panel->toggleProgressMode(true);
  panel->view()->draw(panel);
  panel->toggleProgressMode(false);
}

void Workspace::switchToStartupMode() {
  switchWorkspaceMode(_ui->startupPage);
}
void Workspace::switchToSingleMode() {
  switchWorkspaceMode(_ui->singlePage);
}
void Workspace::switchToSplitMode() {
  switchWorkspaceMode(_ui->splitPage);
}
void Workspace::switchToGridMode() {
  switchWorkspaceMode(_ui->gridPage);
}

void Workspace::switchWorkspaceMode(QWidget *page) {
  _ui->workspaceContents->setCurrentWidget(page);
  _ui->bottomFrame->setEnabled(page != _ui->startupPage);
  updatePanels();
}

void Workspace::updatePageCountLabel() {
  _ui->pagesLabel->setText(QString::number(_currentPanelIndex+1) + " / " + QString::number(_panels.size() - _panels.size()%currentSlotsCount()));
}

QWidget* Workspace::currentModeWidget() const {
  return _ui->workspaceContents->currentWidget();
}

QVector<PlaceHolderWidget*> Workspace::currentModeSlots() const {
  return _modeToSlots[currentModeWidget()];
}

unsigned int Workspace::currentSlotsCount() const {
  return currentModeSlots().size();
}

void Workspace::updateAvailableModes() {
  foreach(QWidget* page, _modeSwitches.keys()) {
    _modeSwitches[page]->setEnabled(_panels.size() >= _modeToSlots[page].size());
  }

  bool enableNavigation = currentModeWidget() != _ui->startupPage;
  _ui->exposeButton->setEnabled(enableNavigation);
  _ui->nextPageButton->setEnabled(enableNavigation);
  _ui->previousPageButton->setEnabled(enableNavigation);
}

void Workspace::updatePanels() {
  foreach(QWidget* mode,_modeToSlots.keys()) {
    if (mode == currentModeWidget())
      continue;
    QVector<PlaceHolderWidget*> panelSlots = _modeToSlots[mode];
    foreach(PlaceHolderWidget* panel, panelSlots) {
      panel->setWidget(NULL);
    }
  }

if (_currentPanelIndex<0)
  _currentPanelIndex=0;
if (_currentPanelIndex > _panels.size()-currentSlotsCount())
  _currentPanelIndex = _panels.size()-currentSlotsCount();

  updatePageCountLabel();

//   Fill up slots according to the current index until there is no panel to show
  int i = _currentPanelIndex;
  foreach (PlaceHolderWidget* slt, currentModeSlots()) {
    if (i >= _panels.size()) {
      slt->setWidget(NULL);
    }
    else if (slt->widget() != _panels[i]) {
      slt->setWidget(NULL);
    }
    i++;
  }

  i = _currentPanelIndex;
  foreach (PlaceHolderWidget* slt, currentModeSlots()) {
    if (i >= _panels.size())
      break;
    else if (slt->widget() != _panels[i]) {
      slt->setWidget(_panels[i]);
    }
    i++;
  }
}

void Workspace::nextPage() {
  _currentPanelIndex++;
  updatePanels();
}

void Workspace::previousPage() {
  _currentPanelIndex--;
  updatePanels();
}

void Workspace::setActivePanel(tlp::View* view) {
  int newIndex = panels().indexOf(view);
//  newIndex -= newIndex%currentSlotsCount();
  _currentPanelIndex = newIndex;
  updatePanels();
}

bool Workspace::eventFilter(QObject* obj, QEvent* ev) {
  if (ev->type() == QEvent::ChildAdded || ev->type() == 70) {
    QObject* childObj = static_cast<QChildEvent*>(ev)->child();
    childObj->installEventFilter(this);
    QGraphicsView* graphicsView = dynamic_cast<QGraphicsView*>(childObj);

    if (graphicsView != NULL && graphicsView->scene() != NULL)  {
      graphicsView->scene()->installEventFilter(this);
    }
  }
  else if (ev->type() == QEvent::ChildRemoved) {
    QObject* childObj = static_cast<QChildEvent*>(ev)->child();
    childObj->removeEventFilter(this);
    QGraphicsView* graphicsView = dynamic_cast<QGraphicsView*>(childObj);

    if (graphicsView != NULL && graphicsView->scene())  {
      graphicsView->scene()->removeEventFilter(this);
    }
  }
  else if (ev->type() == QEvent::FocusIn) {
    if (dynamic_cast<QGraphicsView*>(obj) != NULL) {
      tlp::WorkspacePanel* panel = static_cast<tlp::WorkspacePanel*>(obj->parent());
      emit panelFocused(panel->view());
    }
  }
  else if (ev->type() == QEvent::GraphicsSceneDragEnter || ev->type() == QEvent::GraphicsSceneDragMove) {
    if (static_cast<QGraphicsSceneDragDropEvent*>(ev)->mimeData()->hasColor())
      ev->accept();

    return true;
  }
  else if (ev->type() == QEvent::GraphicsSceneDrop) {
    addPanelFromDropAction(static_cast<QGraphicsSceneDragDropEvent*>(ev)->mimeData());
  }

  return QWidget::eventFilter(obj,ev);
}

bool Workspace::event(QEvent* e) {
  if (e->type() == QEvent::DragEnter) {
    if (static_cast<QDragEnterEvent*>(e)->mimeData()->hasColor())
      e->accept();
  }
  else if (e->type()==QEvent::Drop) {
    addPanelFromDropAction(static_cast<QDropEvent*>(e)->mimeData());
  }

  return QWidget::event(e);
}

void Workspace::addPanelFromDropAction(const QMimeData* mimeData) {
  QList<QVariant> colorData = mimeData->colorData().toList();
  foreach(QVariant v, colorData) {
    Graph* g = v.value<Graph*>();

    if (g == NULL)
      continue;

    emit addPanelRequest(g);
  }
}
