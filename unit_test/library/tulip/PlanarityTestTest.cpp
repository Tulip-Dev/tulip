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
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/PlanarityTest.h>
#include <tulip/PlanarConMap.h>
#include <tulip/ConnectedTest.h>
#include <tulip/ExtendedClusterOperation.h>
#include "PlanarityTestTest.h"

using namespace std;
using namespace tlp;

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
unsigned int eulerIdentity(Graph *graph) {
  return graph->numberOfEdges() - graph->numberOfNodes() 
    + 1u + ConnectedTest::numberOfConnectedComponents(graph);
}
//==========================================================
void PlanarityTestTest::planarGraphsEmbedding() {
  cerr << "==================================" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/grid1010.tlp");
  PlanarConMap *graphMap = new PlanarConMap(graph);
  //  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces());  
  delete graphMap;
  delete graph;
  cerr << "==================================" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/unconnected.tlp");
  graph->setAttribute("name", string("unconnected"));
  graphMap = new PlanarConMap(graph);
  cerr << "Graph name : " << graph->getAttribute<string>("name") << endl;
  //  graphMap->makePlanar();
  /* 
   * The number of faces must be adapted because the Planarity Test split the 
   * external face into several faces (one by connected componnent).
   */
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces() - (ConnectedTest::numberOfConnectedComponents(graph) - 1));  
  delete graphMap;
  delete graph;
  cerr << "==================================" << endl;
  cerr << "unbiconnected" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");

  graphMap = new PlanarConMap(graph);

  //  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces());  

  delete graphMap;
  delete graph;
  cerr << "==================================" << endl;
}
//==========================================================
void PlanarityTestTest::planarMetaGraphsEmbedding() {
  cerr << "===========MetaGraphsEmbedding=======================" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/grid1010.tlp");
  Graph * g= tlp::newCloneSubGraph(graph);
  set<node> toGroup;
  Iterator<node> * itn = g->getNodes();
  for(unsigned int i = 0; i < 10; ++i)
    toGroup.insert(itn->next());
  node meta = g->createMetaNode(toGroup);
  toGroup.clear();
  for(unsigned int i = 0; i < 10; ++i)
    toGroup.insert(itn->next());
  node meta2 = g->createMetaNode(toGroup);
  toGroup.clear();
  toGroup.insert(meta2);
  for(unsigned int i = 0; i < 10; ++i)
    toGroup.insert(itn->next());
  node meta3 = g->createMetaNode(toGroup);
  toGroup.clear();

  PlanarConMap *graphMap = new PlanarConMap(g);
  //  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(true, PlanarityTest::isPlanar(g));//eulerIdentity(g), graphMap->nbFaces());  
  CPPUNIT_ASSERT_EQUAL(true, PlanarityTest::isPlanar(graphMap));//eulerIdentity(g), graphMap->nbFaces());  
  delete graphMap;
  graph->delSubGraph(g);
  delete graph;
  cerr << "==================================" << endl;
  /*
  graph = tlp::loadGraph(GRAPHPATH + "planar/unconnected.tlp");
  graph->setAttribute("name", string("unconnected"));
  graphMap = new PlanarConMap(graph);
  cerr << "Graph name : " << graph->getAttribute<string>("name") << endl;
  graphMap->makePlanar();*/
  /* 
   * The number of faces must be adapted because the Planarity Test split the 
   * external face into several faces (one by connected componnent).
   */
  /*  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces() - (ConnectedTest::numberOfConnectedComponnents(graph) - 1));  
  delete graphMap;
  delete graph;
  cerr << "==================================" << endl;
  cerr << "unbiconnected" << endl;
  graph = tlp::loadGraph(GRAPHPATH + "planar/unbiconnected.tlp");

  graphMap = new PlanarConMap(graph);

  graphMap->makePlanar();
  CPPUNIT_ASSERT_EQUAL(eulerIdentity(graph), graphMap->nbFaces());  

  delete graphMap;
  delete graph;
  cerr << "==================================" << endl;*/
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
  suiteOfTests->addTest( new CppUnit::TestCaller<PlanarityTestTest>( "planar graph embedding", 
  								     &PlanarityTestTest::planarMetaGraphsEmbedding ) );
  
  return suiteOfTests;
}
//==========================================================
