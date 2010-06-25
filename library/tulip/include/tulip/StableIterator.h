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
#ifndef TULIP_STABLEITERATOR_H
#define TULIP_STABLEITERATOR_H
#include <tulip/Iterator.h>
#include <tulip/tulipconf.h>
#include <vector>

namespace tlp {

///Interface of StableIterator,
template<class itType> struct StableIterator : public Iterator<itType> {
  StableIterator(Iterator<itType> *itIn) {
    for (;itIn->hasNext();) {
      cloneIt.push_back(itIn->next());
    }delete itIn;
    itStl=cloneIt.begin();
  }
  ~StableIterator(){};
  itType next() {
    itType tmp=*itStl;
    ++itStl;
    return tmp;
  }
  bool hasNext() {
    return (itStl!=cloneIt.end());
  };

  protected :
  std::vector<itType> cloneIt;
  typename std::vector<itType>::iterator itStl;
};

}
#endif
