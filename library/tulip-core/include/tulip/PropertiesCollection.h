/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef PROPERTYCOLLECTION_H
#define PROPERTYCOLLECTION_H

#include <string>
#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {

class Graph;

/**
 * This class represents the list of strings (identifying the graph properties)  that can be used as
 * plugin parameter to select some of the properties of a given graph.
 */
class TLP_SCOPE PropertiesCollection {
  std::vector<std::string> _props;
  std::vector<std::string> _selected;

public:
  /**
   * Initializes an empty collection.
   */
  PropertiesCollection() {}

  /**
   * Initializes a collection with all properties of a graph selected
   *
   * @param graph the Graph for which the properties have to be selected
   * @param selectViewProperties a flag indicating if the view properties have to be selected.
   * Default is false
   */
  explicit PropertiesCollection(const Graph *graph, bool selectViewProperties = false);

  /**
   * Returns all the selectable string entries.
   **/
  const std::vector<std::string> &getProperties() const {
    return _props;
  }

  /**
   * Returns the currently selected string entries.
   */
  const std::vector<std::string> &getSelected() const {
    return _selected;
  }

  /**
   * Returns the currently unselected string entries.
   */
  std::vector<std::string> getUnselected() const;

  /**
   * Sets the current selected strings
   * Returns true if the provided selected is a subset of the collection
   *
   * @param selected a valid subset
   */
  bool setSelected(const std::vector<std::string> &selected);

  /**
   * Adds a string value to the collection.
   *
   * @param element the string to add to the collection
   */
  void push_back(const std::string &element) {
    _props.push_back(element);
  }

  /**
   * Returns true if the collection is empty.
   */
  inline bool empty() const {
    return _props.empty();
  }

  /**
   * Returns true if the selected subset is empty.
   */
  inline bool emptySelected() const {
    return _selected.empty();
  }

  /**
   * @brief clear the content of the collection.
   */
  void clear() {
    _props.clear();
    _selected.clear();
  }
};
} // namespace tlp
#endif
///@endcond
