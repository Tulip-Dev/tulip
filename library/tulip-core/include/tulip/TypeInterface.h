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


#ifndef TYPEINTERFACE_H
#define TYPEINTERFACE_H

#include <set>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <sstream>

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

  static unsigned int valueSize() {
    return sizeof(T);
  }

  static void write(std::ostream&, const RealType&) {}
  static void writeb(std::ostream& oss, const RealType& v) {
    oss.write((char *) &v, sizeof(v));
  }

  static bool read(std::istream&, RealType&) {
    return false;
  }
  static bool readb(std::istream& iss, RealType& v) {
    return bool(iss.read((char *) &v, sizeof(v)));
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
