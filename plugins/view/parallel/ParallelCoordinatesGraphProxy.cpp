//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/ForEach.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>


#include "ParallelCoordinatesGraphProxy.h"

using namespace std;

namespace tlp {

ParallelCoordinatesGraphProxy::ParallelCoordinatesGraphProxy(Graph *g, const ElementType location):
	GraphDecorator(g), dataLocation(location), selectedGraphInHierarchy(graph_component) {
  fillPropertiesVector();
  Observable::holdObservers();
  originalDataColors = new ColorProperty(graph_component);
  *originalDataColors = *(graph_component->getProperty<ColorProperty>("viewColor"));
  Observable::unholdObservers();
}

ParallelCoordinatesGraphProxy::~ParallelCoordinatesGraphProxy() {
	Observable::holdObservers();
	*(graph_component->getProperty<ColorProperty>("viewColor")) = *originalDataColors;
	delete originalDataColors;
	originalDataColors = NULL;
	Observable::unholdObservers();
}
void ParallelCoordinatesGraphProxy::fillPropertiesVector() {
  Iterator<string> * it = getProperties();
  string propertyName, propertyType;

  vector<string> graphViewProperties(viewPropertiesName, viewPropertiesName + NB_VIEW_PROPERTIES);

  propertiesList.clear();
  while (it->hasNext()) {
    propertyName = it->next();
    propertyType = getProperty(propertyName)->getTypename();

    // don't select graph view properties
    if (std::find(graphViewProperties.begin(), graphViewProperties.end(), propertyName) == graphViewProperties.end()) {
    	// select properties with compatible datatypes
    	if (propertyType == "string" || propertyType == "int" || propertyType == "double") {
    		propertiesList.push_back(propertyName);
    	}
    }
  }
}

unsigned int ParallelCoordinatesGraphProxy::getNumberOfSelectedProperties() const {
	return selectedProperties.size();
}

const vector<string> &ParallelCoordinatesGraphProxy::getSelectedProperties() const {
  return selectedProperties;
}

void ParallelCoordinatesGraphProxy::setSelectedProperties(const vector<string>& properties) {
  selectedProperties = properties;
}

void ParallelCoordinatesGraphProxy::removePropertyFromSelection(const std::string &propertyName) {
	vector<string> selectedPropertiesCopy(selectedProperties);
	vector<string>::iterator it;
	selectedProperties.clear();
	for (it = selectedPropertiesCopy.begin() ; it != selectedPropertiesCopy.end() ; ++it) {
		if (*it != propertyName) {
			selectedProperties.push_back(*it);
		}
	}
}

ElementType ParallelCoordinatesGraphProxy::getDataLocation() const {
  return dataLocation;
}

unsigned int ParallelCoordinatesGraphProxy::getDataCount() const {
  if (getDataLocation() == NODE) {
    return selectedGraphInHierarchy->numberOfNodes();
  } else {
    return selectedGraphInHierarchy->numberOfEdges();
  }
}

Color ParallelCoordinatesGraphProxy::getDataColor(const unsigned int dataId) {
	return getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId);
}

string ParallelCoordinatesGraphProxy::getDataTexture(const unsigned int dataId) {
	return getPropertyValueForData<StringProperty, StringType>("viewTexture", dataId);
}

vector<string> ParallelCoordinatesGraphProxy::getAllProperties() {
  fillPropertiesVector();
  return propertiesList;
}

bool ParallelCoordinatesGraphProxy::isDataSelected(const unsigned int dataId) {
	return getPropertyValueForData<BooleanProperty, BooleanType>("viewSelection", dataId);
}

void ParallelCoordinatesGraphProxy::setDataSelected(const unsigned int dataId, const bool dataSelected) {
	setPropertyValueForData<BooleanProperty, BooleanType>("viewSelection", dataId, dataSelected);
}

Size ParallelCoordinatesGraphProxy::getDataViewSize(const unsigned int dataId) {
	return getPropertyValueForData<SizeProperty, SizeType>("viewSize", dataId);
}

string ParallelCoordinatesGraphProxy::getDataLabel(const unsigned int dataId) {
	return getPropertyValueForData<StringProperty, StringType>("viewLabel", dataId);
}

void ParallelCoordinatesGraphProxy::resetSelection() {
	setPropertyValueForAllData<BooleanProperty, BooleanType>("viewSelection", false);
}

void ParallelCoordinatesGraphProxy::deleteData(const unsigned int dataId) {
	if (getDataLocation() == NODE) {
		delNode(node(dataId));
	} else {
		delEdge(edge(dataId));
	}
}

