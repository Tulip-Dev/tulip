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
#ifndef TYPES_H
#define TYPES_H

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

class Graph;

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

#define FORWARD_TOSTRING(T) static std::string toString(const T::RealType &v) { std::ostringstream oss;  write(oss, v); return oss.str(); }
#define FORWARD_FROMSTRING(T) static bool fromString(T::RealType &v, const std::string &s) { std::istringstream iss(s); return read(iss, v); }
#define FORWARD_STRING_METHODS(T) FORWARD_FROMSTRING(T) FORWARD_TOSTRING(T)

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

class TLP_SCOPE GraphType: public TypeInterface<tlp::Graph*> {
public:
  static RealType undefinedValue();
  static RealType defaultValue();
  static void write(std::ostream &oss, const RealType &v);
  static bool read(std::istream& iss, RealType& v);
  FORWARD_STRING_METHODS(GraphType)
};

class TLP_SCOPE EdgeSetType: public TypeInterface<std::set<edge> > {
public:
  static void write(std::ostream &oss, const RealType &v);
  static bool read(std::istream& iss, RealType& v);
  FORWARD_STRING_METHODS(EdgeSetType)
};

class TLP_SCOPE DoubleType: public SerializableType<double> {
public:
  static double undefinedValue();
  static double defaultValue();
};

class TLP_SCOPE FloatType: public SerializableType<float> {
public:
  static float undefinedValue();
  static float defaultValue();
};

typedef SerializableVectorType<double,false> DoubleVectorType;

class TLP_SCOPE IntegerType: public SerializableType<int> {
public:
  static int undefinedValue();
  static int defaultValue();
};

class TLP_SCOPE LongType: public SerializableType<long> {
public:
  static long undefinedValue();
  static long defaultValue();
};

class TLP_SCOPE UnsignedIntegerType: public SerializableType<unsigned int> {
public:
  static unsigned int undefinedValue();
  static unsigned int defaultValue();
};

typedef SerializableVectorType<int,false> IntegerVectorType;


class TLP_SCOPE BooleanType: public TypeInterface<bool> {
public:
  static RealType undefinedValue();
  static RealType defaultValue();
  static void write(std::ostream&, const RealType&);
  static bool read(std::istream&, RealType&);
  FORWARD_STRING_METHODS(BooleanType)
};

class TLP_SCOPE BooleanVectorType: public TypeInterface<std::vector<bool> > {
public:
  static void write(std::ostream&, const RealType&);
  static bool read(std::istream&, RealType&);
  FORWARD_STRING_METHODS(BooleanVectorType)
};

class TLP_SCOPE LineType: public SerializableVectorType<tlp::Coord,false> {
public:
  static bool read(std::istream&, RealType&);
  FORWARD_STRING_METHODS(LineType)
};

class TLP_SCOPE PointType: public SerializableType<tlp::Coord> {
public:
  static RealType undefinedValue();
  static RealType defaultValue();
  static bool read(std::istream&, RealType&);
  static bool fromString(RealType &, const std::string &);
  FORWARD_TOSTRING(PointType)
};

class TLP_SCOPE SizeType: public SerializableType<tlp::Size> {
public:
  static RealType undefinedValue();
  static RealType defaultValue();
  static bool read(std::istream&, RealType&);
  static bool fromString(RealType &, const std::string &);
  FORWARD_TOSTRING(SizeType)
};

typedef SerializableVectorType<tlp::Size,true> SizeVectorType;

class TLP_SCOPE StringType: public TypeInterface<std::string> {
public:
  static RealType undefinedValue();
  static RealType defaultValue();

  static void write(std::ostream&, const RealType&);
  static bool read(std::istream&, RealType&);

  static std::string toString(const RealType &v);
  static bool fromString(RealType &v, const std::string &s);
};

class TLP_SCOPE StringVectorType: public TypeInterface<std::vector<std::string> > {
public:
  static void write(std::ostream&, const RealType&);
  static bool read(std::istream&, RealType&);
  FORWARD_STRING_METHODS(StringVectorType)
};

class TLP_SCOPE ColorType: public TypeInterface<tlp::Color> {
public:
  static RealType undefinedValue();

  static void write(std::ostream&, const RealType&);
  static bool read(std::istream&, RealType&);

  static std::string toString(const RealType &v);
  static bool fromString(RealType &v, const std::string &s);
};

typedef SerializableVectorType<tlp::Color,true> ColorVectorType;
typedef SerializableVectorType<tlp::Coord,true> CoordVectorType;

DECL_STORED_STRUCT(tlp::EdgeSetType::RealType)
DECL_STORED_STRUCT(std::set<tlp::node>)
DECL_STORED_STRUCT(tlp::DoubleVectorType::RealType)
DECL_STORED_STRUCT(tlp::IntegerVectorType::RealType)
DECL_STORED_STRUCT(tlp::BooleanVectorType::RealType)
DECL_STORED_STRUCT(tlp::LineType::RealType)
DECL_STORED_STRUCT(tlp::PointType::RealType)
DECL_STORED_STRUCT(tlp::SizeType::RealType)
DECL_STORED_STRUCT(tlp::SizeVectorType::RealType)
DECL_STORED_STRUCT(tlp::StringType::RealType)
DECL_STORED_STRUCT(tlp::StringVectorType::RealType)
DECL_STORED_STRUCT(tlp::ColorVectorType::RealType)

extern TLP_SCOPE void initTypeSerializers();
#ifdef _MSC_VER
template class SerializableType<double>;
template class SerializableType<float>;
template class SerializableType<int>;
template class SerializableType<long>;
template class SerializableType<unsigned int>;
template class SerializableType<tlp::Coord>;
template class SerializableType<tlp::Size>;
template class SerializableVectorType<double,false>;
template class SerializableVectorType<int,false>;
template class SerializableVectorType<tlp::Coord,false>;
template class SerializableVectorType<tlp::Size,true>;
template class SerializableVectorType<tlp::Color,true>;
template class SerializableVectorType<tlp::Coord,true>;
template class TypeInterface<tlp::Graph*>;
template class TypeInterface<std::set<edge> >;
template class TypeInterface<bool>;
template class TypeInterface<std::vector<bool> >;
template class TypeInterface<std::vector<std::string> >;
template class TypeInterface<tlp::Color>;
template class TypeInterface<std::string>;

template class AbstractVectorProperty<DoubleVectorType, tlp::DoubleType>;
template class AbstractVectorProperty<IntegerVectorType, tlp::IntegerType>;
template class AbstractVectorProperty<BooleanVectorType, tlp::BooleanType>;
template class AbstractVectorProperty<StringVectorType, tlp::StringType>;
template class AbstractVectorProperty<ColorVectorType, tlp::ColorType>;
template class AbstractVectorProperty<SizeVectorType, tlp::SizeType>;
template class AbstractVectorProperty<CoordVectorType, tlp::PointType>;
#endif
}

/*@}*/
#endif
