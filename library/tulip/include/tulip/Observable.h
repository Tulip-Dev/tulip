#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <list>
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
class Observer {
 public:
  virtual ~Observer() {}
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
};

typedef std::map<Observer *,std::set<Observable *> > ObserverMap;

//=========================================================
/** All instances of that class can be observed by by an instance of the
  * Observer class. 
  */
class TLP_SCOPE Observable {
 public:
  virtual ~Observable() {}
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
   * Need to be call into the dstructor of the observable.
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
  static ObserverMap holdMap;
  std::list<Observer*> observersList;
};


inline void Observable::addObserver(Observer *obs) {
  observersList.push_back(obs); 
}

inline unsigned int Observable::countObservers() { 
  return observersList.size(); 
}

inline void Observable::removeObserver(Observer *item) {  
  observersList.remove(item);
}

inline void Observable::removeObservers() { 
  observersList.clear(); 
}

}

#endif












