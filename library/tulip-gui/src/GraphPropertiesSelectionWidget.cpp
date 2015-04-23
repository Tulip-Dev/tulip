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

#include "tulip/GraphPropertiesSelectionWidget.h"
#include "tulip/Graph.h"

#include <algorithm>

using namespace std;

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

  if(graph)
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
  }
  else {
    selectProperty = true;
  }

  if (selectProperty && !includeViewProperties && propertyName.find("view")==0 && propertyName!="viewMetric") {
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
