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

#ifndef TULIP_ITERATOR_H
#define TULIP_ITERATOR_H

#include <tulip/tulipconf.h>

namespace tlp {

#ifndef DOXYGEN_NOTFOR_DEVEL
extern TLP_SCOPE void incrNumIterators();
extern TLP_SCOPE void decrNumIterators();
extern TLP_SCOPE int getNumIterators();
#endif // DOXYGEN_NOTFOR_DEVEL

/**
* @brief Interface for Tulip iterators.
* @ingroup Iterators
*
* Allows basic iteration operations only.
*
**/
template<class itType> struct Iterator {
  ///
  Iterator() {
#ifndef NDEBUG
    incrNumIterators();
#endif
  }
  ///
  virtual ~Iterator() {
#ifndef NDEBUG
    decrNumIterators();
#endif
  }
  /**
  * @brief Moves the Iterator on the next element.
  *
  * @return The current element pointed by the Iterator.
  **/
  virtual itType next()=0;

  /**
  * @brief Tells if the sequence is at its end.
  *
  * @return bool Whether there are more elements to iterate.
  **/
  virtual bool hasNext()=0;
};

//template<class C>class Iterator;
#ifndef DOXYGEN_NOTFOR_DEVEL
template<typename TYPE> class UINTIterator : public Iterator<TYPE> {
public:
  UINTIterator(Iterator<unsigned int> *it):it(it) {
  }
  ~UINTIterator() {
    delete it;
  }
  bool hasNext() {
    return it->hasNext();
  }
  TYPE next() {
    return TYPE(it->next());
  }
private:
  Iterator<unsigned int> *it;
};
#endif // DOXYGEN_NOTFOR_DEVEL

/**
* @brief Counts the number of iterated elements
* @ingroup Iterators
*
* Counts the number of elements iterated by the provided iterator.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
*
* @return The number of iterated elements
**/
template<typename itType>
unsigned int iteratorCount(Iterator<itType> *it) {
  unsigned int count = 0;
  while(it->hasNext()) {
    ++count;
    it->next();
  }
  delete it;
  return count;
}

/**
* @brief Applies a function to each iterated element
* @ingroup Iterators
*
* Applies a function to each element iterated by the provided iterator.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
* @param mapFunction functor or lambda function taking one parameter of the same type of the iterated elements
*
**/
template<typename itType, class MapFunction>
void iteratorMap(Iterator<itType> *it, MapFunction mapFunction) {
  while(it->hasNext()) {
    mapFunction(it->next());
  }
  delete it;
}

/**
* @brief Reduces iterated elements to a single value
* @ingroup Iterators
*
* Produces a single value from the iterated elements (e.g. sum).
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
* @param initVal initial value of the reduced result
* @param reduceFunction functor or lambda function taking two parameters : first one being the current reduced value,
* second one being the current iterated element and returning intermediate reduced value
*
* @return the reduced value from the iterated elements
*
**/
template<typename itType, typename reduceType, class ReduceFunction>
reduceType iteratorReduce(tlp::Iterator<itType> *it, const reduceType &initVal, ReduceFunction reduceFunction) {
  reduceType val = initVal;
  while(it->hasNext()) {
    val = reduceFunction(val, it->next());
  }
  delete it;
  return val;
}

}

#ifdef _MSC_VER

#include <tulip/Edge.h>
#include <tulip/Node.h>

template struct TLP_SCOPE tlp::Iterator<tlp::edge>;
template struct TLP_SCOPE tlp::Iterator<tlp::node>;
#endif
#endif

///@endcond
