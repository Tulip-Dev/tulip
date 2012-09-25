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

#include <tulip/DataSet.h>

namespace tlp {

class Graph;
class PropertyInterface;
class PluginProgress;

class PluginContext {
public:
  /**
   * @ingroup Plugins
   * @brief required for dynamic casting
   *
   **/
  virtual ~PluginContext() {}
};

///Parameter for calling clustering plugins
class AlgorithmContext : public tlp::PluginContext {
public :
  ///
  Graph *graph;
  DataSet *dataSet;
  PluginProgress *pluginProgress;
  ///
  AlgorithmContext (tlp::Graph* graph = NULL, tlp::DataSet* dataSet = NULL, tlp::PluginProgress* progress = NULL): graph(graph), dataSet(dataSet), pluginProgress(progress) {}
  ///
  ~AlgorithmContext() {}
};


}
#endif








