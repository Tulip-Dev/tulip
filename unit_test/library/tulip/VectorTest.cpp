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
#include <cassert>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "VectorTest.h"

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION( VectorTest );

//==========================================================
void VectorTest::testExternalOperation() {
}
//==========================================================
void VectorTest::testInternalOperation() {
  const unsigned int SIZE = 4;
  Vector<double, SIZE> vect1, vectnull;
  vectnull.fill(0.0);

  for (unsigned int i=0; i<SIZE; ++i) {
    vect1[i] = i;
    CPPUNIT_ASSERT_EQUAL(0.0, vectnull[i]);
    CPPUNIT_ASSERT_EQUAL(double(i), vect1[i]);
  }

  Vector<double, SIZE> vect2(vect1);
  CPPUNIT_ASSERT_EQUAL(vect2, vect1);
  vect1 += 3;
  CPPUNIT_ASSERT(vect1 != vect2);

  for (unsigned int i=0; i<SIZE; ++i)
    CPPUNIT_ASSERT_EQUAL(double(i + 3), vect1[i]);

  vect1 -= 3;
  CPPUNIT_ASSERT_EQUAL(vect2, vect1);
  vect1 += vect2;
  vect1 /= 2.0;
  CPPUNIT_ASSERT_EQUAL(vect2, vect1);
  vect1 -= vect2;
  CPPUNIT_ASSERT_EQUAL(vectnull, vect1);
  vect1 = vect2;
  CPPUNIT_ASSERT_EQUAL(vect2, vect1);

  for (unsigned int i=0; i<SIZE; ++i) {
    vect1[i]=i;
    vect2[i]=i;
  }

  CPPUNIT_ASSERT_EQUAL(false, vect1 < vect2);
  CPPUNIT_ASSERT_EQUAL(false, vect1 > vect2);
  CPPUNIT_ASSERT_EQUAL(vect2, vect1);
  vect2[SIZE-1]++;
  CPPUNIT_ASSERT(vect1 < vect2);
  CPPUNIT_ASSERT_EQUAL(false, vect1 > vect2);
  CPPUNIT_ASSERT(vect1 != vect2);
  vect1[0]++;
  CPPUNIT_ASSERT_EQUAL(false, vect1 < vect2);
  CPPUNIT_ASSERT(vect1 > vect2);
  CPPUNIT_ASSERT(vect1 != vect2);
}
