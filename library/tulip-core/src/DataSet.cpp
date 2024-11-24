/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/TlpTools.h>

#include <tulip/DataSet.h>

using namespace std;
using namespace tlp;

bool DataType::isTulipProperty(const std::string &typeName) {
#define ISPROP(T) typeName.compare(typeid(T).name()) == 0
  return (ISPROP(tlp::BooleanProperty *) || ISPROP(tlp::BooleanVectorProperty *) ||
          ISPROP(tlp::DoubleProperty *) || ISPROP(tlp::DoubleVectorProperty *) ||
          ISPROP(tlp::LayoutProperty *) || ISPROP(tlp::CoordVectorProperty *) ||
          ISPROP(tlp::StringProperty *) || ISPROP(tlp::StringVectorProperty *) ||
          ISPROP(tlp::IntegerProperty *) || ISPROP(tlp::IntegerVectorProperty *) ||
          ISPROP(tlp::SizeProperty *) || ISPROP(tlp::SizeVectorProperty *) ||
          ISPROP(tlp::ColorProperty *) || ISPROP(tlp::ColorVectorProperty *) ||
          ISPROP(tlp::NumericProperty *) || ISPROP(tlp::PropertyInterface *) ||
          ISPROP(tlp::GraphProperty *) || ISPROP(tlp::BooleanProperty) ||
          ISPROP(tlp::BooleanVectorProperty) || ISPROP(tlp::DoubleProperty) ||
          ISPROP(tlp::DoubleVectorProperty) || ISPROP(tlp::LayoutProperty) ||
          ISPROP(tlp::CoordVectorProperty) || ISPROP(tlp::StringProperty) ||
          ISPROP(tlp::StringVectorProperty) || ISPROP(tlp::IntegerProperty) ||
          ISPROP(tlp::IntegerVectorProperty) || ISPROP(tlp::SizeProperty) ||
          ISPROP(tlp::SizeVectorProperty) || ISPROP(tlp::ColorProperty) ||
          ISPROP(tlp::ColorVectorProperty) || ISPROP(tlp::NumericProperty) ||
          ISPROP(tlp::PropertyInterface) || ISPROP(tlp::GraphProperty));
}

DataSet::DataSet(const DataSet &ds) {
  for (const pair<string, DataType *> &p : ds.getValues()) {
    data.emplace_back(p.first, p.second->clone());
  }
  deprecated =
      ds.deprecated ? new std::list<std::pair<std::string, std::string>>(*ds.deprecated) : nullptr;
}

DataSet &DataSet::operator=(const DataSet &set) {
  if (this != &set) {
    data.clear();

    for (auto it = set.data.cbegin(); it != set.data.cend(); ++it) {
      data.emplace_back(it->first, it->second->clone());
    }
  }

  return *this;
}

DataSet::~DataSet() {
  for (auto it = data.begin(); it != data.end(); ++it) {
    delete it->second;
  }
}

const std::string &DataSet::getUsedName(const std::string &oldName) const {
  if (deprecated) {
    for (const std::pair<std::string, std::string> &pp : *deprecated)
      if (pp.first == oldName) {
        tlp::warning() << "Warning: '" << oldName.c_str() << "' is a deprecated DataSet key. Use '"
                       << pp.second.c_str() << "' instead." << std::endl;
        return pp.second;
      }
  }
  return oldName;
}

bool DataSet::exists(const string &str) const {
  for (auto it = data.cbegin(); it != data.cend(); ++it) {
    if (it->first == str)
      return true;
  }

  // check for a deprecated property
  return deprecated && (str != getUsedName(str));
}

void DataSet::addDeprecated(const string &oldName, const string &usedName) {
  // ensure usedName already exists
  assert(exists(usedName));
  if (!deprecated)
    deprecated = new std::list<std::pair<std::string, std::string>>();
#ifndef NDEBUG
  else {
    // ensure an existing deprecated name is not already mapped to usedName
    for (const std::pair<std::string, std::string> &pp : *deprecated) {
      assert(usedName != pp.second);
    }
  }
#endif
  // ensure oldName is not already deprecated
  assert(getUsedName(oldName) == oldName);
  deprecated->emplace_back(oldName, usedName);
}

std::string DataSet::getTypeName(const string &str) const {
  const string &key = getUsedName(str);
  for (auto it = data.cbegin(); it != data.cend(); ++it) {
    if (it->first == key)
      return it->second->getTypeName();
  }

  return std::string();
}

void DataSet::remove(const string &str) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (it->first == str) {
      if (it->second)
        delete it->second;

      data.erase(it);
      break;
    }
  }
}

DataType *DataSet::getData(const string &str) const {
  const string &key = getUsedName(str);
  for (auto it = data.cbegin(); it != data.cend(); ++it) {
    if (it->first == key)
      return it->second ? it->second->clone() : nullptr;
  }

  return nullptr;
}

void DataSet::setData(const std::string &str, const DataType *value) {
  const string &key = getUsedName(str);
  DataType *val = value ? value->clone() : nullptr;

  for (auto it = data.begin(); it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType *> &p = *it;

    if (p.first == key) {
      if (p.second)
        delete p.second;

      p.second = val;
      return;
    }
  }

  data.emplace_back(str, val);
}

