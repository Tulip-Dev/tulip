/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef TULIP_STLITERATOR_H
#define TULIP_STLITERATOR_H
#include <map>
#include "tulip/Iterator.h"

namespace tlp {

  template<typename VALUE, typename ITERATOR> 
  struct StlIterator:public Iterator< VALUE  > {
    StlIterator(const ITERATOR &startIt, const ITERATOR &endIt):
      it(startIt),
      itEnd(endIt) {
    }
    VALUE next() {
      VALUE tmp = *it;
      ++it;
      return tmp;
    }
    bool hasNext() {
      return (itEnd!=it);
    }
  private:
    ITERATOR it, itEnd;
  };
  //=================================================
  template<typename KEY, typename VALUE> 
  struct StlMapIterator:public Iterator< std::pair<KEY,VALUE> > {
    StlMapIterator(typename std::map<KEY,VALUE>::const_iterator startIt, typename std::map<KEY,VALUE>::const_iterator endIt):
      it(startIt),
      itEnd(endIt)
    {}
    std::pair<KEY,VALUE> next();
    bool hasNext();
  private:
    typename std::map<KEY,VALUE>::const_iterator it, itEnd;
  };
  //=================================================
  ///  StlMapIterator implemetation
  template<typename KEY, typename VALUE>
  std::pair<KEY,VALUE> StlMapIterator<KEY,VALUE>::next(){
    std::pair<KEY,VALUE> tmp=*it;
    ++it;
    return tmp;
  }
  template<typename KEY, typename VALUE>
  bool StlMapIterator<KEY,VALUE>::hasNext(){
    return (itEnd!=it);
  }
  //=================================================

}
#endif
