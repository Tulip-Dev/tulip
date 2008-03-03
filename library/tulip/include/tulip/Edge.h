#ifndef Tulip_EDGE_H
#define Tulip_EDGE_H
#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include <climits>
/**
 * \addtogroup graphs
 */ 
namespace tlp {

/*@{*/
/// class edge
struct edge { 
  unsigned int id;
  edge():id(UINT_MAX){}
  explicit edge(unsigned int j):id(j){}
  bool operator==(const edge e) const{return id==e.id;}
  bool operator!=(const edge e) const{return id!=e.id;}
  bool isValid() const {return id!=UINT_MAX;}
};
/*@}*/
}

#ifndef DOXYGEN_NOTFOR_DEVEL

namespace stdext {
  template<>
  struct hash<tlp::edge>{
    size_t operator()(const tlp::edge e) const {return e.id;}
  };
}

namespace std {
  template<>
  struct equal_to<tlp::edge>{size_t operator()(const tlp::edge e,const tlp::edge e2) const {
    return e.id==e2.id;}
  };
  template<>
  struct less<tlp::edge>{size_t operator()(const tlp::edge e,const tlp::edge e2) const {
    return e.id<e2.id;}
  };
}
#endif
#endif
