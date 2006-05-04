#include <string>
#include <tulip/SuperGraph.h>
#include <tulip/TlpTools.h>

#include <tulip/SimpleTest.h>
#include <tulip/TreeTest.h>
#include <tulip/AcyclicTest.h>
#include <tulip/ConnectedTest.h>
#include <tulip/BiconnectedTest.h>
#include <tulip/TriconnectedTest.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "TestAlgorithmTest.h"

using namespace std;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( TestAlgorithmTest );


//==========================================================
void TestAlgorithmTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void TestAlgorithmTest::tearDown() {
  delete graph;
}
//==========================================================
void TestAlgorithmTest::testSimple() {
  cerr << __FUNCTION__ << " : not implemented" << endl;
}
//==========================================================
void TestAlgorithmTest::testTree() {
  graph->clear();
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  edge e0 = graph->addEdge(n1, n2);
  edge e1 = graph->addEdge(n1, n3);
  CPPUNIT_ASSERT(TreeTest::isTree(graph));
  node n4 = graph->addNode();
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  edge e2 = graph->addEdge(n4, n1);
  CPPUNIT_ASSERT(TreeTest::isTree(graph));
  graph->reverse(e1);
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  graph->reverse(e0);
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  graph->reverse(e2);
  graph->delNode(n3);
  CPPUNIT_ASSERT(TreeTest::isTree(graph));
  //known bug test 
  {
    graph->clear();
    node n1 = graph->addNode();
    node n2 = graph->addNode();
    node n3 = graph->addNode();
    graph->addEdge(n1, n2);
    graph->addEdge(n1, n3);
    graph->delNode(n3);
    CPPUNIT_ASSERT(TreeTest::isTree(graph));
  }
}
//==========================================================
void TestAlgorithmTest::testAcyclic() {
  graph->clear();
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  graph->addEdge(n1, n2);
  edge e0 = graph->addEdge(n1, n3);
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(graph));
  edge e1 = graph->addEdge(n2, n3);
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(graph));
  edge e2 = graph->addEdge(n3, n1);
  CPPUNIT_ASSERT(!AcyclicTest::isAcyclic(graph));
  graph->reverse(e2);
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(graph));
}
//==========================================================
void TestAlgorithmTest::testConnected() {
  graph->clear();
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));
  edge e = graph->addEdge(n1, n2);
  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));
  graph->addEdge(n3, n2);
  CPPUNIT_ASSERT(ConnectedTest::isConnected(graph));
  graph->delEdge(e);
  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));
  vector<edge> addedEdge;
  ConnectedTest::makeConnected(graph, addedEdge);
  CPPUNIT_ASSERT(ConnectedTest::isConnected(graph));
  CPPUNIT_ASSERT(addedEdge.size() == 1u);
  graph->delEdge(addedEdge[0]);
  CPPUNIT_ASSERT(ConnectedTest::numberOfConnectedComponnents(graph) == 2u);
}
//==========================================================
const std::string GRAPHPATH = "./DATA/graphs/";

void TestAlgorithmTest::testBiconnected() {
  node n[10];
  edge e[10];
  for (int i=0;i<4;++i)
    n[i]=graph->addNode();
  for (int i=0;i<4;++i)
    e[i]=graph->addEdge(n[i],n[(i+1)%4]);
  CPPUNIT_ASSERT(BiconnectedTest::isBiconnected(graph));
  graph->delEdge(e[0]);
  CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(graph));
  e[0]=graph->addEdge(n[0],n[2]);
  n[4]=graph->addNode();
  e[4]=graph->addEdge(n[4],n[1]);
  e[5]=graph->addEdge(n[4],n[2]);
  CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(graph));
  e[6]=graph->addEdge(n[4],n[0]);
  CPPUNIT_ASSERT(BiconnectedTest::isBiconnected(graph));
  n[5]=graph->addNode();
  CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(graph));
  e[7]=graph->addEdge(n[1],n[5]);
  CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(graph));
  //Root separator
  graph->clear();
  for (int i=0;i<5;++i)
    n[i]=graph->addNode();
  e[0]=graph->addEdge(n[0],n[1]);
  e[1]=graph->addEdge(n[0],n[2]);
  e[2]=graph->addEdge(n[1],n[2]);

  e[3]=graph->addEdge(n[0],n[3]);
  e[4]=graph->addEdge(n[0],n[4]);
  e[5]=graph->addEdge(n[3],n[4]);

  CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(graph));
  e[6]=graph->addEdge(n[2],n[4]);
  CPPUNIT_ASSERT(BiconnectedTest::isBiconnected(graph));
  //Test makeBiconnected
  graph->delEdge(e[6]);
  CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(graph));
  vector<edge> addedEdges;
  BiconnectedTest::makeBiconnected(graph, addedEdges);
  CPPUNIT_ASSERT(BiconnectedTest::isBiconnected(graph));
  CPPUNIT_ASSERT(addedEdges.size() == 1);
  graph->delEdge(addedEdges[0]);
  CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(graph));

  SuperGraph *tmpGraph;
  for (unsigned int i = 0; i<5; ++i) {
    tmpGraph = tlp::loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");
    CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(tmpGraph));
    vector<edge> vEdges;
    BiconnectedTest::makeBiconnected(tmpGraph, vEdges);
    CPPUNIT_ASSERT(BiconnectedTest::isBiconnected(tmpGraph));
    for (vector<edge>::iterator it = vEdges.begin(); it != vEdges.end(); ++it) {
      tmpGraph->delAllEdge(*it);
    }
    CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(tmpGraph));
    delete tmpGraph;
  }

}
//==========================================================
void TestAlgorithmTest::testTriconnected() {
  cerr << __FUNCTION__ << " : not implemented" << endl;
}
//==========================================================
CppUnit::Test * TestAlgorithmTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Graph Test" );
  suiteOfTests->addTest( new CppUnit::TestCaller<TestAlgorithmTest>( "Simple test", 
								     &TestAlgorithmTest::testSimple ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TestAlgorithmTest>( "Tree test", 
								     &TestAlgorithmTest::testTree ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TestAlgorithmTest>( "Acyclic test", 
								     &TestAlgorithmTest::testTree ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TestAlgorithmTest>( "Biconnected test", 
								     &TestAlgorithmTest::testBiconnected ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TestAlgorithmTest>( "Connected test", 
								     &TestAlgorithmTest::testConnected ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TestAlgorithmTest>( "Triconnected test", 
								     &TestAlgorithmTest::testTriconnected ) );
  return suiteOfTests;
}
//==========================================================
