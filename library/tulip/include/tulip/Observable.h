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

#include <tulip/ForEach.h>
#include <tulip/vectorgraph.h>
#include <set>

namespace tlp {
    /**
      * \addtogroup observable
      */
    /*@{*/
    //=======================================
    /**
      * @class OLOException
      * @brief OLOException is the base class of all exceptions sent by the Observable/Listener/Observer system.
      *
      * @see Listener
      * @see Observer
      * @see Observable
      **/
    class  TLP_SCOPE OLOException : public std::exception {
    public:
        OLOException(const std::string &desc):desc(desc) {
        }
        virtual ~OLOException() throw () {
        }
        virtual const char* what() const throw() {
            return desc.c_str();
        }
    private:
        std::string desc;
    };
    //=======================================
    /**
      * @class OLOObject
      * @brief OLOObject is the base class of the Observable/Listener/Observer object hierachy. It manages storage of a unique id for these objects as well
      *        as the storage of relations between them. It also manages destruction of these objects.
      *
      * Each Observable/listener/Observer is stored in a VectorGraph object as a node. An edge in that graph represents
      * a connection between an Observable and a Listener/Observer. OLOObject enables to manage the creation/destruction
      * of node when a Observable/Listener/Observer is created/deleted. OLOObject also enables to guarantee the coherency of
      * relations between Observable and Listener/Observer.
      *
      * OLOObject provides functions to access to the graph of OLOObjects as well as the state of elements in that graph. Using
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
    class  TLP_SCOPE OLOObject {
        friend class Observable;

    public:
        /**
          * @brief return the node representing that OLOObject in the OLOGraph
          */
        tlp::node   getNode() const;

    public:
        /**
          * @bried return a pointer on the object represented by that node
          * @warning If you call that function during an update, it is possible that the pointer do not point on an existing object, use
          *          the isAlive function before to test if your object still valid
          */
        static OLOObject* getObject(tlp::node n);
        /**
          * @brief Enables to test if the object represented by a node has been deleted. Outside a unhold/hold block or an update that function
          *        always return true.
          */
        static bool  isAlive(tlp::node n);
        /**
          * @brief Return a reference on the Observable/Listener/Observer graph.
          */
        static const tlp::VectorGraph& getOLOGraph();

    protected:
        OLOObject();
	OLOObject(const OLOObject &);
        virtual ~OLOObject();
        /**
          * @brief return an iterator on in objects (Observable), the iterator guarantee that all objects are alive (not deleted during hold or notify)
          */
        tlp::Iterator<tlp::node> *getInObjects() const;
        /**
          * @brief return an iterator on out objects (Listener/Observer), the iterator garantee that all objects are alive (not deleted during hold or notify)
          */
        tlp::Iterator<tlp::node> *getOutObjects() const;

    private:
        static tlp::VectorGraph               oGraph;         /** the graph that store all observers and connection between them*/
        static tlp::NodeProperty<OLOObject *> oPointer;       /** store a pointer to the object represented by a node */
        static tlp::NodeProperty<bool>        oAlive;         /** enables to know if an object has been deleted or not */
        static std::vector<tlp::node>         delayedDelNode; /** store deleted nodes, to remove them at the end of the notify*/
        static unsigned int              notifying;      /** counter of nested notify calls */
        static unsigned int              unholding;      /** counter of nested unhold calls */
        static unsigned int              holdCounter;    /** counter of nested holds */
        static bool                      _initialized;   /** use to initialize oGraph when the library is loaded (nice hack) */

