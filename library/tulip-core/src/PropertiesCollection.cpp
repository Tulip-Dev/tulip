/**
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

#include <tulip/PropertiesCollection.h>
#include <tulip/Graph.h>
#include <algorithm>

using namespace std;
using namespace tlp;

PropertiesCollection::PropertiesCollection(const Graph *graph, bool selectViewProp) {
  for (const string &propertyName : graph->getProperties()) {
    _props.push_back(propertyName);
    // all user defined properties are selected
    if (selectViewProp || propertyName.find("view") != 0)
      _selected.push_back(propertyName);
  }
  // and sorted in alphabetic order
  std::sort(_selected.begin(), _selected.end());
}

bool PropertiesCollection::setSelected(const std::vector<std::string> &selected) {
  _selected.clear();
  _selected.reserve(selected.size());
  bool found = false;
  for (auto &s : selected) {
    found = false;
    for (auto &d : _props)
      if (s == d) {
        _selected.push_back(s);
        found = true;
        break;
      }
  }
  return found;
}

std::vector<std::string> PropertiesCollection::getUnselected() const {
  std::vector<std::string> unselected;
  for (auto &prop : _props) {
    bool found = false;
    for (auto &s : _selected) {
      if (s == prop) {
        found = true;
        break;
      }
    }
    if (!found)
      unselected.push_back(prop);
  }
  return unselected;
}
