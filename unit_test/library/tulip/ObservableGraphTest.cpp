#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/ForEach.h>
#include "ObservableGraphTest.h"
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( ObservableGraphTest );

// this class will capture
// everything that will happen to our properties
// synchronously or asynchronously
class GraphObserverTest :public GraphObserver {
public:

  vector<Graph*> graphs;
  vector<Graph*> sGraphs;
  vector<node> nodes;
  vector<edge> edges;
  string pName;

  Graph* getObservedGraph() {
    CPPUNIT_ASSERT(graphs.size() == 1);
    return graphs[0];
  }

  vector<Graph*>& getObservedGraphs() {
    return graphs;
  }

  Graph* getObservedSubgraph() {
    CPPUNIT_ASSERT(sGraphs.size() == 1);
    return sGraphs[0];
  }

  vector<Graph*>& getObservedSubgraphs() {
    return sGraphs;
  }

  node getObservedNode() {
    CPPUNIT_ASSERT(nodes.size() == 1);
    return nodes[0];
  }
  
  vector<node>& getObservedNodes() {
    return nodes;
  }

  edge getObservedEdge() {
    CPPUNIT_ASSERT(edges.size() == 1);
    return edges[0];
  }

  vector<edge>& getObservedEdges() {
    return edges;
  }
  
  string& getObservedPropertyName() { return pName; }

  void reset() {
    graphs.clear(), sGraphs.clear();
    nodes.clear(), edges.clear(), pName = string();
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
};  

static GraphObserverTest* observer;

//==========================================================
void ObservableGraphTest::setUp() {
  graph = tlp::newGraph();
  observer = new GraphObserverTest();
  graph->addObserver(observer);
}
//==========================================================
void ObservableGraphTest::tearDown() {
  delete graph;
  delete observer;
}
//==========================================================
void ObservableGraphTest::testAddDel() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_NODES = 100;

  for (unsigned int i=0; i<NB_NODES; ++i) {
    observer->reset();
    nodes.push_back(graph->addNode());
    CPPUNIT_ASSERT(nodes[i] == observer->getObservedNode());
    CPPUNIT_ASSERT(graph == observer->getObservedGraph());
  }

  for (unsigned int i=0; i< NB_NODES; ++i) {
    observer->reset();
    edges.push_back(graph->addEdge(nodes[i],
				   (i == NB_NODES - 1)
				   ? nodes[0] : nodes[i]));
    CPPUNIT_ASSERT(edges[i] == observer->getObservedEdge());
    CPPUNIT_ASSERT(graph == observer->getObservedGraph());
  }

  for (unsigned int i=0; i < NB_NODES / 2; ++i) {
    observer->reset();
    graph->delEdge(edges[2 * i]);
    CPPUNIT_ASSERT(edges[2 * i] == observer->getObservedEdge());
    CPPUNIT_ASSERT(graph == observer->getObservedGraph());
  }

