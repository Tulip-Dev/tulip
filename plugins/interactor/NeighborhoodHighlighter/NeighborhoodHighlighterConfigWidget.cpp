/*
 * NeighbourhoodHighlighterConfigWidget.cpp
 *
 *  Created on: 2 sept. 2009
 *      Author: antoine
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
