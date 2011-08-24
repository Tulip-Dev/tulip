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

class GraphNeedsSavingObserver :
  public tlp::GraphObserver, public tlp::PropertyObserver {
public :

  GraphNeedsSavingObserver(QTabWidget* tabWidget, int graphIndex, tlp::Graph* graph, bool = true) :_needsSaving(false), _tabIndex(graphIndex), _tabWidget(tabWidget), _graph(graph) {
    addObserver();
  }

protected :

  virtual void addEdge(tlp::Graph* , const tlp::edge ) {
    doNeedSaving();
  }
  virtual void addNode(tlp::Graph* , const tlp::node ) {
    doNeedSaving();
  }
  virtual void afterSetAttribute(tlp::Graph* , const std::string& ) {
    doNeedSaving();
  }
  virtual void beforeSetAllEdgeValue(tlp::PropertyInterface* ) {
    doNeedSaving();
  }
  virtual void beforeSetAllNodeValue(tlp::PropertyInterface* ) {
    doNeedSaving();
  }
  virtual void beforeSetAttribute(tlp::Graph* , const std::string& ) {
    doNeedSaving();
  }
  virtual void beforeSetEdgeValue(tlp::PropertyInterface* , const tlp::edge ) {
    doNeedSaving();
  }
  virtual void beforeSetNodeValue(tlp::PropertyInterface* , const tlp::node ) {
    doNeedSaving();
  }
  virtual void delEdge(tlp::Graph* , const tlp::edge ) {
    doNeedSaving();
  }
  virtual void delNode(tlp::Graph* , const tlp::node ) {
    doNeedSaving();
  }
  virtual void removeAttribute(tlp::Graph* , const std::string& ) {
    doNeedSaving();
  }
  virtual void reverseEdge(tlp::Graph* , const tlp::edge ) {
    doNeedSaving();
  }
  virtual void addLocalProperty(tlp::Graph* , const std::string& ) {
    doNeedSaving();
  }
  virtual void delLocalProperty(tlp::Graph* , const std::string& ) {
    doNeedSaving();
  }
  virtual void addSubGraph(tlp::Graph* , tlp::Graph*) {
    doNeedSaving();
  }
  virtual void delSubGraph(tlp::Graph* , tlp::Graph* ) {
    doNeedSaving();
  }

  virtual void destroy(tlp::Graph* graph) {
    graph->removeGraphObserver(this);
  }
  virtual void destroy(tlp::PropertyInterface* property) {
    property->removePropertyObserver(this);
  }

public :

  void saved() {
    _needsSaving = false;
    addObserver();
  }

  bool needSaving() const {
    return _needsSaving;
  }

  void doNeedSaving() {
    if(!_needsSaving) {
      //add the "*" char in the tab
      _tabWidget->setTabText(_tabIndex, _tabWidget->tabText(_tabIndex) + " *");
      // Stop listening graphs.
      removeObservers();
      _needsSaving = true;
    }
  }



private:
  /**
    * @brief Listen all the observable objects in the graph (subgraphs, properties).
    **/
  void addObserver() {
    std::deque<tlp::Graph*> toObserve;
    toObserve.push_back(_graph);

    while(!toObserve.empty()) {
      tlp::Graph* current = toObserve.front();
      current->addGraphObserver(this);
      toObserve.pop_front();

      //Listen properties.
      tlp::PropertyInterface* property;
      forEach(property,current->getLocalObjectProperties()) {
        property->addPropertyObserver(this);
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

      current->removeGraphObserver(this);

      //Stop listening properties.
      tlp::PropertyInterface* property;
      forEach(property,current->getLocalObjectProperties()) {
        property->removePropertyObserver(this);
      }

      //Fetch subgraphs
      tlp::Graph* subgraphs;
      forEach(subgraphs,current->getSubGraphs()) {
        toUnobserve.push_back(subgraphs);
      }
    }
  }

  bool _needsSaving;
  int _tabIndex;
  QTabWidget* _tabWidget;
  tlp::Graph* _graph;
};

#endif //GRAPHNEEDSSAVINGOBSERVER_H
