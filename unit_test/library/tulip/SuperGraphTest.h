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
#ifndef TLPSUPERGRAPHTEST
#define TLPSUPERGRAPHTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class SuperGraphTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(SuperGraphTest);
  CPPUNIT_TEST(testAddDel);
  CPPUNIT_TEST(testClear);
  CPPUNIT_TEST(testOrderEdgeAndSwap);
  CPPUNIT_TEST(testSubgraph);
  CPPUNIT_TEST(testSubgraphId);
  CPPUNIT_TEST(testDeleteSubgraph);
  CPPUNIT_TEST(testInheritance);
  CPPUNIT_TEST(testIterators);
  CPPUNIT_TEST(testPropertiesIteration);
  CPPUNIT_TEST(testDegree);
  CPPUNIT_TEST(testAttributes);
  CPPUNIT_TEST(testGetNodesEqualTo);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();

  void testAddDel();
  void testClear();
  void testOrderEdgeAndSwap();
  void testSubgraph();
  void testSubgraphId();
  void testDeleteSubgraph();
  void testInheritance();
  void testIterators();
  void testPropertiesIteration();
  void testDegree();
  void testAttributes();
  void testGetNodesEqualTo();
private:
  void build(unsigned int, unsigned int);
};

#endif
