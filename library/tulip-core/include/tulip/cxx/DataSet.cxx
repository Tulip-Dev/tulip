/*
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

//=======================================================================
// DataSet implementation
template <typename T>
bool tlp::DataSet::get(const std::string &str, T &value) const {
  const std::string &key = getUsedName(str);
  for (std::list<std::pair<std::string, tlp::DataType *>>::const_iterator it = data.begin();
       it != data.end(); ++it) {
    if (it->first == key) {
      value = *(static_cast<T *>(it->second->value));
      return true;
    }
  }

  return false;
}

// DataSet implementation
template <typename T>
bool tlp::DataSet::getDeprecated(const std::string &key, const std::string &oldKey,
                                 T &value) const {
  bool found = false;
  for (std::list<std::pair<std::string, tlp::DataType *>>::const_iterator it = data.begin();
       it != data.end(); ++it) {
    // as 'this' DataSet may have been initialized
    // using ParametersDescriptionList::buildDefaultDataSet
    // key and oldKey may be present
    // so check oldKey first
    if (it->first == oldKey) {
      tlp::warning() << "Warning: '" << oldKey.c_str() << "' is a deprecated DataSet key. Use '"
                     << key.c_str() << "' instead." << std::endl;
      value = *(static_cast<T *>(it->second->value));
      return true;
    }
    if (it->first == key) {
      value = *(static_cast<T *>(it->second->value));
      found = true;
    }
  }
  return found;
}

template <typename T>
bool tlp::DataSet::getAndFree(const std::string &str, T &value) {
  const std::string &key = getUsedName(str);
  for (std::list<std::pair<std::string, tlp::DataType *>>::iterator it = data.begin();
       it != data.end(); ++it) {

    if (it->first == key) {
      value = *(static_cast<T *>(it->second->value));
      delete it->second;
      data.erase(it);
      return true;
    }
  }

  return false;
}

template <typename T>
void tlp::DataSet::set(const std::string &key, const T &value) {
  TypedData<T> dtc(new T(value));
  setData(key, &dtc);
}

template <typename T>
void tlp::DataSet::setDeprecated(const std::string &key, const std::string &oldKey,
                                 const T &value) {
  TypedData<T> dtc(new T(value));
  setData(key, &dtc);
  addDeprecated(oldKey, key);
}
//=======================================================================
