/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/PythonInterpreter.h>

#include "PythonPanel.h"
#include "ui_PythonPanel.h"

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/TulipMimes.h>
#include <tulip/TulipMetaTypes.h>

static const QString setCurrentGraphFunction = "graph = None\n"
                                               "def setCurrentGraph(g):\n"
                                               "	global graph\n"
                                               "	graph = g\n";

PythonPanel::PythonPanel(QWidget *parent) : QWidget(parent), _ui(new Ui::PythonPanel) {
  _ui->setupUi(this);
  connect(_ui->graphCombo, SIGNAL(currentItemChanged()), this, SLOT(graphComboIndexChanged()));
  tlp::PythonInterpreter::getInstance()->runString(setCurrentGraphFunction);
  connect(_ui->pythonShellWidget, SIGNAL(beginCurrentLinesExecution()), this,
          SLOT(beginCurrentLinesExecution()));
  connect(_ui->pythonShellWidget, SIGNAL(endCurrentLinesExecution()), this,
          SLOT(endCurrentLinesExecution()));
}

PythonPanel::~PythonPanel() {
  delete _ui;
}

void PythonPanel::setModel(tlp::GraphHierarchiesModel *model) {
  _ui->graphCombo->setModel(model);
}

void PythonPanel::graphComboIndexChanged() {
  tlp::Graph *g = _ui->graphCombo->model()
                      ->data(_ui->graphCombo->selectedIndex(), tlp::TulipModel::GraphRole)
                      .value<tlp::Graph *>();
  tlp::PythonInterpreter::getInstance()->runGraphScript("__main__", "setCurrentGraph", g);
  _ui->pythonShellWidget->getAutoCompletionDb()->setGraph(g);
}

void PythonPanel::dragEnterEvent(QDragEnterEvent *dragEv) {
  const tlp::GraphMimeType *mimeType = dynamic_cast<const tlp::GraphMimeType *>(dragEv->mimeData());

  if (mimeType != NULL) {
    dragEv->accept();
  }
}

void PythonPanel::dropEvent(QDropEvent *dropEv) {
  const tlp::GraphMimeType *mimeType = dynamic_cast<const tlp::GraphMimeType *>(dropEv->mimeData());

  if (mimeType != NULL) {
    tlp::GraphHierarchiesModel *model =
        static_cast<tlp::GraphHierarchiesModel *>(_ui->graphCombo->model());
    QModelIndex graphIndex = model->indexOf(mimeType->graph());

    if (graphIndex == _ui->graphCombo->selectedIndex())
      return;

    _ui->graphCombo->selectIndex(graphIndex);
    dropEv->accept();
  }
}

void PythonPanel::beginCurrentLinesExecution() {
  tlp::Graph *g = _ui->graphCombo->model()
                      ->data(_ui->graphCombo->selectedIndex(), tlp::TulipModel::GraphRole)
                      .value<tlp::Graph *>();

  // undo/redo management
  if (g)
    g->push();
}

void PythonPanel::endCurrentLinesExecution() {
  tlp::Graph *g = _ui->graphCombo->model()
                      ->data(_ui->graphCombo->selectedIndex(), tlp::TulipModel::GraphRole)
                      .value<tlp::Graph *>();
  // undo/redo management
  g->popIfNoUpdates();
}
