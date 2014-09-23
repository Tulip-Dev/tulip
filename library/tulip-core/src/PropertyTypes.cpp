/**
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

#include <tulip/PropertyTypes.h>
#include <tulip/Graph.h>
#include <tulip/StringCollection.h>

#include <stdint.h>

using namespace std;
using namespace tlp;

// GraphType
GraphType::RealType GraphType::undefinedValue() {
  return 0;
}

GraphType::RealType GraphType::defaultValue() {
  return 0;
}

void GraphType::write(ostream &oss, const RealType &v) {
  if (v)
    oss << v->getId();
}

void GraphType::writeb(ostream &oss, const RealType &v) {
  unsigned int id = v ? v->getId() : 0;
  oss.write((char *) &id, sizeof(id));
}

bool GraphType::read(istream& iss, RealType& v) {
  uintptr_t lv = 0;
  bool ok = bool(iss >> lv);

  if (ok)
    v = (RealType) lv;
  else
    v = 0;

  return ok;
}

bool GraphType::readb(istream &, RealType &) {
  // should not be called
  // must be intercepted by GraphProperty::readNodeValue
  assert(false);
  tlp::error() << __PRETTY_FUNCTION__ << " should not be called" << std::endl;
  return false;
}

// EdgeSetType
void EdgeSetType::write(ostream& os, const RealType & v ) {
  os << '(';
  set<edge>::const_iterator it;

  for(it = v.begin() ; it != v.end() ; ++it)
    os << (*it).id << ' ';

  os << ')';
}

void EdgeSetType::writeb(ostream& oss, const RealType & v ) {
  unsigned int vSize = v.size();
  // write the size of the set
  oss.write((char *) &vSize, sizeof(vSize));

  set<edge>::const_iterator it;

  // loop to write the edges
  for(it = v.begin() ; it != v.end() ; ++it)
    oss.write((char *) &(it->id), sizeof(unsigned int));
}

bool EdgeSetType::read(istream& is, RealType & v) {
  v.clear();
  char c  = ' ';
  bool ok;

  // go to first '('
  while((ok = bool(is >> c)) && isspace(c)) {}

  // for compatibility with older version (3.0)
  if (!ok)
    return true;

  if (c != '(')
    return false;

  edge e;

  for( ;; ) {
    if( !(is >> c) )
      return false;

    if (isspace(c)) continue;

    if( c == ')' )
      return true;

    is.unget();

    if( !(is >> e.id) )
      return false;

    v.insert( e );
  }
}

bool EdgeSetType::readb(istream& iss, RealType & s) {
  s.clear();

  unsigned int size;

  // get the set size
  if (!bool(iss.read((char *) &size, sizeof(unsigned int))))
    return false;

  // use a vector to get the edges
  vector<edge> v;
  v.resize(size);
  edge* data = v.data();

  // get the edges in one read
  if (!bool(iss.read((char *) v.data(), size * sizeof(unsigned int))))
    return false;

  // insert edges in the set
  while(size) {
    s.insert(*data);
    ++data;
    --size;
  }

  return true;
}

// DoubleType
double DoubleType::undefinedValue() {
  return -DBL_MAX;
}

double DoubleType::defaultValue() {
  return 0;
}

// add support for inf and -inf
bool DoubleType::read(istream& iss, double& v) {
  char c;
  char sign = 0;

  // go to first non space char
  while((iss >> c) && isspace(c)) {}

  if (c == '-' || c == '+') {
    sign = c;

    if (!(iss >> c))
      return false;
  }

  if (c == 'i') {
    // should be inf
    if (!(iss >> c) || (c != 'n') ||
        !(iss >> c) || (c != 'f'))
      return false;

    if (sign == '-')
      v = -numeric_limits<double>::infinity();
    else
      v = numeric_limits<double>::infinity();

    return true;
  }
  else {
    iss.unget();

    if (sign)
      iss.unget();
  }

  return bool(iss >> v);
}

// FloatType
float FloatType::undefinedValue() {
  return -FLT_MAX;
}

float FloatType::defaultValue() {
  return 0;
}

// IntegerType
int IntegerType::undefinedValue() {
  return INT_MIN;
}

int IntegerType::defaultValue() {
  return 0;
}

// LongType
long LongType::undefinedValue() {
  return LONG_MIN;
}

long LongType::defaultValue() {
  return 0;
}

// UnsignedIntegerType
unsigned int UnsignedIntegerType::undefinedValue() {
  return UINT_MAX;
}

unsigned int UnsignedIntegerType::defaultValue() {
  return 0;
}

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

bool BooleanType::read(istream& is, RealType & v, bool untilEnd) {
  char c = ' ';

  while((is >> c) && isspace(c)) {}

  c = ::tolower(c);

  string s;

  if (c == 't') {
    s.append("true");
    v = true;
  }
  else if (c == 'f') {
    s.append("false");
    v = false;
  }
  else if(c == '1') {
    s.append("1");
    v = true;
  }
  else if (c == '0') {
    s.append("0");
    v = false;
  }
  else
    return false;

  for(unsigned int i = 1; i < s.size(); ++i) {
    if (!(is >> c))
      return false;

    c = ::tolower(c);

    if (c != s[i])
      return false;
  }

  if (untilEnd) {
    // check if there is only space char until the end
    for(;;) {
      if( !(is >> c) )
        return true;

      if (!isspace(c))
        return false;
    }
  }

  return true;
}

bool BooleanType::fromString(RealType &v, const std::string &s,
                             bool untilEnd) {
  std::istringstream iss(s);
  return read(iss, v, untilEnd);
}

// BooleanVectorType
void BooleanVectorType::write(ostream& os, const RealType & v) {
  os << '(';

  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      os << ", ";

    os << (v[i] ? "true" : "false");
  }

  os << ')';
}

void BooleanVectorType::writeb(ostream& oss, const RealType & v) {
  unsigned int vSize = v.size();
  // write the size of the vector
  oss.write((char *) &vSize, sizeof(vSize));

  vector<char> vc;
  vc.resize(vSize);

  // loop to write boolean as char
  char* data = vc.data();

  for(unsigned int i = 0; i < vSize; ++i, ++data) {
    *data = v[i] ? 1 : 0;
  }

  oss.write(vc.data(), vSize);
}

bool BooleanVectorType::read(istream& is,  RealType & v, char openChar,
                             char sepChar, char closeChar) {
  v.clear();

  char c =' ';
  bool firstVal = true;

  // go to first non space char
  while((is >> c) && isspace(c)) {}

  if (openChar) {
    if (c != openChar)
      return false;
  }
  else
    is.unget();

  for(;;) {
    if( !(is >> c) )
      return !closeChar;

    if (isspace(c))
      continue;

    if(c == closeChar) {
      return true;
    }

    if (c == sepChar) {
      if (firstVal)
        return false;
    }
    else
      is.unget();

    bool val;

    if (!BooleanType::read(is, val))
      return false;

    v.push_back(val);
    firstVal = false;
  }
}

bool BooleanVectorType::readb(istream& iss, RealType & v) {
  unsigned int vSize = v.size();

  // read the size of the vector
  if (!iss.read((char *) &vSize, sizeof(vSize)))
    return false;

  vector<char> vc;
  vc.resize(vSize);

  // loop to write boolean as char
  char* data = vc.data();

  if (!bool(iss.read(data, vSize)))
    return false;

  for(unsigned int i = 0; i < vSize; ++i, ++data) {
    v[i] = *data ? true : false;
  }

  return true;
}

// LineType
bool LineType::read(istream& is, RealType& v, char openChar,
                    char sepChar, char closeChar) {
  v.clear();

  char c =' ';
  bool firstVal = true;
  bool dbqFound = false;

  // go to first non space char
  while((is >> c) && isspace(c)) {}

  // value may have been enclosed by double quotes
  if (c == '"') {
    // open double quotes found
    dbqFound = true;

    // skip space chars
    while((is >> c) && isspace(c)) {}
  }

  if (openChar) {
    if (c != openChar)
      return false;
  }
  else
    is.unget();

  for(;;) {
    if( !(is >> c) )
      return !closeChar;

    if (isspace(c))
      continue;

    if(c == closeChar) {
      if (dbqFound) {
        // expect it is the next non space char
        while((is >> c) && isspace(c)) {}

        if (c != '"')
          return false;
      }

      return openChar;
    }

    if (c == sepChar) {
      if (firstVal)
        return false;

      Coord val;

      if (!PointType::read(is, val))
        return false;

      v.push_back(val);
    }
    else {
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

// PointType
Coord PointType::undefinedValue() {
  Coord tmp;
  tmp.set(-FLT_MAX,-FLT_MAX,-FLT_MAX);
  return tmp;
}

Coord PointType::defaultValue() {
  return Coord(0, 0, 0);
}

bool PointType::read(istream& is, RealType & v) {
  // value may have been enclosed by double quotes
  char c = ' ';
  bool ok;

  // skip spaces
  while((ok = bool(is >> c)) && isspace(c)) {}

  bool dbqFound = false;

  if (c == '"')
    // open double quotes found
    dbqFound = true;
  else
    is.unget();

  ok = bool(is >> v);

  if (ok && dbqFound) {
    // look for the close double quote
    ok = bool(is >> c);

    if (c != '"')
      return false;
  }

  return ok;
}

bool PointType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return bool(iss >> v);
}

//
// SizeType
Size SizeType::undefinedValue() {
  return Size(-FLT_MAX,-FLT_MAX,-FLT_MAX);
}

Size SizeType::defaultValue() {
  return Size(1,1,0);
}

bool SizeType::read(istream& is, RealType & v) {
  // value may have been enclosed by double quotes
  char c = ' ';
  bool ok;

  // skip spaces
  while((ok = bool(is >> c)) && isspace(c)) {}

  bool dbqFound = false;

  if (c == '"')
    // open double quotes found
    dbqFound = true;
  else
    is.unget();

  ok = bool(is >> v);

  if (ok && dbqFound) {
    // look for the close double quote
    ok = bool(is >> c);

    if (c != '"')
      return false;
  }

  return ok;
}

bool SizeType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return bool(iss >> v);
}

// StringType
string StringType::undefinedValue() {
  return string("");
}

string StringType::defaultValue() {
  return string("");
}

void StringType::write(ostream& os, const RealType & v, char openCloseChar) {
  if (openCloseChar)
    os << openCloseChar;

  for(char* str = (char *) v.c_str(); *str; ++str) {
    char c = *str;

    if (c == '\\' || c == '"')
      os << '\\';

    os << c;
  }

  if (openCloseChar)
    os << openCloseChar;
}

void StringType::writeb(ostream& os, const RealType & str) {
  // write size of str
  unsigned int size = str.size();
  os.write((char *) &size, sizeof(size));

  // then write c_str()
  os.write((char *) str.c_str(), size);
}

string StringType::toString( const RealType & v ) {
  return v;
}

bool StringType::read(istream& is, RealType & v, char openChar,
                      char closeChar) {
  char c= ' ';

  // go to first non space char
  while((is >> c) && isspace(c)) {}

  // ensure space chars are not skipped
  // in the string to extract
  is.unsetf(ios_base::skipws);

  if (openChar && c != openChar)
    return false;

  if (!openChar)
    is.unget();

  bool bslashFound = false;
  string str;

  for(;;) {
    if ( !(is >> c) ) {
      if (!openChar || !closeChar)
        break;

      return false;
    }

    if (bslashFound) {
      str.push_back(c);
      bslashFound = false;
    }
    else {
      if (c == '\\')
        bslashFound = true;
      else {
        if (closeChar && c == closeChar)
          break;

        str.push_back(c);
      }
    }
  }

  // remove trailing space chars
  std::size_t found = str.find_last_not_of(" \t\f\v\n\r");

  if (found != std::string::npos)
    str.erase(found+1);

  v = str;
  return true;
}

bool StringType::readb(istream& iss, RealType & str) {
  // read size of str
  unsigned int size;

  if (!bool(iss.read((char *)&size, sizeof(size))))
    return false;

  // then read chars
  str.resize(size);
  return bool(iss.read((char *) str.c_str(), size));
}

bool StringType::fromString( RealType & v, const string & s ) {
  v = s;
  return true;
}

// StringVectorType
void StringVectorType::write(ostream& os, const RealType & v) {
  os << '(';

  for( unsigned int i = 0 ; i < v.size() ; i++ ) {
    if (i)
      os << ", ";

    StringType::write(os, v[i]);
  }

  os << ')';
}

void StringVectorType::writeb(ostream& os, const RealType & v) {
  // write size of vector
  unsigned int size = v.size();
  os.write((char *) &size, sizeof(size));

  // loop to write strings
  for(unsigned int i = 0; i < size; ++i)
    StringType::writeb(os, v[i]);
}

bool StringVectorType::read(istream& is, RealType & v, char openChar,
                            char sepChar, char closeChar) {
  v.clear();
  char c = ' ';

  // go to first non space char
  while((is >> c) && isspace(c)) {}

  if (openChar) {
    if (c != openChar)
      return false;
  }
  else
    is.unget();

  is.unsetf(ios_base::skipws);
  bool firstVal = true;
  bool sepFound = false;

  for( ;; ) {
    if( !(is >> c) )
      return (!sepFound && !closeChar);

    if (isspace(c))
      continue;

    if( c == closeChar) {
      if (!closeChar || sepFound)
        return false;

      return true;
    }

    if (c == sepChar) {
      if (sepFound)
        return false;

      sepFound = true;
    }
    else {
      if ((firstVal || sepFound) && (!openChar || c == '"')) {
        string str;
        is.unget();

        if (!(openChar ? StringType::read(is, str) :
              StringType::read(is, str, '\0', sepChar)))
          return false;

        v.push_back(str);

        if (!openChar)
          // last read char was sepChar
          is.unget();

        firstVal = false;
        sepFound = false;
      }
      else
        return false;
    }
  }
}

bool StringVectorType::readb(istream& iss, RealType & v) {
  // read size of vector
  unsigned int size;

  if (!bool(iss.read((char *) &size, sizeof(size))))
    return false;

  v.resize(size);

  // loop to read strings
  for(unsigned int i = 0; i < size; ++i) {
    if (!StringType::readb(iss, v[i]))
      return false;
  }

  return true;
}

// ColorType
Color ColorType::undefinedValue() {
  return Color(255,255,255,255);
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
  while((ok = bool(is >> c)) && isspace(c)) {}

  bool dbqFound = false;

  if (c == '"')
    // open double quotes found
    dbqFound = true;
  else
    is.unget();

  ok = bool(is >> v);

  if (ok && dbqFound) {
    // look for the close double quote
    ok = bool(is >> c);

    if (c != '"')
      return false;
  }

  return ok;
}

bool ColorType::fromString( RealType & v, const string & s ) {
  istringstream iss(s);
  return bool(iss >> v);
}

// some special serializers
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

  bool setData(tlp::DataSet&, const string&, const string&) {
    // no sense
    return false;
  }
};

// some special serializers
struct NodeTypeSerializer : public TypedDataSerializer<node> {

  KnownTypeSerializer<UnsignedIntegerType> *uintSerializer;

  NodeTypeSerializer():TypedDataSerializer<node>("node") {
    uintSerializer = new KnownTypeSerializer<UnsignedIntegerType>("");
  }

  ~NodeTypeSerializer() {
    delete uintSerializer;
  }

  DataTypeSerializer* clone() const {
    return new NodeTypeSerializer();
  }

  void write(ostream& os, const node& n) {
    uintSerializer->write(os, n.id);
  }

  bool read(istream& is, node& n) {
    return uintSerializer->read(is, n.id);
  }

  bool setData(tlp::DataSet&, const string&, const string&) {
    // no sense
    return false;
  }
};

struct NodeVectorTypeSerializer : public TypedDataSerializer<vector<node> > {

  KnownTypeSerializer<UnsignedIntegerVectorType> *uintVecSerializer;

  NodeVectorTypeSerializer():TypedDataSerializer<vector<node> >("nodes") {
    uintVecSerializer = new KnownTypeSerializer<UnsignedIntegerVectorType>("");
  }

  ~NodeVectorTypeSerializer() {
    delete uintVecSerializer;
  }

  DataTypeSerializer* clone() const {
    return new NodeVectorTypeSerializer();
  }

  void write(ostream& os, const vector<node>& vn) {
    uintVecSerializer->write(os, reinterpret_cast<const vector<unsigned int> &>(vn));
  }

  bool read(istream& is, vector<node>& vn) {
    return uintVecSerializer->read(is, reinterpret_cast<vector<unsigned int> &>(vn));
  }

  bool setData(tlp::DataSet&, const string&, const string&) {
    // no sense
    return false;
  }
};

struct EdgeTypeSerializer : public TypedDataSerializer<edge> {

  KnownTypeSerializer<UnsignedIntegerType> *uintSerializer;

  EdgeTypeSerializer():TypedDataSerializer<edge>("edge") {
    uintSerializer = new KnownTypeSerializer<UnsignedIntegerType>("");
  }

  ~EdgeTypeSerializer() {
    delete uintSerializer;
  }

  DataTypeSerializer* clone() const {
    return new EdgeTypeSerializer();
  }

  void write(ostream& os, const edge& e) {
    uintSerializer->write(os, e.id);
  }

  bool read(istream& is, edge& e) {
    return uintSerializer->read(is, e.id);
  }

  bool setData(tlp::DataSet&, const string&, const string&) {
    // no sense
    return false;
  }
};

struct EdgeVectorTypeSerializer : public TypedDataSerializer<vector<edge> > {

  KnownTypeSerializer<UnsignedIntegerVectorType> *uintVecSerializer;

  EdgeVectorTypeSerializer():TypedDataSerializer<vector<edge> >("edges") {
    uintVecSerializer = new KnownTypeSerializer<UnsignedIntegerVectorType>("");
  }

  ~EdgeVectorTypeSerializer() {
    delete uintVecSerializer;
  }

  DataTypeSerializer* clone() const {
    return new EdgeVectorTypeSerializer();
  }

  void write(ostream& os, const vector<edge>& ve) {
    uintVecSerializer->write(os, reinterpret_cast<const vector<unsigned int> &>(ve));
  }

  bool read(istream& is, vector<edge>& ve) {
    return uintVecSerializer->read(is, reinterpret_cast<vector<unsigned int> &>(ve));
  }

  bool setData(tlp::DataSet&, const string&, const string&) {
    // no sense
    return false;
  }

};

struct StringCollectionSerializer :public TypedDataSerializer<StringCollection> {
  StringCollectionSerializer():TypedDataSerializer<StringCollection>("StringCollection") {}

  DataTypeSerializer* clone() const {
    return new StringCollectionSerializer();
  }

  void write(ostream& os, const StringCollection& sc) {
    os << '"';
    std::vector<std::string> values = sc.getValues();

    for(unsigned int i = 0; i < values.size(); ++i) {
      if (i)
        os << ';';

      StringType::write(os, values[i], 0);
    }

    os << '"';
  }

  bool read(istream& is, StringCollection& sc) {
    char c = ' ';

    // go to first '"'
    while((is >> c) && isspace(c)) {}

    if (c != '"')
      return false;

    string str;

    for( ;; ) {
      if( !(is >> c) )
        return false;

      if( c == '"' ) {
        sc.push_back(str);
        return true;
      }

      if (c == ';') {
        sc.push_back(str);
        str.clear();
      }
      else
        str.push_back(c);
    }
  }

  bool setData(tlp::DataSet& dts, const string& prop, const string& val) {
    StringCollection col;

    string::size_type lastPos = val.find_first_not_of(";");
    string::size_type pos = val.find_first_of(";", lastPos);

    while (string::npos != pos || string::npos != lastPos) {
      col.push_back(val.substr(lastPos, pos - lastPos));
      lastPos = val.find_first_not_of(";", pos);
      pos = val.find_first_of(";", lastPos);
    }

    dts.set(prop, col);

    return true;
  }
};

void tlp::initTypeSerializers() {
  DataSet::registerDataTypeSerializer<EdgeSetType::RealType>(KnownTypeSerializer<EdgeSetType>("edgeset"));

  DataSet::registerDataTypeSerializer<DoubleType::RealType>(KnownTypeSerializer<DoubleType>("double"));

  DataSet::registerDataTypeSerializer<FloatType::RealType>(KnownTypeSerializer<FloatType>("float"));

  DataSet::registerDataTypeSerializer<BooleanType::RealType>(KnownTypeSerializer<BooleanType>("bool"));

  DataSet::registerDataTypeSerializer<IntegerType::RealType>(KnownTypeSerializer<IntegerType>("int"));

  DataSet::registerDataTypeSerializer<UnsignedIntegerType::RealType>(KnownTypeSerializer<UnsignedIntegerType>("uint"));

  DataSet::registerDataTypeSerializer<LongType::RealType>(KnownTypeSerializer<LongType>("long"));

  DataSet::registerDataTypeSerializer<ColorType::RealType>(KnownTypeSerializer<ColorType>("color"));

  DataSet::registerDataTypeSerializer<PointType::RealType>(KnownTypeSerializer<PointType>("coord"));

  DataSet::registerDataTypeSerializer<StringType::RealType>(KnownTypeSerializer<StringType>("string"));

  DataSet::registerDataTypeSerializer<DoubleVectorType::RealType>(KnownTypeSerializer<DoubleVectorType>("doublevector"));

  DataSet::registerDataTypeSerializer<BooleanVectorType::RealType>(KnownTypeSerializer<BooleanVectorType>("boolvector"));

  DataSet::registerDataTypeSerializer<IntegerVectorType::RealType>(KnownTypeSerializer<IntegerVectorType>("intvector"));

  DataSet::registerDataTypeSerializer<ColorVectorType::RealType>(KnownTypeSerializer<ColorVectorType>("colorvector"));

  DataSet::registerDataTypeSerializer<LineType::RealType>(KnownTypeSerializer<LineType>("coordvector"));

  DataSet::registerDataTypeSerializer<StringVectorType::RealType>(KnownTypeSerializer<StringVectorType>("stringvector"));

  DataSet::registerDataTypeSerializer<DataSet>(DataSetTypeSerializer());

  DataSet::registerDataTypeSerializer<node>(NodeTypeSerializer());

  DataSet::registerDataTypeSerializer<vector<node> >(NodeVectorTypeSerializer());

  DataSet::registerDataTypeSerializer<edge>(EdgeTypeSerializer());

  DataSet::registerDataTypeSerializer<vector<edge> >(EdgeVectorTypeSerializer());

  DataSet::registerDataTypeSerializer<StringCollection>(StringCollectionSerializer());

}
