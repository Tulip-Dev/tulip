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

#include "MatrixViewConfigurationWidget.h"

#include <tulip/Graph.h>
#include <tulip/ForEach.h>

using namespace tlp;
using namespace std;

MatrixViewConfigurationWidget::MatrixViewConfigurationWidget(QWidget *parent): QWidget(parent), _modifyingMetricList(false) {
  setupUi(this);
  connect(orderingMetricCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(orderingMetricComboIndexChanged(int)));
}

void MatrixViewConfigurationWidget::setGraph(tlp::Graph *g) {
  QString firstString = orderingMetricCombo->itemText(0);
  QString currentString = orderingMetricCombo->currentText();
  _modifyingMetricList = true;
  orderingMetricCombo->clear();
  orderingMetricCombo->addItem(firstString);
  int currentIndex = 0;
  int i=0;
  string s;
  forEach (s, g->getProperties()) {
    string type = g->getProperty(s)->getTypename();

    if (type != "double" && type != "int")
      continue;

    orderingMetricCombo->addItem(s.c_str());

    if (currentString.toStdString().compare(s) == 0)
      currentIndex = i;

    ++i;
  }
  _modifyingMetricList = false;
  orderingMetricCombo->setCurrentIndex(currentIndex);
}

void MatrixViewConfigurationWidget::orderingMetricComboIndexChanged(int i) {
  if (_modifyingMetricList)
    return;

  string name = "";

  if (i > 0)
    name = orderingMetricCombo->itemText(i).toStdString();

  emit metricSelected(name);
}

GridDisplayMode MatrixViewConfigurationWidget::gridDisplayMode() const {
  return (GridDisplayMode)gridDisplayCombo->currentIndex();
}
