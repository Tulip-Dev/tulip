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

///@cond DOXYGEN_HIDDEN

#ifndef UNIQUEITERATOR_H
#define UNIQUEITERATOR_H

#include <set>

#include <tulip/Iterator.h>
#include <tulip/StlIterator.h>
#include <tulip/memorypool.h>

//===============================================================================
namespace tlp {
/**
 * @brief That class implement a default functor that always returns true
 *
 * @see UniqueIterator
 */
template <typename T>
struct CheckAllFunctor {
  bool operator()(const T &) {
    return true;
  }
};

/**
 * @class UniqueIterator
 * @ingroup Iterators
 *
 * @brief UniqueIterator enables to remove duplicated elements in an iterator
 *
 * @param it the iterator in which we want to filter out duplicated elements
 * @param checkFunctor a functor or a lambda function that enables to indicate whether or not the
 *element could be duplicated (default test all elements)
 *
 * The functor function should have the following form
 * @code
 * template <typename T>
 * class ACheckFunctor {
 *  bool operator(const T &a) {
 *   return true if a could be duplicated else false;
 *  }
 * };
 * @endcode
 * checkFunctor are used for optimization purpose to prevent to log(n) test for all elements when
 *not necessary.
 **/
template <typename T, typename CheckFunc = CheckAllFunctor<T>>
class UniqueIterator : public Iterator<T> {
public:
  UniqueIterator(Iterator<T> *it, CheckFunc checkFunctor = CheckFunc())
      : _it(it), _checkFunctor(checkFunctor) {
    update();
  }
  //================================================
  ~UniqueIterator() {
    delete _it;
  }
  //================================================
  T next() {
    T tmp = curVal;
    update();
    return tmp;
  }
  //================================================
  inline bool hasNext() {
    return _hasNext;
  }
  //================================================
  void update() {
    _hasNext = false;

    while (_it->hasNext()) {
      curVal = _it->next();

      if (_checkFunctor(curVal)) {
        if (_flag.find(curVal) == _flag.end()) {
          _hasNext = true;
          _flag.insert(curVal);
          return;
        }
      } else {
        _hasNext = true;
        return;
      }
    }
  }

private:
  std::set<T> _flag;
  bool _hasNext;
  Iterator<T> *_it;
  T curVal;
  CheckFunc _checkFunctor;
};

template <typename T, typename CheckFunc = CheckAllFunctor<T>>
class MPUniqueIterator : public UniqueIterator<T, CheckFunc>,
                         public MemoryPool<MPUniqueIterator<T, CheckFunc>> {
public:
  MPUniqueIterator(Iterator<T> *it, CheckFunc checkFunctor = CheckFunc())
      : UniqueIterator<T, CheckFunc>(it, checkFunctor) {}
};

/**
 * @brief Convenient function for creating a UniqueIterator.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a UniqueIterator from another Iterator.
 * The returned Iterator takes ownership of the one provided as parameter.
 *
 * @param it a Tulip Iterator
 * @param checkFunctor a functor or a lambda function that enables to indicate whether or not the
 *element could be duplicated (default test all elements)
 *
 * @return a UniqueIterator
 *
 **/
template <typename T, typename CheckFunc = CheckAllFunctor<T>>
inline UniqueIterator<T> *uniqueIterator(Iterator<T> *it, CheckFunc checkFunctor = CheckFunc()) {
  return new MPUniqueIterator<T, CheckFunc>(it, checkFunctor);
}

/**
 * @brief Convenient function for creating a UniqueIterator from a STL container.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a UniqueIterator from a STL container (std::list, std::vector, std::set, std::map, ...).
 *
 * @param stlContainer any STL container
 * @param checkFunctor a functor or a lambda function that enables to indicate whether or not the
 *element could be duplicated (default test all elements)
 *
 * @return a UniqueIterator
 *
 **/
template <typename Container, typename CheckFunc = CheckAllFunctor<typename Container::value_type>>
typename std::enable_if<has_const_iterator<Container>::value,
                        UniqueIterator<typename Container::value_type, CheckFunc>
                            *>::type inline uniqueIterator(const Container &stlContainer,
                                                           CheckFunc checkFunctor = CheckFunc()) {
  return new MPUniqueIterator<typename Container::value_type, CheckFunc>(stlIterator(stlContainer),
                                                                         checkFunctor);
}
} // namespace tlp
#endif // UNIQUEITERATOR_H

///@endcond
