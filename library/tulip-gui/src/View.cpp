/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <QGraphicsView>

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

void View::setInteractors(const std::list<tlp::Interactor *> &inters) {
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

    if (graphicsView() != nullptr && i != _currentInteractor)
      // Force cursor reset when changing interactor
      graphicsView()->setCursor(QCursor());
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
  if (_graph != nullptr) {
    _graph->removeListener(this);
    saveState();
  }

  bool center = false;

  if (g != _graph) {
    if (g == nullptr)
      center = true;
    else if (_graph != nullptr && g->getRoot() != _graph->getRoot())
      center = true;
  }

  _graph = g;

  graphChanged(g);
  // ensure current interactor refresh
  if (currentInteractor())
    setCurrentInteractor(currentInteractor());

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

std::list<QWidget *> View::configurationWidgets() const {
  return std::list<QWidget *>();
}

void View::interactorsInstalled(const std::list<tlp::Interactor *> &) {
  emit interactorsChanged();
}

void View::centerView(bool /* graphChanged */) {
  draw();
}

/*
  Triggers
  */
std::list<tlp::Observable *> View::triggers() const {
  return _triggers;
}

void View::removeRedrawTrigger(tlp::Observable *obs) {
  // to be changed for C++20. remove() returns the number of element removed. Returns void with
  // c++17
  if (std::find(_triggers.begin(), _triggers.end(), obs) != _triggers.end()) {
    _triggers.remove(obs);
    obs->removeObserver(this);
  }
}

void View::emitDrawNeededSignal() {
  emit drawNeeded();
}

void View::addRedrawTrigger(tlp::Observable *obs) {
  if ((obs == nullptr) || std::find(_triggers.begin(), _triggers.end(), obs) != _triggers.end())
    return;

  _triggers.push_back(obs);
  obs->addObserver(this);
}

void View::treatEvents(const std::vector<Event> &events) {
  for (unsigned int i = 0; i < events.size(); ++i) {
    Event e = events[i];

    // ensure redraw trigger is removed from the triggers set when it is deleted
    if (e.type() == Event::TLP_DELETE &&
        std::find(_triggers.begin(), _triggers.end(), e.sender()) != _triggers.end()) {
      removeRedrawTrigger(e.sender());
    }

    if (std::find(_triggers.begin(), _triggers.end(), e.sender()) != _triggers.end()) {
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

// define a class to save/restore the View state associated
// to a graph
class ViewStatesMap : public tlp::Observable {
  tlp_hash_map<std::string, tlp_hash_map<Graph *, DataSet>> mMap;

public:
  void saveState(const std::string &viewName, Graph *graph, const DataSet &ds) {
    const auto it = mMap.find(viewName);
    // register this as listener
    if (it == mMap.cend() || (it->second.find(graph) == it->second.end()))
      graph->addListener(this);
    // associate state to graph
    mMap[viewName][graph] = ds;
  }

  DataSet getState(const std::string &viewName, Graph *graph) const {
    // get view map
    const auto it = mMap.find(viewName);

    if (it == mMap.cend())
      return DataSet();

    auto &vMap = it->second;
    // look for a DataSet associated to graph or its nearest parent graph
    while (true) {
      auto itg = vMap.find(graph);
      if (itg != vMap.cend())
        return itg->second;
      auto parent = graph->getSuperGraph();
      if (graph == parent)
        return DataSet();
      graph = parent;
    }
  }

  void treatEvent(const Event &ev) override {
    // look for graph deletion
    if (ev.type() == Event::TLP_DELETE) {
      const GraphEvent *gEv = dynamic_cast<const GraphEvent *>(&ev);
      if (gEv) {
        auto graph = gEv->getGraph();
        // remove graph entry
        auto itm = mMap.begin();
        for (; itm != mMap.end(); ++itm) {
          auto itg = itm->second.begin();
          for (; itg != itm->second.end(); ++itg) {
            if (itg->first == graph) {
              itm->second.erase(graph);
              return;
            }
          }
        }
      }
    }
  }
};

static ViewStatesMap _viewStatesMap;

void View::saveState() {
  if (_graph) {
    _viewStatesMap.saveState(name(), _graph, state());
  }
}

DataSet View::getState(Graph *graph) {
  if (graph)
    return _viewStatesMap.getState(name(), graph);
  return DataSet();
}
