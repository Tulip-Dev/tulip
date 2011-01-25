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
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "GraphToolsTest.h"
#include <tulip/GraphTools.h>
#include <tulip/DoubleProperty.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( GraphToolsTest );

//==========================================================
void GraphToolsTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void GraphToolsTest::tearDown() {
  delete graph;
}
//==========================================================
void GraphToolsTest::build(unsigned int nbNodes, unsigned int edgeRatio) {
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_ADD  = nbNodes;
  unsigned int EDGE_RATIO = edgeRatio;
  for (unsigned int i=0; i<NB_ADD; ++i)
    nodes.push_back(graph->addNode());
  unsigned int NB_EDGES = EDGE_RATIO * NB_ADD;
  for (unsigned int i=0; i< NB_EDGES; ++i)
    graph->addEdge(nodes[rand()%NB_ADD], nodes[rand()%NB_ADD]);
}
//==========================================================
void GraphToolsTest::testComputeEqualValueClustering() {
  DoubleProperty metric(graph); 
  // check minimum call to computeEqualValueClustering
  // with an empty graph
  CPPUNIT_ASSERT(computeEqualValueClustering(graph, &metric));

  // fill graph & metric
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_ADD  = 100;
  unsigned int EDGE_RATIO = 100;
  for (unsigned int i=0; i<NB_ADD; ++i) {
    nodes.push_back(graph->addNode());
    metric.setNodeValue(nodes[i], (double) (rand()%NB_ADD));
  }
  unsigned int NB_EDGES = EDGE_RATIO * NB_ADD;
  for (unsigned int i=0; i< NB_EDGES; ++i)
    graph->addEdge(nodes[rand()%NB_ADD], nodes[rand()%NB_ADD]);
  
  // check dcall to computeEqualValueClustering
  CPPUNIT_ASSERT(computeEqualValueClustering(graph, &metric));
}
//==========================================================
CppUnit::Test* GraphToolsTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Graph" );
  suiteOfTests->addTest( new CppUnit::TestCaller<GraphToolsTest>( "computeEqualValueClustering", 
								  &GraphToolsTest::testComputeEqualValueClustering) );

  return suiteOfTests;
}
//==========================================================
