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
//DataSet implementation
template<typename T> bool tlp::DataSet::get(const std::string& key,T& value) const {
  std::map<std::string, tlp::DataType*>::const_iterator it = data.find(key);
  if(it != data.end()) {
    //no assert can be done here on the type, as we might have stored a DoubleProperty and want only to retrieve a PropertyInterface
    value = *((T*)it->second->value);
  }

  return it != data.end();
}

template<typename T> bool tlp::DataSet::getAndFree(const std::string &key,T& value) {
  std::map<std::string, tlp::DataType*>::iterator it = data.find(key);
  if(it != data.end()) {
    //no assert can be done here on the type, as we might have stored a DoubleProperty and want only to retrieve a PropertyInterface
    value = *((T*)it->second->value);
    delete it->second;
    data.erase(it);
  }
  
  return it != data.end();
}

template<typename T> void tlp::DataSet::set(const std::string &key,
    const T& value) {
  TypedData<T> dtc(new T(value));
  setData(key, &dtc);
}
//=======================================================================
