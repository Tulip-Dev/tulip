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

#ifndef STRINGPROPERTYTEST_H
#define STRINGPROPERTYTEST_H

#include "CppUnitIncludes.h"

namespace tlp {
class Graph;
}

class StringPropertyTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(StringPropertyTest);
  CPPUNIT_TEST(simpleVectorTest);
  CPPUNIT_TEST(complexVectorTest);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph* graph;
public:
  void setUp();
  void tearDown();

  void simpleVectorTest();
  void complexVectorTest();

};

#endif // STRINGPROPERTYTEST_H
