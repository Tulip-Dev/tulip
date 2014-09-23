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

#ifndef _TLPMUTABLECONTAINER_
#define _TLPMUTABLECONTAINER_

#include <deque>
#include <iostream>
#include <string>
#include <cassert>
#include <climits>
#include <cstring>
#include <tulip/tulipconf.h>
#include <tulip/tuliphash.h>
#include <tulip/StoredType.h>
#include <tulip/DataSet.h>
#include <tulip/Iterator.h>

namespace tlp {

#ifndef DOXYGEN_NOTFOR_DEVEL
//===================================================================
// we first define an interface
// to make easier the iteration on values
// stored in a MutableContainer for the GraphUpdatesRecorder
class IteratorValue: public Iterator<unsigned int> {
public:
  IteratorValue() {}
  virtual ~IteratorValue() {}
  virtual unsigned int nextValue(DataMem&) = 0;
};
#endif // DOXYGEN_NOTFOR_DEVEL
//===================================================================
template <typename TYPE>
class MutableContainer {
  friend class MutableContainerTest;
  friend class GraphUpdatesRecorder;
public:
  MutableContainer();
  ~MutableContainer();
  /**
   * set the default value
   */
  void setAll(const TYPE &value);
  /**
   * set the value associated to i
   */
  void set(const unsigned int i, const TYPE &value);
  /**
   * add val to the value associated to i
   */
  void add(const unsigned int i, TYPE val);
  /**
   * get the value associated to i
   */
  typename StoredType<TYPE>::ReturnedConstValue get(const unsigned int i) const;
  /**
   * get the value associated to i and indicates if it is not the default one
   */
  typename StoredType<TYPE>::ReturnedValue get(const unsigned int i, bool& isNotDefault) const;
  /**
   * get the default value
   */
  typename StoredType<TYPE>::ReturnedValue getDefault() const;
  /**
   * return if the value associated to i is not the default one
   */
  bool hasNonDefaultValue(const unsigned int i) const;
  /**
   * return a pointer on an iterator for all the elements whose associated value
   * is equal to a given value or different from the default value.
   * A null pointer is returned in case of an iteration on all the elements
   * whose value is equal to the default value.
   */
  Iterator<unsigned int>* findAll(const TYPE &value, bool equal = true) const;
  /**
   * return the number of non default values
   */
  unsigned int numberOfNonDefaultValues() const;
private:
  MutableContainer(const MutableContainer<TYPE> &) {}
  void operator=(const MutableContainer<TYPE> &) {}
  typename StoredType<TYPE>::ReturnedConstValue operator[](const unsigned int i) const;
  void vecttohash();
  void hashtovect();
  void compress(unsigned int min, unsigned int max, unsigned int nbElements);
  inline void vectset(const unsigned int i, typename StoredType<TYPE>::Value value);
  IteratorValue* findAllValues(const TYPE &value, bool equal = true) const;
private:
  std::deque<typename StoredType<TYPE>::Value> *vData;
  TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value> *hData;
  unsigned int minIndex,maxIndex;
  typename StoredType<TYPE>::Value defaultValue;
  enum State { VECT=0, HASH=1 };
  State state;
  unsigned int elementInserted;
  double ratio;
  bool compressing;
};

//===================================================================
// we implement 2 templates with IteratorValue as parent class
// for the two kinds of storage used in a MutableContainer
// one for vector storage
template <typename TYPE>
class IteratorVect : public tlp::IteratorValue {
public:
  IteratorVect(const TYPE &value, bool equal, std::deque<typename StoredType<TYPE>::Value> *vData, unsigned int minIndex):
    _value(value),
    _equal(equal),
    _pos(minIndex),
    vData(vData),
    it(vData->begin()) {
    while (it!=(*vData).end() &&
           StoredType<TYPE>::equal((*it), _value) != _equal) {
      ++it;
      ++_pos;
    }
  }
  bool hasNext() {
    return (_pos<UINT_MAX && it!=(*vData).end());
  }
  unsigned int next() {
    unsigned int tmp = _pos;

    do {
      ++it;
      ++_pos;
    }
    while (it!=(*vData).end() &&
           StoredType<TYPE>::equal((*it), _value) != _equal);

    return tmp;
  }
  unsigned int nextValue(DataMem& val) {
    ((TypedValueContainer<TYPE>&) val).value = StoredType<TYPE>::get(*it);
    unsigned int pos = _pos;

    do {
      ++it;
      ++_pos;
    }
    while (it!=(*vData).end() &&
           StoredType<TYPE>::equal((*it), _value) != _equal);

    return pos;
  }
private:
  const TYPE _value;
  bool _equal;
  unsigned int _pos;
  std::deque<typename StoredType<TYPE>::Value> *vData;
  typename std::deque<typename StoredType<TYPE>::Value>::const_iterator it;
};

// one for hash storage
template <typename TYPE>
class IteratorHash : public IteratorValue {
public:
  IteratorHash(const TYPE &value, bool equal, TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value> *hData):
    _value(value),
    _equal(equal),
    hData(hData) {
    it=(*hData).begin();

    while (it!=(*hData).end() &&
           StoredType<TYPE>::equal((*it).second,_value) != _equal)
      ++it;
  }
  bool hasNext() {
    return (it!=(*hData).end());
  }
  unsigned int next() {
    unsigned int tmp = (*it).first;

    do {
      ++it;
    }
    while (it!=(*hData).end() &&
           StoredType<TYPE>::equal((*it).second,_value) != _equal);

    return tmp;
  }
  unsigned int nextValue(DataMem& val) {
    ((TypedValueContainer<TYPE>&) val).value =
      StoredType<TYPE>::get((*it).second);
    unsigned int pos = (*it).first;

    do {
      ++it;
    }
    while (it!=(*hData).end() &&
           StoredType<TYPE>::equal((*it).second,_value) != _equal);

    return pos;
  }
private:
  const TYPE _value;
  bool _equal;
  TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value> *hData;
  typename TLP_HASH_MAP<unsigned int, typename StoredType<TYPE>::Value>::const_iterator it;
};

#ifndef DOXYGEN_NOTFOR_DEVEL

}

#include "cxx/MutableContainer.cxx"

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
///@endcond
