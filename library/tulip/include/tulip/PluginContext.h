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

#include <tulip/Reflect.h>

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
  /**
   * @brief The Graph on which the plug-in will run.
   **/
  Graph *graph;
  /**
   * @brief A means to give feedback on the progress of the plug-in.
   **/
  PluginProgress *pluginProgress;
  /**
   * @brief Additional informations, such as the value of the plug-in's parameters, if any.
   **/
  DataSet *dataSet;
  ///
  AlgorithmContext ():
    graph(NULL),pluginProgress(NULL), dataSet(NULL) {}
  ///
  ~AlgorithmContext() {}
};

// for compilation compatibility with 3.6
typedef AlgorithmContext PropertyContext;

/*@}*/

}
#endif








