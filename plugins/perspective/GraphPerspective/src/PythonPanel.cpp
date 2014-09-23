/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
#include "OutputPanelButton.h"

#include <tulip/GraphHierarchiesModel.h>
#include <tulip/TulipMimes.h>
#include <tulip/TulipMetaTypes.h>

#include <QPropertyAnimation>

static const QString setCurrentGraphFunction =
  "graph = None\n"
  "def setCurrentGraph(g):\n"
  "	global graph\n"
  "	graph = g\n"
  ;

PythonPanel::PythonPanel(QWidget *parent) : QWidget(parent), _ui(new Ui::PythonPanel), _button(NULL), _animation(NULL) {
  _ui->setupUi(this);
  connect(_ui->clearButton, SIGNAL(clicked()), this, SLOT(clearConsole()));
  connect(_ui->consoleWidget, SIGNAL(textChanged()), this, SLOT(newOutputInConsole()));
  connect(_ui->graphCombo,SIGNAL(currentItemChanged()),this,SLOT(graphComboIndexChanged()));
  tlp::PythonInterpreter::getInstance()->runString(setCurrentGraphFunction);
}

PythonPanel::~PythonPanel() {
  delete _ui;
}

QPlainTextEdit *PythonPanel::consoleWidget() {
  return _ui->consoleWidget;
}

void PythonPanel::setModel(tlp::GraphHierarchiesModel* model) {
  _ui->graphCombo->setModel(model);
}

void PythonPanel::graphComboIndexChanged() {
  tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),tlp::TulipModel::GraphRole).value<tlp::Graph*>();
  tlp::PythonInterpreter::getInstance()->runGraphScript("__main__", "setCurrentGraph", g);
  _ui->pythonShellWidget->getAutoCompletionDb()->setGraph(g);
}

void PythonPanel::clearConsole() {
  _ui->consoleWidget->clear();
}

void PythonPanel::setPanelButton(OutputPanelButton *button) {
  _button = button;
  connect(_button, SIGNAL(toggled(bool)), this, SLOT(stopButtonAnimation()));
}

void PythonPanel::newOutputInConsole() {
  _ui->tabWidget->setCurrentIndex(1);

  if (!_button->isChecked()) {
    startButtonAnimation();
  }
}

void PythonPanel::startButtonAnimation() {
  _animation = new QPropertyAnimation(_button, "glowColor");
  _animation->setKeyValueAt(0, QColor(255,255,255,0));
  _animation->setKeyValueAt(0.25, QColor(255,0,0));
  _animation->setKeyValueAt(0.75, QColor(255,0,0));
  _animation->setKeyValueAt(1, QColor(255,255,255,0));
  _animation->setDuration(1000);
  _animation->setLoopCount(-1);
  _animation->start();
}

void PythonPanel::stopButtonAnimation() {
  if (_animation) {
    _animation->stop();
    delete _animation;
    _animation = NULL;
    _button->resetBackgroundColor();
  }
}

void PythonPanel::dragEnterEvent(QDragEnterEvent *dragEv) {
  const tlp::GraphMimeType* mimeType = dynamic_cast<const tlp::GraphMimeType*>(dragEv->mimeData());

  if (mimeType != NULL) {
    dragEv->accept();
  }
}

void PythonPanel::dropEvent(QDropEvent* dropEv) {
  const tlp::GraphMimeType* mimeType = dynamic_cast<const tlp::GraphMimeType*>(dropEv->mimeData());

  if (mimeType != NULL) {
    tlp::GraphHierarchiesModel* model = static_cast<tlp::GraphHierarchiesModel*>(_ui->graphCombo->model());
    QModelIndex graphIndex = model->indexOf(mimeType->graph());

    if (graphIndex == _ui->graphCombo->selectedIndex())
      return;

    _ui->graphCombo->selectIndex(graphIndex);
    dropEv->accept();
  }
}
