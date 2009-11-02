#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableProperty.h"

using namespace stdext;
using namespace tlp;

PropertyObserver::~PropertyObserver(){
/*#ifndef NDEBUG
  if(observables.size()!=0)
    std::cerr << "Delete a property observer without remove it from observable list" << std::endl;
#endif*/
  for(slist<ObservableProperty *>::iterator it=observables.begin();it!=observables.end();++it){
    (*it)->removeOnlyPropertyObserver(this);
  }
}

void PropertyObserver::addObservable(ObservableProperty *property){
  observables.push_front(property);
}

void PropertyObserver::removeObservable(ObservableProperty *property){
  slist<ObservableProperty*>::iterator itObs = observables.begin();
  slist<ObservableProperty*>::iterator ite = observables.end();
  while(itObs!=ite){
    if(property == (*itObs)){
      observables.erase(itObs);
      return;
    }
    ++itObs;
  }
}

void ObservableProperty::addPropertyObserver(PropertyObserver *obs) const {
  // ensure obs does not already exists in observers
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    if (obs == (*itObs))
      return;
    ++itObs;
  }
  observers.push_front(obs);
  obs->addObservable((ObservableProperty*)this);
}

void ObservableProperty::notifyBeforeSetNodeValue(PropertyInterface* p,
						  const node n) {
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->beforeSetNodeValue(p, n);
  }
}

void ObservableProperty::notifyAfterSetNodeValue(PropertyInterface* p,
						 const node n) {
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->afterSetNodeValue(p, n);
  }
}

void ObservableProperty::notifyBeforeSetEdgeValue(PropertyInterface* p,
					    const edge e) {
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->beforeSetEdgeValue(p, e);
  }
}

void ObservableProperty::notifyAfterSetEdgeValue(PropertyInterface* p,
						 const edge e) {
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->afterSetEdgeValue(p, e);
  }
}

void ObservableProperty::notifyBeforeSetAllNodeValue(PropertyInterface* p) {
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->beforeSetAllNodeValue(p);
  }
}

void ObservableProperty::notifyAfterSetAllNodeValue(PropertyInterface* p) {
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->afterSetAllNodeValue(p);
  }
}

void ObservableProperty::notifyBeforeSetAllEdgeValue(PropertyInterface* p) {
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->beforeSetAllEdgeValue(p);
  }
}

void ObservableProperty::notifyAfterSetAllEdgeValue(PropertyInterface* p) {
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->afterSetAllEdgeValue(p);
  }
}

void ObservableProperty::notifyDestroy(PropertyInterface* p) {
  // use a copy to avoid the invalidation of the iterator
  // when an observer remove itself from the list
  slist<PropertyObserver*> copy(observers);
  slist<PropertyObserver*>::iterator itObs = copy.begin();
  slist<PropertyObserver*>::iterator ite = copy.end();
  while (itObs != ite) {
    (*itObs)->destroy(p);
    ++itObs;
  }
}

