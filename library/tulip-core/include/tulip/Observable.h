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

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <tulip/TulipException.h>
#include <tulip/ForEach.h>
#include <tulip/vectorgraph.h>
#include <set>

namespace tlp {

class Observable;
//=======================================
/**
  * @ingroup Observation
  * @brief Event is the base class for all events used in the Observation mechanism.
  *
  * An Event is characterized by its type. The base Event class only carries information as to the type of event, nothing specific.
  *
  * Event::DELETE : send directly to all Observers/Listeners, not affected by Observable::holdObservers().
  * Event::MODIFICATION : sent to all Observers/Listeners. MODIFICATION are first sent to Observers and then to Listeners.
  * Event::INFORMATION : sent only to Listeners.
  * Event::INVALID : never sent, used internally for delaying events.
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
 * @ingroup Observation
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
 * @ingroup Observation
 * @brief The Observable class is the base of Tulip's observation system.
 *
 * Each class that wishes to send or receive notifications needs to inherit from Observable.
 *
 * Tulip has two separate mechanisms of observation, Observers and Listeners.
 * These two mechanisms work through the same class, the difference lies
 * in the way an Observer or a Listener is attached to the object whose events
 * it will receive.
 *
 * The Listener is closer to the original pattern, where events are sent directly to the recipient.
 * The Observer is a twist for performance purposes, it can receive the events in a delayed fashion, and cannot know if the event was just sent or was delayed.
 *
 * The purpose of this twist is to allow algorithms that perform a high number of modifications (e.g. creating a grid to route edges,
 *  creating multiple subgraphs with metrics or layouts) to run smoothly without overcrowding the event queue.
 * As events are sent for every graph modification (e.g. adding a node, deleting a node, setting a value on a node), the sheer quantity of events sent by these
 * algorithms would cause a big performance hit.
 *
 * This can be avoided by using Observable::holdObservers().
 * This holds all events in a queue and only sends them when Observable::unholdObservers() is called.
 *
 * The only exception to this mechanism is the Event::DELETE kind of event, that is never held back.
 * Think of it as an unmaskable POSIX signal; whatever the connection to the object and the state of holdObserver, this event will get through.
 * This is used to prevent crashes in the case where and object is listened or observed and is deleted, as it is likely the recipient
 * keeps a pointer to the now deleted object.
 *
 * The Listener however is not affected by the use of Observable::holdObservers() and Observable::unholdObservers().
 *
 * The observable Observers and Listeners are internally store in a Graph structure, allowing to easily visualize their connections.
 * This eases debugging of Observation-related bugs.
 *
 * Events are always sent to Listeners first, and then to Observers, even when there is no hold.
 *
 * If you wish to receive events, you must inherit from Observable, and implement one of two virtual functions.
 *
 * If you need to received events without delay, you will be a Listener, and need to implement treatEvent().
 * You will attach to the object you wish to receive events from using addListener().
 *
 * If you can receive events after a delay, you will be an Observer, and need to implement treatEvents().
 * You will attach to the object you wish to receive events from using addObserver().
 *
 **/
class  TLP_SCOPE Observable {
  friend class Event;

public:
  /**
  * @brief Holds back all events until Observable::unholdObservers() is called.
  *
  * Listeners are not affected by this function.
  * Once this function is called, all events heading to an Observer will be held, except DELETE events.
  * The events are stored in a queue, and will be sent once Observable::unholdObservers() is called.
  *
  * It is possible to nest calls to  Observable::holdObservers() and  Observable::unholdObservers(),
  * and in this case the events will only be sent when there
  * have been as many calls to  Observable::unholdObservers() as to  Observable::holdObservers().
  *
  * It is possible to check whether the events are being help by checking the Observable::observersHoldCounter() function.
  *
  * @see unholdObservers
  * @see observersHoldCounter
  */
  static void holdObservers();
  /**
  * @brief Sends all held events to the Observers.
  *
  * Listeners are not affected by this function.
  *
  * In debug mode, if the hold counter is less than one when calling this function, an ObservableException will be raised.
  *
  * @see holdObservers
  * @see observersHoldCounter
  */
  static void unholdObservers();

  /**
   * @brief observersHoldCounter gives the number of times holdObservers() has been called without a matching unholdObservers() call.
   * @return the number of times holdObservers() has been called without a matching unholdObservers() call.
   */
  static unsigned int observersHoldCounter() {
    return _oHoldCounter;
  }

  /**
   * @brief Adds an Observer to this object.
   *
   * The observer will receive events sent by this object, if there is no hold applied.
   * @param observer The object that will receive events.
   */
  void addObserver(Observable * const observer) const;

  /**
   * @brief Adds a Listener to this object.
   *
   * The Listener will receive events regardless of the state of holdObservers() and unholdObservers().
   * @param listener The object that will receive events.
   */
  void addListener(Observable * const listener) const;

  /**
   * @brief Removes an observer from this object.
   *
   * The observer will no longer receive events from this object.
   * @param observer The observer to remove from this object.
   */
  void  removeObserver(Observable  * const observerver) const;

