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

#ifndef EXISTEDGETEST_H_
#define EXISTEDGETEST_H_

#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class ExistEdgeTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(ExistEdgeTest);
  CPPUNIT_TEST(testExistEdge);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void testExistEdge();

private :
  tlp::Graph *graph;
  tlp::node n0, n1;
  tlp::edge e0, e1;
};

#endif /* EXISTEDGETEST_H_ */
