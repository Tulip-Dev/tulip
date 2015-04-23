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

#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/ForEach.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>


#include "ParallelCoordinatesGraphProxy.h"
#include "ParallelTools.h"

using namespace std;

namespace tlp {

ParallelCoordinatesGraphProxy::ParallelCoordinatesGraphProxy(Graph *g, const ElementType location):
  GraphDecorator(g),  graphColorsChanged(false), dataLocation(location), unhighlightedEltsColorAlphaValue(20) {
  dataColors = graph_component->getProperty<ColorProperty>("viewColor");
  dataColors->addObserver(this);
  originalDataColors = new ColorProperty(graph_component);
  *originalDataColors = *(graph_component->getProperty<ColorProperty>("viewColor"));
}

ParallelCoordinatesGraphProxy::~ParallelCoordinatesGraphProxy() {
  dataColors->removeObserver(this);
  Observable::holdObservers();
  *dataColors = *originalDataColors;
  delete originalDataColors;
  originalDataColors = NULL;
  Observable::unholdObservers();
}

unsigned int ParallelCoordinatesGraphProxy::getNumberOfSelectedProperties() const {
  return selectedProperties.size();
}

vector<string> ParallelCoordinatesGraphProxy::getSelectedProperties() {
  vector<string> selectedPropertiesTmp;
  vector<string>::iterator it;

  // check if one of the selected properties has not been deleted by an undo operation
  for (it = selectedProperties.begin() ; it != selectedProperties.end() ; ++it) {
    if (existProperty(*it)) {
      selectedPropertiesTmp.push_back(*it);
    }
  }

  selectedProperties = selectedPropertiesTmp;
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
    return numberOfNodes();
  }
  else {
    return numberOfEdges();
  }
}

Color ParallelCoordinatesGraphProxy::getDataColor(const unsigned int dataId) {
  return getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId);
}

string ParallelCoordinatesGraphProxy::getDataTexture(const unsigned int dataId) {
  return getPropertyValueForData<StringProperty, StringType>("viewTexture", dataId);
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
  }
  else {
    delEdge(edge(dataId));
  }
}

Iterator<unsigned int> *ParallelCoordinatesGraphProxy::getDataIterator() {
  if (getDataLocation() == NODE) {
    return new ParallelCoordinatesDataIterator<node>(getNodes());
  }
  else {
    return new ParallelCoordinatesDataIterator<edge>(getEdges());
  }
}

Iterator<unsigned int> *ParallelCoordinatesGraphProxy::getSelectedDataIterator() {
  BooleanProperty *viewSelection = (BooleanProperty *) getProperty("viewSelection");

  if (getDataLocation() == NODE) {
    return new ParallelCoordinatesDataIterator<node>(viewSelection->getNodesEqualTo(true, graph_component));
  }
  else {
    return new ParallelCoordinatesDataIterator<edge>(viewSelection->getEdgesEqualTo(true, graph_component));
  }
}

Iterator<unsigned int> *ParallelCoordinatesGraphProxy::getUnselectedDataIterator() {
  BooleanProperty *viewSelection = (BooleanProperty *) getProperty("viewSelection");

  if (getDataLocation() == NODE) {
    return new ParallelCoordinatesDataIterator<node>(viewSelection->getNodesEqualTo(false));
  }
  else {
    return new ParallelCoordinatesDataIterator<edge>(viewSelection->getEdgesEqualTo(false));
  }
}

string ParallelCoordinatesGraphProxy::getToolTipTextforData(const unsigned int dataId) {
  string ttipText;

  if (getDataLocation() == NODE) {
    ttipText = "node ";
  }
  else {
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
  }
  else {
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
  return !highlightedElts.empty();
}

void ParallelCoordinatesGraphProxy::selectHighlightedElements() {
  set<unsigned int>::iterator it;

  for (it = highlightedElts.begin() ; it != highlightedElts.end() ; ++it) {
    setDataSelected(*it, true);
  }
}

void ParallelCoordinatesGraphProxy::colorDataAccordingToHighlightedElts() {

  static bool lastHighlightedElementsSet = false;

  if (originalDataColors == NULL) {
    return;
  }

  graphColorsChanged = false;

  // If new colors have been set for the graph elements, backup the change to restore the correct ones
  // when unhighlighting
  if (highlightedEltsSet()) {
    Iterator<unsigned int> *dataIt = getDataIterator();

    while (dataIt->hasNext()) {
      unsigned int dataId = dataIt->next();
      Color currentColor = getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId);
      Color originalColor;

      if (getDataLocation() == NODE) {
        originalColor = originalDataColors->getNodeValue(node(dataId));
      }
      else {
        originalColor = originalDataColors->getEdgeValue(edge(dataId));
      }

      if (!isDataHighlighted(dataId) && currentColor.getA() != unhighlightedEltsColorAlphaValue) {
        if (getDataLocation() == NODE) {
          originalDataColors->setNodeValue(node(dataId), Color(currentColor.getR(), currentColor.getG(), currentColor.getB(), originalColor.getA()));
        }
        else {
          originalDataColors->setEdgeValue(edge(dataId), Color(currentColor.getR(), currentColor.getG(), currentColor.getB(), originalColor.getA()));
        }

        Color newColor = getOriginalDataColor(dataId);
        newColor.setA(unhighlightedEltsColorAlphaValue);
        setPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId, newColor);
      }

      if (highlightedEltsSet() && isDataHighlighted(dataId) && currentColor != originalColor) {
        if (getDataLocation() == NODE) {
          originalDataColors->setNodeValue(node(dataId), Color(currentColor.getR(), currentColor.getG(), currentColor.getB(), originalColor.getA()));
        }
        else {
          originalDataColors->setEdgeValue(edge(dataId), Color(currentColor.getR(), currentColor.getG(), currentColor.getB(), originalColor.getA()));
        }

        setPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId, getOriginalDataColor(dataId));
      }
    }

    delete dataIt;
    lastHighlightedElementsSet = true;
  }
  else if (lastHighlightedElementsSet) {
    *(graph_component->getProperty<ColorProperty>("viewColor")) = *originalDataColors;
    lastHighlightedElementsSet = false;
  }
  else {
    *originalDataColors = *dataColors;
  }


}

Color ParallelCoordinatesGraphProxy::getOriginalDataColor(const unsigned int dataId) {
  if (getDataLocation() == NODE) {
    return originalDataColors->getNodeValue(node(dataId));
  }
  else {
    return originalDataColors->getEdgeValue(edge(dataId));
  }
}

void ParallelCoordinatesGraphProxy::removeHighlightedElement(const unsigned int dataId) {
  highlightedElts.erase(dataId);
}

void ParallelCoordinatesGraphProxy::update(std::set<Observable *>::iterator begin ,std::set<Observable *>::iterator end) {
  (void) begin;
  (void) end;
  graphColorsChanged = true;
}

}