  /**
   * @brief Removes a listener from this object.
   *
   * The listener will no longer receive events from this object.
   * @param listener The listener to remove from this object.
   */
  void  removeListener(Observable  * const listener) const;

  /**
   * @brief gets the number of sent events.
   * @return the number of sent events (0 when compiling with -DNDEBUG).
   */
  unsigned int getSent() const;
  \
  /**
   * @brief get the number of received events.
   * @return the number of received events (0 when compiling with -DNDEBUG).
   */
  unsigned int getReceived() const;

  /**
   * @brief gets the number of observers attached to this object.
   * @return the number of observers attached to this object.
   */
  unsigned int countObservers() const;

  /**
   * @brief gets the number of listeners attached to this object.
   * @return the number of listeners attached to this object.
   */
  unsigned int countListeners() const;

public:
  /**
   * @brief internal function for debugging purpose
   * If there is no hold currently applied, or no update ongoing, always returns true.
   * Checks if the object represented by the node has been deleted.
   * @param n The node to check for life signs.
   * @return Whether the node is dead.
  **/
  static bool isAlive(tlp::node n);

  /**
   * @brief internal function for debugging purpose
   * If called during an update, it is possible the pointed object has been deleted.
   * use isAlive() to check for this and avoid a crash.
   * @param n The node representing the object to retrieve.
   * @return The object represented by the node.
  **/
  static Observable* getObject(tlp::node n);

  /**
   * @brief internal function for debugging purpose
   * getScheduled get the number of scheduled events for this Observable.
   * @param n The node of an Observable object
   * @return the number of scheduled events involving this Observable as sender or receiver.
   */
  static unsigned int getScheduled(tlp::node n);

  /**
   * @brief internal function for debugging purpose
   * Get the node representing this object in the Observable Graph.
   * @param obs the Observable object
   * @return the node of the Observable object in the Observable Graph.
   */
  static tlp::node getNode(const tlp::Observable* obs);

  /**
   * @brief Gets the observation graph.
   * @return The graph containing a node for each Observable/Observer/Listener, and an edge between connected objects.
   */
  static const tlp::VectorGraph& getObservableGraph();

protected:
  Observable();
  Observable(const Observable &);
  virtual ~Observable();
  Observable& operator=(const Observable &);

  /**
   * @brief Sends an event to all the Observers/Listeners.
   * It is higly recommended to check if there are observers/listeners to send the event to before actually sending it
   * to avoid the overhead of creating too many objects.
   *
   * This can be achieved by using the hasOnLookers() function :
   *
   * @code
   *    if (hasOnlookers()) {
   *       sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODE, n));
   *    }
   * @endcode
   *
   * @param message The message to send to the listeners/observers.
   */
  void sendEvent(const Event &message);

  /**
   * @brief This function is called when events are sent to Observers, and Observers only.
   *
   * It is passed a vector of all the events that happened since the last call.
   * If events were held, this vector can be pretty large, and if events were not held it is likely it only contains a single element.
   *
   * @param events The events that happened since the last unHoldObservers().
   */
  virtual void treatEvents(const std::vector<Event> &events);

  /**
   * @brief This function is called when events are sent to the Listeners, and Listeners only.
   *
   * Is it passed a reference to the event that just happened.
   *
   * @param message The event that was sent.
   */
  virtual void treatEvent(const Event &message);

  /**
  * @brief Sends the Event::DELETE before the deletion of the subclass and its internal objects.
  *
  * The observation system automatically sends the DELETE event when the Observable is deleted, but
  * in the case you need to access members of the class inheriting from Observable, you need the event
  * sent *before* the outermost objects are destroyed.
  *
  * To achieve this, you can call this function in your destructor, and the DELETE event will be sent.
  * This will allow your Listeners/Observers to access your members one last time before deletion.
  *
  * @warning This function must be called only once per object.
  * Make sure no other class in the inheritance tree calls this function before adding this call to your destructor.
  */
  void observableDeleted();

  /**
   * @brief Checks whether there are Observers/Listeners attached to this object.
   *
   * Using this function avoids the creation of events that no-one will see :
   * @code
   *    if (hasOnlookers()) {
   *       sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODE, n));
   *    }
   * @endcode
   *
   * @return
   */
  bool hasOnlookers() const;

  /**
  * @brief use for old observer tulip compatibility
  */
  _DEPRECATED  tlp::Iterator<tlp::Observable *> * getObservables() const;

  /**
  * @brief use for old observer tulip compatibility
  */
  _DEPRECATED void notifyObservers();

private:
  enum OBSERVABLEEDGETYPE {OBSERVABLE = 0x01, OBSERVER = 0x02, LISTENER = 0x04};

  /**
   * @brief deleteMsgSent This allows for calling observableDeleted() multiple times safely.
   */
  bool deleteMsgSent;

  /**
   * @brief queuedEvent Used to prevent unecessary elements insertion in the set of events.
   */
  mutable bool queuedEvent;

  /**
   * @brief _n The node that represents this object in the ObservableGraph.
   */
  tlp::node _n;
#ifndef NDEBUG
  /**
   * @brief sent The number of sent events
   */
  unsigned int sent;

