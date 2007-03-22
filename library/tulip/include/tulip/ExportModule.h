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

class ExportModuleFactory:public Plugin{
public:
  static TLP_SCOPE TemplateFactory<ExportModuleFactory,ExportModule,AlgorithmContext > *factory;
  static void initFactory() {
    if (!factory) {
      factory = new TemplateFactory<ExportModuleFactory,ExportModule,AlgorithmContext >;
     }
  }    
  virtual ~ExportModuleFactory() {}
  virtual ExportModule * createPluginObject(AlgorithmContext)=0;
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
