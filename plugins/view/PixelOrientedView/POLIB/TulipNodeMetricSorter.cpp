/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include "TulipNodeMetricSorter.h"

#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>

using namespace std;
using namespace tlp;

namespace pocore {

map<Graph *, TulipNodeMetricSorter *> TulipNodeMetricSorter::instances;

TulipNodeMetricSorter *TulipNodeMetricSorter::getInstance(Graph *graph) {
  if (instances.find(graph) == instances.end()) {
    instances[graph] = new TulipNodeMetricSorter(graph);
  }

  return instances[graph];
}

TulipNodeMetricSorter::TulipNodeMetricSorter(Graph *graph) : graph(graph) {}

TulipNodeMetricSorter::~TulipNodeMetricSorter() {
  reset();
  instances.erase(graph);
}

void TulipNodeMetricSorter::sortNodesForProperty(const string &propertyName) {
  cleanupSortNodesForProperty(propertyName);
  nodeSortingMap[propertyName] = graph->nodes();

  const string &propertyType = graph->getProperty(propertyName)->getTypename();

  if (propertyType == "double") {
    sort(nodeSortingMap[propertyName].begin(), nodeSortingMap[propertyName].end(),
         NodeMetricPropertyOrderRelation<DoubleType, DoubleProperty>(graph, propertyName));
  } else if (propertyType == "int") {
    sort(nodeSortingMap[propertyName].begin(), nodeSortingMap[propertyName].end(),
         NodeMetricPropertyOrderRelation<IntegerType, IntegerProperty>(graph, propertyName));
  }
}

void TulipNodeMetricSorter::cleanupSortNodesForProperty(const std::string &propertyName) {
  nodeSortingMap.erase(propertyName);
}

node TulipNodeMetricSorter::getNodeAtRankForProperty(const unsigned int rank,
                                                     const string &propertyName) {
  if (nodeSortingMap.find(propertyName) == nodeSortingMap.end()) {
    sortNodesForProperty(propertyName);
  }

  return nodeSortingMap[propertyName][rank];
}

unsigned int TulipNodeMetricSorter::getNbValuesForProperty(const string &propertyName) {
  if (nbValuesPropertyMap.find(propertyName) == nbValuesPropertyMap.end()) {
    unsigned int count = 0;
    const string &propertyType = graph->getProperty(propertyName)->getTypename();

    if (propertyType == "double") {
      set<double> sd;

      for (auto n : graph->nodes()) {
        sd.insert(graph->getProperty<DoubleProperty>(propertyName)->getNodeValue(n));
      }

      count = sd.size();
    } else if (propertyType == "int") {
      set<int> si;

      for (auto n : graph->nodes()) {
        si.insert(graph->getProperty<IntegerProperty>(propertyName)->getNodeValue(n));
      }

      count = si.size();
    }

    nbValuesPropertyMap[propertyName] = count;
  }

  return nbValuesPropertyMap[propertyName];
}

void TulipNodeMetricSorter::reset() {
  nodeSortingMap.clear();
}

unsigned int TulipNodeMetricSorter::getNodeRankForProperty(tlp::node n,
                                                           const string &propertyName) {
  if (nodeSortingMap.find(propertyName) == nodeSortingMap.end()) {
    sortNodesForProperty(propertyName);
  }

  for (unsigned int i = 0; i < nodeSortingMap[propertyName].size(); ++i) {
    if (nodeSortingMap[propertyName][i] == n) {
      return i;
    }
  }

  return 0;
}
} // namespace pocore
