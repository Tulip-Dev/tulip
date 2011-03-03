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

#ifndef _IMPORTMODULE_H
#define _IMPORTMODULE_H

#include "tulip/WithParameter.h"
#include "tulip/WithDependency.h"
#include "tulip/Plugin.h"
#include "tulip/TemplateFactory.h"

/** \addtogroup plugins */ 
namespace tlp {
class PluginProgress;
class Graph;
class DataSet;
/*@{*/

/**
* @brief Base class for import plug-ins.
* 
**/
class ImportModule :public WithParameter, public WithDependency
{
public:
  /**
  * @brief Initializes the DataSet to the one passed in the context.
  *
  * @param context THe context this import plug-in runs into.
  **/
  ImportModule (AlgorithmContext context) : 
    graph(context.graph),pluginProgress(context.pluginProgress), dataSet(context.dataSet) {}
  virtual ~ImportModule(){};
  
  /**
  * @brief The import operations should take place here.
  *
  * @param  unused, deprecated parameter
  * @return bool Whether the import was sucessful or not.
  **/
  virtual bool import(const std::string &)=0;
  
  /**
  * @brief The Graph in which to write the data to import.
  **/
  Graph *graph;
  
  /**
  * @brief A means to report progress to the user.
  **/
  PluginProgress *pluginProgress;

  /**
  * @brief A container for the parameters of this import plug-in.
  **/
  DataSet *dataSet;
};

class ImportModuleFactory:public Plugin{
public:
  static TLP_SCOPE TemplateFactory<ImportModuleFactory,ImportModule,AlgorithmContext > *factory;
  static void initFactory() {
    if (!factory) {
      factory = new TemplateFactory<ImportModuleFactory,ImportModule,AlgorithmContext >;
    }
  }    
  virtual ~ImportModuleFactory() {}
  virtual ImportModule * createPluginObject(AlgorithmContext)=0;
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
