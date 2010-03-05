#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/ForEach.h>
#include "PushPopTest.h"
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ConnectedTest.h>
#include <tulip/ExtendedClusterOperation.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( PushPopTest );

//==========================================================
void PushPopTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void PushPopTest::tearDown() {
  //graph->pop();
  delete graph;
}
//==========================================================
void PushPopTest::build(unsigned int nbNodes, unsigned int edgeRatio) {
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_ADD  = nbNodes;
  unsigned int EDGE_RATIO = edgeRatio;
  for (unsigned int i=0; i<NB_ADD; ++i)
    nodes.push_back(graph->addNode());
  unsigned int NB_EDGES = EDGE_RATIO * NB_ADD;
  for (unsigned int i=0; i< NB_EDGES; ++i)
    edges.push_back(graph->addEdge(nodes[rand()%NB_ADD], nodes[rand()%NB_ADD]));
}
//==========================================================
void PushPopTest::testAddDel() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  graph->push();
  node n2 = graph->addNode();
  node n3 = graph->addNode();

  edge e1 = graph->addEdge(n2, n3);
  CPPUNIT_ASSERT(graph->existEdge(n2, n3) == e1);

  graph->reverse(e0);
  CPPUNIT_ASSERT(graph->existEdge(n1, n0) == e0);

  graph->delNode(n0);
  CPPUNIT_ASSERT(graph->isElement(n0) == false);
  CPPUNIT_ASSERT(graph->isElement(e0) == false);

  graph->pop();

  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->existEdge(n0, n1) == e0);

  graph->unpop();
  CPPUNIT_ASSERT(graph->existEdge(n2, n3) == e1);
  CPPUNIT_ASSERT(graph->isElement(e0) == false);

  graph->reverse(e1);
  CPPUNIT_ASSERT(graph->existEdge(n3, n2) == e1);
  
  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == false);
  CPPUNIT_ASSERT(graph->isElement(n3) == false);

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e1) == true);
  CPPUNIT_ASSERT(graph->isElement(n2) == true);
  CPPUNIT_ASSERT(graph->isElement(n3) == true);
  CPPUNIT_ASSERT(graph->existEdge(n3, n2) == e1);

  graph->delEdge(e1);
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == true);
  CPPUNIT_ASSERT(graph->isElement(n3) == true);

  graph->delNode(n2);
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == false);
  CPPUNIT_ASSERT(graph->isElement(n3) == true);
  
  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == false);
  CPPUNIT_ASSERT(graph->isElement(n3) == false);

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e1) == false);
  CPPUNIT_ASSERT(graph->isElement(n2) == false);
  CPPUNIT_ASSERT(graph->isElement(n3) == true);
  
}

//==========================================================
void PushPopTest::testSetValue() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  DoubleProperty* prop = graph->getProperty<DoubleProperty>("prop");

  prop->setNodeValue(n1, 1.0);
  prop->setEdgeValue(e0, 1.0);

  graph->push();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n0, n2);

  prop->setNodeValue(n2, 2.0);
  prop->setEdgeValue(e1, 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 2.0);

  prop->setNodeValue(n1, 2.0);
  prop->setEdgeValue(e0, 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 2.0);

  graph->push();
  prop->setAllNodeValue(3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 3.0);
  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 2.0);
  
  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 3.0);

  prop->setAllEdgeValue(3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 3.0);

  graph->delNode(n1);
  CPPUNIT_ASSERT(graph->isElement(n1) == false);
  CPPUNIT_ASSERT(graph->isElement(e0) == false);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 2.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 3.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 3.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 3.0);

  prop->setNodeValue(n0, 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 2.0);
  prop->setEdgeValue(e0, 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 2.0);  
}

//==========================================================
void PushPopTest::testCopyProperty() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);
  edge e1 = graph->addEdge(n1, n0);

  DoubleProperty* prop = graph->getProperty<DoubleProperty>("prop");

  prop->setNodeValue(n0, 1.0);
  prop->setEdgeValue(e0, 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);  
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 0.0);  

  DoubleProperty p(graph);

  p.setNodeValue(n1, 1.0);
  p.setEdgeValue(e1, 1.0);
  CPPUNIT_ASSERT(p.getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(p.getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(p.getEdgeValue(e0) == 0.0);  
  CPPUNIT_ASSERT(p.getEdgeValue(e1) == 1.0);

  graph->push();

  *prop = p;
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 0.0);  
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 1.0);  

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);  
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 0.0);

  graph->push();
  prop->setAllNodeValue(0.0);
  prop->setAllEdgeValue(0.0);
  prop->setNodeValue(n1, 1.0);
  prop->setEdgeValue(e1, 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 0.0);  
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 1.0);  

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);  
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 0.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 0.0);  
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 1.0);  
}

