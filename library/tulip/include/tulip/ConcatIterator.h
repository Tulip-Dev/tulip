/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

/**
 * This iterator enables to concatenate two iterators.
 * The iterators given in parameter will be automatically
 * delete when the iterator will be deleted.
 */
namespace tlp {

template<class itType> struct ConcatIterator : public Iterator<itType> {
  ConcatIterator(Iterator<itType> *itOne,Iterator<itType> *itTwo) : 
    itOne(itOne),
    itTwo(itTwo) {
  }
  ~ConcatIterator() {
    delete itOne;
    delete itTwo;
  };
  itType next() {
    if (itOne->hasNext())
      return itOne->next();
    else {
      return itTwo->next();
    }
  }
  bool hasNext() {
    return (itOne->hasNext() || itTwo->hasNext());
  };

  private :
  Iterator<itType> *itOne;
  Iterator<itType> *itTwo;
};

}
#endif
