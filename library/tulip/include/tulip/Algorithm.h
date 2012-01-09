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
#include <tulip/TulipRelease.h>
#include <tulip/WithParameter.h>
#include <tulip/WithDependency.h>
#include <tulip/PluginContext.h>
#include <tulip/Plugin.h>
#include <tulip/TemplateFactory.h>
#include <tulip/TulipException.h>

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
class Algorithm : public WithParameter, public WithDependency {
public :
  /**
   * @brief Constructs an algorithm and initializes members from the AlgorithmContext.
   *
   * @param context The context this algorithm runs in, containing the graph, a DataSet for the parameters, and a PluginProgress
   * to give feedback to the user about the tasks the algorithm is performing.
   */
  Algorithm(const AlgorithmContext& context):graph(context.graph),pluginProgress(context.pluginProgress),dataSet(context.dataSet) {}
  virtual ~Algorithm() {}
  /**
   * @brief Runs the algorithm on the context that was specified during construction.
   *
   * @return bool Whether the algorithm was successful or not.
   */
  virtual bool run() {
    return true;
  }
  /**
   * @brief Checks if the algorithm can run on the context it was given.
   *
   * @return Whether the algorithm can be applied or not.
   */
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

/**
 * @brief A class for algorithm plug-ins
 * The factory the registers itself in the Tulip plug-in system (through the static initFactory() method when the library is loaded..
 * The actual registration is delegated to a TemplateFactory for code factorization.
 */
class AlgorithmPlugin:public PluginInfoInterface {
public:
  /**
   * @brief A static factory that is initialized when the library is loaded.
   */
  static TLP_SCOPE TemplateFactory<AlgorithmPlugin, Algorithm, AlgorithmContext > *factory;
  /**
   * @brief This static initialization is called when the plug-in library is loaded, and registers the new plug-in in the
   * Tulip plug-in system.
   */
  static void initFactory(bool initing = false) {
    if (!factory) {
      if (initing == false)
        throw TulipException("Error: tlp::initTulipLib() has to be called before loading a Tulip algorithm plugin");

      factory = new TemplateFactory<AlgorithmPlugin, Algorithm, AlgorithmContext >;
    }
  }
  virtual ~AlgorithmPlugin() {}
  /**
   * @brief Creates a new Algorithm object.
   *
   * @return Algorithm A newly created algorithm plug-in.
   */
  virtual Algorithm * createPluginObject(const AlgorithmContext& context)=0;

  virtual std::string getTulipRelease() const {
    return std::string(TULIP_RELEASE);
  }
  virtual  std::string getMajor() const {
    return tlp::getMajor(getRelease());
  }
  virtual  std::string getMinor() const  {
    return tlp::getMinor(getRelease());
  }
  virtual  std::string getTulipMajor() const {
    return tlp::getMajor(getTulipRelease());
  }
  virtual  std::string getTulipMinor() const  {
    return tlp::getMinor(getTulipRelease());
  }
  virtual std::string getClassName() const = 0;
  virtual std::string getName() const = 0;
  virtual std::string getGroup() const = 0;
  virtual std::string getAuthor() const = 0;
  virtual std::string getDate() const = 0;
  virtual std::string getInfo() const = 0;
  virtual std::string getRelease() const = 0;
};
/*@}*/

}
#endif
