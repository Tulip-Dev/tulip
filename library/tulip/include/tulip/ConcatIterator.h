//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 10/05/2003
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
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
