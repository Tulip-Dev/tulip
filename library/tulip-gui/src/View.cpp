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

#include <tulip/View.h>

#include <QDebug>
#include <QFile>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMenu>
#include <QMainWindow>
#include <QStatusBar>

#include <tulip/Interactor.h>
#include <tulip/Graph.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>
#include <tulip/ViewToolTipAndUrlManager.h>

using namespace tlp;

View::View()
    : _currentInteractor(nullptr), _graph(nullptr), _tturlManager(nullptr),
      interactorsActivated(false), _displayContextMenu(true) {}

View::~View() {
  for (auto i : _interactors) {
    // as the current view is being deleted
    // we must detach it from the interactors before
    // deleting them
    i->setView(nullptr);
    delete i;
  }
  delete _tturlManager;
}

void View::toggleInteractors(const bool activate,
                             const std::unordered_set<const char *> &exceptions) {
  for (auto it : _interactors) {
    if (exceptions.find(it->name().c_str()) == exceptions.end()) {
      it->action()->setEnabled(activate);
      if (!activate) {
        it->action()->setChecked(false);
      }
    } else if (!activate) {
      it->action()->setChecked(true);
    }
    interactorsActivated = activate;
  }
}

void View::setInteractors(const QList<tlp::Interactor *> &inters) {
  _interactors = inters;

  for (auto i : inters)
    i->setView(this);

  interactorsInstalled(inters);
}
Interactor *View::currentInteractor() const {
  return _currentInteractor;
}
void View::setCurrentInteractor(tlp::Interactor *i) {
  if (_currentInteractor) {
    _currentInteractor->uninstall();

    if (graphicsView() != nullptr)
      graphicsView()->setCursor(QCursor()); // Force reset cursor when interactor is changed
  }

  _currentInteractor = i;
  currentInteractorChanged(i);

  // We need a refresh here to clear last interactor displayed and init the next one
  refresh();
}
void View::currentInteractorChanged(tlp::Interactor *i) {
  if (i)
    i->install(graphicsView());
}

void View::activateTooltipAndUrlManager(QWidget *w) {
  delete _tturlManager;
  _tturlManager = new tlp::ViewToolTipAndUrlManager(this, w);
}

void View::fillContextMenu(QMenu *menu, const QPointF &) {
  if (_tturlManager)
    _tturlManager->fillContextMenu(menu);
}

void View::fillContextMenu(QMenu *menu, node n) {
  if (_tturlManager)
    _tturlManager->fillContextMenu(menu, n);
}

void View::fillContextMenu(QMenu *menu, edge e) {
  if (_tturlManager)
    _tturlManager->fillContextMenu(menu, e);
}

bool View::showContextMenu(const QPoint &point, const QPointF &scenePoint) {
  // try to display current interactor context menu first
  if (_currentInteractor && _currentInteractor->showContextMenu(point, scenePoint))
    return true;

  if (_displayContextMenu) {
    QMenu menu;
    menu.setStyleSheet("QMenu::item:disabled {color: white; background-color: "
                       "qlineargradient(spread:pad, x1:0, y1:0, x2:, y2:1, stop:0 rgb(75,75,75), "
                       "stop:1 rgb(60, 60, 60))}");
    fillContextMenu(&menu, scenePoint);

    if (!menu.actions().empty()) {
      Perspective::redirectStatusTipOfMenu(&menu);
      menu.move(point);
      menu.exec();
      return true;
    }
  }
  return false;
}

DataSet View::state() const {
  DataSet data;
  if (_tturlManager)
    _tturlManager->state(data);
  return data;
}

void View::setState(const DataSet &dataSet) {
  if (_tturlManager)
    _tturlManager->setState(dataSet);
}

void View::undoCallback() {
  centerView();
}

Graph *View::graph() const {
  return _graph;
}
void View::setGraph(tlp::Graph *g) {
  if (_graph != nullptr)
    _graph->removeListener(this);

  bool center = false;

  if (g != _graph) {
    if (g == nullptr)
      center = true;
    else if (_graph != nullptr && g->getRoot() != _graph->getRoot())
      center = true;
  }

  _graph = g;

  graphChanged(g);

  if (_graph != nullptr)
    _graph->addListener(this);

  emit graphSet(g);

  if (center)
    centerView();
}

void View::treatEvent(const Event &ev) {
  const GraphEvent *gEv = dynamic_cast<const GraphEvent *>(&ev);

  if (ev.type() == Event::TLP_DELETE && ev.sender() == _graph) {
#ifndef NDEBUG
    Graph *old = _graph;
#endif // NDEBUG

    if (_graph->getRoot() == _graph)
      graphDeleted(nullptr);
    else
      graphDeleted(_graph->getSuperGraph());

#ifndef NDEBUG

    if (_graph == old) {
      qWarning() << __PRETTY_FUNCTION__ << ": Graph pointer is unchanged.";
    }

#endif // NDEBUG
  } else if (gEv && gEv->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY) {
    QString propName = gEv->getPropertyName().c_str();

    if (propName.startsWith("view")) {
      addRedrawTrigger(_graph->getProperty(QStringToTlpString(propName)));
    }
  }
}

QList<QWidget *> View::configurationWidgets() const {
  return QList<QWidget *>();
}

void View::interactorsInstalled(const QList<tlp::Interactor *> &) {
  emit interactorsChanged();
}

void View::centerView(bool /* graphChanged */) {
  draw();
}

/*
  Triggers
  */
QSet<tlp::Observable *> View::triggers() const {
  return _triggers;
}

void View::removeRedrawTrigger(tlp::Observable *obs) {
  if (_triggers.remove(obs))
    obs->removeObserver(this);
}

void View::emitDrawNeededSignal() {
  emit drawNeeded();
}

void View::addRedrawTrigger(tlp::Observable *obs) {
  if (_triggers.contains(obs) || obs == nullptr)
    return;

  _triggers.insert(obs);
  obs->addObserver(this);
}

void View::treatEvents(const std::vector<Event> &events) {
  for (unsigned int i = 0; i < events.size(); ++i) {
    Event e = events[i];

    // ensure redraw trigger is removed from the triggers set when it is deleted
    if (e.type() == Event::TLP_DELETE && _triggers.contains(e.sender())) {
      removeRedrawTrigger(e.sender());
    }

    if (_triggers.contains(e.sender())) {
      emit drawNeeded();
      break;
    }
  }
}

QGraphicsItem *View::centralItem() const {
  return nullptr;
}

void View::clearRedrawTriggers() {
  for (auto t : triggers())
    removeRedrawTrigger(t);
}

void View::applySettings() {}
