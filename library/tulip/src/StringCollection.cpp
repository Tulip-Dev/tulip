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
#include<vector>
#include<tulip/StringCollection.h>

using namespace std;
using namespace tlp;

StringCollection::StringCollection() {
  current = 0;
}
    
StringCollection::StringCollection(const vector<string> &vectorParam) 
  : vector<string>(vectorParam)  {     
  current = 0;
}

StringCollection::StringCollection(const string param) {
  string temp;
  string::const_iterator itChar = param.begin();
  while (itChar != param.end()) {
    if ( *itChar == ';') {
      this->push_back(temp);
      temp = "";
    }
    else 
      temp += *itChar;
    itChar++;        
  }
  if (temp.size())
    this->push_back(temp);
  current = 0;
} 
     
StringCollection::StringCollection(const vector<string>&  vectorParam, 
				   int currentParam)
  : vector<string>(vectorParam) {
  if (currentParam < int(size())) 
    current = currentParam;
  else 
    current = 0;
}

StringCollection::StringCollection(const vector<string>& vectorParam, 
                        string currentString) 
  : vector<string>(vectorParam) {
  current = 0;
  for (vector<string>::const_iterator itS = begin();
       itS != end(); ++itS, ++current) {
    if ((*itS) == currentString)
      return;
  }
  current = 0;
}

    
string StringCollection::getCurrentString() {
  if (current < size())
    return at(current);
  return string();
}


bool StringCollection::setCurrent(unsigned int param) {
  if (param < size()) {
    current =  param;
    return true;
  }
  return false;
}

bool StringCollection::setCurrent(string param) {
  for (unsigned int i = 0; i< size(); i++) {
    if (at(i) == param ) {
      current = i;
      return true;
    }
  }
  return false;
}

int StringCollection::getCurrent() {
  return current;
} 
