#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/TlpTools.h>
#include "SelectionProxyTest.h"

using namespace std;

#include <cppunit/extensions/HelperMacros.h>

// Warning SelectionProxy has been renamed in Selection

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
  graph    = tlp::newGraph();
  selection = graph->getProperty<BooleanProperty>("Select Test");
  buildGraph(graph);
}
//==========================================================
void SelectionProxyTest::tearDown() {
  delete graph;
}
//==========================================================
void SelectionProxyTest::testIterators() {
  selection->setAllNodeValue(true);
  selection->setAllEdgeValue(true);
  {
    Iterator<node> *itSG = graph->getNodes();
    Iterator<node> *itSE = selection->getNodesEqualTo(true);
    while (itSG->hasNext() && itSE->hasNext()) {
      CPPUNIT_ASSERT(itSG->next()==itSE->next());
    }
    CPPUNIT_ASSERT(!itSG->hasNext());
    CPPUNIT_ASSERT(!itSE->hasNext());
    delete itSG; delete itSE;
  }
  {
    Iterator<edge> *itSG = graph->getEdges();
    Iterator<edge> *itSE = selection->getEdgesEqualTo(true);
    while (itSG->hasNext() && itSE->hasNext()) {
      CPPUNIT_ASSERT(itSG->next()==itSE->next());
    }
    CPPUNIT_ASSERT(!itSG->hasNext());
    CPPUNIT_ASSERT(!itSE->hasNext());
    delete itSG; delete itSE;
  }
  
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
void SelectionProxyTest::testCopy() {
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
  bool value = true;
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

  Selection tmp(graph);
  tmp = *selection;
  itN=graph->getNodes();
  while(itN->hasNext()) {
    node n  = itN->next();
    CPPUNIT_ASSERT_EQUAL( selection->getNodeValue(n), tmp.getNodeValue(n) );
  } delete itN;
  itE=graph->getEdges();
  while(itE->hasNext()) {
    edge e = itE->next();
    CPPUNIT_ASSERT_EQUAL( selection->getEdgeValue(e), tmp.getEdgeValue(e) );
  } delete itE;
}
//==========================================================
void SelectionProxyTest::testSetGet() {
  testSetGet(false);
  testSetGet(true);
}
//==========================================================
CppUnit::Test * SelectionProxyTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Selection" );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test iterators", 
								      &SelectionProxyTest::testIterators ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test setAll", 
								      &SelectionProxyTest::testSetAll ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test set/get", 
								      &SelectionProxyTest::testSetGet ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test copy operator", 
								      &SelectionProxyTest::testCopy ) );
  return suiteOfTests;
}
