/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
	if (!observables.empty()) {
		for(std::list<ObservableProperty *>::iterator it=observables.begin();it!=observables.end();++it){
			(*it)->removeOnlyPropertyObserver(this);
		}
	}
}

void PropertyObserver::addObservable(ObservableProperty *property){
	if (updateObservables) {
		observables.push_front(property);
	}
}

void PropertyObserver::removeObservable(ObservableProperty *property) {
	if (updateObservables) {
		observables.remove(property);
	}
}

void ObservableProperty::addPropertyObserver(PropertyObserver *obs) const {
	// ensure obs does not already exists in observers
	if (observersSet.find(obs) == observersSet.end()) {
		observers.push_front(obs);
		observersSet[obs] = observers.begin();
		obs->addObservable((ObservableProperty*)this);
	}
}

void ObservableProperty::notifyBeforeSetNodeValue(PropertyInterface* p,
		const node n) {
	if (observers.empty())
		return;
	std::list<PropertyObserver*>::iterator itObs = observers.begin();
	std::list<PropertyObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<PropertyObserver*>::iterator itObs = observers.begin();
	std::list<PropertyObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<PropertyObserver*>::iterator itObs = observers.begin();
	std::list<PropertyObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<PropertyObserver*>::iterator itObs = observers.begin();
	std::list<PropertyObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<PropertyObserver*>::iterator itObs = observers.begin();
	std::list<PropertyObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<PropertyObserver*>::iterator itObs = observers.begin();
	std::list<PropertyObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<PropertyObserver*>::iterator itObs = observers.begin();
	std::list<PropertyObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<PropertyObserver*>::iterator itObs = observers.begin();
	std::list<PropertyObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<PropertyObserver*> copy(observers);
	std::list<PropertyObserver*>::iterator itObs = copy.begin();
	std::list<PropertyObserver*>::iterator ite = copy.end();
	while (itObs != ite) {
		(*itObs)->destroy(p);
		++itObs;
	}
}

void ObservableProperty::removePropertyObservers() {
	if (observers.empty())
		return;
	for(std::list<PropertyObserver*>::iterator it=observers.begin();it!=observers.end();++it){
		(*it)->removeObservable(this);
	}
	observers.clear();
	observersSet.clear();
}


