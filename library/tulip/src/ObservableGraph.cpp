#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableGraph.h"

using namespace stdext;
using namespace tlp;

GraphObserver::~GraphObserver(){
/*#ifndef NDEBUG
  if(observables.size()!=0)
    std::cerr << "Delete a graph observer without remove it from observable list" << std::endl;
#endif*/
  for(slist<ObservableGraph *>::iterator it=observables.begin();it!=observables.end();++it){
    (*it)->removeOnlyGraphObserver(this);
  }
}

void GraphObserver::addObservable(ObservableGraph *graph){
  observables.push_front(graph);
}

void GraphObserver::removeObservable(ObservableGraph *graph){
  slist<ObservableGraph*>::iterator itObs = observables.begin();
  slist<ObservableGraph*>::iterator ite = observables.end();
  while(itObs!=ite){
    if(graph == (*itObs)){
      observables.erase(itObs);
      return;
    }
    ++itObs;
  }
}

void ObservableGraph::addGraphObserver(GraphObserver *obs) const {
  // ensure obs does not already exists in observers
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    if (obs == (*itObs))
      return;
    ++itObs;
  }
  observers.push_front(obs); 
  obs->addObservable((ObservableGraph *)this);
}

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
  // use a copy to avoid the invalidation of the iterator
  // when an observer remove itself from the list
  slist<GraphObserver*> copy(observers);
  slist<GraphObserver*>::iterator itObs = copy.begin();
  slist<GraphObserver*>::iterator ite = copy.end();
  while (itObs != ite) {
    (*itObs)->destroy(sg);
    ++itObs;
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

void ObservableGraph::notifyBeforeSetAttribute(Graph *g, const std::string& name) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->beforeSetAttribute(g, name);
  }
}

void ObservableGraph::notifyAfterSetAttribute(Graph *g, const std::string& name) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->afterSetAttribute(g, name);
  }
}

void ObservableGraph::notifyRemoveAttribute(Graph *g, const std::string& name) {
  slist<GraphObserver*>::iterator itObs = observers.begin();
  slist<GraphObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    GraphObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->removeAttribute(g, name);
  }
}

void ObservableGraph::removeGraphObservers() {
  for(stdext::slist<GraphObserver*>::iterator it=observers.begin();it!=observers.end();++it){
    (*it)->removeObservable(this);
  }
  observers.clear();
}