unsigned int DataSet::size() const {
  return uint(data.size());
}

bool DataSet::empty() const {
  return data.empty();
}

Iterator<pair<string, DataType *>> *DataSet::getValues() const {
  list<pair<string, DataType *>>::const_iterator begin = data.begin();
  list<pair<string, DataType *>>::const_iterator end = data.end();

  return new StlIterator<pair<string, DataType *>, list<pair<string, DataType *>>::const_iterator>(
      begin, end);
}

// management of the serialization
// the 2 hash maps

DataTypeSerializerContainer DataSet::serializerContainer;

// registering of a data type serializer
void DataSet::registerDataTypeSerializer(const std::string &typeName, DataTypeSerializer *dts) {

#ifndef NDEBUG
  tlp_hash_map<std::string, DataTypeSerializer *>::iterator it =
      serializerContainer.tnTodts.find(typeName);

  if (it != serializerContainer.tnTodts.end())
    tlp::warning() << "Warning: a data type serializer is already registered for type "
                   << demangleClassName(typeName.c_str()).c_str() << std::endl;

  it = serializerContainer.otnTodts.find(dts->outputTypeName);

  if (it != serializerContainer.otnTodts.end())
    tlp::warning() << "Warning: a data type serializer is already registered for read type "
                   << dts->outputTypeName << std::endl;

#endif

  serializerContainer.tnTodts[typeName] = serializerContainer.otnTodts[dts->outputTypeName] = dts;
}

// data write
void DataSet::writeData(std::ostream &os, const std::string &prop, const DataType *dt) const {
  tlp_hash_map<std::string, DataTypeSerializer *>::iterator it =
      serializerContainer.tnTodts.find(dt->getTypeName());

  if (it == serializerContainer.tnTodts.end()) {
    tlp::warning() << "Write error: No data serializer found for type "
                   << demangleClassName(dt->getTypeName().c_str()).c_str() << std::endl;
    return;
  }

  DataTypeSerializer *dts = (*it).second;
  os << '(' << dts->outputTypeName << " \"" << prop << "\" ";
  dts->writeData(os, dt);
  os << ')' << endl;
}

void DataSet::write(std::ostream &os, const DataSet &ds) {
  os << endl;
  // iterate over pair attribute/value
  for (const pair<string, DataType *> &p : ds.getValues()) {
    ds.writeData(os, p.first, p.second);
  }
}

// data read
bool DataSet::readData(std::istream &is, const std::string &prop,
                       const std::string &outputTypeName) {
  auto it = serializerContainer.otnTodts.find(outputTypeName);

  if (it == serializerContainer.otnTodts.end()) {
    tlp::warning() << "Read error: No data type serializer found for read type " << outputTypeName
                   << std::endl;
    return false;
  }

  DataTypeSerializer *dts = (*it).second;
  DataType *dt = dts->readData(is);

  if (dt) {
    // replace any preexisting value associated to prop
    for (auto it = data.begin(); it != data.end(); ++it) {
      std::pair<std::string, tlp::DataType *> &p = *it;

      if (p.first == prop) {
        if (p.second)
          delete p.second;

        p.second = dt;
        return true;
      }
    }

    // no preexisting value
    data.emplace_back(prop, dt);
    return true;
  }

  return false;
}

bool DataSet::read(std::istream &is, DataSet &ds) {
  is.unsetf(ios_base::skipws);

  for (;;) {
    char c;

    if (!(is >> c)) {
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
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok)
        return false;

      string otn;

      // read output type name until next space char
      do {
        otn.push_back(c);
      } while ((ok = bool(is >> c)) && !isspace(c));

      // skip spaces before prop
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok)
        return false;

      if (c != '"')
        return false;

      string prop;

      // read prop until next "
      while ((ok = bool(is >> c)) && (c != '"')) {
        prop.push_back(c);
      }

      if (!ok)
        return false;

      // skip spaces before data type
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok)
        return false;

      is.unget();

      // read data type
      if (!ds.readData(is, prop, otn))
        return false;

      // skip spaces before )
      while ((ok = bool(is >> c)) && isspace(c)) {
      }

      if (!ok)
        return false;

      if (c != ')')
        return false;

    } else
      return false;
  }
}

DataTypeSerializer *DataSet::typenameToSerializer(const std::string &name) {
  if (serializerContainer.tnTodts.count(name) == 0)
    return nullptr;

  return serializerContainer.tnTodts[name];
}

string DataSet::toString() const {
  stringstream ss;
  for (const pair<string, DataType *> &p : getValues()) {
    DataTypeSerializer *serializer = DataSet::typenameToSerializer(p.second->getTypeName());

    if (serializer) {
      ss << "'" << p.first << "'=";
      ss << serializer->toString(p.second).c_str();
      ss << " ";
    } else {
      if (p.second->isTulipProperty()) {
        PropertyInterface *prop = *(static_cast<PropertyInterface **>(p.second->value));
        ss << "'" << p.first << "'=";

        if (prop)
          ss << '"' << prop->getName().c_str() << '"';
        else
          ss << "None";

        ss << " ";
      }
    }
  }
  return ss.str();
}
