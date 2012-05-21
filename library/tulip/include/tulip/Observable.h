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

#include <set>

// TulipRelease.h is included
// to prevent any misuse of NDEBUG
// while the classes below will be different
// in debug and release modes
#include <tulip/TulipRelease.h>

#include <tulip/TulipException.h>
#include <tulip/ForEach.h>
#include <tulip/vectorgraph.h>

namespace tlp {
/**
  * \addtogroup observable
  */
/*@{*/


class Observable;
//=======================================
/**
  * @class Event
  * @brief Event is the base class for all events used in the Observable/listener/Observer mechanism.
  *
  * Event is the base class to send notification to Observer or Listerner. The nature of an Event is
  * characterized by its type. In the current implemenation there is 4 kind of event :
  *
  * event::DELETE : send directly to all Observers/Listeners, it is not possible to delay its processing.<br>
  * event::MODIFICATION : sent to all Observers/Listeners, it is possible to delay its processing. MODIFACTION are first sent to Observer then to Listenr<br>
  * event::INFORMATION : sent only to Listeners, it is not possible to delay its processing.<br>
  * event::INVALID : never sent use internally for delaying events.<br>
  *
  * @see Listener
  * @see Observer
  * @see Observable
  **/
class  TLP_SCOPE Event {
  friend class Observable;
  friend class Graph;
  friend class PropertyInterface;
public:
  enum EventType {TLP_DELETE = 0, TLP_MODIFICATION, TLP_INFORMATION, TLP_INVALID};
  virtual ~Event();
  Observable* sender() const;
  Event(const Observable &sender, EventType type);
  EventType type() const {
    return _type;
  }
private:
  Event() {}
  tlp::node _sender;
  EventType _type;
};
//=======================================
/**
  * @class ObservableException
  * @brief ObservableException is the base class of all exceptions sent by the Observable/Listener/Observer system.
  *
  * @see Listener
  * @see Observer
  * @see Observable
  **/
class  TLP_SCOPE ObservableException : public tlp::TulipException {
public:
  ObservableException(const std::string &desc):tlp::TulipException(desc) {
  }
};
/**
  * @class Observable
  * @brief Observable is the base class for the implementation of observable Object by Listener or Observer objects.
  *
  * Observable the base class of all objects that need to be Observed or listened. To notify its Onlooker an Observable
  * have to call the notify function whenever it wants. The event will be send directly to all Listener and Observer (if not holded)
  * The Event send by the Observable is a const reference, thus the observer is responsible of the creation and destruction of Event.
  * Usually the Event is created on the stack in the block of code that call the notify function, thus there is no need to take care of it.
  *
  * Observable provides a mechanism (hold/unhold) to delay the sending of events to Observer, the mechanism is global. Calling the hold
  * function will stop the transmission of Event::MODIFICATION to all the Observer, there is no way to delay the sending of events only
  * for one Observable. Nested call to hold/unhold are permitted.
  *
  * The Event::DELETE event cannot be created by user, it is automatically send to all Observer/Listener at the destruction of an Observable.
  * However, using the ObservableDeleted function it is possible to send the Event from the destructor of a specific Observable implementation.
  * The update function of observer/Listener can the be called before the destruction of your internal objects. It enables the Observer/Listener, to
  * to access to the internal data of your Observable before it disappear.
  *
  * @see Listener
  * @see Observer
  * @see Event
  **/

//=======================================
/**
  * @class ObservableObject
  * @brief ObservableObject is the base class of the Observable/Listener/Observer object hierachy. It manages storage of a unique id for these objects as well
  *        as the storage of relations between them. It also manages destruction of these objects.
  *
  * Each Observable/listener/Observer is stored in a VectorGraph object as a node. An edge in that graph represents
  * a connection between an Observable and a Listener/Observer. ObservableObject enables to manage the creation/destruction
  * of node when a Observable/Listener/Observer is created/deleted. ObservableObject also enables to guarantee the coherency of
  * relations between Observable and Listener/Observer.
  *
  * ObservableObject provides functions to access to the graph of ObservableObjects as well as the state of elements in that graph. Using
  * these functions can ease debugging of complex Observable/Listener/Observer systems.
  *
  * @warning Due to the possibility to delay event sending, it is possible that nodes or edges in the graph don't represent
  * the connections and objects that the user expect. Indeed, node deletion, edge creation and edge deletion
  * are delayed until the Obserable::notify or Obserable::unholdObserver function terminate. Only node creation is not delayed.
  *
  * @see Listener
  * @see Observer
  * @see Observable
  **/
class  TLP_SCOPE Observable {
  friend class Event;

public:
  /**
  * @brief start delay of events to all Observer
  *
  * After a call to holdObservers, no more events are sent to Observer(execpt DELETE events),
  * events are still transmit to listeners. It is possible to call several time the holdObservers
  * function, in that case the observers are holded until all holdObservers calls are closed
  * by unholdOBservers calls.
  *
  * @see unholdObservers
  * @see Observer
  */
  static void holdObservers();
  /**
  * @brief Send queued event to all Observer.
  *
  * If the stack of Observable lock is empty (ie. number of calls to unhold equal to the number of calls
  * to hold), send all queued events to observers.
  *
  * @warning Calling that function when the hold stack is empty raises an ObservableException.
  *
  * @see holdObservers
  * @see Observer
  */
  static void unholdObservers();

