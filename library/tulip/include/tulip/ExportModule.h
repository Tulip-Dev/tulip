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
#include "MethodFactory.h"
#include "WithParameter.h"
#include "Reflect.h"

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
/*@}*/
#endif
