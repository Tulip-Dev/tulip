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

#ifndef TYPES_H
#define TYPES_H

#include <set>
#include <vector>
//#include <string>
//#include <list>
//#include <iostream>
#include <sstream>
//#include <float.h>
#include <tulip/StoredType.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Color.h>
//#include <tulip/StringCollection.h>
#include <tulip/Edge.h>
#include <tulip/Node.h>
#include <tulip/AbstractProperty.h>
#include <tulip/TypeInterface.h>
#include <tulip/SerializableType.h>

namespace tlp {

class Graph;

class TLP_SCOPE GraphType: public TypeInterface<tlp::Graph*> {
public:
  static RealType undefinedValue();
  static RealType defaultValue();
  static void write(std::ostream &oss, const RealType &v);
  static void writeb(std::ostream &oss, const RealType &v);
  static bool read(std::istream& iss, RealType& v);
  static bool readb(std::istream& iss, RealType& v);
  FORWARD_STRING_METHODS(GraphType)
};

class TLP_SCOPE EdgeSetType: public TypeInterface<std::set<tlp::edge> > {
public:
  static void write(std::ostream &oss, const RealType &v);
  static void writeb(std::ostream &oss, const RealType &v);
  static bool read(std::istream& iss, RealType& v);
  static bool readb(std::istream& iss, RealType& v);
  FORWARD_STRING_METHODS(EdgeSetType)
};

class TLP_SCOPE DoubleType: public TypeInterface<double> {
public:
  static double undefinedValue();
  static double defaultValue();
  static void write(std::ostream &oss, const RealType &v) {
    oss << v;
  }
  static bool read(std::istream& iss, RealType& v);
  FORWARD_STRING_METHODS(DoubleType)
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
typedef SerializableVectorType<unsigned int,false> UnsignedIntegerVectorType;

class TLP_SCOPE BooleanType: public TypeInterface<bool> {
public:
  static RealType undefinedValue();
  static RealType defaultValue();
  static void write(std::ostream&, const RealType&);
  static bool read(std::istream&, RealType&, bool untilEnd = false);
  static bool fromString(RealType &, const std::string &, bool untilEnd = false);
  FORWARD_TOSTRING(BooleanType)
};

class TLP_SCOPE BooleanVectorType: public TypeInterface<std::vector<bool> > {
public:
  static void write(std::ostream&, const RealType&);
  static void writeb(std::ostream&, const RealType&);
  static bool read(std::istream&, RealType&, char openChar = '(',
                   char sepChar = ',', char closeChar = ')');
  static bool readb(std::istream&, RealType&);
  FORWARD_STRING_METHODS(BooleanVectorType)
};

class TLP_SCOPE LineType: public SerializableVectorType<tlp::Coord,false> {
public:
  static bool read(std::istream&, RealType&, char openChar = '(',
                   char sepChar = ',', char closeChar = ')');
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

  static void write(std::ostream&, const RealType&, char openCloseChar = '"');
  static void writeb(std::ostream&, const RealType&);
  static bool read(std::istream&, RealType&,
                   char openChar = '"', char closeChar = '"');
  static bool readb(std::istream&, RealType&);

  static std::string toString(const RealType &v);
  static bool fromString(RealType &v, const std::string &s);
};

class TLP_SCOPE StringVectorType: public TypeInterface<std::vector<std::string> > {
public:
  static void write(std::ostream&, const RealType&);
  static void writeb(std::ostream& oss, const RealType& vStr);
  static bool read(std::istream&, RealType&, char openChar = '(',
                   char sepchar = ',', char closeChar = ')');
  static bool readb(std::istream& iss, RealType& vStr);
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
//DECL_STORED_STRUCT(tlp::SizeType::RealType) <=> PointType::RealType
//DECL_STORED_STRUCT(tlp::SizeVectorType::RealType) <=> LineType::RealType
DECL_STORED_STRUCT(tlp::StringType::RealType)
DECL_STORED_STRUCT(tlp::StringVectorType::RealType)
DECL_STORED_STRUCT(tlp::ColorVectorType::RealType)

// template class to automatize definition of serializers
template<typename T>
struct KnownTypeSerializer :public TypedDataSerializer<typename T::RealType> {
  KnownTypeSerializer(const std::string& otn):TypedDataSerializer<typename T::RealType>(otn) {}
  KnownTypeSerializer(const char* otn):TypedDataSerializer<typename T::RealType>(std::string(otn)) {}

  DataTypeSerializer* clone() const {
    return new KnownTypeSerializer<T>(this->outputTypeName);
  }

  void write(std::ostream& os, const typename T::RealType& v) {
    T::write(os, v);
  }
  bool read(std::istream& iss, typename T::RealType& v) {
    return T::read(iss, v);
  }
  bool setData(tlp::DataSet& ds, const std::string& prop, const std::string& value) {
    bool result = true;
    typename T::RealType val;

    if (value.empty())
      val = T::defaultValue();
    else
      result = T::fromString(val, value);

    ds.set(prop, val);
    return result;
  }
};

extern void initTypeSerializers();
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
template class TypeInterface<std::set<tlp::edge> >;
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


#endif
///@endcond
