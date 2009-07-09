/*
	  ParallelCoordsDataConfigWidget.cpp

   Created on: 6 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include "ParallelCoordsDataConfigWidget.h"

using namespace std;

namespace tlp {

ParallelCoordsDataConfigWidget::ParallelCoordsDataConfigWidget(QWidget *parent) : QWidget(parent) {
	setupUi(this);
	propertyTypesFilter.push_back("double");
	propertyTypesFilter.push_back("int");
	propertyTypesFilter.push_back("string");
}

ParallelCoordsDataConfigWidget::ParallelCoordsDataConfigWidget(ParallelCoordinatesGraphProxy *graphProxy, QWidget *parent) : QWidget(parent), graphProxy(graphProxy) {
	setupUi(this);
	propertyTypesFilter.push_back("double");
	propertyTypesFilter.push_back("int");
	propertyTypesFilter.push_back("string");
	graphPropertiesSelectionWidget->setWidgetParameters(graphProxy, propertyTypesFilter);
}

void ParallelCoordsDataConfigWidget::setGraphProxy(ParallelCoordinatesGraphProxy *graphProx) {
	graphProxy = graphProx;
	graphPropertiesSelectionWidget->setWidgetParameters(graphProxy, propertyTypesFilter);
}

const ElementType ParallelCoordsDataConfigWidget::getDataLocation() const {
	if (nodesButton->isChecked()) {
		return NODE;
	} else {
		return EDGE;
	}
}

void ParallelCoordsDataConfigWidget::setDataLocation(const ElementType location) {
	if (location == NODE) {
		edgesButton->setChecked(false);
		nodesButton->setChecked(true);
	} else {
		edgesButton->setChecked(true);
		nodesButton->setChecked(false);
	}
}

vector<string> ParallelCoordsDataConfigWidget::getSelectedProperties() const {
	return graphPropertiesSelectionWidget->getSelectedProperties();
}

void ParallelCoordsDataConfigWidget::updateSelectedProperties() {
	Iterator<string> *properties = graphProxy->getProperties();
	vector<string> selectedProperties = graphProxy->getSelectedProperties();
	vector<string> stringList;
	vector<string>::iterator it;
	string propertyName;
	graphPropertiesSelectionWidget->clearLists();

	for (it = selectedProperties.begin() ; it != selectedProperties.end() ; ++it) {
		stringList.push_back(*it);
	}
	graphPropertiesSelectionWidget->setOutputPropertiesList(stringList);

	stringList.clear();

	while (properties->hasNext()) {
		propertyName = properties->next();
		if (std::find(selectedProperties.begin(), selectedProperties.end(), propertyName) == selectedProperties.end()) {
			stringList.push_back(propertyName);
		}
	}
	delete properties;
	graphPropertiesSelectionWidget->setInputPropertiesList(stringList);
}

}
