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

#include "tulip/Types.h"
#include "tulip/Graph.h"

using namespace std;
using namespace tlp;

namespace {
  template<typename T, int openParen>
  static bool readVector(istream& is, vector<T>& v) {
    v.clear();

    char c =' ';
    T val;
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
      } else {
	if (firstVal || sepFound) {
	  if (openParen && c != '(')
	    return false;	    
	  is.unget();
	  if( !(is >> val) )
	    return false;
	  v.push_back(val);
	  firstVal = false;
	  sepFound = false;
	} else
	  return false;
      }
    }
  }

  template<typename T>
  static void writeVector(ostream& os, const vector<T>& v) {
    os << '(';
    for( unsigned int i = 0 ; i < v.size() ; i++ ) {
      if (i)
	os << ", ";
      os << v[i];
    }
    os << ')';
  }
}

//
// GraphType

GraphType::RealType GraphType::undefinedValue() {
  return 0;
}

GraphType::RealType GraphType::defaultValue() {
  return 0;
}

string GraphType::toString( const RealType & v ) {
  // NULL ?
  if( !v )
    return string();
  ostringstream oss;
  oss << v->getId();
  return oss.str();
}

bool GraphType::fromString(RealType & v, const string & s ) {
  istringstream iss;
  iss.str( s );
  unsigned long lv;
  bool ok = iss >> lv;
  if (ok)
    v = (RealType) lv;
  else
    v = 0;
  return ok;
}

//
// EdgeSetType

set<edge> EdgeSetType::undefinedValue() {
  return set<edge>();
}

set<edge> EdgeSetType::defaultValue() {
  return set<edge>();
}

void EdgeSetType::write(ostream& os, const RealType & v ) {
  os << '(';
  set<edge>::const_iterator it;
  for(it = v.begin() ; it != v.end() ; ++it)
    os << (*it).id << ' ';
  os << ')';
}

string EdgeSetType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool EdgeSetType::read(istream& is, RealType & v) {
  v.clear();
  char c  = ' ';
  bool ok;
  // go to first '('
  while((ok = (is >> c)) && isspace(c)) {}
  // for compatibility with older version (3.0)
  if (!ok)
    return true;
  if (c != '(')
    return false;
  edge e;
  for( ;; ) {
    if( !(is >> c) )
      return false;
    if( c == ')' )
      return true;
    is.unget();
    if( !(is >> e.id) )
      return false;
    v.insert( e );
  }
}

bool EdgeSetType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return read(iss, v);
}

//
// DoubleType

double DoubleType::undefinedValue() {
  return -DBL_MAX;
}

double DoubleType::defaultValue() {
  return 0;
}

void DoubleType::write(ostream& os, const RealType & v) {
  os << v;
}

string DoubleType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool DoubleType::read(istream& iss, RealType & v) {
  return (iss >> v);
}

bool DoubleType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return (iss >> v);
}

//
// DoubleVectorType

vector<double> DoubleVectorType::undefinedValue() {
  return vector<double>();
}

vector<double> DoubleVectorType::defaultValue() {
  return vector<double>();
}

void DoubleVectorType::write(ostream& os, const RealType & v) {
  writeVector<double>(os, v);
}

string DoubleVectorType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool DoubleVectorType::read(istream& iss, RealType & v) {
  return readVector<double, false>(iss, v);
}

bool DoubleVectorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return read(iss, v);
}

//
// IntegerType
int IntegerType::undefinedValue() {
  return INT_MIN;
}

int IntegerType::defaultValue() {
  return 0;
}

void IntegerType::write(ostream& os, const RealType & v) {
  os << v;
}

string IntegerType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool IntegerType::read(istream& iss, RealType & v) {
  return (iss >> v);
}

bool IntegerType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return (iss >> v);
}

//
// IntegerVectorType

vector<int> IntegerVectorType::undefinedValue() {
  return vector<int>();
}

vector<int> IntegerVectorType::defaultValue() {
  return vector<int>();
}

void IntegerVectorType::write(ostream& os, const RealType & v) {
  writeVector<int>(os, v);
}

string IntegerVectorType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool IntegerVectorType::read(istream& iss, RealType & v) {
  return readVector<int,false>(iss, v);
}

bool IntegerVectorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return read(iss, v);
}

//
// BooleanType

bool BooleanType::undefinedValue() {
  return false;
}

bool BooleanType::defaultValue() {
  return false;
}

void BooleanType::write(ostream& os, const RealType &v) {
  if (v)
    os << "true";
  else
    os << "false";
}

