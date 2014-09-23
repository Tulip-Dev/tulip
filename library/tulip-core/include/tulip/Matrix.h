/*
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
///@cond DOXYGEN_HIDDEN

//@TLPGEOLICENCE#

#ifndef _TLP_GEO_MATRIX_H
#define _TLP_GEO_MATRIX_H

#include <cassert>
#include <iostream>
#include <vector>
#include <tulip/Vector.h>


namespace tlp {

#define MATRIX tlp::Matrix<Obj,SIZE>


/**
 * @ingroup Structures
 * \brief class for mathematical square matrix
 *
 * Enables to create a Square Matrix of Obj with a
 * limited size and provides Mathematical operation. Mathematical
 * operators must be defined for Obj. Out of bound accesses
 * are only checked in debug mode.
 *
 * \author : David Auber auber@tulip-software.org
 *
 * \author Contributor : Maxime Delorme
 * \version 0.0.2 27/04/2005
 */
template<typename Obj,unsigned int SIZE>
class Matrix:public Array< Vector<Obj,SIZE> , SIZE > {
public:
  Matrix() {}
  Matrix(const Array< Vector<Obj,SIZE> , SIZE > &a) :
    Array< Vector<Obj,SIZE> , SIZE >(a) {
  }

  // Builds a correlation matrix from a covariance matrix !
  Matrix(const std::vector<std::vector<Obj> > &covarianceMatrix);

  /**
   * Fill the matrix with the value of obj
   */
  inline MATRIX& fill(Obj obj);
  /**
   * Compute the determinant of the matrix,
   */
  Obj determinant() const;
  /**
   * Transpose the matrix and return "&(*this)".
   */
  MATRIX& transpose();
  /**
   * Inverse the matrix and return "&(*this)"
   */
  MATRIX& inverse();
  /**
   * add another matrix to the current one and return "&(*this)"
   */
  inline MATRIX & operator+=(const MATRIX &mat);
  /**
   * substract another matrix from the current and return "&(*this)"
   */
  inline MATRIX & operator-=(const MATRIX &mat);
  /**
   * Check equality of two Matrices
   */
  inline bool operator==(const MATRIX &) const;
  /**
   * Check non equality of two Matrices
   */
  inline bool operator!=(const MATRIX &) const;
  /**
   * Multiply the matrix by another matrix and return "&(*this)"
   */
  inline MATRIX & operator*=(const MATRIX &mat);
  /**
   * Multiply all elements of the matrix by obj, return "&(*this)"
   */
  inline MATRIX & operator*=(const Obj &obj);
  /**
   *  Divide the matrix by another one return "&(*this)"
   */
  inline MATRIX & operator/=(const MATRIX &mat);
  /**
   * Divide all elements of the matrix by obj, return "&(*this)"
   */
  inline MATRIX & operator/=(const Obj &obj);
  /**
   * Returns the cofactor Matrix of this
   */
  MATRIX cofactor() const;
  /**
   * Returns a new matrix equal to the division of the matrix by
   * another matrix"
   */
  MATRIX operator/(const MATRIX &mat2) const;

  /**
   * Returns a new matrix equal to the division of the matrix by
   * obj"
   */
  MATRIX operator/(const Obj &obj) const;


  /**
   * Returns a new vector equal to the most influent eigenvector of the
   * matrix
   */
  inline Vector<Obj,SIZE> powerIteration(const unsigned int nIterations) const;

#ifndef DOXYGEN_NOTFOR_DEVEL
  /**
   * Simplifies a 3x3 matrix in 2x2 matrix to be used with computeEigenVector
   */
  inline bool simplify(Matrix<Obj, 2> &simplifiedMatrix) const;

  /**
   * Returns the EigenVector of the matrix corresponding to the EigenValue passed, with a base x
   *           /!\ This can only be used with a 2x2 matrix !!! /!\
   */
  inline bool computeEigenVector(const float x, Vector<Obj, 3> &eigenVector) const;
#endif // DOXYGEN_NOTFOR_DEVEL

};

typedef Matrix<float,  3> Mat3f;
typedef Matrix<double, 3> Mat3d;
typedef Matrix<float,  4> Mat4f;
typedef Matrix<double, 4> Mat4d;

/**
 * Returns a new matrix equal to the sum of 2 matrices
 */
template<typename Obj, unsigned int SIZE>
inline MATRIX operator+(const MATRIX &mat1, const MATRIX &mat2);
/**
 * Returns a new matrix equal to the difference of 2 matrices
 */
template<typename Obj, unsigned int SIZE>
inline MATRIX operator-(const MATRIX &mat1, const MATRIX &mat2);
/**
 * Returns a new matrix equal to the multiplication of the matrix by
 * obj
 */
template<typename Obj, unsigned int SIZE>
inline MATRIX operator*(const MATRIX &mat, const Obj &obj);
/**
 * Returns a new matrix equal to the multiplication of the matrix by
 * another matrix
 */
template<typename Obj, unsigned int SIZE>
inline MATRIX operator*(const MATRIX &mat1, const MATRIX &mat2);
/**
 * Returns a new vector equal to the multiplication of the vector by
 * a matrix,(the vector is automatically transposed to do the multiplication)
 */
template<typename Obj, unsigned int SIZE>
inline Vector<Obj,SIZE> operator*(const Vector<Obj,SIZE> &vec, const tlp::Matrix<Obj, SIZE> &);
/**
 * Returns a new vector equal to the multiplication of the matrix by
 * a vector
 */
template<typename Obj, unsigned int SIZE>
inline Vector<Obj,SIZE> operator*( const Matrix<Obj, SIZE> &, const Vector<Obj,SIZE> &vec);



}

#include "cxx/Matrix.cxx"
#endif
///@endcond
