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
#ifndef _CLUSTERING_H
#define _CLUSTERING_H

#include <list>
#include <string>
#include <tulip/WithParameter.h>
#include <tulip/WithDependency.h>
#include <tulip/MethodFactory.h>

/**
 * \addtogroup plugins
 */ 
namespace tlp {

  class PluginProgress;
  class Graph;
  class DataSet;
  
/*@{*/
/**
 * @brief This abstract class describes a basic algorithm plugin.
 * It inherits on WithParameter and WithDependency for convenience.
 * Basic functionality consists in checking the algorithm can run on the current Graph (e.g. is the graph simple ?),
 * running the algorithm and resetting the algorithm to re-apply it.
 * The algorithm can and should report progress and which task it is performing if it is decomposed in multiple phases (e.g. layouting the graph, coloring it, ...).
 */
class Algorithm : public WithParameter, public WithDependency
{ 
public :
  /**
   * @brief Constructs an algorithm and initializes members from the AlgorithmContext.
   *
   * @param context The context this algorithm runs in, containing the graph, a dataSet for the arameters, and a pluginProgress
   * to give feedback to the user about the tasks the algorithm is performing.
   */
  Algorithm (AlgorithmContext context):graph(context.graph),pluginProgress(context.pluginProgress),dataSet(context.dataSet){}
  virtual ~Algorithm() {}
  /**
   * @brief Runs the algorithm on the context that was specified during construction.
   *
   * @return bool Whether the algorithm was sucessfull or not.
   */
  virtual bool run() = 0;
  /**
   * @brief Checks if the algorithm can run on the context it was gieven.
   *
   * @return Whether the algorithm can be applied or not.
   */
  virtual bool check(std::string &) {return true;}
  virtual void reset() {}  

  /**
   * @brief The Graph this algorithm will be run on. Retrieved from the context at construction.
   */
  Graph *graph;
  /**
   * @brief A pluginProgress to give feedback to the user, retrieved from the context. It can be a NULL pointer, so use with caution.
   */
  PluginProgress *pluginProgress;
  /**
   * @brief A DataSet containing parameters for this algorithm, if any. Retrived from the context at construction.
   */
  DataSet *dataSet;
};

typedef StaticPluginLister<Algorithm,AlgorithmContext> AlgorithmLister;

#ifdef WIN32
template class TLP_SCOPE PluginLister<Algorithm,AlgorithmContext>;
#endif

/*@}*/

}
#endif
