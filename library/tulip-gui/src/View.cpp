#include "tulip/View.h"

using namespace tlp;

View::View()
  : _hasInteractorsResponsibility(true), _hasConfigurationWidgetsResponsibility(true),
    _interactors(QSet<Interactor*>()), _activeInteractor(NULL),
    _triggers(QSet<Observable*>()),
    _graph(NULL) {
}

View::~View() {
}

void View::setInteractors(const QSet<tlp::Interactor *> &inters) {
  _interactors = inters;
  interactorsInstalled(inters);
}

void View::setActiveInteractor(tlp::Interactor *i) {
  _activeInteractor = i;
  activeInteractorChanged(i);
}

Interactor* View::activeInteractor() const {
  return _activeInteractor;
}

void View::registerTrigger(tlp::Observable *obs) {
  if (_triggers.contains(obs))
    return;
  _triggers.insert(obs);
  obs->addObserver(this);
}

void View::removeTrigger(tlp::Observable *obs) {
  _triggers.remove(obs);
  obs->removeObserver(this);
}

Graph* View::graph() const {
  return _graph;
}

void View::setGraph(tlp::Graph *g) {
  _graph = g;
  graphChanged(g);
}

void View::interactorsInstalled(const QSet<tlp::Interactor*>&) {
}

void View::activeInteractorChanged(tlp::Interactor*) {
}

void View::graphChanged(tlp::Graph*) {
  emit drawNeeded();
}
