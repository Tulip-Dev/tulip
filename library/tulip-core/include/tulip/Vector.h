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
//@TLPGEOLICENCE#

#ifndef _TLP_GEO_VECTOR_H
#define _TLP_GEO_VECTOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <tulip/Array.h>

#define VECTOR Vector<TYPE,SIZE>
#define TEMPLATEVECTOR template <typename TYPE, unsigned int SIZE>

namespace tlp {
  /**
   * \addtogroup basic
   */ 

  /*@{*/
  /**
   * \brief class for mathematical vector
   *
   * Enables to create a Vector of TYPE (must be a numeric basic type) with a
   * fixed size and provides Mathematical operation. Mathematical 
   * operators must be defined for TYPE. Out of bound accesses are only checked
   * in debug mode.
   *
   * \author : David Auber auber@tulip-software.org
   * \version 0.0.1 24/01/2003 
   */
  TEMPLATEVECTOR
    class TLP_SCOPE Vector:public Array<TYPE,SIZE> {
    public:

    explicit inline VECTOR(int v = 0);      
    inline VECTOR & operator*=(const TYPE &);
    inline VECTOR & operator*=(const VECTOR &);
    inline VECTOR & operator/=(const TYPE &);
    inline VECTOR & operator/=(const VECTOR &);
    inline VECTOR & operator+=(const TYPE &);
    inline VECTOR & operator+=(const VECTOR &);
    inline VECTOR & operator-=(const TYPE &);
    inline VECTOR & operator-=(const VECTOR &);
    inline VECTOR & operator^=(const VECTOR &);
    
    inline bool operator>(const VECTOR &) const;
    inline bool operator<(const VECTOR &) const;
    inline bool operator!=(const VECTOR &) const;
    inline bool operator==(const VECTOR &) const;
    inline VECTOR & fill(const TYPE &obj);
    inline TYPE norm () const;
    inline TYPE dist (const VECTOR &) const;
    inline TYPE dotProduct(const VECTOR &) const;
  };

  /**
    * Return the minimun of each dimension of the two vectors
    * for instance for a 2 vectors of dim 2 :
    * min(V1, V2) = (min(V1[0], v2[0]), min(V1[1], v2[1))
    */
  TEMPLATEVECTOR
   inline VECTOR minVector(const VECTOR &u, const VECTOR &v) {
      VECTOR tmp;
      for(unsigned int i = 0; i<SIZE; ++i)
          tmp[i] = std::min(u[i], v[i]);
       return tmp;
   }
  /**
    * Return the maximum of each dimension of the two vectors
    * for instance for a 2 vectors of dim 2 :
    * max(V1, V2) = (max(V1[0], v2[0]), max(V1[1], v2[1))
    */
  TEMPLATEVECTOR
   inline VECTOR maxVector(const VECTOR &u, const VECTOR &v)  {
      VECTOR tmp;
      for(unsigned int i = 0; i<SIZE; ++i)
          tmp[i] = std::max(u[i], v[i]);
       return tmp;
   }

  TEMPLATEVECTOR
    inline VECTOR operator*(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator*(const TYPE & , const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator*(const VECTOR &, const TYPE &);
  TEMPLATEVECTOR
    inline VECTOR operator+(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator+(const VECTOR &, const TYPE &);
  TEMPLATEVECTOR
    inline VECTOR operator-(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator-(const VECTOR &, const TYPE &);
  TEMPLATEVECTOR
    inline VECTOR operator/(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator/(const VECTOR &, const TYPE &);
  TEMPLATEVECTOR
    inline VECTOR operator^(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator-(const VECTOR&);
    /**
      * @brief typedef for 2D vector of double
      */
  typedef Vector<double, 2> Vec2d;
  /**
    * @brief typedef for 3D vector of double
    */
  typedef Vector<double, 3> Vec3d;
  /**
    * @brief typedef for 4D vector of double
    */
  typedef Vector<double, 4> Vec4d;
  /**
    * @brief typedef for 2D vector of float
    */
  typedef Vector<float,  2> Vec2f;
  /**
    * @brief typedef for 3D vector of float
    */
  typedef Vector<float,  3> Vec3f;
  /**
    * @brief typedef for 4D vector of float
    */
  typedef Vector<float,  4> Vec4f;
  /*@}*/

#ifdef _MSC_VER
  template struct Array<float, 1>;
  template struct Array<float, 2>;
  template struct Array<float, 3>;
  template struct Array<float, 4>;
  template struct Array<float, 5>;
  template class Vector<float, 1>;
  template class Vector<float, 2>;
  template class Vector<float, 3>;
  template class Vector<float, 4>;
  template class Vector<float, 5>;

  template struct Array<int, 1>;
  template struct Array<int, 2>;
  template struct Array<int, 3>;
  template struct Array<int, 4>;
  template struct Array<int, 5>;
  template class Vector<int, 1>;
  template class Vector<int, 2>;
  template class Vector<int, 3>;
  template class Vector<int, 4>;
  template class Vector<int, 5>;
  
  template struct Array<bool, 1>;
  template struct Array<bool, 2>;
  template struct Array<bool, 3>;
  template struct Array<bool, 4>;
  template struct Array<bool, 5>;
  
  template struct Array<double, 1>;
  template struct Array<double, 2>;
  template struct Array<double, 3>;
  template struct Array<double, 4>;
  template struct Array<double, 5>;
  template class Vector<double, 1>;
  template class Vector<double, 2>;
  template class Vector<double, 3>;
  template class Vector<double, 4>;
  template class Vector<double, 5>;

  template struct Array<tlp::Vector<double,1>,1>;
  template struct Array<tlp::Vector<double,2>,2>;
  template struct Array<tlp::Vector<double,3>,3>;
  template struct Array<tlp::Vector<double,4>,4>;

  template struct Array<tlp::Vector<float,1>,1>;
  template struct Array<tlp::Vector<float,2>,2>;
  template struct Array<tlp::Vector<float,3>,3>;
  template struct Array<tlp::Vector<float,4>,4>;
  
  template struct Array<tlp::Vector<int,2>,2>;

  template class tlp::Vector<unsigned char ,4>;
#endif
}

#ifdef _MSC_VER
  //template<unsigned int SIZE>
  static double sqrt(tlp::Vector<float, 5>& v) {
	  return sqrt((double)v[0]);
  }
#endif

#include "cxx/Vector.cxx"
#endif
