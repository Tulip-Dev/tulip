/*
 * SpreadConfigurationWidget.cpp
 *
 *  Created on: 1 juil. 2009
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
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
