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

#ifndef DYNAMICVECTOR_H_
#define DYNAMICVECTOR_H_
template<class T>
class DynamicVector {
public:
  DynamicVector() :
    array(NULL), size(0) {
  }

  DynamicVector(const DynamicVector<T>& vect) {
    array = new T[vect.size];
    size = vect.size;

    for (unsigned int i = 0; i < vect.size; ++i) {
      array[i] = vect.array[i];
    }
  }

  DynamicVector(unsigned int size) :
    size(size) {
    array = new T[size];
  }
  virtual ~DynamicVector() {
    if (size != 0)
      delete[] array;
  }

  inline DynamicVector<T>& operator=(const DynamicVector<T>&);

  inline T operator[](const unsigned int i) const;
  inline T& operator[](const unsigned int i);

  inline DynamicVector<T> & operator*=(const T &);
  inline DynamicVector<T> & operator*=(const DynamicVector<T> &);
  inline DynamicVector<T> & operator/=(const T &);
  inline DynamicVector<T> & operator/=(const DynamicVector<T> &);
  inline DynamicVector<T> & operator+=(const T &);
  inline DynamicVector<T> & operator+=(const DynamicVector<T> &);
  inline DynamicVector<T> & operator-=(const T &);
  inline DynamicVector<T> & operator-=(const DynamicVector<T> &);
  inline DynamicVector<T> & operator^=(const DynamicVector<T> &);

  inline DynamicVector<T> operator+(const DynamicVector<T> &) const;
  inline DynamicVector<T> operator+(const T &) const;
  inline DynamicVector<T> operator-(const DynamicVector<T> &) const;
  inline DynamicVector<T> operator-(const T &) const;

  inline DynamicVector<T> operator/(const DynamicVector<T> &) const;
  inline DynamicVector<T> operator/(const T &) const;
  inline DynamicVector<T> operator^(const DynamicVector<T> &) const;

  inline bool operator!=(const DynamicVector<T> &) const;
  inline bool operator==(const DynamicVector<T> &) const;
  inline DynamicVector<T> & fill(const T &val);
  inline T norm() const;
  inline T dist(const DynamicVector<T> &) const;
  inline T dotProduct(const DynamicVector<T> &) const;

  inline unsigned int getSize() const {
    return size;
  }
protected:
  T *array;
  unsigned int size;
};

template<class T>
std::ostream& operator<<(std::ostream &os, const DynamicVector<T> &);
template<class T>
std::istream& operator>>(std::istream &is, DynamicVector<T> &);

#include "DynamicVector.cxx"
#endif /* DYNAMICVECTOR_H_ */
