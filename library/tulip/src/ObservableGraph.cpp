#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableGraph.h"

using namespace stdext;
using namespace tlp;

void ObservableGraph::notifyAddNode(Graph *sg, const node n) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->addNode(sg, n);
  }
}

void ObservableGraph::notifyDelNode(Graph *sg, const node n) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->delNode(sg, n);
  }
}

void ObservableGraph::notifyAddEdge(Graph *sg, const edge e) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->addEdge(sg, e);
  }
}

void ObservableGraph::notifyDelEdge(Graph *sg, const edge e) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->delEdge(sg, e);
  }
}

void ObservableGraph::notifyReverseEdge(Graph *sg, const edge e) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->reverseEdge(sg, e);
  }
}

void ObservableGraph::notifyDestroy(Graph *sg) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->destroy(sg);
  }
}

void ObservableGraph::notifyAddSubGraph(Graph *g, Graph *sg) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->addSubGraph(g, sg);
  }
}

void ObservableGraph::notifyDelSubGraph(Graph *g, Graph *sg) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->delSubGraph(g, sg);
  }
}

void ObservableGraph::notifyAddLocalProperty(Graph *g, const std::string& name) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->addLocalProperty(g, name);
  }
}

void ObservableGraph::notifyDelLocalProperty(Graph *g, const std::string& name) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->delLocalProperty(g, name);
  }
}
