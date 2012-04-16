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
#ifndef GRAPHNEEDSSAVINGOBSERVER_H
#define GRAPHNEEDSSAVINGOBSERVER_H

#include <tulip/Observable.h>
#include <tulip/ObservableGraph.h>
#include <tulip/ObservableProperty.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

#include <QtGui/qtabwidget.h>
#include <tulip/PropertyInterface.h>

#include <deque>

class GraphNeedsSavingObserver : public QObject, tlp::Observable {
  Q_OBJECT
public :

  GraphNeedsSavingObserver(tlp::Graph* graph) :_needsSaving(false), _graph(graph) {
    addObserver();
  }

protected :

  virtual void treatEvents(const std::vector<tlp::Event>&) {
    doNeedSaving();
  }

public :

  void saved() {
    _needsSaving = false;
    addObserver();
  }

  bool needsSaving() const {
    return _needsSaving;
  }

signals:
  void savingNeeded();

private:

  void doNeedSaving() {
    if(!_needsSaving) {
      // Stop listening graphs.
      removeObservers();
      _needsSaving = true;
      emit(savingNeeded());
    }
  }

  /**
    * @brief Listen all the observable objects in the graph (subgraphs, properties).
    **/
  void addObserver() {
    std::deque<tlp::Graph*> toObserve;
    toObserve.push_back(_graph);

    while(!toObserve.empty()) {
      tlp::Graph* current = toObserve.front();
      current->addObserver(this);
      toObserve.pop_front();

      //Listen properties.
      tlp::PropertyInterface* property;
      forEach(property,current->getLocalObjectProperties()) {
        property->addObserver(this);
      }

      //Fetch subgraphs
      tlp::Graph* subgraphs;
      forEach(subgraphs,current->getSubGraphs()) {
        toObserve.push_back(subgraphs);
      }
    }
  }

  /**
    * @brief  Stop listening all the observable objects in the graph (subgraphs, properties).
    **/
  void removeObservers() {
    std::deque<tlp::Graph*> toUnobserve;
    toUnobserve.push_back(_graph);

    while(!toUnobserve.empty()) {
      tlp::Graph* current = toUnobserve.front();
      toUnobserve.pop_front();

      current->removeObserver(this);

      //Stop listening properties.
      tlp::PropertyInterface* property;
      forEach(property,current->getLocalObjectProperties()) {
        property->removeObserver(this);
      }

      //Fetch subgraphs
      tlp::Graph* subgraphs;
      forEach(subgraphs,current->getSubGraphs()) {
        toUnobserve.push_back(subgraphs);
      }
    }
  }

  bool _needsSaving;
  tlp::Graph* _graph;
};

#endif //GRAPHNEEDSSAVINGOBSERVER_H
