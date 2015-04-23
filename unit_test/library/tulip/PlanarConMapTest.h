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

#ifndef PLANARCONMAPTEST_H
#define PLANARCONMAPTEST_H

#include <tulip/PlanarConMap.h>
#include <tulip/TlpTools.h>

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {

class PlanarConMapTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PlanarConMapTest);
  CPPUNIT_TEST(testAddEdgeMap);
  CPPUNIT_TEST(testDelEdgeMap);
  CPPUNIT_TEST(testNbFaces);
  CPPUNIT_TEST(testUpdate);
  CPPUNIT_TEST(testMergeFaces);
  CPPUNIT_TEST(testSplitFace);
  CPPUNIT_TEST(testSuccCycleEdge);
  CPPUNIT_TEST(testPrecCycleEdge);
  CPPUNIT_TEST(testComputeFaces);
  CPPUNIT_TEST_SUITE_END();

private :
  tlp::Graph* graph;
  tlp::PlanarConMap* carte;

  std::vector<edge> edges;
  std::vector<node> nodes;

  void build();
  void build2();
  void build3();
  void build4();

public :
  void setUp() {
    graph = tlp::newGraph();
    //carte = new PlanarConMap(graph);
    edges.clear();
    nodes.clear();
  }

  void tearDown() {
    delete carte;
    delete graph;
  }

  void testAddEdgeMap();
  void testDelEdgeMap();
  void testNbFaces();

  void testUpdate();

  void testMergeFaces();
  void testSplitFace();

  void testSuccCycleEdge();
  void testPrecCycleEdge();

  void testComputeFaces();
};

}
#endif
