//@TLPGEOLICENCE#

#ifndef _TLP_GEO_MATRIX_H
#define _TLP_GEO_MATRIX_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include <tulip/Vector.h>
//#include <tulip/Polynome.h>
#include <vector>


namespace tlp {

  /**
   * \addtogroup basic
   */ 
#define MATRIX Matrix<Obj,SIZE>

  /*@{*/
  /**
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
    class TLP_SCOPE Matrix:public Vector< Vector<Obj,SIZE> , SIZE > {
    public:
      Matrix(){}
      Matrix(const Vector< Vector<Obj,SIZE> , SIZE > &a):Vector< Vector<Obj,SIZE> , SIZE >(a){};

      // Builds a correlation matrix from a covariance matrix !
      Matrix(const std::vector<std::vector<Obj> > &covarianceMatrix);

      /**
       * Fill the matrix with the value of obj
       */
#if __GNUC__ > 3
      inline MATRIX& fill(Obj obj);
#else
      inline void fill(Obj obj);
#endif
      /**
       * Compute the determinant of the matrix,
       */
      Obj determinant() const;
      /**
       * Transpose the matrix and return "&(*this)".
       */
#if __GNUC__ > 3
      MATRIX& transpose();
#else
      void transpose();
#endif
      /**
       * Inverse the matrix and return "&(*this)"
       */
      MATRIX& inverse();
      /**
       * Multiply the matrix by another matrix and return "&(*this)"
       */
#if __GNUC__ > 3
      inline MATRIX & operator*=(const MATRIX &mat);
#else
      inline void operator*=(const MATRIX &mat);
#endif
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
       * Returns a new matrix equal to the multiplication of the matrix by 
       * another matrix"
       */
      MATRIX operator*(const MATRIX &mat2) const;
      /**
       * Returns a new matrix equal to the division of the matrix by 
       * another matrix"
       */
      MATRIX operator/(const MATRIX &mat2) const;
      /**
       * Returns a new matrix equal to the multiplication of the matrix by 
       * obj"
       */
      MATRIX operator*(const Obj &obj) const;
      /**
       * Returns a new matrix equal to the division of the matrix by 
       * obj"
       */
      MATRIX operator/(const Obj &obj) const;
      /**
       * Returns a new vector equal to the multiplication of the matrix by 
       * a vector"
       */
      inline Vector<Obj,SIZE> operator*(const Vector<Obj,SIZE> &vec) const;

      /**
       * Returns a new vector equal to the most influent eigenvector of the
       * matrix
       */
      inline Vector<Obj,SIZE> powerIteration(const int nIterations) const;

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
    /*@}*/

}

#include "./cxx/Matrix.cxx"
#endif
