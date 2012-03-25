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
class TLP_SCOPE TypeInterface {
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
