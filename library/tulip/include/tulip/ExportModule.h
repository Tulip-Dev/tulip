//-*-C++-*
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 31/07/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _EXPORTMODULE_H
#define _EXPORTMODULE_H

#include <iostream>
#include "SuperGraph.h"
#include "WithParameter.h"
#include "Reflect.h"
#include "PluginProgress.h"
#include "Plugin.h"
#include "TemplateFactory.h"


/** \addtogroup plugins */ 
/*@{*/

/// Interface for exportModule plug-ins
class ExportModule:public WithParameter
{
public:
  ///
  ExportModule (ClusterContext context):superGraph(context.superGraph),pluginProgress(context.pluginProgress),dataSet(context.dataSet){}
  ///
  virtual ~ExportModule(){};
  ///
  virtual bool exportGraph(std::ostream &,SuperGraph *)=0;
  /** It is the root superGraph*/
  SuperGraph *superGraph;
  ///
  PluginProgress *pluginProgress;
  DataSet *dataSet;
};

class ExportModuleFactory:public Plugin{
public:
  static TLP_SCOPE TemplateFactory<ExportModuleFactory,ExportModule,ClusterContext > *factory;
  static void initFactory() {
    if (!factory) {
      factory = new TemplateFactory<ExportModuleFactory,ExportModule,ClusterContext >;
      factory->currentLoader = 0;
    }
  }    
  virtual ~ExportModuleFactory() {}
  virtual ExportModule * createObject(ClusterContext)=0;
};

/*@}*/
#endif
