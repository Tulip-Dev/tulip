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

#include "NeighborhoodHighlighterConfigWidget.h"

NeighborhoodHighlighterConfigWidget::NeighborhoodHighlighterConfigWidget(QWidget *parent) : QWidget(parent) {
	setupUi(this);
}

NodeNeighborhoodView::NeighborNodesType NeighborhoodHighlighterConfigWidget::getNeighborsType() const {
	if (outputEdgesRB->isChecked()) {
		return NodeNeighborhoodView::OUT_NEIGHBORS;
	} else if (inputEdgesRB->isChecked()) {
		return NodeNeighborhoodView::IN_NEIGHBORS;
	} else {
		return NodeNeighborhoodView::IN_OUT_NEIGHBORS;
	}
}

bool NeighborhoodHighlighterConfigWidget::computeReachableSubGraph() const {
	return computeReachableSGCB->isChecked();
}

void NeighborhoodHighlighterConfigWidget::setCurrentMaxDistanceForReachableNodes(unsigned int distance) {
	distanceLineEdit->setText(QString::number(distance));
}

bool NeighborhoodHighlighterConfigWidget::bringAndGoAnimation1() const {
	return bagAnimation1->isChecked();
}

int NeighborhoodHighlighterConfigWidget::numberOfNodesToBring() const {
  return nodesNumber->value();
}

std::string NeighborhoodHighlighterConfigWidget::propertyToUse() const {
  return propertyName->currentText().toStdString();
}

void NeighborhoodHighlighterConfigWidget::setNumberOfNodes(int nodesNb) {
  nodesNumber->setValue(nodesNb);
}

void NeighborhoodHighlighterConfigWidget::setPropertyToUse(std::string propertyName) {
  this->propertyName->addItem(QString::fromStdString(propertyName));
  this->propertyName->setCurrentIndex(this->propertyName->findText(QString::fromStdString(propertyName)));
}
