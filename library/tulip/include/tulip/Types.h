//-*-c++-*-
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
class Type {
public:
  Type(){};
  ~Type();
};

class TLP_SCOPE GraphType:public Type {
public:
  typedef Graph* RealType;
  static GraphType::RealType undefinedValue();
  static GraphType::RealType defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString(RealType & v, const std::string & s );
};

class TLP_SCOPE EdgeSetType:public Type
{
public:
  typedef std::set<edge> RealType;
  static std::set<edge> undefinedValue();
  static std::set<edge> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE DoubleType:public Type {
public:
  typedef double RealType;
  static double undefinedValue();
  static double defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE DoubleVectorType:public Type {
public:
  typedef std::vector<double> RealType;
  static std::vector<double> undefinedValue();
  static std::vector<double> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE IntegerType:public Type
{
public:
  typedef int RealType;
  static int undefinedValue();
  static int defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE IntegerVectorType:public Type
{
public:
  typedef std::vector<int> RealType;
  static std::vector<int> undefinedValue();
  static std::vector<int> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE BooleanType:public Type {
public:
  typedef bool RealType;
  static bool undefinedValue();
  static bool defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE BooleanVectorType:public Type {
public:
  typedef std::vector<bool> RealType;
  static std::vector<bool> undefinedValue();
  static std::vector<bool> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE LineType:public Type
{
public:
  typedef std::vector<Coord> RealType;
  static std::vector<Coord> undefinedValue();
  static std::vector<Coord> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE PointType:public Type {
public:
  typedef Coord RealType;
  static Coord undefinedValue();
  static Coord defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE SizeType:public Type {
public:
  typedef Size RealType;
  static Size undefinedValue();
  static Size defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE SizeVectorType:public Type {
public:
  typedef std::vector<Size> RealType;
  static std::vector<Size> undefinedValue();
  static std::vector<Size> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE StringType:public Type {
public:
  typedef std::string RealType;
  static std::string undefinedValue();
  static std::string defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE StringVectorType:public Type {
public:
  typedef std::vector<std::string> RealType;
  static std::vector<std::string> undefinedValue();
  static std::vector<std::string> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE ColorType:public Type {
public:
  typedef Color RealType;
  static Color undefinedValue();
  static Color defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE ColorVectorType:public Type {
public:
  typedef std::vector<Color> RealType;
  static std::vector<Color> undefinedValue();
  static std::vector<Color> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE CoordVectorType:public Type {
public:
  typedef std::vector<Coord> RealType;
  static std::vector<Coord> undefinedValue();
  static std::vector<Coord> defaultValue();
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};
}
/*@}*/
#endif






