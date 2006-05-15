//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 10/12/2002
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef TULIP_STABLEITERATOR_H
#define TULIP_STABLEITERATOR_H
#include <tulip/Iterator.h>
#include <tulip/tulipconf.h>
#include <vector>

namespace tlp {

///Interface of StableIterator,
template<class itType> struct TLP_SCOPE StableIterator : public Iterator<itType> {
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
  itType element;
};

}
#endif
