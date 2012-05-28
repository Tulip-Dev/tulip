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

#include <cassert>
#include <tulip/Array.h>
#include <tulip/tuliphash.h>

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
  * @brief typedef for 2D vector of unsigned int
  */
typedef Vector<unsigned int, 2> Vec2ui;
/**
  * @brief typedef for 3D vector of unsigned int
  */
typedef Vector<unsigned int, 3> Vec3ui;
/**
  * @brief typedef for 4D vector of unsigned int
  */
typedef Vector<unsigned int, 4> Vec4ui;
/**
  * @brief typedef for 2D vector of int
  */
typedef Vector<int, 2> Vec2i;
/**
  * @brief typedef for 3D vector of int
  */
typedef Vector<int, 3> Vec3i;
/**
  * @brief typedef for 4D vector of int
  */
typedef Vector<int, 4> Vec4i;
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
template struct TLP_SCOPE Array<float, 1>;
template struct TLP_SCOPE Array<float, 2>;
template struct TLP_SCOPE Array<float, 3>;
template struct TLP_SCOPE Array<float, 4>;
template struct TLP_SCOPE Array<float, 5>;
template class TLP_SCOPE Vector<float, 1>;
template class TLP_SCOPE Vector<float, 2>;
template class TLP_SCOPE Vector<float, 3>;
template class TLP_SCOPE Vector<float, 4>;
template class TLP_SCOPE Vector<float, 5>;

template struct TLP_SCOPE Array<int, 1>;
template struct TLP_SCOPE Array<int, 2>;
template struct TLP_SCOPE Array<int, 3>;
template struct TLP_SCOPE Array<int, 4>;
template struct TLP_SCOPE Array<int, 5>;
template class TLP_SCOPE Vector<int, 1>;
template class TLP_SCOPE Vector<int, 2>;
template class TLP_SCOPE Vector<int, 3>;
template class TLP_SCOPE Vector<int, 4>;
template class TLP_SCOPE Vector<int, 5>;

template struct TLP_SCOPE Array<bool, 1>;
template struct TLP_SCOPE Array<bool, 2>;
template struct TLP_SCOPE Array<bool, 3>;
template struct TLP_SCOPE Array<bool, 4>;
template struct TLP_SCOPE Array<bool, 5>;

template struct TLP_SCOPE Array<double, 1>;
template struct TLP_SCOPE Array<double, 2>;
template struct TLP_SCOPE Array<double, 3>;
template struct TLP_SCOPE Array<double, 4>;
template struct TLP_SCOPE Array<double, 5>;
template class TLP_SCOPE Vector<double, 1>;
template class TLP_SCOPE Vector<double, 2>;
template class TLP_SCOPE Vector<double, 3>;
template class TLP_SCOPE Vector<double, 4>;
template class TLP_SCOPE Vector<double, 5>;
template class TLP_SCOPE Vector<long double, 1>;
template class TLP_SCOPE Vector<long double, 2>;
template class TLP_SCOPE Vector<long double, 3>;


template struct TLP_SCOPE Array<tlp::Vector<double,1>,1>;
template struct TLP_SCOPE Array<tlp::Vector<double,2>,2>;
template struct TLP_SCOPE Array<tlp::Vector<double,3>,3>;
template struct TLP_SCOPE Array<tlp::Vector<double,4>,4>;

template struct TLP_SCOPE Array<tlp::Vector<long double, 4>,4>;
template struct TLP_SCOPE Array<tlp::Vector<long double, 3>,3>;
template struct TLP_SCOPE Array<tlp::Vector<long double, 2>,2>;
template struct TLP_SCOPE Array<tlp::Vector<long double, 1>,1>;



template struct TLP_SCOPE Array<tlp::Vector<float,1>,1>;
template struct TLP_SCOPE Array<tlp::Vector<float,2>,2>;
template struct TLP_SCOPE Array<tlp::Vector<float,3>,3>;
template struct TLP_SCOPE Array<tlp::Vector<float,4>,4>;

template struct TLP_SCOPE Array<tlp::Vector<int,2>,2>;

template class TLP_SCOPE tlp::Vector<unsigned char ,4>;
#endif
}

#ifdef _MSC_VER
//template<unsigned int SIZE>
static double sqrt(tlp::Vector<float, 5>& v) {
  return sqrt((double)v[0]);
}
#endif

TLP_BEGIN_HASH_NAMESPACE {
  TEMPLATEVECTOR
  size_t hash_vector(const tlp::VECTOR &v) {
    size_t seed = 0;

    for (unsigned int i = 0 ; i < SIZE ; ++i) {
      hash_combine(seed, v[i]);
    }

    return seed;
  }

  template <>
  struct hash<tlp::Vec2ui> {
    inline std::size_t operator()(const tlp::Vec2ui &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec3ui> {
    inline std::size_t operator()(const tlp::Vec3ui &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec4ui> {
    inline std::size_t operator()(const tlp::Vec4ui &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec2i> {
    inline std::size_t operator()(const tlp::Vec2i &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec3i> {
    inline std::size_t operator()(const tlp::Vec3i &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec4i> {
    inline std::size_t operator()(const tlp::Vec4i &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec2d> {
    inline std::size_t operator()(const tlp::Vec2d &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec3d> {
    inline std::size_t operator()(const tlp::Vec3f &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec4d> {
    inline std::size_t operator()(const tlp::Vec4d &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec2f> {
    inline std::size_t operator()(const tlp::Vec2f &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec3f> {
    inline std::size_t operator()(const tlp::Vec3f &v) const {
      return hash_vector(v);
    }
  };

  template <>
  struct hash<tlp::Vec4f> {
    inline std::size_t operator()(const tlp::Vec4f &v) const {
      return hash_vector(v);
    }
  };

} TLP_END_HASH_NAMESPACE

#include "cxx/Vector.cxx"
#endif
