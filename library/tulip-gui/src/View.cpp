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
