/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <cstring>

#define VECTORTLP tlp::Vector<TYPE, SIZE, OTYPE, DTYPE>

//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator*=(const TYPE scalaire) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] *= scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator*=(const VECTORTLP &vecto) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] *= vecto[i];

  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator/=(const TYPE scalaire) {
  assert(scalaire != static_cast<TYPE>(0));

  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] /= scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator/=(const VECTORTLP &vecto) {
  for (size_t i = 0; i < SIZE; ++i) {
    assert(vecto[i] != static_cast<TYPE>(0));
    (*this)[i] /= vecto[i];
  }

  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator+=(const TYPE scalaire) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] += scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator+=(const VECTORTLP &vecto) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] += vecto[i];

  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator-=(const TYPE scalaire) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] -= scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator-=(const VECTORTLP &vecto) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] -= vecto[i];

  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::operator^=(const VECTORTLP &v) {
  (*this) = (*this) ^ v;
  return (*this);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator+(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) += v;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator+(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) += scalaire;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator-(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) -= v;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator-(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) -= scalaire;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator*(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) *= v;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator*(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) *= scalaire;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator*(const TYPE scalaire, const VECTORTLP &u) {
  return VECTORTLP(u) *= scalaire;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator/(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) /= v;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator/(const VECTORTLP &u, const TYPE scalaire) {
  return VECTORTLP(u) /= scalaire;
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator^(const VECTORTLP &u, const VECTORTLP &v) {

  switch (SIZE) {
  case 3:
    return VECTORTLP(static_cast<TYPE>(static_cast<OTYPE>(u.y()) * static_cast<OTYPE>(v.z()) -
                                       static_cast<OTYPE>(u.z()) * static_cast<OTYPE>(v.y())),
                     static_cast<TYPE>(static_cast<OTYPE>(u.z()) * static_cast<OTYPE>(v.x()) -
                                       static_cast<OTYPE>(u.x()) * static_cast<OTYPE>(v.z())),
                     static_cast<TYPE>(static_cast<OTYPE>(u.x()) * static_cast<OTYPE>(v.y()) -
                                       static_cast<OTYPE>(u.y()) * static_cast<OTYPE>(v.x())));
    break;

  default:
    tlp::warning() << "cross product not implemented for dimension :" << SIZE << std::endl;
    VECTORTLP result;
    return result;
    break;
  }
}
//======================================================
TEMPLATEVECTOR
VECTORTLP tlp::operator-(const VECTORTLP &u) {
  return VECTORTLP(u) *= static_cast<TYPE>(-1);
}
//======================================================
TEMPLATEVECTOR
bool VECTORTLP::operator>(const VECTORTLP &vecto) const {
  return vecto < (*this);
}
//======================================================
TEMPLATEVECTOR
bool VECTORTLP::operator<(const VECTORTLP &v) const {
  if (std::numeric_limits<TYPE>::is_integer) {
    return memcmp(this->data(), v.data(), SIZE * sizeof(TYPE)) < 0;
  }

  for (size_t i = 0; i < SIZE; ++i) {
    OTYPE tmp = static_cast<OTYPE>((*this)[i]) - static_cast<OTYPE>(v[i]);

    if (tmp > sqrt(std::numeric_limits<TYPE>::epsilon()) ||
        tmp < -sqrt(std::numeric_limits<TYPE>::epsilon())) {
      if (tmp > 0)
        return false;

      if (tmp < 0)
        return true;
    }
  }

  return false;
}
//======================================================
TEMPLATEVECTOR
bool VECTORTLP::operator!=(const VECTORTLP &vecto) const {
  return (!((*this) == vecto));
}
//======================================================
TEMPLATEVECTOR
bool VECTORTLP::operator==(const VECTORTLP &v) const {
  if (std::numeric_limits<TYPE>::is_integer) {
    return memcmp(this->data(), v.data(), SIZE * sizeof(TYPE)) == 0;
  }

  for (size_t i = 0; i < SIZE; ++i) {
    OTYPE tmp = static_cast<OTYPE>((*this)[i]) - static_cast<OTYPE>(v[i]);

    if (tmp > sqrt(std::numeric_limits<TYPE>::epsilon()) ||
        tmp < -sqrt(std::numeric_limits<TYPE>::epsilon())) {
      return false;
    }
  }

  return true;
}
//======================================================
TEMPLATEVECTOR
TYPE VECTORTLP::dotProduct(const VECTORTLP &v) const {
  assert(SIZE > 0);
  OTYPE tmpO = static_cast<OTYPE>((*this)[0]) * static_cast<OTYPE>(v[0]);

  for (size_t i = 1; i < SIZE; ++i)
    tmpO += static_cast<OTYPE>((*this)[i]) * static_cast<OTYPE>(v[i]);

  return static_cast<TYPE>(tmpO);
}
//======================================================
TEMPLATEVECTOR
VECTORTLP &VECTORTLP::fill(const TYPE scalaire) {
  for (size_t i = 0; i < SIZE; ++i)
    (*this)[i] = scalaire;

  return (*this);
}
//======================================================
TEMPLATEVECTOR
TYPE VECTORTLP::norm() const {
  switch (SIZE) {
  case 1:
    return (*this)[0];

  case 2:
    return tlpsqrt<TYPE, OTYPE>(tlpsqr<TYPE, OTYPE>(x()) + tlpsqr<TYPE, OTYPE>(y()));

  case 3:
    return tlpsqrt<TYPE, OTYPE>(tlpsqr<TYPE, OTYPE>(x()) + tlpsqr<TYPE, OTYPE>(y()) +
                                tlpsqr<TYPE, OTYPE>(z()));

  default:
    OTYPE tmp = tlpsqr<TYPE, OTYPE>((*this)[0]);

    for (size_t i = 1; i < SIZE; ++i)
      tmp += tlpsqr<TYPE, OTYPE>((*this)[i]);

    return (tlpsqrt<TYPE, OTYPE>(tmp));
  }
}
//======================================================
TEMPLATEVECTOR
DTYPE VECTORTLP::dist(const VECTOR &c) const {
  switch (SIZE) {
  case 1:
    return static_cast<TYPE>(fabs(x() - c.x()));

  case 2:
    return tlpsqrt<DTYPE, OTYPE>(tlpsqr<DTYPE, OTYPE>(x() - c.x()) +
                                 tlpsqr<DTYPE, OTYPE>(y() - c.y()));

  case 3:
    return tlpsqrt<DTYPE, OTYPE>(tlpsqr<DTYPE, OTYPE>(x() - c.x()) +
                                 tlpsqr<DTYPE, OTYPE>(y() - c.y()) +
                                 tlpsqr<DTYPE, OTYPE>(z() - c.z()));

  default:
    OTYPE tmp = tlpsqr<DTYPE, OTYPE>((*this)[0] - c[0]);

    for (size_t i = 1; i < SIZE; ++i)
      tmp += tlpsqr<DTYPE, OTYPE>((*this)[i] - c[i]);

    return (tlpsqrt<DTYPE, OTYPE>(tmp));
  }
}

#undef VECTORTLP