string BooleanType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool BooleanType::read(istream& is, RealType & v) {
  char c = ' ';
  while((is >> c) && isspace(c)) {}
  c = ::tolower(c);
  if (c != 't' && c != 'f')
    return false;
  string s;
  if (c == 't') {
    s.append("true");
    v = true;
  } else {
    s.append("false");
    v = false;
  }
  for(unsigned int i = 1; i < s.size(); ++i) {
    if (!(is >> c))
      return false;
    c = ::tolower(c);
    if (c != s[i])
      return false;
  }
  return true;
}

bool BooleanType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return read(iss, v);
}

//
// BooleanVectorType

vector<bool> BooleanVectorType::undefinedValue() {
  return vector<bool>();
}

vector<bool> BooleanVectorType::defaultValue() {
  return vector<bool>();
}

void BooleanVectorType::write(ostream& os, const RealType & v) {
  os << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      os << ", ";
    os << (v[i] ? "true" : "false");
  }
  os << ')';
}

string BooleanVectorType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool BooleanVectorType::read(istream& is,  RealType & v) {
  v.clear();

  char c =' ';
  bool firstVal = true;
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
      return true;
    }
    if (c == ',') {
      if (firstVal)
	return false;
    } else
      is.unget();
    bool val;
    if (!BooleanType::read(is, val))
      return false;
    v.push_back(val);
    firstVal = false;
  }
}

bool BooleanVectorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return read(iss, v);
}

//
// LineType

vector<Coord> LineType::undefinedValue() {
  return vector<Coord>();
}

vector<Coord> LineType::defaultValue() {
  return vector<Coord>();
}

void LineType::write(ostream& os, const RealType & v) {
  writeVector<Coord>(os, v);
}

string LineType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool LineType::read(istream& is, RealType& v) {
  v.clear();

  char c =' ';
  bool firstVal = true;
  bool dbqFound = false;
  // go to first '('
  // skip space chars
  while((is >> c) && isspace(c)) {}
  // value may have been enclosed by double quotes
  if (c == '"') {
    // open double quotes found
    dbqFound = true;
    // skip space chars
    while((is >> c) && isspace(c)) {}
  }
  if (c != '(')
    return false;
  for(;;) {
    if( !(is >> c) )
      return false;
    if (isspace(c))
      continue;
    if(c == ')') {
      if (dbqFound) {
	// expect it is the next non space char
	while((is >> c) && isspace(c)) {}
	if (c != '"')
	  return false;
      }
      return true;
    }
    if (c == ',') {
      if (firstVal)
	return false;
      Coord val;
      if (!PointType::read(is, val))
	return false;
      v.push_back(val);
     } else {
      // , is not required between coords
      is.unget();
      Coord val;
      if (!PointType::read(is, val))
	return false;
      v.push_back(val);
      firstVal = false;
    }
  }
}

bool LineType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return read(iss, v);
}

//
// PointType
Coord PointType::undefinedValue() {
  Coord tmp;
  tmp.set(-DBL_MAX,-DBL_MAX,-DBL_MAX);
  return tmp;
}

Coord PointType::defaultValue() {
  return Coord(0, 0, 0);
}

void PointType::write(ostream& os, const RealType & v) {
  os << v;
}

string PointType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool PointType::read(istream& is, RealType & v) {
  // value may have been enclosed by double quotes
  char c = ' ';
  bool ok;
  // skip spaces
  while((ok = (is >> c)) && isspace(c)) {}
  bool dbqFound = false;
  if (c == '"')
    // open double quotes found
    dbqFound = true;
  else
    is.unget();
  ok = is >> v;
  if (ok && dbqFound) {
    // look for the close double quote
    ok = is >> c;
    if (c != '"')
      return false;
  }
  return ok; 
}

bool PointType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return iss >> v;
}

//
// SizeType

Size SizeType::undefinedValue() {
  return Size(-DBL_MAX,-DBL_MAX,-DBL_MAX);
}

Size SizeType::defaultValue() {
  return Size(1,1,0);
}

void SizeType::write(ostream& os, const RealType & v) {
  os << v;
}

string SizeType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool SizeType::read(istream& is, RealType & v) {
  // value may have been enclosed by double quotes
  char c = ' ';
  bool ok;
  // skip spaces
  while((ok = (is >> c)) && isspace(c)) {}
  bool dbqFound = false;
  if (c == '"')
    // open double quotes found
    dbqFound = true;
  else
    is.unget();
  ok = is >> v;
  if (ok && dbqFound) {
    // look for the close double quote
    ok = is >> c;
    if (c != '"')
      return false;
  }
  return ok; 
}

bool SizeType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return iss >> v;
}

//
// SizeVectorType

