/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN


#ifndef Tulip_FACE_H
#define Tulip_FACE_H
#include <tulip/tulipconf.h>
#include <tulip/tuliphash.h>

#include <climits>

namespace tlp {

struct Face {
  unsigned int id;
  Face():id(UINT_MAX) {}
  explicit Face(unsigned int j):id(j) {}
  bool operator!=(const Face f) const {
    return id!=f.id;
  }
  bool operator==(const Face f) const {
    return id==f.id;
  }
  bool isValid() const {
    return id!=UINT_MAX;
  }
};

}

#ifndef DOXYGEN_NOTFOR_DEVEL

TLP_BEGIN_HASH_NAMESPACE {
  template<> struct hash<tlp::Face> {
    size_t operator()(const tlp::Face f) const {return f.id;}
  };
}
TLP_END_HASH_NAMESPACE

namespace std {
template<> struct equal_to<tlp::Face> {
  size_t operator()(const tlp::Face f,const tlp::Face f2) const {
    return f.id == f2.id;
  }
};
template<> struct less<tlp::Face> {
  size_t operator()(const tlp::Face f,const tlp::Face f2) const {
    return f.id < f2.id;
  }
};
}
#endif // DOXYGEN_NOTFOR_DEVEL

#endif
///@endcond
