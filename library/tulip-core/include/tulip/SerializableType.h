#ifndef SERIALIZABLETYPE_H
#define SERIALIZABLETYPE_H

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
#include <tulip/TypeInterface.h>
#include <tulip/SerializableType.h>
#include <tulip/TypeInterface.h>

#define FORWARD_TOSTRING(T) static std::string toString(const T::RealType &v) { std::ostringstream oss;  write(oss, v); return oss.str(); }
#define FORWARD_FROMSTRING(T) static bool fromString(T::RealType &v, const std::string &s) { std::istringstream iss(s); return read(iss, v); }
#define FORWARD_STRING_METHODS(T) FORWARD_FROMSTRING(T) FORWARD_TOSTRING(T)

namespace tlp {
template<typename T>
class TLP_SCOPE SerializableType: public TypeInterface<T> {
public:
  static void write(std::ostream& oss, const typename TypeInterface<T>::RealType& v) {
    oss << v;
  }
  static bool read(std::istream& iss, typename TypeInterface<T>::RealType& v) {
    return (iss >> v);
  }
  FORWARD_STRING_METHODS(typename TypeInterface<T>)
};

template<typename VT, int openParen>
class TLP_SCOPE SerializableVectorType: public TypeInterface<std::vector<VT> > {
  static bool readVector(std::istream& is, std::vector<VT>& v) {
    v.clear();

    char c =' ';
    VT val;
    bool firstVal = true;
    bool sepFound = false;

    // go to first '('
    while((is >> c) && isspace(c)) {}

    if (c != '(')
      return false;

    for(;;) {
      if( !(is >> c) )
        return false;

      if (isspace(c))
        continue;

      if(c == ')') {
        if (sepFound)
          return false;

        return true;
      }

      if (c == ',') {
        if (firstVal || sepFound)
          return false;

        sepFound = true;
      }
      else {
        if (firstVal || sepFound) {
          if (openParen && c != '(')
            return false;

          is.unget();

          if( !(is >> val) )
            return false;

          v.push_back(val);
          firstVal = false;
          sepFound = false;
        }
        else
          return false;
      }
    }
  }
  static void writeVector(std::ostream& os, const std::vector<VT>& v) {
    os << '(';

    for( unsigned int i = 0 ; i < v.size() ; i++ ) {
      if (i)
        os << ", ";

      os << v[i];
    }

    os << ')';
  }

public:
  static void write(std::ostream& oss, const typename TypeInterface<std::vector<VT> >::RealType& v) {
    writeVector(oss, v);
  }
  static bool read(std::istream& iss, typename TypeInterface<std::vector<VT> >::RealType& v) {
    return readVector(iss, v);
  }
  FORWARD_STRING_METHODS(typename TypeInterface<std::vector<VT> >)
};
}

#endif // SERIALIZABLETYPE_H
