#include <cassert>
#include <iomanip>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "MatrixTest.h"

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( MatrixTest );

template<typename Obj, unsigned int SIZE>
void display(const Matrix<Obj, SIZE> &mat) {
  for (unsigned int row=0; row<SIZE; ++row) {
    cout << "| ";
    for (unsigned int col=0; col<SIZE; ++col) {
      cout << setw(4) << mat[row][col] << setw(1) << " ";
    }
    cout << "|" << endl;
  }
}

//==========================================================
void MatrixTest::testExternalOperation() {
  const unsigned int SIZE = 4;
  Matrix<double, SIZE> mat1, matid, matnull;
  matnull.fill(0);
  matid.fill(0);
  Vector<double, SIZE> vec;
  for (unsigned int i=0; i<SIZE; ++i) {
    vec[i] = i;
    for (unsigned int j=0; j<SIZE; ++j) {
      mat1[i][j] = double((i+1)*(j+SIZE));
      if (i==j) matid[i][j] = 1.0;
      if (i==j) mat1[i][j] = 0.0; //ensure be inversible
    }
  }
  
  const Matrix<double, SIZE> mat2(mat1);
  Matrix<double, SIZE> result;
  result = mat2;
  result = mat2 + result;
  result = result / 2.0;
  CPPUNIT_ASSERT_EQUAL(true, result == mat2);
  result = mat2 * 2.0;
  result = result - mat2;
  CPPUNIT_ASSERT_EQUAL(true, result == mat2);
  result = result - mat2;
  CPPUNIT_ASSERT_EQUAL(true, result == matnull);
  result = mat2;
  CPPUNIT_ASSERT_EQUAL(true, result == mat2);
  Matrix<double, SIZE> matinv(mat2);
  matinv.inverse();
  result = mat2 * matinv;
  for (unsigned int i=0; i<SIZE; ++i) 
    for (unsigned int j=0; j<SIZE; ++j) {
      double res = fabs(matid[i][j] - result[i][j]);
      bool ok = false;
      if (res < 1.E-5) ok = true;
      CPPUNIT_ASSERT_EQUAL(true, ok);
    }
  Vector<double, SIZE> vec2;
  vec2 = matid *vec;
  CPPUNIT_ASSERT_EQUAL(true, vec == vec2);
  //  cerr << vec << endl;
  //  display(mat2);
  vec2 = mat2 * vec;
  //  cerr << vec2 << endl;
  //  display(matinv);
  vec2 = matinv * vec2;
  //  cerr << vec2 << endl;
  for (unsigned int j=0; j<SIZE; ++j) {
    double res = fabs(vec2[j] - vec[j]);
    bool ok = false;
    if (res < 1.E-5) ok = true;
    CPPUNIT_ASSERT_EQUAL(true, ok);
  }

}
//==========================================================
void MatrixTest::testInternalOperation() {
  const unsigned int SIZE = 4;
  Matrix<double, SIZE> mat1, matnull, matid;
  matnull.fill(0);
  matid.fill(0);
  for (unsigned int i=0; i<SIZE; ++i) 
    for (unsigned int j=0; j<SIZE; ++j) {
      mat1[i][j] = double((i+1)*(j+SIZE));
      if (i==j) matid[i][j] = 1.0;
      CPPUNIT_ASSERT_EQUAL((double)0.0, matnull[i][j]);
      CPPUNIT_ASSERT_EQUAL((double)((i+1)*(j+SIZE)), mat1[i][j]);
      if (i==j) mat1[i][j] = 0.0;
    }
  Matrix<double, SIZE> mat2(mat1);
  CPPUNIT_ASSERT_EQUAL(true, mat1 == mat2);
  mat1[1] += 3;
  CPPUNIT_ASSERT_EQUAL(false, mat1 == mat2);
  mat1[1] -= 3;
  CPPUNIT_ASSERT_EQUAL(true, mat1 == mat2);
  mat1 += mat2;
  mat1 /= 2.0;
  CPPUNIT_ASSERT_EQUAL(true, mat1 == mat2);
  mat1 *= 2.0;
  mat1 -= mat2;
  CPPUNIT_ASSERT_EQUAL(true, mat1 == mat2);
  mat1 -= mat2;
  CPPUNIT_ASSERT_EQUAL(true, mat1 == matnull);
  mat1 = mat2;
  CPPUNIT_ASSERT_EQUAL(true, mat1 == mat2);
  Matrix<double, SIZE> matinv(mat1);
  matinv.inverse();
  mat1 *= matinv;
  for (unsigned int i=0; i<SIZE; ++i) 
    for (unsigned int j=0; j<SIZE; ++j) {
      double res = fabs(matid[i][j] - mat1[i][j]);
      bool ok = false;
      if (res < 1.E-5) ok = true;
      CPPUNIT_ASSERT_EQUAL(true, ok);
    }
  mat1.fill(1.0);
  mat2 = mat1;
  mat1*= mat2;
  mat2*= SIZE;
  CPPUNIT_ASSERT_EQUAL(true, mat1 == mat2);
  mat1.fill(1.0);
  mat1*= mat1;
  CPPUNIT_ASSERT_EQUAL(true, mat1 == mat2);
}
//==========================================================
CppUnit::Test * MatrixTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Matrix" );
  suiteOfTests->addTest( new CppUnit::TestCaller<MatrixTest>( "Internal functions", 
							      &MatrixTest::testInternalOperation ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<MatrixTest>( "External functions", 
							      &MatrixTest::testExternalOperation ) );
  return suiteOfTests;
}
//==========================================================
