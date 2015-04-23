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

#include <tulip/View.h>

#include <QDebug>
#include <QFile>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMenu>

#include <tulip/Interactor.h>
#include <tulip/Graph.h>

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
  if (_currentInteractor) {
    _currentInteractor->uninstall();

    if (graphicsView() != NULL)
      graphicsView()->setCursor(QCursor()); // Force reset cursor when interactor is changed
  }

  _currentInteractor = i;
  currentInteractorChanged(i);

  //We need a refresh here to clear last interactor displayed and init the next one
  refresh();
}
void View::currentInteractorChanged(tlp::Interactor *i) {
  i->install(graphicsView());
}

void View::showContextMenu(const QPoint &point,const QPointF &scenePoint) {
  QMenu menu;
  menu.setStyleSheet("QMenu::item:disabled {color: white; background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, stop:0 rgb(75,75,75), stop:1 rgb(60, 60, 60))}");
  fillContextMenu(&menu,scenePoint);

  if (!menu.actions().empty()) {
    menu.move(point);
    menu.exec();
  }
}

void View::undoCallback() {
  centerView();
}

Graph* View::graph() const {
  return _graph;
}
void View::setGraph(tlp::Graph *g) {
  if (_graph != NULL)
    _graph->removeListener(this);

  bool center = false;

  if (g != _graph) {
    if (g == NULL)
      center = true;
    else if (_graph != NULL && g->getRoot() != _graph->getRoot())
      center = true;
  }

  _graph = g;

  graphChanged(g);

  if (_graph != NULL)
    _graph->addListener(this);

  emit graphSet(g);

  if (center)
    centerView();
}

void View::treatEvent(const Event& ev) {
  const GraphEvent *gEv = dynamic_cast<const GraphEvent*>(&ev);

  if (ev.type() == Event::TLP_DELETE && ev.sender() == _graph) {
#ifndef NDEBUG
    Graph* old = _graph;
#endif // NDEBUG

    if (_graph->getRoot() == _graph)
      graphDeleted(NULL);
    else
      graphDeleted(_graph->getSuperGraph());

#ifndef NDEBUG

    if (_graph == old) {
      qWarning() << __PRETTY_FUNCTION__ << ": Graph pointer is unchanged.";
    }

#endif // NDEBUG
  }
  else if (gEv && gEv->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY) {
    QString propName = gEv->getPropertyName().c_str();

    if (propName.startsWith("view")) {
      addRedrawTrigger(_graph->getProperty(propName.toStdString()));
    }
  }
}

QList<QWidget*> View::configurationWidgets() const {
  return QList<QWidget*>();
}

QString View::configurationWidgetsStyleSheet() const {
  QFile css(":/tulip/gui/txt/view_configurationtab.css");
  css.open(QIODevice::ReadOnly);
  QString style(css.readAll());
  css.close();
  return style;
}

void View::interactorsInstalled(const QList<tlp::Interactor *> &) {
  emit interactorsChanged();
}

void View::centerView(bool/* graphChanged */) {
  draw();
}

/*
  Triggers
  */
QSet<tlp::Observable*> View::triggers() const {
  return _triggers;
}

void View::removeRedrawTrigger(tlp::Observable* obs) {
  if (_triggers.remove(obs))
    obs->removeObserver(this);
}

void View::emitDrawNeededSignal() {
  emit drawNeeded();
}

void View::addRedrawTrigger(tlp::Observable* obs) {
  if (_triggers.contains(obs) || obs == NULL)
    return;

  _triggers.insert(obs);
  obs->addObserver(this);
}

void View::treatEvents(const std::vector<Event> &events) {
  for(unsigned int i=0; i<events.size(); ++i) {
    Event e = events[i];

    if (_triggers.contains(e.sender())) {
      emit drawNeeded();
      break;
    }
  }
}

QGraphicsItem* View::centralItem() const {
  return NULL;
}

void View::clearRedrawTriggers() {
  foreach(Observable* t, triggers()) removeRedrawTrigger(t);
}

void View::applySettings() {
}
