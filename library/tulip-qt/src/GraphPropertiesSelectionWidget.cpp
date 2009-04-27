/*
 * GraphPropertiesSelectionWidget.cpp
 *
 *  Created on: 4 févr. 2009
 *      Author: antoine
 */

#include "tulip/GraphPropertiesSelectionWidget.h"

using namespace std;

const string viewPropertiesName[] = {"viewBorderColor", "viewBorderWidth", "viewColor", "viewLabel",
									 "viewLabelColor", "viewLabelPosition", "viewLayout", "viewMetaGraph",
									 "viewRotation", "viewSelection", "viewShape", "viewSize",
									 "viewTexture"};

const unsigned int nbViewProperties = sizeof(viewPropertiesName) / sizeof(string);

const vector<string> graphViewProperties(viewPropertiesName, viewPropertiesName + nbViewProperties);

namespace tlp {

GraphPropertiesSelectionWidget::GraphPropertiesSelectionWidget(QWidget *parent, const StringsListSelectionWidget::ListType &listType, const unsigned int maxNbSelectedProperties) :
	StringsListSelectionWidget(parent, listType, maxNbSelectedProperties) {}

GraphPropertiesSelectionWidget::GraphPropertiesSelectionWidget(Graph *graph, QWidget *parent, const StringsListSelectionWidget::ListType &listType,
															   const unsigned int maxNbSelectedProperties, const std::vector<std::string> &propertiesTypes,
															   const bool includeViewProperties)  :
	StringsListSelectionWidget(parent, listType, maxNbSelectedProperties), graph(graph), propertiesTypes(propertiesTypes), includeViewProperties(includeViewProperties) {
	initWidget();
}

void GraphPropertiesSelectionWidget::setWidgetParameters(Graph *graph, const std::vector<std::string> &propertiesTypes, const bool includeViewProperties) {
	this->graph = graph;
	this->propertiesTypes = propertiesTypes;
	this->includeViewProperties = includeViewProperties;
	initWidget();
}

void GraphPropertiesSelectionWidget::initWidget() {
	Iterator<string> * it = graph->getProperties();
	string propertyName;
	vector<string> inputProperties;

	setUnselectedStringsListLabel("Available properties");
	setSelectedStringsListLabel("Selected properties");

	while (it->hasNext()) {
		propertyName = it->next();
	    if (propertySelectable(propertyName)) {
	    	inputProperties.push_back(propertyName);
	    }
	}

	setUnselectedStringsList(inputProperties);
}

bool GraphPropertiesSelectionWidget::propertySelectable(const std::string &propertyName) {
	bool selectProperty = false;
	if (propertiesTypes.size() > 0) {
		string propertyType = graph->getProperty(propertyName)->getTypename();
		if (std::find(propertiesTypes.begin(), propertiesTypes.end(), propertyType) != propertiesTypes.end()) {
			selectProperty = true;
		}
	} else {
		selectProperty = true;
	}

	if (selectProperty && !includeViewProperties && std::find(graphViewProperties.begin(), graphViewProperties.end(), propertyName) != graphViewProperties.end()) {
		selectProperty = false;
	}
	return selectProperty;
}

void GraphPropertiesSelectionWidget::setInputPropertiesList(vector<string> &inputPropertiesList) {
	vector<string> inputPropertiesListFiltered;
	for (unsigned int i = 0 ; i < inputPropertiesList.size() ; ++i) {
		if (propertySelectable(inputPropertiesList[i])) {
			inputPropertiesListFiltered.push_back(inputPropertiesList[i]);
		}
	}
	setUnselectedStringsList(inputPropertiesListFiltered);
}

void GraphPropertiesSelectionWidget::setOutputPropertiesList(vector<string> &outputPropertiesList) {
	vector<string> outputPropertiesListFiltered;
	for (unsigned int i = 0 ; i < outputPropertiesList.size() ; ++i) {
		if (propertySelectable(outputPropertiesList[i])) {
			outputPropertiesListFiltered.push_back(outputPropertiesList[i]);
		}
	}
	setSelectedStringsList(outputPropertiesListFiltered);
}

vector<string> GraphPropertiesSelectionWidget::getSelectedProperties() const {
	return getSelectedStringsList();
}

void GraphPropertiesSelectionWidget::clearLists() {
	clearSelectedStringsList();
	clearUnselectedStringsList();
}

}
