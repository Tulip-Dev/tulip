//-*-c++-*-
#ifndef TYPES_H
#define TYPES_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <float.h>
#include "Coord.h"
#include "Size.h"
#include "Color.h"
#include "StringCollection.h"

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
  static GraphType::RealType undefineValue();
  static GraphType::RealType defaultValue();
  static void  del(GraphType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE DoubleType:public Type {
public:
  typedef double RealType;
  static double undefineValue();
  static double defaultValue();
  static void  del(DoubleType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE IntegerType:public Type
{
public:
  typedef int RealType;
  static int undefineValue();
  static int defaultValue();
  static void  del(IntegerType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE BooleanType:public Type {
public:
  typedef bool RealType;
  static bool undefineValue();
  static bool defaultValue();
  static void del(BooleanType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE LineType:public Type
{
public:
  typedef std::vector<Coord> RealType;
  static std::vector<Coord> undefineValue();
  static std::vector<Coord> defaultValue();
  static void del(LineType::RealType &element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE PointType:public Type {
public:
  typedef Coord RealType;
  static Coord undefineValue();
  static Coord defaultValue();
  static void del(PointType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE SizeType:public Type {
public:
  typedef  Size RealType;
  static Size undefineValue();
  static Size defaultValue();
  static void del(SizeType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE StringType:public Type {
public:
  typedef std::string RealType;
  static std::string undefineValue();
  static std::string defaultValue();
  static void del(StringType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE ColorType:public Type {
public:
  typedef Color RealType;
  static Color undefineValue();
  static Color defaultValue();
  static void del(ColorType::RealType element);
  static std::string toString( const RealType & v );
  static bool fromString( RealType & v, const std::string & s );
};

class TLP_SCOPE StringCollectionType: public Type {
public:
  typedef StringCollection RealType;
  static  RealType         undefineValue();
  static  RealType         defaultValue();
  static  void             del(StringCollectionType::RealType element);
  static  std::string      toString(const RealType& sourceVector);
  static  bool             fromString( RealType& v, const std::string& s);
};

}
/*@}*/
#endif









