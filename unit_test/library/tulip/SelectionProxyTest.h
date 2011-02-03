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
#ifndef TLPSELECTIONPROXYTEST
#define TLPSELECTIONPROXYTEST

#include <string>
#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

// Warning SelectionProxy has been renamed in BooleanAlgorithm

namespace tlp {
  class BooleanProperty;
}

class SelectionProxyTest : public CppUnit::TestFixture {
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
  static CppUnit::Test *suite();
};

#endif
