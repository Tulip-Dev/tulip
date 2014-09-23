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
#ifndef _PLUGINCONTEXT
#define _PLUGINCONTEXT

#include <tulip/DataSet.h>

namespace tlp {

class Graph;
class PropertyInterface;
class PluginProgress;

/**
 * @ingroup Plugins
 * @brief Contains runtime parameters for a plugin.
 *
 * tlp::PluginContext is a data structure containing a set of parameters required by a plugin.
 * Each tlp::Plugin subclass has its associated tlp::PluginContext subclass that contains specific parameters related to the Plugin.
 * For instance, the tlp::AlgorithmContext (related to tlp::Algorithm) contains the Graph on which the algorithm should be run
 * In essence, you will never have to access to a plugin's context since the base class constructor will copy its members into protected members of the plugin interface.
 */
class PluginContext {
public:
  // Required for dynamic casting
  virtual ~PluginContext() {}
};

/**
 * @ingroup Plugins
 * @brief Parameters structure for a tlp::Algorithm
 *
 * This class contains data required for a tlp::Algorithm to run. Each of its member is copied into a protected member at the object construction.
 */
class AlgorithmContext : public tlp::PluginContext {
public :
  /**
   * @brief The pointer to the tlp::Graph on which the algorithm will be run.
   */
  Graph *graph;

  /**
   * @brief Input parameters set by the user when running the plugin.
   * @see tlp::DataSet
   * @see tlp::WithDependency
   */
  DataSet *dataSet;

  /**
   * @brief A progress handler to notify the user about the progress state of the algorithm when run.
   * @see tlp::PluginProgress
   */
  PluginProgress *pluginProgress;

  AlgorithmContext (tlp::Graph* graph = NULL, tlp::DataSet* dataSet = NULL, tlp::PluginProgress* progress = NULL): graph(graph), dataSet(dataSet), pluginProgress(progress) {}
  ~AlgorithmContext() {}
};


}
#endif
