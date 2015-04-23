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
#ifndef BASICLAYOUTTEST_H
#define BASICLAYOUTTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
class Graph;
}

class BasicLayoutTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(BasicLayoutTest);
  CPPUNIT_TEST(testBubbleTree);
  CPPUNIT_TEST(testCircular);
  CPPUNIT_TEST(testConeTreeExtended);
  CPPUNIT_TEST(testConnectedComponentPacking);
  CPPUNIT_TEST(testDendrogram);
  CPPUNIT_TEST(testGEMLayout);
  CPPUNIT_TEST(testHierarchicalGraph);
  CPPUNIT_TEST(testImprovedWalker);
  CPPUNIT_TEST(testMixedModel);
  CPPUNIT_TEST(testRandomLayout);
  CPPUNIT_TEST(testSquarifiedTreeMap);
  CPPUNIT_TEST(testTreeLeaf);
  CPPUNIT_TEST(testTreeMap);
  CPPUNIT_TEST(testTreeRadial);
  CPPUNIT_TEST(testTreeReingoldAndTilfordExtended);
  CPPUNIT_TEST(testTutte);
  CPPUNIT_TEST(testInducedSubGraphSelection);
  CPPUNIT_TEST(testKruskal);
  CPPUNIT_TEST(testFastOverlapRemoval);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;
  void initializeGraph(const std::string& type);
  template<typename PropType>
  bool computeProperty(const std::string &algorithm, const std::string& graphType = "Planar Graph", PropType* prop = NULL);
public:
  virtual void setUp();
  virtual void tearDown();

  void testBubbleTree();
  void testCircular();
  void testConeTreeExtended();
  void testConnectedComponentPacking();
  void testDendrogram();
  void testGEMLayout();
  void testHierarchicalGraph();
  void testImprovedWalker();
  void testMixedModel();
  void testRandomLayout();
  void testSquarifiedTreeMap();
  void testTreeLeaf();
  void testTreeMap();
  void testTreeRadial();
  void testTreeReingoldAndTilfordExtended();
  void testTutte();
  void testInducedSubGraphSelection();
  void testKruskal();
  void testFastOverlapRemoval();
};

#endif // BASICLAYOUTTEST_H
