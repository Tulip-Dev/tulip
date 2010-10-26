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

/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef TULIP_PROPERTY_H
#define TULIP_PROPERTY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/PluginContext.h"
#include "tulip/Graph.h"
#include "tulip/Reflect.h"
#include "tulip/WithParameter.h"
#include "tulip/WithDependency.h"

namespace tlp {

/**
 * \addtogroup plugins
 */ 
/*@{*/
///
class TLP_SCOPE PropertyAlgorithm: public WithParameter, public WithDependency {
public :
  ///
  PropertyAlgorithm(const tlp::PropertyContext & context) :
    graph(context.graph),
    pluginProgress(context.pluginProgress),
    dataSet(context.dataSet) {
  }
  ///
  virtual ~PropertyAlgorithm(){}
  ///
  virtual bool run() {return true;}
  ///
  virtual bool check(std::string &) {return true;}
  ///
  bool preservePropertyUpdates(PropertyInterface* prop) {
    return graph->nextPopKeepPropertyUpdates(prop);
  }

protected:
  Graph *graph;
  PluginProgress *pluginProgress;
  DataSet *dataSet;
};
/*@}*/

}
#endif
