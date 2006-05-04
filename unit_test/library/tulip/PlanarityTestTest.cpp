/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
 */

#include <cassert>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/TlpTools.h>
#include <tulip/PlanarityTest.h>
#include <tulip/SuperGraphMap.h>
#include <tulip/ConnectedTest.h>
#include "PlanarityTestTest.h"

using namespace std;

const std::string GRAPHPATH = "./DATA/graphs/";

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( PlanarityTestTest );
//==========================================================
void PlanarityTestTest::setUp() {
  //  graph = tlp::newGraph();
}
//==========================================================
void PlanarityTestTest::tearDown() {
  //  delete graph;
}
//==========================================================
void PlanarityTestTest::planarGraphs() {
  graph = tlp::loadGraph(GRAPHPATH + "planar/grid1010.tlp");
  CPPUNIT_ASSERT_EQUAL(true, PlanarityTest::isPlanar(graph));
  delete graph;
  graph = tlp::loadGraph(GRAPHPATH + "planar/unconnected.tlp");
  CPPUNIT_ASSERT_EQUAL(true, PlanarityTest::isPlanar(graph));
  delete graph;
  graph = tlp::loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");
  CPPUNIT_ASSERT_EQUAL(true, PlanarityTest::isPlanar(graph));
  delete graph;
}
//==========================================================
void PlanarityTestTest::notPlanarGraphs() {
  graph = tlp::loadGraph(GRAPHPATH + "notplanar/k33lostInGrip.tlp.gz");
  CPPUNIT_ASSERT_EQUAL(false, PlanarityTest::isPlanar(graph));
  delete graph;
  graph = tlp::loadGraph(GRAPHPATH + "notplanar/k33k55.tlp.gz");
  CPPUNIT_ASSERT_EQUAL(false, PlanarityTest::isPlanar(graph));
  delete graph;  
  graph = tlp::loadGraph(GRAPHPATH + "notplanar/k5lostingrid5050.tlp.gz");
  CPPUNIT_ASSERT_EQUAL(false, PlanarityTest::isPlanar(graph));
  delete graph;
}
//==========================================================
unsigned int eulerIdentity(SuperGraph *graph) {
  return graph->numberOfEdges() - graph->numberOfNodes() 
    + 1u + ConnectedTest::numberOfConnectedComponnents(graph);
}
//==========================================================
void PlanarityTestTest::planarGraphsEmbedding() {
  cerr << "==================================" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/grid1010.tlp");
  SuperGraphMap *graphMap = new SuperGraphMap(graph);
  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces());  
  delete graphMap;
  delete graph;
  cerr << "==================================" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/unconnected.tlp");
  graph->setAttribute("name", string("unconnected"));
  graphMap = new SuperGraphMap(graph);
  cerr << "Graph name : " << graph->getAttribute<string>("name") << endl;
  graphMap->makePlanar();
  /* 
   * The number of faces must be adapted because the Planarity Test split the 
   * external face into several faces (one by connected componnent).
   */
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces() - (ConnectedTest::numberOfConnectedComponnents(graph) - 1));  
  delete graphMap;
  delete graph;
  cerr << "==================================" << endl;
  cerr << "unbiconnected" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");

  graphMap = new SuperGraphMap(graph);

  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces());  

  delete graphMap;
  delete graph;
  cerr << "==================================" << endl;
}
//==========================================================
void PlanarityTestTest::notPlanarGraphsObstruction() {
  
}
//==========================================================
CppUnit::Test * PlanarityTestTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Planarity Test" );
  suiteOfTests->addTest( new CppUnit::TestCaller<PlanarityTestTest>( "planar graph", 
								     &PlanarityTestTest::planarGraphs ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PlanarityTestTest>( "not planar graph", 
								     &PlanarityTestTest::notPlanarGraphs ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PlanarityTestTest>( "planar graph embedding", 
  								     &PlanarityTestTest::planarGraphsEmbedding ) );

  return suiteOfTests;
}
//==========================================================
