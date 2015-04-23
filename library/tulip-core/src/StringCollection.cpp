/**
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

#include <tulip/StringCollection.h>

using namespace std;
using namespace tlp;

StringCollection::StringCollection():current(0) {

}

StringCollection::StringCollection(const std::vector<string> &vectorParam)
  : _data(vectorParam),current(0)  {
}

StringCollection::StringCollection(const std::string &param):current(0) {
  string temp;
  string::const_iterator itChar = param.begin();
  bool escapeChar = false;

  while (itChar != param.end()) {
    if (escapeChar) {
      // when it escaped
      // semi colon can be added to the string
      if (*itChar == ';') {
        temp += *itChar;
        escapeChar = false;
        ++itChar;
        continue;
      }
      else {
        escapeChar = false;
        temp += '\\';
      }
    }

    // check string separator
    if (*itChar == ';') {
      escapeChar = false;
      _data.push_back(temp);
      temp = "";
      ++itChar;
      continue;
    }

    // check escape char
    if (*itChar == '\\')
      escapeChar = true;
    else
      temp += *itChar;

    ++itChar;
  }

  if (temp.size())
    _data.push_back(temp);
}

StringCollection::StringCollection(const std::vector<string>&  vectorParam,
                                   const int currentParam)
  : _data(vectorParam) {
  if (currentParam < int(_data.size()))
    current = currentParam;
  else
    current = 0;
}

StringCollection::StringCollection(const std::vector<string>& vectorParam,
                                   const std::string &currentString)
  : _data(vectorParam),current(0) {

  for (vector<string>::const_iterator itS = _data.begin(); itS != _data.end(); ++itS, ++current) {
    if ((*itS) == currentString)
      return;
  }

  current = 0;
}


const std::string& StringCollection::getCurrentString() const {
  if (current < _data.size())
    return _data.at(current);

  static std::string emptyString;
  return emptyString;
}


bool StringCollection::setCurrent(const unsigned int param) {
  if (param < _data.size()) {
    current =  param;
    return true;
  }

  return false;
}

bool StringCollection::setCurrent(const string param) {
  for (unsigned int i = 0; i< _data.size(); ++i) {
    if (_data.at(i) == param ) {
      current = i;
      return true;
    }
  }

  return false;
}

int StringCollection::getCurrent() const {
  return current;
}

const vector<string>& StringCollection::getValues()const {
  return _data;
}
