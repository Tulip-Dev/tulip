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
#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class ExtendedClusterOperationTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(ExtendedClusterOperationTest);
  CPPUNIT_TEST(testCreateMetaNode);
  CPPUNIT_TEST(testBugCreateOpenMetaNode);
  CPPUNIT_TEST(testBugOpenInSubgraph);
  CPPUNIT_TEST(testOpenMetaNode);
  CPPUNIT_TEST(testOpenMetaNodes);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph,*quotient;
  tlp::node meta;
  std::vector<tlp::node> nodes;
  std::vector<tlp::edge> edges;
  std::set<tlp::node> group;
public:
  void setUp();
  void tearDown();
  void testCreateMetaNode();
  void testBugCreateOpenMetaNode();
  void testBugOpenInSubgraph();
  void testOpenMetaNode();
  void testOpenMetaNodes();
};

#endif
