/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "SpreadConfigurationWidget.h"
#include <tulip/Graph.h>
#include <tulip/ForEach.h>

using namespace tlp;
using namespace std;
SpreadConfigurationWidget::SpreadConfigurationWidget(QWidget* parent) :
	QWidget(parent) {
	setupUi(this);
	connect(applyPushButton, SIGNAL(clicked()), this, SIGNAL(updated()));

}

SpreadConfigurationWidget::~SpreadConfigurationWidget() {
}

void SpreadConfigurationWidget::setGraph(tlp::Graph *graph) {

	graphPropertiesSelectionWidget->clearLists();
	graphPropertiesSelectionWidget->setWidgetParameters(graph, std::vector<std::string>(), true);
	vector<string> properties;
	string propertyName;
	forEach(propertyName,graph->getProperties()) {
		properties.push_back(propertyName);
	}

	graphPropertiesSelectionWidget->setOutputPropertiesList(properties);
}

std::vector<std::string> SpreadConfigurationWidget::getSelectedProperties() {
	return graphPropertiesSelectionWidget->getSelectedProperties();
}

void SpreadConfigurationWidget::setSelectedProperties(const vector<string>& properties) {
	vector<string> copy = graphPropertiesSelectionWidget->getCompleteStringsList();
	graphPropertiesSelectionWidget->setInputPropertiesList(copy);
	copy = properties;
	graphPropertiesSelectionWidget->setOutputPropertiesList(copy);
}

bool SpreadConfigurationWidget::allPropertiesSelected() {
	return graphPropertiesSelectionWidget->getSelectedProperties().size()
			== graphPropertiesSelectionWidget->getCompleteStringsList().size();
}
