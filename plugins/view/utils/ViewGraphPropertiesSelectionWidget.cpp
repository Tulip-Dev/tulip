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

#include <algorithm>
#include "ViewGraphPropertiesSelectionWidget.h"

using namespace std;

namespace tlp {

ViewGraphPropertiesSelectionWidget::ViewGraphPropertiesSelectionWidget(QWidget *parent) : QWidget(parent),graph(NULL) {
  setupUi(this);
  connect(applyButton,SIGNAL(clicked()),this,SLOT(applySettings()));
}

void ViewGraphPropertiesSelectionWidget::setWidgetParameters(Graph *graph, vector<string> graphPropertiesTypesFilter) {

  bool notSameGraph=true;

  if(graph==this->graph)
    notSameGraph=false;

  if(notSameGraph && this->graph!=NULL && graph!=NULL)
    this->graph->removeListener(this);

  this->graph = graph;

  if(graph!=NULL && notSameGraph)
    graph->addListener(this);

  this->graphPropertiesTypesFilter = graphPropertiesTypesFilter;

  vector<string> lastSelectedProperties = getSelectedGraphProperties();

  graphPropertiesSelectionWidget->clearLists();
  graphPropertiesSelectionWidget->setWidgetParameters(graph, graphPropertiesTypesFilter);

  if (lastSelectedProperties.size() > 0 && graph) {
    Iterator<string> *properties= graph->getProperties();
    vector<string> stringList;
    vector<string>::iterator it;
    string propertyName;

    for (it = lastSelectedProperties.begin() ; it != lastSelectedProperties.end() ; ++it) {
      if (graph->existProperty(*it)) {
        stringList.push_back(*it);
      }
    }

    graphPropertiesSelectionWidget->setOutputPropertiesList(stringList);

    stringList.clear();

    while (properties->hasNext()) {
      propertyName = properties->next();

      if (graph->existProperty(propertyName) && std::find(lastSelectedProperties.begin(), lastSelectedProperties.end(), propertyName) == lastSelectedProperties.end()) {
        stringList.push_back(propertyName);
      }
    }

    delete properties;
    graphPropertiesSelectionWidget->setInputPropertiesList(stringList);
  }
  else {
    this->lastSelectedProperties.clear();
  }
}

vector<string> ViewGraphPropertiesSelectionWidget::getSelectedGraphProperties() {
  return graphPropertiesSelectionWidget->getSelectedProperties();
}

void ViewGraphPropertiesSelectionWidget::setWidgetEnabled(const bool enabled) {
  groupBox->setEnabled(enabled);
  groupBox2->setEnabled(enabled);
  applyButton->setEnabled(enabled);

}

void ViewGraphPropertiesSelectionWidget::setSelectedProperties(vector<string> selectedProperties) {
  if(!graph)
    return;

  Iterator<string> *properties = graph->getProperties();
  vector<string> stringList;
  vector<string> finalStringList;
  string propertyName;
  graphPropertiesSelectionWidget->clearLists();

  while (properties->hasNext()) {
    propertyName = properties->next();
    stringList.push_back(propertyName);
  }

  vector<string> selectedPropertiesCopy(selectedProperties);

  for (unsigned int i = 0 ; i < stringList.size() ; ++i) {
    if (std::find(selectedProperties.begin(), selectedProperties.end(), stringList[i]) != selectedProperties.end()) {
      finalStringList.push_back(selectedPropertiesCopy.front());
      selectedPropertiesCopy.erase(remove(selectedPropertiesCopy.begin(), selectedPropertiesCopy.end(), selectedPropertiesCopy.front()), selectedPropertiesCopy.end());
    }
    else {
      finalStringList.push_back(stringList[i]);
    }
  }

  delete properties;
  graphPropertiesSelectionWidget->setInputPropertiesList(finalStringList);
  graphPropertiesSelectionWidget->setOutputPropertiesList(selectedProperties);
}

ElementType ViewGraphPropertiesSelectionWidget::getDataLocation() const {
  if (nodesButton->isChecked()) {
    return NODE;
  }
  else {
    return EDGE;
  }
}

void ViewGraphPropertiesSelectionWidget::setDataLocation(const ElementType location) {
  if (location == NODE) {
    edgesButton->setChecked(false);
    nodesButton->setChecked(true);
  }
  else {
    edgesButton->setChecked(true);
    nodesButton->setChecked(false);
  }
}

void ViewGraphPropertiesSelectionWidget::treatEvent(const Event &evt) {
  const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

  if (graphEvent) {
    switch(graphEvent->getType()) {

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
    case GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY: {
      setWidgetParameters(graph,graphPropertiesTypesFilter);
      break;
    }

    default:
      break;
    }
  }
}

bool ViewGraphPropertiesSelectionWidget::configurationChanged() {
  vector<string> selectedProperties=getSelectedGraphProperties();

  if(selectedProperties.size()!=lastSelectedProperties.size()) {
    lastSelectedProperties=selectedProperties;
    return true;
  }

  bool sameSelectedProperties=true;
  vector<string>::iterator itLast=lastSelectedProperties.begin();

  for(vector<string>::iterator it=selectedProperties.begin(); it!=selectedProperties.end(); ++it) {
    if((*it)!=(*itLast)) {
      sameSelectedProperties=false;
      break;
    }

    ++itLast;
  }

  if(!sameSelectedProperties) {
    return true;
    lastSelectedProperties=selectedProperties;
  }

  return false;
}

void ViewGraphPropertiesSelectionWidget::applySettings() {
  emit(applySettingsSignal());
}

}
