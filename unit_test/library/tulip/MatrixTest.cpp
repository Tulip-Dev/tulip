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
#include <iomanip>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "MatrixTest.h"

using namespace std;
using namespace tlp;

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

  CPPUNIT_ASSERT_EQUAL(mat2, result);

  result = mat2 * 2.0;

  result = result - mat2;

  CPPUNIT_ASSERT_EQUAL(mat2, result);

  result = result - mat2;

  CPPUNIT_ASSERT_EQUAL(matnull, result);

  result = mat2;

  CPPUNIT_ASSERT_EQUAL(result, mat2);

  Matrix<double, SIZE> matinv(mat2);

  matinv.inverse();

  result = mat2 * matinv;

  for (unsigned int i=0; i<SIZE; ++i)
    for (unsigned int j=0; j<SIZE; ++j) {
      double res = fabs(matid[i][j] - result[i][j]);
      bool ok = false;

      if (fabs(res) < 1.E-5) ok = true;

      CPPUNIT_ASSERT(ok);
    }

  Vector<double, SIZE> vec2, vec3;
  vec2 = matid * vec;

  CPPUNIT_ASSERT_EQUAL(vec, vec2);

  vec2 = mat2 * vec;
  vec2 = matinv * vec2;

  for (unsigned int j=0; j<SIZE; ++j) {
    double res = fabs(vec2[j] - vec[j]);
    bool ok = false;

    if (res < 1.E-5) ok = true;

    CPPUNIT_ASSERT(ok);
  }

  vec2 = mat2 * vec;
  vec3 = vec * mat2;
  bool ok = false;

  for (unsigned int j=0; j<SIZE; ++j) {
    double res = fabs(vec2[j] - vec3[j]);

    if (res > 1.E-5) ok = true;
  }

  CPPUNIT_ASSERT(ok);

  vec2 = vec * mat2;
  vec3 = vec2 * matinv;

  for (unsigned int j=0; j<SIZE; ++j) {
    bool ok = false;
    double res = fabs(vec[j] - vec3[j]);

    if (res < 1.E-5) ok = true;

    CPPUNIT_ASSERT(ok);
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

      CPPUNIT_ASSERT_EQUAL(0.0, matnull[i][j]);
      CPPUNIT_ASSERT_EQUAL((double)((i+1)*(j+SIZE)), mat1[i][j]);

      if (i==j) mat1[i][j] = 0.0;
    }

  Matrix<double, SIZE> mat2(mat1);
  CPPUNIT_ASSERT_EQUAL(mat2, mat1);
  mat1[1] += 3;
  CPPUNIT_ASSERT(mat1 != mat2);
  mat1[1] -= 3;
  CPPUNIT_ASSERT_EQUAL(mat2, mat1);
  mat1 += mat2;
  mat1 /= 2.0;
  CPPUNIT_ASSERT_EQUAL(mat2, mat1);
  mat1 *= 2.0;
  mat1 -= mat2;
  CPPUNIT_ASSERT_EQUAL(mat2, mat1);
  mat1 -= mat2;
  CPPUNIT_ASSERT_EQUAL(matnull, mat1);
  mat1 = mat2;
  CPPUNIT_ASSERT_EQUAL(mat2, mat1);
  Matrix<double, SIZE> matinv(mat1);
  matinv.inverse();
  mat1 *= matinv;

  for (unsigned int i=0; i<SIZE; ++i)
    for (unsigned int j=0; j<SIZE; ++j) {
      double res = fabs(matid[i][j] - mat1[i][j]);
      bool ok = false;

      if (res < 1.E-5) ok = true;

      CPPUNIT_ASSERT(ok);
    }

  mat1.fill(1.0);
  mat2 = mat1;
  mat1*= mat2;
  mat2*= SIZE;
  CPPUNIT_ASSERT_EQUAL(mat2, mat1);
  mat1.fill(1.0);
  mat1*= mat1;
  CPPUNIT_ASSERT_EQUAL(mat2, mat1);
}
