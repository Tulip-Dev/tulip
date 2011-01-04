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
#ifndef GRAPHOBSERVABLE_H
#define GRAPHOBSERVABLE_H
#include <list>
#include <algorithm>

#include "tulip/Node.h"
#include "tulip/Edge.h"

namespace tlp {

class Graph;
class PropertyInterface;
class ObservableGraph;
//=========================================================

/** \addtogroup graphs */
/*@{*/
/**
 * \class GraphObserver
 * \brief That class enables to receive a message (call back/handler function)
 * after each modification of a Graph.
 *
 * To receive a call back from an ObservableGraph, the GraphObserver must
 * first connect to the ObservableGraph by calling the addGraphObserver
 * function of an ObservableGraph
 *
 * One must implement necessary function in his/her class that inherits
 * from GraphObserver according to which message one wants to receive.
 *
 * If manageObservables is set to true, the GraphObserver will automatically
 * unregister from an ObservableGraph when deleted. (ie, unregistering is done
 * automatically)
 *
 * The Observer pattern is described pp293-304 of Design Patterns Book by Gamma, Helm, Johnson, and Vlissides.
 *
 * \example This is a small example of an observer that display a message on the standard output each time a
 * a node is added in a graph.
 * \code
 * class PrintObs : public GraphObserver {
 *    void addNode(Graph *g, const node n) {
 *       cout << "node " << n.id << " added in" << g << endl << flush;
 *    }
 * };
 *
 * int main() {
 *   Graph *g = tlp::newGraph();
 *   PrintObs obs;
 *   g->addObserver(&obs);
 *   g->addNode(); //output node 0 added in 0x
 *   g->addNode(); //output node 0 added in 0x
 * }
 * \endcode
 */
class TLP_SCOPE GraphObserver {
public:
	GraphObserver(bool manageObservables = true)
	:updateObservables(manageObservables) {}
	virtual ~GraphObserver();
	virtual void addNode(Graph *, const node ){}
	virtual void addEdge(Graph *, const edge ){}
	virtual void beforeSetEnds(Graph *, const edge){}
	virtual void afterSetEnds(Graph *, const edge){}
	virtual void delNode(Graph *,const node ){}
	virtual void delEdge(Graph *,const edge ){}
	virtual void reverseEdge(Graph *,const edge ){}
	virtual void destroy(Graph *){}
	virtual void addSubGraph(Graph *, Graph *){}
	virtual void delSubGraph(Graph *, Graph *){}
	virtual void addLocalProperty(Graph*, const std::string&){}
	virtual void delLocalProperty(Graph*, const std::string&){}
	virtual void beforeSetAttribute(Graph*, const std::string&) {}
	virtual void afterSetAttribute(Graph*, const std::string&) {}
	virtual void removeAttribute(Graph*, const std::string&) {}

	void addObservable(ObservableGraph *);
	void removeObservable(ObservableGraph *);
private:
	std::list<ObservableGraph *> observables;
	bool updateObservables;
};
/*@}*/
}

namespace tlp {

/** \addtogroup graphs */
/*@{*/
/**
 * \class ObservableGraph
 * \brief That class enables to easily implement several type of
 *        Observable graph.
 *
 * That class is used internally in Tulip to factorize the code of
 * Observable for all the graph implementation. End user can use it
 * if he/she wants to implement a new Graph Implementation (almost never).
 */
class  TLP_SCOPE ObservableGraph {
	friend class GraphObserver;

public:
	virtual ~ObservableGraph() {removeGraphObservers();}
	/**
	 * Register a new observer
	 */
	void addGraphObserver(GraphObserver *) const;
	/**
	 * Returns the number of observers
	 */
	unsigned int countGraphObservers();
	/**
	 * Remove an observer
	 */
	void removeGraphObserver(GraphObserver *) const;
	/**
	 * Remove all observers
	 */
	void removeGraphObservers();

protected:
	void notifyAddNode(Graph *,const node n);
	void notifyAddEdge(Graph *,const edge e);
	void notifyBeforeSetEnds(Graph *, const edge e);
	void notifyAfterSetEnds(Graph *,const edge e);
	void notifyDelNode(Graph *,const node n);
	void notifyDelEdge(Graph *,const edge e);
	void notifyReverseEdge(Graph *,const edge e);
	void notifyAddSubGraph(Graph*, Graph*);
	void notifyDelSubGraph(Graph*, Graph*);
	void notifyAddLocalProperty(Graph*, const std::string&);
	void notifyDelLocalProperty(Graph*, const std::string&);
	void notifyBeforeSetAttribute(Graph*, const std::string&);
	void notifyAfterSetAttribute(Graph*, const std::string&);
	void notifyRemoveAttribute(Graph*, const std::string&);
	void notifyDestroy(Graph*);
	void removeOnlyGraphObserver(GraphObserver *) const;
	mutable std::list<GraphObserver*> observers;
	mutable TLP_HASH_SET<GraphObserver*> observersSet;
};

inline unsigned int ObservableGraph::countGraphObservers() {
	return observers.size();
}


inline void ObservableGraph::removeGraphObserver(GraphObserver *item) const {
	if (!observers.empty() && observersSet.find(item) != observersSet.end()) {
		observers.remove(item);
		observersSet.erase(item);
		item->removeObservable(const_cast<ObservableGraph*>(this));
	}
}

inline void ObservableGraph::removeOnlyGraphObserver(GraphObserver *item) const {
	if (!observers.empty() && observersSet.find(item) != observersSet.end()) {
		observers.remove(item);
		observersSet.erase(item);
	}
}

}

#endif

