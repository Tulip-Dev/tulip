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


#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef _SIMPLE_VECTOR_H_
#define _SIMPLE_VECTOR_H_
#include <cstdlib>

namespace tlp {

// SimpleVector
//   simple copy constructor
//   deallocate with hand

template<typename T>
class SimpleVector {
protected:
  T *beginP;
  T *middleP;
  T *endP;

public:
  typedef T *iterator;
  typedef const T *const_iterator;

  SimpleVector():beginP(NULL),middleP(NULL),endP(NULL) {  }
  SimpleVector(const SimpleVector &v):
    beginP(v.beginP),middleP(v.middleP),endP(v.endP) { }
  explicit SimpleVector(size_t s):beginP(NULL),middleP(NULL),endP(NULL) {
    resize(s);
  }
  void resize(size_t s) {
    middleP=beginP+s;

    if (middleP>endP || size()<capacity()/2)
      doRealloc(size());
  }
  iterator begin() {
    return beginP;
  }
  iterator end() {
    return middleP;
  }
  const_iterator begin() const {
    return beginP;
  }
  const_iterator end() const {
    return middleP;
  }
  T &front() {
    assert(!empty());
    return *beginP;
  }
  const T &front() const {
    assert(!empty());
    return *beginP;
  }
  T &back() {
    assert(!empty());
    return *(middleP-1);
  }
  const T &back() const {
    assert(!empty());
    return *(middleP-1);
  }
  size_t size() const {
    return size_t(middleP-beginP);
  }
  size_t capacity() const {
    return size_t(endP-beginP);
  }
  bool empty() const {
    return beginP == endP;
  }
  T &operator[](size_t i) {
    assert(i<size());
    return *(beginP+i);
  }
  T operator[](size_t i) const {
    assert(i<size());
    return *(beginP+i);
  }
  void push_back(const T &v) {
    if (middleP==endP) {
      size_t s=size();
      doRealloc(s==0?1:2*s);
    }

    *middleP=v;
    ++middleP;
  }
  void pop_back() {
    assert(!empty());
    --middleP;

    if (size()<capacity()/2)
      doRealloc(size());
  }
  void clear() {
    deallocateAll();
    beginP=middleP=endP=NULL;
  }
  void deallocateAll() {
    free(beginP);
  }
protected:
  void doRealloc(size_t s) {
    size_t i=middleP-beginP;
    beginP=(T *)realloc(beginP,s*sizeof(T));
    middleP=beginP+i;
    endP=beginP+s;
    assert(middleP<=endP);
  }
};

}
#endif
#endif //DOXYGEN_NOTFOR_DEVEL
///@endcond
