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
#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/Graph.h>
#include <tulip/Vector.h>

#include "CppUnitIncludes.h"

class VectorTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(VectorTest);
  CPPUNIT_TEST(testExternalOperation);
  CPPUNIT_TEST(testInternalOperation);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {}
  void tearDown() {}
  void testExternalOperation();
  void testInternalOperation();
};

#endif
