#include <assert.h>
#include <tulip/Iterator.h>

namespace tlp {
  template<typename TYPE>
  struct _TLP_IT {
    _TLP_IT(TYPE &_n, Iterator<TYPE> *_it ) : _it(_it), _n(_n) {
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
    _TLP_IT<TYPE> &_cit = *(_TLP_IT<TYPE>*)_it;
    assert(_cit._it !=0);
    if(_cit._it->hasNext()) {
      _cit._n = _cit._it->next(); 
      return true;
    } 
    else {
      delete _cit._it;
      _cit._it = 0;
      return false;
    }
  }
}

#define forEach(A, B) \
  for(void *_it_foreach = tlp::_tlp_get_it(A, B); tlp::_tlp_if_test(A, _it_foreach);)
