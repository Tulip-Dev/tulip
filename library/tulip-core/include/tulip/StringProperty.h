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

#ifndef TULIP_STRING_H
#define TULIP_STRING_H

#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>

namespace tlp {

class PropertyContext;

typedef AbstractProperty<tlp::StringType, tlp::StringType> AbstractStringProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::string value to graph elements.
 */
class TLP_SCOPE StringProperty:public AbstractStringProperty {
public :
  StringProperty (Graph *g, const std::string& n="") :AbstractStringProperty(g, n) {}

  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }
  // the value size of a StringVectorProperty is not fixed
  virtual unsigned int nodeValueSize() const {
    return 0;
  }
  virtual unsigned int edgeValueSize() const {
    return 0;
  }
  int compare(const node n1,const node n2)const;
  int compare(const edge e1,const edge e2)const;
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<std::string> value to graph elements.
 */
class TLP_SCOPE StringVectorProperty:public AbstractVectorProperty<tlp::StringVectorType, tlp::StringType> {
public :
  StringVectorProperty(Graph *g, const std::string& n=""):AbstractVectorProperty<StringVectorType, tlp::StringType>(g, n) {}

  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string&);
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }
  // the value size of a StringVectorProperty is not fixed
  virtual unsigned int nodeValueSize() const {
    return 0;
  }
  virtual unsigned int edgeValueSize() const {
    return 0;
  }
};
/*\@}*/
}
#endif
