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

#include <math.h>

template<class T>
DynamicVector<T>& DynamicVector<T>::operator=(const DynamicVector<T>& vec) {
  if (this != &vec) {
    if (size != 0)
      delete[] array;

    size = vec.size;
    array = new T[size];

    for (unsigned int i = 0; i < size; ++i) {
      array[i] = vec.array[i];
    }
  }

  return *this;
}
//=================================================================
template<class T>
T DynamicVector<T>::operator[](const unsigned int i) const {

  assert(i < size);
  return array[i];
}
//=================================================================
template<class T>
T& DynamicVector<T>::operator[](const unsigned int i) {
  assert(i < size);
  return array[i];
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator*=(const T &scalaire) {
  for (unsigned int i = 0; i < size; ++i)
    DynamicVector<T>::array[i] *= scalaire;

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator*=(const DynamicVector<T> &vecto) {
  assert(vecto.size == size);

  for (unsigned int i = 0; i < size; ++i)
    DynamicVector<T>::array[i] *= vecto[i];

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator/=(const T &scalaire) {
  assert(scalaire != 0);

  for (unsigned int i = 0; i < size; ++i)
    DynamicVector<T>::array[i] /= scalaire;

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator/=(const DynamicVector<T> &vecto) {
  assert(vecto.size == size);

  for (unsigned int i = 0; i < size; ++i) {
    assert(vecto[i] != 0);
    DynamicVector<T>::array[i] /= vecto[i];
  }

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator+=(const T &scalaire) {
  for (unsigned int i = 0; i < size; ++i)
    DynamicVector<T>::array[i] += scalaire;

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator+=(const DynamicVector<T> &vecto) {
  assert(vecto.size == size);

  for (unsigned int i = 0; i < size; ++i)
    DynamicVector<T>::array[i] += vecto[i];

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator-=(const T &scalaire) {

  for (unsigned int i = 0; i < size; ++i)
    DynamicVector<T>::array[i] -= scalaire;

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator-=(const DynamicVector<T> &vecto) {
  assert(vecto.size == size);

  for (unsigned int i = 0; i < size; ++i)
    DynamicVector<T>::array[i] -= vecto[i];

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::operator^=(const DynamicVector<T> &v) {
  assert(v.size == size);
  DynamicVector<T> tmp(*this);

  switch (size) {
  case 3:
    (*this)[0] = tmp[1] * v[2] - tmp[2] * v[1];
    (*this)[1] = tmp[2] * v[0] - tmp[0] * v[2];
    (*this)[2] = tmp[0] * v[1] - tmp[1] * v[0];
    break;

  default:
    std::cerr << "cross product not implemented for dimension :" << size
              << std::endl;
    break;
  }

  return (*this);
}
//======================================================
template<class T>
DynamicVector<T> DynamicVector<T>::operator+(const DynamicVector<T> &v) const {
  assert(v.size == size);
  return DynamicVector<T> (*this) += v;
}
//======================================================
template<class T>
DynamicVector<T> DynamicVector<T>::operator+(const T& scalaire) const {
  return DynamicVector<T> (*this) += scalaire;
}
//======================================================
template<class T>
DynamicVector<T> DynamicVector<T>::operator-(const DynamicVector<T> &v) const {
  assert(v.size == size);
  return DynamicVector<T> (*this) -= v;
}
//======================================================
template<class T>
DynamicVector<T> DynamicVector<T>::operator-(const T& scalaire) const {
  return DynamicVector<T> (*this) -= scalaire;
}
//======================================================
template<class T>
DynamicVector<T> operator*(const DynamicVector<T> &v1,
                           const DynamicVector<T> &v2) {
  assert(v1.size == v2.size);
  return DynamicVector<T> (v1) *= v2;
}
//======================================================
template<class T>
DynamicVector<T> operator*(const DynamicVector<T> &v, const T& scalaire) {
  return DynamicVector<T> (v) *= scalaire;
}
//======================================================
template<class T>
DynamicVector<T> DynamicVector<T>::operator/(const DynamicVector<T> &v) const {
  return DynamicVector<T> (*this) /= v;
}
//======================================================
template<class T>
DynamicVector<T> DynamicVector<T>::operator/(const T& scalaire) const {
  return DynamicVector<T> (*this) /= scalaire;
}
//======================================================
template<class T>
DynamicVector<T> DynamicVector<T>::operator^(const DynamicVector<T> &v) const {
  return DynamicVector<T> (*this) ^= v;
}
//======================================================
template<class T>
bool DynamicVector<T>::operator!=(const DynamicVector<T> &vecto) const {
  for (unsigned int i = 0; i < size; ++i)
    if (array[i] != vecto[i])
      return true;

  return false;
}
//======================================================
template<class T>
bool DynamicVector<T>::operator==(const DynamicVector<T> &vecto) const {
  assert(size == vecto.size);

  for (unsigned int i = 0; i < size; ++i) {
    if (array[i] != vecto[i]) {
      return false;
    }
  }

  return true;
}
//======================================================
template<class T>
T DynamicVector<T>::dotProduct(const DynamicVector<T> &v) const {
  assert(size > 0);
  T tmpO = DynamicVector<T>::array[0] * v[0];

  for (unsigned int i = 1; i < size; ++i)
    tmpO += DynamicVector<T>::array[i] * v[i];

  return tmpO;
}
//======================================================
template<class T>
DynamicVector<T> & DynamicVector<T>::fill(const T& scalaire) {
  for (unsigned int i = 0; i < size; ++i)
    DynamicVector<T>::array[i] = scalaire;

  return (*this);
}
//======================================================
template<class T>
T DynamicVector<T>::norm() const {
  switch (size) {
  case 1:
    return DynamicVector<T>::array[0];

  case 2:
    return sqrt(DynamicVector<T>::array[0] * DynamicVector<T>::array[0]
                + DynamicVector<T>::array[1] * DynamicVector<T>::array[1]);
    break;

  case 3:
    return sqrt(DynamicVector<T>::array[0] * DynamicVector<T>::array[0]
                + DynamicVector<T>::array[1] * DynamicVector<T>::array[1]
                + DynamicVector<T>::array[2] * DynamicVector<T>::array[2]);
    break;

  default:
    T tmp = 0;

    for (unsigned int i = 0; i < size; ++i)
      tmp += DynamicVector<T>::array[i] * DynamicVector<T>::array[i];

    return (sqrt(tmp));
    break;
  }
}
//======================================================
template<class T>
T DynamicVector<T>::dist(const DynamicVector<T> &c) const {
  switch (size) {
  case 1:
    return fabs(DynamicVector<T>::array[0] - c.DynamicVector<T>::array[0]);

  case 2:
    return sqrt((DynamicVector<T>::array[0] - c.DynamicVector<T>::array[0])
                * (DynamicVector<T>::array[0] - c.DynamicVector<T>::array[0])
                + (DynamicVector<T>::array[1] - c.DynamicVector<T>::array[1])
                * (DynamicVector<T>::array[1] - c.DynamicVector<T>::array[1]));
    break;

  case 3:
    return sqrt((DynamicVector<T>::array[0] - c.DynamicVector<T>::array[0])
                * (DynamicVector<T>::array[0] - c.DynamicVector<T>::array[0])
                + (DynamicVector<T>::array[1] - c.DynamicVector<T>::array[1])
                * (DynamicVector<T>::array[1] - c.DynamicVector<T>::array[1])
                + (DynamicVector<T>::array[2] - c.DynamicVector<T>::array[2])
                * (DynamicVector<T>::array[2] - c.DynamicVector<T>::array[2]));
    break;

  default:
    T tmp = 0;

    for (unsigned int i = 0; i < size; ++i)
      tmp += (DynamicVector<T>::array[i] - c.DynamicVector<T>::array[i])
             * (DynamicVector<T>::array[i] - c.DynamicVector<T>::array[i]);

    return (sqrt(tmp));
    break;
  }
}
//=======================================================================
template<class T>
std::ostream& operator<<(std::ostream &os, const DynamicVector<T> &a) {
  os << "(";

  for (unsigned int i = 0; i < a.getSize(); ++i) {
    if (i > 0)
      os << ",";

    os << a[i];
  }

  os << ")";
  return os;
}
//=================================================================
template<class T>
std::istream & operator>>(std::istream &is, DynamicVector<T> & outA) {
  char c;
  int pos = is.tellg();
  is.clear();

  if (!(is >> c) || c != '(') {
    is.seekg(pos);
    is.setstate(std::ios::failbit);
    return is;
  }

  for (unsigned int i = 0; i < outA.size; ++i) {
    if (i > 0 && (!(is >> c) || c != ',')) {
      is.seekg(pos);
      is.setstate(std::ios::failbit);
      return is;
    }

    bool done = (is >> outA.array[i]);

    if (!done) {
      is.seekg(pos);
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  if (!(is >> c) || c != ')') {
    is.seekg(pos);
    is.setstate(std::ios::failbit);
    return is;
  }

  return is;
}
