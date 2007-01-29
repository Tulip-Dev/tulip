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
#include "Algorithm.h"
#include "ImportModule.h"
#include "ExportModule.h"

/** \addtogroup plugins */ 
/*@{*/

//===========================================================
// Declaration of Properties plugin mechanism
//===========================================================
/// Macro for factorization of source code pf Properties plugin mechanism
#define PROPERTYPLUGINFACTORY(T,C,N,A,D,I,V,R,G)          \
class C##T##Factory:public tlp::PropertyFactory<tlp::T##Algorithm>	\
{                                                       \
 public:						\
  C##T##Factory(){					\
    tlp::T##Property::initFactory();			\
    tlp::T##Property::factory->registerPlugin((tlp::PropertyFactory<tlp::T##Algorithm> *) this); \
  }							\
  ~C##T##Factory(){}					\
  std::string getClassName() const { return std::string(#T);} \
  std::string getName() const { return std::string(N);}	\
  std::string getGroup() const { return std::string(G);}\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}	\
  std::string getInfo() const {return std::string(I);}	\
  std::string getRelease() const {return std::string(R);}\
  std::string getVersion() const {return std::string(V);}\
  tlp::T##Algorithm * createPluginObject(const tlp::PropertyContext &context)\
   {							\
     C *tmp=new C(context);				\
     return ((tlp::T##Algorithm *) tmp);		\
   }							\
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define BOOLEANPLUGINOFGROUP(C,N,A,D,I,V,R,G) PROPERTYPLUGINFACTORY(Boolean,C,N,A,D,I,V,R,G)
#define BOOLEANPLUGIN(C,N,A,D,I,V,R) BOOLEANPLUGINOFGROUP(C,N,A,D,I,V,R,"")
#define COLORPLUGINOFGROUP(C,N,A,D,I,V,R,G) PROPERTYPLUGINFACTORY(Color,C,N,A,D,I,V,R,G)
#define COLORPLUGIN(C,N,A,D,I,V,R) COLORPLUGINOFGROUP(C,N,A,D,I,V,R,"")
#define DOUBLEPLUGINOFGROUP(C,N,A,D,I,V,R,G)  PROPERTYPLUGINFACTORY(Double,C,N,A,D,I,V,R,G)
#define DOUBLEPLUGIN(C,N,A,D,I,V,R)  DOUBLEPLUGINOFGROUP(C,N,A,D,I,V,R,"")
#define GRAPHPLUGINOFGROUP(C,N,A,D,I,V,R,G) PROPERTYPLUGINFACTORY(Graph,C,N,A,D,I,V,R,G)
#define GRAPHPLUGIN(C,N,A,D,I,V,R) GRAPHPLUGINOFGROUP(C,N,A,D,I,V,R,"")
#define INTEGERPLUGINOFGROUP(C,N,A,D,I,V,R,G) PROPERTYPLUGINFACTORY(Integer,C,N,A,D,I,V,R,G)
#define INTEGERPLUGIN(C,N,A,D,I,V,R) INTEGERPLUGINOFGROUP(C,N,A,D,I,V,R,"")
#define LAYOUTPLUGINOFGROUP(C,N,A,D,I,V,R,G) PROPERTYPLUGINFACTORY(Layout,C,N,A,D,I,V,R,G)
#define LAYOUTPLUGIN(C,N,A,D,I,V,R) LAYOUTPLUGINOFGROUP(C,N,A,D,I,V,R,"")
#define SIZEPLUGINOFGROUP(C,N,A,D,I,V,R,G) PROPERTYPLUGINFACTORY(Size,C,N,A,D,I,V,R,G)
#define SIZEPLUGIN(C,N,A,D,I,V,R) SIZEPLUGINOFGROUP(C,N,A,D,I,V,R,"")
#define STRINGPLUGINOFGROUP(C,N,A,D,I,V,R,G)  PROPERTYPLUGINFACTORY(String,C,N,A,D,I,V,R,G)
#define STRINGPLUGIN(C,N,A,D,I,V,R)  STRINGPLUGINOF(C,N,A,D,I,V,R,"")

//===========================================================
// Declaration of Graph modification plug-in Mechanism
//===========================================================
/// Macro for factorization of source code of Graph modification plugin mechanism 
#define GRAPHPLUGINFACTORY(T,C,N,A,D,I,V,R,G)	\
class C##T##Factory:public tlp::T##Factory              \
{                                                       \
 public:						\
  C##T##Factory(){					\
    initFactory();					\
    factory->registerPlugin(this);			\
  }							\
  ~C##T##Factory(){}					\
  std::string getName() const { return std::string(N);}	\
  std::string getGroup() const { return std::string(G);}\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}	\
  std::string getInfo() const {return std::string(I);}	\
  std::string getRelease() const {return std::string(R);}\
  std::string getVersion() const {return std::string(V);}\
  tlp::T * createPluginObject(AlgorithmContext context)	\
   {							\
     C *tmp=new C(context);				\
     return ((tlp::T *) tmp);				\
   }							\
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define ALGORITHMPLUGINOFGROUP(C,N,A,D,I,V,R,G) GRAPHPLUGINFACTORY(Algorithm,C,N,A,D,I,V,R,G)
#define ALGORITHMPLUGIN(C,N,A,D,I,V,R) ALGORITHMPLUGINOFGROUP(C,N,A,D,I,V,R,"")
#define EXPORTPLUGINOFGROUP(C,N,A,D,I,V,R,G) GRAPHPLUGINFACTORY(ExportModule,C,N,A,D,I,V,R,G)
#define EXPORTPLUGIN(C,N,A,D,I,V,R) EXPORTPLUGINOFGROUP(C,N,A,D,I,V,R,"") 
#define IMPORTPLUGINOFGROUP(C,N,A,D,I,V,R,G) GRAPHPLUGINFACTORY(ImportModule,C,N,A,D,I,V,R,G)
#define IMPORTPLUGIN(C,N,A,D,I,V,R) IMPORTPLUGINOFGROUP(C,N,A,D,I,V,R,"")
/*@}*/
#endif
