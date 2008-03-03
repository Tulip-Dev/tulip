#include <cassert>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "VectorTest.h"

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
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
  const Vector<double, SIZE> vect2(vect1);
  CPPUNIT_ASSERT_EQUAL(true, vect1 == vect2);
  vect1 += 3;
  CPPUNIT_ASSERT_EQUAL(false, vect1 == vect2);
  for (unsigned int i=0; i<SIZE; ++i)
    CPPUNIT_ASSERT_EQUAL(double(i + 3), vect1[i]);
  
  vect1 -= 3;
  CPPUNIT_ASSERT_EQUAL(true, vect1 == vect2);
  vect1 += vect2;
  vect1 /= 2.0;
  CPPUNIT_ASSERT_EQUAL(true, vect1 == vect2);
  vect1 -= vect2;
  CPPUNIT_ASSERT_EQUAL(true, vect1 == vectnull);
  vect1 = vect2;
  CPPUNIT_ASSERT_EQUAL(true, vect1 == vect2);
}
//==========================================================
CppUnit::Test * VectorTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Vector" );
  suiteOfTests->addTest( new CppUnit::TestCaller<VectorTest>( "Internal functions", 
							      &VectorTest::testInternalOperation ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<VectorTest>( "External functions", 
							      &VectorTest::testExternalOperation ) );
  return suiteOfTests;
}
//==========================================================
