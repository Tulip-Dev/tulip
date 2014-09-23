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

#include "MatrixViewConfigurationWidget.h"
#include "ui_MatrixViewConfigurationWidget.h"

#include <tulip/Graph.h>
#include <tulip/ForEach.h>
#include <tulip/Perspective.h>
#include <tulip/TlpQtTools.h>

#include <QMainWindow>

using namespace tlp;
using namespace std;

MatrixViewConfigurationWidget::MatrixViewConfigurationWidget(QWidget *parent): QWidget(parent), _ui(new Ui::MatrixViewConfigurationWidget()), _modifyingMetricList(false) {
  _ui->setupUi(this);
  connect(_ui->orderingMetricCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(orderingMetricComboIndexChanged(int)));
  connect(_ui->backgroundColorBtn, SIGNAL(colorChanged(QColor)), this, SIGNAL(changeBackgroundColor(QColor)));
  connect(_ui->gridDisplayCombo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(setGridDisplayMode()));
  connect(_ui->showedgesbox, SIGNAL(clicked(bool)), this, SIGNAL(showEdges(bool)));

  if (Perspective::instance())
    _ui->backgroundColorBtn->setDialogParent(Perspective::instance()->mainWindow());
}

MatrixViewConfigurationWidget::~MatrixViewConfigurationWidget() {
  delete _ui;
}

void MatrixViewConfigurationWidget::setBackgroundColor(const QColor &c) {
  _ui->backgroundColorBtn->setColor(c);
}

void MatrixViewConfigurationWidget::setDisplayEdges(const bool state) {
  _ui->showedgesbox->setChecked(state);
}

void MatrixViewConfigurationWidget::setAscendingOrder(const bool state) {
  _ui->ascendingOrderCBox->setChecked(state);
}

bool MatrixViewConfigurationWidget::ascendingOrder() const {
  return _ui->ascendingOrderCBox->isChecked();
}

void MatrixViewConfigurationWidget::setGraph(tlp::Graph *g) {
  QString firstString = _ui->orderingMetricCombo->itemText(0);
  QString currentString = _ui->orderingMetricCombo->currentText();
  _modifyingMetricList = true;
  _ui->orderingMetricCombo->clear();
  _ui->orderingMetricCombo->addItem(firstString);
  int currentIndex = 0;
  int i=0;
  string s;
  forEach (s, g->getProperties()) {
    string type = g->getProperty(s)->getTypename();

    if (type != "double" && type != "int" && type != "string")
      continue;

    _ui->orderingMetricCombo->addItem(tlpStringToQString(s));

    if (QStringToTlpString(currentString).compare(s) == 0)
      currentIndex = i;

    ++i;
  }
  _modifyingMetricList = false;
  _ui->orderingMetricCombo->setCurrentIndex(currentIndex);
}

void MatrixViewConfigurationWidget::orderingMetricComboIndexChanged(int i) {
  if (_modifyingMetricList)
    return;

  string name("");

  if (i > 0)
    name = QStringToTlpString(_ui->orderingMetricCombo->itemText(i));

  emit metricSelected(name);
}

GridDisplayMode MatrixViewConfigurationWidget::gridDisplayMode() const {
  return (GridDisplayMode)_ui->gridDisplayCombo->currentIndex();
}

int MatrixViewConfigurationWidget::orderingProperty() const {
  return _ui->orderingMetricCombo->currentIndex();
}

void MatrixViewConfigurationWidget::setOrderingProperty(int index) {
  _ui->orderingMetricCombo->setCurrentIndex(index);
}

void MatrixViewConfigurationWidget::setgridmode(int index) {
  _ui->gridDisplayCombo->setCurrentIndex(index);
}
