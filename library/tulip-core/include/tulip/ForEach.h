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
#ifndef Tulip_FOREACH_H
#define Tulip_FOREACH_H

#include <assert.h>
#include <tulip/Iterator.h>
#include <tulip/StableIterator.h>
#include <tulip/tulipconf.h>

#ifndef DOXYGEN_NOTFOR_DEVEL
namespace tlp {
  /**
  * @brief Encapsulation of a Tulip Iterator intended to be allocated on the stack instead of the heap,
  * so it gets deleted when out of scope.
  * 
  **/
  template<typename TYPE>
  struct _TLP_IT {
    _TLP_IT(Iterator<TYPE> *_it) : _it(_it) {
    }
    ~_TLP_IT() {
      delete _it;
    }
    Iterator<TYPE> *_it;
  };

  /**
  * @brief 
  **/
  template<typename TYPE>
  inline bool _tlp_if_test(TYPE &n, _TLP_IT<TYPE> &_it) {
    assert(_it._it != NULL);
    if(_it._it->hasNext()) {
      n = _it._it->next();
      return true;
    } 
    else {
      return false;
    }
  }
}
#endif //DOXYGEN_NOTFOR_DEVEL

/**
 * @brief Allows to iterate on the nodes or edges of a Graph in a clear and concise way.
 * It also avoid having to manage a tulip Iterator, whose deletion is often forgotten.
 */
#define forEach(A, B) \
for(tlp::_TLP_IT<TYPEOF(A) > _it_foreach(B); tlp::_tlp_if_test(A, _it_foreach);)

/**
 * @brief Allows to iterate on the nodes or edges of a copy of a Graph in a clear and concise way.
 * The stable Iterator creates a copy of the Graph, and iterates on the said copy.
 * It allows deletion operations to be performed without invalidating the iterator.
 * It also avoid having to manage a tulip Iterator, whose deletion is often forgotten.
 */
#define stableForEach(A, B)  \
  for(tlp::_TLP_IT<TYPEOF(A) > _it_foreach(new StableIterator<TYPEOF(A) >(B));  tlp::_tlp_if_test(A, _it_foreach);)

#endif
