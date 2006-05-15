//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 10/01/2006
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_FOREACH_H
#define Tulip_FOREACH_H

#include <assert.h>
#include <tulip/Iterator.h>

namespace tlp {

  template<typename TYPE>
  struct _TLP_IT {
    _TLP_IT(TYPE &_n, Iterator<TYPE> *_it ) : _it(_it), _n(_n) {
    }
    ~_TLP_IT() {
      delete _it;
    }
    Iterator<TYPE> *_it;
    TYPE &_n;
  };

  template <typename TYPE>
  inline void * _tlp_get_it(TYPE &n, Iterator<TYPE> *_it) {
    return (void *)new _TLP_IT<TYPE>(n, _it);
  }

  template<typename TYPE>
  inline bool _tlp_if_test(TYPE &n, void *_it) {
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


/**
 * Warning, do not use break or return inside a for each block;
 * it causes a memory leak.
 */
#define forEach(A, B) \
  for(void *_it_foreach = tlp::_tlp_get_it(A, B); tlp::_tlp_if_test(A, _it_foreach);)

#endif
