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
#include <cmath>
#include <limits>
#if defined(_MSC_VER)
#include <tulip/vs_compat.h>
#endif

#define VECTORTLP tlp::Vector<TYPE,SIZE>

//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP::Vector(int v) {
  for (unsigned int i=0; i<SIZE; ++i)
    (*this)[i] = static_cast<TYPE>(v);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator*=(const TYPE &scalaire) {
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
VECTORTLP & VECTORTLP::operator/=(const TYPE &scalaire) {
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
VECTORTLP & VECTORTLP::operator+=(const TYPE &scalaire) {
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
VECTORTLP & VECTORTLP::operator-=(const TYPE &scalaire) {
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
  VECTORTLP tmp(*this);

  switch(SIZE) {
  case 3:
    (*this)[0] = tmp[1]*v[2] - tmp[2]*v[1];
    (*this)[1] = tmp[2]*v[0] - tmp[0]*v[2];
    (*this)[2] = tmp[0]*v[1] - tmp[1]*v[0];
    break;

  default :
    std::cerr << "cross product not implemented for dimension :" << SIZE << std::endl;
    break;
  }

  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator+(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u)+=v;
}
//======================================================
/* @todo find why the compiler do not want that code here
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::minVector(const VECTORTLP &u, const VECTORTLP &v) {
   VECTORTLP tmp;
   for(unsigned int i = 0; i<SIZE; ++i)
       tmp[i] = std::min(u[i], v[i]);
    return tmp;
}
*/
//======================================================
/* @todo find why the compiler do not want that code here
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::maxVector(const VECTORTLP &u, VECTORTLP &v) {
   VECTORTLP tmp;
   for(unsigned int i = 0; i<SIZE; ++i)
       tmp[i] = std::max(u[i], v[i]);
    return tmp;
}
*/
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator+(const VECTORTLP &u, const TYPE& scalaire) {
  return VECTORTLP(u)+=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) -= v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u, const TYPE& scalaire) {
  return VECTORTLP(u) -= scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator*(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u)*=v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP  tlp::operator*(const VECTORTLP &u, const TYPE& scalaire) {
  return VECTORTLP(u)*=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP  tlp::operator*(const TYPE& scalaire, const VECTORTLP &u ) {
  return VECTORTLP(u)*=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator/(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u)/=v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator/(const VECTORTLP &u, const TYPE& scalaire) {
  return VECTORTLP(u)/=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator^(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) ^= v;
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
bool VECTORTLP::operator<(const VECTORTLP &vecto) const {
  if (std::numeric_limits<TYPE>::is_exact ||
      !std::numeric_limits<TYPE>::is_specialized) {
    for (unsigned int i=0; i<SIZE; ++i) {
      if ((*this)[i] > vecto[i])
        return false;
      else if ((*this)[i] < vecto[i])
        return true;
    }

    return false;
  }

  if ((*this).dist(vecto) < sqrt(std::numeric_limits<TYPE>::epsilon())) return false;

  for (unsigned int i=0; i<SIZE; ++i) {
    TYPE tmp = (*this)[i] - vecto[i];

    if (tmp > sqrt(std::numeric_limits<TYPE>::epsilon()) || tmp < -sqrt(std::numeric_limits<TYPE>::epsilon())) {
      if ( (*this)[i] > vecto[i]) return false;

      if ( (*this)[i] < vecto[i]) return true;
    }
  }

  return false;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator!=(const VECTORTLP &vecto) const {
  return (!((*this) == vecto));
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator==(const VECTORTLP &vecto) const {
  if (std::numeric_limits<TYPE>::is_exact ||
      !std::numeric_limits<TYPE>::is_specialized) {
    for (unsigned int i=0; i<SIZE; ++i)
      if ((*this)[i] != vecto[i]) return false;
  }
  else {
    if ((*this).dist(vecto) > sqrt(std::numeric_limits<TYPE>::epsilon())) return false;
  }

  return true;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
TYPE VECTORTLP::dotProduct(const VECTORTLP &v) const {
  assert (SIZE>0);
  TYPE tmpO= VECTORTLP::array[0] * v[0];

  for (unsigned int i=1; i<SIZE; ++i)
    tmpO+= (*this)[i] * v[i];

  return tmpO;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::fill(const TYPE& scalaire) {
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
    return static_cast<TYPE>(sqrt(VECTORTLP::array[0]*VECTORTLP::array[0]+VECTORTLP::array[1]*VECTORTLP::array[1]));

  case 3:
    return static_cast<TYPE>(sqrt(VECTORTLP::array[0]*VECTORTLP::array[0]+VECTORTLP::array[1]*VECTORTLP::array[1]+VECTORTLP::array[2]*VECTORTLP::array[2]));

  default :
    TYPE tmp=0;

    for (unsigned int i=0; i<SIZE; ++i)
      tmp+=(*this)[i]*(*this)[i];

    return(static_cast<TYPE>(sqrt(tmp)));
  }
}
//======================================================
template <typename TYPE,unsigned int SIZE>
TYPE  VECTORTLP::dist(const VECTOR &c) const {
  switch(SIZE) {
  case 1:
    return fabs(VECTORTLP::array[0] - c.VECTORTLP::array[0]);

  case 2:
    return static_cast<TYPE>(sqrt((VECTORTLP::array[0]-c.VECTORTLP::array[0])*(VECTORTLP::array[0]-c.VECTORTLP::array[0])+(VECTORTLP::array[1]-c.VECTORTLP::array[1])*(VECTORTLP::array[1]-c.VECTORTLP::array[1])));

  case 3:
    return static_cast<TYPE>(sqrt((VECTORTLP::array[0]-c.VECTORTLP::array[0])*(VECTORTLP::array[0]-c.VECTORTLP::array[0])+(VECTORTLP::array[1]-c.VECTORTLP::array[1])*(VECTORTLP::array[1]-c.VECTORTLP::array[1])+(VECTORTLP::array[2]-c.VECTORTLP::array[2])*(VECTORTLP::array[2]-c.VECTORTLP::array[2])));

  default :
    TYPE tmp=0;

    for (unsigned int i=0; i<SIZE; ++i)
      tmp+=((*this)[i]-c[i])*((*this)[i]-c[i]);

    return(static_cast<TYPE>(sqrt(tmp)));
  }
}
//=======================================================================
