/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include "CppUnitIncludes.h"

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
  void setUp() override {}
  void tearDown() override {}
  void testSetGetBool();
  void testSetGetDouble();
  void testSetGetInt();
  void testSetGetString();
  void testSetGetStruct();
  void testDataSetSerialization();
};
}
#endif
