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
#include <string>
#include <tulip/PropertyTypes.h>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/Color.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

#include <tulip/Reflect.h>

using namespace std;
using namespace tlp;

DataSet::DataSet(const DataSet &set) {
  *this = set;
}

DataSet & DataSet::operator=(const DataSet &set) {
  if (this != &set) {
    data.clear();
    for (std::list< std::pair<std::string, tlp::DataType*> >::const_iterator it =
	   set.data.begin(); it != set.data.end(); ++it) {
      data.push_back(std::pair<std::string, tlp::DataType*>((*it).first, (*it).second->clone()));
    }
  }
  return *this;
}

DataSet::~DataSet() {
  for (std::list< std::pair<std::string, tlp::DataType*> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    if (it->second)
      delete it->second;
  }
}

bool DataSet::exist(const string &str) const {
  for (std::list< std::pair<std::string, tlp::DataType*> >::const_iterator it =
	 data.begin(); it != data.end(); ++it) {
    if ((*it).first == str)
      return true;
  }
  return false;
}

void DataSet::remove(const string &str) {
  for (std::list< std::pair<std::string, tlp::DataType*> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    if ((*it).first == str) {
      if (it->second)
	delete it->second;
      data.erase(it);
      break;
    }
  }
}

DataType* DataSet::getData(const string &str) const {
  for (std::list< std::pair<std::string, tlp::DataType*> >::const_iterator it =
	 data.begin(); it != data.end(); ++it) {
    if ((*it).first == str)
      return it->second ? it->second->clone() : NULL;
  }
  return NULL;
}

void DataSet::setData(const std::string &str, const DataType* value) {
  DataType* val = value ? value->clone() : NULL;
  for (std::list< std::pair<std::string, tlp::DataType*> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType*> &p = *it;
    if (p.first == str) {
      if (p.second)
	delete p.second;
      p.second = val;
      return;
    }
  }
  data.push_back(std::pair<std::string, tlp::DataType*>(str, val));
}

unsigned int DataSet::size() const {
  return (unsigned int) data.size();
}

bool DataSet::empty() const {
  return data.empty();
}

Iterator< pair<string, DataType*> >* DataSet::getValues() const {
  list< pair<string, DataType*> >::const_iterator begin = data.begin();
  list< pair<string, DataType*> >::const_iterator end = data.end();
  
  return new StlIterator<pair<string, DataType*>, list< pair<string, DataType*> >::const_iterator>(begin, end);
}

// management of the serialization
// the 2 hash maps
TLP_HASH_MAP<std::string, DataTypeSerializer*> DataSet::tnTodts;
TLP_HASH_MAP<std::string, DataTypeSerializer*> DataSet::otnTodts;

// registering of a data type serializer
void DataSet::registerDataTypeSerializer(const std::string& typeName,
					 DataTypeSerializer* dts) {
  TLP_HASH_MAP<std::string, DataTypeSerializer*>::iterator it =
    tnTodts.find(typeName);
  if (it != tnTodts.end())
    std::cerr << "Warning: a data type serializer is already registered for mangled type " << typeName << endl;
  it = otnTodts.find(dts->outputTypeName);
  if (it != otnTodts.end())
    std::cerr << "Warning: a data type serializer is already registered for read type " << dts->outputTypeName << endl;
  tnTodts[typeName] = otnTodts[dts->outputTypeName] = dts;
}

// data write
void DataSet::writeData(std::ostream& os, const std::string& prop,
			const DataType* dt) const {
  TLP_HASH_MAP<std::string, DataTypeSerializer*>::iterator it =
    tnTodts.find(dt->getTypeName());
  if (it == tnTodts.end()) {
    std::cerr << "Write error: No data type serializer found for mangled type " <<
      dt->getTypeName() << std::endl;
    return;
  }
  DataTypeSerializer* dts = (*it).second;
  os << '(' << dts->outputTypeName << " \"" << prop << "\" ";
  dts->writeData(os, dt);
  os << ')' << endl;
}

