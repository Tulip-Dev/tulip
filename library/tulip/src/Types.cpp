//-*-c++-*-

#include "tulip/Types.h"
#include "tulip/Graph.h"

using namespace std;
using namespace tlp;

namespace {
  static int nocase_cmp(const string & s1, const string& s2) {
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

  template<typename T, int openParen>
  bool vectorFromString(vector<T>& v, const string &s) {
    v.clear();
    unsigned long pos = s.find_first_not_of(' ', 0);
    if (pos == string::npos)
      return false;

    istringstream iss;
    iss.str(s.substr(pos));
    char c;
    if( !(iss >> c) || c!='(' )
      return false;
    T val;
    bool endFound = false;
    bool firstVal = true;
    bool sepFound = false;
    for(;;) {
      if( !(iss >> c) )
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
	if (firstVal || sepFound) {
	  if (openParen && c != '(')
	    return false;	    
	  iss.unget();
	  if( !(iss >> val) )
	    return false;
	  v.push_back(val);
	  firstVal = false;
	  sepFound = false;
	} else
	  return false;
      }
    }
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

string EdgeSetType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  set<edge>::const_iterator it;
  for(it = v.begin() ; it != v.end() ; ++it)
    oss << (*it).id << ' ';
  oss << ')';
  return oss.str();
}

bool EdgeSetType::fromString( RealType & v, const string & s ) {
  v.clear();
  // for compatibility with older version (3.0)
  if (s.empty())
    return true;
  istringstream iss;
  iss.str( s );
  char c;
  if( !(iss >> c) || c!='(' )
    return false;
  edge e;
  for( ;; ) {
    if( !(iss >> c) )
      return false;
    if( c == ')' )
      return true;
    iss.unget();
    if( !(iss >> e.id) )
      return false;
    v.insert( e );
  }
}

//
// DoubleType

double DoubleType::undefinedValue() {
  return DBL_MIN;
}

double DoubleType::defaultValue() {
  return 0;
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
// DoubleVectorType

vector<double> DoubleVectorType::undefinedValue() {
  return vector<double>();
}

vector<double> DoubleVectorType::defaultValue() {
  return vector<double>();
}

string DoubleVectorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      oss << ", ";
    oss << v[i];
  }
  oss << ')';
  return oss.str();
}

bool DoubleVectorType::fromString( RealType & v, const string & s ) {
  return vectorFromString<double, false>(v, s);
}

//
// IntegerType
int IntegerType::undefinedValue() {
  return INT_MIN;
}

int IntegerType::defaultValue() {
  return 0;
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
// IntegerVectorType

vector<int> IntegerVectorType::undefinedValue() {
  return vector<int>();
}

vector<int> IntegerVectorType::defaultValue() {
  return vector<int>();
}

string IntegerVectorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      oss << ", ";
    oss << v[i];
  }
  oss << ')';
  return oss.str();
}

bool IntegerVectorType::fromString( RealType & v, const string & s ) {
  return vectorFromString<int,false>(v, s);
}

//
// BooleanType

bool BooleanType::undefinedValue() {
  return false;
}

bool BooleanType::defaultValue() {
  return false;
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
// BooleanVectorType

vector<bool> BooleanVectorType::undefinedValue() {
  return vector<bool>();
}

vector<bool> BooleanVectorType::defaultValue() {
  return vector<bool>();
}

string BooleanVectorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      oss << ", ";
    oss << (v[i] ? "true" : "false");
  }
  oss << ')';
  return oss.str();
}

bool BooleanVectorType::fromString( RealType & v, const string & s ) {
  v.clear();
  unsigned long pos = s.find_first_not_of(' ', 0);
  if (pos == string::npos)
    return false;

  istringstream iss;
  iss.str(s.substr(pos));
  char c;
  if( !(iss >> c) || c!='(' )
    return false;
  bool endFound = false;
  bool needToken = false;
  string token;
  for( ;; ) {
    bool sepFound = false;
    if( !(iss >> c) )
      return endFound;
    if (isspace(c))
      continue;
    if (endFound)
      return false;
    if (c == ')')
      endFound = true;
    if (c == ',') {
      sepFound = true;
      needToken = true;
    }
    if (endFound || sepFound) {
      if (token.empty() && !needToken)
	continue;
      if (nocase_cmp(token, "false") == 0) {
	v.push_back(false);
	token.clear();
	continue;
      }
      if( nocase_cmp(token, "true") == 0) {
 	v.push_back(true);
	token.clear();
	continue;
      }
      return false;
    } else {
      token.push_back(c);
      needToken = true;
    }
  }
}

//
// LineType

vector<Coord> LineType::undefinedValue() {
  return vector<Coord>();
}

