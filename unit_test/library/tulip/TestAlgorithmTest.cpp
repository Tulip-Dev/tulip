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
#include <string>
#include <tulip/Graph.h>
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
using namespace tlp;

static Graph* tlp_loadGraph(const std::string& filename) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *sg = tlp::importGraph("TLP Import", dataSet);
  return sg;
}

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
  tlp::warning() << __FUNCTION__ << " : not implemented" << endl;
}
//==========================================================
void TestAlgorithmTest::testFreeTree() {
  node n1 = graph->addNode();
  edge e = graph->addEdge(n1, n1);
  CPPUNIT_ASSERT(!TreeTest::isFreeTree(graph));
  graph->delEdge(e);
  CPPUNIT_ASSERT(TreeTest::isFreeTree(graph));
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  edge e0 = graph->addEdge(n1, n2);
  edge e1 = graph->addEdge(n3, n1);
  CPPUNIT_ASSERT(TreeTest::isFreeTree(graph));
  node n4 = graph->addNode();
  CPPUNIT_ASSERT(!TreeTest::isFreeTree(graph));
  graph->addEdge(n4, n1);
  CPPUNIT_ASSERT(TreeTest::isFreeTree(graph));
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  Graph* clone = graph->addCloneSubGraph();
  CPPUNIT_ASSERT(TreeTest::isFreeTree(clone));
  clone->reverse(e1);
  CPPUNIT_ASSERT(TreeTest::isFreeTree(graph));
  CPPUNIT_ASSERT(TreeTest::isFreeTree(clone));
  clone->reverse(e0);
  CPPUNIT_ASSERT(TreeTest::isFreeTree(clone));
  CPPUNIT_ASSERT(TreeTest::isFreeTree(graph));
  clone->delEdge(e1);
  CPPUNIT_ASSERT(TreeTest::isFreeTree(graph));
  CPPUNIT_ASSERT(!TreeTest::isFreeTree(clone));
  clone->delNode(n3);
  CPPUNIT_ASSERT(TreeTest::isFreeTree(graph));
  CPPUNIT_ASSERT(TreeTest::isFreeTree(clone));
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
  Graph* clone = graph->addCloneSubGraph();
  CPPUNIT_ASSERT(TreeTest::isTree(clone));
  graph->reverse(e1);
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  CPPUNIT_ASSERT(!TreeTest::isTree(clone));
  clone->reverse(e0);
  CPPUNIT_ASSERT(!TreeTest::isTree(clone));
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  graph->reverse(e2);
  clone->delNode(n3);
  CPPUNIT_ASSERT(!TreeTest::isTree(graph));
  CPPUNIT_ASSERT(TreeTest::isTree(clone));
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
  //TODO check this test, as it was not executed for some time
  graph->clear();
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  graph->addEdge(n1, n2);
  graph->addEdge(n1, n3);
  Graph* clone = graph->addCloneSubGraph();
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(graph));
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(clone));
  clone->addEdge(n2, n3);
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(graph));
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(clone));
  edge e2 = clone->addEdge(n3, n1);
  CPPUNIT_ASSERT(!AcyclicTest::isAcyclic(graph));
  CPPUNIT_ASSERT(!AcyclicTest::isAcyclic(clone));
  clone->reverse(e2);
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(graph));
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(clone));
  clone->delEdge(e2);
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(graph));
  CPPUNIT_ASSERT(AcyclicTest::isAcyclic(clone));
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
  CPPUNIT_ASSERT_EQUAL(size_t(1), addedEdge.size());
  graph->delEdge(addedEdge[0]);
  CPPUNIT_ASSERT_EQUAL(2u, ConnectedTest::numberOfConnectedComponents(graph));
}
//==========================================================
const std::string GRAPHPATH = "./DATA/graphs/";

void TestAlgorithmTest::testBiconnected() {
  node n[10];
  edge e[10];

  for (int i=0; i<4; ++i)
    n[i]=graph->addNode();

  for (int i=0; i<4; ++i)
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

  for (int i=0; i<5; ++i)
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

  Graph *tmpGraph;

  for (unsigned int i = 0; i<5; ++i) {
    tmpGraph = tlp_loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");
    CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(tmpGraph));
    vector<edge> vEdges;
    BiconnectedTest::makeBiconnected(tmpGraph, vEdges);
    CPPUNIT_ASSERT(BiconnectedTest::isBiconnected(tmpGraph));

    for (vector<edge>::iterator it = vEdges.begin(); it != vEdges.end(); ++it) {
      tmpGraph->delEdge(*it, true);
    }

    CPPUNIT_ASSERT(!BiconnectedTest::isBiconnected(tmpGraph));
    delete tmpGraph;
  }

}
//==========================================================
void TestAlgorithmTest::testTriconnected() {
  tlp::warning() << __FUNCTION__ << " : not implemented" << endl;
}
