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

%Docstring
This class represents a list of selectable string entries that can be used as plugin parameter.
The list will appear as a combo box in the Plugin Parameter Dialog from the Tulip GUI.
%End

// ===========================================================================

  StringCollection();
  
  StringCollection(const std::vector<std::string> &vectorParam);

  StringCollection(const std::string param);   
  
  StringCollection(const std::vector<std::string>&  vectorParam, int currentParam);

  StringCollection(const std::vector<std::string>& vectorParam, std::string currentString);

// ===========================================================================

  std::string getCurrentString();
%Docstring
tlp.StringCollection.getCurrentString()

Returns the current selected string value.

:rtype: string
%End
  
// ===========================================================================
  
  bool setCurrent(unsigned int param);
%Docstring
tlp.StringCollection.setCurrent(index)

Sets the current selected string index.
Returns :const:`True` if the provided index is valid.

:param index: a valid index in the string collection
:type index: integer
:rtype: boolean
%End

// ===========================================================================
  
  bool setCurrent(std::string param);
%Docstring
tlp.StringCollection.setCurrent(value)

Sets the current selected string value.
Returns :const:`True` if the string value exists in the collection.

:param value: a string value from the collection
:type value: string
:rtype: boolean
%End 
  
// ===========================================================================  
  
  int  getCurrent();
%Docstring
tlp.StringCollection.getCurrent()

Returns the index of the current selected string.

:rtype: integer
%End
  
// ===========================================================================  
  
  void push_back(const std::string& element);
%Docstring
tlp.StringCollection.push_back(element)

Adds a string value to this string collection.

:param element: the string to add to the collection
:type element: string
%End

// ===========================================================================
  
  bool empty();
%Docstring
tlp.StringCollection.empty()

Returns :const:`True` if the collection is empty.

:rtype: boolean
%End

// ===========================================================================
  
  std::string at(const unsigned int index);
%Docstring
tlp.StringCollection.at(index)

Returns the string element at a certain index in the collection.

:param index: a valid index in the collection
:type index: integer
:rtype: string
%End

// ===========================================================================
  
  unsigned int size();
%Docstring
tlp.StringCollection.size()

Returns the number of strings in the collection.

:rtype: integer
%End

// ===========================================================================
  
  std::string operator[](const unsigned int i) const;
%MethodCode
    if (a0 < sipCpp->size()) {
      sipRes = new std::string((*sipCpp)[a0]);
    } else {
      sipIsErr = -1;
      PyErr_SetNone(PyExc_IndexError);
    }
%End
  
  void __setitem__(int i, const std::string &value);
%MethodCode
		(*sipCpp)[a0] = *a1;
%End

};
};