  for (unsigned int i=0; i < NB_NODES; ++i) {
    observer->reset();
    graph->delNode(nodes[i]);
    CPPUNIT_ASSERT(nodes[i] == observer->getObservedNode());
    if (i == 0 || (i%2 && i != (NB_NODES - 1))) {
      vector<Graph*>& graphs = observer->getObservedGraphs();
      CPPUNIT_ASSERT(graphs.size() == 2);
      CPPUNIT_ASSERT(graphs[0] == graph);
      CPPUNIT_ASSERT(graphs[1] == graph);
      if (i == 0)
	CPPUNIT_ASSERT(edges[NB_NODES - 1] == observer->getObservedEdge());
      else
	CPPUNIT_ASSERT(edges[i] == observer->getObservedEdge());
    } else
      CPPUNIT_ASSERT(graph == observer->getObservedGraph());
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
				   ? nodes[0] : nodes[i]));
  }

  observer->reset();

  graph->clear();

  vector<node>& oNodes = observer->getObservedNodes();
  CPPUNIT_ASSERT(oNodes.size() == NB_NODES);
  for(unsigned int i = 0; i < NB_NODES; ++i) {
    CPPUNIT_ASSERT(nodes[i] == oNodes[i]);
  }
  vector<edge>& oEdges = observer->getObservedEdges();
  CPPUNIT_ASSERT(oEdges.size() == NB_NODES);
  for(unsigned int i = 0; i < NB_NODES; ++i) {
    if (i == 0)
      CPPUNIT_ASSERT(oEdges[i] == edges[NB_NODES - 1]);
    else
      CPPUNIT_ASSERT(oEdges[i] == edges[i - 1]);
  }
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
				   ? nodes[0] : nodes[i]));
  }
  for (unsigned int i=0; i< NB_NODES; ++i) {
    observer->reset();
    graph->reverse(edges[i]);
    CPPUNIT_ASSERT(edges[i] == observer->getObservedEdge());
    CPPUNIT_ASSERT(graph == observer->getObservedGraph());
  }
}
//==========================================================
void ObservableGraphTest::testSubgraph() {
  Graph *g1, *g2, *g3, *g4;

  observer->reset();
  g1 = graph->addSubGraph();
  CPPUNIT_ASSERT(observer->getObservedSubgraph() == g1);
  CPPUNIT_ASSERT(observer->getObservedGraph() == graph);
  observer->reset();
  g2 = graph->addSubGraph();
  CPPUNIT_ASSERT(observer->getObservedSubgraph() == g2);
  CPPUNIT_ASSERT(observer->getObservedGraph() == graph);
  observer->reset();
  CPPUNIT_ASSERT(observer->sGraphs.size() == 0);
  g2->addObserver(observer);
  g3 = g2->addSubGraph();
  CPPUNIT_ASSERT(observer->getObservedSubgraph() == g3);
  CPPUNIT_ASSERT(observer->getObservedGraph() == g2);
  observer->reset();
  g4 = g2->addSubGraph();
  CPPUNIT_ASSERT(observer->getObservedSubgraph() == g4);
  CPPUNIT_ASSERT(observer->getObservedGraph() == g2);

  observer->reset();
  g3->addObserver(observer);
  node n1 = g3->addNode();
  vector<Graph*>& graphs = observer->getObservedGraphs();
  CPPUNIT_ASSERT(graphs.size() == 3);
  CPPUNIT_ASSERT(graphs[0] == graph);
  CPPUNIT_ASSERT(graphs[1] == g2);
  CPPUNIT_ASSERT(graphs[2] == g3);
  vector<node>& nodes = observer->getObservedNodes();
  CPPUNIT_ASSERT(nodes.size() == 3);
  CPPUNIT_ASSERT(nodes[0] == n1);
  CPPUNIT_ASSERT(nodes[1] == n1);
  CPPUNIT_ASSERT(nodes[2] == n1);
  
  observer->reset();
  g4->addObserver(observer);
  node n2 = g4->addNode();
  CPPUNIT_ASSERT(graphs.size() == 3);
  CPPUNIT_ASSERT(graphs[0] == graph);
  CPPUNIT_ASSERT(graphs[1] == g2);
  CPPUNIT_ASSERT(graphs[2] == g4);
  CPPUNIT_ASSERT(nodes.size() == 3);
  CPPUNIT_ASSERT(nodes[0] == n2);
  CPPUNIT_ASSERT(nodes[1] == n2);
  CPPUNIT_ASSERT(nodes[2] == n2);
  
  observer->reset();
  edge e = g2->addEdge(n1, n2);
  CPPUNIT_ASSERT(graphs.size() == 2);
  CPPUNIT_ASSERT(graphs[0] == graph);
  CPPUNIT_ASSERT(graphs[1] == g2);
  vector<edge>& edges = observer->getObservedEdges();
  CPPUNIT_ASSERT(edges.size() == 2);
  CPPUNIT_ASSERT(edges[0] == e);
  CPPUNIT_ASSERT(edges[1] == e);
  
  observer->reset();
  g2->delNode(n2);
  CPPUNIT_ASSERT(graphs.size() == 3);
  CPPUNIT_ASSERT(graphs[0] == g2);
  CPPUNIT_ASSERT(graphs[1] == g4);
  CPPUNIT_ASSERT(graphs[2] == g2);
  CPPUNIT_ASSERT(nodes.size() == 2);
  CPPUNIT_ASSERT(nodes[0] == n2);
  CPPUNIT_ASSERT(nodes[1] == n2);  
  CPPUNIT_ASSERT(edges.size() == 1);
  CPPUNIT_ASSERT(edges[0] == e);

  observer->reset();
  g2->addNode(n2);
  CPPUNIT_ASSERT(graphs[0] == observer->getObservedGraph());
  CPPUNIT_ASSERT(n2 == observer->getObservedNode());
  observer->reset();
  g2->addEdge(e);
  CPPUNIT_ASSERT(graphs[0] == observer->getObservedGraph());
  CPPUNIT_ASSERT(e == observer->getObservedEdge());

  observer->reset();
  g2->delAllNode(n2);
  CPPUNIT_ASSERT(graphs.size() == 4);
  CPPUNIT_ASSERT(graphs[0] == graph);
  CPPUNIT_ASSERT(graphs[1] == g2);
  CPPUNIT_ASSERT(graphs[2] == g2);
  CPPUNIT_ASSERT(graphs[3] == graph);
  CPPUNIT_ASSERT(nodes.size() == 2);
  CPPUNIT_ASSERT(nodes[0] == n2);
  CPPUNIT_ASSERT(nodes[1] == n2);  
  CPPUNIT_ASSERT(edges.size() == 2);
  CPPUNIT_ASSERT(edges[0] == e);
  CPPUNIT_ASSERT(edges[1] == e);
}
//==========================================================
void ObservableGraphTest::testDeleteSubgraph() {
  Graph *g1, *g2, *g3, *g4;

  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g2->addObserver(observer);
  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  observer->reset();
  g2->delSubGraph(g3);
  CPPUNIT_ASSERT(observer->getObservedSubgraph() == g3);
  CPPUNIT_ASSERT(observer->getObservedGraph() == g2);

  observer->reset();
  g2->delSubGraph(g4);
  CPPUNIT_ASSERT(observer->getObservedSubgraph() == g4);
  CPPUNIT_ASSERT(observer->getObservedGraph() == g2);

  observer->reset();
  graph->delAllSubGraphs(g2);
  CPPUNIT_ASSERT(observer->getObservedSubgraph() == g2);
  vector<Graph*>& graphs = observer->getObservedGraphs();
  CPPUNIT_ASSERT(graphs[0] == graph);
  CPPUNIT_ASSERT(graphs[1] == g2);
}
//==========================================================
CppUnit::Test * ObservableGraphTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Graph" );
  suiteOfTests->addTest( new CppUnit::TestCaller<ObservableGraphTest>( "Add/Del operations (Simple Graph)", 
								  &ObservableGraphTest::testAddDel) );
  suiteOfTests->addTest( new CppUnit::TestCaller<ObservableGraphTest>( "Clean operations (Simple Graph)", 
								  &ObservableGraphTest::testClear) );
  suiteOfTests->addTest( new CppUnit::TestCaller<ObservableGraphTest>( "Edge reversing", 
								  &ObservableGraphTest::testReverse) );

  suiteOfTests->addTest( new CppUnit::TestCaller<ObservableGraphTest>( "Sub Graph operations (add/del/clean/iterators)", 
								  &ObservableGraphTest::testSubgraph) );
  suiteOfTests->addTest( new CppUnit::TestCaller<ObservableGraphTest>( "Test Sub Graph delete", 
								  &ObservableGraphTest::testDeleteSubgraph) );


  return suiteOfTests;
}
//==========================================================
