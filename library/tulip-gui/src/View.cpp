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
#include "tulip/View.h"

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsItem>

#include <tulip/Interactor.h>

using namespace tlp;

View::View()
  :_currentInteractor(NULL), _graph(NULL) {
}

View::~View() {
  foreach(Interactor* i, _interactors) {
    delete i;
  }
}

QList<Interactor*> View::interactors() const {
  return _interactors;
}
void View::setInteractors(const QList<tlp::Interactor *> &inters) {
  _interactors = inters;
  foreach(Interactor* i,inters)
  i->setView(this);
  interactorsInstalled(inters);
}
Interactor* View::currentInteractor() const {
  return _currentInteractor;
}
void View::setCurrentInteractor(tlp::Interactor *i) {
  if (_currentInteractor)
    _currentInteractor->uninstall();

  _currentInteractor = i;
  currentInteractorChanged(i);
}
void View::currentInteractorChanged(tlp::Interactor *i) {
  i->install(graphicsView());
}

Graph* View::graph() const {
  return _graph;
}
void View::setGraph(tlp::Graph *g) {
  if (_graph != NULL)
    _graph->removeListener(this);

  _graph = g;

  graphChanged(g);

  if (_graph != NULL)
    _graph->addListener(this);

  emit graphSet(g);
}

void View::treatEvent(const Event& ev) {
  if (ev.type() == Event::TLP_DELETE && ev.sender() == _graph) {
    Graph* oldGraph = _graph;
    graphDeleted();
#ifndef NDEBUG

    if (_graph == oldGraph) {
      std::cerr << "The graphChanged() callback associated to this View did not change the current graph pointer. This could lead to undefined behavior. Please read View::graphDeleted() documentation for details." << std::endl;
    }

#endif
    assert(_graph != oldGraph); // Checks that the graph has been changed during the callback
  }
}

QList<QWidget*> View::configurationWidgets() const {
  return QList<QWidget*>();
}

void View::interactorsInstalled(const QList<tlp::Interactor *> &) {
}

void View::graphDeleted() {
  _graph = NULL;
  this->deleteLater();
}

void View::centerView() {
}

/*
  Triggers
  */
QSet<tlp::Observable*> View::triggers() const {
  return _triggers;
}

void View::removeRedrawTrigger(tlp::Observable* obs) {
  if (_triggers.remove(obs))
    obs->removeListener(this);
}

void View::addRedrawTrigger(tlp::Observable* obs) {
  if (_triggers.contains(obs) || obs == NULL)
    return;

  _triggers.insert(obs);
  obs->addObserver(this);
}

void View::treatEvents(const std::vector<Event> &events) {
  for(int i=0; i<events.size(); ++i) {
    Event e = events[i];

    if (_triggers.contains(e.sender())) {
      emit drawNeeded();
      break;
    }
  }
}

bool View::isLayoutProperty(tlp::PropertyInterface *) const {
  return false;
}
