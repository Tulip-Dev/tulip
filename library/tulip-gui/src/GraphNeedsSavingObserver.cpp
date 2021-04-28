/*
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

#include <tulip/Graph.h>
#include <tulip/GraphNeedsSavingObserver.h>
#include <tulip/PropertyInterface.h>

#include <deque>

#include <QMainWindow>

using namespace tlp;
using namespace std;

GraphNeedsSavingObserver::GraphNeedsSavingObserver(Graph *graph,
                                                   QMainWindow *mainWindow)
    : _needsSaving(false), _graph(graph), _mainWindow(mainWindow) {
  addObserver();
}

void GraphNeedsSavingObserver::treatEvents(const vector<Event> &) {
  if (!_needsSaving)
    forceToSave();
}

void GraphNeedsSavingObserver::saved() {
  _needsSaving = false;
  removeObservers();
  addObserver();

  if (_mainWindow)
    _mainWindow->setWindowModified(false);
}

bool GraphNeedsSavingObserver::needsSaving() const { return _needsSaving; }

void GraphNeedsSavingObserver::forceToSave() {
  _needsSaving = true;
  // No need to listen to the graph anymore
  removeObservers();

  if (_mainWindow)
    _mainWindow->setWindowModified(true);

  emit(savingNeeded());
}

/**
 * @brief Listen all the observable objects in the graph (subgraphs,
 *properties).
 **/
void GraphNeedsSavingObserver::addObserver() {
  deque<Graph *> toObserve;
  toObserve.push_back(_graph);

  while (!toObserve.empty()) {
    Graph *current = toObserve.front();
    current->addObserver(this);
    toObserve.pop_front();

    // Listen properties.
    for (PropertyInterface *property : current->getLocalObjectProperties()) {
      property->addObserver(this);
    }

    // Fetch subgraphs
    for (Graph *subgraph : current->subGraphs()) {
      toObserve.push_back(subgraph);
    }
  }
}

/**
 * @brief  Stop listening all the observable objects in the graph (subgraphs,
 *properties).
 **/
void GraphNeedsSavingObserver::removeObservers() {
  deque<Graph *> toUnobserve;
  toUnobserve.push_back(_graph);

  while (!toUnobserve.empty()) {
    Graph *current = toUnobserve.front();
    toUnobserve.pop_front();

    current->removeObserver(this);

    // Stop listening properties.
    for (PropertyInterface *property : current->getLocalObjectProperties()) {
      property->removeObserver(this);
    }

    // Fetch subgraphs
    for (Graph *subgraph : current->subGraphs()) {
      toUnobserve.push_back(subgraph);
    }
  }
}