void DataSet::write(std::ostream& os, const DataSet& ds) {
  os << endl;
  // get iterator over pair attribute/value
  Iterator< pair<string, DataType*> > *it = ds.getValues();
  while( it->hasNext() ) {
    pair<string, DataType*> p = it->next();
    ds.writeData(os, p.first, p.second);
  } delete it;
 }

// data read
bool DataSet::readData(std::istream& is, const std::string& prop,
		       const std::string& outputTypeName) {
  TLP_HASH_MAP<std::string, DataTypeSerializer*>::iterator it =
    otnTodts.find(outputTypeName);
  if (it == otnTodts.end()) {
    std::cerr << "Read error: No data type serializer found for read type " <<
      outputTypeName << std::endl;
    return false;
  }
  DataTypeSerializer* dts = (*it).second;
  DataType* dt = dts->readData(is);
  if (dt) {
    // replace any prexisting value associated to prop
    for (std::list< std::pair<std::string, tlp::DataType*> >::iterator it =
	   data.begin(); it != data.end(); ++it) {
      std::pair<std::string, tlp::DataType*> &p = *it;
      if (p.first == prop) {
	if (p.second)
	  delete p.second;
	p.second = dt;
	return true;
      }
    }
    // no prexisting value
    data.push_back(std::pair<std::string, tlp::DataType*>(prop, dt));
    return true;
  }
  return false;
}

bool DataSet::read(std::istream& is, DataSet& ds) {
  is.unsetf(ios_base::skipws);
  for(;;) {
    char c;
    if( !(is >> c) )
      return false;
    if (isspace(c))
      continue;
    if (c == ')') {
      // no open paren at the beginning
      // so the close paren must be read by the caller
      is.unget();
      return true;
    }
    if (c == '(') {
      bool ok;
      // skip spaces before output type name
      while((ok = (is >> c)) && isspace(c)) {}
      if (!ok)
	return false;
      string otn;
      // read output type name until next space char
      do {
	otn.push_back(c);
      } while((ok = (is >> c)) && !isspace(c));
      // skip spaces before prop
      while((ok = (is >> c)) && isspace(c)) {}
      if (!ok)
	return false;
      if (c != '"')
	return false;
      string prop;
      // read prop until next "
      while((ok = (is >> c)) && (c != '"')) {
	prop.push_back(c);
      }
      if (!ok)
	return false;
      // skip spaces before data type
      while((ok = (is >> c)) && isspace(c)) {}
      if (!ok)
	return false;
      is.unget();
      // read data type
      if (!ds.readData(is, prop, otn))
	return false;
      // skip spaces before )
      while((ok = (is >> c)) && isspace(c)) {}
      if (!ok)
	return false;
      if (c != ')')
	return false;
    } else
      return false;
  }
}
  
bool StructDef::hasField(string str) {
  for (std::list< std::pair<std::string, std::string> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    if ((*it).first == str)
      return true;
  }
  return false;
}

void StructDef::erase(string str) {
  for (std::list< std::pair<std::string, std::string> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    if ((*it).first == str) {
      data.erase(it);
      break;
    }
  }
  help.erase( help.find(str) );
  defValue.erase( defValue.find(str) );
}

Iterator< pair<string, string> >* StructDef::getField() const {
  list< pair<string, string> >::const_iterator begin = data.begin();
  list< pair<string, string> >::const_iterator end = data.end();
  
  return new StlIterator<pair<string, string>, list< pair<string, string> >::const_iterator>(begin, end);
}

string StructDef::getHelp( string str ) const {
  map<string,string>::const_iterator it = help.find(str);
  if( it == help.end() )
  	return string();
  else
  	return it->second;
}

string StructDef::getDefValue( string str ) const {
  map<string,string>::const_iterator it = defValue.find(str);
  if( it == defValue.end() )
    return string();
  else
    return it->second;
}

