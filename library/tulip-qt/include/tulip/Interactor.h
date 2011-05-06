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
#ifndef _INTERACTOR_H
#define _INTERACTOR_H

#include <QtCore/qobject.h>
#include <QtGui/QAction>

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/TemplateFactory.h>
#include <tulip/InteractorManager.h>
#include <tulip/Plugin.h>

class QAction;

namespace tlp {

  class GlMainWidget;
  class View;
  class Interactor;

  /** \brief InteractorAction extend QAction to provide Interactor information
   *
   */
  class TLP_QT_SCOPE InteractorAction : public QAction {

    Q_OBJECT;

  public:

    InteractorAction(Interactor *interactor,const QIcon &icon, const QString &text);

    Interactor *getInteractor() {return interactor;}

  protected :

    Interactor *interactor;

  };

  /** \brief Tulip interactor main class
   *
   */
  class TLP_QT_SCOPE Interactor  : public QObject, public WithParameter, public WithDependency {

  protected :

    int priority;
    
    QString configurationWidgetText;

  public:
    /**
     * Default constructor
     */
    Interactor():priority(0) {}

    /*
     * Default destructor
     */
    virtual ~Interactor() {}

    /**
     * Set the view attached with this interactor
     */
    virtual void setView(View *view) = 0;

    /**
     * Install eventFilters of interactor on given widget
     */
    virtual void install(QWidget *) = 0;

    /**
     * Remove eventFilters of interactor
     */
    virtual void remove() = 0;

    /**
     * set html text displayed by configuration widget
     */
    virtual void setConfigurationWidgetText(const QString &text);

    /**
     * return widget of configuration of this interactor
     * if you previouly call setHtmlText, getConfigurationWidget return a QTextEdit with this text
     */
    virtual QWidget *getConfigurationWidget();

    /**
     * return if this interactor is compatible with given View
     */
    virtual bool isCompatible(const std::string &viewName) = 0;

    /**
     * Return the menu display priority
     */
    int getPriority() {
      return priority;
    }

    /**
     * Set the menu display priority
     */
    void setPriority(int number) {
      priority=number;
    }

    /**
     * return QAction of this interactor
     */
    virtual InteractorAction* getAction() = 0;

    /**
     * Compute InteractorComponents include in this interactor
     */
    virtual void compute(GlMainWidget *) = 0;

    /**
     * Draw InteractorComponents include in this interactor
     */
    virtual void draw(GlMainWidget *) = 0;

    /**
     * This function is call when an undo is perform by the controller
     */
    virtual void undoIsDone() {}
  };

  class TLP_QT_SCOPE InteractorContext {
  };

  class TLP_QT_SCOPE InteractorFactory: public tlp::FactoryInterface<Interactor,InteractorContext*> {
  public:
    virtual ~InteractorFactory() {}
  };

}

#define INTERACTORPLUGINFACTORY(T,C,N,A,D,I,R,G)     \
class C##T##Factory:public T##Factory	 \
{                                                \
public:                                          \
  C##T##Factory(){				 \
    initFactory(); 			         \
    factory->registerPlugin(this);	         \
  }       					 \
  std::string getName() const { return std::string(N);}	 \
  std::string getGroup() const { return std::string(G);}	 \
  std::string getAuthor() const {return std::string(A);}	 \
  std::string getDate() const {return std::string(D);}	 \
  std::string getInfo() const {return std::string(I);}	 \
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);} \
  T * createPluginObject(tlp::InteractorContext *)		     \
  {						 \
    C *tmp = new C();				 \
    return ((T *) tmp);			 \
  }						 \
};                                               \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define INTERACTORPLUGINOFGROUP(C,N,A,D,I,R,G) INTERACTORPLUGINFACTORY(Interactor,C,N,A,D,I,R,G)
#define INTERACTORPLUGIN(C,N,A,D,I,R) INTERACTORPLUGINOFGROUP(C,N,A,D,I,R,"")

/*
CN : New interactor class name
CNT : New interactor name
BCNT : Extended interactor name
VCN : View compatibility name
A : Author
D : Date
I : Infor
R : Revision
*/
#define INTERACTORPLUGINVIEWEXTENSION(CN,CNT,BCNT,VCN,A,D,I,R)     \
class CN : public Interactor { \
public : \
CN() {baseInteractor=InteractorManager::getInst().getInteractor(BCNT);if(baseInteractor){setPriority(baseInteractor->getPriority());}} \
  void setView(View *view){if(baseInteractor){baseInteractor->setView(view);}} \
  void install(QWidget *widget){if(baseInteractor){baseInteractor->install(widget);}} \
  void remove(){if(baseInteractor){baseInteractor->remove();}} \
  QWidget *getConfigurationWidget(){if(baseInteractor){return baseInteractor->getConfigurationWidget();}else{return NULL;}} \
  bool isCompatible(const std::string &viewName){if(baseInteractor){return viewName==VCN;}else{return false;}} \
  InteractorAction* getAction(){if(baseInteractor){return baseInteractor->getAction();}else{return NULL;}} \
  void compute(GlMainWidget *widget){if(baseInteractor){baseInteractor->compute(widget);}} \
  void draw(GlMainWidget *widget){if(baseInteractor){baseInteractor->draw(widget);}} \
protected : \
  Interactor *baseInteractor; \
}; \
INTERACTORPLUGIN(CN, CNT, A , D , I , R);

/*
Like INTERACTORPLUGINVIEWEXTENSION(CN,CNT,BCNT,VCN,A,D,I,R) with P
P : Priority of this interactor (in ne menu)
*/
#define INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY(CN,CNT,BCNT,VCN,A,D,I,R,P)     \
class CN : public Interactor { \
public : \
CN() {setPriority(P);baseInteractor=InteractorManager::getInst().getInteractor(BCNT);} \
 void setView(View *view){if(baseInteractor){baseInteractor->setView(view);}} \
 void install(QWidget *widget){if(baseInteractor){baseInteractor->install(widget);}} \
 void remove(){if(baseInteractor){baseInteractor->remove();}} \
 QWidget *getConfigurationWidget(){if(baseInteractor){return baseInteractor->getConfigurationWidget();}else{return NULL;}} \
 bool isCompatible(const std::string &viewName){if(baseInteractor){return viewName==VCN;}else{return false;}} \
 InteractorAction* getAction(){if(baseInteractor){return baseInteractor->getAction();}else{return NULL;}} \
 void compute(GlMainWidget *widget){if(baseInteractor){baseInteractor->compute(widget);}} \
 void draw(GlMainWidget *widget){if(baseInteractor){baseInteractor->draw(widget);}} \
protected : \
  Interactor *baseInteractor; \
}; \
INTERACTORPLUGIN(CN, CNT, A , D , I , R);

#endif






