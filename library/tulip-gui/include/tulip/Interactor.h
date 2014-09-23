/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/Plugin.h>

#include <QObject>
#include <QCursor>
#include <QMap>

#include <string>

class QAction;

namespace tlp {

static const std::string INTERACTOR_CATEGORY = "Interactor";

class View;

/**
  @ingroup Plugins
  @brief Interactor provides a way to handle user inputs over a view.
  Basically, The interactor class is an overlay to the Qt's event filtering mechanism. It adds several features like the ability to define priorities, custom cursors, etc

  When an interactor is constructed, the following methods are called in this order:
  @li The constructor. This method should almost be a no-op. Interactors may be instantiated by the plugin manager system and doing unneeded operations in the constructor may lead to poor performances when the application loads up.
  @li isCompatible. When creating a View, the application will check for all interactors to see if they could be installed on it. This is done using the isCompatible method.
  @li construct. Initialize the interactor. Since the constructor should be a no-op, initialization code should be put here.
  @li setView. Defines the view this interactor is bound to. The Interactor may keep a reference to the view object to perform specific operations on user inputs.

  Methods listed above are only called once. Once the interactor is initialized, it may be installed/removed several times on different QObjects. It will then repond to user inputs via the eventFilter method
  @see QObject::eventFilter()
  */
class TLP_QT_SCOPE Interactor: public QObject, public Plugin {
  Q_OBJECT
  Q_PROPERTY(unsigned int priority READ priority)
  Q_PROPERTY(QAction* action READ action)
  Q_PROPERTY(tlp::View* view READ view WRITE setView)
  Q_PROPERTY(QCursor cursor READ cursor)

public:
  virtual std::string category() const {
    return INTERACTOR_CATEGORY;
  }
  std::string icon() const {
    return ":/tulip/gui/icons/32/plugin_interactor.png";
  }
  /**
    @brief Checks the compatibility between the interactor and the given view (identified by its name).
    If this method returns true, it's very likely that the interactor will be installed on the associated view.
    */
  virtual bool isCompatible(const std::string& viewName) const=0;

