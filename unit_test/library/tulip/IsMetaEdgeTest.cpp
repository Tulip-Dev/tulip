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
#include <tulip/BooleanProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/ForEach.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>

#include "IsMetaEdgeTest.h"

using namespace tlp;
using namespace std;

const unsigned int nbNodesPerCluster = 5;

void createCompleteGraphAndSelect(Graph *graph, BooleanProperty *selection) {

  vector<node> addedNodes;

  for (unsigned int j=0; j<nbNodesPerCluster; ++j) {
    node n = graph->addNode();
    addedNodes.push_back(n);
    selection->setNodeValue(n, true);
  }

  for (size_t i = 0 ; i < addedNodes.size() ; ++i) {
    for (size_t j = 0 ; j < addedNodes.size() ; ++j) {
      if (addedNodes[i] != addedNodes[j]) {
        edge e = graph->addEdge(addedNodes[i], addedNodes[j]);
        selection->setEdgeValue(e, true);
      }
    }
  }
}

CPPUNIT_TEST_SUITE_REGISTRATION( IsMetaEdgeTest );

void IsMetaEdgeTest::setUp() {

  graph = tlp::newGraph();
  BooleanProperty selec(graph);

  // create a first complete graph that will be our first cluster
  createCompleteGraphAndSelect(graph, &selec);
  Graph *sg1 = graph->addSubGraph(&selec);

  selec.setAllNodeValue(false);
  selec.setAllEdgeValue(false);

  // create another complete graph that will be our second cluster
  createCompleteGraphAndSelect(graph, &selec);
  Graph *sg2 = graph->addSubGraph(&selec);

  // link each node of the first cluster to each node of the second cluster
  node n1;
  forEach(n1, sg1->getNodes()) {
    node n2;
    forEach(n2, sg2->getNodes()) {
      graph->addEdge(n1, n2);
    }
  }

  // Create our quotient graph, should contain two meta-nodes and one meta-edge
  quotientGraph = graph->addSubGraph();
  Iterator<Graph *> *itS = graph->getSubGraphs();
  vector<node> mNodes;
  graph->createMetaNodes(itS, quotientGraph, mNodes);
  delete itS;

}

void IsMetaEdgeTest::tearDown() {
  graph->delAllSubGraphs(quotientGraph);
  delete graph;
}

void IsMetaEdgeTest::testIsMetaEdge() {
  unsigned int nbMetaNodes = 0;
  unsigned int nbMetaEdges = 0;

  // check if we have two nodes and one edge in the quotient
  CPPUNIT_ASSERT_EQUAL(2u, quotientGraph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(1u, quotientGraph->numberOfEdges());

  node n;
  forEach(n, quotientGraph->getNodes()) {
    if (quotientGraph->isMetaNode(n)) {
      ++nbMetaNodes;
    }
  }

  edge e;
  forEach(e, quotientGraph->getEdges()) {
    if (quotientGraph->isMetaEdge(e)) {
      ++nbMetaEdges;
    }
  }

  // check that the two quotient nodes are meta nodes
  CPPUNIT_ASSERT_EQUAL(2u, nbMetaNodes);


  set<edge> underlyingEdgesInMetaEdge = quotientGraph->getProperty<GraphProperty>("viewMetaGraph")->getEdgeValue(quotientGraph->getOneEdge());

  // check the number of underlying edges in meta edge
  CPPUNIT_ASSERT_EQUAL((size_t)nbNodesPerCluster * nbNodesPerCluster, underlyingEdgesInMetaEdge.size());

  // check if the quotient edge is a meta edge
  CPPUNIT_ASSERT_EQUAL(1u, nbMetaEdges);
}
