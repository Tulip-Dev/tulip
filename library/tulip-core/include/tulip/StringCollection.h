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
///@cond DOXYGEN_HIDDEN

#ifndef STRINGCOLLECTION_H
#define STRINGCOLLECTION_H

#include <string>
#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {

/**
 * This class represents a list of selectable string entries that can be used as plugin parameter.
 * The list will appear as a combo box in the Plugin Parameter Dialog from the Tulip GUI.
 */
class TLP_SCOPE StringCollection {

  std::vector<std::string> _data;
  size_t current;

public:

  /**
   * Initializes an empty string collection.
   */
  StringCollection();

  /**
   * Initializes a string collection with strings stored in a vector.
   *
   * @param vectorParam a vector of strings entries for the string collection
   */
  explicit StringCollection(const std::vector<std::string> &vectorParam);

  /**
   * Initializes a string collection from a semicolon separated values string.
   *
   * @param param  a semicolon separated values string ("entry1;...;entryN"). If an entry need to contain a semicolon, you have to escape it the following way : \; .
   */
  explicit StringCollection(const std::string &param);

  /**
   * Initializes a string collection with strings stored in a vector.
   *
   * @param vectorParam a vector of strings entries for the string collection
   * @param currentParam the index of the current selected string in the vector
   */
  StringCollection(const std::vector<std::string>&  vectorParam, const int currentParam);

  /**
   * Initializes a string collection with strings stored in a vector.
   *
   * @param vectorParam a vector of strings entries for the string collection
   * @param currentString the current selected string value from the vector
   */
  StringCollection(const std::vector<std::string>& vectorParam, const std::string &currentString);

  /**
  * Returns all the selectable string entries.
  **/
  const std::vector<std::string>& getValues()const;

  /**
   * Returns the current selected string value.
   */
  const std::string& getCurrentString() const;

  /**
   * Sets the current selected string index.
   * Returns true if the provided index is valid.
   *
   * @param param a valid index in the string collection
   */
  bool setCurrent(const unsigned int param);

  /**
   * Sets the current selected string value.
   * Returns true if the string value exists in the collection.
   *
   * @param param a string value from the collection
   */
  bool setCurrent(const std::string param);

  /**
   * Returns the index of the current selected string.
   */
  int getCurrent() const;

  /**
   * Adds a string value to this string collection.
   *
   * @param element the string to add to the collection
   */
  void push_back(const std::string& element) {
    _data.push_back(element);
  }

  /**
   * Returns true if the collection is empty.
   */
  inline bool empty() const {
    return _data.empty();
  }

  /**
   * Returns the string element at a certain index in the collection.
   *
   * @param index a valid index in the collection
   */
  inline std::string at(const size_t index) const {
    return _data.at(index);
  }

  /**
   * Returns the number of strings in the collection.
   */
  inline size_t size() const {
    return _data.size();
  }

  inline std::string operator[](const unsigned int i) const {
    return _data[i];
  }
  inline std::string& operator[](const unsigned int i) {
    return _data[i];
  }
};

}
#endif
///@endcond
