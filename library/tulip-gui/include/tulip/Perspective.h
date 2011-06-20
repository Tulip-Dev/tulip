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
#include <tulip/TulipProject.h>
#include <QtCore/QObject>
#include <QtCore/QVariant>

class QMainWindow;

namespace tlp {

class TLP_QT_SCOPE PerspectiveContext {
public:
  PerspectiveContext(): mainWindow(0), project(0) {}
  QMainWindow *mainWindow;
  TulipProject *project;
  QString externalFile;
  QVariantMap parameters;
};

class TLP_QT_SCOPE Perspective : public QObject, public WithParameter, public WithDependency {
  Q_OBJECT

protected:
  TulipProject *_project;
  QMainWindow *_mainWindow;
  QString _externalFile;
  QVariantMap _parameters;
public:

  Perspective(PerspectiveContext &c);

  virtual ~Perspective();

  virtual bool isCompatible(tlp::TulipProject *);

  virtual void construct()=0;

public slots:

  virtual void terminated() {}

signals:

  void showTulipWelcomeScreen();

  void showTulipPluginsCenter();

  void showTulipAboutPage();

  void createPerspective(QString name, const QVariantMap &parameters = QVariantMap());

  void openProject(QString);

  void openProjectWith(QString,QString,const QVariantMap &parameters = QVariantMap());

  void showOpenProjectWindow();

  void addPluginRepository(QString);

  void removePluginRepository(QString);
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

#define PERSPECTIVEPLUGINOFGROUP(C,N,A,D,I,R,G) PERSPECTIVEPLUGINFACTORY(Perspective,C,N,A,D,I,R,G)
#define PERSPECTIVEPLUGIN(C,N,A,D,I,R) PERSPECTIVEPLUGINOFGROUP(C,N,A,D,I,R,"")

#endif //_PERSPECTIVE_H