void StructDef::setDefValue(string name, string value) {
  map<string,string>::iterator it = defValue.find(name);
  if( it == defValue.end() )
    defValue[name] = value;
  else
    it->second = value;
}

bool StructDef::isMandatory( string str ) const {
  map<string,bool>::const_iterator it = mandatory.find(str);
  if( it == mandatory.end() )
    return false;
  else
    return it->second;
}

#undef TN
#define	TN( T )	typeid(T).name()

void StructDef::buildDefaultDataSet(DataSet &ioDataSet, Graph *inG) {
  Iterator< pair<string,string> > * defIt;
  defIt = getField();

  while( defIt->hasNext() ) {
    pair<string,string> def;
    def = defIt->next();
    const string & name  = def.first;
    const string & tname = def.second;
    // Already defined ?
    if( ioDataSet.exist(name) )
      continue;
    // Has def value ?
    const string & defv = getDefValue( name );
    if( defv.size() == 0 )
      continue;
    // bool
    if( tname == TN(bool) ){
      bool v;
      bool res = BooleanType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, v );
    }
    // int
    else if( tname == TN(int) ) {
      int v;
      bool res = IntegerType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, v );
    }
    // unsigned int
    else if( tname == TN(unsigned int) ) {
      unsigned int v = atol( defv.c_str() );
      ioDataSet.set( name, v );
    }
    // unsigned int
    else if( tname == TN(long) ) {
      long v = atol( defv.c_str() );
      ioDataSet.set( name, v );
    }
    // fp
    else if( tname == TN(double) ) {
      double v;
      bool res = DoubleType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, (double)v );
    }
    else if( tname == TN(float) ) {
      double v;
      bool res = DoubleType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, (float)v );
    }
    // string
    else if( tname == TN(string) ) {
      ioDataSet.set( name, defv );
    }
    // Color
    else if( tname == TN(Color) ) {
      Color v;
      bool res = ColorType::fromString( v, defv );
      assert( res );
      if( res ) {
	ioDataSet.set( name, v );
	//cout << v;
      }
    }
    // Size
    else if( tname == TN(Size) ) {
      Size v;
      bool res = SizeType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, v );
    } else
      // look for an already existing property
      if (inG && (inG->existProperty(defv))) {
	// BooleanProperty
	if( tname == TN(BooleanProperty) ) {
	  ioDataSet.set( name, inG->getProperty<BooleanProperty>(defv) );
	}
	// DoubleProperty
	else if( tname == TN(DoubleProperty) ) {
	  // look for an already existing property
	  if (inG->existProperty(defv))
	    ioDataSet.set( name, inG->getProperty<DoubleProperty>(defv) );
	}
	// LayoutProperty
	else if( tname == TN(LayoutProperty) ) {
	  ioDataSet.set( name, inG->getProperty<LayoutProperty>(defv) );
	}
	// StringProperty
	else if( tname == TN(StringProperty) ) {
	  ioDataSet.set( name, inG->getProperty<StringProperty>(defv) );
	}
	// IntegerProperty
	else if( tname == TN(IntegerProperty) ) {
	  ioDataSet.set( name, inG->getProperty<IntegerProperty>(defv) );
	}
	// SizeProperty
	else if( tname == TN(SizeProperty) ) {
	  ioDataSet.set( name, inG->getProperty<SizeProperty>(defv) );
	}
	// ColorProperty
	else if( tname == TN(ColorProperty) ) {
	  ioDataSet.set( name, inG->getProperty<ColorProperty>(defv) );
	}
	//
	else if ( tname == TN(PropertyInterface*) ) {
	  ioDataSet.set( name, inG->getProperty(defv) );
	}
      }
  }
  delete defIt;
}

DataTypeSerializer *DataSet::typenameToSerializer(const std::string &name) {
  if (tnTodts.count(name) == 0)
    return NULL;
  return tnTodts[name];
}
