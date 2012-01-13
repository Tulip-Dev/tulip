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

#include <QtCore/QObject>
#include <QtGui/QAction>

#include <tulip/TulipRelease.h>
#include <tulip/MethodFactory.h>
#include <tulip/AbstractPluginInfo.h>
#include <tulip/PluginLister.h>
#include <tulip/AbstractPluginInfo.h>

namespace tlp {

class View;

/**
  @class Interactor provides a way to handle user inputs over a view.
  Basically, The interactor class is an overlay to the Qt's event filtering mechanism. It adds several features like the ability to define priorities, custom cursors, etc

  When an interactor is constructed, the following methods are called in this order:
  @li The constructor. This method should almost be a no-op. Interactors may be instantiated by the plugin manager system and doing unneeded operations in the constructor may lead to poor performances when the application loads up.
  @li isCompatible. When creating a View, the application will check for all interactors to see if they could be installed on it. This is done using the isCompatible method.
  @li construct. Initialize the interactor. Since the constructor should be a no-op, initialization code should be put here.
  @li setView. Defines the view this interactor is bound to. The Interactor may keep a reference to the view object to perform specific operations on user inputs.

  Methods listed above are only called once. Once the interactor is initialized, it may be installed/removed several times on different QObjects. It will then repond to user inputs via the eventFilter method
  @see QObject::eventFilter()
  */
class TLP_QT_SCOPE Interactor: public QObject, public tlp::WithParameter, public tlp::WithDependency {
  Q_OBJECT
  Q_PROPERTY(unsigned int priority READ priority)
  Q_PROPERTY(QAction* action READ action)
  Q_PROPERTY(tlp::View* view READ view WRITE setView)
  Q_PROPERTY(QCursor cursor READ cursor)

public:
  /**
    @brief Checks the compatibility between the interactor and the given view (identified by its name).
    If this method returns true, it's very likely that the interactor will be installed on the associated view.
    */
  virtual bool isCompatible(const std::string& viewName)=0;

  /**
    @return the configuration widget used to set up the interactor.
    @warning This method MUST ALWAYS return the same pointer. Doing otherwise may lead to memory leaks.
    @note The configuration widget may be instantiated from the construct method.
    */
  virtual QWidget* configurationWidget() const=0;

  /**
    @return the interactor's priority.
    Priority defines how interactors gets ordered when displayed in the View's toolbar.
    Interactors with the top-most priority value will be displayed at the beginning of the list while lowest priority will be position at the end.
    */
  virtual unsigned int priority() const=0;

  /**
    @return a QAction associated to this interactor.
    This is used by the overleying system to associate an icon/text to the interactor.
    @warning The parent (QObject::parent()) object of this QAction MUST BE the Interactor.
    */
  virtual QAction* action() const=0;

  /**
    @return the View object associated to this Interactor.
    @warning The returned object MUST be the same as the one passed down to the setView method.
    */
  virtual tlp::View* view() const=0;

  /**
    @return The cursor associated to this interactor.
    When the interactor gets active on a view, the View's cursor is changed to what this method returns.
    */
  virtual QCursor cursor() const=0;

  /**
    @brief Builds up the interactor's internal state.
    This method should be used instead of the constructor to initialize the interactor.
    */
  virtual void construct()=0;

public slots:
  /**
    @brief Defines the view object associated to this interactor.
    @warning The view() method MUST ALWAYS return the same pointer as the one passed down to this method.
    */
  virtual void setView(tlp::View*)=0;

  /**
    @brief Install the interactor on the given target
    A call to this method means thatr the interactor should start listening to the target's events and handle them.
    Returning true prevents further handling of the event. Doing otherwise means that the interactor will let following filters to hand over this kind of event.
    */
  virtual void install(QObject* target)=0;

  /**
    @brief Removes the interactor from the previously set target.
    Interactors can be installed on only one target at once.
    */
  virtual void uninstall()=0;

  /**
    @brief Informs the interactor when the undo command (Ctrl+Z) has been triggered
    */
  virtual void undoIsDone()=0;

protected:
  /**
    @brief Provides input filtering for the interactor
    @see QObject::eventFilter()
    */
  inline virtual bool eventFilter(QObject* obj, QEvent* ev) {
    return QObject::eventFilter(obj,ev);
  }
};

class TLP_QT_SCOPE InteractorContext {
};

class TLP_QT_SCOPE InteractorLister: public StaticPluginLister<Interactor, InteractorContext*> {
  static QMap<std::string,QList<std::string> > _compatibilityMap;
public:
  static void initInteractorsDependencies();
  static QList<std::string> compatibleInteractors(const std::string& viewName);
};

#ifdef WIN32
template class TLP_QT_SCOPE PluginLister<Interactor,InteractorContext*>;
#endif

}

#define INTERACTORPLUGINOFGROUP(C,N,A,D,I,R,G) POINTERCONTEXTPLUGINFACTORY(Interactor,C,N,A,D,I,R,G)
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
#define INTERACTORPLUGINVIEWEXTENSION(CN,CNT,BCNT,VCN,A,D,I,R)\
class CN : public tlp::Interactor {\
  tlp::Interactor* _component;\
public:\
  CN() { _component = tlp::InteractorLister::getPluginObject(BCNT); assert(_component != NULL); }\
  bool isCompatible(const std::string& viewName) { return viewName == VCN; }\
  QWidget* configurationWidget() const { return _component->configurationWidget(); }\
  unsigned int priority() const { return _component->priority(); }\
  QAction* action() const { return _component->action(); }\
  tlp::View* view() const { return _component->view(); }\
  QCursor cursor() const { return _component->cursor(); }\
  void construct() { _component->construct(); }\
  void setView(tlp::View* v) { _component->setView(v); }\
  void install(QObject* target) { _component->install(target); }\
  void uninstall() { _component->uninstall(); }\
  void undoIsDone() { _component->undoIsDone(); }\
};\
INTERACTORPLUGIN(CN, CNT, A , D , I , R)

#define INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY(CN,CNT,BCNT,VCN,A,D,I,R,P)     \
class CN : public tlp::Interactor {\
  tlp::Interactor* _component;\
public:\
  CN() { _component = tlp::InteractorLister::getPluginObject(BCNT); assert(_component != NULL); }\
  bool isCompatible(const std::string& viewName) { return viewName == VCN; }\
  QWidget* configurationWidget() const { return _component->configurationWidget(); }\
  unsigned int priority() const { return P; }\
  QAction* action() const { return _component->action(); }\
  tlp::View* view() const { return _component->view(); }\
  QCursor cursor() const { return _component->cursor(); }\
  void construct() { _component->construct(); }\
  void setView(tlp::View* v) { _component->setView(v); }\
  void install(QObject* target) { _component->install(target); }\
  void uninstall() { _component->uninstall(); }\
  void undoIsDone() { _component->undoIsDone(); }\
};\
INTERACTORPLUGIN(CN, CNT, A , D , I , R)

#endif
