/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/PluginLister.h>

#include <QAction>
#include <QCursor>
#include <QMap>

#include <string>

class QLabel;

namespace tlp {

static const std::string INTERACTOR_CATEGORY = "Interactor";

class View;

/**
  @ingroup Plugins
  @brief Interactor provides a way to handle user inputs over a view.
  Basically, The interactor class is an overlay to the Qt's event filtering mechanism. It adds
  several features like the ability to define priorities, custom cursors, etc

  When an interactor is constructed, the following methods are called in this order:
  @li The constructor. This method should almost be a no-op. Interactors may be instantiated by the
  plugin manager system and doing unneeded operations in the constructor may lead to poor
  performances when the application loads up.
  @li isCompatible. When creating a View, the application will check for all interactors to see if
  they could be installed on it. This is done using the isCompatible method.
  @li construct. Initialize the interactor. Since the constructor should be a no-op, initialization
  code should be put here.
  @li setView. Defines the view this interactor is bound to. The Interactor may keep a reference to
  the view object to perform specific operations on user inputs.

  Methods listed above are only called once. Once the interactor is initialized, it may be
  installed/removed several times on different QObjects. It will then repond to user inputs via the
  eventFilter method
  @see QObject::eventFilter()
  */
class TLP_QT_SCOPE Interactor : public QObject, public Plugin {
  Q_OBJECT
  Q_PROPERTY(unsigned int priority READ priority)
  Q_PROPERTY(QAction *action READ action)
  Q_PROPERTY(QCursor cursor READ cursor)

public:
  std::string category() const override {
    return INTERACTOR_CATEGORY;
  }
  std::string icon() const override {
    return ":/tulip/gui/icons/32/plugin_interactor.png";
  }
  /**
    @brief Checks the compatibility between the interactor and the given view (identified by its
    name).
    If this method returns true, it's very likely that the interactor will be installed on the
    associated view.
    */
  virtual bool isCompatible(const std::string &viewName) const = 0;

  /**
    @return The interactor documentation.
    @warning This method MUST ALWAYS return the same pointer. Doing otherwise may lead to memory
    leaks.
    @note The interactor document has to be instantiated from the construct method.
    @note It is up to the interactor developer to delete the returned pointer.
    */
  virtual QLabel *configurationDocWidget() const {
    return nullptr;
  }

  /**
    @return the configuration options widget used to set up the interactor.
    @warning This method MUST ALWAYS return the same pointer. Doing otherwise may lead to memory
    leaks.
    @note The configuration widget has to be instantiated from the construct method.
    @note It is up to the interactor developer to delete the configuration widget
    */
  virtual QWidget *configurationOptionsWidget() const {
    return nullptr;
  }

  /**
    @return the interactor's priority.
    Priority defines how interactors gets ordered when displayed in the View's toolbar.
    Interactors with the top-most priority value will be displayed at the beginning of the list
    while lowest priority will be position at the end.
    */
  virtual unsigned int priority() const = 0;

  /**
    @return a QAction associated to this interactor.
    This is used by the overleying system to associate an icon/text to the interactor.
    @warning The parent (QObject::parent()) object of this QAction MUST BE the Interactor.
    */
  virtual QAction *action() const = 0;

  /**
    @return the View object associated to this Interactor.
    @warning The returned object MUST be the same as the one passed down to the setView method.
    */
  virtual tlp::View *view() const = 0;

  /**
    @return The cursor associated to this interactor.
    When the interactor gets active on a view, the View's cursor is changed to what this method
    returns.
    */
  virtual QCursor cursor() const = 0;

  /**
    @brief Builds up the interactor's internal state.
    This method should be used instead of the constructor to initialize the interactor.
    */
  virtual void construct() = 0;

  /**
    @brief this method should be called before setting up the ui
    * of an interactor config widget
    */
  static void setupConfigWidget(QWidget *);

public slots:
  /**
    @brief Defines the view object associated to this interactor.
    @warning The view() method MUST ALWAYS return the same pointer as the one passed down to this
    method.
    */
  virtual void setView(tlp::View *) = 0;

  /**
   * @brief This method is called whenever the context menu is required on the view.
   * @param point The screen coordinates where the context menu should be displayed.
    @return true or false whether the context menu has been shown or not
   */
  virtual bool showContextMenu(const QPoint & /*point*/, const QPointF & /*scenePoint*/) {
    return false;
  }

  /**
    @brief Install the interactor on the given target
    A call to this method means that the interactor should start listening to the target's events
    and handle them.
    Returning true prevents further handling of the event. Doing otherwise means that the interactor
    will let following filters to hand over this kind of event.
    */
  virtual void install(QObject *target) = 0;

  /**
    @brief Removes the interactor from the previously set target.
    Interactors can be installed on only one target at once.
    */
  virtual void uninstall() = 0;

  /**
    @brief Informs the interactor when the undo command (Ctrl+Z) has been triggered
    */
  virtual void undoIsDone() = 0;

protected:
  /**
    @brief Provides input filtering for the interactor
    @see QObject::eventFilter()
    */
  inline bool eventFilter(QObject *obj, QEvent *ev) override {
    return QObject::eventFilter(obj, ev);
  }
};

///@cond DOXYGEN_HIDDEN
/**
 * @ingroup Plugins
 * @brief The InteractorLister class lists compatible interactors for a given tlp::View
 */
class TLP_QT_SCOPE InteractorLister {
  static QMap<std::string, std::list<std::string>> _compatibilityMap;

public:
  static void initInteractorsDependencies();
  static std::list<std::string> compatibleInteractors(const std::string &viewName);
};
///@endcond

} // namespace tlp

#endif
