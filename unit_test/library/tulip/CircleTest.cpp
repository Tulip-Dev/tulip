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
#include <cassert>
#include <iomanip>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "CircleTest.h"
#include <tulip/Circle.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( CircleTest );

void CircleTest::intersect() {
  Circle<double> c1(0,0,5),c2(7,0,5);
  Vec2d p1, p2;
  bool inter = tlp::intersection(c1, c2, p1, p2);
  cerr << p1 << "  " << p2 << endl;
  CPPUNIT_ASSERT_EQUAL(true, inter);
  c2[0] = 10;
  inter = tlp::intersection(c1, c2, p1, p2);
  cerr << p1 << "  " << p2 << endl;
  CPPUNIT_ASSERT_EQUAL(true, p1 == p2);
  c2[0] = 10.1;
  inter = tlp::intersection(c1, c2, p1, p2);
  cerr << p1 << "  " << p2 << endl;
  CPPUNIT_ASSERT_EQUAL(false, inter);

}

CppUnit::Test * CircleTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Circle" );
  suiteOfTests->addTest( new CppUnit::TestCaller<CircleTest>( "Internal functions",
                         &CircleTest::intersect ) );
  return suiteOfTests;
}
//==========================================================
