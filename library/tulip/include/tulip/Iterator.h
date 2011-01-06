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
#ifndef TULIP_ITERATOR_H
#define TULIP_ITERATOR_H

#include <tulip/tulipconf.h>

namespace tlp {
  
#ifndef DOXYGEN_NOTFOR_DEVEL
extern TLP_SCOPE void incrNumIterators();
extern TLP_SCOPE void decrNumIterators();
extern TLP_SCOPE int getNumIterators();
#endif // DOXYGEN_NOTFOR_DEVEL

///Interface of Iterators
template<class itType> struct Iterator {
  ///
  Iterator(){
#ifndef NDEBUG
    incrNumIterators();
#endif
}
  ///
  virtual ~Iterator() {
#ifndef NDEBUG
    decrNumIterators();
#endif
  }
  ///Return the next element
  virtual itType next()=0;
  ///Return true if it exist a next element
  virtual bool hasNext()=0;
};

//template<class C>class Iterator;
#ifndef DOXYGEN_NOTFOR_DEVEL
template<typename TYPE> class UINTIterator : public Iterator<TYPE> {
public:
  UINTIterator(Iterator<unsigned int> *it):it(it) {
  }
  ~UINTIterator(){
    delete it;
  }
  bool hasNext() {
    return it->hasNext();
  }
  TYPE next() {
    return TYPE(it->next());
  }
private:
  Iterator<unsigned int> *it;
};
#endif // DOXYGEN_NOTFOR_DEVEL

}
#endif

