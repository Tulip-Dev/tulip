#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableProperty.h"

using namespace stdext;
using namespace tlp;

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
  slist<PropertyObserver*>::iterator itObs = observers.begin();
  slist<PropertyObserver*>::iterator ite = observers.end();
  while (itObs != ite) {
    PropertyObserver* observer = *itObs;
    // iterator is incremented before
    // to ensure it will not be invalidated
    // during the call to the method on the observer
    ++itObs;
    observer->destroy(p);
  }
}

