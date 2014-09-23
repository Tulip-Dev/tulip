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

#include "NeighborhoodHighlighterConfigWidget.h"
#include "ui_NeighborhoodHighlighterConfigWidget.h"

#include <tulip/TlpQtTools.h>

using namespace tlp;

NeighborhoodHighlighterConfigWidget::NeighborhoodHighlighterConfigWidget(QWidget *parent) : QWidget(parent),_ui(new Ui::NeighborhoodHighlighterConfigWidgetData) {
  _ui->setupUi(this);
  connect(_ui->applyButton, SIGNAL(clicked()), this, SIGNAL(updateNeighborhoodGraph()));
}

NeighborhoodHighlighterConfigWidget::~NeighborhoodHighlighterConfigWidget() {
  delete _ui;
}

NodeNeighborhoodView::NeighborNodesType NeighborhoodHighlighterConfigWidget::getNeighborsType() const {
  if (_ui->outputEdgesRB->isChecked()) {
    return NodeNeighborhoodView::OUT_NEIGHBORS;
  }
  else if (_ui->inputEdgesRB->isChecked()) {
    return NodeNeighborhoodView::IN_NEIGHBORS;
  }
  else {
    return NodeNeighborhoodView::IN_OUT_NEIGHBORS;
  }
}

bool NeighborhoodHighlighterConfigWidget::computeReachableSubGraph() const {
  return _ui->computeReachableSGCB->isChecked();
}

void NeighborhoodHighlighterConfigWidget::setCurrentMaxDistanceForReachableNodes(unsigned int distance) {
  _ui->distanceLineEdit->setText(QString::number(distance));
}

bool NeighborhoodHighlighterConfigWidget::bringAndGoAnimation1() const {
  return _ui->bagAnimation1->isChecked();
}

int NeighborhoodHighlighterConfigWidget::numberOfNodesToBring() const {
  return _ui->nodesNumber->value();
}

std::string NeighborhoodHighlighterConfigWidget::propertyToUse() const {
  return tlp::QStringToTlpString(_ui->propertyName->currentText());
}

void NeighborhoodHighlighterConfigWidget::setNumberOfNodes(int nodesNb) {
  _ui->nodesNumber->setValue(nodesNb);
}

void NeighborhoodHighlighterConfigWidget::setPropertyToUse(std::string &propertyName) {
  _ui->propertyName->addItem(QString::fromUtf8(propertyName.c_str()));
  _ui->propertyName->setCurrentIndex(_ui->propertyName->findText(QString::fromUtf8(propertyName.c_str())));
}

bool NeighborhoodHighlighterConfigWidget::isdisplayEdgesCBChecked() const {
  return _ui->displayEdgesCB->isChecked();
}
