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

#ifndef Tulip_FOREACH_H
#define Tulip_FOREACH_H

#include <cassert>
#include <tulip/Iterator.h>
#include <tulip/StableIterator.h>
#include <tulip/tulipconf.h>

#ifndef DOXYGEN_NOTFOR_DEVEL
namespace tlp {
/**
* @brief Encapsulation of a Tulip Iterator intended to be allocated on the stack instead of the heap,
* so it gets deleted when out of scope.
*
**/
template<typename TYPE>
struct _TLP_IT {
  _TLP_IT(Iterator<TYPE> *_it) : _it(_it) {
  }
  ~_TLP_IT() {
    delete _it;
  }
  Iterator<TYPE> *_it;
};

/**
* @brief
**/
template<typename TYPE>
inline bool _tlp_if_test(TYPE &n, _TLP_IT<TYPE> &_it) {
  assert(_it._it != NULL);

  if(_it._it->hasNext()) {
    n = _it._it->next();
    return true;
  }
  else {
    return false;
  }
}
}
#endif //DOXYGEN_NOTFOR_DEVEL

/**
 * @brief Allows to iterate the nodes or edges of a Graph in a clear and concise way.
 * It also avoid having to manage a tulip Iterator, whose deletion is often forgotten.
 *
 * This code shows how forEach can be used instead of an Iterator to iterate a Graph's nodes
 * @code
 * node n;
 * forEach(n, graph->getNodes()) {
 *   // Do something with node n
 * }
 * @endcode
 *
 * This macro can be used with any Iterator subclass as it's based on the existence of the next() and hasNext() methods
 */
#define forEach(A, B) \
for(tlp::_TLP_IT<TYPEOF(A) > _it_foreach(B); tlp::_tlp_if_test(A, _it_foreach);)

/**
 * @brief Allows to iterate the nodes or edges of a copy of a Graph in a clear and concise way.
 * The stable Iterator creates a copy of the Graph, and iterates on the said copy.
 * It allows deletion operations to be performed without invalidating the iterator.
 * It also avoid having to manage a tulip Iterator, whose deletion is often forgotten.
 */
#define stableForEach(A, B)  \
  for(tlp::_TLP_IT<TYPEOF(A) > _it_foreach(new StableIterator<TYPEOF(A) >(B));  tlp::_tlp_if_test(A, _it_foreach);)

#if defined(__GXX_EXPERIMENTAL_CXX0X__)
namespace tlp {

template<typename T>
class iterator_t  {
public:
  enum IteratorType {
    Begin = 0,
    End = 1
  };

  iterator_t(tlp::Iterator<T>* it, IteratorType begin = End) : _finished(false), _iteratorType(begin), _it(it) {
    if(_iteratorType == Begin) {
      if(_it->hasNext()) {
        _current = _it->next();
      }
      else {
        _finished = true;
      }
    }
  }

  ~iterator_t() {
    if(_iteratorType == Begin) {
      delete _it;
    }
  }

  bool operator!=(const iterator_t&) const {
    return !_finished;
  }

  const iterator_t& operator++() {
    _finished = !_it->hasNext();

    if(!_finished)
      _current = _it->next();

    return *this;
  }

  T operator*() const {
    return _current;
  }

protected:
  bool _finished;
  IteratorType _iteratorType;
  tlp::Iterator<T>* _it;
  T _current;
};

template<typename T>
iterator_t<T> begin(tlp::Iterator<T>* it) {
  return iterator_t<T>(it, iterator_t<T>::Begin);
}

template<typename T>
iterator_t<T> end(tlp::Iterator<T>* it) {
  return iterator_t<T>(it);
}

}
#endif //__GXX_EXPERIMENTAL_CXX0X__

#endif
