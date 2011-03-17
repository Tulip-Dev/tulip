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
#include <typeinfo>
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
            return dynamic_cast<Observable*>(object[n]);
        }
        tlp::NodeProperty<OLOObject *> object;
    } node2Observable;
    //======================================================================
    struct Node2Onlooker {
        Onlooker* operator()(tlp::node n) {
            return dynamic_cast<Onlooker*>(object[n]);
        }
        tlp::NodeProperty<OLOObject *> object;
    } node2Onlooker;
    //======================================================================
    struct AliveFilter {
        bool operator()(tlp::node n) {
            return alive[n];
        }
        tlp::NodeProperty<bool> alive;
    } objectAlive;
}


namespace tlp {

    //=================================
    VectorGraph               OLOObject::oGraph;
    unsigned int              OLOObject::notifying   = 0;
    unsigned int              OLOObject::unholding   = 0;
    unsigned int              OLOObject::holdCounter = 0;
    NodeProperty<OLOObject *> OLOObject::oPointer;
    NodeProperty<bool >       OLOObject::oAlive;
    vector<node>              OLOObject::delayedDelNode;
    bool                      OLOObject::_initialized = OLOObject::init();
    bool                      Observable::eventQueued = false;
    //----------------------------------
    OLOObject::OLOObject():n(oGraph.addNode()) {
        oPointer[n] = this;
        oAlive[n]   = true;
    }
    //----------------------------------
    OLOObject::~OLOObject() {
        oAlive[n] = false;
        if (notifying == 0 && unholding == 0 && holdCounter == 0) {
            oGraph.delNode(n);
        }
        else {
            delayedDelNode.push_back(n);
            oGraph.delEdges(n);
        }
    }
    //----------------------------------
    Iterator<node> * OLOObject::getInObjects() const {
        return new FilterIterator<node, AliveFilter>(oGraph.getInNodes(n), objectAlive);
    }
    //----------------------------------
    Iterator<node> * OLOObject::getOutObjects() const {
        return new FilterIterator<node, AliveFilter>(oGraph.getOutNodes(n), objectAlive);
    }
    //----------------------------------
    node OLOObject::getNode() const {
        return n;
    }
    //----------------------------------
    bool   OLOObject::isAlive(tlp::node n) {
        return oAlive[n];
    }

