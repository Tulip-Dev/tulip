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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <set>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <float.h>
#include <tulip/StoredType.h>
#include "tulip/Coord.h"
#include "tulip/Size.h"
#include "tulip/Color.h"
#include "tulip/StringCollection.h"
#include "tulip/Edge.h"

namespace tlp {

class Graph;

/**
 * \defgroup types
 */ 
/*@{*/

///
#ifndef SWIG
 DECL_STORED_STRUCT(std::set<tlp::node>);
#endif

class TLP_SCOPE GraphType {
public:
  typedef Graph* RealType;
  static GraphType::RealType undefinedValue();
  static GraphType::RealType defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString(RealType & v, const std::string & s );
};

#ifndef SWIG
 DECL_STORED_STRUCT(std::set<tlp::edge>);
#endif

class TLP_SCOPE EdgeSetType
{
public:
  typedef std::set<edge> RealType;
  static std::set<edge> undefinedValue();
  static std::set<edge> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

class TLP_SCOPE DoubleType {
public:
  typedef double RealType;
  static double undefinedValue();
  static double defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

#ifndef SWIG
DECL_STORED_STRUCT(std::vector<double>);
#endif

class TLP_SCOPE DoubleVectorType {
public:
  typedef std::vector<double> RealType;
  static std::vector<double> undefinedValue();
  static std::vector<double> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

class TLP_SCOPE IntegerType
{
public:
  typedef int RealType;
  static int undefinedValue();
  static int defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

#ifndef SWIG
DECL_STORED_STRUCT(std::vector<int>);
#endif

class TLP_SCOPE IntegerVectorType
{
public:
  typedef std::vector<int> RealType;
  static std::vector<int> undefinedValue();
  static std::vector<int> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

class TLP_SCOPE BooleanType {
public:
  typedef bool RealType;
  static bool undefinedValue();
  static bool defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};


#ifndef SWIG
DECL_STORED_STRUCT(std::vector<bool>);
#endif

class TLP_SCOPE BooleanVectorType {
public:
  typedef std::vector<bool> RealType;
  static std::vector<bool> undefinedValue();
  static std::vector<bool> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

#ifndef SWIG
 DECL_STORED_STRUCT(std::vector<tlp::Coord>);
#endif

class TLP_SCOPE LineType
{
public:
  typedef std::vector<Coord> RealType;
  static std::vector<Coord> undefinedValue();
  static std::vector<Coord> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

 DECL_STORED_STRUCT(tlp::Coord);

class TLP_SCOPE PointType {
public:
  typedef Coord RealType;
  static Coord undefinedValue();
  static Coord defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

#ifndef SWIG
 DECL_STORED_STRUCT(tlp::Size);
#endif

class TLP_SCOPE SizeType {
public:
  typedef Size RealType;
  static Size undefinedValue();
  static Size defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

#ifndef SWIG
 DECL_STORED_STRUCT(std::vector<tlp::Size>);
#endif

class TLP_SCOPE SizeVectorType {
public:
  typedef std::vector<Size> RealType;
  static std::vector<Size> undefinedValue();
  static std::vector<Size> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

#ifndef SWIG
DECL_STORED_STRUCT(std::string);
#endif

class TLP_SCOPE StringType {
public:
  typedef std::string RealType;
  static std::string undefinedValue();
  static std::string defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

#ifndef SWIG
DECL_STORED_STRUCT(std::vector<std::string>);
#endif

class TLP_SCOPE StringVectorType {
public:
  typedef std::vector<std::string> RealType;
  static std::vector<std::string> undefinedValue();
  static std::vector<std::string> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

class TLP_SCOPE ColorType {
public:
  typedef Color RealType;
  static Color undefinedValue();
  static Color defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

#ifndef SWIG
 DECL_STORED_STRUCT(std::vector<tlp::Color>);
#endif

class TLP_SCOPE ColorVectorType {
public:
  typedef std::vector<Color> RealType;
  static std::vector<Color> undefinedValue();
  static std::vector<Color> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

class TLP_SCOPE CoordVectorType {
public:
  typedef std::vector<Coord> RealType;
  static std::vector<Coord> undefinedValue();
  static std::vector<Coord> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
  static void write(std::ostream&, const RealType &v);
  static bool read(std::istream& istr, RealType& value);
};

extern TLP_SCOPE void initTypeSerializers();
}
/*@}*/
#endif
