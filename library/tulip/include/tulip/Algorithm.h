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
#include "tulip/WithParameter.h"
#include "tulip/WithDependency.h"
#include "tulip/Plugin.h"
#include "tulip/TemplateFactory.h"
#include "tulip/MethodFactory.h"

/**
 * \addtogroup plugins
 */ 
namespace tlp {

  class PluginProgress;
  class Graph;
  class DataSet;
  
/*@{*/
///Interface for general algorithm plug-ins
class Algorithm : public WithParameter, public WithDependency
{ 
public :
  Algorithm (AlgorithmContext context):graph(context.graph),pluginProgress(context.pluginProgress),dataSet(context.dataSet){}
  virtual ~Algorithm() {}
  virtual bool run() = 0;
  virtual bool check(std::string &) {return true;}
  virtual void reset() {}  

  Graph *graph;
  PluginProgress *pluginProgress;
  DataSet *dataSet;
};

class AlgorithmFactory:public Plugin{
public:
  static TLP_SCOPE TemplateFactory<AlgorithmFactory, Algorithm,AlgorithmContext > *factory;
  static void initFactory() {
    if (!factory) {
      factory = new TemplateFactory<AlgorithmFactory, Algorithm,AlgorithmContext >;
    }
  }
  virtual ~AlgorithmFactory() {}
  virtual Algorithm * createPluginObject(AlgorithmContext)=0;
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
};
/*@}*/

}
#endif






