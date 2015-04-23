/**
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

#if _MSC_VER
// disable Visual Studio warning about using this in base member initializer list
// this is not critical in our case as we only store a pointer
#pragma warning(disable: 4355)
#endif

#include <iostream>
#include <sstream>
#include <map>

#include <tulip/Observable.h>
#include <tulip/conversioniterator.h>
#include <tulip/filteriterator.h>

using namespace std;
using namespace tlp;

namespace {
//======================================================================
struct Node2Observable {
  Observable* operator()(tlp::node n) {
    return object[n];
  }
  tlp::NodeProperty<Observable *> object;
} node2Observable;
//======================================================================
struct AliveFilter {
  bool operator()(tlp::node n) {
    return alive[n];
  }
  tlp::NodeProperty<bool> alive;
} objectAlive;
//======================================================================
template<unsigned int linkType>
struct LinkFilter {
  bool operator()(tlp::node n) {
    edge link(oGraph.existEdge(n, oNode));
    return (link.isValid() && (oType[link] & linkType));
  }

  LinkFilter(const VectorGraph& graph,
             const EdgeProperty<unsigned char>& type,
             node n): oGraph(graph), oType(type), oNode(n) {}

  const VectorGraph& oGraph;
  const EdgeProperty<unsigned char>& oType;
  node oNode;
};

}


namespace tlp {

//=================================
VectorGraph                 Observable::_oGraph;
unsigned int                Observable::_oNotifying   = 0;
unsigned int                Observable::_oUnholding   = 0;
unsigned int                Observable::_oHoldCounter = 0;
NodeProperty<Observable *>  Observable::_oPointer;
NodeProperty<bool>          Observable::_oAlive;
NodeProperty<unsigned int>  Observable::_oEventsToTreat;
EdgeProperty<unsigned char> Observable::_oType;
std::set<std::pair<tlp::node, tlp::node> > Observable::_oDelayedEvents = std::set<std::pair<tlp::node, tlp::node> >();
vector<node>              Observable::_oDelayedDelNode;
bool                      Observable::_oInitialized = Observable::init();

//----------------------------------
Iterator<node> * Observable::getInObjects() const {
  assert(_n.isValid());
  return new FilterIterator<node, AliveFilter>(_oGraph.getInNodes(_n), objectAlive);
}
//----------------------------------
Iterator<node> * Observable::getOutObjects() const {
  assert(_n.isValid());
  return new FilterIterator<node, AliveFilter>(_oGraph.getOutNodes(_n), objectAlive);
}
//----------------------------------
node Observable::getNode() const {
  return _n;
}
//----------------------------------
node Observable::getBoundNode() {
  if (_n.isValid() == false) {
    _n = _oGraph.addNode();
    _oPointer[_n] = this;
    _oAlive[_n] = true;
    _oEventsToTreat[_n] = 0;
  }

  return _n;
}
//----------------------------------
unsigned int Observable::getSent() const {
#ifndef NDEBUG
  return sent;
#endif
  return 0;
}
//----------------------------------
unsigned int Observable::getReceived() const {
#ifndef NDEBUG
  return received;
#endif
  return 0;
}
//----------------------------------
bool Observable::isAlive(tlp::node n) {
  return _oAlive[n];
}
//----------------------------------
unsigned int Observable::getScheduled(tlp::node n) {
  return _oEventsToTreat[n];
}
//----------------------------------
Observable* Observable::getObject(node n) {
  assert(_oAlive[n]);

  if (!_oAlive[n])
    throw ObservableException("That object has been deleted it is no more accessbile");

  return _oPointer[n];
}
//----------------------------------
tlp::node Observable::getNode(const Observable* obs) {
  return obs->getNode();
}
//----------------------------------
bool Observable::init() {
  Observable::_oGraph.alloc(Observable::_oPointer);
  Observable::_oGraph.alloc(Observable::_oAlive);
  Observable::_oGraph.alloc(Observable::_oEventsToTreat);
  Observable::_oGraph.alloc(Observable::_oType);

  objectAlive.alive = Observable::_oAlive;
  node2Observable.object = Observable::_oPointer;
  return true;
}
//----------------------------------
const tlp::VectorGraph& Observable::getObservableGraph() {
  return Observable::_oGraph;
}
//=================================
Event::Event(const Observable &sender, EventType type):
  _sender(sender.getNode()),
  _type(type) {
  assert(_type != TLP_DELETE);

  if (_type == TLP_DELETE)
    throw ObservableException("It is forbidden to create a delete events, DeleteEvents are autmotically generated at the observable destruction");
}
//----------------------------------
Event::~Event() {
}
//----------------------------------
Observable* Event::sender() const {
  return Observable::getObject(_sender); /** only Observable can be use to create event */
}
//=================================
// define a class for an empty Iterator of Observable *
class NoObservableIterator : public Iterator<Observable *> {
public:
  Observable* next() {
    return NULL;
  }
  bool hasNext() {
    return false;
  }
};
//=================================
Iterator<Observable *> *Observable::getObservables() const {
  if (isBound())
    return
      new ConversionIterator<node, Observable*,
      Node2Observable>(getOutObjects(), node2Observable);

  return new NoObservableIterator();
}
//=================================
void Observable::treatEvents(const  std::vector<Event> & ) {
#ifndef NDEBUG
  tlp::debug() << __PRETTY_FUNCTION__ << ": not implemented" << std::endl;
#endif
}
//=================================
void Observable::treatEvent(const Event &) {
#ifndef NDEBUG
  tlp::debug() << __PRETTY_FUNCTION__ << ": not implemented" << std::endl;
#endif
}
//=================================
Observable::Observable(): deleteMsgSent(false), queuedEvent(false), _n(node()) {
#ifndef NDEBUG
  sent = received = 0;
#endif
  //tlp::debug() << "[Observable node] created:" << n.id << "::" << this << endl;
}
//----------------------------------
Observable::Observable(const Observable &):deleteMsgSent(false), queuedEvent(false), _n(node()) {
#ifndef NDEBUG
  sent = received = 0;
#endif
  //tlp::debug() << "[Observable node] created (copy constructor):" << n.id << "::" << this << endl;
}
//----------------------------------
Observable& Observable::operator=(const Observable &) {
#ifndef NDEBUG
  tlp::debug() << "[Observable Warning]: Observable object should reimplement their operator= else nothing is copied" << endl;
#endif
  return *this;
}
//----------------------------------
Observable::~Observable() {
  if (_n.isValid() == false)
    return;

  if (!deleteMsgSent)
    observableDeleted();

#ifdef _OPENMP
  #pragma omp critical(ObservableGraphUpdate)
#endif
  {
    assert(_oAlive[_n]);

    if (!_oAlive[_n])
      throw ObservableException("Observable object has already been deleted, possible double free!!!");

    //tlp::debug() << "[Observable node] destructor:" << n.id  << "::" << this << endl;
    _oAlive[_n] = false;

    bool noDelay =
      (_oNotifying == 0) && (_oUnholding == 0) && (_oHoldCounter == 0);

    if ((noDelay == false) &&  (_oEventsToTreat[_n] == 0)) {
      // _n cannot be deleted only if it is observed
      // then its deletion is delayed until the observers are unhold
      noDelay = true;
      edge e;
      forEach(e, _oGraph.getInEdges(_n)) {
        if (_oType[e] & OBSERVER) {
          noDelay = false;
          break;
        }
      }
    }

    if (noDelay) {
      assert(_oEventsToTreat[_n] == 0);
      _oGraph.delNode(_n);
      //tlp::debug() << "[Observable node] deleted:" << _n.id << "::" << this << endl;
    }
    else {
      _oDelayedDelNode.push_back(_n);
      //tlp::debug() << "[Observable node] delayed delete:" << _n.id << "::" << this << endl;
      _oGraph.delEdges(_n);
    }
  }
}

