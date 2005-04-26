//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _PLUGINCONTEXT
#define _PLUGINCONTEXT

#include "Reflect.h"

class SuperGraph;
class PProxy;
class PluginProgress;
///Parameter for calling clustering plugins

/**
 * \defgroup plugins
 */ 
/*@{*/

class ClusterContext {
  public :
  ///
  SuperGraph *superGraph;
  PluginProgress *pluginProgress;
  DataSet *dataSet;
  ///
  ClusterContext ():superGraph(0),pluginProgress(0), dataSet(0) {}
  ///
  ~ClusterContext(){}
};

///Parameter for calling Property plugins
class PropertyContext {
  public :
  ///
  SuperGraph *superGraph;
  PProxy *propertyProxy;
  PluginProgress *pluginProgress;
  DataSet *dataSet;
  ///
  PropertyContext ():superGraph(0),propertyProxy(0),pluginProgress(0),dataSet(0){}
  ///
  ~PropertyContext(){}
};
/*@}*/
#endif








