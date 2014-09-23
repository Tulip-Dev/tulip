/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/ForEach.h>
#include "ObservableGraphTest.h"
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION( ObservableGraphTest );

tlp::Graph* ObservableGraphTest::graph = NULL;

// these classes will capture
// everything that will happen to our properties
// synchronously or asynchronously
class ObserverGTest :public Observable {
public:
  std::set<Observable*> observables;
  bool print;

  ObserverGTest() :print(false) {}

  void reset() {
    observables.clear();
  }

  unsigned int nbObservables() {
    return observables.size();
  }

  bool found(Observable *obs) {
    return observables.find(obs) != observables.end();
  }

  // Observable methods
  void treatEvents(const vector<Event> &events) {
    if (events[0].type() == Event::TLP_DELETE) {
      observables.insert(events[0].sender());
    }
    else {
      for (unsigned int i=0; i<events.size(); ++i) {
        observables.insert(events[i].sender());
      }
    }
  }
};

static ObserverGTest* observer;

class PropertiesDeletedObserver :public Observable {
public:
  PropertiesDeletedObserver():inheritedPropertyExist(false),initialized(false) {

  }
  bool inheritedPropertyExist;
  bool initialized;

  vector<PropertyInterface*> _inheritedProperties;
  vector<PropertyInterface*> _localProperties;
  void beforeDelLocalProperty(Graph *g, const std::string &pName) {
    CPPUNIT_ASSERT(g->existLocalProperty(pName));
    beforeDelLocalPropertyCalledGraphs.push_back(g);
  }

  void afterDelInheritedProperty(Graph *g, const std::string&) {
    afterDelInheritedPropertyCalledGraphs.push_back(g);
  }

  void afterDelLocalProperty(Graph *g, const std::string &pName) {
    CPPUNIT_ASSERT(!g->existLocalProperty(pName));
    afterDelLocalPropertyCalledGraphs.push_back(g);
  }

  void beforeDelInheritedProperty(Graph *g, const std::string &pName) {
    CPPUNIT_ASSERT(!g->existLocalProperty(pName));
    CPPUNIT_ASSERT(g->existProperty(pName));
    beforeDelInheritedPropertyCalledGraphs.push_back(g);
  }

  void addLocalProperty(Graph *g, const std::string & pName) {
    CPPUNIT_ASSERT(g->existLocalProperty(pName));
    addLocalPropertyCalledGraphs.push_back(g);
  }

  void addInheritedProperty(Graph *g, const std::string &pName) {
    CPPUNIT_ASSERT(!g->existLocalProperty(pName));
    CPPUNIT_ASSERT(g->existProperty(pName));
    addInheritedPropertyCalledGraphs.push_back(g);
  }

  virtual void treatEvent(const Event& evt) {

    const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

    if (gEvt) {
      Graph* graph = gEvt->getGraph();

      switch (gEvt->getType()) {
      case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY:
        beforeDelLocalProperty(graph, gEvt->getPropertyName());
        return;

      case GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY:
        afterDelInheritedProperty(graph, gEvt->getPropertyName());
        return;

      case GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY:
        afterDelLocalProperty(graph, gEvt->getPropertyName());
        return;

      case GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY:
        beforeDelInheritedProperty(graph, gEvt->getPropertyName());
        return;

      case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
        addLocalProperty(graph, gEvt->getPropertyName());
        return;

      case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
        addInheritedProperty(graph, gEvt->getPropertyName());
        return;

      default:
        break;
      }
    }
  }


  vector<Graph*> beforeDelLocalPropertyCalledGraphs;
  vector<Graph*> beforeDelInheritedPropertyCalledGraphs;
  vector<Graph*> afterDelLocalPropertyCalledGraphs;
  vector<Graph*> afterDelInheritedPropertyCalledGraphs;

  vector<Graph*> addInheritedPropertyCalledGraphs;
  vector<Graph*> addLocalPropertyCalledGraphs;
};

class GraphObserverTest :public Observable {
public:

  vector<Graph*> graphs;
  vector<Graph*> sGraphs;
  vector<node> nodes;
  vector<edge> edges;
  string pName;
  string spName;
  bool deleteBug747;

  GraphObserverTest() {
    deleteBug747 = false;
  }

