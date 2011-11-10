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

#include <tulip/View.h>
#include <tulip/WorkspacePanel.h>

#include "ui_Workspace.h"

#include <QtCore/QDebug>

using namespace tlp;

Workspace::Workspace(QWidget *parent)
  : QWidget(parent), _ui(new Ui::Workspace), _currentPanelIndex(0) {
  _ui->setupUi(this);

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
  updatePageCountLabel();
}

Workspace::~Workspace() {
  delete _ui;
}

QList<tlp::View*> Workspace::views() const {
  QList<tlp::View*> result;
  foreach(WorkspacePanel* panel, _panels) {
    result.push_back(panel->view());
  }
  return result;
}

tlp::View* Workspace::addView(const QString& viewName,Graph* g, const DataSet& data) {
  // Create view and panel
  assert(ViewLister::pluginExists(viewName.toStdString()));
  View* view = ViewLister::getPluginObject(viewName.toStdString(),NULL);
  view->setupUi();
  WorkspacePanel* panel = new WorkspacePanel(view,viewName);
  connect(panel,SIGNAL(closeNeeded()),this,SLOT(panelClosed()));
  connect(view,SIGNAL(drawNeeded()),this,SLOT(viewNeedsDraw()));
  view->setGraph(g);
  view->setState(data);

  // Add it to the list
  _panels.push_back(panel);

  // If on startup mode, switch to single
  if (currentModeWidget() == _ui->startupPage) {
    switchToSingleMode();
  }

  // activate available modes
  updateAvailableModes();

  updatePageCountLabel();
  updatePanels();
  return view;
}


void Workspace::delView(tlp::View* view) {
  WorkspacePanel* panel = NULL;
  foreach(WorkspacePanel* it, _panels) {
    if (it->view() == view) {
      panel = it;
      break;
    }
  }

  if (panel != NULL)
    removePanel(panel);
}

void Workspace::removePanel(WorkspacePanel* panel) {
  panel->deleteLater();
  int removeCount = _panels.removeAll(panel);
  assert(removeCount>0);
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

  // Adjust current panel index if we were on the last panel
  else if (_currentPanelIndex == _panels.size()) {
    if (_panels.size()%currentSlotsCount() == 0) { // go back one page
      _currentPanelIndex -= currentSlotsCount();
    }
    else
      _currentPanelIndex--;

    if (_currentPanelIndex < 0)
      _currentPanelIndex = 0;
  }

  updatePageCountLabel();
  updatePanels();
}

void Workspace::viewNeedsDraw() {
}

void Workspace::panelClosed() {
  removePanel(static_cast<WorkspacePanel*>(sender()));
}

void Workspace::switchToStartupMode() {
  switchWorkspaceMode(_ui->startupPage);
}
void Workspace::switchToSingleMode() {
  switchWorkspaceMode(_ui->singlePage);
  updatePanels();
}
void Workspace::switchToSplitMode() {
  switchWorkspaceMode(_ui->splitPage);
  updatePanels();
}
void Workspace::switchToGridMode() {
  switchWorkspaceMode(_ui->gridPage);
  updatePanels();
}

void Workspace::switchWorkspaceMode(QWidget *page) {
  _ui->workspaceContents->setCurrentWidget(page);
  updatePageCountLabel();
  _ui->bottomFrame->setEnabled(page != _ui->startupPage);
  if (currentSlotsCount() == 0)
    _currentPanelIndex = 0;
  else
    _currentPanelIndex -= _currentPanelIndex%currentSlotsCount();
}

void Workspace::updatePageCountLabel() {
  unsigned int totalPages = ceil(_panels.size() * 1. /currentSlotsCount());
  unsigned int currentPage = ceil((_currentPanelIndex+1.)/currentSlotsCount());
  _ui->pagesLabel->setText(QString::number(currentPage) + " / " + QString::number(totalPages));
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
}

void Workspace::updatePanels() {
  foreach(QWidget* mode,_modeToSlots.keys()) {
    if (mode == currentModeWidget())
      continue;
    QVector<PlaceHolderWidget*> panelSlots = _modeToSlots[mode];
    foreach(PlaceHolderWidget* panel, panelSlots) {
      panel->takeWidget();
    }
  }

  // Fill up slots according to the current index until there is no panel to show
  int i = _currentPanelIndex;
  foreach (PlaceHolderWidget* slt, currentModeSlots()) {
    if (i>=_panels.size()) {
      slt->takeWidget();
    }

    else if (slt->widget() != _panels[i]) {
      slt->takeWidget();
    }
    i++;
  }

  i = _currentPanelIndex;
  foreach (PlaceHolderWidget* slt, currentModeSlots()) {
    if (i>=_panels.size())
      break;

    else if (slt->widget() != _panels[i]) {
      slt->setWidget(_panels[i]);
    }
    i++;
  }
}

void Workspace::nextPage() {
  int newIndex = _currentPanelIndex + currentSlotsCount();

  if (newIndex < _panels.size()) {
    _currentPanelIndex = newIndex;
    updatePanels();
    updatePageCountLabel();
  }
}

void Workspace::previousPage() {
  int newIndex = _currentPanelIndex - currentSlotsCount();

  if (newIndex >= 0) {
    _currentPanelIndex = newIndex;
    updatePanels();
    updatePageCountLabel();
  }
}
