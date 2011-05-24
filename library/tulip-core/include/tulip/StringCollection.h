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
#ifndef STRINGCOLLECTION_H
#define STRINGCOLLECTION_H

#include <string>

#include "tulipconf.h"
#include <vector>

namespace tlp {

struct TLP_SCOPE StringCollection {

private:
  size_t current;
  std::vector<std::string> _data;

public:
  StringCollection();
  explicit StringCollection(const std::vector<std::string> &vectorParam);
  StringCollection(const std::string param);   
  StringCollection(const std::vector<std::string>&  vectorParam, int currentParam);
  StringCollection(const std::vector<std::string>& vectorParam, std::string currentString);
  std::string getCurrentString();
  bool        setCurrent(unsigned int param);
  bool        setCurrent(std::string param);
  int         getCurrent();
  void push_back(const std::string& element) { _data.push_back(element); };
  inline bool empty() { return _data.empty(); }
  inline std::string at(size_t index) { return _data.at(index); }
  inline size_t size() { return _data.size(); }
  inline std::string operator[](const unsigned int i) const { return _data[i]; }
  inline std::string& operator[](const unsigned int i) { return _data[i];}
};

}
#endif