vector<Coord> LineType::defaultValue() {
  return vector<Coord>();
}

string LineType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      oss << ", ";
    oss << v[i];
  }
  oss << ')';
  return oss.str();
}

bool LineType::fromString( RealType & v, const string & s ) {
  v.clear();
  unsigned long pos = s.find_first_not_of(' ', 0);
  if (pos == string::npos)
    return false;

  istringstream iss;
  iss.str(s.substr(pos));
  char c;
  if( !(iss >> c) || c!='(' )
    return false;
  Coord co;
  bool endFound = false;
  bool sepFound = false;
  for( ;; ) {
    if( !(iss >> c) )
      return endFound;
    if (isspace(c))
      continue;
    if (endFound)
      return false;
    if( c == ')' ) {
      endFound = true;
      continue;
    }
    if (c == '(') {
      sepFound = false;
      iss.unget();
      if( !(iss >> co) )
	return false;
      v.push_back( co );
    } else {
      if (c == ',') {
	if (sepFound)
	  return false;
	sepFound = true;
      } else if (c != ' ')
	return false;
    }
  }
}



//
// PointType
Coord PointType::undefinedValue() {
  Coord tmp;
  tmp.set(DBL_MIN,DBL_MIN,DBL_MIN);
  return tmp;
}

Coord PointType::defaultValue() {
  return Coord(0, 0, 0);
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
// SizeVectorType

vector<Size> SizeVectorType::undefinedValue() {
  return vector<Size>();
}

vector<Size> SizeVectorType::defaultValue() {
  return vector<Size>();
}

string SizeVectorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      oss << ", ";
    oss << v[i];
  }
  oss << ')';
  return oss.str();
}

bool SizeVectorType::fromString( RealType & v, const string & s ) {
  return vectorFromString<Size, true>(v, s);
}

//
// StringType

string StringType::undefinedValue() {
  return string("");
}

string StringType::defaultValue() {
  return string("");
}

string StringType::toString( const RealType & v ) {
  return string( v );
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

string StringVectorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      oss << ", ";
    string tmp = v[i];
    oss << '"';
    for (unsigned int j=0; j < tmp.length(); ++j) {
      if (tmp[j]=='\"')
	oss << "\\\"";
      else {
	if (tmp[j] == '\\')
	  oss << "\\\\";
	else
	  oss << tmp[j];
      }
    }
    oss << '"';
  }
  oss << ')';
  return oss.str();
}

bool StringVectorType::fromString( RealType & v, const string & s ) {
  v.clear();
  unsigned long pos = s.find_first_not_of(' ', 0);
  if (pos == string::npos)
    return false;

  istringstream iss;
  iss.unsetf(ios_base::skipws);
  iss.str(s.substr(pos));
  char c;
  if(!(iss >> c) || c!='(')
    return false;
  bool endFound = false;
  bool firstVal = true;
  bool sepFound = false;
  for( ;; ) {
    if( !(iss >> c) )
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
      if ((firstVal || sepFound) && c == '\"') {
	bool bslashFound = false;
	string str("");
	for(;;) {
	  if ( !(iss >> c) )
	    return false;
	  if (bslashFound) {
	    str.push_back(c);
	    bslashFound = false;
	  } else {
	    if (c == '\\')
	      bslashFound = true;
	    else {
	      if (c == '\"')
		break;
	      str.push_back(c);
	    }
	  }
	}
	v.push_back(str);
	firstVal = false;
	sepFound = false;
      } else
	return false;
    }
  }
}

//
// ColorType

Color ColorType::undefinedValue() {
  return Color(255,255,255,255);
}

Color ColorType::defaultValue() {
  return Color();
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
// ColorVectorType

vector<Color> ColorVectorType::undefinedValue() {
  return vector<Color>();
}

vector<Color> ColorVectorType::defaultValue() {
  return vector<Color>();
}

string ColorVectorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      oss << ", ";
    oss << v[i];
  }
  oss << ')';
  return oss.str();
}

bool ColorVectorType::fromString( RealType & v, const string & s ) {
  return vectorFromString<Color, true>(v, s);
}

//
// CoordVectorType

vector<Coord> CoordVectorType::undefinedValue() {
  return vector<Coord>();
}

vector<Coord> CoordVectorType::defaultValue() {
  return vector<Coord>();
}

string CoordVectorType::toString( const RealType & v ) {
  ostringstream oss;
  oss << '(';
  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      oss << ", ";
    oss << v[i];
  }
  oss << ')';
  return oss.str();
}

bool CoordVectorType::fromString( RealType & v, const string & s ) {
  return vectorFromString<Coord, true>(v, s);
}
