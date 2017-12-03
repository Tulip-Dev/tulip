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

#ifndef TULIP_ITERATOR_H
#define TULIP_ITERATOR_H

#include <list>
#include <set>
#include <tuple>
#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {

///@cond DOXYGEN_HIDDEN
extern TLP_SCOPE void incrNumIterators();
extern TLP_SCOPE void decrNumIterators();
extern TLP_SCOPE int getNumIterators();
///@endcond

/**
* @brief Interface for Tulip iterators.
* Allows basic iteration operations only.
*
* Below are some examples about how to use Tulip iterators in C++ code.
*
* @code
*
* // graph is a pointer to a tlp::Graph instance
*
* // shortest syntax using C++11 for range based loops
* for (const tlp::node &n : graph->getNodes()) {
*   // do someting with n
* }
*
* // legacy syntax using Tulip forEach macro
* #include <tulip/ForEach.h>
* tlp::node n;
* forEach(n, graph->getNodes()) {
*   // do something with n
* }
*
* // no syntactic sugar syntax (needs to explicitely delete the iterator
* // after its use)
* tlp::Iterator<tl::node> *nodesIt = graph->getNodes();
* while (nodesIt->hasNext()) {
*   tlp::node n = nodes->next();
*   // do something with n
* }
* delete nodesIt;
* @endcode
**/
template <typename T>
struct Iterator {
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
  virtual T next() = 0;

  /**
  * @brief Tells if the sequence is at its end.
  *
  * @return bool Whether there are more elements to iterate.
  **/
  virtual bool hasNext() = 0;
};

/**
* @brief Counts the number of iterated elements
* @ingroup Iterators
*
* @since Tulip 5.2
*
* Counts the number of elements iterated by the provided iterator.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
*
* @return The number of iterated elements
**/
template <typename T>
inline unsigned int iteratorCount(Iterator<T> *it) {
  unsigned int count = 0;
  for (const T &v : it) {
    std::ignore = v;
    ++count;
  }
  return count;
}

/**
* @brief Checks a mimimum amount of iterated elements
* @ingroup Iterators
*
* @since Tulip 5.2
*
* Checks if the iterator returns at least n values.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
*
* @return true if the iterator returns at least n values
**/
template <typename T>
inline bool iteratorCountCheck(Iterator<T> *it, unsigned int minNbElements) {
  unsigned int count = 0;
  for (const T &v : it) {
    std::ignore = v;
    ++count;
    if (count == minNbElements) {
      return true;
    }
  }
  return false;
}

/**
* @brief Checks if an iterator is empty
* @ingroup Iterators
*
* @since Tulip 5.2
*
* Checks if the iterator does not return any value.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
*
* @return true if the iterator is empty
**/
template <typename T>
inline bool iteratorEmpty(Iterator<T> *it) {
  return !iteratorCountCheck(it, 1);
}

/**
* @brief Applies a function to each iterated element
* @ingroup Iterators
*
* @since Tulip 5.2
*
* Applies a function to each element iterated by the provided iterator.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
* @param mapFunction functor or lambda function taking one parameter of the same type of the
*iterated elements
*
*
**/
template <typename T, class MapFunction>
inline void iteratorMap(Iterator<T> *it, MapFunction &&mapFunction) {
  for (const T &v : it) {
    mapFunction(v);
  }
  delete it;
}

/**
* @brief Reduces iterated elements to a single value
* @ingroup Iterators
*
* @since Tulip 5.2
*
* Produces a single value from the iterated elements (e.g. sum).
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
* @param initVal initial value of the reduced result
* @param reduceFunction functor or lambda function taking two parameters : first one being the
*current reduced value,
* second one being the current iterated element and returning intermediate reduced value
*
* @return the reduced value from the iterated elements
*
**/
template <typename itType, typename reduceType, class ReduceFunction>
inline reduceType iteratorReduce(Iterator<itType> *it, const reduceType &initVal,
                                 ReduceFunction reduceFunction) {
  reduceType val = initVal;
  while (it->hasNext()) {
    val = reduceFunction(val, it->next());
  }
  delete it;
  return val;
}

/**
* @brief Converts an iterator to a std::list
* @ingroup Iterators
*
* @since Tulip 5.2
*
* Returns a std::list filled with the iterated elements.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
*
* @return a std::list filled with iterated elements
**/
template <typename T>
inline std::list<T> iteratorList(Iterator<T> *it) {
  std::list<T> ret;
  for (const T &v : it) {
    ret.push_back(v);
  }
  return ret;
}

/**
* @brief Converts an iterator to a std::vector
* @ingroup Iterators
*
* @since Tulip 5.2
*
* Returns a std::vector filled with the iterated elements.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
*
* @return a std::vector filled with iterated elements
**/
template <typename T>
inline std::vector<T> iteratorVector(Iterator<T> *it) {
  std::vector<T> ret;
  for (const T &v : it) {
    ret.push_back(v);
  }
  return ret;
}

/**
* @brief Converts an iterator to a std::set
* @ingroup Iterators
*
* @since Tulip 5.2
*
* Returns a std::set filled with the iterated elements.
* That function takes ownership of the iterator and will delete it.
*
* @param it a Tulip iterator
*
* @return a std::set filled with iterated elements
**/
template <typename T>
inline std::set<T> iteratorSet(Iterator<T> *it) {
  std::set<T> ret;
  for (const T &v : it) {
    ret.insert(v);
  }
  return ret;
}

#ifndef DOXYGEN_NOTFOR_DEVEL
template <typename TYPE>
class UINTIterator : public Iterator<TYPE> {
public:
  UINTIterator(Iterator<unsigned int> *it) : it(it) {}
  ~UINTIterator() override {
    delete it;
  }
  bool hasNext() override {
    return it->hasNext();
  }
  TYPE next() override {
    return TYPE(it->next());
  }

private:
  Iterator<unsigned int> *it;
};

// glue code in order to use Tulip iterators with C++11 for range based loops
template <typename T>
class iterator_t {
public:
  enum IteratorType { Begin = 0, End = 1 };

  iterator_t(tlp::Iterator<T> *it, IteratorType iteratorType = End)
      : _finished(true), _iteratorType(iteratorType), _it(it) {
    if (_iteratorType == Begin) {
      _finished = !_it->hasNext();
    }
  }

  ~iterator_t() {
    if (_iteratorType == Begin) {
      delete _it;
    }
  }

  inline bool operator!=(const iterator_t &it) const {
    return _finished != it._finished;
  }

  inline const iterator_t &operator++() {
    _finished = !_it->hasNext();
    return *this;
  }

  inline T operator*() const {
    return _it->next();
  }

protected:
  bool _finished;
  IteratorType _iteratorType;
  tlp::Iterator<T> *_it;
};

template <typename T>
inline iterator_t<T> begin(tlp::Iterator<T> *it) {
  return iterator_t<T>(it, iterator_t<T>::Begin);
}

template <typename T>
inline iterator_t<T> end(tlp::Iterator<T> *it) {
  return iterator_t<T>(it);
}

#endif // DOXYGEN_NOTFOR_DEVEL
}

#ifdef _MSC_VER

#include <tulip/Edge.h>
#include <tulip/Node.h>

template struct TLP_SCOPE tlp::Iterator<tlp::edge>;
template struct TLP_SCOPE tlp::Iterator<tlp::node>;
#endif
#endif