Iterator<unsigned int> *ParallelCoordinatesGraphProxy::getDataIterator(const bool allData) {
	if (getDataLocation() == NODE) {
		if (allData) {
			return new ParallelCoordinatesDataIterator<node>(graph_component->getNodes());
		} else {
			return new ParallelCoordinatesDataIterator<node>(selectedGraphInHierarchy->getNodes());
		}
	} else {
		if (allData) {
			return new ParallelCoordinatesDataIterator<edge>(graph_component->getEdges());
		} else {
			return new ParallelCoordinatesDataIterator<edge>(selectedGraphInHierarchy->getEdges());
		}
	}
}

string ParallelCoordinatesGraphProxy::getToolTipTextforData(const unsigned int dataId) {
	string ttipText;
	if (getDataLocation() == NODE) {
		ttipText = "node ";
	} else {
		ttipText = "edge ";
	}
	ttipText += getStringFromNumber(dataId);
	string label = getDataLabel(dataId);
	if (!label.empty()) {
		ttipText = label + " (" + ttipText +")";
	}
	return ttipText;
}

void ParallelCoordinatesGraphProxy::addOrRemoveEltToHighlight(const unsigned int eltId) {
	if (isDataHighlighted(eltId)) {
		highlightedElts.erase(eltId);
	} else {
		highlightedElts.insert(eltId);
	}
}

void ParallelCoordinatesGraphProxy::unsetHighlightedElts() {
	highlightedElts.clear();
}

void ParallelCoordinatesGraphProxy::resetHighlightedElts(const set<unsigned int> &highlightedData) {
	highlightedElts.clear();
	set<unsigned int>::iterator it;
	for (it = highlightedData.begin() ; it != highlightedData.end() ; ++it) {
		addOrRemoveEltToHighlight(*it);
	}
}

bool ParallelCoordinatesGraphProxy::isDataHighlighted(const unsigned int dataId) {
	return highlightedElts.find(dataId) != highlightedElts.end();
}

bool ParallelCoordinatesGraphProxy::highlightedEltsSet() const {
	return highlightedElts.size() != 0;
}

void ParallelCoordinatesGraphProxy::selectHighlightedElements() {
	set<unsigned int>::iterator it;
	for (it = highlightedElts.begin() ; it != highlightedElts.end() ; ++it) {
		setDataSelected(*it, true);
	}
}

void ParallelCoordinatesGraphProxy::colorDataAccordingToHighlightedElts() {

	if (originalDataColors == NULL) {
		return;
	}

	// If new colors have been set for the graph elements, backup the change to restore the correct ones
	// when unhighlighting
	Iterator<unsigned int> *dataIt = getDataIterator(true);
	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();
		Color currentColor = getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId);
		Color originalColor;
		if (getDataLocation() == NODE) {
			originalColor = originalDataColors->getNodeValue(node(dataId));
		} else {
			originalColor = originalDataColors->getEdgeValue(edge(dataId));
		}
		if (currentColor != originalColor && currentColor != COLOR_NON_HIGHLIGHT) {
			if (getDataLocation() == NODE) {
				originalDataColors->setNodeValue(node(dataId), currentColor);
			} else {
				originalDataColors->setEdgeValue(edge(dataId), currentColor);
			}
		}
	}
	delete dataIt;

	if (!highlightedEltsSet()) {
		*(graph_component->getProperty<ColorProperty>("viewColor")) = *originalDataColors;
		return;
	}

	dataIt = getDataIterator(true);
	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();
		if (isDataHighlighted(dataId)) {
			setPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId, getOriginalDataColor(dataId));
		} else {
			setPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId, COLOR_NON_HIGHLIGHT);
		}
	}
	delete dataIt;
}

Color ParallelCoordinatesGraphProxy::getOriginalDataColor(const unsigned int dataId) {
	if (getDataLocation() == NODE) {
		return originalDataColors->getNodeValue(node(dataId));
	} else {
		return originalDataColors->getEdgeValue(edge(dataId));
	}
}

bool ParallelCoordinatesGraphProxy::unhighlightedEltsColorOk() {
	bool ret = true;
	Iterator<unsigned int> *dataIt = getDataIterator(true);
	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();
		if (highlightedElts.find(dataId) == highlightedElts.end()) {
			if (getDataColor(dataId) != COLOR_NON_HIGHLIGHT) {
				ret = false;
				break;
			}
		}
	}
	delete dataIt;

	return ret;
}

void ParallelCoordinatesGraphProxy::removeHighlightedElement(const unsigned int dataId) {
	highlightedElts.erase(dataId);
}

}
