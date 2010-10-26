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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/StringAlgorithm.h"

namespace tlp {

class PropertyContext;

  typedef AbstractProperty<tlp::StringType, tlp::StringType, tlp::StringAlgorithm> AbstractStringProperty;
/** \addtogroup properties */ 
/*\@{*/
class TLP_SCOPE StringProperty:public AbstractStringProperty{ 

  friend class StringAlgorithm;

public :
  StringProperty (Graph *g, std::string n="") :AbstractStringProperty(g, n) {}

  PropertyInterface* clonePrototype(Graph *, const std::string&);
};

class TLP_SCOPE StringVectorProperty:public AbstractProperty<tlp::StringVectorType, tlp::StringVectorType>{ 
public :
  StringVectorProperty(Graph *g, std::string n=""):AbstractProperty<StringVectorType, StringVectorType>(g, n) {}

  PropertyInterface* clonePrototype(Graph *, const std::string&);
};
/*\@}*/
}
#endif
