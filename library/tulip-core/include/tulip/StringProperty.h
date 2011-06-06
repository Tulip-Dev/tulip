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
#ifndef TULIP_STRING_H
#define TULIP_STRING_H

#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>
#include <tulip/PropertyAlgorithm.h>

namespace tlp {

class PropertyContext;

  typedef AbstractProperty<tlp::StringType, tlp::StringType, tlp::StringAlgorithm> AbstractStringProperty;
/** \addtogroup properties */ 
/*\@{*/
class TLP_SCOPE StringProperty:public AbstractStringProperty{ 
public :
  StringProperty (Graph *g, std::string n="") :AbstractStringProperty(g, n) {}

  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  static const std::string propertyTypename;
   std::string getTypename() const {
     return propertyTypename;
   }


  /**
    * @brief Specific implementation of AbstractProperty::compare(node n1,node n2)
    **/
  int compare(const node n1,const node n2);

  /**
    * @brief Specific implementation of AbstractProperty::compare(edge e1,edge e2)
    **/
  int compare(const edge e1,const edge e2);
};

  class TLP_SCOPE StringVectorProperty:public AbstractVectorProperty<tlp::StringVectorType, tlp::StringType>{ 
public :
  StringVectorProperty(Graph *g, std::string n=""):AbstractVectorProperty<StringVectorType, tlp::StringType>(g, n) {}

  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  static const std::string propertyTypename;
   std::string getTypename() const {
     return propertyTypename;
   }

};
/*\@}*/
}
#endif
