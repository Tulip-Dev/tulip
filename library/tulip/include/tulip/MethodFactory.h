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
#include "SizeAlgorithm.h"
#include "IntegerAlgorithm.h"
#include "DoubleAlgorithm.h"
#include "ColorAlgorithm.h"
#include "LayoutAlgorithm.h"
#include "StringAlgorithm.h"
#include "BooleanAlgorithm.h"
#include "GraphAlgorithm.h"
#include "Clustering.h"
#include "ImportModule.h"
#include "ExportModule.h"

//===========================================================
// Declaclartion of Properties plugin mechanism
//===========================================================
/** \addtogroup plugins */ 
/*@{*/

// Macro for factorization of source code
#define PROPERTYPLUGINFACTORY(T,C,N,A,D,I,V,R)          \
class C##T##Factory:public tlp::PropertyFactory<tlp::T##Algorithm>	\
{                                                       \
 public:						\
  C##T##Factory(){					\
    tlp::T##Property::initFactory();			\
    tlp::T##Property::factory->getPluginParameters((tlp::PropertyFactory<tlp::T##Algorithm> *) this); \
  }							\
  ~C##T##Factory(){}					\
  std::string getName() const { return std::string(N);}	\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}	\
  std::string getInfo() const {return std::string(I);}	\
  std::string getRelease() const {return std::string(R);}\
  std::string getVersion() const {return std::string(V);}\
 tlp::T##Algorithm * createObject(const tlp::PropertyContext &context)	\
   {							\
     C *tmp=new C(context);				\
     return ((tlp::T##Algorithm *) tmp);		\
   }							\
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define INTEGERPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Integer,C,N,A,D,I,V,R)
#define COLORPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Color,C,N,A,D,I,V,R)
#define LAYOUTPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Layout,C,N,A,D,I,V,R)
#define GRAPHPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Graph,C,N,A,D,I,V,R)
#define DOUBLEPLUGIN(C,N,A,D,I,V,R)  PROPERTYPLUGINFACTORY(Double,C,N,A,D,I,V,R)
#define BOOLEANPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Boolean,C,N,A,D,I,V,R)
#define SIZEPLUGIN(C,N,A,D,I,V,R) PROPERTYPLUGINFACTORY(Size,C,N,A,D,I,V,R)
#define STRINGPLUGIN(C,N,A,D,I,V,R)  PROPERTYPLUGINFACTORY(String,C,N,A,D,I,V,R)

//===========================================================
// Declaration of Graph modification plug-in Mechanism
//===========================================================
///
#define GRAPHPLUGINFACTORY(T,C,N,A,D,I,V,R)	\
class C##T##Factory:public tlp::T##Factory              \
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
 tlp::T * createObject(ClusterContext context)		\
   {							\
     C *tmp=new C(context);				\
     return ((tlp::T *) tmp);				\
   }							\
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define EXPORTPLUGIN(C,N,A,D,I,V,R) GRAPHPLUGINFACTORY(ExportModule,C,N,A,D,I,V,R)
#define IMPORTPLUGIN(C,N,A,D,I,V,R) GRAPHPLUGINFACTORY(ImportModule,C,N,A,D,I,V,R)
#define CLUSTERINGPLUGIN(C,N,A,D,I,V,R) GRAPHPLUGINFACTORY(Clustering,C,N,A,D,I,V,R)
/*@}*/
#endif
