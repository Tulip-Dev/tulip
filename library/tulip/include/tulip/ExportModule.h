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

#ifndef _EXPORTMODULE_H
#define _EXPORTMODULE_H

#include <iostream>
#include <tulip/WithParameter.h>
#include <tulip/WithDependency.h>
#include <tulip/Reflect.h>
#include <tulip/AbstractPluginInfo.h>
#include <tulip/PluginManager.h>

/** \addtogroup plugins */ 

namespace tlp {
class Graph;
class PluginProgress;
/*@{*/
/// Interface for exportModule plug-ins
class ExportModule:public WithParameter, public WithDependency
{
public:
  ///
  ExportModule (AlgorithmContext context):graph(context.graph),pluginProgress(context.pluginProgress),dataSet(context.dataSet){}
  ///
  virtual ~ExportModule(){};
  ///
  virtual bool exportGraph(std::ostream &,Graph *)=0;
  /** It is the root graph*/
  Graph *graph;
  ///
  PluginProgress *pluginProgress;
  DataSet *dataSet;
};

class ExportModuleFactory:public FactoryInterface<ExportModule,AlgorithmContext>{
public:
  virtual ~ExportModuleFactory() {}
};

typedef StaticPluginManager<ExportModule,AlgorithmContext> ExportModuleManager;

/*@}*/
}
#endif
