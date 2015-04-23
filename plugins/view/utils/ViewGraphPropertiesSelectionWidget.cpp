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

#include <algorithm>

#include "ViewGraphPropertiesSelectionWidget.h"
#include "ui_ViewGraphPropertiesSelectionWidget.h"

using namespace std;

namespace tlp {

ViewGraphPropertiesSelectionWidget::ViewGraphPropertiesSelectionWidget(QWidget *parent) : QWidget(parent),_ui(new Ui::ViewGraphPropertiesSelectionWidgetData),graph(NULL) {
  _ui->setupUi(this);
}

ViewGraphPropertiesSelectionWidget::~ViewGraphPropertiesSelectionWidget() {
  delete _ui;
}

void ViewGraphPropertiesSelectionWidget::enableEdgesButton(const bool enable) {
  _ui->edgesButton->setEnabled(enable);
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

  _ui->graphPropertiesSelectionWidget->clearLists();
  _ui->graphPropertiesSelectionWidget->setWidgetParameters(graph, graphPropertiesTypesFilter);

  if (!lastSelectedProperties.empty() && graph) {
    vector<string> stringList;


    for (vector<string>::const_iterator it = lastSelectedProperties.begin() ; it != lastSelectedProperties.end() ; ++it) {
      string prop(*it);

      if (graph->existProperty(prop)) {
        stringList.push_back(prop);
      }
    }

    _ui->graphPropertiesSelectionWidget->setOutputPropertiesList(stringList);

    stringList.clear();
    string propertyName;
    forEach(propertyName, graph->getProperties()) {
      if (graph->existProperty(propertyName) && std::find(lastSelectedProperties.begin(), lastSelectedProperties.end(), propertyName) == lastSelectedProperties.end()) {
        stringList.push_back(propertyName);
      }
    }
    _ui->graphPropertiesSelectionWidget->setInputPropertiesList(stringList);
  }
  else {
    this->lastSelectedProperties.clear();
  }
}

vector<string> ViewGraphPropertiesSelectionWidget::getSelectedGraphProperties() {
  return _ui->graphPropertiesSelectionWidget->getSelectedProperties();
}

void ViewGraphPropertiesSelectionWidget::setWidgetEnabled(const bool enabled) {
  _ui->groupBox->setEnabled(enabled);
  _ui->groupBox2->setEnabled(enabled);
}

void ViewGraphPropertiesSelectionWidget::setSelectedProperties(vector<string> selectedProperties) {
  if(!graph)
    return;

  Iterator<string> *properties = graph->getProperties();
  vector<string> stringList;
  vector<string> finalStringList;
  string propertyName;
  _ui->graphPropertiesSelectionWidget->clearLists();

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
  _ui->graphPropertiesSelectionWidget->setInputPropertiesList(finalStringList);
  _ui->graphPropertiesSelectionWidget->setOutputPropertiesList(selectedProperties);
}

ElementType ViewGraphPropertiesSelectionWidget::getDataLocation() const {
  if (_ui->nodesButton->isChecked()) {
    return NODE;
  }
  else {
    return EDGE;
  }
}

void ViewGraphPropertiesSelectionWidget::setDataLocation(const ElementType location) {
  if (location == NODE) {
    _ui->edgesButton->setChecked(false);
    _ui-> nodesButton->setChecked(true);
  }
  else {
    _ui->edgesButton->setChecked(true);
    _ui->nodesButton->setChecked(false);
  }

  lastDataLocation = location;
}

void ViewGraphPropertiesSelectionWidget::treatEvent(const Event &evt) {
  const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

  if (graphEvent) {
    switch(graphEvent->getType()) {

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
    case GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY:
    case GraphEvent::TLP_AFTER_RENAME_LOCAL_PROPERTY: {
      setWidgetParameters(graph,graphPropertiesTypesFilter);
      break;
    }

    default:
      break;
    }
  }
}

bool ViewGraphPropertiesSelectionWidget::configurationChanged() {
  ElementType dataLocation = getDataLocation();

  if (lastDataLocation != dataLocation) {
    lastDataLocation = dataLocation;
    return true;
  }

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
    lastSelectedProperties=selectedProperties;
    return true;
  }

  return false;
}

}
