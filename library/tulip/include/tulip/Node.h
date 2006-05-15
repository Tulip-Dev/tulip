#ifndef Tulip_NODE_H
#define Tulip_NODE_H
#include "tulipconf.h"
#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include <climits>

/**
 * \defgroup sgs
 */ 
/*@{*/
/// class node
namespace tlp {

struct node { 
  unsigned int id;
  node():id(UINT_MAX){}
  explicit node(unsigned int j):id(j){}
  bool operator!=(const node n) const {return id!=n.id;}
  bool operator==(const node n) const {return id==n.id;}
  bool isValid() const {return id!=UINT_MAX;}
};

}
/*@}*/

#ifndef DOXYGEN_NOTFOR_DEVEL

namespace stdext {
  template<> struct hash<tlp::node> {
    size_t operator()(const tlp::node n) const {return n.id;}
  };
}

namespace std {
  template<> struct equal_to<tlp::node> {
    size_t operator()(const tlp::node n,const tlp::node n2) const {return n.id==n2.id;}
  };
  template<> struct less<tlp::node>{
    size_t operator()(const tlp::node n,const tlp::node n2) const {return n.id<n2.id;}
  };
}
#endif // DOXYGEN_NOTFOR_DEVEL

#endif
