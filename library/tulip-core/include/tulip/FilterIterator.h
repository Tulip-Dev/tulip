/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef FILTERITERATOR_H
#define FILTERITERATOR_H

#include <tulip/Iterator.h>
#include <tulip/StlIterator.h>
#include <tulip/memorypool.h>

namespace tlp {
/**
 * @class FilterIterator
 * @ingroup Iterators
 * @brief Iterator that enables to filter an other Iterator
 * @param it the iterator that should be filtered
 * @param filter the functor or lambda function that enables to test whether or not an element is
 *filtered
 *
 * The functor function should have the following form:
 * @code
 * class AFilterFunctor {
 *  bool operator()(TYPE a) {
 *    return true if a should be iterated, false if a should be removed;
 *  }
 * };
 * @endcode
 **/
template <typename TYPE, typename FILTER>
class FilterIterator : public Iterator<TYPE> {
public:
  FilterIterator(Iterator<TYPE> *it, FILTER filter) : _it(it), _filter(filter) {
    update();
  }
  ~FilterIterator() {
    delete _it;
  }
  inline TYPE next() {
    TYPE tmp = _curVal;
    update();
    return tmp;
  }
  inline bool hasNext() {
    return _hasNext;
  }

private:
  void update() {
    _hasNext = false;

    while (_it->hasNext()) {
      _curVal = _it->next();

      if (_filter(_curVal)) {
        _hasNext = true;
        break;
      }
    }
  }

  bool _hasNext;
  Iterator<TYPE> *_it;
  TYPE _curVal;
  FILTER _filter;
  size_t _nbele;
};

template <typename TYPE, typename FILTER>
class MPFilterIterator : public FilterIterator<TYPE, FILTER>,
                         public MemoryPool<MPFilterIterator<TYPE, FILTER>> {
public:
  MPFilterIterator(Iterator<TYPE> *it, FILTER filter) : FilterIterator<TYPE, FILTER>(it, filter) {}
};

/**
 * @brief Convenient function for creating a FilterIterator.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a FilterIterator from another iterator and a filter function.
 * The returned iterator takes ownership of the one provided as parameter.
 *
 * @param it a Tulip iterator
 * @param filter the functor or lambda function that enables to test whether or not an element is
 *filtered
 * @return a FilterIterator
 **/
template <typename TYPE, typename FILTER>
inline FilterIterator<TYPE, FILTER> *filterIterator(Iterator<TYPE> *it, FILTER filter) {
  return new MPFilterIterator<TYPE, FILTER>(it, filter);
}

/**
 * @brief Convenient function for creating a FilterIterator from a STL container.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a FilterIterator from a STL container (std::list, std::vector, std::set, std::map, ...)
 * and a filter function.
 *
 * @param stlContainer any STL container
 * @param filter the functor or lambda function that enables to test whether or not an element is
 *filtered
 * @return a FilterIterator
 **/
template <typename Container, typename FILTER>
typename std::enable_if<has_const_iterator<Container>::value,
                        MPFilterIterator<typename Container::value_type, FILTER>
                            *>::type inline filterIterator(const Container &stlContainer,
                                                           FILTER filter) {
  return new MPFilterIterator<typename Container::value_type, FILTER>(stlIterator(stlContainer),
                                                                      filter);
}
} // namespace tlp
#endif // FILTERITERATOR_H
