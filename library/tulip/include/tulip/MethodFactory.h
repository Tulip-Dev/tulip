//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef METHODFACTORY_H
#define METHODFACTORY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <string>

#include "Plugin.h"
#include "PluginContext.h"
#include "Sizes.h"
#include "Int.h"
#include "Metric.h"
#include "Colors.h"
#include "Layout.h"
#include "String.h"
#include "Selection.h"
#include "MetaGraph.h"
#include "Clustering.h"
#include "ImportModule.h"
#include "ExportModule.h"
#include "TlpTools.h"

//===========================================================
// Declaclartion of Properties plugin mechanism
//===========================================================
/** \addtogroup plugins */ 
/*@{*/

// Macro for factorization of source code
#define PROPERTYPLUGINFACTORY(T,C,N,A,D,I,V,R)          \
class C##T##Factory:public PropertyFactory<T>           \
{                                                       \
 public:						\
  C##T##Factory(){					\
    T##Proxy::initFactory();				\
    T##Proxy::factory->getPluginParameters((PropertyFactory< T > *) this); \
  }							\
  ~C##T##Factory(){}					\
  std::string getName() const { return std::string(N);}	\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}	\
  std::string getInfo() const {return std::string(I);}	\
  std::string getRelease() const {return std::string(R);}\
  std::string getVersion() const {return std::string(V);}\
 T * createObject(const PropertyContext &context)	\
   {							\
     C *tmp=new C(context);				\
     return ((T *) tmp);				\
   }							\
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define METRICPLUGIN(C,N,A,D,I,V,R)  PROPERTYPLUGINFACTORY(Metric,C,N,A,D,I,V,R)
#define STRINGPLUGIN(C,N,A,D,I,V,R)  PROPERTYPLUGINFACTORY(String,C,N,A,D,I,V,R)
#define SELECTIONPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Selection,C,N,A,D,I,V,R)
#define LAYOUTPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Layout,C,N,A,D,I,V,R)
#define COLORSPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Colors,C,N,A,D,I,V,R)
#define INTPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Int,C,N,A,D,I,V,R)
#define SIZESPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Sizes,C,N,A,D,I,V,R)
#define METAGRAPHPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(MetaGraph,C,N,A,D,I,V,R)

//===========================================================
// Declaclartion of SuperGraph modification plug-in Mechanism
//===========================================================
///
#define SUPERGRAPHPLUGINFACTORY(T,C,N,A,D,I,V,R)	\
class C##T##Factory:public T##Factory                   \
{                                                       \
 public:						\
  C##T##Factory(){					\
    initFactory();					\
    factory->getPluginParameters(this);			\
  }							\
  ~C##T##Factory(){}					\
  std::string getName() const { return std::string(N);}	\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}	\
  std::string getInfo() const {return std::string(I);}	\
  std::string getRelease() const {return std::string(R);}\
  std::string getVersion() const {return std::string(V);}\
 T * createObject(ClusterContext context)		\
   {							\
     C *tmp=new C(context);				\
     return ((T *) tmp);				\
   }							\
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define EXPORTPLUGIN(C,N,A,D,I,V,R) SUPERGRAPHPLUGINFACTORY(ExportModule,C,N,A,D,I,V,R)
#define IMPORTPLUGIN(C,N,A,D,I,V,R) SUPERGRAPHPLUGINFACTORY(ImportModule,C,N,A,D,I,V,R)
#define CLUSTERINGPLUGIN(C,N,A,D,I,V,R) SUPERGRAPHPLUGINFACTORY(Clustering,C,N,A,D,I,V,R)
/*@}*/
#endif
