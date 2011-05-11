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
 * but WITHOUT ANY WARRANTY; witho  ut even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef _PERSPECTIVE_H
#define _PERSPECTIVE_H

#include <tulip/WithParameter.h>
#include <tulip/WithDependency.h>

namespace tlp {

class TLP_QT_SCOPE Perspective : public WithParameter, public WithDependency {
};

class TLP_QT_SCOPE PerspectiveContext {
};

typedef StaticPluginManager<Perspective, PerspectiveContext> PerspectivePluginManager;

#ifdef WIN32
template class TLP_QT_SCOPE PluginManager<Perspective, PerspectiveContext>;
#endif
}

#define PERSPECTIVEPLUGINFACTORY(T,C,N,A,D,I,R,G) \
class C##T##Factory:public tlp::T##Factory              \
{                                                       \
public:            \
  C##T##Factory(){          \
    StaticPluginManager<tlp::T, tlp::T##Context>::registerPlugin(this);     \
  }             \
  ~C##T##Factory(){}          \
  std::string getName() const { return std::string(N);} \
  std::string getGroup() const { return std::string(G);}\
  std::string getAuthor() const {return std::string(A);}\
  std::string getDate() const {return std::string(D);}  \
  std::string getInfo() const {return std::string(I);}  \
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);}\
  tlp::T * createPluginObject(tlp::T##Context context)    \
  {              \
    C *tmp=new C(context);       \
    return ((tlp::T *) tmp);       \
  }              \
  };                                                      \
  extern "C" {                                            \
    C##T##Factory C##T##FactoryInitializer;               \
  }

#define PERSPECTIVEPLUGIN(C,N,A,D,I,R) PERSPECTIVEPLUGINFACTORY(Perspective,C,N,A,D,I,R,"")

#endif //_PERSPECTIVE_H
