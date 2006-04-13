#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/TlpTools.h>
#include "MetaGraphProxyTest.h"

using namespace std;

#include <cppunit/extensions/HelperMacros.h>

// Warning MetaGraphProxy has been renamed in MetaGraph

CPPUNIT_TEST_SUITE_REGISTRATION( MetaGraphProxyTest );

//==========================================================
void MetaGraphProxyTest::setUp() {
  graph    = tlp::newSuperGraph();
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
void MetaGraphProxyTest::tearDown() {
  delete graph;
}
//==========================================================
void MetaGraphProxyTest::testDestroyGraph() {
  //build the hierarchy
  SuperGraph * g1 = tlp::newCloneSubGraph(graph, "G1");
  SuperGraph * g2 = tlp::newCloneSubGraph(graph, "G2");
  SuperGraph * meta1 = tlp::newSubGraph(graph, "META1");
  MetaGraph * proxy1 = meta1->getLocalProperty<MetaGraph>("viewMetaGraph");
  MetaGraph proxy3(meta1);
  node mnode1 = meta1->addNode();
  node mnode2 = meta1->addNode();
  proxy1->setNodeValue(mnode1, g1);
  proxy1->setNodeValue(mnode2, g2);
  proxy3.setNodeValue(mnode1, g1);
  proxy3.setNodeValue(mnode2, g2);
  graph->delSubGraph(g2);
  CPPUNIT_ASSERT(0  == proxy1->getNodeValue(mnode2));
  CPPUNIT_ASSERT(0  == proxy3.getNodeValue(mnode2));
  CPPUNIT_ASSERT(g1 == proxy1->getNodeValue(mnode1));
  CPPUNIT_ASSERT(g1 == proxy3.getNodeValue(mnode1));
  graph->delSubGraph(g1);
  CPPUNIT_ASSERT(0 == proxy1->getNodeValue(mnode2));
  CPPUNIT_ASSERT(0 == proxy3.getNodeValue(mnode2));
  CPPUNIT_ASSERT(0 == proxy1->getNodeValue(mnode1));
  CPPUNIT_ASSERT(0 == proxy3.getNodeValue(mnode1));
}
//==========================================================
void MetaGraphProxyTest::testSetGet() {
  //build the hierarchy
  SuperGraph * g1 = tlp::newCloneSubGraph(graph, "G1");
  SuperGraph * g2 = tlp::newCloneSubGraph(graph, "G2");
  SuperGraph * g3 = tlp::newCloneSubGraph(graph, "G3");
  SuperGraph * meta1 = tlp::newSubGraph(graph, "META1");
  MetaGraph * proxy1 = meta1->getLocalProperty<MetaGraph>("viewMetaGraph");
  MetaGraph proxy3(meta1);
  node mnode1 = meta1->addNode();
  node mnode2 = meta1->addNode();
  proxy1->setNodeValue(mnode1, g1);
  proxy1->setNodeValue(mnode2, g2);
  proxy3.setNodeValue(mnode1, g1);
  proxy3.setNodeValue(mnode2, g2);
  proxy1->setNodeValue(mnode2, g3);
  graph->delSubGraph(g2);
  CPPUNIT_ASSERT(g3 == proxy1->getNodeValue(mnode2));
  CPPUNIT_ASSERT(0 == proxy3.getNodeValue(mnode2));
  CPPUNIT_ASSERT(g1 == proxy1->getNodeValue(mnode1));
  CPPUNIT_ASSERT(g1 == proxy3.getNodeValue(mnode1));
}
//==========================================================
void MetaGraphProxyTest::testSetAll() {
  //build the hierarchy
  SuperGraph * g1 = tlp::newCloneSubGraph(graph, "G1");
  SuperGraph * g2 = tlp::newCloneSubGraph(graph, "G2");
  SuperGraph * g3 = tlp::newCloneSubGraph(graph, "G3");
  SuperGraph * meta1 = tlp::newSubGraph(graph, "META1");
  MetaGraph proxy(meta1);
  node mnode1 = meta1->addNode();
  node mnode2 = meta1->addNode();
  node mnode3 = meta1->addNode();
  proxy.setAllNodeValue(g3);
  proxy.setNodeValue(mnode1, g1);
  proxy.setNodeValue(mnode2, g2);
  CPPUNIT_ASSERT(g1 == proxy.getNodeValue(mnode1));
  CPPUNIT_ASSERT(g2 == proxy.getNodeValue(mnode2));
  CPPUNIT_ASSERT(g3 == proxy.getNodeValue(mnode3));
  graph->delSubGraph(g3);
  CPPUNIT_ASSERT(g1 == proxy.getNodeValue(mnode1));
  CPPUNIT_ASSERT(g2 == proxy.getNodeValue(mnode2));
  CPPUNIT_ASSERT(0  == proxy.getNodeValue(mnode3));
  proxy.setAllNodeValue(0);
  graph->delSubGraph(g1);
  graph->delSubGraph(g2);
}
//==========================================================
CppUnit::Test * MetaGraphProxyTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : MetaGraph" );
  suiteOfTests->addTest( new CppUnit::TestCaller<MetaGraphProxyTest>( "test graph destruction", 
								      &MetaGraphProxyTest::testDestroyGraph ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<MetaGraphProxyTest>( "test set/get", 
								      &MetaGraphProxyTest::testSetGet ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<MetaGraphProxyTest>( "test set all node value", 
								      &MetaGraphProxyTest::testSetAll ) );
  return suiteOfTests;
}
