/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef ITERATORTEST_H
#define ITERATORTEST_H

#include <string>

#include "CppUnitIncludes.h"

namespace tlp {
class Graph;
class DoubleProperty;
} // namespace tlp

class IteratorTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(IteratorTest);
  CPPUNIT_TEST(testIterator);
  CPPUNIT_TEST(testStableIterator);
  CPPUNIT_TEST(testSortIterator);
  CPPUNIT_TEST(testFilterIterator);
  CPPUNIT_TEST(testUniqueIterator);
  CPPUNIT_TEST(testConversionIterator);
  CPPUNIT_TEST(testConcatIterator);
  CPPUNIT_TEST(testIteratorCount);
  CPPUNIT_TEST(testIteratorMap);
  CPPUNIT_TEST(testIteratorReduce);
  CPPUNIT_TEST(testIteratorCountCheck);
  CPPUNIT_TEST(testIteratorToStlContainers);
  CPPUNIT_TEST_SUITE_END();

private:
  tlp::Graph *graph;
  tlp::DoubleProperty *idMetric;

public:
  void setUp();
  void tearDown();
  void testIterator();
  void testStableIterator();
  void testSortIterator();
  void testFilterIterator();
  void testUniqueIterator();
  void testConversionIterator();
  void testConcatIterator();
  void testIteratorCount();
  void testIteratorMap();
  void testIteratorReduce();
  void testIteratorCountCheck();
  void testIteratorToStlContainers();
};

#endif // ITERATORTEST_H
