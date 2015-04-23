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

#ifndef _ALGORITHM_H
#define _ALGORITHM_H

#include <list>
#include <string>
#include <tulip/Plugin.h>
#include <tulip/PluginContext.h>

namespace tlp {

static const std::string ALGORITHM_CATEGORY = "Algorithm";

class PluginProgress;
class Graph;
class DataSet;


/**
 * @ingroup Plugins
 * @brief This abstract class describes a basic algorithm plugin.
 *
 * It inherits on WithParameter and WithDependency for convenience.
 * Basic functionality consists in checking the algorithm can run on the current Graph (e.g. is the graph simple ?), running the algorithm and resetting the algorithm to re-apply it.
 * The algorithm can and should report progress and which task it is performing if it is decomposed in multiple phases (e.g. layouting the graph, coloring it, ...).
 */
class Algorithm : public tlp::Plugin {
public :
  /**
   * @brief Constructs an algorithm and initializes members from the AlgorithmContext.
   *
   * @param context The context this algorithm runs in, containing the graph, a DataSet for the parameters, and a PluginProgress
   * to give feedback to the user about the tasks the algorithm is performing.
   */
  Algorithm (const PluginContext* context) : graph(NULL),pluginProgress(NULL),dataSet(NULL) {
    if(context != NULL) {
      const AlgorithmContext* algorithmContext = dynamic_cast<const AlgorithmContext*>(context);
      assert(algorithmContext != NULL);
      graph = algorithmContext->graph;
      pluginProgress = algorithmContext->pluginProgress;
      dataSet = algorithmContext->dataSet;
    }
  }
  virtual ~Algorithm() {}

  std::string icon() const {
    return ":/tulip/gui/icons/32/plugin_algorithm.png";
  }
  /**
   * @brief Runs the algorithm.
   * It is a good practice to report progress through the PluginProgress, Even if your algorithm is very fast.
   * Keep in mind that Tulip can handle very large graphs.
   * The PluginProgress should also be used to report errors, if any.
   *
   * @return bool Whether the algorithm execution was successful or not.
   **/
  virtual bool run() = 0;

  virtual std::string category() const {
    return ALGORITHM_CATEGORY;
  }
  /**
   * @brief Checks whether the algorithm can be applied on this graph or not.
   * If not, the reason why should be reported through the PluginProgress.
   *
   * @param errorMessage A string whose value will be modified to an error message, if the check fails.
   * @return bool Whether the plug-in can run on this Graph.
   **/
  virtual bool check(std::string &) {
    return true;
  }
  /**
   * @brief The graph this algorithm will be run on. Retrieved from the context at construction.
   */
  Graph *graph;
  /**
   * @brief A PluginProgress to give feedback to the user, retrieved from the context. It can be a NULL pointer, so use with caution.
   */
  PluginProgress *pluginProgress;
  /**
   * @brief A DataSet containing parameters for this algorithm, if any. Retrieved from the context at construction.
   */
  DataSet *dataSet;
};

}
#endif
