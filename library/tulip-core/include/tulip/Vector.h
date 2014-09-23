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

#ifndef _TLP_VECTOR_H
#define _TLP_VECTOR_H

#include <cassert>
#include <tulip/Array.h>
#include <tulip/tuliphash.h>
#include <cmath>
#include <limits>
#include <cstring>

#define VECTOR Vector<TYPE,SIZE,OTYPE,DTYPE>
#define TEMPLATEVECTOR template <typename TYPE, unsigned int SIZE, typename OTYPE, typename DTYPE>

namespace tlp {

template<typename TYPE, typename OTYPE>
inline OTYPE tlpsqr(const TYPE a) {
  return static_cast<OTYPE>(a) * static_cast<OTYPE>(a);
}

template<typename TYPE, typename OTYPE>
inline TYPE tlpsqrt(const OTYPE a) {
  return static_cast<TYPE>(sqrt(a));
}

template<>
inline double tlpsqrt<double, long double>(long double a) {
  return static_cast<double>(sqrtl(a));
}


/**
 * @ingroup Structures
 * \brief class for mathematical vector
 *
 * Enables to create a Vector of TYPE (must be a numeric basic type) with a
 * fixed size and provides Mathematical operation. Mathematical
 * operators must be defined for TYPE. Out of bound accesses are only checked
 * in debug mode. The OTYPE is used for temporary computation to prevent overflow,
 * by default OTYPE is a double.
 *
 * \author : David Auber auber@tulip-software.org
 * \version 0.0.1 24/01/2003
 */
template <typename TYPE, unsigned int SIZE, typename OTYPE = double, typename DTYPE = TYPE>
class Vector:public Array<TYPE,SIZE> {
public:
  inline VECTOR() {
    memset( &((*this)[0]), 0, SIZE * sizeof(TYPE) );
  }
  inline VECTOR(const Vector<TYPE, SIZE, OTYPE> &v) {
    set(v);
  }

  inline VECTOR(const Vector<TYPE, SIZE + 1, OTYPE> &v) {
    set(v);
  }

  explicit inline VECTOR(const TYPE x) {
    fill(x);
    /*
    if (int(SIZE) - 1 > 0)
        memset( &((*this)[1]), 0, (SIZE - 1) * sizeof(TYPE) );
    set(x);
    */
  }

  explicit inline VECTOR(const TYPE x, const TYPE y) {
    if (int(SIZE) - 2 > 0)
      memset( &((*this)[2]), 0, (SIZE - 2) * sizeof(TYPE) );

    set(x,y);
  }

  explicit inline VECTOR(const TYPE x, const TYPE y, const TYPE z) {
    if (int(SIZE) - 3 > 0)
      memset( &((*this)[3]), 0, (SIZE - 3) * sizeof(TYPE) );

    set(x, y, z);
  }
  explicit inline VECTOR(const Vector<TYPE, 2, OTYPE> &v, const TYPE z) {
    set(v, z);
  }
  explicit inline VECTOR(const TYPE x, const TYPE y, const TYPE z, const TYPE w) {
    set(x, y, z, w);
  }
  explicit inline VECTOR(const Vector<TYPE, 2, OTYPE> &v, const TYPE z, const TYPE w) {
    set(v, z, w);
  }
  explicit inline VECTOR(const Vector<TYPE, 3, OTYPE> &v, const TYPE w) {
    set(v, w);
  }

