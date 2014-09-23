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

#ifndef TULIP_CONCATITERATOR_H
#define TULIP_CONCATITERATOR_H
#include <tulip/Iterator.h>

namespace tlp {

/**
* @brief This Iterator iterates over the sequence formed by the concatenation of the sequences it is given.
* @warning This class takes ownership of the Iterators it is given.
**/
template<class itType> struct ConcatIterator : public Iterator<itType> {

  /**
  * @brief Creates an Iterator that iterates over the concatenation of the two sequences it is given.
  *
  * @param itOne The first sequence to iterate upon.
  * @param itTwo The second sequence, which will be iterated upon after the first sequence has been completely iterated upon.
  **/
  ConcatIterator(Iterator<itType> *itOne,Iterator<itType> *itTwo) :
    itOne(itOne),
    itTwo(itTwo) {
  }

  /**
  * @brief Deletes the two iterators it was given at construction.
  **/
  ~ConcatIterator() {
    delete itOne;
    delete itTwo;
  }

  itType next() {
    if (itOne->hasNext())
      return itOne->next();
    else {
      return itTwo->next();
    }
  }

  bool hasNext() {
    return (itOne->hasNext() || itTwo->hasNext());
  }

private :
  Iterator<itType> *itOne;
  Iterator<itType> *itTwo;
};

}
#endif
///@endcond
