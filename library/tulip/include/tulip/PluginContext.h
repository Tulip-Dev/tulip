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


/**
 * @brief This class is used as only parameter when calling Property plug-ins.
 * It contains a Graph on whyich to run the plug-in, a PropertyInterface which will hold the result of the plug-in, and a PluginProgress to give feedback on the plug-in's progress.
 **/
class PropertyContext {
  public :
  ///
  /**
   * @brief The Graph on which the plug-in will run.
   **/
  Graph *graph;

  /**
   * @brief A property the plug-in can modify freely.
   * No other property should be modified.
   **/
  PropertyInterface *propertyProxy;
  /**
   * @brief A means to give feedback on the progress of the plug-in.
   **/
  PluginProgress *pluginProgress;

  /**
   * @brief Additional informations, such as the value of the plug-in's parameters, if any.
   **/
  DataSet *dataSet;
  ///
  PropertyContext ():graph(0),propertyProxy(0),pluginProgress(0),dataSet(0){}
  ///
  ~PropertyContext(){}
};
/*@}*/

}
#endif








