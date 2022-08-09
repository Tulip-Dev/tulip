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

#ifndef TULIP_STABLEITERATOR_H
#define TULIP_STABLEITERATOR_H

#include <vector>
#include <cstdlib>
#include <algorithm>

#include <tulip/Iterator.h>
#include <tulip/StlIterator.h>
#include <tulip/tulipconf.h>

namespace tlp {
/**
 * @class StableIterator
 * @ingroup Iterators
 * @brief Stores the elements of an iterator and iterates a copy.
 *
 * This Iterator stores all the elements accessible by another Iterator into an internal data
 * structure (created at the construction), and then uses this structure for the iteration.
 * Iteration order is the same.
 *
 * @warning By default StableIterator takes ownership of the iterator given in parameter, (ie,
 * delete will be called on the input iterator). The deletion takes place when constructing the
 *StableIterator.
 *
 * This class is really useful when one needs to modify the graph during an iteration. For
 * instance the following code remove all nodes that match the function myfunc().
 * Using standard iterators for that operation is not possible since we modify the graph.
 *
 * @code
 * StableIterator<node> it(graph->getNodes());
 * while(it.hasNext()) {
 *  node n = it.next();
 *  if (myfunc(n))
 *     graph->delNode(n);
 * }
 * @endcode
 *
 **/
template <typename T>
struct StableIterator : public Iterator<T> {
  //=============================
  /**
   * @brief Creates a stable Iterator, that allows to delete elements from a graph while iterating
   *on them.
   *
   * @param inputIterator Input Iterator, which defines the sequence on which this Iterator will
   *iterate.
   * @param nbElements The number of elements the iteration will take place on. Defaults to 0.
   * @param deleteIterator Whether or not to delete the Iterator given as first parameter. Defaults
   *to true.
   **/
  StableIterator(Iterator<T> *inputIterator, size_t nbElements = 0, bool deleteIterator = true,
                 bool sortCopy = false) {
    sequenceCopy.reserve(nbElements);

    for (; inputIterator->hasNext();) {
      sequenceCopy.push_back(inputIterator->next());
    }

    if (deleteIterator)
      delete inputIterator;

    if (sortCopy)
      std::sort(sequenceCopy.begin(), sequenceCopy.end());

    copyIterator = sequenceCopy.begin();
  }
  //=============================
  ~StableIterator() {}
  //=============================
  T next() {
    T tmp(*copyIterator);
    ++copyIterator;
    return tmp;
  }
  //=============================
  bool hasNext() {
    return (copyIterator != sequenceCopy.end());
  }
  //=============================

  /**
   * @brief Restarts the iteration by moving the Iterator to the beginning of the sequence.
   *
   * @return void
   **/
  void restart() {
    copyIterator = sequenceCopy.begin();
  }
  //=============================
protected:
  /**
   * @brief A copy of the sequence of the elements to iterate.
   **/
  std::vector<T> sequenceCopy;

  /**
   * @brief STL const_iterator on the cloned sequence.
   **/
  typename std::vector<T>::const_iterator copyIterator;
};

/**
 * @brief Convenient function for creating a StableIterator.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a StableIterator from another Iterator.
 * The returned Iterator takes ownership of the one provided as parameter.
 *
 * @param it a Tulip Iterator
 * @return a StableIterator
 **/
template <class T>
inline StableIterator<T> *stableIterator(Iterator<T> *it) {
  return new StableIterator<T>(it);
}

/**
 * @brief Convenient function for creating a StableIterator from a STL container.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a StableIterator from a STL container (std::list, std::vector, std::set, std::map, ...).
 *
 * @param stlContainer any STL container
 * @return a StableIterator
 **/
template <typename Container>
typename std::enable_if<has_const_iterator<Container>::value,
                        StableIterator<typename Container::value_type>
                            *>::type inline stableIterator(const Container &stlContainer) {
  return new StableIterator<typename Container::value_type>(stlIterator(stlContainer));
}
} // namespace tlp
#endif
