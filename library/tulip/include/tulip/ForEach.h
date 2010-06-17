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
#ifndef Tulip_FOREACH_H
#define Tulip_FOREACH_H

#include <assert.h>
#include <tulip/Iterator.h>
#include <tulip/StableIterator.h>

#ifndef DOXYGEN_NOTFOR_DEVEL
namespace tlp {
  template<typename TYPE>
  struct _TLP_IT {
    _TLP_IT(TYPE &_n, Iterator<TYPE> *_it, void (*_d) (void *)) : _d(_d), _it(_it), _n(_n) {
    }
    ~_TLP_IT() {
      delete _it;
    }
    void (*_d) (void *);
    Iterator<TYPE> *_it;
    TYPE &_n;
  };

  template <typename TYPE>
  static void _tlp_delete_it(void *_it) {
    delete (_TLP_IT<TYPE>*)_it;
  }

  template <typename TYPE>
  inline void * _tlp_get_it(TYPE &n, Iterator<TYPE> *_it) {
    return (void *)new _TLP_IT<TYPE>(n, _it, (void (*) (void *)) &_tlp_delete_it<TYPE>);
  }

  template <typename TYPE>
  inline void * _tlp_get_stable_it(TYPE &n, Iterator<TYPE> *_it) {
    return (void *)new _TLP_IT<TYPE>(n, new StableIterator<TYPE>(_it),
				     (void (*) (void *)) &_tlp_delete_it<TYPE>);
  }

  template<typename TYPE>
  inline bool _tlp_if_test(TYPE &, void *_it) {
    assert(((_TLP_IT<TYPE>*)_it)->_it !=0);
    if(((_TLP_IT<TYPE>*)_it)->_it->hasNext()) {
      ((_TLP_IT<TYPE>*)_it)->_n = ((_TLP_IT<TYPE>*)_it)->_it->next(); 
      return true;
    } 
    else {
      delete (_TLP_IT<TYPE>*)_it;
      return false;
    }
  }
}
#endif //DOXYGEN_NOTFOR_DEVEL


/**
 * Warning, do not use break or return inside a for each block;
 * it causes a memory leak; use breakForEach pr returnForEachInstead
 */
#define forEach(A, B) \
  for(void *_it_foreach = tlp::_tlp_get_it(A, B); tlp::_tlp_if_test(A, _it_foreach);)

#define stableForEach(A, B)  \
  for(void *_it_foreach = tlp::_tlp_get_stable_it(A, B); tlp::_tlp_if_test(A, _it_foreach);)

#define _delete_it_foreach ((**((void (**) (void *)) _it_foreach))(_it_foreach))

#define breakForEach {_delete_it_foreach; break;}

#define returnForEach(VAL) {_delete_it_foreach; return VAL;}

#endif
