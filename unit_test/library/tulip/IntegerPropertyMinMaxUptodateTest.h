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

#ifndef INTEGERPROPERTYMINMAXUPDATETEST_H_
#define INTEGERPROPERTYMINMAXUPDATETEST_H_

#include <tulip/Graph.h>
#include <tulip/IntegerProperty.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class IntegerPropertyMinMaxUpdateTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(IntegerPropertyMinMaxUpdateTest);
  CPPUNIT_TEST(testIntegerPropertyMinUpdate);
  CPPUNIT_TEST(testIntegerPropertyMaxUpdate);
  CPPUNIT_TEST(testIntegerPropertyMinUpdateFromString);
  CPPUNIT_TEST(testIntegerPropertyMaxUpdateFromString);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void testIntegerPropertyMinUpdate();
  void testIntegerPropertyMaxUpdate();
  void testIntegerPropertyMinUpdateFromString();
  void testIntegerPropertyMaxUpdateFromString();

private :
  tlp::Graph *graph;
  tlp::node n1, n4;
};

#endif /* INTEGERPROPERTYMINMAXUPDATETEST_H_ */
