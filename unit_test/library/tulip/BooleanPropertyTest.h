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
#ifndef TLPSELECTIONPROXYTEST
#define TLPSELECTIONPROXYTEST

#include <string>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
class BooleanProperty;
}

class BooleanPropertyTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(BooleanPropertyTest);
  CPPUNIT_TEST(testSetAll);
  CPPUNIT_TEST(testSetGet);
  CPPUNIT_TEST(testCopy);
  CPPUNIT_TEST(testIterators);
  CPPUNIT_TEST(testDelete);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;
  tlp::BooleanProperty *selection;

public:
  void setUp();
  void tearDown();
  void testSetAll();
  void testSetAll(bool);
  void testSetGet();
  void testSetGet(bool);
  void testCopy();
  void testIterators();
  void testDelete(bool value);
  void testDelete();
};

#endif