  inline void set(const TYPE x) {
    (*this)[0] = x;
  }
  inline void set(const TYPE x, const TYPE y) {
    assert(SIZE>1);
    (*this)[0] = x;
    (*this)[1] = y;
  }
  inline void set(const TYPE x, const TYPE y, const TYPE z) {
    assert(SIZE>2);
    (*this)[0] = x;
    (*this)[1] = y;
    (*this)[2] = z;
  }
  inline void set(const TYPE x, const TYPE y, const TYPE z, const TYPE w) {
    assert(SIZE>3);
    (*this)[0] = x;
    (*this)[1] = y;
    (*this)[2] = z;
    (*this)[3] = w;
  }
  inline void set(const Vector<TYPE, 2, OTYPE> &v, const TYPE z) {
    assert(SIZE>2);
    memcpy( &((*this)[0]), (void*)&(v.array[0]), 2 * sizeof(TYPE) );
    (*this)[2] = z;
  }
  inline void set(const Vector<TYPE, 2, OTYPE> &v, const TYPE z, const TYPE w) {
    assert(SIZE>3);
    memcpy( &((*this)[0]), (void*)&(v.array[0]), 2 * sizeof(TYPE) );
    (*this)[2] = z;
    (*this)[3] = w;
  }
  inline void set(const Vector<TYPE, 3, OTYPE> &v, const TYPE w) {
    assert(SIZE>3);
    memcpy( &((*this)[0]), (void*)&(v.array[0]), 3 * sizeof(TYPE) );
    (*this)[3] = w;
  }
  inline void set(const Vector<TYPE, SIZE, OTYPE> &v) {
    memcpy(&((*this)[0]), (void*)&(v.array[0]), SIZE * sizeof(TYPE) );
  }
  inline void set(const Vector<TYPE, SIZE + 1, OTYPE> &v) {
    memcpy(&((*this)[0]), &(v.array[0]), SIZE * sizeof(TYPE) );
  }
  inline void get(TYPE &x) const {
    x = (*this)[0];
  }
  inline void get(TYPE &x,TYPE &y) const {
    assert(SIZE>1);
    x = (*this)[0];
    y = (*this)[1];
  }
  inline void get(TYPE &x,TYPE &y,TYPE &z) const {
    assert(SIZE>2);
    x = (*this)[0];
    y = (*this)[1];
    z = (*this)[2];
  }
  inline void get(TYPE &x,TYPE &y,TYPE &z,TYPE &w) const {
    assert(SIZE>3);
    x = (*this)[0];
    y = (*this)[1];
    z = (*this)[2];
    w = (*this)[3];
  }

  //convenient accessor for coordinates
  inline TYPE x() const {
    return (*this)[0];
  }
  inline TYPE y() const {
    assert(SIZE>1);
    return (*this)[1];
  }
  inline TYPE z() const {
    assert(SIZE>2);
    return (*this)[2];
  }
  inline TYPE w() const {
    assert(SIZE>3);
    return (*this)[3];
  }

  inline TYPE& x() {
    return (*this)[0];
  }
  inline TYPE& y() {
    assert(SIZE>1);
    return (*this)[1];
  }
  inline TYPE& z() {
    assert(SIZE>2);
    return (*this)[2];
  }
  inline TYPE& w() {
    assert(SIZE>3);
    return (*this)[3];
  }

  inline TYPE width()  const {
    return x();
  }
  inline TYPE height() const {
    return y();
  }
  inline TYPE depth()  const {
    return z();
  }

  inline TYPE& width()  {
    return x();
  }
  inline TYPE& height() {
    return y();
  }
  inline TYPE& depth()  {
    return z();
  }

  inline TYPE r() const {
    return x();
  }
  inline TYPE g() const {
    return y();
  }
  inline TYPE b() const {
    return z();
  }
  inline TYPE a() const {
    return w();
  }

  inline TYPE& r() {
    return x();
  }
  inline TYPE& g() {
    return y();
  }
  inline TYPE& b() {
    return z();
  }
  inline TYPE& a() {
    return w();
  }

  inline TYPE s() const {
    return x();
  }
  inline TYPE t() const {
    return y();
  }
  inline TYPE p() const {
    return z();
  }
  inline TYPE q() const {
    return w();
  }

  inline TYPE& s() {
    return x();
  }
  inline TYPE& t() {
    return y();
  }
  inline TYPE& p() {
    return z();
  }
  inline TYPE& q() {
    return w();
  }

  inline void setX(TYPE xx) {
    x() = xx;
  }
  inline void setY(TYPE yy) {
    y() = yy;
  }
  inline void setZ(TYPE zz) {
    z() = zz;
  }

  inline TYPE getX() const {
    return x();
  }
  inline TYPE getY() const {
    return y();
  }
  inline TYPE getZ() const {
    return z();
  }

  inline void setW(const TYPE width) {
    x() = width;
  }

  inline void setH(const TYPE height) {
    y() = height;
  }

  inline void setD(const TYPE depth) {
    z() = depth;
  }