  static void _DEPRECATED unholdObservers(bool) {
    unholdObservers();
  }

  static unsigned int observersHoldCounter() {
    return _oHoldCounter;
  }

public:

  /**
  * @brief
  */
  void addObserver(Observable * const obs) const;
  /**
  * @brief
  */
  void addListener(Observable * const obs) const;
  /**
   * @brief
   */
  void  removeObserver(Observable  * const obs) const;
  void  removeListener(Observable  * const obs) const;


  /**
  * @brief return the number of sent nofication
  */
  unsigned int getSent() const;
  /**
  * @brief return the number of received nofication
  */
  unsigned int getReceived() const;
  /**
  * @brief return the number of Listeners of that Object
  */
  unsigned int countListeners() const;
  /**
  * @brief return the number of Observers of that Object
  */
  unsigned int countObservers() const;


public:
  /**
  * @bried return a pointer on the object represented by that node
  * @warning If you call that function during an update, it is possible that the pointer do not point on an existing object, use
  *          the isAlive function before to test if your object still valid
  */
  static Observable* getObject(tlp::node n);
  /**
  * @brief Enables to test if the object represented by a node has been deleted. Outside a unhold/hold block or an update that function
  *        always return true.
  */
  static bool  isAlive(tlp::node n);
  /**
  * @brief Return a reference on the Observable/Listener/Observer graph.
  */
  static const tlp::VectorGraph& getObservableGraph();

protected:
  Observable();
  Observable(const Observable &);
  virtual ~Observable();
  Observable& operator=(const Observable &);
  /**
  * @brief Enable to send an event to all Observer/Listener
  *
  * According to the type of the event that function has got different behaviour.
  * event::DELETE : are sent directly to all Observers/Listeners, it is not possible to delay their processing.<br>
  * event::MODIFICATION : sent to all Observer/Listener, it is possible to delay their processing (only for Observer) <br>
  * event::INFORMATION : sent only to Listener, it is not possible to delay their processing.<br>
  * event::INVALID : never sent use internally.<br>
  *
  * @note In any cases, Event are first sent to Listener and then to Observer
  *
  * To prevent building of too many objects(Event) a notify code block in an Observable should look like that:
  * @code
  *    if (hasOnlookers()) {
  *       notify(MyEvent(*this, param1, param2, ...));
  *    }
  ° @endcode
  */
  void sendEvent(const Event &);
  /**
   * that function should be virtual pure
   * the current implementation is only here to insure compatibility with
   * tulip 3.x x<6.
   */
  virtual void treatEvents(const  std::vector<Event> &events );
  virtual void treatEvent(const Event &);

  /**
  * @brief Enable to send Event::DELETE before the deletion of subclass internal objects.
  *
  * The Observable mecahnism automatically send DELETE event to Listener/Observer when an
  * Obseravble is deleted. However when that Event is send the subclass of Observable are
  * already deleted, thus it is no more possible to access to their members. In case you
  * want to allow to access to your Observable member variables/functions when Listner/Observer
  * receive a DELETE events, you should call that function in your destructor.
  *
  * @warning That function must be called only one time in the destructor of your object, in
  * case of several inheritance be carefull not to call it in all the destructors implied in
  * in the inheritance tree.
  *
  */
  void observableDeleted();
  /**
  * @brief Enable to know if there is at least one Observer/Listener connected.
  *
  * it enables to prevent creation and sending of Event for nothing.
  * Usually a notify code block in an Observable should look like that:
  * @code
  *    if (hasOnlookers()) {
  *       notify(MyEvent(*this, param1, param2, ...));
  *    }
  ° @endcode
  */
  bool hasOnlookers() const;

