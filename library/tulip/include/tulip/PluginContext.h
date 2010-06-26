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

#ifndef _PLUGINCONTEXT
#define _PLUGINCONTEXT

#include "tulip/Reflect.h"

namespace tlp {

class Graph;
class PropertyInterface;
class PluginProgress;

/**
 * \defgroup plugins
 */ 
/*@{*/

///Parameter for calling clustering plugins
class AlgorithmContext {
  public :
  ///
  Graph *graph;
  PluginProgress *pluginProgress;
  DataSet *dataSet;
  ///
  AlgorithmContext ():graph(0),pluginProgress(0), dataSet(0) {}
  ///
  ~AlgorithmContext(){}
};

///Parameter for calling Property plugins
class PropertyContext {
  public :
  ///
  Graph *graph;
  PropertyInterface *propertyProxy;
  PluginProgress *pluginProgress;
  DataSet *dataSet;
  ///
  PropertyContext ():graph(0),propertyProxy(0),pluginProgress(0),dataSet(0){}
  ///
  ~PropertyContext(){}
};
/*@}*/

}
#endif








