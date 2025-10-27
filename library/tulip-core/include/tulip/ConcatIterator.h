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

#ifndef TULIP_CONCATITERATOR_H
#define TULIP_CONCATITERATOR_H

#include <tulip/Iterator.h>

namespace tlp {

/**
 * @brief This Iterator iterates over the sequence formed by the concatenation of the sequences it
 *is given.
 * @warning This class takes ownership of the Iterators it is given.
 **/
template <class T>
struct ConcatIterator : public Iterator<T> {

  /**
   * @brief Creates an Iterator that iterates over the concatenation of the two sequences it is
   *given.
   *
   * @param itOne The first sequence to iterate upon.
   * @param itTwo The second sequence, which will be iterated upon after the first sequence has been
   *completely iterated upon.
   **/
  ConcatIterator(Iterator<T> *itOne, Iterator<T> *itTwo) : itOne(itOne), itTwo(itTwo) {}

  /**
   * @brief Deletes the two iterators it was given at construction.
   **/
  ~ConcatIterator() override {
    delete itOne;
    delete itTwo;
  }

  T next() override {
    if (itOne->hasNext())
      return itOne->next();
    else {
      return itTwo->next();
    }
  }

  bool hasNext() override {
    return (itOne->hasNext() || itTwo->hasNext());
  }

private:
  Iterator<T> *itOne;
  Iterator<T> *itTwo;
};

/**
 * @brief Convenient function for creating a ConcatIterator.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a ConcatIterator from two other Iterators.
 * The returned Iterator takes ownership of the one provided as parameter.
 *
 * @param itOne the first input Iterator
 * @param itTwo the second input Iterator
 * @return a ConcatIterator
 **/
template <class T>
Iterator<T> *concatIterator(Iterator<T> *itOne, Iterator<T> *itTwo) {
  return new ConcatIterator<T>(itOne, itTwo);
}
} // namespace tlp
#endif