vector<Size> SizeVectorType::undefinedValue() {
  return vector<Size>();
}

vector<Size> SizeVectorType::defaultValue() {
  return vector<Size>();
}

void SizeVectorType::write(ostream& os, const RealType & v) {
  writeVector<Size>(os, v);
}

string SizeVectorType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool SizeVectorType::read(istream& is, RealType& v) {
  return readVector<Size, true>(is, v);
}

bool SizeVectorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return readVector<Size, true>(iss, v);
}

//
// StringType

string StringType::undefinedValue() {
  return string("");
}

string StringType::defaultValue() {
  return string("");
}

void StringType::write(ostream& os, const RealType & v ) {
  os << '"';
  for(char* str = (char *) v.c_str(); *str; ++str) {
    char c = *str;
    if (c == '\\' || c == '"')
      os << '\\';
    os << c;
  }
  os << '"';
}

string StringType::toString( const RealType & v ) {
  return v;
}

bool StringType::read(istream& is, RealType & v) {
  char c= ' ';
  // go to first '"'
  while((is >> c) && isspace(c)) {}
  if (c != '"')
    return false;
  bool bslashFound = false;
  string str;
  for(;;) {
    if ( !(is >> c) )
      return false;
    if (bslashFound) {
      str.push_back(c);
      bslashFound = false;
    } else {
      if (c == '\\')
	bslashFound = true;
      else {
	if (c == '"')
	  break;
	str.push_back(c);
      }
    }
  }
  v = str;
  return true;
}

bool StringType::fromString( RealType & v, const string & s ) {
  v = s;
  return true;
}

//
// StringVectorType

vector<string> StringVectorType::undefinedValue() {
  return vector<string>();
}

vector<string> StringVectorType::defaultValue() {
  return vector<string>();
}

void StringVectorType::write(ostream& os, const RealType & v) {
  os << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      os << ", ";
    StringType::write(os, v[i]);
  }
  os << ')';
}

string StringVectorType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool StringVectorType::read(istream& is, RealType & v) {
  v.clear();
  char c = ' ';
  // go to first '('
  while((is >> c) && isspace(c)) {}
  if (c != '(')
    return false;

  is.unsetf(ios_base::skipws);
  bool endFound = false;
  bool firstVal = true;
  bool sepFound = false;
  for( ;; ) {
    if( !(is >> c) )
      return endFound;
    if (isspace(c))
      continue;
    if (endFound)
      return false;
    if( c == ')' ) {
      if (sepFound)
	return false;
      endFound = true;
      continue;
    }
    if (c == ',') {
      if (sepFound)
	return false;
      sepFound = true;
    } else {
      if ((firstVal || sepFound) && c == '"') {
	string str;
	is.unget();
	if (!StringType::read(is, str))
	  return false;
	v.push_back(str);
	firstVal = false;
	sepFound = false;
      } else
	return false;
    }
  }
}

bool StringVectorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return read(iss, v);
}

//
// ColorType

Color ColorType::undefinedValue() {
  return Color(255,255,255,255);
}

Color ColorType::defaultValue() {
  return Color();
}

void ColorType::write(ostream& os, const RealType & v) {
  // add double quotes to ensure compatibility in gui tests
  os << '"' << v << '"';
}

string ColorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool ColorType::read(istream& is, RealType & v) {
  // value may have been enclosed by double quotes
  char c = ' ';
  bool ok;
  // skip spaces
  while((ok = (is >> c)) && isspace(c)) {}
  bool dbqFound = false;
  if (c == '"')
    // open double quotes found
    dbqFound = true;
  else
    is.unget();
  ok = is >> v;
  if (ok && dbqFound) {
    // look for the close double quote
    ok = is >> c;
    if (c != '"')
      return false;
  }
  return ok; 
}

bool ColorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return iss >> v;
}

//
// ColorVectorType

vector<Color> ColorVectorType::undefinedValue() {
  return vector<Color>();
}

vector<Color> ColorVectorType::defaultValue() {
  return vector<Color>();
}

void ColorVectorType::write(ostream& os, const RealType & v) {
  writeVector<Color>(os, v);
}

string ColorVectorType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool ColorVectorType::read(istream& is, RealType& v) {
  return readVector<Color, true>(is, v);
}

bool ColorVectorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return readVector<Color, true>(iss, v);
}

//
// CoordVectorType

vector<Coord> CoordVectorType::undefinedValue() {
  return vector<Coord>();
}

vector<Coord> CoordVectorType::defaultValue() {
  return vector<Coord>();
}

void CoordVectorType::write(ostream& os, const RealType & v) {
  writeVector<Coord>(os, v);
}

