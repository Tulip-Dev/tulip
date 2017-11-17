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

#ifndef DOUBLEPROPERTYTEST_H_
#define DOUBLEPROPERTYTEST_H_

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>

#include "CppUnitIncludes.h"

class DoublePropertyTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(DoublePropertyTest);
  CPPUNIT_TEST(testAnonymousDoublePropertyMaxUpdate);
  CPPUNIT_TEST(testDoublePropertyMinUpdate);
  CPPUNIT_TEST(testDoublePropertyMaxUpdate);
  CPPUNIT_TEST(testDoublePropertyMinUpdateFromString);
  CPPUNIT_TEST(testDoublePropertyMaxUpdateFromString);
  CPPUNIT_TEST(testDoublePropertySubGraphMin);
  CPPUNIT_TEST(testDoublePropertyInfValue);
  CPPUNIT_TEST(testDoublePropertySetAllValue);
  CPPUNIT_TEST(testDoublePropertySetDefaultValue);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void testAnonymousDoublePropertyMaxUpdate();
  void testDoublePropertyMinUpdate();
  void testDoublePropertyMaxUpdate();
  void testDoublePropertyMinUpdateFromString();
  void testDoublePropertyMaxUpdateFromString();
  void testDoublePropertySubGraphMin();
  void testDoublePropertySubGraphMax();
  void testDoublePropertyInfValue();
  void testDoublePropertySetAllValue();
  void testDoublePropertySetDefaultValue();

private :
  tlp::Graph *graph;
  tlp::node n1, n2, n3, n4;
  tlp::edge e1, e2;
};

#endif /* DOUBLEPROPERTYTEST_H_ */
