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

#include "TulipGraphDimension.h"

#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/StringProperty.h>

#include <set>

using namespace tlp;
using namespace std;

namespace pocore {

map<Graph *, unsigned int> TulipGraphDimension::graphDimensionsMap;

TulipGraphDimension::TulipGraphDimension(Graph *graph, const string& dimName) : graph(graph), dimName(dimName) {
  nodeSorter = TulipNodeMetricSorter::getInstance(graph);
  nodeSorter->sortNodesForProperty(dimName);
  propertyType = graph->getProperty(dimName)->getTypename();

  if (graphDimensionsMap.find(graph) == graphDimensionsMap.end()) {
    graphDimensionsMap[graph] = 1;
  }
  else {
    ++graphDimensionsMap[graph];
  }
}

TulipGraphDimension::~TulipGraphDimension() {
  --graphDimensionsMap[graph];

  if (graphDimensionsMap[graph] == 0) {
    delete nodeSorter;
    graphDimensionsMap.erase(graph);
  }
}

void TulipGraphDimension::updateNodesRank() {
  nodeSorter->sortNodesForProperty(dimName);
}

unsigned int TulipGraphDimension::numberOfItems() const {
  return graph->numberOfNodes();
}

unsigned int TulipGraphDimension::numberOfValues() const {
  return nodeSorter->getNbValuesForProperty(dimName);
}

template <typename PROPERTY>
double TulipGraphDimension::getNodeValue(const node n) const {
  PROPERTY *dimValues = graph->getProperty<PROPERTY>(dimName);
  double d = dimValues->getNodeValue(n);
  double delta = maxValue() - minValue();
  return (d - minValue()) / delta;
}

std::string TulipGraphDimension::getItemLabelAtRank(const unsigned int rank) const {
  node n = nodeSorter->getNodeAtRankForProperty(rank, dimName);
  string label = graph->getProperty<StringProperty>("viewLabel")->getNodeValue(n);
  return label;
}

std::string TulipGraphDimension::getItemLabel(const unsigned int itemId) const {
  string label = graph->getProperty<StringProperty>("viewLabel")->getNodeValue(node(itemId));
  return label;
}

double TulipGraphDimension::getItemValue(const unsigned int itemId) const {
  if (propertyType == "double") {
    return getNodeValue<DoubleProperty>(node(itemId));
  }
  else if (propertyType == "int") {
    return getNodeValue<IntegerProperty>(node(itemId));
  }
  else {
    return 0;
  }
}

double TulipGraphDimension::getItemValueAtRank(const unsigned int rank) const {
  node n = nodeSorter->getNodeAtRankForProperty(rank, dimName);

  if (propertyType == "double") {
    return getNodeValue<DoubleProperty>(n);
  }
  else if (propertyType == "int") {
    return getNodeValue<IntegerProperty>(n);
  }
  else {
    return 0;
  }
}

unsigned int TulipGraphDimension::getItemIdAtRank(const unsigned int rank) {
  node n = nodeSorter->getNodeAtRankForProperty(rank, dimName);
  return n.id;
}

unsigned int TulipGraphDimension::getRankForItem(const unsigned int itemId) {
  return nodeSorter->getNodeRankForProperty(node(itemId), dimName);
}

double TulipGraphDimension::minValue() const {
  if (propertyType == "double") {
    return graph->getProperty<DoubleProperty>(dimName)->getNodeMin(graph);
  }
  else if (propertyType == "int") {
    return graph->getProperty<IntegerProperty>(dimName)->getNodeMin(graph);
  }
  else {
    return 0;
  }
}

double TulipGraphDimension::maxValue() const {
  if (propertyType == "double") {
    return graph->getProperty<DoubleProperty>(dimName)->getNodeMax(graph);
  }
  else if (propertyType == "int") {
    return graph->getProperty<IntegerProperty>(dimName)->getNodeMax(graph);
  }
  else {
    return 0;
  }
}

vector<unsigned int> TulipGraphDimension::links(const unsigned int itemId) const {
  vector<unsigned int> v;

  Iterator<node> *inNodesIt = graph->getInNodes(node(itemId));

  while (inNodesIt->hasNext()) {
    v.push_back(inNodesIt->next().id);
  }

  delete inNodesIt;

  Iterator<node> *outNodesIt = graph->getOutNodes(node(itemId));

  while (outNodesIt->hasNext()) {
    v.push_back(outNodesIt->next().id);
  }

  delete outNodesIt;

  return v;
}

}
