//@TLPGEOLICENCE#

#ifndef _TLP_GEO_MATRIX_H
#define _TLP_GEO_MATRIX_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include <tulip/Vector.h>

#define MATRIX Matrix<Obj,SIZE>


namespace tlp {
  /**
   * \addtogroup basic
   */ 
  /*@{*/
  /**
     * \brief class for mathematical square matrix
     *
     * Enables to create a Square Matrix of Obj with a
     * Fixed size and provide Mathematical operation. Mathematical 
     * operators must be defined for Obj. 
     *
     * Author : <a href="mailto:auber@tulip-software.org>David Auber</A>
     * Version 0.0.1 24/01/2003 
     */
    template<typename Obj,unsigned int SIZE>
    class TLP_SCOPE Matrix:public Vector< Vector<Obj,SIZE> , SIZE > {
    public:
      Matrix(){}
      Matrix(const Vector< Vector<Obj,SIZE> , SIZE > &a):Vector< Vector<Obj,SIZE> , SIZE >(a){};
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
       * Return the cofactor Matrix of this
       */
      MATRIX cofactor() const;
      /**
       * Return a new matrix equal to the multiplication of the matrix by 
       * another matrix"
       */
      MATRIX operator*(const MATRIX &mat2) const;
      /**
       * Return a new matrix equal to the division of the matrix by 
       * another matrix"
       */
      MATRIX operator/(const MATRIX &mat2) const;
      /**
       * Return a new matrix equal to the multiplication of the matrix by 
       * obj"
       */
      MATRIX operator*(const Obj &obj) const;
      /**
       * Return a new matrix equal to the division of the matrix by 
       * obj"
       */
      MATRIX operator/(const Obj &obj) const;
      /**
       * Return a new vector equal to the multiplication of the matrix by 
       * a vector"
       */
      inline Vector<Obj,SIZE> operator*(const Vector<Obj,SIZE> &vec) const;
    };
    /*@}*/
}

#include "./cxx/Matrix.cxx"
#endif
