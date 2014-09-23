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
#include <tulip/PluginProgress.h>
#include <tulip/PropertyInterface.h>

//================================================================================
template<typename ATTRIBUTETYPE>
bool tlp::Graph::getAttribute(const std::string &name, ATTRIBUTETYPE& value) const {
  return getAttributes().get(name, value);
}
//================================================================================
template<typename ATTRIBUTETYPE>
void tlp::Graph::setAttribute(const std::string &name,const ATTRIBUTETYPE&value) {
  tlp::DataSet &data=getNonConstAttributes();
  notifyBeforeSetAttribute(name);
  data.set(name,value);
  notifyAfterSetAttribute(name);
}
//================================================================================
template<typename PropertyType>
PropertyType* tlp::Graph::getLocalProperty(const std::string &name) {
  if (existLocalProperty(name)) {
    PropertyInterface* prop = getProperty(name);
    assert (dynamic_cast<PropertyType *>(prop)!=NULL);
    return dynamic_cast<PropertyType *>(prop);
  }
  else {
    PropertyType* prop = new PropertyType(this, name);
    this->addLocalProperty(name, prop);
    return prop;
  }
}
//====================================================================================
template<typename PropertyType>
PropertyType* tlp::Graph::getProperty(const std::string &name) {
  if (existProperty(name)) {
    tlp::PropertyInterface* prop = getProperty(name);
    assert (dynamic_cast<PropertyType *>(prop)!=NULL);
    return dynamic_cast<PropertyType *>(prop);
  }
  else {
    return getLocalProperty<PropertyType>(name);
  }
}
