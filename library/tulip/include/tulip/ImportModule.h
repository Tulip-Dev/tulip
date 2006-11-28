//-*-c++-*
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _IMPORTMODULE_H
#define _IMPORTMODULE_H

#include "Graph.h"
#include "WithParameter.h"
#include "WithDependency.h"
#include "Reflect.h"
#include "PluginProgress.h"
#include "Plugin.h"
#include "TemplateFactory.h"


/** \addtogroup plugins */ 
namespace tlp {

/*@{*/
/// Interface for importModule plug-ins
class ImportModule :public WithParameter, public WithDependency
{
public:
  DataSet *dataSet;
  ///
  ImportModule (ClusterContext context) : dataSet(context.dataSet),
    graph(context.graph),pluginProgress(context.pluginProgress) {}
  virtual ~ImportModule(){};
  ///
  virtual bool import(const std::string &)=0;
  /** It is the Graph where the plug-ins should build the imported graph*/
  Graph *graph;
  ///
  PluginProgress *pluginProgress;

  static const char* getClassName() {
    return "ImportModule";
  }
};

class ImportModuleFactory:public Plugin{
public:
  static TLP_SCOPE TemplateFactory<ImportModuleFactory,ImportModule,ClusterContext > *factory;
  static void initFactory() {
    if (!factory) {
      factory = new TemplateFactory<ImportModuleFactory,ImportModule,ClusterContext >;
      factory->currentLoader = 0;
    }
  }    
  virtual ~ImportModuleFactory() {}
  virtual ImportModule * createPluginObject(ClusterContext)=0;
};
/*@}*/
}
#endif
