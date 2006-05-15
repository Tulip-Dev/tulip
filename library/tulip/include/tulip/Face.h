//-*-c++-*-
/*
 Author: Romain BOURQUI
 Email : bourqui@labri.fr
 Last Version : 10/01/2006
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_FACE_H
#define Tulip_FACE_H
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
/// class face
namespace tlp {

struct Face { 
  unsigned int id;
  Face():id(UINT_MAX){}
  explicit Face(unsigned int j):id(j){}
  bool operator!=(const Face f) const {return id!=f.id;}
  bool operator==(const Face f) const {return id==f.id;}
  bool isValid() const {return id!=UINT_MAX;}
};
}
/*@}*/

#ifndef DOXYGEN_NOTFOR_DEVEL

namespace stdext {
  template<> struct hash<tlp::Face> {
    size_t operator()(const tlp::Face f) const {return f.id;}
  };
}

namespace std {
  template<> struct equal_to<tlp::Face> {
    size_t operator()(const tlp::Face f,const tlp::Face f2) const {return f.id == f2.id;}
  };
  template<> struct less<tlp::Face>{
    size_t operator()(const tlp::Face f,const tlp::Face f2) const {return f.id < f2.id;}
  };
}
#endif // DOXYGEN_NOTFOR_DEVEL

#endif
