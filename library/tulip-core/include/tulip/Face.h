/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <climits>
#include <functional>

namespace tlp {

struct Face {
  unsigned int id;
  Face() : id(UINT_MAX) {}
  explicit Face(unsigned int j) : id(j) {}
  bool operator!=(const Face f) const {
    return id != f.id;
  }
  bool operator==(const Face f) const {
    return id == f.id;
  }
  bool isValid() const {
    return id != UINT_MAX;
  }
};
} // namespace tlp

namespace std {
template <>
struct hash<tlp::Face> {
  size_t operator()(const tlp::Face f) const {
    return f.id;
  }
};
template <>
struct equal_to<tlp::Face> {
  size_t operator()(const tlp::Face f, const tlp::Face f2) const {
    return f.id == f2.id;
  }
};
template <>
struct less<tlp::Face> {
  size_t operator()(const tlp::Face f, const tlp::Face f2) const {
    return f.id < f2.id;
  }
};
} // namespace std

#endif
///@endcond
