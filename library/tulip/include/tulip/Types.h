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
  static void  del(GraphType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString(RealType & v, const std::string & s );
};

class TLP_SCOPE EdgeSetType:public Type
{
public:
  typedef std::set<edge> RealType;
  static std::set<edge> undefinedValue();
  static std::set<edge> defaultValue();
  static void del(EdgeSetType::RealType &element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE DoubleType:public Type {
public:
  typedef double RealType;
  static double undefinedValue();
  static double defaultValue();
  static void  del(DoubleType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE IntegerType:public Type
{
public:
  typedef int RealType;
  static int undefinedValue();
  static int defaultValue();
  static void  del(IntegerType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE BooleanType:public Type {
public:
  typedef bool RealType;
  static bool undefinedValue();
  static bool defaultValue();
  static void del(BooleanType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE LineType:public Type
{
public:
  typedef std::vector<Coord> RealType;
  static std::vector<Coord> undefinedValue();
  static std::vector<Coord> defaultValue();
  static void del(LineType::RealType &element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE PointType:public Type {
public:
  typedef Coord RealType;
  static Coord undefinedValue();
  static Coord defaultValue();
  static void del(PointType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE SizeType:public Type {
public:
  typedef  Size RealType;
  static Size undefinedValue();
  static Size defaultValue();
  static void del(SizeType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE StringType:public Type {
public:
  typedef std::string RealType;
  static std::string undefinedValue();
  static std::string defaultValue();
  static void del(StringType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE ColorType:public Type {
public:
  typedef Color RealType;
  static Color undefinedValue();
  static Color defaultValue();
  static void del(ColorType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE StringCollectionType: public Type {
public:
  typedef StringCollection RealType;
  static  RealType         undefinedValue();
  static  RealType         defaultValue();
  static  void             del(StringCollectionType::RealType element);
  static  std::string      toString(const RealType& sourceVector);
  static  bool             fromString( RealType& v, const std::string& s);
};

}
/*@}*/
#endif









