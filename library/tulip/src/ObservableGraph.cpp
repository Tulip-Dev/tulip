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
#include "tulip/ObservableGraph.h"

using namespace stdext;
using namespace tlp;

GraphObserver::~GraphObserver(){
	/*#ifndef NDEBUG
  if(observables.size()!=0)
    std::cerr << "Delete a graph observer without remove it from observable list" << std::endl;
#endif*/
	if (!observables.empty()) {
		for(std::list<ObservableGraph *>::iterator it=observables.begin();it!=observables.end();++it){
			(*it)->removeOnlyGraphObserver(this);
		}
	}
}

void GraphObserver::addObservable(ObservableGraph *graph){
	if (updateObservables) {
		observables.push_front(graph);
	}
}

void GraphObserver::removeObservable(ObservableGraph *graph) {
	if (updateObservables) {
		observables.remove(graph);
	}
}

void ObservableGraph::addGraphObserver(GraphObserver *obs) const {
	// ensure obs does not already exists in observers
	if (observersSet.find(obs) == observersSet.end()) {
		observers.push_front(obs);
		observersSet.insert(obs);
		obs->addObservable(const_cast<ObservableGraph*>(this));
	}
}

void ObservableGraph::notifyAddNode(Graph *sg, const node n) {
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
	while (itObs != ite) {
		GraphObserver* observer = *itObs;
		// iterator is incremented before
		// to ensure it will not be invalidated
		// during the call to the method on the observer
		++itObs;
		observer->addEdge(sg, e);
	}
}

void ObservableGraph::notifyBeforeSetEnds(Graph *sg, const edge e) {
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
	while (itObs != ite) {
		GraphObserver* observer = *itObs;
		// iterator is incremented before
		// to ensure it will not be invalidated
		// during the call to the method on the observer
		++itObs;
		observer->beforeSetEnds(sg, e);
	}
}

void ObservableGraph::notifyAfterSetEnds(Graph *sg, const edge e) {
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
	while (itObs != ite) {
		GraphObserver* observer = *itObs;
		// iterator is incremented before
		// to ensure it will not be invalidated
		// during the call to the method on the observer
		++itObs;
		observer->afterSetEnds(sg, e);
	}
}

void ObservableGraph::notifyDelEdge(Graph *sg, const edge e) {
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	// use a copy to avoid the invalidation of the iterator
	// when an observer remove itself from the list
	std::list<GraphObserver*> copy(observers);
	std::list<GraphObserver*>::iterator itObs = copy.begin();
	std::list<GraphObserver*>::iterator ite = copy.end();
	while (itObs != ite) {
		(*itObs)->destroy(sg);
		++itObs;
	}
}

void ObservableGraph::notifyAddSubGraph(Graph *g, Graph *sg) {
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	std::list<GraphObserver*>::iterator itObs = observers.begin();
	std::list<GraphObserver*>::iterator ite = observers.end();
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
	if (observers.empty())
		return;
	for(std::list<GraphObserver*>::iterator it=observers.begin();it!=observers.end();++it){
		(*it)->removeObservable(this);
	}
	observers.clear();
	observersSet.clear();
}
