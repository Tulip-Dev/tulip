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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {

class StringCollection {
%TypeHeaderCode
#include <tulip/StringCollection.h>
%End
public:
  StringCollection();
  StringCollection(const std::vector<std::string> &vectorParam);
%Docstring
tlp.StringCollection(list of string)
%End  
  StringCollection(const std::string param);   
  StringCollection(const std::vector<std::string>&  vectorParam, int currentParam);
%Docstring
tlp.StringCollection(list of string, int)
%End    
  StringCollection(const std::vector<std::string>& vectorParam, std::string currentString);
%Docstring
tlp.StringCollection(list of string, string)
%End    
  std::string getCurrentString();
  bool setCurrent(unsigned int param);
  bool setCurrent(std::string param);
  int  getCurrent();
  void push_back(const std::string& element);
  bool empty();
  std::string at(const unsigned int index);
  unsigned int size();
  std::string operator[](const unsigned int i) const;
  void __setitem__(int i, const std::string &value);
%MethodCode
		(*sipCpp)[a0] = *a1;
%End
};
};

