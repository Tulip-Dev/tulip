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
#include <string>
#include <tulip/PropertyTypes.h>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/Color.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/TlpTools.h>

#include <tulip/DataSet.h>

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

DataTypeSerializerContainer DataSet::serializerContainer;

// registering of a data type serializer
void DataSet::registerDataTypeSerializer(const std::string& typeName,
    DataTypeSerializer* dts) {

#ifndef NDEBUG
  TLP_HASH_MAP<std::string, DataTypeSerializer*>::iterator it =
    serializerContainer.tnTodts.find(typeName);

  if (it != serializerContainer.tnTodts.end())
    tlp::warning() << "Warning: a data type serializer is already registered for type " << demangleClassName(typeName.c_str()).c_str() << std::endl;

  it = serializerContainer.otnTodts.find(dts->outputTypeName);

  if (it != serializerContainer.otnTodts.end())
    tlp::warning() << "Warning: a data type serializer is already registered for read type " << dts->outputTypeName << std::endl;

#endif

  serializerContainer.tnTodts[typeName] = serializerContainer.otnTodts[dts->outputTypeName] = dts;
}

// data write
void DataSet::writeData(std::ostream& os, const std::string& prop,
                        const DataType* dt) const {
  TLP_HASH_MAP<std::string, DataTypeSerializer*>::iterator it =
    serializerContainer.tnTodts.find(dt->getTypeName());

  if (it == serializerContainer.tnTodts.end()) {
#ifndef EMSCRIPTEN
    tlp::warning() << "Write error: No data serializer found for type " << demangleClassName(dt->getTypeName().c_str()).c_str() << std::endl;
#endif
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
  }

  delete it;
}

// data read
bool DataSet::readData(std::istream& is, const std::string& prop,
                       const std::string& outputTypeName) {
  TLP_HASH_MAP<std::string, DataTypeSerializer*>::iterator it =
    serializerContainer.otnTodts.find(outputTypeName);

  if (it == serializerContainer.otnTodts.end()) {
    tlp::warning() << "Read error: No data type serializer found for read type " << outputTypeName << std::endl;
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

    if( !(is >> c) ) {
      return is.eof();
    }

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
      while((ok = bool(is >> c)) && isspace(c)) {}

      if (!ok)
        return false;

      string otn;

      // read output type name until next space char
      do {
        otn.push_back(c);
      }
      while((ok = bool(is >> c)) && !isspace(c));

      // skip spaces before prop
      while((ok = bool(is >> c)) && isspace(c)) {}

      if (!ok)
        return false;

      if (c != '"')
        return false;

      string prop;

      // read prop until next "
      while((ok = bool(is >> c)) && (c != '"')) {
        prop.push_back(c);
      }

      if (!ok)
        return false;

      // skip spaces before data type
      while((ok = bool(is >> c)) && isspace(c)) {}

      if (!ok)
        return false;

      is.unget();

      // read data type
      if (!ds.readData(is, prop, otn))
        return false;

      // skip spaces before )
      while((ok = bool(is >> c)) && isspace(c)) {}

      if (!ok)
        return false;

      if (c != ')')
        return false;

    }
    else
      return false;
  }
}

DataTypeSerializer *DataSet::typenameToSerializer(const std::string &name) {
  if (serializerContainer.tnTodts.count(name) == 0)
    return NULL;

  return serializerContainer.tnTodts[name];
}

string DataSet::toString() const {
  stringstream ss;
  pair<string, DataType*> p;
  forEach(p, getValues()) {
    DataTypeSerializer *serializer = DataSet::typenameToSerializer(p.second->getTypeName());

    if (serializer) {
      ss << "'" << p.first << "'=";
      serializer->writeData(ss, p.second);
      ss << " ";
    }
  }
  return ss.str();
}
