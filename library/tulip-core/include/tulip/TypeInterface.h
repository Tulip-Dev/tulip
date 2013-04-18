/*
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
///@cond DOXYGEN_HIDDEN


#ifndef TYPEINTERFACE_H
#define TYPEINTERFACE_H

#include <set>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <float.h>
#include <tulip/StoredType.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Color.h>
#include <tulip/StringCollection.h>
#include <tulip/Edge.h>
#include <tulip/AbstractProperty.h>

namespace tlp {
template<typename T>
class TypeInterface {
public:
  typedef T RealType;
  static RealType undefinedValue() {
    return T();
  }
  static RealType defaultValue() {
    return T();
  }

  static void write(std::ostream&, const RealType&) {}
  static bool read(std::istream&, RealType&) {
    return false;
  }

  static std::string toString(const RealType &) {
    return "";
  }
  static bool fromString(RealType &, const std::string &) {
    return false;
  }
};
}

#endif // TYPEINTERFACE_H
///@endcond