  Graph* getObservedGraph() {
    assert(graphs.size() == 1);
    return graphs[0];
  }

  vector<Graph*>& getObservedGraphs() {
    return graphs;
  }

  Graph* getObservedSubgraph() {
    assert(sGraphs.size() == 1);
    return sGraphs[0];
  }

  vector<Graph*>& getObservedSubgraphs() {
    return sGraphs;
  }

  node getObservedNode() {
    assert(nodes.size() == 1);
    return nodes[0];
  }

  vector<node>& getObservedNodes() {
    return nodes;
  }

  edge getObservedEdge() {
    assert(edges.size() == 1);
    return edges[0];
  }

  vector<edge>& getObservedEdges() {
    return edges;
  }

  string& getLocalPropertyName() {
    return pName;
  }
  string& getInheritedPropertyName() {
    return spName;
  }

  void reset() {
    graphs.clear(), sGraphs.clear();
    nodes.clear(), edges.clear(), pName.clear(), spName.clear();
  }

  void addNode(Graph* g, const node n) {
    graphs.push_back(g), nodes.push_back(n);
  }
  void addEdge(Graph* g, edge e) {
    graphs.push_back(g), edges.push_back(e);
  }
  void delNode(Graph* g, const node n) {
    graphs.push_back(g), nodes.push_back(n);
  }
  void delEdge(Graph* g, edge e) {
    graphs.push_back(g), edges.push_back(e);
  }
  void reverseEdge(Graph* g, edge e) {
    graphs.push_back(g), edges.push_back(e);
  }
  void destroy(Graph* g) {
    graphs.push_back(g);
  }
  void addSubGraph(Graph* g, Graph* sg) {
    graphs.push_back(g), sGraphs.push_back(sg);
  }
  void delSubGraph(Graph* g, Graph* sg) {
    graphs.push_back(g), sGraphs.push_back(sg);
  }
  void addLocalProperty(Graph* g, const string& name) {
    graphs.push_back(g), pName = name;
  }
  void delLocalProperty(Graph* g, const string& name) {
    graphs.push_back(g), pName = name;
  }
  void addInheritedProperty(Graph* g, const string& name) {
    sGraphs.push_back(g), spName = name;
  }
  void afterDelInheritedProperty(Graph* g, const string& name) {
    sGraphs.push_back(g), spName = name;
  }
  virtual void treatEvent(const Event& evt) {
    const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

    if (gEvt) {
      Graph* graph = gEvt->getGraph();

      switch (gEvt->getType()) {
      case GraphEvent::TLP_ADD_NODE:
	addNode(graph, gEvt->getNode());
	break;

      case GraphEvent::TLP_DEL_NODE:
	delNode(graph, gEvt->getNode());
	break;

      case GraphEvent::TLP_ADD_EDGE:
	addEdge(graph, gEvt->getEdge());
	break;

      case GraphEvent::TLP_DEL_EDGE:
	delEdge(graph, gEvt->getEdge());
	break;

      case GraphEvent::TLP_REVERSE_EDGE:
	reverseEdge(graph, gEvt->getEdge());
	break;

      case GraphEvent::TLP_AFTER_ADD_SUBGRAPH:
	addSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
	break;

      case GraphEvent::TLP_AFTER_DEL_SUBGRAPH:
	delSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
	break;

      case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
	addLocalProperty(graph, gEvt->getPropertyName());
	break;

      case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY:
	delLocalProperty(graph, gEvt->getPropertyName());
	break;

      case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
        addInheritedProperty(graph, gEvt->getPropertyName());
        return;

      case GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY:
        afterDelInheritedProperty(graph, gEvt->getPropertyName());
        return;

      /*case GraphEvent::TLP_BEFORE_SET_ENDS:
	beforeSetEnds(graph, gEvt->getEdge());
	break;

      case GraphEvent::TLP_AFTER_SET_ENDS:
	afterSetEnds(graph, gEvt->getEdge());
	break;

      case GraphEvent::TLP_ADD_NODES: {
	const std::vector<node>& nodes = gEvt->getNodes();

	for (unsigned int i = 0; i < nodes.size(); ++i)
	  addNode(graph, nodes[i]);

	break;
      }

      case GraphEvent::TLP_ADD_EDGES: {
	const std::vector<edge>& edges = gEvt->getEdges();

	for (unsigned int i = 0; i < edges.size(); ++i)
	  addEdge(graph, edges[i]);

	break;
      }

      case GraphEvent::TLP_BEFORE_SET_ATTRIBUTE:
	beforeSetAttribute(graph, gEvt->getAttributeName());
	break;

      case GraphEvent::TLP_AFTER_SET_ATTRIBUTE:
	afterSetAttribute(graph, gEvt->getAttributeName());
	break;

      case GraphEvent::TLP_REMOVE_ATTRIBUTE:
	removeAttribute(graph, gEvt->getAttributeName());

      case GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY:
      case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
      case GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY:
      case GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY:
      case GraphEvent::TLP_BEFORE_ADD_DESCENDANTGRAPH:
      case GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH:
      case GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH:
      case GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH:
      case GraphEvent::TLP_BEFORE_ADD_SUBGRAPH:
      case GraphEvent::TLP_BEFORE_DEL_SUBGRAPH:
      case GraphEvent::TLP_BEFORE_ADD_LOCAL_PROPERTY:
      case GraphEvent::TLP_BEFORE_ADD_INHERITED_PROPERTY:
      break;*/

      default:
        break;
      }
    }
    else {
      Graph* graph =
	// From my point of view the use of dynamic_cast should be correct
	// but it fails, so I use reinterpret_cast (pm)
	reinterpret_cast<Graph *>(evt.sender());

      if (graph && evt.type() == Event::TLP_DELETE) {
	if (deleteBug747) {
	  delete observer;
	  GraphObserverTest* obs = new GraphObserverTest();
	  addListener(obs);
	  observer = NULL;
	  ObservableGraphTest::setGraph(NULL);
	}
	else
	  destroy(graph);
      }
    }
  }
};

