/*
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

template<typename T> void tlp::DataSet::set(const std::string &key,
    const T& value) {
  TypedData<T> dtc(new T(value));
  setData(key, &dtc);
}
//=======================================================================
