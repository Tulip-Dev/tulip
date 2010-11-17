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
#ifndef Tulip_EDGE_H
#define Tulip_EDGE_H
#include <climits>
#include "tulip/tuliphash.h"
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
  operator unsigned int() {
    return id;
  }
  bool operator==(const edge e) const{return id==e.id;}
  bool operator!=(const edge e) const{return id!=e.id;}
  bool isValid() const {return id!=UINT_MAX;}
};
/*@}*/
}

#ifndef DOXYGEN_NOTFOR_DEVEL

TLP_BEGIN_HASH_NAMESPACE {
  template<>
  struct hash<tlp::edge>{
    size_t operator()(const tlp::edge e) const {return e.id;}
  };
} TLP_END_HASH_NAMESPACE

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
