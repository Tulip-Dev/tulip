#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/TlpTools.h>
#include "SelectionProxyTest.h"

using namespace std;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( SelectionProxyTest );

//==========================================================
void buildGraph(SuperGraph *graph) {
  //add three nodes
  node n1=graph->addNode();
  node n2=graph->addNode();
  node n3=graph->addNode();
  //add three edges
  graph->addEdge(n2,n3);
  graph->addEdge(n1,n2);
  graph->addEdge(n3,n1);
}
//==========================================================
void SelectionProxyTest::setUp() {
  graph    = tlp::newSuperGraph();
  selection = graph->getProperty<SelectionProxy>("Select Test");
  buildGraph(graph);
}
//==========================================================
void SelectionProxyTest::tearDown() {
  delete graph;
}
//==========================================================
void SelectionProxyTest::testSetAll(bool value) {
  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);
  Iterator<node> *itN=graph->getNodes();
  while(itN->hasNext()) {
    CPPUNIT_ASSERT( selection->getNodeValue(itN->next()) == value );
  } delete itN;
  Iterator<edge> *itE=graph->getEdges();
  while(itE->hasNext()) {
    CPPUNIT_ASSERT( selection->getEdgeValue(itE->next()) == value );
  } delete itE;
}
//==========================================================
void SelectionProxyTest::testSetAll() {
  testSetAll(true);
  testSetAll(false);
}
//==========================================================
void SelectionProxyTest::testSetGet(bool value) {
  vector<node> nodes(graph->numberOfNodes());
  vector<edge> edges(graph->numberOfEdges());
  unsigned int i=0;
  Iterator<node> *itN=graph->getNodes();
  while(itN->hasNext()) {
    nodes[i++]=itN->next();
  } delete itN;
  Iterator<edge> *itE=graph->getEdges();
  i=0;
  while(itE->hasNext()) {
    edges[i++]=itE->next();
  } delete itE;

  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);
  for (unsigned int i=0;i<graph->numberOfNodes()*10;++i) {
    unsigned int rando=rand()%graph->numberOfNodes();
    selection->setNodeValue(nodes[rando],!value);
    CPPUNIT_ASSERT( selection->getNodeValue(nodes[rando]) == !value );
  }
  for (unsigned int i=0;i<graph->numberOfEdges()*10;++i) {
    unsigned int rando=rand()%graph->numberOfEdges();
    selection->setEdgeValue(edges[rando],!value);
    CPPUNIT_ASSERT( selection->getEdgeValue(edges[rando]) == !value );
  }
}
//==========================================================
void SelectionProxyTest::testSetGet() {
  testSetGet(false);
  testSetGet(true);
}
//==========================================================
CppUnit::Test * SelectionProxyTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip Selection Proxy Test Suite" );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( 
								       "test setAll", 
								       &SelectionProxyTest::testSetAll ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( 
								       "test set/get", 
								       &SelectionProxyTest::testSetGet ) );
  return suiteOfTests;
}
