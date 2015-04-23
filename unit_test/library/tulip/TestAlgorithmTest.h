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
#ifndef TLPTESTALGORITHMTEST
#define TLPTESTALGORITHMTEST

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
class Graph;
}

class TestAlgorithmTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestAlgorithmTest);
  CPPUNIT_TEST(testSimple);
  CPPUNIT_TEST(testFreeTree);
  CPPUNIT_TEST(testTree);
  CPPUNIT_TEST(testAcyclic);
  CPPUNIT_TEST(testConnected);
  CPPUNIT_TEST(testBiconnected);
  CPPUNIT_TEST(testTriconnected);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();
  void testSimple();
  void testFreeTree();
  void testTree();
  void testAcyclic();
  void testConnected();
  void testBiconnected();
  void testTriconnected();
};

#endif
