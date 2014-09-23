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
#ifndef DATASETTEST
#define DATASETTEST

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {

class DataSetTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(DataSetTest);
  CPPUNIT_TEST(testSetGetBool);
  CPPUNIT_TEST(testSetGetDouble);
  CPPUNIT_TEST(testSetGetInt);
  CPPUNIT_TEST(testSetGetString);
  CPPUNIT_TEST(testSetGetStruct);
  CPPUNIT_TEST(testDataSetSerialization);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {}
  void tearDown() {}
  void testSetGetBool();
  void testSetGetDouble();
  void testSetGetInt();
  void testSetGetString();
  void testSetGetStruct();
  void testDataSetSerialization();

};

}
#endif