  /**
   * @brief received The number of received events.
   */
  unsigned int received;
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
   * @brief getInObjects Retrieves Inbound objects (observed objects; i.e. Observable).
   * @return an iterator on 'in' objects (Observable), the iterator guarantees that all objects are alive (not deleted during hold or notify).
   */
  tlp::Iterator<tlp::node> *getInObjects() const;

  /**
   * @brief getOutObjects Retrieves Outbound objects (observing objects; i.e. Listeners and Observers).
   * @return an iterator on out objects (Listener/Observer), the iterator garantees that all objects are alive (not deleted during hold or notify).
   */
  tlp::Iterator<tlp::node> *getOutObjects() const;

  /**
   * @brief addOnlooker Adds an Observer/Listener to this object.
   *
   * The added Onlookers will received the next Event sent by the Observable.
   * In case of nested unholding (higly unlikely), calling that function inside hold/unhold block
   * can make the Observer receive an event that has been sent before it was Observing the object.
   *
   * @param obs The Observer/Listener to add to this object.
   * @param type The kind of observation (Listener or Observer).
   */
  void addOnlooker(const Observable &obs, OBSERVABLEEDGETYPE type) const;

  /**
   * @brief removeOnlooker removes an Observer/Listener from this object.
   *
   * In case of nested unholding (higly unlikely), calling that function inside a hold/unhold block could
   * make the Observer not receive an event that was sent when it was connected.
   *
   * @warning removing OnLooker that has been created outside of your code can create serious
   * problems in your application. Objects that are listening/observing could need to receive
   * the events to work properly.
   *
   * @param obs The Observer/Listener to remove
   * @param type The kind of connection (Listener or Observer).
   */
  void removeOnlooker(const Observable &obs, OBSERVABLEEDGETYPE type) const;

  /**
   * @brief getNode Get the node representing this object in the ObservableGraph.
   * @return the node representing that ObservableObject in the ObservableGraph.
   */
  tlp::node getNode() const;

  //static members and methods
  /**
   * @brief _oGraph the graph used to store all observers and connection between them.
   */
  static tlp::VectorGraph _oGraph;

  /**
   * @brief _oPointer a pointer to the object represented by a node
   */
  static tlp::NodeProperty<Observable *> _oPointer;

  /**
   * @brief _oAlive whether an object has been deleted or not
   */
  static tlp::NodeProperty<bool> _oAlive;

  /**
   * @brief _oEventsToTreat the count of events scheduled to be treated by an object
   * the object's associated node is deleted only when this count is null
   * in order to prevent the node reuse and ensure the _oAlive associated value
   */
  static tlp::NodeProperty<unsigned int> _oEventsToTreat;

  /**
   * @brief _oType the type of relation between two Observable Objects
   */
  static tlp::EdgeProperty<unsigned char> _oType;

  /**
   * @brief _oDelayedDelNode store deleted nodes, to remove them at the end of the notify
   */
  static std::vector<tlp::node> _oDelayedDelNode;

  static std::set<std::pair<tlp::node, tlp::node> > _oDelayedEvents;

  /**
   * @brief _oNotifying counter of nested notify calls
   */
  static unsigned int _oNotifying;

  /**
   * @brief _oUnholding counter of nested unhold calls
   */
  static unsigned int _oUnholding;

  /**
   * @brief _oHoldCounter counter of nested holds
   */
  static unsigned int _oHoldCounter;

  /**
   * @brief _oInitialized use to initialize oGraph when the library is loaded (nice hack)
   */
  static bool _oInitialized;

  /**
  * @brief delete nodes from the ObservableGraph that have been preserved to keep coherency and check bad use of the mechanism.
  */
  static void updateObserverGraph();

  /**
   * @brief getBoundNode
   * @return the bound node representing this ObservableObject in the ObservableGraph,
   * if _n is not valid it is then bound to a new added node
   */
  tlp::node getBoundNode();
  bool isBound() const {
    return _n.isValid();
  }

  /**
  * @brief Trick to init the ObservableGraph properties (called at the loading of the library, during static initialization).
  */
  static bool init();
};

/**
 * @brief The ObserverHolder class is a convenience class to automatically hold and unhold observers.
 * It performs a call to Observable::holdObserver() at its creation and a call to Observable::unholdObserver() at its destruction.
 * You can use it if you have to hold observers in a function with multiple return points to avoid to call Observable::unholdObserver() for each of them.
 * @code
 * void myFunc(){
 *  ObserverHolder holder;//Automatically call Observable::holdObserver()
 *
 *  if(someTest()){
 *      someOperation1();
 *      return;//No need to call Observable::unholdObserver() it will be called with the destruction of the locker object
 *  }
 *
 * }
 * @endcode
 *
 */
class TLP_SCOPE ObserverHolder {
public :
  ObserverHolder() {
    Observable::holdObservers();
  }
  ~ObserverHolder() {
    Observable::unholdObservers();
  }
};

// deprecated name of this class
_DEPRECATED_TYPEDEF(ObserverHolder, ObserverLocker);

}

#endif
