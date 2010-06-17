/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <stdio.h>
#include <string>

#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;

class IdLabels:public StringAlgorithm { 
public:
  IdLabels(const PropertyContext &context):StringAlgorithm(context){}
  ~IdLabels(){}
  string getNodeValue(const node n) {
    char tmp[255];
    sprintf(tmp,"%d",n.id);
   return string(tmp);
  }
  string getEdgeValue(const edge e) {
    char tmp[255];
    sprintf(tmp,"%d",e.id);
    return string(tmp);
  }
};
STRINGPLUGIN(IdLabels,"IdLabels","Auber","04/05/2001","","1.0");