//----------------------------------------
void Observable::holdObservers() {
#ifdef _OPENMP
  #pragma omp critical(ObservableGraphUpdate)
#endif
  ++_oHoldCounter;
}
//----------------------------------------
void Observable::unholdObservers() {
  {
    assert(_oHoldCounter > 0);

    if (_oHoldCounter == 0) {
#ifndef NDEBUG
      throw ObservableException("unhold call without a previous call to hold");
#endif
      return;
    }

    --_oHoldCounter;
    {
      if (_oHoldCounter > 0 || _oDelayedEvents.empty() ) return;

      ++_oUnholding;
      ++_oHoldCounter; /** rehold the observer to buffer message sent during unholding */

      set< pair<node, node> > backupEvents;
      backupEvents.swap(_oDelayedEvents);
      set< pair<node, node> >::const_iterator it;

      for( it = backupEvents.begin(); it != backupEvents.end(); ++it) {
        if (Observable::_oAlive[it->first]) {
          Observable *sender = static_cast<Observable *>(Observable::_oPointer[it->first]);
          sender->queuedEvent = false;
        }
      }

      map<node, vector<Event> > preparedEvents;

      for( it = backupEvents.begin(); it != backupEvents.end(); ++it) {
        if (Observable::_oAlive[it->first] && Observable::_oAlive[it->second]) {
          // schedule a new Event
          _oEventsToTreat[it->second] += 1;
          Observable *sender = static_cast<Observable *>(Observable::_oPointer[it->first]);
          preparedEvents[it->second].push_back(Event(*sender, Event::TLP_MODIFICATION));
        }
      }

      {
        map<node, vector<Event> >::const_iterator it;

        for (it = preparedEvents.begin(); it!=preparedEvents.end(); ++it) {
          // treat scheduled events
          _oEventsToTreat[it->first] -= it->second.size();

          if (_oAlive[it->first]) {
            Observable *obs  = static_cast<Observable *>(_oPointer[it->first]);
#ifndef NDEBUG
            ++(obs->received);
#endif
            obs->treatEvents(it->second);
          }
        }
      }

      -- _oUnholding;
      unholdObservers(); /** treat queued events during the unhold */
      updateObserverGraph();

      if (_oHoldCounter > 0) {
        throw ObservableException("Observable after unholdd call, bad nested hold/unhold function call in an Observer:: update() function");
      }
    }
  }
}
//----------------------------------------
Iterator<Observable *> *Observable::getOnlookers() const {
  if (isBound()) {
    assert(_oAlive[_n]);

    if (!_oAlive[_n]) {
      throw ObservableException("getObservers called on a deleted Observable");
    }

    return new ConversionIterator<node, Observable*, Node2Observable>(getInObjects(), node2Observable);
  }

  return new NoObservableIterator();
}
//----------------------------------------
void Observable::addOnlooker(const Observable &obs, OBSERVABLEEDGETYPE type) const {
#ifdef _OPENMP
  #pragma omp critical(ObservableGraphUpdate)
#endif
  {
    assert(!isBound() || _oAlive[_n]);

    if (isBound() && !_oAlive[_n]) {
      throw ObservableException("addObserver called on a deleted Observable");
    }

    // check for an existing link
    edge link;

    if (isBound() && obs.isBound())
      link = _oGraph.existEdge(obs.getNode(), getNode());

    if (!link.isValid()) {
      // add new link
      // at this time both Observables need to be bound
      link = _oGraph.addEdge(((Observable &) obs).getBoundNode(),
      ((Observable *) this)->getBoundNode());
      _oType[link] = type;
    }
    else {
#ifndef NDEBUG

      if (_oType[link] & type) {
        tlp::warning() << "[Observable Warning]: observer already connected" << std::endl;
      }

#endif
      // add the bit for the given type on the edge
      _oType[link] |= type;
    }
  }
}
//----------------------------------------
void Observable::addObserver(Observable * const observer) const {
  assert(observer != NULL);
  queuedEvent = false;
  addOnlooker(*observer, OBSERVER);
}
//----------------------------------------
void Observable::addListener(Observable * const listener) const {
  assert(listener != NULL);
  addOnlooker(*listener, LISTENER);
}
//----------------------------------------
void Observable::observableDeleted() {
  assert(deleteMsgSent == false);

  if (deleteMsgSent) {
    throw ObservableException("Delete message has been sent several time.");
  }

  deleteMsgSent = true;

  if (hasOnlookers()) {
    Event msg(*this, Event::TLP_INVALID); //create a modify event to prevent raise exception, (hack) to forbid creation of Delete exception without calling that fucnton
    msg._type = Event::TLP_DELETE;
    sendEvent(msg);
  }
}
//----------------------------------------
void Observable::sendEvent(const Event &message) {
  if (!isBound())
    return;

  //cerr << "send event " << _oPointer[_n] << " " << message.type() << " indeg " << _oGraph.indeg(_n) << " outdeg: " << _oGraph.outdeg(_n) << endl;
  if (!_oGraph.isElement(_n) || !_oAlive[_n]) {
    throw ObservableException("Notify called on a deleted Observable");
  }


  const unsigned int RECCALL = 200;

  if (_oNotifying > RECCALL) {
    std::stringstream str;
    str << "Maximum number of nested calls (" << RECCALL << ") reached, contact tulip team if that limit is too restrictive";
    throw ObservableException(str.str());
    //return;
  }

  node backn = _n; /** to keep trace of the node if the observable is deleted during the notification, in that crazy case, (*this) is dead thus n is not accessible*/
  ++_oNotifying;

  //create two separate list of observer & listeners
  //could be extended if we need recorders
  vector< pair<Observable *, node> > observerTonotify;
  vector< pair<Observable *, node> > listenerTonotify;
  edge e;
  bool delayedEventAdded = false;
  forEach(e, _oGraph.getInEdges(_n)) {
    node src(_oGraph.source(e));

    if (_oAlive[src]) {
      Observable *obs = _oPointer[src];
      assert(obs != NULL);

      if ((_oType[e] & OBSERVER) && (message.type() != Event::TLP_INFORMATION)) {
        if (_oHoldCounter == 0  || message.type() == Event::TLP_DELETE) {
          // schedule event
          _oEventsToTreat[backn] += 1;
          _oEventsToTreat[src] += 1;
          observerTonotify.push_back(pair<Observable*, node>(obs, src));
        }
        else if (!queuedEvent) {
          delayedEventAdded = true;
#ifdef _OPENMP
          #pragma omp critical(ObservableGraphUpdate)
#endif
          {
            _oDelayedEvents.insert(pair<node, node>(_n, src));
          }
        }
      }

      if (_oType[e] & LISTENER) {
        // schedule event
        _oEventsToTreat[backn] += 1;
        _oEventsToTreat[src] += 1;
        listenerTonotify.push_back(pair<Observable*, node>(obs, src));
      }

    }
  }

  if (delayedEventAdded) {
    queuedEvent = true;
  }

  //send message to listeners
  if (!listenerTonotify.empty()) {
    vector< pair<Observable *, node> >::const_iterator itobs;

    for(itobs = listenerTonotify.begin(); itobs != listenerTonotify.end(); ++itobs) {
      if (itobs->second == backn && message.type() == Event::TLP_DELETE) {
        tlp::debug() << "[Observable info]: An observable onlook itself Event::DELETE msg can't be sent to it." << endl;
        // treat scheduled event
        _oEventsToTreat[backn] -= 2;
        continue;
      }

      // treat scheduled event
      _oEventsToTreat[itobs->second] -= 1;

      if (_oAlive[itobs->second]) { //other listeners/observers could be destroyed during the treat event
#ifndef NDEBUG
        ++(itobs->first->received);
#endif
        itobs->first->treatEvent(message);
      }

      // we decrement after treating event
      // to prevent reuse of backn
      _oEventsToTreat[backn] -= 1;

      assert(_oAlive[backn]);

      if (!_oAlive[backn]) {
        throw ObservableException("An observable has been deleted during the notifification of its observer (ie. an observer has deleted its caller during an update)");
      }
    }
  }

  //send simple event to observers
  if (!observerTonotify.empty()) {
    vector<Event> tmp(1, message);
    vector< pair<Observable *, node> >::const_iterator itobs;

    for(itobs = observerTonotify.begin(); itobs != observerTonotify.end(); ++itobs) {
      if (itobs->second == backn && message.type() == Event::TLP_DELETE) {
        tlp::debug() << "[Observable info]: An observable onlook itself Event::DELETE msg can't be sent to it." << endl;
        // treat scheduled event
        _oEventsToTreat[backn] -= 2;
        continue;
      }

      // treat scheduled event
      _oEventsToTreat[itobs->second] -= 1;

      if (_oAlive[itobs->second]) { //other listeners/observers could be destroyed during the treat event
#ifndef NDEBUG
        ++(itobs->first->received);
#endif
        itobs->first->treatEvents(tmp);
      }

      // we decrement after treating event
      // to prevent reuse of backn
      _oEventsToTreat[backn] -= 1;

      assert(_oAlive[backn]);

      if (!_oAlive[backn]) {
        throw ObservableException("An observable has been deleted during the notifification of its observer (ie. an observer has deleted its caller during an update)");
      }
    }
  }

#ifndef NDEBUG
  ++sent;
#endif
  --_oNotifying;

  if (!observerTonotify.empty() || !listenerTonotify.empty() || message.type() == Event::TLP_DELETE)
    updateObserverGraph();
}
//----------------------------------------
void Observable::updateObserverGraph() {
  //tlp::debug() << __PRETTY_FUNCTION__ << endl << flush;
  if (_oNotifying == 0 && _oUnholding == 0 && _oHoldCounter == 0) {
    vector<node>::const_iterator itNodes;
#ifdef _OPENMP
    #pragma omp critical(ObservableGraphUpdate)
#endif
    {
      for( itNodes = _oDelayedDelNode.begin(); itNodes != _oDelayedDelNode.end(); ++itNodes) {
        node toDel = *itNodes;

        if (_oEventsToTreat[toDel] == 0)
          _oGraph.delNode(toDel);
      }
    }
    _oDelayedDelNode.clear();
  }
}
//----------------------------------------
void Observable::removeOnlooker(const Observable &obs, OBSERVABLEEDGETYPE type) const {
  // nothing to do if one of the observables is unbound
  if (!isBound() || !obs.isBound())
    return;

#ifdef _OPENMP
  #pragma omp critical(ObservableGraphUpdate)
#endif
  {
    assert(_oAlive[_n]);

    if (!_oAlive[_n]) {
      throw ObservableException("removeOnlooker called on a deleted Observable");
    }

    edge link(_oGraph.existEdge(obs.getNode(), getNode()));

    if (link.isValid()) {
      _oType[link] = _oType[link] & ~type; //bitwise operation to remove the bit  for the given type on the edge

      if (_oType[link] == 0)
        _oGraph.delEdge(link);
    }
  }
}
//----------------------------------------
void Observable::removeObserver(Observable  * const observerver) const {
  assert(observerver != NULL);
  removeOnlooker(*observerver, OBSERVER);
}
//----------------------------------------
void Observable::removeListener(Observable  * const listener) const {
  assert(listener != NULL);
  removeOnlooker(*listener, LISTENER);
}
//----------------------------------------
void Observable::notifyObservers() {
  if (!isBound())
    return;

  assert(_oAlive[_n]);

  if (!_oAlive[_n]) {
    throw ObservableException("notifyObservers called on a deleted Observable");
  }

  if (_oGraph.indeg(_n) > 0) // hasOnlookers
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//----------------------------------------
bool Observable::hasOnlookers() const {
  if (!isBound())
    return false;

  assert(_oAlive[_n]);

  if (!_oAlive[_n]) {
    throw ObservableException("hasOnlookers called on a deleted Observable");
  }

  return _oGraph.indeg(_n) > 0;
}
//----------------------------------------
unsigned int Observable::countListeners() const {
  if (!hasOnlookers())
    return 0;

  unsigned int result = 0;
  node n;
  forEach(n, (new FilterIterator<node, LinkFilter<LISTENER> >(_oGraph.getInNodes(getNode()), LinkFilter<LISTENER>(_oGraph, _oType, getNode()))))
  ++result;
  return result;
}
//----------------------------------------
unsigned int Observable::countObservers() const {
  if (!hasOnlookers())
    return 0;

  unsigned int result = 0;
  node n;
  forEach(n, (new FilterIterator<node, LinkFilter<OBSERVER> >(_oGraph.getInNodes(getNode()), LinkFilter<OBSERVER>(_oGraph, _oType, getNode()))))
  ++result;
  return result;
}
}
