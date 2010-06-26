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
#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <ext/slist>
#include <map>
#include <set>

namespace tlp {

class Observable;

//=========================================================
/**
 * The Observer pattern is a 
 * framework for handling state dependency between observer and observed 
 * object. It is described in the lecture notes and pp293-304 
 * of Design Patterns by Gamma, Helm, Johnson, and Vlissides.
 */
class TLP_SCOPE Observer {
  stdext::slist<Observable *> observables;
  bool updateObservables;
 public:
  Observer(bool manageObservables = true)
   :updateObservables(manageObservables) {}
  virtual ~Observer();
  /**
   * Methods called when a change occur in the observed objects
   * Due to the possibility to differs notificatiosn several objects can
   * send a notify events simultaneously. The iterators given in parameter
   * enable to iterate all these objects.
   */
  virtual void update(std::set<Observable *>::iterator begin ,std::set<Observable *>::iterator end)=0;

  /**
   * Methods called when an observable has been deleted. holdObservers and unHoldObservers function
   * have no effects on this function.
   */
  virtual void observableDestroyed(Observable *) = 0;

  /**
   * This method is call when this observer is add to an observable
   */
  void addObservable(Observable *);

  /**
   * This method is call when this observer is remove from an observable
   */
  void removeObservable(Observable *);
};

typedef std::map<Observer *,std::set<Observable *> > ObserverMap;
typedef std::map<Observable *,std::set<Observer *> > ObservableMap;

//=========================================================
/** All instances of that class can be observed by by an instance of the
  * Observer class. 
  */
class TLP_SCOPE Observable {
  friend class Observer;

 public:
  virtual ~Observable() {removeObservers();}
  /**
   * Register a new observer
   */
  void addObserver(Observer *);
  /**
   * Returns the number of observers
   */
  unsigned int countObservers();
  /**
   * Remove an observer
   */
  void removeObserver(Observer *);
  /**
   * Remove all observers
   */
  void removeObservers();
  /**
   * Notify all the observers
   */
  void notifyObservers();
  /**
   * Notify all the observers that the object will be destroyed. 
   * Need to be called into the destructor of the observable.
   */
  void notifyDestroy();
  /**
   * Queue notifications
   */
  static void holdObservers();
  /**
   * UnQueue notifications
   */
  static void unholdObservers();

 protected:
  static int holdCounter;
  static ObserverMap holdObserverMap;
  static ObservableMap holdObservableMap;
  stdext::slist<Observer*> observersList;
  void removeOnlyObserver(Observer *);
};


inline unsigned int Observable::countObservers() { 
  return observersList.size(); 
}

inline void Observable::removeObserver(Observer *item) {  
  observersList.remove(item);
  item->removeObservable((Observable*)this);
}

inline void Observable::removeOnlyObserver(Observer *item) {
  observersList.remove(item);
}

}

#endif












