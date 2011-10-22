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
  foreach(Interactor* i, _interactors)
  delete i;
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
  _graph = g;
  graphChanged(g);
}

QList<QWidget*> View::configurationWidgets() const {
  return QList<QWidget*>();
}