    private:
        tlp::node n; /** node that represent that object in the OLOGraph.*/
        /**
          * @brief Trick to init the OLOGraph properties (automatically call at the loading of the library.
          */
        static bool init();
    };


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
    public:
        enum EventType {TLP_DELETE = 0, TLP_MODIFICATION, TLP_INFORMATION, TLP_INVALID};
        virtual ~Event();
        Observable* sender() const;
        Event(const Observable &sender, EventType type);
        EventType type() const {
            return _type;
        }
    private:
        tlp::node _sender;
        EventType _type;
    };
    //=======================================
    /**
      * @class Onlooker
      * @brief Onlooker is the base class for Observer and Listener, it enbales to unify these two classes.
      *
      * @see Listener
      * @see Observer
      * @see Observable
      * @see Event
      **/
    class  TLP_SCOPE Onlooker : virtual public OLOObject {
    public:
        /**
         * @brief return an iterators on Observable connected to that OnLooker.
         */
        tlp::Iterator<Observable *> *getObservables() const;
    protected:
        virtual ~Onlooker() {
        }
    };
    //=======================================
    /**
      * @class Listener
      * @brief Listener is the base class for the implementation of an Observbale listener.
      *
      * All Observable Listener must inherit from that class. The update function will be call
      * each time an Observable send notification. The event received in parameter is a reference,
      * thus it is possible to dynamic_cast it into any kind of specialized Event.
      *
      * On the destruction of a Listener, there is no need (and you should not) to remove it from
      * the Observable it is listening to. The OLOObject manage
      * automatically disconnection and coherency.
      *
      * The update function is a call back function,  it is invoked by the Observable himself. Thus it is not allowed to delete the sender Observable
      * during an update. Take care not to delete an other observable that could be implied in nested notifying calls (An exception will
      * raised in that case). Even if it is permitted, usually
      * it is not a good practice to delete any existing Observable during an update. The main problem is that it is difficult to know
      * which Observable are implied in nested notify calls.
      * Deleting any Onlooker is safe and is well managed by the mechanism.
      *
      * @warning Destruction of an Observable implied in nested notifying calls during an update will raise an OLOException.
      *
      * @see Observable
      * @see Event
      * @see Observer
      * In the following example we demonstrate the use of a Listener to display all modification applied to a Point:
      *
      * @code
      * class Point : public Observable {
      * public:
      *     ~Point() {
      *       //observableDeleted();
      *     }
*     void setXY(const double x, const double y);
*     void getXY(double &x, double &y) const {
*         x = _x;
*         y = _y;
*     }
* private:
*     double _x, _y;
* };
*
* class PointEvent : public Event {
* public:
*     PointEvent(const Point &p, double px, double py):
*             Event(p, Event::MODIFICATION), prevX(px), prevY(py) {
*     }
*     double prevX, prevY;
* };
*
* void Point::setXY(const double x, const double y) {
*     PointEvent ev(*this, _x, _y);
*     _x = x;
*     _y = y;
*     if (hasOnlookers())
*         notify(ev);
* }
*
* class PointsViewer : public Observer {
* public:
*     void update(const vector<Event> &evs) {
*         for (size_t i=0; i< evs.size(); ++i)
*             if (evs[i].type() == Event::DELETE)  {
*             points.erase(reinterpret_cast<Point *>(evs[i].sender()));
*             cout << "PointsViewer::Point has been deleted :" << reinterpret_cast<Point *>(evs[i].sender()) << endl << flush;
*             cout << "PointsViewer::updating view : ";
*             updateView();
*             return;
*         }
*         cout << "PointsViewer::objects modified : " << evs.size() << " updating view : ";
*         updateView();
*     }
*     void addPoint(Point& p) {
*         points.insert(&p);
*         p.addOnlooker(*this);
*         cout << "PointsViewer::point added : " << &p << endl;
*     }
*     void updateView() {
*         for (set<const Point *>::const_iterator it = points.begin(); it != points.end(); ++it) {
*             double x,y;
*             (*it)->getXY(x, y);
*             cout << "(" << x << "," << y << ")";
*         }
*         cout << endl;
*     }
*
* private:
*     set<const Point *> points;
* };
*
* class PointListener : public Listener {
* public:
*     void update(const Event &ev) {
*         if (ev.type() == Event::DELETE)  {
*             cout << "PointListener::Point has been deleted : " << reinterpret_cast<Point *>(ev.sender()) << endl << flush;
*             return;
*         }
*         const PointEvent *event = dynamic_cast<const PointEvent *>(&ev);
*         if (event) {
*             Point *p = reinterpret_cast<Point *>(ev.sender());
*             double x,y;
*             p->getXY(x, y);
*             cout << "PointListener::Point change (" << event->prevX << "," << event->prevY << ") ==> (" << x << "," << y << ")" << endl << flush;
*         }
*     }
* };
*
* int main() {
*     Point *point[8];
*     PointsViewer pv;
*     PointListener pl;
*     for (int i=0; i < 8; ++i) {
*         point[i] = new Point();
*         point[i]->setXY(i,i);
*         pv.addPoint(*point[i]);
*     }
*     point[4]->addOnlooker(pl);
*     pv.updateView();
*     Observable::holdObservers();//all observers are holded;
*     for (int i=0; i < 4; ++i) {
*         delete point[i]; //pv receive 4 DELETE events (ie 4 call to update function);
*     }
*     for (int j = 0; j<5; ++j) {
*         for (int i=4; i < 8; ++i) {
*             point[i]->setXY(i, j); //no event sent to pv
*         }
*     }
*     Observable::unholdObservers(); //only one call to pointViewer::update with the 4 last event sent by the 4 observed points
*     for (int i=4; i < 8; ++i) {
*         delete point[i]; //pv receive 4 DELETE events (ie 10 call to update function);
*     }
*     return EXIT_SUCCESS;
      * }
      * @endcode
      *
      **/
    class  TLP_SCOPE Listener : virtual public Onlooker {
    public:
        virtual void treatEvent(const Event &) = 0;
    };
    //=======================================
    /**
      * @class Observer
      * @brief Observer is the base class for the implementation of an Observbale Observer.
      *
      * All Observable Observer must derivate from tat class. The update function will be call
      * each time an Observable receive a notification. The event received in parameter is an object not a reference,
      * thus it is not possible to dynamic_cast it. Only the type of the Event can be used by an Observer.
      * The event send to the Observer are of two types (DELETE or MODIFICATION), the DELETE event is always directly sent
      * when the Observable is deleted. For the MODIFICATION event, the event can be delayed according to the observable
      * hold unhold state. In case of hold/unhold of events, the Observer receive only the last event. It means that if there is
      * 100 Event::MODFICICATION between the hold/unhold the Observer receive only one events after the unhold.
      *
      * On the destruction of an Observer, there is no need to remove it from the Observable it is observing. The OLOObject manage
      * automatically disconnection and coherency.
      *
      * Observer are often use to enable updates of visualized elements. The hold/unhold mechanism prevents from redisplaying
      * too many times the view, when large number of modifications are done. The hold/unhold mechanism is global thus all the
      * observers are holded simultaneously when holdObservers function is called.
      *
      * The update function is a call back function,  it is invoked by the Observable himself. Thus it is lot allowed to delete the sender Observable
      * during an update. Even if it is permitted, usually it is not a good idea to delete any existing Observable during an update. Deleting an Onlooker is safe and is well managed by the mechanism.
      *
      * All modification of the OLOObject (creation, destruction, connections), are allowed during an update.
      *
      * @warning Destruction of the Observable during an update will raise an OLOException.
      *
      * @see Observable
      * @see Event
      * @see Observer
      *
      * That simple example demonstrates the use of an observer to maintain a set of points that we want to display (here on stdout).
      *
      * @code
      * class Point : public Observable {
      * public:
      *     ~Point() {
      *       //observableDeleted();
      *     }
*     void setXY(const double x, const double y);
*     void getXY(double &x, double &y) const {
*         x = _x;
*         y = _y;
*     }
* private:
*     double _x, _y;
* };
*
* class PointEvent : public Event {
* public:
*     PointEvent(const Point &p, double px, double py):
*             Event(p, Event::MODIFICATION), prevX(px), prevY(py) {
*     }
*     double prevX, prevY;
* };
*
* void Point::setXY(const double x, const double y) {
*     PointEvent ev(*this, _x, _y);
*     _x = x;
*     _y = y;
*     if (hasOnlookers())
*         notify(ev);
* }
*
* class PointsViewer : public Observer {
* public:
*     void update(const vector<Event> &evs) {
*         for (size_t i=0; i< evs.size(); ++i)
*             if (evs[i].type() == Event::DELETE)  {
*             points.erase(reinterpret_cast<Point *>(evs[i].sender()));
*             cout << "PointsViewer::Point has been deleted :" << reinterpret_cast<Point *>(evs[i].sender()) << endl << flush;
*             cout << "PointsViewer::updating view : ";
*             updateView();
*             return;
*         }
*         cout << "PointsViewer::objects modified : " << evs.size() << " updating view : ";
*         updateView();
*     }
*     void addPoint(Point& p) {
*         points.insert(&p);
*         p.addOnlooker(*this);
*         cout << "PointsViewer::point added : " << &p << endl;
*     }
*     void updateView() {
*         for (set<const Point *>::const_iterator it = points.begin(); it != points.end(); ++it) {
*             double x,y;
*             (*it)->getXY(x, y);
*             cout << "(" << x << "," << y << ")";
*         }
*         cout << endl;
*     }
*
* private:
*     set<const Point *> points;
* };
*
* class PointListener : public Listener {
* public:
*     void update(const Event &ev) {
*         if (ev.type() == Event::DELETE)  {
*             cout << "PointListener::Point has been deleted : " << reinterpret_cast<Point *>(ev.sender()) << endl << flush;
*             return;
*         }
*         const PointEvent *event = dynamic_cast<const PointEvent *>(&ev);
*         if (event) {
*             Point *p = reinterpret_cast<Point *>(ev.sender());
*             double x,y;
*             p->getXY(x, y);
*             cout << "PointListener::Point change (" << event->prevX << "," << event->prevY << ") ==> (" << x << "," << y << ")" << endl << flush;
*         }
*     }
* };
*
* int main() {
*     Point *point[8];
*     PointsViewer pv;
*     PointListener pl;
*     for (int i=0; i < 8; ++i) {
*         point[i] = new Point();
*         point[i]->setXY(i,i);
*         pv.addPoint(*point[i]);
*     }
*     point[4]->addOnlooker(pl);
*     pv.updateView();
*     Observable::holdObservers();//all observers are holded;
*     for (int i=0; i < 4; ++i) {
*         delete point[i]; //pv receive 4 DELETE events (ie 4 call to update function);
*     }
*     for (int j = 0; j<5; ++j) {
*         for (int i=4; i < 8; ++i) {
*             point[i]->setXY(i, j); //no event sent to pv
*         }
*     }
*     Observable::unholdObservers(); //only one call to pointViewer::update with the 4 last event sent by the 4 observed points
*     for (int i=4; i < 8; ++i) {
*         delete point[i]; //pv receive 4 DELETE events (ie 10 call to update function);
*     }
*     return EXIT_SUCCESS;
      * }

      * @endcode
      *
      **/
    class  TLP_SCOPE Observer : virtual public Onlooker {
    public:
      Observer() {}
      _DEPRECATED Observer(bool){}
      /**
       * that function should be virtual pure
       * the current implementation is only here to insure compatibility with
       * tulip 3.x x<6.
       */
      virtual void treatEvents(const  std::vector<Event> &events );
      virtual void _DEPRECATED update(std::set<Observable*>::iterator, std::set<Observable*>::iterator);
      virtual void _DEPRECATED observableDestroyed(Observable *);
    };
    //=======================================
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
    class  TLP_SCOPE Observable: virtual public OLOObject {
    public:
        virtual ~Observable();
        /**
         * @brief return an Iterator on all Onlookers
         * @warning adding or removing Onlooker to that Observable will devalidate the iterator
         * @see StableIterator
         * @see forEach
         * @see stableForEach
         */
        tlp::Iterator<Onlooker *> *getOnlookers() const;
        /**
          * @brief add an Observer/Listener to the observable
          *
          * The added Onlookers will received the next Event sent by the Observable.
          * In case of nested unholding (almost never), calling that function inside hold/unhold block
          * can make the Observer receive an event that has been sent before it was Observing the object.
          */
        void addOnlooker(const Onlooker &);
  /**
   * @brief use for old observer tulip compatibility
  */
  void _DEPRECATED addObserver(Observer * const obs);
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
        void removeOnlooker(const Onlooker &);
  /**
   * @brief use for old observer tulip compatibility
   */
  void _DEPRECATED removeObserver(Observer  * const obs);

	/**
	 * @brief use for old observer tulip compatibility
	 */
	void _DEPRECATED notifyObservers();

	/**
	 * @brief use for old observer tulip compatibility
	 */
	void _DEPRECATED notifyDestroy();

	/**
	 * Return the number of OnLooker (ie : Listeners + Observers)
	 */
	unsigned int countObservers() const ;

	/**
	 * Return the number of OnLooker (ie : Listeners + Observers)
	 */
	unsigned int countOnLookers() const;

  /**
          * @brief remove all Observer/Listener of the observable
          *
          * In case of nested unholding (almost never), Calling that function inside a hold/unhold block could
          * make the Observers not receive an event that was sent when they were connected.
          *
          * @warning removing OnLooker that has been created outside of your code can create serious
          * problem in your application. Objects that are listening/observing could need to receive
          * the events to work properly.
          */
        void removeOnlookers();
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
          * @warning Calling that function when the hold stack is empty raises an OLOException.
          *
          * @see holdObservers
          * @see Observer
          */
        static void unholdObservers();

        static void _DEPRECATED unholdObservers(bool){
          unholdObservers();
        }

        static unsigned int observersHoldCounter() {
          return holdCounter;
        }

    protected:
        Observable();
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

    private:
        static bool eventQueued; /** Indicates if there is at least one event queued */
        /**
          * @brief delete nodes from the OLOGraph that have been preserved to keep coherency and check bad use of the mecanism
          */
        static void updateObserverGraph();

        bool deleteMsgSent; /** use to enable subclasses to send the Event::DELETE in there destructor */
        Event queuedEvent; /** store the last event to send it to Observers after an hold/unhold block temrination */
    };
    //=======================================
    /*@}*/
}


#endif












