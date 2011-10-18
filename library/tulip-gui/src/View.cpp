#include "tulip/View.h"

#include <QtGui/QGraphicsItem>

using namespace tlp;

View::View()
  : _interactors(QList<Interactor*>()), _activeInteractor(NULL),
    _triggers(QSet<Observable*>()),
    _graph(NULL) {
}

View::~View() {
}

void View::setInteractors(const QList<tlp::Interactor *> &inters) {
  _interactors = inters;
  interactorsInstalled(inters);
}

QList<Interactor*> View::interactors() const {
  return _interactors;
}

void View::setCurrentInteractor(tlp::Interactor *i) {
  _activeInteractor = i;
  currentInteractorChanged(i);
}

Interactor* View::currentInteractor() const {
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

void View::interactorsInstalled(const QList<tlp::Interactor*>&) {
}

void View::graphChanged(tlp::Graph*) {
  emit drawNeeded();
}

QPointF View::pos() const {
  return graphicsItem()->pos();
}

QSizeF View::size() const {
  return graphicsItem()->boundingRect().size();
}

void View::setPos(const QPointF &p) {
  graphicsItem()->setPos(p);
}

void View::resize(const QSizeF &newSize) {
  QSizeF originalSize = graphicsItem()->boundingRect().size();
  graphicsItem()->scale(newSize.width()/originalSize.height(),newSize.height()/originalSize.height());
}
