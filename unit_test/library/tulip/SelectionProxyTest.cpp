#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/TlpTools.h>
#include "SelectionProxyTest.h"

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>

// Warning SelectionProxy has been renamed in BooleanAlgorithm

CPPUNIT_TEST_SUITE_REGISTRATION( SelectionProxyTest );

//==========================================================
void buildGraph(Graph *graph) {
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

  BooleanProperty tmp(graph);
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
void SelectionProxyTest::testDelete(bool value) {
  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);
  Iterator<node> *itN=graph->getNodes();
  node n;
  while(itN->hasNext()) {
    n = itN->next();
  } delete itN;
  selection->setNodeValue(n, !value);
  CPPUNIT_ASSERT(selection->getNodeValue(n) == !value);
  graph->delNode(n);
  n = graph->addNode();
  CPPUNIT_ASSERT(selection->getNodeValue(n) == value);
  edge e = graph->addEdge(n, n);
  selection->setEdgeValue(e, !value);
  CPPUNIT_ASSERT(selection->getEdgeValue(e) == !value);
  graph->delEdge(e);
  e = graph->addEdge(n, n);
  CPPUNIT_ASSERT(selection->getEdgeValue(e) == value);
}
//==========================================================
void SelectionProxyTest::testDelete() {
  testDelete(false);
  testDelete(true);
}
//==========================================================

CppUnit::Test * SelectionProxyTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : BooleanProperty" );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test iterators", 
								      &SelectionProxyTest::testIterators ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test setAll", 
								      &SelectionProxyTest::testSetAll ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test set/get", 
								      &SelectionProxyTest::testSetGet ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test copy operator", 
								      &SelectionProxyTest::testCopy ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SelectionProxyTest>( "test delete", 
								      &SelectionProxyTest::testDelete ) );
  return suiteOfTests;
}
