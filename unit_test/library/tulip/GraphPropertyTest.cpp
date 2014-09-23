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
#include <tulip/TlpTools.h>
#include "GraphPropertyTest.h"

using namespace std;
using namespace tlp;

tlp::Graph* nullGraph = NULL;

CPPUNIT_TEST_SUITE_REGISTRATION( GraphPropertyTest );

//==========================================================
void GraphPropertyTest::setUp() {
  graph    = tlp::newGraph();
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
void GraphPropertyTest::tearDown() {
  delete graph;
}
//==========================================================
void GraphPropertyTest::testDestroyGraph() {
  //build the hierarchy
  Graph * g1 = graph->addCloneSubGraph("G1");
  Graph * g2 = graph->addCloneSubGraph("G2");
  Graph * meta1 = graph->addSubGraph("META1");
  GraphProperty * proxy1 = meta1->getLocalProperty<GraphProperty>("viewMetaGraph");
  node mnode1 = meta1->addNode();
  node mnode2 = meta1->addNode();
  proxy1->setNodeValue(mnode1, g1);
  proxy1->setNodeValue(mnode2, g2);
  graph->delSubGraph(g2);
  CPPUNIT_ASSERT_EQUAL(nullGraph, proxy1->getNodeValue(mnode2));
  CPPUNIT_ASSERT_EQUAL(g1, proxy1->getNodeValue(mnode1));
  graph->delSubGraph(g1);
  CPPUNIT_ASSERT_EQUAL(nullGraph, proxy1->getNodeValue(mnode2));
  CPPUNIT_ASSERT_EQUAL(nullGraph, proxy1->getNodeValue(mnode1));
}
//==========================================================
void GraphPropertyTest::testSetGet() {
  //build the hierarchy
  Graph * g1 = graph->addCloneSubGraph("G1");
  Graph * g2 = graph->addCloneSubGraph("G2");
  Graph * g3 = graph->addCloneSubGraph("G3");
  Graph * meta1 = graph->addSubGraph("META1");
  GraphProperty * proxy1 = meta1->getLocalProperty<GraphProperty>("viewMetaGraph");
  node mnode1 = meta1->addNode();
  node mnode2 = meta1->addNode();
  proxy1->setNodeValue(mnode1, g1);
  proxy1->setNodeValue(mnode2, g2);
  proxy1->setNodeValue(mnode2, g3);
  graph->delSubGraph(g2);
  CPPUNIT_ASSERT_EQUAL(g3, proxy1->getNodeValue(mnode2));
  CPPUNIT_ASSERT_EQUAL(g1, proxy1->getNodeValue(mnode1));
}
//==========================================================
void GraphPropertyTest::testSetAll() {
  //build the hierarchy
  Graph * g1 = graph->addCloneSubGraph("G1");
  Graph * g2 = graph->addCloneSubGraph("G2");
  Graph * g3 = graph->addCloneSubGraph("G3");
  Graph * meta1 = graph->addSubGraph("META1");
  GraphProperty proxy(meta1);
  node mnode1 = meta1->addNode();
  node mnode2 = meta1->addNode();
  node mnode3 = meta1->addNode();
  proxy.setAllNodeValue(g3);
  proxy.setNodeValue(mnode1, g1);
  proxy.setNodeValue(mnode2, g2);
  CPPUNIT_ASSERT_EQUAL(g1, proxy.getNodeValue(mnode1));
  CPPUNIT_ASSERT_EQUAL(g2, proxy.getNodeValue(mnode2));
  CPPUNIT_ASSERT_EQUAL(g3, proxy.getNodeValue(mnode3));
  graph->delSubGraph(g3);
  CPPUNIT_ASSERT_EQUAL(g1, proxy.getNodeValue(mnode1));
  CPPUNIT_ASSERT_EQUAL(g2, proxy.getNodeValue(mnode2));
  CPPUNIT_ASSERT_EQUAL(nullGraph , proxy.getNodeValue(mnode3));
  proxy.setAllNodeValue(0);
  graph->delSubGraph(g1);
  graph->delSubGraph(g2);
}