void PushPopTest::testSubgraph() {
  Graph *g1, *g2, *g3, *g4;

  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g2->addNode(n0);
  g2->addNode(n1);
  g2->addEdge(e0);

  graph->push();

  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  Iterator<Graph *> *it= g2->getSubGraphs();
  Graph *a,*b;
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->unpop();
  
  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  node n2 = g3->addNode();
  node n3 = g3->addNode();
  edge e = g2->addEdge(n2,n3);

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g3->isElement(n3));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

  graph->pop();

  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(!g2->isElement(n2));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(!graph->isElement(n2));
  CPPUNIT_ASSERT(!graph->isElement(n3));

  graph->unpop();

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g3->isElement(n3));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

  g2->delNode(n3);

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(!g3->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));

  graph->delNode(n0);

  CPPUNIT_ASSERT(!g2->isElement(n0));
  CPPUNIT_ASSERT(!g2->isElement(e0));
  CPPUNIT_ASSERT(!graph->isElement(n0));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  graph->pop();

  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(!graph->isElement(n3));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n0));
  CPPUNIT_ASSERT(g2->isElement(e0));
  CPPUNIT_ASSERT(graph->isElement(n0));
  CPPUNIT_ASSERT(graph->isElement(e0));

  graph->unpop();

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(!g3->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(!g2->isElement(n0));
  CPPUNIT_ASSERT(!g2->isElement(e0));
  CPPUNIT_ASSERT(!graph->isElement(n0));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  g3->addNode(n1);
  e = g3->addEdge(n1, n2);
  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n1));
  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n1));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n1));
  CPPUNIT_ASSERT(graph->isElement(n2));

  graph->pop();
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n1));
  CPPUNIT_ASSERT(!g2->isElement(n2));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n1));
  CPPUNIT_ASSERT(!graph->isElement(n2));

  graph->unpop();
  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n1));
  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n1));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n1));
  CPPUNIT_ASSERT(graph->isElement(n2));

  graph->push();

  g2->delSubGraph(g3);
  g2->delSubGraph(g4);

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n1));
  CPPUNIT_ASSERT(g3->isElement(n2));

  g2->delSubGraph(g4);
  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(!it->hasNext());
  CPPUNIT_ASSERT(a == g3);
  delete it;

  graph->push();
  graph->delSubGraph(g2);
  it= graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g1 && b==g3) || (a==g3 && b==g1));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();
  it= graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g1 && b==g2) || (a==g2 && b==g1));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;
}

void PushPopTest::testTests() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);
  edge e1 = graph->addEdge(n0, n2);

  node n3 = graph->addNode();

  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));
  
  graph->push();
  vector<edge> addedEdges;
  ConnectedTest::makeConnected(graph, addedEdges);
  CPPUNIT_ASSERT(ConnectedTest::isConnected(graph));

  graph->pop();
  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));
}

void PushPopTest::testAddDelProps() {
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));

  BooleanProperty* bProp = graph->getProperty<BooleanProperty>("boolean");
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  graph->push();

  DoubleProperty* dProp = graph->getProperty<DoubleProperty>("double");
  CPPUNIT_ASSERT(graph->existProperty("double"));

  graph->pop();

  CPPUNIT_ASSERT(!graph->existProperty("double"));
  
  graph->unpop();
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double") == dProp);
  CPPUNIT_ASSERT(graph->existProperty("boolean"));

  graph->delLocalProperty("boolean");
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  graph->delLocalProperty("double");
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  // we must try to ensure to not allocate the memory previously allocated
  // to old "double" to a new "double", so allocate chunk before "double"
  void* chunk = malloc(500);
  graph->getProperty<DoubleProperty>("double");
  free(chunk);
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double") != dProp);

  graph->push();
  CPPUNIT_ASSERT(graph->existProperty("double"));
  graph->delLocalProperty("double");
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  graph->pop();
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double") != dProp);

  graph->pop();
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->getProperty<BooleanProperty>("boolean") == bProp);
}

void PushPopTest::testMetaNode() {
  node n0 = graph->addNode();
  LayoutProperty* layout = graph->getProperty<LayoutProperty>("viewLayout");
  Coord coord0(-1, -1, -1);
  layout->setNodeValue(n0, coord0);

  set<node> tmp;
  tmp.insert(n0);

  graph->push();

  // create clone graph
  Graph* clone = tlp::newCloneSubGraph(graph, "groups");

  // create meta node
  node metaNode = clone->createMetaNode(tmp);
  Color mColor(255, 255, 255, 127);
  ColorProperty* color = graph->getProperty<ColorProperty>("viewColor");
  color->setNodeValue(metaNode, mColor);

  CPPUNIT_ASSERT(graph->isElement(metaNode));
  CPPUNIT_ASSERT(graph->isMetaNode(metaNode));
  CPPUNIT_ASSERT(coord0 == layout->getNodeValue(metaNode));
  CPPUNIT_ASSERT(mColor == color->getNodeValue(metaNode));

  graph->pop();
  CPPUNIT_ASSERT(!graph->isElement(metaNode));
  CPPUNIT_ASSERT(coord0 != layout->getNodeValue(metaNode));

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(metaNode));
  CPPUNIT_ASSERT(graph->isMetaNode(metaNode));
  CPPUNIT_ASSERT(coord0 == layout->getNodeValue(metaNode));
  CPPUNIT_ASSERT(mColor == color->getNodeValue(metaNode));
}

//==========================================================
CppUnit::Test * PushPopTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Push/Pop" );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Add/Del operations (Simple Graph)", 
								  &PushPopTest::testAddDel) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "SetNode/EdgeValue operations", 
								  &PushPopTest::testSetValue) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Copy Prop operations", 
								  &PushPopTest::testCopyProperty) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "addSubGraph operations", 
								  &PushPopTest::testSubgraph) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Tests operations", 
								  &PushPopTest::testTests) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "Properties operations", 
								  &PushPopTest::testAddDelProps) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PushPopTest>( "MetaNode operations", 
								  &PushPopTest::testMetaNode) );
  return suiteOfTests;
}
//==========================================================