string CoordVectorType::toString( const RealType & v ) {
  ostringstream oss;
  write(oss, v);
  return oss.str();
}

bool CoordVectorType::read(istream& is, RealType& v) {
  return readVector<Coord, true>(is, v);
}

bool CoordVectorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return readVector<Coord, true>(iss, v);
}

// template class to automatize definition of serializers
template<typename T>
struct KnownTypeSerializer :public TypedDataSerializer<typename T::RealType> {
  KnownTypeSerializer(const string& otn):TypedDataSerializer<typename T::RealType>(otn) {}
  KnownTypeSerializer(const char* otn):TypedDataSerializer<typename T::RealType>(string(otn)) {}
  
  DataTypeSerializer* clone() const {
    return new KnownTypeSerializer<T>(this->outputTypeName);
  }

  void write(ostream& os, const typename T::RealType& v) {
    T::write(os, v);
  }
  bool read(istream& iss, typename T::RealType& v) {
    return T::read(iss, v);
  }
};

// some special serializers
struct UintTypeSerializer :public TypedDataSerializer<unsigned int> {
  UintTypeSerializer():TypedDataSerializer<unsigned int>("uint") {}

  DataTypeSerializer* clone() const {
    return new UintTypeSerializer();
  }

  void write(ostream& os, const unsigned int& v) {
    os << v;
  }

  bool read(istream& is, unsigned int& v) {
    return is >> v;
  }
};

struct FloatTypeSerializer :public TypedDataSerializer<float> {
  FloatTypeSerializer():TypedDataSerializer<float>("float") {}

  DataTypeSerializer* clone() const {
    return new FloatTypeSerializer();
  }

  void write(ostream& os, const float& v) {
    os << v;
  }

  bool read(istream& is, float& v) {
    return is >> v;
  }
};

struct DataSetTypeSerializer :public TypedDataSerializer<DataSet> {
  DataSetTypeSerializer():TypedDataSerializer<DataSet>("DataSet") {}

  DataTypeSerializer* clone() const {
    return new DataSetTypeSerializer();
  }

  void write(ostream& os, const DataSet& ds) {
    DataSet::write(os, ds);
  }

  bool read(istream& is, DataSet& ds) {
    return DataSet::read(is, ds);
  }
};

void tlp::initTypeSerializers() {
  DataSet::registerDataTypeSerializer<EdgeSetType::RealType>(KnownTypeSerializer<EdgeSetType>("edgeset"));

  DataSet::registerDataTypeSerializer<DoubleType::RealType>(KnownTypeSerializer<DoubleType>("double"));

  DataSet::registerDataTypeSerializer<float>(FloatTypeSerializer());

  DataSet::registerDataTypeSerializer<BooleanType::RealType>(KnownTypeSerializer<BooleanType>("bool"));

  DataSet::registerDataTypeSerializer<IntegerType::RealType>(KnownTypeSerializer<IntegerType>("int"));

  DataSet::registerDataTypeSerializer<unsigned int>(UintTypeSerializer());

  DataSet::registerDataTypeSerializer<ColorType::RealType>(KnownTypeSerializer<ColorType>("color"));

  DataSet::registerDataTypeSerializer<PointType::RealType>(KnownTypeSerializer<PointType>("coord"));

  DataSet::registerDataTypeSerializer<SizeType::RealType>(KnownTypeSerializer<SizeType>("size"));

  DataSet::registerDataTypeSerializer<StringType::RealType>(KnownTypeSerializer<StringType>("string"));

  DataSet::registerDataTypeSerializer<DoubleVectorType::RealType>(KnownTypeSerializer<DoubleVectorType>("doublevector"));

  DataSet::registerDataTypeSerializer<BooleanVectorType::RealType>(KnownTypeSerializer<BooleanVectorType>("boolvector"));

  DataSet::registerDataTypeSerializer<IntegerVectorType::RealType>(KnownTypeSerializer<IntegerVectorType>("intvector"));

  DataSet::registerDataTypeSerializer<ColorVectorType::RealType>(KnownTypeSerializer<ColorVectorType>("colorvector"));

  DataSet::registerDataTypeSerializer<LineType::RealType>(KnownTypeSerializer<LineType>("coordvector"));

  DataSet::registerDataTypeSerializer<SizeVectorType::RealType>(KnownTypeSerializer<SizeVectorType>("sizevector"));

  DataSet::registerDataTypeSerializer<StringVectorType::RealType>(KnownTypeSerializer<StringVectorType>("stringvector"));

  DataSet::registerDataTypeSerializer<DataSet>(DataSetTypeSerializer());
}

 
