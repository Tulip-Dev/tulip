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
#include <algorithm>

#include <tulip/Observable.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

namespace tlp {

class Graph;
//=========================================================

/** \addtogroup graphs */
/*@{*/
/**
 * \class GraphObserver
 * \brief That class receives a message (call back/handler function)
 * after each modification of a Graph.
 *
 * To receive a notification from an ObservableGraph, the GraphObserver must
 * first be connected to the ObservableGraph.
 * This is done by calling ObservableGraph::addGraphObserver.
 *
 * Depending on which messages you want to receive, you need to override different functions of GraphObserver.
 *
 * If manageObservables is set to true, the GraphObserver will automatically
 * unregister from an ObservableGraph when deleted. (ie, unregistering is done
 * automatically)
 *
 * The Observer pattern is described pp293-304 of the book 'Design Patterns' by Gamma, Helm, Johnson, and Vlissides.
 *
 * \example PrintObserver This is a small example of an observer that displays a message on the standard output each time a
 * a node is added to a graph.
 * \code
 * class PrintObs : public GraphObserver {
 *    void addNode(Graph *g, const node n) {
 *       cout << "node " << n.id << " added in " << g << endl << flush;
 *    }
 * };
 *
 * int main() {
 *   Graph *g = tlp::newGraph();
 *   PrintObs obs;
 *   g->addGraphObserver(&obs);
 *   g->addNode(); //output node 0 added in 0x
 *   g->addNode(); //output node 0 added in 0x
 * }
 * \endcode
 */
class TLP_SCOPE GraphObserver {
  friend class Graph;
  // this class is for import of the observation graph
  friend class Observergraph;

private:
  class TLP_SCOPE RealGraphObserver :public Observable {
  public:
    GraphObserver* visibleObserver;
    RealGraphObserver(GraphObserver *observer) :visibleObserver(observer) {}
    virtual void treatEvent(const Event &);
  };

  RealGraphObserver hiddenObserver;

public:
  GraphObserver() :hiddenObserver(this) {}
  virtual ~GraphObserver(){}
  // no copy for hiddenObserver
  GraphObserver(const GraphObserver&) :hiddenObserver(this) {}
  GraphObserver& operator=(const GraphObserver &) {
    return *this;
  }

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
  virtual void addInheritedProperty(Graph*, const std::string&){}
  virtual void delInheritedProperty(Graph*, const std::string&){}
  virtual void beforeSetAttribute(Graph*, const std::string&) {}
  virtual void afterSetAttribute(Graph*, const std::string&) {}
  virtual void removeAttribute(Graph*, const std::string&) {}
};
/*@}*/
}

#endif