  /**
    @return the configuration widget used to set up the interactor.
    @warning This method MUST ALWAYS return the same pointer. Doing otherwise may lead to memory leaks.
    @note The configuration widget has to be instantiated from the construct method.
    @note It is up to the interactor developper to delete the configuration widget
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

/**
 * @ingroup Plugins
 * @brief The InteractorLister class lists compatible interactors for a given tlp::View
 */
class TLP_QT_SCOPE InteractorLister {
  static QMap<std::string,QList<std::string> > _compatibilityMap;
public:
  static void initInteractorsDependencies();
  static QList<std::string> compatibleInteractors(const std::string& viewName);
};

/**
 * @ingroup Plugins
 * @def INTERACTORPLUGINVIEWEXTENSION(CLASS_NAME,STRING_CLASS_NAME,BASE_INTERACTOR_STRING_NAME,VIEW_STRING_NAME,AUTHOR,DATE,DESCRIPTION,VERSION)
 *
 * @brief Copy an existing Tulip interactor and sets it compatible with a given View.
 *
 * This macro is used when you're making your own View and want to use an existing interactor with it. Interactors are declared to be compatible with a list of View. This macro extends the compatibility of an existing interactor by subclassing it.
 *
 * @note: This macro used the same interactor priority as the base interactor. To define your own priority, see INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY
 *
 * @param CLASS_NAME The name of the interactor class to generate.
 * @param STRING_CLASS_NAME The name of the interactor plugin to generate (see tlp::Plugin::name())
 * @param BASE_INTERACTOR_STRING_NAME The name of the interactor to extend
 * @param VIEW_STRING_NAME The name of the View to set the interactor compatible with
 * @param AUTHOR see tlp::Plugin::author()
 * @param DATE see tlp::Plugin::date()
 * @param DESCRIPTION see tlp::Plugin::info()
 * @param VERSION see tlp::Plugin::version()
 */
#define INTERACTORPLUGINVIEWEXTENSION(CLASS_NAME,STRING_CLASS_NAME,BASE_INTERACTOR_STRING_NAME,VIEW_STRING_NAME,AUTHOR,DATE,DESCRIPTION,VERSION)\
class CLASS_NAME : public tlp::Interactor {\
  mutable tlp::Interactor* _component;\
public:\
  std::string name() const { return std::string(STRING_CLASS_NAME); } \
  std::string author() const { return std::string(AUTHOR); }\
  std::string date() const { return std::string(DATE); }  \
  std::string info() const { return std::string(DESCRIPTION); }  \
  std::string release() const { return std::string(VERSION); }\
  std::string tulipRelease() const { return std::string(TULIP_VERSION); }\
  std::string group() const { return getComponent()->group(); }\
  CLASS_NAME(const PluginContext *):_component(NULL) {}\
  bool isCompatible(const std::string& viewName) const { return viewName == VIEW_STRING_NAME; }\
  QWidget* configurationWidget() const { return getComponent()->configurationWidget(); }\
  unsigned int priority() const { return getComponent()->priority(); }\
  QAction* action() const { return getComponent()->action(); }\
  tlp::View* view() const { return getComponent()->view(); }\
  QCursor cursor() const { return getComponent()->cursor(); }\
  void construct() { getComponent()->construct(); }\
  void setView(tlp::View* v) { getComponent()->setView(v); }\
  void install(QObject* target) { getComponent()->install(target); }\
  void uninstall() { getComponent()->uninstall(); }\
  void undoIsDone() { getComponent()->undoIsDone(); }\
  tlp::Interactor* getComponent() const {\
    if(!_component) {\
      _component = tlp::PluginLister::instance()->getPluginObject<Interactor>(BASE_INTERACTOR_STRING_NAME,NULL); assert(_component != NULL);\
    }\
    return _component;\
  }\
};\
PLUGIN(CLASS_NAME)

/**
 * @ingroup Plugins
 * @def INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY(CLASS_NAME,STRING_CLASS_NAME,BASE_INTERACTOR_STRING_NAME,VIEW_STRING_NAME,AUTHOR,DATE,DESCRIPTION,VERSION,PRIORITY)
 * @brief Similar to INTERACTORPLUGINVIEWEXTENSION but allows to define the generated interactor's priority.
 * @see tlp::Interactor::priority()
 * @see INTERACTORPLUGINVIEWEXTENSION
 */
#define INTERACTORPLUGINVIEWEXTENSIONWITHPRIORITY(CLASS_NAME,STRING_CLASS_NAME,BASE_INTERACTOR_STRING_NAME,VIEW_STRING_NAME,AUTHOR,DATE,DESCRIPTION,VERSION,PRIORITY)     \
class CLASS_NAME : public tlp::Interactor {\
  mutable tlp::Interactor* _component;\
public:\
  std::string name() const { return std::string(STRING_CLASS_NAME); } \
  std::string author() const { return std::string(AUTHOR); }\
  std::string date() const { return std::string(DATE); }  \
  std::string info() const { return std::string(DESCRIPTION); }  \
  std::string release() const { return std::string(VERSION); }\
  std::string tulipRelease() const { return std::string(TULIP_VERSION); }\
  std::string group() const { return getComponent()->group(); }\
  CLASS_NAME(const PluginContext *):_component(NULL) {}\
  bool isCompatible(const std::string& viewName) const { return viewName == VIEW_STRING_NAME; }\
  QWidget* configurationWidget() const { return getComponent()->configurationWidget(); }\
  unsigned int priority() const { return PRIORITY; }\
  QAction* action() const { return getComponent()->action(); }\
  tlp::View* view() const { return getComponent()->view(); }\
  QCursor cursor() const { return getComponent()->cursor(); }\
  void construct() { getComponent()->construct(); }\
  void setView(tlp::View* v) { getComponent()->setView(v); }\
  void install(QObject* target) { getComponent()->install(target); }\
  void uninstall() { getComponent()->uninstall(); }\
  void undoIsDone() { getComponent()->undoIsDone(); }\
  tlp::Interactor* getComponent() const {\
    if(!_component) {\
      _component = tlp::PluginLister::instance()->getPluginObject<Interactor>(BASE_INTERACTOR_STRING_NAME,NULL); assert(_component != NULL);\
    }\
    return _component;\
  }\
};\
PLUGIN(CLASS_NAME)

}

#endif
