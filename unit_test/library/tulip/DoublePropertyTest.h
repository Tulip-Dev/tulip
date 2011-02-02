/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class DoublePropertyTest : public CppUnit::TestFixture {
 public:

  void setUp();
  void tearDown();
  void testDoublePropertyMinUpdate();
  void testDoublePropertyMaxUpdate();
  void testDoublePropertyMinUpdateFromString();
  void testDoublePropertyMaxUpdateFromString();
  void testDoublePropertySubGraphMin();
  void testDoublePropertySubGraphMax();

  static CppUnit::Test *suite();

 private :

  tlp::Graph *graph;
  tlp::node n1, n4;
};

#endif /* DOUBLEPROPERTYTEST_H_ */
