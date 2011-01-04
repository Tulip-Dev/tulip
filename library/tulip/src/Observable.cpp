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
#include <cassert>
#include <iostream>
#include "tulip/Observable.h"


using namespace std;
using namespace tlp;

#ifdef _WIN32 
#ifdef DLL_EXPORT
unsigned int Observable::holdCounter=0;
ObserverMap Observable::holdObserverMap;
ObservableMap Observable::holdObservableMap;
#endif
#else
unsigned int Observable::holdCounter=0;
ObserverMap Observable::holdObserverMap;
ObservableMap Observable::holdObservableMap;
#endif

static bool unholdLock=false;

Observer::~Observer(){
	/*#ifndef NDEBUG
  if(observables.size()!=0)
    std::cerr << "Delete an observer without remove it from observable list" << std::endl;
#endif*/
	if (!observables.empty()) {
		for(std::list<Observable *>::iterator it=observables.begin();it!=observables.end();++it){
			(*it)->removeOnlyObserver(this);
		}
	}
}

void Observer::addObservable(Observable *observable) {
	if (updateObservables) {
		observables.push_front(observable);
	}
}

void Observer::removeObservable(Observable *observable){
	if (updateObservables) {
		observables.remove(observable);
	}
}

void Observable::addObserver(Observer *obs) {
	// ensure obs does not already exists in observersList
	if (observersSet.find(obs) == observersSet.end()) {
		observersList.push_front(obs);
		observersSet[obs] = observersList.begin();
		obs->addObservable(const_cast<Observable*>(this));
	}
}

//===============================================================
void Observable::notifyDestroy() {
	//  cerr << "Observable::notifyObservers" << endl;
	if (observersList.empty())
		return;
	std::list<Observer*>::iterator itlObs = observersList.begin();
	std::list<Observer*>::iterator itle = observersList.end();
	while (itlObs != itle) {
		Observer* observer = *itlObs;
		// iterator is incremented before
		// to ensure it will not be invalidated
		// during the call to the observableDestroyed method
		++itlObs;
		observer->observableDestroyed(this);
		if (holdCounter) {
			// remove this from registered sets of Observable in holdObserverMap
			ObservableMap::iterator ito = holdObservableMap.find(this);
			if (ito != holdObservableMap.end()) {
				set<Observer*>::iterator itob = (*ito).second.begin();
				set<Observer*>::iterator itoe = (*ito).second.end();
				while(itob != itoe) {
#ifdef NDEBUG
					holdObserverMap[*itob].erase(this);
#else
					assert(holdObserverMap[*itob].erase(this) == 1);
#endif
					++itob;
				}
				holdObservableMap.erase(ito);
			}
		}
	}
}
//===============================================================
void Observable::notifyObservers() {
	if (observersList.empty())
		return;
	std::list<Observer*>::iterator itlObs = observersList.begin();
	std::list<Observer*>::iterator itle = observersList.end();
	if (itlObs == itle)
		return;

	if (unholdLock) {
		cerr << "Cannot notifyObservers during unholdings" << endl;
		return;
	}
	//  cerr << "Observable::notifyObservers" << endl;
	if (holdCounter)
		for (;itlObs != itle; ++itlObs) {
			holdObserverMap[*itlObs].insert(this);
			holdObservableMap[this].insert(*itlObs);
		}
	else {
		set<Observable *> tmpSet;
		tmpSet.insert(this);
		while(itlObs != itle) {
			Observer* observer = *itlObs;
			// iterator is incremented before
			// to ensure it will not be invalidated
			// during the call to the update method
			++itlObs;
			observer->update(tmpSet.begin(),tmpSet.end());
		}
	}
}
//===============================================================
void Observable::holdObservers() {
	//  cerr << __PRETTY_FUNCTION__ << " :=> " << holdCounter << endl;
	holdCounter++;
}
//===============================================================
void Observable::unholdObservers(bool force) {
	//  cerr << __PRETTY_FUNCTION__ << " :=> " << holdCounter << endl;

	if (force) {
		unholdLock=true;
		holdObservableMap.clear();
		ObserverMap tmp(holdObserverMap);
		holdObserverMap.clear();
		ObserverMap::iterator itMObs;
		for (itMObs=tmp.begin();itMObs!=tmp.end();++itMObs) {
			std::set<Observable *>::iterator begin = itMObs->second.begin();
			std::set<Observable *>::iterator end = itMObs->second.end();
			if (begin != end)
				itMObs->first->update(begin, end);
		}
		unholdLock=false;
		holdCounter = 0;
	}

	if (holdCounter == 0) return;
	holdCounter--;
	if (unholdLock) return;
	unholdLock=true;
	if (holdCounter==0) {
		holdObservableMap.clear();
		ObserverMap tmp(holdObserverMap);
		holdObserverMap.clear();
		ObserverMap::iterator itMObs;
		for (itMObs=tmp.begin();itMObs!=tmp.end();++itMObs) {
			std::set<Observable *>::iterator begin = itMObs->second.begin();
			std::set<Observable *>::iterator end = itMObs->second.end();
			if (begin != end)
				itMObs->first->update(begin, end);
		}
	}
	unholdLock=false;
}
//===============================================================
void Observable::removeObservers() {
	if (observersList.empty())
		return;
	for(std::list<Observer*>::iterator it=observersList.begin();it!=observersList.end();++it){
		(*it)->removeObservable(this);
	}
	observersList.clear();
	observersSet.clear();
}