    //----------------------------------
    OLOObject* OLOObject::getObject(node n) {
        if (!oAlive[n])
            throw OLOException("That object has been deleted it is no more accessbile");
        return oPointer[n];
    }
    //----------------------------------
    bool OLOObject::init() {
        OLOObject::oGraph.alloc(OLOObject::oPointer);
        OLOObject::oGraph.alloc(OLOObject::oAlive);
        objectAlive.alive = OLOObject::oAlive;
        node2Observable.object = OLOObject::oPointer;
        node2Onlooker.object = OLOObject::oPointer;
        return true;
    }
    //----------------------------------
    const tlp::VectorGraph& OLOObject::getOLOGraph() {
        return OLOObject::oGraph;
    }
    //=================================
    Event::Event(const Observable &sender, EventType type):
            _sender(sender.getNode()),
            _type(type) {
        if (_type == DELETE)
            throw OLOException("It is forbidden to create a delete events, DeleteEvents are autmotically generated at the observable destruction");
    }
    //----------------------------------
    Event::~Event() {
    }
    //----------------------------------
    Observable* Event::sender() const {
        return dynamic_cast<Observable*>(OLOObject::getObject(_sender)); /** only Observable can be use to create event */
    }
    //=================================
    Iterator<Observable *> *Onlooker::getObservables() const {
        return new ConversionIterator<node, Observable*, Node2Observable>(getOutObjects(), node2Observable);
    }
    //=================================
    void Observer::treatEvents(const  std::vector<Event> &events ) {
      if (events[0].type() == Event::DELETE) {
	observableDestroyed(events[0].sender());
	return;
      }
      std::set<Observable*> observables;
      for(size_t k=0; k < events.size(); ++k) {
        observables.insert(events[k].sender());
      }
      update(observables.begin(), observables.end());
    }
    //=================================
    void Observer::update(std::set<Observable*>::iterator, std::set<Observable*>::iterator) {
      std::cout << __PRETTY_FUNCTION__ << " : non implemented" << std::endl;
    }
    //=================================
    void Observer::observableDestroyed(Observable *) {
      std::cout << __PRETTY_FUNCTION__ << " : non implemented" << std::endl;
    }
    //=================================
    Observable::Observable():deleteMsgSent(false), queuedEvent(*this, Event::INVALID) {
    }
    //----------------------------------------
    Observable::~Observable() {
        if (!deleteMsgSent)
            observableDeleted();
    }
    //----------------------------------------
    void Observable::holdObservers() {
        ++holdCounter;
    }
    //----------------------------------------
    void Observable::unholdObservers() {
        if (holdCounter == 0)
            throw OLOException("unhold call without a previous call to hold");
        --holdCounter;
        if (holdCounter > 0 || !eventQueued ) return;
        ++unholding;
        ++holdCounter; /** rehold the oberver to buffer message sent during unholding */

        map<node, vector<Event> > preparedEvents;
        node n;
        forEach(n, OLOObject::oGraph.getNodes()) {
            if (OLOObject::oAlive[n]) {
                Observable *observable = dynamic_cast<Observable *>(OLOObject::oPointer[n]);
                if (observable) {
                    if (observable->queuedEvent.type() == Event::INVALID) continue;
                    Onlooker *observer;
                    forEach(observer, observable->getOnlookers()) {
                        if (dynamic_cast<Observer *>(observer)) {
                            preparedEvents[observer->getNode()].push_back(observable->queuedEvent);
                        }
                    }
                    observable->queuedEvent._type = Event::INVALID;
                }
            }
        }

        eventQueued = false;

        map<node, vector<Event> >::const_iterator it;
        for (it = preparedEvents.begin(); it!=preparedEvents.end(); ++it) {
            if (oAlive[it->first]) {
                Observer *obs  = dynamic_cast<Observer *>(oPointer[it->first]);
                obs->treatEvents(it->second);
            }
        }

        -- unholding;
        unholdObservers(); /** treat queued events during the unhold */
        updateObserverGraph();
        if (holdCounter > 0) {
            throw OLOException("Observable after unholdd call, bad nested hold/unhold function call in an Observer:: update() function");
        }
    }
    //----------------------------------------
    Iterator<Onlooker *> *Observable::getOnlookers() const {
        if (!oAlive[n]) {
            throw OLOException("getObservers called on a deleted Observable");
        }
        return new ConversionIterator<node, Onlooker*, Node2Onlooker>(getInObjects(), node2Onlooker);
    }
    //----------------------------------------
    void Observable::addOnlooker(const Onlooker &obs) {
        if (!oAlive[n]) {
            throw OLOException("addObserver called on a deleted Observable");
        }
        if (!oGraph.existEdge(obs.getNode(), getNode()).isValid()) {
            oGraph.addEdge(obs.getNode(), getNode());
        }
        else {
#ifndef NDEBUG
            cerr << "[OLO Warning]: observer already connected" << endl;
#endif
        }
    }
    //----------------------------------------
    void Observable::addObserver(Observer * const obs) {
      assert(obs != 0);
      addOnlooker(*obs);
    }
    //----------------------------------------
    void Observable::observableDeleted() {
        if (deleteMsgSent) {
            throw OLOException("Delete message has been sent several time.");
        }
        deleteMsgSent = true;
        if (hasOnlookers()) {
            Event msg(*this, Event::INVALID); //create a modify event to prevent raise exception, (hack) to forbid creation of Delete exception without calling that fucnton
            msg._type = Event::DELETE;
            sendEvent(msg);
        }
    }
    //----------------------------------------
    void Observable::sendEvent(const Event &message) {
        if (!oAlive[n]) {
            throw OLOException("Notify called on a deleted Observable");
        }
        const unsigned int RECCALL = 200;
        if (notifying > RECCALL) {
            std::stringstream str;
            str << "Maximum number of nested calls (" << RECCALL << ") reached, contact tulip team if that limit is too restrictive";
            throw OLOException(str.str());
            return;
        }
        node backn = n; /** to keep trace of the node if the observable is deleted during the notification, in that crazy case, (*this) is dead thus n is not accessible*/
        ++notifying;
        node ni;
        vector< pair<Observer *, node > > observerTonotify;
        stableForEach(ni, oGraph.getInNodes(n)) {
            if (oAlive[ni]) {
                if (ni == n && message.type() == Event::DELETE) {
                    cout << "[OLO info]: An observable olook itself Event::DELETE msg can't be sent to it." << endl;
                    continue;
                }
                Observer * hobs = dynamic_cast<Observer*>(oPointer[ni]);
                if (!hobs)
                    dynamic_cast<Listener*>(oPointer[ni])->treatEvent(message);
                else {
                    if (message.type() == Event::MODIFICATION ||  message.type() == Event::DELETE) {
                        if (holdCounter == 0  || message.type() == Event::DELETE) {
                            observerTonotify.push_back(pair<Observer *, node >(hobs, ni));
                        }
                        else {
                            queuedEvent = message;
                            eventQueued = true;
                        }
                    }
                }
                if (!oAlive[backn]) {
                    throw OLOException("An observable has been deleted during the notifification of its observer (ie. an observer has deleted its caller during an update)");
                }
            }
        }
        if (!observerTonotify.empty()) {
            vector<Event> tmp;
            tmp.push_back(message);
            for (size_t i=0; i<observerTonotify.size(); ++i) {
                if (oAlive[observerTonotify[i].second])
                    observerTonotify[i].first->treatEvents(tmp);
                if (!oAlive[backn]) {
                    throw OLOException("An observable implied in the notification process has been deleted during the notifification of its observer (ie. an observer has deleted its caller during an update)");
                }
            }
        }

        --notifying;
        updateObserverGraph();
    }
    //----------------------------------------
    void Observable::updateObserverGraph() {
        //cout << __PRETTY_FUNCTION__ << endl << flush;
        if (notifying == 0 && unholding == 0 && holdCounter == 0) {
            vector<node>::const_iterator itNodes;
            for( itNodes = delayedDelNode.begin(); itNodes != delayedDelNode.end(); ++itNodes) {
                oGraph.delNode(*itNodes);
            }
            delayedDelNode.clear();
        }
    }
    //----------------------------------------
    void Observable::removeOnlooker(const Onlooker &obs) {
        if (!oAlive[n]) {
            throw OLOException("removeObserver called on a deleted Observable");
        }
        edge link(oGraph.existEdge(obs.getNode(), getNode()));
        if (link.isValid())
            oGraph.delEdge(link);
    }
    //----------------------------------------
    void Observable::removeObserver(Observer  * const obs) {
      assert(obs != 0);
      removeOnlooker(*obs);
    }
    //----------------------------------------
    void Observable::notifyObservers() {
      if(hasOnlookers())
        sendEvent(Event(*this, Event::MODIFICATION));
    }
    //----------------------------------------
    void Observable::notifyDestroy() {
      std::cout << "notify destroy no more useful" << std::endl;
    }
    //----------------------------------------
    unsigned int Observable::countObservers() const {
      return countOnLookers();
    }
    //----------------------------------------
    unsigned int Observable::countOnLookers() const {
      Onlooker *obs ;
      unsigned int result = 0;
      forEach(obs, getOnlookers())
        ++result;
      return result;
    }
    //----------------------------------------
    bool Observable::hasOnlookers() const {
        if (!oAlive[n]) {
            throw OLOException("observed called on a deleted Observable");
        }
        return oGraph.indeg(getNode()) > 0;
    }
    //----------------------------------------
    void Observable::removeOnlookers() {
        if (!oAlive[n]) {
            throw OLOException("remove Observers called on a deleted Observable");
        }
        Onlooker *obs;
        stableForEach(obs, getOnlookers())
                removeOnlooker(*obs);
    }
    //----------------------------------------
}
