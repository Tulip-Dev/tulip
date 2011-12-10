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


#include <string>

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/PluginContext.h>
#include <tulip/SizeAlgorithm.h>
#include <tulip/IntegerAlgorithm.h>
#include <tulip/DoubleAlgorithm.h>
#include <tulip/ColorAlgorithm.h>
#include <tulip/LayoutAlgorithm.h>
#include <tulip/StringAlgorithm.h>
#include <tulip/BooleanAlgorithm.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>

/** \addtogroup plugins */
/*@{*/

//===========================================================
// Declaration of Algorithm plugin mechanism
//===========================================================
/// Macro for factorization of source code pf Properties plugin mechanism
#define PLUGINFACTORY(T,C,N,A,D,I,R,G)          \
class C##T##Factory:public tlp::AlgorithmPlugin \
{                                               \
 public:            \
  C##T##Factory(){          \
    initFactory();      \
    factory->registerPlugin(this); \
  }             \
  ~C##T##Factory(){}          \
  std::string getClassName() const { return std::string(#T);} \
  std::string getName() const { return std::string(N);} \
  std::string getGroup() const { return std::string(G);}\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}  \
  std::string getInfo() const {return std::string(I);}  \
  std::string getRelease() const {return std::string(R);}\
  tlp::Algorithm * createPluginObject(const tlp::AlgorithmContext &context)\
   {              \
     return new C(context);       \
   }              \
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define BOOLEANPLUGINOFGROUP(C,N,A,D,I,R,G) PLUGINFACTORY(Boolean,C,N,A,D,I,R,G)
#define BOOLEANPLUGIN(C,N,A,D,I,R) BOOLEANPLUGINOFGROUP(C,N,A,D,I,R,"")
#define COLORPLUGINOFGROUP(C,N,A,D,I,R,G) PLUGINFACTORY(Color,C,N,A,D,I,R,G)
#define COLORPLUGIN(C,N,A,D,I,R) COLORPLUGINOFGROUP(C,N,A,D,I,R,"")
#define DOUBLEPLUGINOFGROUP(C,N,A,D,I,R,G)  PLUGINFACTORY(Double,C,N,A,D,I,R,G)
#define DOUBLEPLUGIN(C,N,A,D,I,R)  DOUBLEPLUGINOFGROUP(C,N,A,D,I,R,"")
#define INTEGERPLUGINOFGROUP(C,N,A,D,I,R,G) PLUGINFACTORY(Integer,C,N,A,D,I,R,G)
#define INTEGERPLUGIN(C,N,A,D,I,R) INTEGERPLUGINOFGROUP(C,N,A,D,I,R,"")
#define LAYOUTPLUGINOFGROUP(C,N,A,D,I,R,G) PLUGINFACTORY(Layout,C,N,A,D,I,R,G)
#define LAYOUTPLUGIN(C,N,A,D,I,R) LAYOUTPLUGINOFGROUP(C,N,A,D,I,R,"")
#define SIZEPLUGINOFGROUP(C,N,A,D,I,R,G) PLUGINFACTORY(Size,C,N,A,D,I,R,G)
#define SIZEPLUGIN(C,N,A,D,I,R) SIZEPLUGINOFGROUP(C,N,A,D,I,R,"")
#define STRINGPLUGINOFGROUP(C,N,A,D,I,R,G)  PLUGINFACTORY(String,C,N,A,D,I,R,G)
#define STRINGPLUGIN(C,N,A,D,I,R)  STRINGPLUGINOF(C,N,A,D,I,R,"")

#define ALGORITHMPLUGINOFGROUP(C,N,A,D,I,R,G) PLUGINFACTORY(Algorithm,C,N,A,D,I,R,G)
#define ALGORITHMPLUGIN(C,N,A,D,I,R) ALGORITHMPLUGINOFGROUP(C,N,A,D,I,R,"")

#define OTHERPLUGINFACTORY(T,C,N,A,D,I,R,G) \
class C##T##Factory:public tlp::T##Factory              \
{                                                       \
 public:            \
  C##T##Factory(){          \
    initFactory();          \
    factory->registerPlugin(this);      \
  }             \
  ~C##T##Factory(){}          \
  std::string getName() const { return std::string(N);} \
  std::string getGroup() const { return std::string(G);}\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}  \
  std::string getInfo() const {return std::string(I);}  \
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);}\
  tlp::T * createPluginObject(tlp::AlgorithmContext context)    \
   {              \
     C *tmp=new C(context);       \
     return ((tlp::T *) tmp);       \
   }              \
};                                                      \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define EXPORTPLUGINOFGROUP(C,N,A,D,I,R,G) OTHERPLUGINFACTORY(ExportModule,C,N,A,D,I,R,G)
#define EXPORTPLUGIN(C,N,A,D,I,R) EXPORTPLUGINOFGROUP(C,N,A,D,I,R,"")
#define IMPORTPLUGINOFGROUP(C,N,A,D,I,R,G) OTHERPLUGINFACTORY(ImportModule,C,N,A,D,I,R,G)
#define IMPORTPLUGIN(C,N,A,D,I,R) IMPORTPLUGINOFGROUP(C,N,A,D,I,R,"")
/*@}*/
#endif