class DelInheritedPropertyObserverTest :public Observable {
public:
  DelInheritedPropertyObserverTest():inheritedPropertyExist(false),initialized(false) {

  }
  bool inheritedPropertyExist;
  bool initialized;
  void delInheritedProperty(Graph* g, const string& name) {
    if(!initialized) {
      inheritedPropertyExist = g->existProperty(name);

      if(inheritedPropertyExist) {
        g->getProperty(name)->getName();
      }

      initialized = true;
    }
    else {
      inheritedPropertyExist = inheritedPropertyExist & (g->getProperty(name)!=NULL);

      if(inheritedPropertyExist) {
        g->getProperty(name)->getName();
      }
    }
  }
  virtual void treatEvent(const Event& evt) {
    const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

    if (gEvt) {
      Graph* graph = gEvt->getGraph();

      if (gEvt->getType() == GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY)
        delInheritedProperty(graph, gEvt->getPropertyName());
    }
  }
};

static GraphObserverTest* gObserver;

//==========================================================
void ObservableGraphTest::setUp() {
  graph = tlp::newGraph();
  gObserver = new GraphObserverTest();
  observer = new ObserverGTest();
  graph->addListener(gObserver);
  graph->addObserver(observer);
}
//==========================================================
void ObservableGraphTest::tearDown() {
  delete graph;
  delete gObserver;
  delete observer;
}
//==========================================================
void ObservableGraphTest::testAddDel() {
  //  qWarning() << __PRETTY_FUNCTION__ << endl;
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_NODES = 2;

  for (unsigned int i=0; i<NB_NODES; ++i) {
    gObserver->reset();
    observer->reset();
    nodes.push_back(graph->addNode());
    CPPUNIT_ASSERT_EQUAL(nodes[i], gObserver->getObservedNode());
    CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
    CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
    CPPUNIT_ASSERT(observer->found(graph));
  }

  for (unsigned int i=0; i< NB_NODES; ++i) {
    gObserver->reset();
    observer->reset();
    edges.push_back(graph->addEdge(nodes[i],
                                   (i == NB_NODES - 1)
                                   ? nodes[0] : nodes[i]));
    CPPUNIT_ASSERT_EQUAL(edges[i], gObserver->getObservedEdge());
    CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
    CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
    CPPUNIT_ASSERT(observer->found(graph));
  }

  for (unsigned int i=0; i < NB_NODES / 2; ++i) {
    gObserver->reset();
    observer->reset();
    graph->delEdge(edges[2 * i]);
    CPPUNIT_ASSERT_EQUAL(edges[2 * i], gObserver->getObservedEdge());
    CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
    CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
    CPPUNIT_ASSERT(observer->found(graph));
  }

  for (unsigned int i=0; i < NB_NODES; ++i) {
    gObserver->reset();
    observer->reset();
    graph->delNode(nodes[i]);
    CPPUNIT_ASSERT_EQUAL(nodes[i], gObserver->getObservedNode());

    if (i == 0 || (i%2 && i != (NB_NODES - 1))) {
      vector<Graph*>& graphs = gObserver->getObservedGraphs();
      CPPUNIT_ASSERT_EQUAL(size_t(2), graphs.size());
      CPPUNIT_ASSERT_EQUAL(graph, graphs[0]);
      CPPUNIT_ASSERT_EQUAL(graph, graphs[1]);

      if (i == 0)
        CPPUNIT_ASSERT_EQUAL(edges[NB_NODES - 1], gObserver->getObservedEdge());
      else
        CPPUNIT_ASSERT_EQUAL(edges[i], gObserver->getObservedEdge());
    }
    else {
      CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
    }

    CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
    CPPUNIT_ASSERT(observer->found(graph));
  }
}
//==========================================================
void ObservableGraphTest::testClear() {
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_NODES = 100;

  for (unsigned int i=0; i<NB_NODES; ++i) {
    nodes.push_back(graph->addNode());
  }

  for (unsigned int i=0; i< NB_NODES; ++i) {
    edges.push_back(graph->addEdge(nodes[i],
                                   (i == NB_NODES - 1)
                                   ? nodes[i] : nodes[i + 1]));
  }

  gObserver->reset();
  observer->reset();

  graph->clear();

  vector<node>& oNodes = gObserver->getObservedNodes();
  CPPUNIT_ASSERT_EQUAL(size_t(NB_NODES), oNodes.size());

  for(unsigned int i = 0; i < NB_NODES; ++i) {
    CPPUNIT_ASSERT_EQUAL(oNodes[i], nodes[i]);
  }

  vector<edge>& oEdges = gObserver->getObservedEdges();
  CPPUNIT_ASSERT_EQUAL(size_t(NB_NODES), oEdges.size());

  for(unsigned int i = 0; i < NB_NODES; ++i) {
    CPPUNIT_ASSERT(oEdges[i] == edges[i]);
  }

  CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
}
//==========================================================
void ObservableGraphTest::testReverse() {
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_NODES = 100;

  for (unsigned int i=0; i<NB_NODES; ++i) {
    nodes.push_back(graph->addNode());
  }

  for (unsigned int i=0; i< NB_NODES; ++i) {
    edges.push_back(graph->addEdge(nodes[i],
                                   (i == NB_NODES - 1)
                                   ? nodes[0] : nodes[i + 1]));
  }

  for (unsigned int i=0; i< NB_NODES; ++i) {
    gObserver->reset();
    observer->reset();
    graph->reverse(edges[i]);
    CPPUNIT_ASSERT_EQUAL(gObserver->getObservedEdge(), edges[i]);
    CPPUNIT_ASSERT_EQUAL(gObserver->getObservedGraph(), graph);
    CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
    CPPUNIT_ASSERT(observer->found(graph));
  }

  // sub graph test
  Graph* g1 = graph->addSubGraph();
  g1->addNode(nodes[0]);
  g1->addNode(nodes[1]);
  g1->addEdge(edges[0]);
  gObserver->reset();
  observer->reset();
  g1->addListener(gObserver);
  g1->addObserver(observer);
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  CPPUNIT_ASSERT_EQUAL(size_t(0), gObserver->getObservedGraphs().size());
  g1->reverse(edges[0]);
  // 2 calls only to reverseEdge & update
  CPPUNIT_ASSERT_EQUAL(size_t(2), gObserver->getObservedEdges().size());
  CPPUNIT_ASSERT_EQUAL(size_t(2), gObserver->getObservedGraphs().size());
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
}
//==========================================================
void ObservableGraphTest::testSubgraph() {
  Graph *g1, *g2, *g3, *g4;

  gObserver->reset();
  observer->reset();
  g1 = graph->addSubGraph();
  CPPUNIT_ASSERT_EQUAL(g1, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  gObserver->reset();
  observer->reset();
  g2 = graph->addSubGraph();
  CPPUNIT_ASSERT_EQUAL(g2, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  gObserver->reset();
  observer->reset();
  CPPUNIT_ASSERT_EQUAL(size_t(0), gObserver->sGraphs.size());
  g2->addListener(gObserver);
  g2->addObserver(observer);
  g3 = g2->addSubGraph();
  CPPUNIT_ASSERT_EQUAL(g3, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(g2, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));
  gObserver->reset();
  observer->reset();
  g4 = g2->addSubGraph();
  CPPUNIT_ASSERT_EQUAL(g4, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(g2, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));

  gObserver->reset();
  observer->reset();
  g3->addListener(gObserver);
  g3->addObserver(observer);
  Observable::holdObservers();
  // 2 hold more for testing purpose
  Observable::holdObservers();
  Observable::holdObservers();
  node n1 = g3->addNode();
  vector<Graph*>& graphs = gObserver->getObservedGraphs();
  CPPUNIT_ASSERT_EQUAL(size_t(3), graphs.size());
  CPPUNIT_ASSERT_EQUAL(graph, graphs[0]);
  CPPUNIT_ASSERT_EQUAL(g2, graphs[1]);
  CPPUNIT_ASSERT_EQUAL(g3, graphs[2]);
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  CPPUNIT_ASSERT_EQUAL(3u, Observable::observersHoldCounter());
  // first unhold
  Observable::unholdObservers();
  // nothing happens
  CPPUNIT_ASSERT_EQUAL(2u, Observable::observersHoldCounter());
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  CPPUNIT_ASSERT(!observer->found(graph));
  CPPUNIT_ASSERT(!observer->found(g2));
  CPPUNIT_ASSERT(!observer->found(g3));
  // second unhold
  Observable::unholdObservers();
  CPPUNIT_ASSERT_EQUAL(1u, Observable::observersHoldCounter());
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  CPPUNIT_ASSERT(!observer->found(graph));
  CPPUNIT_ASSERT(!observer->found(g2));
  CPPUNIT_ASSERT(!observer->found(g3));
  // third unhold
  Observable::unholdObservers();
  CPPUNIT_ASSERT_EQUAL(0u, Observable::observersHoldCounter());
  CPPUNIT_ASSERT_EQUAL(3u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));
  CPPUNIT_ASSERT(observer->found(g3));
  vector<node>& nodes = gObserver->getObservedNodes();
  CPPUNIT_ASSERT_EQUAL(size_t(3), nodes.size());
  CPPUNIT_ASSERT_EQUAL(n1, nodes[0]);
  CPPUNIT_ASSERT_EQUAL(n1, nodes[1]);
  CPPUNIT_ASSERT_EQUAL(n1, nodes[2]);

  gObserver->reset();
  observer->reset();
  g4->addListener(gObserver);
  g4->addObserver(observer);
  node n2 = g4->addNode();
  CPPUNIT_ASSERT_EQUAL(size_t(3), graphs.size());
  CPPUNIT_ASSERT_EQUAL(graph, graphs[0]);
  CPPUNIT_ASSERT_EQUAL(g2, graphs[1]);
  CPPUNIT_ASSERT_EQUAL(g4, graphs[2]);
  CPPUNIT_ASSERT_EQUAL(3u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));
  CPPUNIT_ASSERT(observer->found(g4));
  CPPUNIT_ASSERT_EQUAL(size_t(3), nodes.size());
  CPPUNIT_ASSERT_EQUAL(n2, nodes[0]);
  CPPUNIT_ASSERT_EQUAL(n2, nodes[1]);
  CPPUNIT_ASSERT_EQUAL(n2, nodes[2]);

  gObserver->reset();
  observer->reset();
  Observable::holdObservers();
  edge e = g2->addEdge(n1, n2);
  CPPUNIT_ASSERT_EQUAL(size_t(2), graphs.size());
  CPPUNIT_ASSERT_EQUAL(graph, graphs[0]);
  CPPUNIT_ASSERT_EQUAL(g2, graphs[1]);
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  Observable::unholdObservers();
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));
  vector<edge>& edges = gObserver->getObservedEdges();
  CPPUNIT_ASSERT_EQUAL(size_t(2), edges.size());
  CPPUNIT_ASSERT_EQUAL(e, edges[0]);
  CPPUNIT_ASSERT_EQUAL(e, edges[1]);

  gObserver->reset();
  observer->reset();
  Observable::holdObservers();
  g2->delNode(n2);
  CPPUNIT_ASSERT_EQUAL(size_t(3), graphs.size());
  CPPUNIT_ASSERT_EQUAL(g2, graphs[0]);
  CPPUNIT_ASSERT_EQUAL(g4, graphs[1]);
  CPPUNIT_ASSERT_EQUAL(g2, graphs[2]);
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  Observable::unholdObservers();
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(g2));
  CPPUNIT_ASSERT(observer->found(g4));
  CPPUNIT_ASSERT_EQUAL(size_t(2), nodes.size());
  CPPUNIT_ASSERT_EQUAL(n2, nodes[0]);
  CPPUNIT_ASSERT_EQUAL(n2, nodes[1]);
  CPPUNIT_ASSERT_EQUAL(size_t(1), edges.size());
  CPPUNIT_ASSERT_EQUAL(e, edges[0]);

  gObserver->reset();
  observer->reset();
  g2->addNode(n2);
  CPPUNIT_ASSERT_EQUAL(graphs[0], gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(n2, gObserver->getObservedNode());
  CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(g2));
  gObserver->reset();
  observer->reset();
  g2->addEdge(e);
  CPPUNIT_ASSERT_EQUAL(graphs[0], gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(e, gObserver->getObservedEdge());
  CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(g2));

  gObserver->reset();
  observer->reset();
  Observable::holdObservers();
  g2->delNode(n2, true);
  CPPUNIT_ASSERT_EQUAL(size_t(4), graphs.size());
  CPPUNIT_ASSERT_EQUAL(graph, graphs[0]);
  CPPUNIT_ASSERT_EQUAL(g2, graphs[1]);
  CPPUNIT_ASSERT_EQUAL(g2, graphs[2]);
  CPPUNIT_ASSERT_EQUAL(graph, graphs[3]);
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  Observable::unholdObservers();
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));
  CPPUNIT_ASSERT_EQUAL(size_t(2), nodes.size());
  CPPUNIT_ASSERT_EQUAL(n2, nodes[0]);
  CPPUNIT_ASSERT_EQUAL(n2, nodes[1]);
  CPPUNIT_ASSERT_EQUAL(size_t(2), edges.size());
  CPPUNIT_ASSERT_EQUAL(e, edges[0]);
  CPPUNIT_ASSERT_EQUAL(e, edges[1]);
}
//==========================================================
void ObservableGraphTest::testDeleteSubgraph() {
  Graph *g2, *g3, *g4;

  graph->addSubGraph();
  g2 = graph->addSubGraph();
  g2->addListener(gObserver);
  g2->addObserver(observer);
  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  gObserver->reset();
  observer->reset();
  g2->delSubGraph(g3);
  CPPUNIT_ASSERT_EQUAL(g3, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(g2, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));

  gObserver->reset();
  observer->reset();
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  Observable::holdObservers();
  g2->delSubGraph(g4);
  CPPUNIT_ASSERT_EQUAL(g4, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(g2, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  Observable::unholdObservers();
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));

  gObserver->reset();
  observer->reset();
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  graph->delAllSubGraphs(g2);
  CPPUNIT_ASSERT_EQUAL(g2, gObserver->getObservedSubgraph());
  vector<Graph*>& graphs = gObserver->getObservedGraphs();
  CPPUNIT_ASSERT_EQUAL(graph, graphs[0]);
  CPPUNIT_ASSERT_EQUAL(g2, graphs[1]);
  CPPUNIT_ASSERT_EQUAL(2u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(observer->found(g2));

  observer->reset();
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  Observable::holdObservers();
  g2 = graph->addSubGraph();
  g2->addObserver(observer);
  g3 = g2->addSubGraph();
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  g3->addObserver(observer);
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  g2->delSubGraph(g3);
  CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(g3));
  observer->reset();
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  graph->delSubGraph(g2);
  CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(g2));
  observer->reset();
  CPPUNIT_ASSERT_EQUAL(0u, observer->nbObservables());
  Observable::unholdObservers();
  CPPUNIT_ASSERT_EQUAL(1u, observer->nbObservables());
  CPPUNIT_ASSERT(observer->found(graph));
  CPPUNIT_ASSERT(!observer->found(g2));
}
//==========================================================
void ObservableGraphTest::testAddDelProperties() {
  Graph *g1, *g2;

  CPPUNIT_ASSERT(gObserver->graphs.empty());
  CPPUNIT_ASSERT(gObserver->sGraphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getInheritedPropertyName());

  graph->getProperty<BooleanProperty>("test");
  CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(string("test"), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT(gObserver->sGraphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getInheritedPropertyName());

  gObserver->reset();
  CPPUNIT_ASSERT(gObserver->graphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT(gObserver->sGraphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getInheritedPropertyName());
  graph->delLocalProperty("test");
  CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(string("test"), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT(gObserver->sGraphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getInheritedPropertyName());

  g1 = graph->addSubGraph();
  g1->addListener(gObserver);
  gObserver->reset();

  graph->getProperty<BooleanProperty>("test");
  CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(string("test"), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT_EQUAL(g1, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(string("test"), gObserver->getInheritedPropertyName());

  gObserver->reset();
  CPPUNIT_ASSERT(gObserver->graphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT(gObserver->sGraphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getInheritedPropertyName());

  graph->delLocalProperty("test");
  CPPUNIT_ASSERT_EQUAL(graph, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(string("test"), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT_EQUAL(g1, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(string("test"), gObserver->getInheritedPropertyName());

  g2 = g1->addSubGraph();
  g2->addListener(gObserver);
  gObserver->reset();
  CPPUNIT_ASSERT(gObserver->graphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT(gObserver->sGraphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getInheritedPropertyName());

  g1->getProperty<BooleanProperty>("test1");
  CPPUNIT_ASSERT_EQUAL(g1, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(string("test1"), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT_EQUAL(g2, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(string("test1"), gObserver->getInheritedPropertyName());

  gObserver->reset();
  CPPUNIT_ASSERT(gObserver->graphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT(gObserver->sGraphs.empty());
  CPPUNIT_ASSERT_EQUAL(string(""), gObserver->getInheritedPropertyName());

  g1->delLocalProperty("test1");
  CPPUNIT_ASSERT_EQUAL(g1, gObserver->getObservedGraph());
  CPPUNIT_ASSERT_EQUAL(string("test1"), gObserver->getLocalPropertyName());
  CPPUNIT_ASSERT_EQUAL(g2, gObserver->getObservedSubgraph());
  CPPUNIT_ASSERT_EQUAL(string("test1"), gObserver->getInheritedPropertyName());
}
//==========================================================
void ObservableGraphTest::testObserverWhenRemoveObservable() {
  unsigned int listeners = graph->countListeners();
  GraphObserverTest *graphObserverTmp=new GraphObserverTest();
  graph->addListener(graphObserverTmp);
  CPPUNIT_ASSERT_EQUAL(listeners + 1, graph->countListeners());
  delete graphObserverTmp;
  CPPUNIT_ASSERT_EQUAL(listeners, graph->countListeners());
}

void ObservableGraphTest::testDelInheritedPropertyExistWhenDelInheritedPropertyIsSend() {
  Graph *g1 = graph->addSubGraph();
  DelInheritedPropertyObserverTest *observer = new DelInheritedPropertyObserverTest();
  g1->addListener(observer);
  graph->getLocalProperty<BooleanProperty>("test");
  graph->delLocalProperty("test");
  CPPUNIT_ASSERT(observer->initialized);
  CPPUNIT_ASSERT(observer->inheritedPropertyExist);
}

void ObservableGraphTest::testNotifyDelInheritedPropertyIsSendWhenLocalPropertyIsDeleted() {

  Graph* g1 = graph->addSubGraph();
  Graph* g2 = g1->addSubGraph();

  PropertiesDeletedObserver observer;

  graph->addListener(&observer);
  g1->addListener(&observer);
  g2->addListener(&observer);

  graph->getLocalProperty<BooleanProperty>("test");
  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs.empty());
  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs.empty());
  CPPUNIT_ASSERT(observer.afterDelLocalPropertyCalledGraphs.empty());
  CPPUNIT_ASSERT(observer.beforeDelLocalPropertyCalledGraphs.empty());

  CPPUNIT_ASSERT(observer.addLocalPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.addLocalPropertyCalledGraphs[0]==graph);
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs.size()==2);
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs[0]==g1);
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs[1]==g2);

  observer.afterDelInheritedPropertyCalledGraphs.clear();
  observer.beforeDelInheritedPropertyCalledGraphs.clear();
  observer.afterDelLocalPropertyCalledGraphs.clear();
  observer.beforeDelLocalPropertyCalledGraphs.clear();
  observer.addLocalPropertyCalledGraphs.clear();
  observer.addInheritedPropertyCalledGraphs.clear();

  graph->delLocalProperty("test");

  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs.size()==2);
  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs[0]==g1);
  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs[1]==g2);

  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs.size()==2);
  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs[0]==g1);
  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs[1]==g2);

  CPPUNIT_ASSERT(observer.afterDelLocalPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.afterDelLocalPropertyCalledGraphs[0]==graph);
  CPPUNIT_ASSERT(observer.beforeDelLocalPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.beforeDelLocalPropertyCalledGraphs[0]==graph);

  CPPUNIT_ASSERT(observer.addLocalPropertyCalledGraphs.empty());
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs.empty());

  graph->getLocalProperty<BooleanProperty>("test");

  observer.afterDelInheritedPropertyCalledGraphs.clear();
  observer.beforeDelInheritedPropertyCalledGraphs.clear();
  observer.afterDelLocalPropertyCalledGraphs.clear();
  observer.beforeDelLocalPropertyCalledGraphs.clear();
  observer.addLocalPropertyCalledGraphs.clear();
  observer.addInheritedPropertyCalledGraphs.clear();

  g1->getLocalProperty<BooleanProperty>("test");

  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs.size()==2);
  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs[0]==g1);
  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs[1]==g2);

  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs.size()==2);
  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs[0]==g1);
  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs[1]==g2);

  CPPUNIT_ASSERT(observer.beforeDelLocalPropertyCalledGraphs.empty());
  CPPUNIT_ASSERT(observer.afterDelLocalPropertyCalledGraphs.empty());

  CPPUNIT_ASSERT(observer.addLocalPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.addLocalPropertyCalledGraphs[0]==g1);
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs[0]==g2);

  graph->getLocalProperty<BooleanProperty>("test");
  g1->getLocalProperty<BooleanProperty>("test");

  observer.afterDelInheritedPropertyCalledGraphs.clear();
  observer.beforeDelInheritedPropertyCalledGraphs.clear();
  observer.afterDelLocalPropertyCalledGraphs.clear();
  observer.beforeDelLocalPropertyCalledGraphs.clear();
  observer.addLocalPropertyCalledGraphs.clear();
  observer.addInheritedPropertyCalledGraphs.clear();

  g1->delLocalProperty("test");

  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.afterDelInheritedPropertyCalledGraphs[0]==g2);

  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.beforeDelInheritedPropertyCalledGraphs[0]==g2);

  CPPUNIT_ASSERT(observer.afterDelLocalPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.afterDelLocalPropertyCalledGraphs[0]==g1);
  CPPUNIT_ASSERT(observer.beforeDelLocalPropertyCalledGraphs.size()==1);
  CPPUNIT_ASSERT(observer.beforeDelLocalPropertyCalledGraphs[0]==g1);

  CPPUNIT_ASSERT(observer.addLocalPropertyCalledGraphs.empty());
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs.size()==2);
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs[0]==g1);
  CPPUNIT_ASSERT(observer.addInheritedPropertyCalledGraphs[1]==g2);

}

void ObservableGraphTest::testDeleteBug747() {
  CPPUNIT_ASSERT(observer != NULL);
  CPPUNIT_ASSERT(graph != NULL);

  gObserver->deleteBug747 = true;
  delete graph;

  CPPUNIT_ASSERT(observer == NULL);
  CPPUNIT_ASSERT(graph == NULL);
}
  
