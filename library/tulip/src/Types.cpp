//-*-c++-*-

#include "tulip/Types.h"
#include "tulip/Graph.h"
#include "tulip/StringCollection.h"

using namespace std;
using namespace tlp;

namespace {
  int nocase_cmp(const string & s1, const string& s2) {
    string::const_iterator it1=s1.begin();
    string::const_iterator it2=s2.begin();
    
    //stop when either string's end has been reached
    while ( (it1!=s1.end()) && (it2!=s2.end()) ) { 
      if(::toupper(*it1) != ::toupper(*it2)) //letters differ?
	// return -1 to indicate smaller than, 1 otherwise
	return (::toupper(*it1)  < ::toupper(*it2)) ? -1 : 1; 
      //proceed to the next character in each string
      ++it1;
      ++it2;
    }
    size_t size1=s1.size(), size2=s2.size();// cache lengths
    //return -1,0 or 1 according to strings' lengths
    if (size1==size2) 
      return 0;
    return (size1<size2) ? -1 : 1;
  }
  
  bool isempty(const string & s1) {
    unsigned int s = s1.size();
    if( s == 0 )
      return true;
    for( unsigned int i = 0 ; i < s ; i++ ) {
      if( !isspace(s1[i]) )
	return false;
    }
    return true;
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

void GraphType::del(GraphType::RealType element) {
}

string GraphType::toString( const RealType & v ) {
  // NULL ?
  if( !v )
    return string();
  ostringstream oss;
  oss << v->getId();
  return oss.str();
}

bool GraphType::fromString( RealType & v, const string & s ) {
  v = 0;
  return false;
}


//
// DoubleType

double DoubleType::undefinedValue() {
  return DBL_MIN;
}

double DoubleType::defaultValue() {
  return 0;
}

void DoubleType::del(DoubleType::RealType element) {
}

string DoubleType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool DoubleType::fromString( RealType & v, const string & s ) {
  istringstream iss;
  iss.str( s );
  return (iss >> v);
}

//
// IntegerType
int IntegerType::undefinedValue() {
	return INT_MIN;
}

int IntegerType::defaultValue() {
  return 0;
}

void IntegerType::del(IntegerType::RealType element) {
}

string IntegerType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool IntegerType::fromString( RealType & v, const string & s ) {
  istringstream iss;
  iss.str( s );
  return (iss >> v);
}



//
// BooleanType

bool BooleanType::undefinedValue() {
  return false;
}

bool BooleanType::defaultValue() {
  return false;
}

void BooleanType::del(BooleanType::RealType element) {
}

string BooleanType::toString( const RealType & v ) {
  return v ? string("true") : string("false");
}

bool BooleanType::fromString( RealType & v, const string & s ) {
  if( nocase_cmp(s,"false")==0 ) {
    v = false;
    return true;
  }
  else if( nocase_cmp(s,"true")==0 ) {
    v = true;
    return true;
  }
  return false;
}
//
// LineType

vector<Coord> LineType::undefinedValue() {
  vector<Coord> tmp;
  return tmp;
}

vector<Coord> LineType::defaultValue() {
  vector<Coord> tmpList;
  return tmpList;
}

void LineType::del(LineType::RealType &element) {
	element.clear();
}

string LineType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ )
    oss << v[i];
  oss << ')';
  return oss.str();
}

bool LineType::fromString( RealType & v, const string & s ) {
  v.clear();
  istringstream iss;
  iss.str( s );
  char c;
  if( !(iss >> c) || c!='(' )
    return false;
  Coord co;
  for( ;; ) {
    if( !(iss >> c) )
      return false;
    if( c == ')' )
      return true;
    iss.unget();
    if( !(iss >> co) )
      return false;
    v.push_back( co );
  }
}



//
// PointType
Coord PointType::undefinedValue() {
  Coord tmp;
  tmp.set(DBL_MIN,DBL_MIN,DBL_MIN);
  return tmp;
}

Coord
PointType::defaultValue() {
  Coord tmp;
  tmp.set(rand()%1024,rand()%1024,rand()%1024);
  return tmp;
}

void PointType::del(PointType::RealType element) {
}

string PointType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool PointType::fromString( RealType & v, const string & s ) {
  istringstream iss;
  iss.str( s );
  return (iss >> v);
}

//
// SizeType

Size SizeType::undefinedValue() {
  return Size(DBL_MIN,DBL_MIN,DBL_MIN);
}

Size SizeType::defaultValue() {
  return Size(1,1,0);
}

void SizeType::del(SizeType::RealType element) {
}

string SizeType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool SizeType::fromString( RealType & v, const string & s ) {
  istringstream iss;
  iss.str( s );
  return (iss >> v);
}

//
// StringType

string StringType::undefinedValue() {
  return string("");
}

string StringType::defaultValue() {
  return string("");
}

void StringType::del(StringType::RealType element) {
}

string StringType::toString( const RealType & v ) {
  return string( v );
}

bool StringType::fromString( RealType & v, const string & s ) {
  v = s;
  return true;
}

//
// ColorType

Color ColorType::undefinedValue() {
  return Color(255,255,255,255);
}

Color ColorType::defaultValue() {
	return Color();
}

void ColorType::del(ColorType::RealType element) {
}

string ColorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << v;
  return oss.str();
}

bool ColorType::fromString( RealType & v, const string & s ) {
  istringstream iss;
  iss.str( s );
  return (iss >> v);
}

//
// StringCollectionType

StringCollection StringCollectionType::undefinedValue() {
  return StringCollection();
}

StringCollection StringCollectionType::defaultValue() {
  return StringCollection();
}

void StringCollectionType::del(StringCollectionType::RealType element) {
}

string
StringCollectionType::toString( const RealType & v ) {
    string result;
    for (unsigned int i = 0; i < v.size(); i++)
      result += v[i];
    return result;
}

bool StringCollectionType::fromString( RealType & v, const string & s ){
  return v.setCurrent(s);
}
