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

class QMainWindow;

namespace tlp {
/**
  @brief Class representing a Perspective plugin
  The perspective plugin system replaces the old Controller mechanism available in Tulip 3.x. It's been designed to be more flexible and to allow a complete control
  on the user interface.

  Keep in mind that Perspective plugins have been designed to work inside their own processes. Those process might be instanciated from a ROOT PROCESS (tulip_app in case of a
  Perspective run into Tulip or as standalone applications.

  Several signals and slots are available. They are meant to establish a simple communication protocol between the perspective's process and the tulip_app parent process.
  Those elements provide access to common features like asking the tulip_app to open the plugins manager, to show the help screen, to properly close the perspective etc.
  // TODO: add a link to some online documentation about tulip's process encapsulation.
  @see class documentation for an exhaustive list of signals/slots
  @note Those signals and slots are meant to be used when run along with the tulip_app process. If no tulip_app is present, those functions will do nothing.

  Perspective plugins are given a PerspectiveContext when they are initialized. The PerspectiveContext contains the following fields:
  @list
  @li mainWindow (QMainWindow): This is a Qt main window widget which has been partly initialized by the process. This window is initially hidden. The perspective
                                takes this ownership of the main window. Which means that one can delete it if the perspective is not meant to run into a GUI.
  @li document: Still to be defined.
  @endlist
  */
class TLP_QT_SCOPE Perspective : public WithParameter, public WithDependency {
  Q_OBJECT

  /**
    @brief Construct the perspective along with its respective context.
    @warning The context object is not automatically stored and will have to be handled specificly by implementations.
    */
  explicit Perspective(PerspectiveContext &) {}

  signals:
  /**
    @brief Asks the underlying tulip_app to display the welcome screen.
    */
  void showWelcomeScreen();

  /**
    @brief Asks the underlying tulip_app to display the welcome screen.
    */
  void showPluginsCenterScreen();

  /**
    @brief Asks the underlying tulip_app to display the welcome screen.
    */
  void showHelpScreen();

  // TODO: add open new controller function and some stuff to handle documents
};

struct TLP_QT_SCOPE PerspectiveContext {
  QMainWindow *mainWindow;
};

typedef StaticPluginLister<Perspective, PerspectiveContext> PerspectivePluginLister;

#ifdef WIN32
template class TLP_QT_SCOPE PluginLister<Perspective, PerspectiveContext>;
#endif
}

#define PERSPECTIVEPLUGINFACTORY(T,C,N,A,D,I,R,G) \
class C##T##Factory:public tlp::T##Factory              \
{                                                       \
public:            \
  C##T##Factory(){          \
    StaticPluginLister<tlp::T, tlp::T##Context>::registerPlugin(this);     \
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
