#include <cassert>
#include <vector>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/ExtendedClusterOperation.h>
#include <tulip/ForEach.h>

#include "ExtendedClusterOperationTest.h"

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( ExtendedClusterOperationTest );
//==========================================================
void ExtendedClusterOperationTest::setUp() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  graph = tlp::newGraph();
  for (unsigned int i=0; i<5; ++i) {
    nodes.push_back(graph->addNode());
  }
  edges.push_back(graph->addEdge(nodes[0], nodes[1]));
  edges.push_back(graph->addEdge(nodes[0], nodes[2]));
  edges.push_back(graph->addEdge(nodes[1], nodes[3]));
  edges.push_back(graph->addEdge(nodes[1], nodes[4]));
  edges.push_back(graph->addEdge(nodes[2], nodes[3]));

  quotient = tlp::newCloneSubGraph(graph);

  group.insert(nodes[0]);
  group.insert(nodes[1]);
}
//==========================================================
void ExtendedClusterOperationTest::tearDown() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  delete graph;
  nodes.clear();
  edges.clear();
  group.clear();
}
//==========================================================
void ExtendedClusterOperationTest::testCreateMetaNode() {
  cerr << endl << "L'erreur à suivre sur createMetaNode fait partie de testCreateMetaNode" << endl;
  meta = graph->createMetaNode(group);
  CPPUNIT_ASSERT_EQUAL(false, meta.isValid());

  meta = quotient->createMetaNode(group);
  CPPUNIT_ASSERT_EQUAL(false, quotient->isElement(nodes[0]));
  CPPUNIT_ASSERT_EQUAL(false, quotient->isElement(nodes[1]));
  CPPUNIT_ASSERT_EQUAL(true, quotient->existEdge(meta, nodes[4]).isValid());
  CPPUNIT_ASSERT_EQUAL(true, quotient->existEdge(meta, nodes[2]).isValid());
  CPPUNIT_ASSERT_EQUAL(true, quotient->existEdge(meta, nodes[3]).isValid());
  CPPUNIT_ASSERT_EQUAL(4u, quotient->numberOfEdges());
  CPPUNIT_ASSERT_EQUAL(6u, graph->numberOfNodes());

  GraphProperty *clusterInfo = quotient->getProperty<GraphProperty>("viewMetaGraph");
  Graph *cluster = clusterInfo->getNodeValue(meta);
  CPPUNIT_ASSERT(cluster!=0);
  CPPUNIT_ASSERT_EQUAL(true, cluster->isElement(nodes[0]));
  CPPUNIT_ASSERT_EQUAL(true, cluster->isElement(nodes[1]));
  CPPUNIT_ASSERT_EQUAL(1u, cluster->numberOfEdges());
  CPPUNIT_ASSERT_EQUAL(2u, cluster->numberOfNodes());
}
//==========================================================
void ExtendedClusterOperationTest::testBugOpenInSubgraph() {
  cerr << __PRETTY_FUNCTION__ << endl;
  Graph * graph = tlp::loadGraph("./DATA/graphs/openmetanode1.tlp.gz");
  //take the quotient graph
  Graph * subgraph = 0;
  bool find = false;
  forEach(subgraph, graph->getSubGraphs()){
    string name = subgraph->getAttribute<string>("name");
    if (name == "unnamed") {
      find = true;
      breakForEach;
    }
  }
  CPPUNIT_ASSERT(find && subgraph != 0);
  CPPUNIT_ASSERT_EQUAL(2u, subgraph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(1u, subgraph->numberOfEdges());
  //open all meta nodes
  node n;
  stableForEach(n, subgraph->getNodes()) {
    subgraph->openMetaNode(n);
  }
  CPPUNIT_ASSERT_EQUAL(6u, subgraph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(5u, subgraph->numberOfEdges());
  delete graph;
}
//==========================================================
void ExtendedClusterOperationTest::testOpenMetaNode() {

  meta = quotient->createMetaNode(group);
  quotient->openMetaNode(meta);
  
  CPPUNIT_ASSERT_EQUAL(false, quotient->isElement(meta));
  CPPUNIT_ASSERT_EQUAL(false, graph->isElement(meta));

  CPPUNIT_ASSERT_EQUAL( 5u, graph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL( 5u, graph->numberOfEdges());
  
  CPPUNIT_ASSERT_EQUAL( 5u, quotient->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL( 5u, quotient->numberOfEdges());
  
  CPPUNIT_ASSERT(quotient->existEdge(nodes[0], nodes[1]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(nodes[0], nodes[2]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(nodes[1], nodes[3]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(nodes[1], nodes[4]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(nodes[2], nodes[3]).isValid());

}
//==========================================================
CppUnit::Test * ExtendedClusterOperationTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : ExtendedClusterOperation" );
  suiteOfTests->addTest( new CppUnit::TestCaller<ExtendedClusterOperationTest>( "Group a set of nodes", 
								    &ExtendedClusterOperationTest::testCreateMetaNode ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<ExtendedClusterOperationTest>( "Ungroup a MetaNode", 
								    &ExtendedClusterOperationTest::testOpenMetaNode ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<ExtendedClusterOperationTest>( "Open a metanode in a hierarchy of subgraph #BUG-1", 
										&ExtendedClusterOperationTest::testBugOpenInSubgraph ) );
  return suiteOfTests;
}
//==========================================================