  /**
  * @brief use for old observer tulip compatibility
  */
  tlp::Iterator<Observable *> *getObservables() const;
  /**
  * @brief use for old observer tulip compatibility
  */
  virtual void _DEPRECATED update(std::set<Observable*>::iterator, std::set<Observable*>::iterator);
  /**
  * @brief use for old observer tulip compatibility
  */
  virtual void _DEPRECATED observableDestroyed(Observable *);

  /**
  * @brief use for old observer tulip compatibility
  */
  void _DEPRECATED notifyObservers();

private:
  enum OBSERVABLEEDGETYPE {OBSERVABLE = 0x01, OBSERVER = 0x02, LISTENER = 0x04};
  bool deleteMsgSent; /** use to enable subclasses to send the Event::DELETE in there destructor */
  mutable bool queuedEvent;  /** Use to prevent unecessary elements insertion in the set of events */
  tlp::node _n; /** node that represent that object in the ObservableGraph.*/
#ifndef NDEBUG
  unsigned int sent; /* counter of sent notification */
  unsigned int received; /* counter of received notification */
#endif


  /**
  * @brief return an Iterator on all Onlookers
  * @warning adding or removing Onlooker to that Observable will devalidate the iterator
  * @see StableIterator
  * @see forEach
  * @see stableForEach
  */
  tlp::Iterator<Observable *> *getOnlookers() const;
  /**
  * @brief return an iterator on in objects (Observable), the iterator guarantee that all objects are alive (not deleted during hold or notify)
  */
  tlp::Iterator<tlp::node> *getInObjects() const;
  /**
  * @brief return an iterator on out objects (Listener/Observer), the iterator garantee that all objects are alive (not deleted during hold or notify)
  */
  tlp::Iterator<tlp::node> *getOutObjects() const;
  /**
  * @brief add an Observer/Listener to the observable
  *
  * The added Onlookers will received the next Event sent by the Observable.
  * In case of nested unholding (almost never), calling that function inside hold/unhold block
  * can make the Observer receive an event that has been sent before it was Observing the object.
  */
  void addOnlooker(const Observable &, OBSERVABLEEDGETYPE type) const;
  /**
  * @brief remove an Observer/Listener of the observable
  *
  * In case of nested unholding (almost never), Calling that function inside a hold/unhold block could
  * make the Observer not receive an event that was sent when it was connected.
  *
  * @warning removing OnLooker that has been created outside of your code can create serious
  * problem in your application. Objects that are listening/observing could need to receive
  * the events to work properly.
  */
  void removeOnlooker(const Observable &, OBSERVABLEEDGETYPE type) const;
  /**
  * @brief return the node representing that ObservableObject in the ObservableGraph
  */
  tlp::node getNode() const;



  //static members and methods
private:
  static tlp::VectorGraph                 _oGraph;         /** the graph that store all observers and connection between them*/
  static tlp::NodeProperty<Observable *>  _oPointer;       /** store a pointer to the object represented by a node */
  static tlp::NodeProperty<bool>          _oAlive;         /** enables to know if an object has been deleted or not */
  static tlp::EdgeProperty<unsigned char> _oType;         /** enables to know the type of relation between to Observable Objects*/
  static std::vector<tlp::node>           _oDelayedDelNode; /** store deleted nodes, to remove them at the end of the notify*/
  static std::set<std::pair<tlp::node, tlp::node> > _oDelayedEvents;
  static unsigned int              _oNotifying;      /** counter of nested notify calls */
  static unsigned int              _oUnholding;      /** counter of nested unhold calls */
  static unsigned int              _oHoldCounter;    /** counter of nested holds */
  static bool                      _oInitialized;   /** use to initialize oGraph when the library is loaded (nice hack) */

  /**
  * @brief delete nodes from the ObservableGraph that have been preserved to keep coherency and check bad use of the mecanism
  */
  static void updateObserverGraph();

  /**
  * @brief return the bound node representing this ObservableObject in the ObservableGraph,
  * if _n is not valid it is then bind to a new added node
  */
  tlp::node getBoundNode();
  bool isBound() const {
    return _n.isValid();
  }

  /**
  * @brief Trick to init the ObservableGraph properties (automatically call at the loading of the library.
  */
  static bool init();
};

typedef _DEPRECATED Observable Observer;

/*@}*/
}

#endif
