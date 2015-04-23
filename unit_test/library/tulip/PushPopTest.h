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
#ifndef TLPPUSHPOPTEST
#define TLPPUSHPOPTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class PushPopTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PushPopTest);
  CPPUNIT_TEST(testAddDel);
  CPPUNIT_TEST(testSetValue);
  CPPUNIT_TEST(testSetEltValue);
  CPPUNIT_TEST(testVectorValue);
  CPPUNIT_TEST(testSetEnds);
  CPPUNIT_TEST(testCopyProperty);
  CPPUNIT_TEST(testSubgraph);
  CPPUNIT_TEST(testPushAddSubgraphsPop);
  CPPUNIT_TEST(testDelSubgraph);
  CPPUNIT_TEST(testDelAllSgPopUnpop);
  CPPUNIT_TEST(testDeletePushPopFalse);
  CPPUNIT_TEST(testTests);
  CPPUNIT_TEST(testAddDelProps);
  CPPUNIT_TEST(testRenameProps);
  CPPUNIT_TEST(testObserveDelProps);
  CPPUNIT_TEST(testAddSubgraphProp);
  CPPUNIT_TEST(testMetaNode);

  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();

  void testAddDel();
  void testSetValue();
  void testSetEltValue();
  void testVectorValue();
  void testSetEnds();
  void testCopyProperty();
  void testSubgraph();
  void testPushAddSubgraphsPop();
  void testDelSubgraph();
  void testDelAllSgPopUnpop();
  void testDeletePushPopFalse();
  void testTests();
  void testAddDelProps();
  void testRenameProps();
  void testObserveDelProps();
  void testAddSubgraphProp();
  void testMetaNode();
private:
  void build(unsigned int, unsigned int);
};

#endif
