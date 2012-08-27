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

#include <cstring>

#define VECTORTLP tlp::Vector<TYPE,SIZE>

//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator*=(const TYPE scalaire) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i]*=scalaire;

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator*=(const VECTORTLP &vecto) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i]*=vecto[i];

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator/=(const TYPE scalaire) {
  assert(scalaire!=static_cast<TYPE>(0));

  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i]/=scalaire;

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator/=(const VECTORTLP &vecto) {
  for (unsigned int i=0; i<SIZE; ++i) {
    assert(vecto[i]!=static_cast<TYPE>(0));
    (*this)[i]/=vecto[i];
  }

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator+=(const TYPE scalaire) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i]+=scalaire;

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator+=(const VECTORTLP &vecto) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i]+=vecto[i];

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator-=(const TYPE scalaire) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i]-=scalaire;

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator-=(const VECTORTLP &vecto) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i]-=vecto[i];

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator^=(const VECTORTLP &v) {
  (*this) = (*this) ^ v;
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator+(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u)+=v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator+(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u)+=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) -= v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) -= scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator*(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u)*=v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP  tlp::operator*(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u)*=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP  tlp::operator*(const TYPE scalaire, const VECTORTLP &u ) {
  return VECTORTLP(u)*=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator/(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u)/=v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator/(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u)/=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator^(const VECTORTLP &u, const VECTORTLP &v) {

  switch(SIZE) {
  case 3:
    return VECTORTLP(
             static_cast<TYPE>(
               static_cast<long double>(u.y()) * static_cast<long double>(v.z())
               -
               static_cast<long double>(u.z()) * static_cast<long double>(v.y())
             )
             ,
             static_cast<TYPE>(
               static_cast<long double>(u.z()) * static_cast<long double>(v.x())
               -
               static_cast<long double>(u.x()) * static_cast<long double>(v.z())
             )
             ,
             static_cast<TYPE>(
               static_cast<long double>(u.x()) * static_cast<long double>(v.y())
               -
               static_cast<long double>(u.y()) * static_cast<long double>(v.x())
             )
           );
    break;

  default :
    std::cerr << "cross product not implemented for dimension :" << SIZE;
    VECTORTLP result;
    return result;
    break;
  }
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u) {
  return VECTORTLP(u) *= static_cast<TYPE>(-1);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator>(const VECTORTLP &vecto) const {
  return vecto < (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator<(const VECTORTLP &v) const {
  if (std::numeric_limits<TYPE>::is_integer) {
    return memcmp(&((*this).array[0]), (void*)&(v.array[0]), SIZE * sizeof(TYPE)) < 0;
  }

  for (unsigned int i=0; i<SIZE; ++i) {
    long double tmp = static_cast<long double>((*this)[i]) - static_cast<long double>(v[i]);

    if (tmp > sqrt(std::numeric_limits<TYPE>::epsilon()) || tmp < -sqrt(std::numeric_limits<TYPE>::epsilon())) {
      if (tmp > 0) return false;

      if (tmp < 0) return true;
    }
  }

  return false;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator!=(const VECTORTLP &vecto) const {
  return (! ( (*this) == vecto ));
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator==(const VECTORTLP &v) const {
  if (std::numeric_limits<TYPE>::is_integer) {
    return memcmp((void*)&((*this).array[0]), (void*)&(v.array[0]), SIZE * sizeof(TYPE)) == 0;
  }

  for (unsigned int i=0; i<SIZE; ++i) {
    long double tmp = static_cast<long double>((*this)[i]) - static_cast<long double>(v[i]);

    if (tmp > sqrt(std::numeric_limits<TYPE>::epsilon()) || tmp < -sqrt(std::numeric_limits<TYPE>::epsilon())) {
      return false;
    }
  }

  return true;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
TYPE VECTORTLP::dotProduct(const VECTORTLP &v) const {
  assert (SIZE>0);
  long double tmpO = static_cast<long double>(VECTORTLP::array[0]) * static_cast<long double>(v[0]);

  for (unsigned int i=1; i<SIZE; ++i)
    tmpO += static_cast<long double>((*this)[i]) * static_cast<long double>(v[i]);

  return static_cast<TYPE>(tmpO);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::fill(const TYPE scalaire) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i]=scalaire;

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
TYPE  VECTORTLP::norm() const {
  switch(SIZE) {
  case 1:
    return VECTORTLP::array[0];

  case 2:
    return tlpsqrt<TYPE>(tlpsqr<TYPE>(x())
                         +
                         tlpsqr<TYPE>(y())
                        );

  case 3:
    return tlpsqrt<TYPE>(tlpsqr<TYPE>(x())
                         +
                         tlpsqr<TYPE>(y())
                         +
                         tlpsqr<TYPE>(z())
                        );

  default :
    long double tmp = tlpsqr<TYPE>((*this)[0]);

    for (unsigned int i=1; i<SIZE; ++i)
      tmp += tlpsqr<TYPE>((*this)[i]);

    return(tlpsqrt<TYPE>(tmp));
  }
}
//======================================================
template <typename TYPE,unsigned int SIZE>
TYPE  VECTORTLP::dist(const VECTOR &c) const {
  switch(SIZE) {
  case 1:
    return static_cast<TYPE>(fabs(x()-c.x()));

  case 2:
    return tlpsqrt<TYPE>(tlpsqr<TYPE>(x()-c.x())
                         +
                         tlpsqr<TYPE>(y()-c.y()));

  case 3:
    return tlpsqrt<TYPE>(tlpsqr<TYPE>(x()-c.x())
                         +
                         tlpsqr<TYPE>(y()-c.y())
                         +
                         tlpsqr<TYPE>(z()-c.z())
                        );

  default :
    long double tmp = tlpsqr<TYPE>((*this)[0] - c[0]);

    for (unsigned int i=1; i<SIZE; ++i)
      tmp += tlpsqr<TYPE>((*this)[i]-c[i]);

    return(tlpsqrt<TYPE>(tmp));
  }
}
//=======================================================================
