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
#include <tulip/PluginLister.h>
#include <QtCore/QObject>

class QMainWindow;

namespace tlp {
class TulipProject;

class TLP_QT_SCOPE PerspectiveContext {
public:
  QMainWindow *mainWindow;
};


/**
  @brief Perspective class is the top-most class of the Tulip 4 Perspective system.
  The perspective system replaces the Tulip 3 controller mechanism.
  A perspective plugin is meant to provide a whole GUI application encapsulating a complete business layer, using graphs as backend.

  A perspective plugin is meant to run in its own process and has complete control over it. The host process can be run into two modes: managed and headless.
  Here is a complete workflow of Tulip processes encapsulation leading to the initialization of a Perspective:

  1 => tulip_agent is run. This process verifies that plugins load correctly and display error reports. It also provide GUI facility to manage plugins (fix loading errors, install new plugins, etc)
       The tulip_agent process also provides the user with a list of installed and valid perspectives. When a perspective is chosen, the tulip_agent process run a new "tulip" process that will instantiate the perspective.
       The tulip_agent also provides automatic updates for plugins coming from plugins servers.
  2 => tulip process is run. At this point, we are sure that the plugin environment is valid and that every plugin needed by our perspective loads flawlessly.
       The tulip_process will load plugins once again, then it will parse its arguments:
       @list
       @li If a file is provided as first argument, the tulip process will create a TulipProject object with it and check the associated perspective plugin.
       @li If the --perspective flag is provided, the tulip process will create the corresponding perspective plugin instance without a project.
       @endlist
  3 => The tulip process instantiate a (hidden) QMainWindow with some features (putting tulip icon on it, setting its titlebar, etc)
  4 => The perspective plugin is created and given its QMainWindow. QMainWindow ownership is left to the perspective plugin.
  5 => The construct method is called, allowing the perspective to build-up its GUI on the main window.
  6 => If a file was provided when running the tulip process, the construct method is given a TulipProject * instance as it's first member.

  Perspectives can be run into two modes:
  @list
  @li Managed: The full workflow described above is followed, starting from tulip_center.
  @li Headless: We start directly the tulip process. This mean that plugins environment has not been previously checked for errors.
  @endlist

  Remember that your perspective has complete control over the tulip process. You choose when to stop it or what to do with your QMainWidnow.

  Finally, a Perspective declares several signals. Those signals can be used to communicate with the tulip_agent process. Each signal expose a specific feature of the tulip_agent process.
  Communication between process is hidden and its inner workings should not be known or used by the user.
  */
class TLP_QT_SCOPE Perspective : public QObject, public WithParameter, public WithDependency {
  Q_OBJECT

protected:
  QMainWindow *_mainWindow;
public:

  /**
    @brief Build a perspective along with its context
    */
  Perspective(PerspectiveContext &c);

  /**
    @brief This method checks if a perspective plugin is compatible with a specific TulipProject.
    Tulip4 is meant to be able to hot switch between perspectives using the same input data.
    @return This method should parse the TulipProject files and return true if it can import data from any of those files.
    */
  virtual bool isCompatible(tlp::TulipProject *);

  /**
    @brief Build the perspective UI but does not associate the perspective with any project.
    @warning Since perspective plugins may be instanciated at load time, no operation should be done into the constructor. Use this method instead.
    */
  virtual void construct()=0;

  /**
    @brief Build the perspective UI, associating it with the given project.
    @warning Since perspective plugins may be instanciated at load time, no operation should be done into the constructor. Use this method instead.
    */
  virtual void construct(tlp::TulipProject *)=0;

};

typedef StaticPluginLister<Perspective, PerspectiveContext> PerspectivePluginLister;

#ifdef WIN32
template class TLP_QT_SCOPE PluginLister<Perspective, PerspectiveContext>;
#endif

}

#define PERSPECTIVEPLUGINFACTORY(T,C,N,A,D,I,R,G) \
class C##T##Factory:public tlp::FactoryInterface<tlp::T,tlp::T##Context> \
{                                                       \
public:            \
  C##T##Factory(){          \
    tlp::StaticPluginLister<tlp::T, tlp::T##Context>::registerPlugin(this);     \
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
