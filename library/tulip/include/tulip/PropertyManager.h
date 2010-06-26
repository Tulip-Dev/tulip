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

#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef TLP_PROPERTYMANAGER_H
#define TLP_PROPERTYMANAGER_H

#include <map>
#include <set>
#include <string>
#include "tulip/Iterator.h"
#include "tulip/Node.h"
#include "tulip/Edge.h"

namespace tlp {

class PropertyInterface;
class Graph;

class PropertyManager {

private:
  std::map<std::string, PropertyInterface*> localProperties;
  std::map<std::string, PropertyInterface*> inheritedProperties;

public:
  Graph *graph;
  explicit  PropertyManager(Graph*);
  ~PropertyManager();
  //======================================================================================
  bool existProperty(const std::string&);
  bool existLocalProperty(const std::string&);
  bool existInheritedProperty(const std::string&);
  void setInheritedProperty(const std::string&, PropertyInterface *);
  void setLocalProperty(const std::string&, PropertyInterface *);
  PropertyInterface* getProperty(const std::string&);
  PropertyInterface* getLocalProperty(const std::string&);
  PropertyInterface* getInheritedProperty(const std::string&);
  void delLocalProperty(const std::string&);
  void delInheritedProperty(const std::string&);
  void erase(const node );
  void erase(const edge );
  //======================================================================================
  Iterator<std::string>* getLocalProperties();
  Iterator<std::string>* getInheritedProperties();
  Iterator<PropertyInterface*>* getLocalObjectProperties();
  Iterator<PropertyInterface*>* getInheritedObjectProperties();
};

}

#endif
#endif //DOXYGEN_NOTFOR_DEVEL
