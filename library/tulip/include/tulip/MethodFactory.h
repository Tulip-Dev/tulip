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

#ifndef METHODFACTORY_H
#define METHODFACTORY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <string>

#include <tulip/AbstractPluginInfo.h>
#include <tulip/PluginLister.h>
#include <tulip/TulipRelease.h>
#include <tulip/PluginContext.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>

/** \addtogroup plugins */ 
/*@{*/

//===========================================================
// Declaration of Properties plugin mechanism
//===========================================================
/// Macro for factorization of source code pf Properties plugin mechanism
#define PROPERTYPLUGINFACTORY(T,C,N,A,D,I,R,G)          \
class C##T##Factory:public tlp::FactoryInterface<tlp::T##Algorithm, tlp::PropertyContext>	\
{                                                       \
 public:						\
  C##T##Factory(){					\
    tlp::PropertyPluginLister<tlp::T##Algorithm>::registerPlugin((tlp::FactoryInterface<tlp::T##Algorithm, tlp::PropertyContext> *) this); \
  }							\
  ~C##T##Factory(){}					\
  std::string getClassName() const { return std::string(#T);} \
  std::string getName() const { return std::string(N);}	\
  std::string getGroup() const { return std::string(G);}\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}	\
  std::string getInfo() const {return std::string(I);}	\
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);}\
  tlp::T##Algorithm * createPluginObject(tlp::PropertyContext context)\
   {							\
     C *tmp=new C(context);				\
     return ((tlp::T##Algorithm *) tmp);		\
   }							\
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define BOOLEANPLUGINOFGROUP(C,N,A,D,I,R,G) PROPERTYPLUGINFACTORY(Boolean,C,N,A,D,I,R,G)
#define BOOLEANPLUGIN(C,N,A,D,I,R) BOOLEANPLUGINOFGROUP(C,N,A,D,I,R,"")
#define COLORPLUGINOFGROUP(C,N,A,D,I,R,G) PROPERTYPLUGINFACTORY(Color,C,N,A,D,I,R,G)
#define COLORPLUGIN(C,N,A,D,I,R) COLORPLUGINOFGROUP(C,N,A,D,I,R,"")
#define DOUBLEPLUGINOFGROUP(C,N,A,D,I,R,G)  PROPERTYPLUGINFACTORY(Double,C,N,A,D,I,R,G)
#define DOUBLEPLUGIN(C,N,A,D,I,R)  DOUBLEPLUGINOFGROUP(C,N,A,D,I,R,"")
#define GRAPHPLUGINOFGROUP(C,N,A,D,I,R,G) PROPERTYPLUGINFACTORY(Graph,C,N,A,D,I,R,G)
#define GRAPHPLUGIN(C,N,A,D,I,R) GRAPHPLUGINOFGROUP(C,N,A,D,I,R,"")
#define INTEGERPLUGINOFGROUP(C,N,A,D,I,R,G) PROPERTYPLUGINFACTORY(Integer,C,N,A,D,I,R,G)
#define INTEGERPLUGIN(C,N,A,D,I,R) INTEGERPLUGINOFGROUP(C,N,A,D,I,R,"")
#define LAYOUTPLUGINOFGROUP(C,N,A,D,I,R,G) PROPERTYPLUGINFACTORY(Layout,C,N,A,D,I,R,G)
#define LAYOUTPLUGIN(C,N,A,D,I,R) LAYOUTPLUGINOFGROUP(C,N,A,D,I,R,"")
#define SIZEPLUGINOFGROUP(C,N,A,D,I,R,G) PROPERTYPLUGINFACTORY(Size,C,N,A,D,I,R,G)
#define SIZEPLUGIN(C,N,A,D,I,R) SIZEPLUGINOFGROUP(C,N,A,D,I,R,"")
#define STRINGPLUGINOFGROUP(C,N,A,D,I,R,G)  PROPERTYPLUGINFACTORY(String,C,N,A,D,I,R,G)
#define STRINGPLUGIN(C,N,A,D,I,R)  STRINGPLUGINOF(C,N,A,D,I,R,"")

//===========================================================
// Declaration of Graph modification plug-in Mechanism
//===========================================================
/// Macro for factorization of source code of Graph modification plugin mechanism 
#define GRAPHPLUGINFACTORY(T,C,N,A,D,I,R,G)	\
class C##T##Factory:public tlp::FactoryInterface<tlp::T, tlp::AlgorithmContext>              \
{                                                       \
 public:						\
  C##T##Factory(){					\
    StaticPluginLister<tlp::T, tlp::AlgorithmContext>::registerPlugin(this);			\
  }							\
  ~C##T##Factory(){}					\
  std::string getName() const { return std::string(N);}	\
  std::string getGroup() const { return std::string(G);}\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}	\
  std::string getInfo() const {return std::string(I);}	\
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);}\
  tlp::T * createPluginObject(tlp::AlgorithmContext context)		\
   {							\
     C *tmp=new C(context);				\
     return ((tlp::T *) tmp);				\
   }							\
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define ALGORITHMPLUGINOFGROUP(C,N,A,D,I,R,G) GRAPHPLUGINFACTORY(Algorithm,C,N,A,D,I,R,G)
#define ALGORITHMPLUGIN(C,N,A,D,I,R) ALGORITHMPLUGINOFGROUP(C,N,A,D,I,R,"")
#define EXPORTPLUGINOFGROUP(C,N,A,D,I,R,G) GRAPHPLUGINFACTORY(ExportModule,C,N,A,D,I,R,G)
#define EXPORTPLUGIN(C,N,A,D,I,R) EXPORTPLUGINOFGROUP(C,N,A,D,I,R,"") 
#define IMPORTPLUGINOFGROUP(C,N,A,D,I,R,G) GRAPHPLUGINFACTORY(ImportModule,C,N,A,D,I,R,G)
#define IMPORTPLUGIN(C,N,A,D,I,R) IMPORTPLUGINOFGROUP(C,N,A,D,I,R,"")

#define POINTERCONTEXTPLUGINFACTORY(T,C,N,A,D,I,R,G)     \
class C##T##Factory:public tlp::FactoryInterface<T,T##Context *> \
{                                                \
public:                                          \
  C##T##Factory(){         \
    T##Lister::registerPlugin(this);           \
  }                  \
  std::string getName() const { return std::string(N);}  \
  std::string getGroup() const { return std::string(G);}   \
  std::string getAuthor() const {return std::string(A);}   \
  std::string getDate() const {return std::string(D);}   \
  std::string getInfo() const {return std::string(I);}   \
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);} \
  tlp::T * createPluginObject(tlp::T##Context *)    \
  {            \
    C *tmp = new C();        \
    return ((tlp::T *) tmp);       \
  }            \
  };                                               \
  extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
  }

#define PLUGINWITHIDENTIFIERFACTORY(T,C,N,A,D,I,R,ID,G)     \
class C##T##Factory:public tlp::FactoryInterface<T, T##Context*>   \
{                                                \
public:                                          \
  C##T##Factory(){         \
    T##PluginLister::registerPlugin(this);           \
  }                  \
  std::string getName() const { return std::string(N);}  \
  std::string getGroup() const { return std::string(G);}   \
  std::string getAuthor() const {return std::string(A);}   \
  std::string getDate() const {return std::string(D);}   \
  std::string getInfo() const {return std::string(I);}   \
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);}\
  int    getId() const {return ID;}    \
  tlp::T * createPluginObject(tlp::T##Context *gc)   \
  {            \
    C *tmp = new C(gc);        \
    return ((tlp::T *) tmp);       \
  }            \
  };                                               \
  extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
  }

/*@}*/
#endif
