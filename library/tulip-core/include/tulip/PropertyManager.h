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


#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef TLP_PROPERTYMANAGER_H
#define TLP_PROPERTYMANAGER_H

#include <map>
#include <string>

namespace tlp {

template<class itType >
struct Iterator;

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
  bool existProperty(const std::string&) const;
  bool existLocalProperty(const std::string&) const;
  bool existInheritedProperty(const std::string&) const;
  void setLocalProperty(const std::string&, PropertyInterface *);
  bool renameLocalProperty(PropertyInterface *, const std::string&);
  PropertyInterface* getProperty(const std::string&) const;
  PropertyInterface* getLocalProperty(const std::string&) const;
  PropertyInterface* getInheritedProperty(const std::string&) const;
  void delLocalProperty(const std::string&);
  void notifyBeforeDelInheritedProperty(const std::string&);
  void erase(const node );
  void erase(const edge );
  //======================================================================================
  Iterator<std::string>* getLocalProperties();
  Iterator<std::string>* getInheritedProperties();
  Iterator<PropertyInterface*>* getLocalObjectProperties();
  Iterator<PropertyInterface*>* getInheritedObjectProperties();

protected:
  void setInheritedProperty(const std::string&, PropertyInterface *);
};

}

#endif
#endif //DOXYGEN_NOTFOR_DEVEL
///@endcond