  inline TYPE getW() const {
    return x();
  }
  inline TYPE getH() const {
    return y();
  }
  inline TYPE getD() const {
    return z();
  }



//    inline VECTOR & operator*=(const OTYPE );
  inline VECTOR & operator*=(const TYPE );
  inline VECTOR & operator*=(const VECTOR &);
//    inline VECTOR & operator/=(const OTYPE );
  inline VECTOR & operator/=(const TYPE );
  inline VECTOR & operator/=(const VECTOR &);
//    inline VECTOR & operator+=(const OTYPE );
  inline VECTOR & operator+=(const TYPE );
  inline VECTOR & operator+=(const VECTOR &);
//    inline VECTOR & operator-=(const OTYPE );
  inline VECTOR & operator-=(const TYPE );
  inline VECTOR & operator-=(const VECTOR &);
  inline VECTOR & operator^=(const VECTOR &);

  inline bool operator>(const VECTOR &) const;
  inline bool operator<(const VECTOR &) const;
  inline bool operator!=(const VECTOR &) const;
  inline bool operator==(const VECTOR &) const;
  inline VECTOR & fill(const TYPE obj);
  inline TYPE norm () const;
  inline TYPE length () const {
    return norm();
  }
  inline VECTOR & normalize () {
    OTYPE tmp = 0;

    for (unsigned int i=0; i<SIZE; ++i)
      tmp += tlpsqr<TYPE, OTYPE>((*this)[i]);

    if (tmp < sqrt(std::numeric_limits<TYPE>::epsilon())) {
      return *this;
    }

    for (unsigned int i=0; i<SIZE; ++i) {
      if ((*this)[i] < 0.)
        (*this)[i] = -tlpsqrt<TYPE, OTYPE>(tlpsqr<TYPE, OTYPE>((*this)[i]) / tmp);
      else
        (*this)[i] = tlpsqrt<TYPE, OTYPE>(tlpsqr<TYPE, OTYPE>((*this)[i]) / tmp);
    }

    return *this;
  }
  inline DTYPE dist (const VECTOR &) const;
  inline TYPE dotProduct(const VECTOR &) const;
};

TEMPLATEVECTOR
inline TYPE dotProduct(const VECTOR &a, const VECTOR &b) {
  return a.dotProduct(b);
}

TEMPLATEVECTOR
inline TYPE dist(const VECTOR &a, const VECTOR &b) {
  return a.dist(b);
}


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
inline VECTOR operator*(const TYPE  , const VECTOR &);
TEMPLATEVECTOR
inline VECTOR operator*(const VECTOR &, const TYPE );

//TEMPLATEVECTOR
//inline VECTOR operator*(const OTYPE  , const VECTOR &);
//TEMPLATEVECTOR
//inline VECTOR operator*(const VECTOR &, const OTYPE );

TEMPLATEVECTOR
inline VECTOR operator+(const VECTOR &, const VECTOR &);
TEMPLATEVECTOR
inline VECTOR operator+(const VECTOR &, const TYPE );
//TEMPLATEVECTOR
//inline VECTOR operator+(const VECTOR &, const OTYPE );

TEMPLATEVECTOR
inline VECTOR operator-(const VECTOR &, const VECTOR &);
TEMPLATEVECTOR
inline VECTOR operator-(const VECTOR &, const TYPE );
//TEMPLATEVECTOR
//inline VECTOR operator-(const VECTOR &, const OTYPE );

TEMPLATEVECTOR
inline VECTOR operator/(const VECTOR &, const VECTOR &);
TEMPLATEVECTOR
inline VECTOR operator/(const VECTOR &, const TYPE );
//TEMPLATEVECTOR
//inline VECTOR operator/(const VECTOR &, const OTYPE );

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
typedef Vector<double, 2, long double> Vec2d;
/**
  * @brief typedef for 3D vector of double
  */
typedef Vector<double, 3, long double> Vec3d;
/**
  * @brief typedef for 4D vector of double
  */
typedef Vector<double, 4, long double> Vec4d;
/**
  * @brief typedef for 2D vector of float
  */
typedef Vector<float,  2, double> Vec2f;
/**
  * @brief typedef for 3D vector of float
  */
typedef Vector<float,  3, double> Vec3f;
/**
  * @brief typedef for 4D vector of float
  */
typedef Vector<float,  4, double> Vec4f;
}

#ifdef _MSC_VER
//template<unsigned int SIZE>
static double sqrt(tlp::Vector<float, 5>& v) {
  return sqrt((double)v[0]);
}

template class TLP_SCOPE tlp::Vector<unsigned char, 4>;

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

// fix for bug #3598871: allow use of VECTOR keyword in other software
#undef VECTOR
#undef TEMPLATEVECTOR

#endif
///@endcond
