#include <assert.h>
#include <tulip/Iterator.h>

namespace tlp {
  template<typename TYPE>
struct _TLP_IT {
  _TLP_IT(TYPE &n, Iterator<TYPE> *it ) : it(it), n(n) {
  }
  Iterator<TYPE> *it;
  TYPE &n;
};

template <typename TYPE>
inline void * _tlp_get_it(TYPE &n, Iterator<TYPE> *it) {
  return (void *)new _TLP_IT<TYPE>(n, it);
}

template<typename TYPE>
inline bool _tlp_if_test(TYPE &n, void *_it) {
  _TLP_IT<TYPE> &cit = *(_TLP_IT<TYPE>*)_it;
  assert(cit.it !=0);
  if(cit.it->hasNext()) {
    cit.n = cit.it->next(); 
    return true;
  } 
  else {
    delete cit.it;
    cit.it = 0;
    return false;
  }
}

}
#define forEach(A, B) \
  for(void *it=tlp::_tlp_get_it(A, B);tlp::_tlp_if_test(A,it);)
