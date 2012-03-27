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
  CPPUNIT_ASSERT_EQUAL(true, inter);
  c2[0] = 10;
  inter = tlp::intersection(c1, c2, p1, p2);
  CPPUNIT_ASSERT_EQUAL(p1, p2);
  c2[0] = 10.1;
  inter = tlp::intersection(c1, c2, p1, p2);
  CPPUNIT_ASSERT_EQUAL(false, inter);

  for (int i=0; i<5000; ++i) {
    Vec2d c1,c2;
    double r1, r2;
    c1[0] = rand()%100 - 50;
    c1[1] = rand()%100 - 50;
    c2[0] = rand()%100 - 50;
    c2[1] = rand()%100 - 50;
    r1 = rand()%50;
    r2 = rand()%50;
    Circle<double> cc1(c1, r1);
    Circle<double> cc2(c2, r2);
    Vec2d p1, p2;

    if (tlp::intersection(cc1, cc2, p1, p2)) {
      double dc1a = c1.dist(p1);
      double dc1b = c1.dist(p2);
      CPPUNIT_ASSERT(fabs(cc1.radius - dc1a) < 1E-3);
      CPPUNIT_ASSERT(fabs(cc1.radius - dc1b) < 1E-3);

      double dc2a = c2.dist(p1);
      double dc2b = c2.dist(p2);
      CPPUNIT_ASSERT(fabs(cc2.radius - dc2a) < 1E-3);
      CPPUNIT_ASSERT(fabs(cc2.radius - dc2b) < 1E-3);
    }
  }

}

CppUnit::Test * CircleTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Circle" );
  suiteOfTests->addTest( new CppUnit::TestCaller<CircleTest>( "Internal functions",
                         &CircleTest::intersect ) );
  return suiteOfTests;
}
//==========================================================
