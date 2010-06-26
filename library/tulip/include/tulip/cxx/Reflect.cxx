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

//=======================================================================
//Strucdef implementation
template<typename T> void tlp::StructDef::add(const char* str, const char* inHelp,
					      const char* inDefValue, bool isMandatory) {
  std::list< std::pair<std::string, std::string> >::const_iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if ((*it).first == str)
      break;
  }
  if (it == data.end()) {
    data.push_back(std::pair<std::string, std::string>(std::string(str), std::string(typeid(T).name())));
    if (inHelp)
      help[str] = std::string(inHelp);
    if (inDefValue)
      defValue[str] = std::string(inDefValue);
    mandatory[str] = isMandatory;
  }
#ifndef NDEBUG
  else {
    std::cerr << "StructDef::addVar " << str << " already exists" << std::endl;
  }
#endif
}
//=======================================================================
//DataSet implementation
template<typename T> bool tlp::DataSet::get(const std::string& str,T& value) const {
  for (std::list< std::pair<std::string, tlp::DataType*> >::const_iterator it =
	 data.begin(); it != data.end(); ++it) {
    const std::pair<std::string, tlp::DataType*> &p = *it;
    if (p.first == str) {
      value = *((T*) p.second->value);
      return true;
    }
  }
  return false;
}

template<typename T> bool tlp::DataSet::getAndFree(const std::string &str,T& value) {
  for (std::list< std::pair<std::string, tlp::DataType*> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    std::pair<std::string, tlp::DataType *> &p = *it;
    if (p.first == str) {
      value = *((T*) p.second->value);
      delete p.second;
      data.erase(it);
      return true;
    }
  }
  return false;
}

template<typename T>
struct DataTypeContainer :public tlp::DataType {
  DataTypeContainer(void *value, const std::string& str) :DataType(value, str) {}
  ~DataTypeContainer() {
    delete (T*) value;
  }
  DataType* clone() const {
    return new DataTypeContainer<T>(new T(*(T*)value), typeName);
  }
};

template<typename T> void tlp::DataSet::set(const std::string &str,const T& value) {
  DataTypeContainer<T> dtc(new T(value), std::string(typeid(T).name()));
  setData(str, &dtc);
}
//=======================================================================
