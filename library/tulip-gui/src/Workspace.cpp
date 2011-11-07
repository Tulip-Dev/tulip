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

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

#include <tulip/View.h>
#include <tulip/WorkspacePanel.h>

#include "ui_Workspace.h"

#include <QtCore/QDebug>

using namespace tlp;

Workspace::Workspace(QWidget *parent)
  : QWidget(parent), _ui(new Ui::Workspace) {
  _ui->setupUi(this);
}

Workspace::~Workspace() {
  delete _ui;
}

tlp::View* Workspace::addView(const QString& viewName,Graph* g, const DataSet& data) {
  assert(ViewLister::pluginExists(viewName.toStdString()));
  View* view = ViewLister::getPluginObject(viewName.toStdString(),NULL);

  WorkspacePanel* panel = new WorkspacePanel(view,viewName);
  int viewCounter = 0;
  foreach(WorkspacePanel* it, _panels) {
    if (it->viewName() == viewName)
      viewCounter++;
  }
  panel->setWindowTitle(viewName + (viewCounter>0 ? " <" + QString::number(viewCounter) + ">" : ""));
  _panels.push_back(panel);

  connect(panel->view(),SIGNAL(drawNeeded()),this,SLOT(viewNeedsDraw()));
  connect(panel,SIGNAL(closeNeeded()),this,SLOT(panelClosed()));
  view->setupUi();
  view->setGraph(g);
  view->setState(data);

  return panel->view();
}

QList<tlp::View*> Workspace::views() const {
  QList<tlp::View*> result;
  foreach(WorkspacePanel* panel, _panels) {
    result.push_back(panel->view());
  }
  return result;
}

void Workspace::delView(tlp::View* view) {
  WorkspacePanel* removedPanel = NULL;

  foreach(WorkspacePanel* panel,_panels) {
    if (panel->view() == view) {
      removedPanel = panel;
      break;
    }
  }

  assert(removedPanel != NULL);


}

void Workspace::switchToFullPage() {

}

void Workspace::switchToSplitPage()  {

}

void Workspace::switchToGridPage()  {

}

void Workspace::switchToExposePage() {

}

void Workspace::showViewSwitcher() {

}

void Workspace::nextPage() {

}

void Workspace::previousPage() {

}

void Workspace::viewNeedsDraw() {

}

void Workspace::panelClosed() {

}
