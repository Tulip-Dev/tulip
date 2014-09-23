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

#ifndef TULIP_STABLEITERATOR_H
#define TULIP_STABLEITERATOR_H
#include <tulip/Iterator.h>
#include <tulip/tulipconf.h>
#include <vector>
#include <cstdlib>

namespace tlp {
/**
  * @class StableIterator
  * @brief Stores the elements of an iterator and iterates a copy.
  *
  * This Iterator stores all the elements accessible by another Iterator into an internal data
  * structure (created at the construction), and then uses this structure for the iteration.
  * Iteration order is the same.
  *
  * @warning By default StableIterator takes ownership of the iterator given in parameter, (ie,
  * delete will be called on the input iterator). The deletion takes place when constructing the StableIterator.
  *
  * This class is really useful when one needs to modify the graph during an iteration. For
  * instance the following code remove all nodes that match the function myfunc().
  * Using standard iterators for that operations is not possible since we modify the graph.
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
  * @see stableForEach
  */
template<class itType>
struct StableIterator : public Iterator<itType> {
  //=============================
  /**
  * @brief Creates a stable Iterator, that allows to delete elements from a graph while iterating on them.
  *
  * @param inputIterator Input Iterator, which defines the sequence on which this Iterator will iterate.
  * @param nbElements The number of elements the iteration will take place on. Defaults to 0.
  * @param deleteIterator Whether or not to delete the Iterator given as first parameter. Defaults to true.
  **/
  StableIterator(Iterator<itType> *inputIterator, size_t nbElements = 0, bool deleteIterator = true) {
    sequenceCopy.reserve(nbElements);

    for (; inputIterator->hasNext();) {
      sequenceCopy.push_back(inputIterator->next());
    }

    if (deleteIterator)
      delete inputIterator;

    copyIterator = sequenceCopy.begin();
  }
  //=============================
  ~StableIterator() {}
  //=============================
  itType next() {
    itType tmp(*copyIterator);
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
protected :
  /**
  * @brief A copy of the sequence of the elements to iterate.
  **/
  std::vector<itType> sequenceCopy;

  /**
  * @brief STL const_iterator on the cloned sequence.
  **/
  typename std::vector<itType>::const_iterator copyIterator;
};

}
#endif
///@endcond
