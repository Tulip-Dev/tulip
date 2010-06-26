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
#ifndef Tulip_NODE_H
#define Tulip_NODE_H
#include "tulip/tulipconf.h"
#include "tulip/tuliphash.h"
#include <climits>

/**
 * \addtogroup graphs
 */ 
namespace tlp {

/*@{*/
/// class node
struct node { 
  unsigned int id;
  node():id(UINT_MAX){}
  explicit node(unsigned int j):id(j){}
  bool operator!=(const node n) const {return id!=n.id;}
  bool operator==(const node n) const {return id==n.id;}
  bool isValid() const {return id!=UINT_MAX;}
};
/*@}*/
}

#ifndef DOXYGEN_NOTFOR_DEVEL

TLP_BEGIN_HASH_NAMESPACE {
  template<> struct hash<tlp::node> {
    size_t operator()(const tlp::node n) const {return n.id;}
  };
} TLP_END_HASH_NAMESPACE


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
