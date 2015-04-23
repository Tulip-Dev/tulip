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
#include <cassert>
#include <vector>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/ForEach.h>

#include "ExtendedClusterOperationTest.h"

using namespace std;
using namespace tlp;

static Graph* tlp_loadGraph(const std::string& filename) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *sg = tlp::importGraph("TLP Import", dataSet);
  return sg;
}

CPPUNIT_TEST_SUITE_REGISTRATION( ExtendedClusterOperationTest );
//==========================================================
void ExtendedClusterOperationTest::setUp() {
  //  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  graph = tlp::newGraph();

  for (unsigned int i=0; i<5; ++i) {
    nodes.push_back(graph->addNode());
  }

  edges.push_back(graph->addEdge(nodes[0], nodes[1]));
  edges.push_back(graph->addEdge(nodes[0], nodes[2]));
  edges.push_back(graph->addEdge(nodes[0], nodes[3]));
  edges.push_back(graph->addEdge(nodes[1], nodes[3]));
  edges.push_back(graph->addEdge(nodes[1], nodes[4]));
  edges.push_back(graph->addEdge(nodes[2], nodes[3]));
  edges.push_back(graph->addEdge(nodes[4], nodes[0]));

  quotient = graph->addCloneSubGraph();

  group.insert(nodes[0]);
  group.insert(nodes[1]);
}
//==========================================================
void ExtendedClusterOperationTest::tearDown() {
  //  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  delete graph;
  nodes.clear();
  edges.clear();
  group.clear();
}
//==========================================================
void ExtendedClusterOperationTest::testCreateMetaNode() {
  tlp::warning() << endl << "L'erreur à suivre sur createMetaNode fait partie de testCreateMetaNode" << endl;
  meta = graph->createMetaNode(group);
  CPPUNIT_ASSERT(!meta.isValid());

  meta = quotient->createMetaNode(group);
  CPPUNIT_ASSERT(!quotient->isElement(nodes[0]));
  CPPUNIT_ASSERT(!quotient->isElement(nodes[1]));
  CPPUNIT_ASSERT(quotient->existEdge(meta, nodes[4]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(meta, nodes[2]).isValid());
  std::vector<edge> mEdges = quotient->getEdges(meta, nodes[3]);
  CPPUNIT_ASSERT(mEdges.size() == 2);
  Iterator<edge>* ite = quotient->getEdgeMetaInfo(mEdges[0]);
  CPPUNIT_ASSERT(ite->next() == edges[2]);
  CPPUNIT_ASSERT(ite->hasNext() == false);
  delete ite;
  ite = quotient->getEdgeMetaInfo(mEdges[1]);
  CPPUNIT_ASSERT(ite->next() == edges[3]);
  CPPUNIT_ASSERT(ite->hasNext() == false);
  delete ite;
  
  mEdges.clear();
  mEdges = quotient->getEdges(meta, nodes[4]);
  CPPUNIT_ASSERT(mEdges.size() == 1);
  ite = quotient->getEdgeMetaInfo(mEdges[0]);
  CPPUNIT_ASSERT(ite->next() == edges[4]);
  CPPUNIT_ASSERT(ite->hasNext() == false);
  delete ite;
  mEdges.clear();
  mEdges = quotient->getEdges(meta, nodes[4], false);
  CPPUNIT_ASSERT(mEdges.size() == 2);
  ite = quotient->getEdgeMetaInfo(mEdges[0]);
  edge e = ite->next();
  CPPUNIT_ASSERT(e == edges[4] || e == edges[6]);
  CPPUNIT_ASSERT(ite->hasNext() == false);
  delete ite;
  ite = quotient->getEdgeMetaInfo(mEdges[1]);
  e = ite->next();
  CPPUNIT_ASSERT(e == edges[4] || e == edges[6]);
  CPPUNIT_ASSERT(ite->hasNext() == false);
  delete ite;
  
  CPPUNIT_ASSERT_EQUAL(6u, quotient->numberOfEdges());
  CPPUNIT_ASSERT_EQUAL(6u, graph->numberOfNodes());

  Graph *cluster = quotient->getNodeMetaInfo(meta);
  CPPUNIT_ASSERT(cluster!=NULL);
  CPPUNIT_ASSERT(cluster->isElement(nodes[0]));
  CPPUNIT_ASSERT(cluster->isElement(nodes[1]));
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
  }

  delete ite;
  CPPUNIT_ASSERT_EQUAL(2u, nbEdges);

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
  }

  delete ite;
  CPPUNIT_ASSERT_EQUAL(2u, nbEdges);

  quotient->openMetaNode(meta);
  CPPUNIT_ASSERT(!quotient->isElement(metaE));
  CPPUNIT_ASSERT(quotient->isElement(e02));
  CPPUNIT_ASSERT(quotient->isElement(edges[1]));
}
//==========================================================
void ExtendedClusterOperationTest::testBugOpenInSubgraph() {
  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  Graph * graph = tlp_loadGraph("./DATA/graphs/openmetanode1.tlp.gz");
  //take the quotient graph
  Graph * subgraph = 0;
  bool find = false;
  forEach(subgraph, graph->getSubGraphs()) {
    string name;
    subgraph->getAttribute("name", name);

    if (name == "unnamed") {
      find = true;
      break;
    }
  }
  CPPUNIT_ASSERT(find && subgraph != NULL);
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

  CPPUNIT_ASSERT(!quotient->isElement(meta));
  CPPUNIT_ASSERT(!graph->isElement(meta));

  CPPUNIT_ASSERT_EQUAL(5u, graph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(7u, graph->numberOfEdges());

  CPPUNIT_ASSERT_EQUAL(5u, quotient->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(7u, quotient->numberOfEdges());

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

  CPPUNIT_ASSERT(!quotient->isElement(meta));
  CPPUNIT_ASSERT(!graph->isElement(meta));
  CPPUNIT_ASSERT(!quotient->isElement(meta2));
  CPPUNIT_ASSERT(!graph->isElement(meta2));

  CPPUNIT_ASSERT_EQUAL(5u, graph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(7u, graph->numberOfEdges());

  CPPUNIT_ASSERT_EQUAL(5u, quotient->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(7u, quotient->numberOfEdges());

  CPPUNIT_ASSERT(quotient->existEdge(nodes[0], nodes[1]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(nodes[0], nodes[2]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(nodes[1], nodes[3]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(nodes[1], nodes[4]).isValid());
  CPPUNIT_ASSERT(quotient->existEdge(nodes[2], nodes[3]).isValid());

}
