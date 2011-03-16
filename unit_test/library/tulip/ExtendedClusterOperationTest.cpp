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
void ExtendedClusterOperationTest::testBugCreateOpenMetaNode() {
  edge e02 = quotient->addEdge(nodes[0], nodes[2]);
  Iterator<edge>* ite = quotient->getInEdges(nodes[2]);
  unsigned nbEdges = 0;
  while (ite->hasNext()) {
    ++nbEdges;
    edge e = ite->next();
    CPPUNIT_ASSERT(e == e02 || e == edges[1]);
  } delete ite;
  CPPUNIT_ASSERT(nbEdges = 2);

  CPPUNIT_ASSERT(quotient->isElement(e02));
  CPPUNIT_ASSERT(quotient->isElement(edges[1]));

  meta = quotient->createMetaNode(group, false);

  edge metaE = quotient->existEdge(meta, nodes[2]);
  CPPUNIT_ASSERT(metaE.isValid());

  ite = quotient->getEdgeMetaInfo(metaE);
  nbEdges = 0;
  while (ite->hasNext()) {
    ++nbEdges;
    edge e = ite->next();
    CPPUNIT_ASSERT(e == e02 || e == edges[1]);
  } delete ite;
  CPPUNIT_ASSERT(nbEdges = 2);

  quotient->openMetaNode(meta);
  CPPUNIT_ASSERT(quotient->isElement(metaE) == false);
  CPPUNIT_ASSERT(quotient->isElement(e02));
  CPPUNIT_ASSERT(quotient->isElement(edges[1]));
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
void ExtendedClusterOperationTest::testOpenMetaNodes() {

  meta = quotient->createMetaNode(group);
  group.clear();
  group.insert(nodes[3]);
  group.insert(nodes[4]);
  node meta2 = quotient->createMetaNode(group);
  quotient->openMetaNode(meta);
  quotient->openMetaNode(meta2);
  
  CPPUNIT_ASSERT_EQUAL(false, quotient->isElement(meta));
  CPPUNIT_ASSERT_EQUAL(false, graph->isElement(meta));
  CPPUNIT_ASSERT_EQUAL(false, quotient->isElement(meta2));
  CPPUNIT_ASSERT_EQUAL(false, graph->isElement(meta2));

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
  suiteOfTests->addTest( new CppUnit::TestCaller<ExtendedClusterOperationTest>( "Ungroup MetaNodes in reverse order", 
								    &ExtendedClusterOperationTest::testOpenMetaNodes ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<ExtendedClusterOperationTest>( "Open a metanode in a hierarchy of subgraph #BUG-1", 
										&ExtendedClusterOperationTest::testBugOpenInSubgraph ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<ExtendedClusterOperationTest>( "Create a metanode on a non simple graph #BUG-1", 
										&ExtendedClusterOperationTest::testBugCreateOpenMetaNode ) );
  return suiteOfTests;
}
//==========================================================
