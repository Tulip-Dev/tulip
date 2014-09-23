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

#ifndef INTERACTORCOMPOSITE_H
#define INTERACTORCOMPOSITE_H

#include <tulip/Interactor.h>

#include <QObject>

class QIcon;

namespace tlp {

/**
  @class InteractorComposite represent an event handler stored inside an InteractorComposite

  This is meant to be focused on event handling only. An InteractorComponent should respond to user inputs in its eventFilter method and return true to avoid further propagation of the event to other components.
  This system is inherited from Qt event handling, see QObject::eventFilter() for details.
  */
class TLP_QT_SCOPE InteractorComponent: public QObject {
  Q_OBJECT

  View* _view;
public:

  /**
   * @brief The init() method is called after the component gets installed on a target. Note that this method is called before any event from could have been retrieved.
   */
  virtual void init();

  /**
   * @brief The main event handling method.
   * See QObject::eventFilter for details.
   * @note The target object is the one on which the InteractorComponent has been installed.
   */
  virtual bool eventFilter(QObject*, QEvent*);

  /**
   * @brief This method is called after the component is uninstalled from its target.
   */
  virtual void clear() {}

  /**
   * @brief setView is called when the InteractorComposite is installed on a new view.
   * @see InteractorComposite::setView
   */
  void setView(View* view);

  /**
   * @return The view on which the interactor is installed (NULL if none)
   */
  View* view() const;

  /**
   * @brief A callback method after setView was called.
   */
  virtual void viewChanged(View *) {}
};

/**
  @class InteractorComposite is a subclass of Interactor that allows building Interactors using component classes focused specifically on input handling.

  The composite behaves like a regular interactor. Event handling is made using InteractorComponent subclasses.
  Basically, an InteractorComposite is a list of InteractorComponent. Each of them are meant to be pushed in the list using the push_back and push_front methods.
  When an event is caught by the InteractorComposite, it will iterate over all components (in th order provided by the list) and run eventFilter on each one of them until one of the component returns true.

  When subclassing InteractorComposite, you should push your components in the construct() method (take care of the order they are pushed in).
  Once the setView() method is called (after construct()), the init() method will be run on every components.
  */
class TLP_QT_SCOPE InteractorComposite : public tlp::Interactor {
  Q_OBJECT
  QAction* _action;
  tlp::View* _view;
  QObject* _lastTarget;

protected:
  QList<tlp::InteractorComponent*> _components;
  /**
    @brief backup a QObject into the InteractorComposite.
    This can be used to store the last target the interactor was installed on. This can be used when uninstall is called.
    */
  void setLastTarget(QObject*);

  /**
    @return The last target the interactor was installed on.
    @note If the lastTarget is destroyed before uninstall was called, the lastTarget() method will return NULL.
    */
  QObject* lastTarget() const;

protected slots:
  void lastTargetDestroyed();

public:
  typedef QList<InteractorComponent*>::iterator iterator;
  typedef QList<InteractorComponent*>::const_iterator const_iterator;

  /**
    @brief Default constructor
    @param icon The icon set on the interactor's action
    @param text The text set on the interactor's action
    */
  InteractorComposite(const QIcon& icon, const QString& text="");
  virtual ~InteractorComposite();

  virtual tlp::View* view() const;
  virtual QAction* action() const;
  virtual QCursor cursor() const;

  /**
    @brief Since InteractorComposte behaves like a list of InteractorComponent, this method is here to allow this class to be iterable
    */
  iterator begin();
  /**
    @brief Since InteractorComposte behaves like a list of InteractorComponent, this method is here to allow this class to be iterable
    */
  iterator end();
  /**
    @brief Since InteractorComposte behaves like a list of InteractorComponent, this method is here to allow this class to be iterable
    */
  const_iterator begin() const;
  /**
    @brief Since InteractorComposte behaves like a list of InteractorComponent, this method is here to allow this class to be iterable
    */
  const_iterator end() const;

  /**
    @brief Pushs an InteractorComponent at the end of the list
    */
  void push_back(InteractorComponent* component);

  /**
    @brief Pushs an InteractorComponent at the beggining of the list
    */
  void push_front(InteractorComponent* component);

public slots:
  virtual void undoIsDone();
  virtual void setView(tlp::View* view);
  virtual void install(QObject* target);
  virtual void uninstall();
};

}
#endif // INTERACTORCOMPOSITE_H
