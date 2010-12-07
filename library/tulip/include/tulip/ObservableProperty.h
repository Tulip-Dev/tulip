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
#ifndef PROPERTYOBSERVABLE_H
#define PROPERTYOBSERVABLE_H
#include <list>
#include "tulip/tulipconf.h"
#include "tulip/Node.h"
#include "tulip/Edge.h"

namespace tlp {

class PropertyInterface;
class ObservableProperty;
//=========================================================

/** \addtogroup properties */ 
/*@{*/
/// Observer for Property
/**
 * The Observer pattern is described in the lecture notes and pp293-304 
 * of Design Patterns by Gamma, Helm, Johnson, and Vlissides. It is a 
 * framework for handling state dependency between observer and observed 
 * object.
 */
class  TLP_SCOPE PropertyObserver {
	std::list<ObservableProperty *> observables;
	bool updateObservables;
public:
	PropertyObserver(bool manageObservables = true)
	:updateObservables(manageObservables) {}
	virtual ~PropertyObserver();
	virtual void beforeSetNodeValue(PropertyInterface*, const node){}
	virtual void afterSetNodeValue(PropertyInterface*, const node){}
	virtual void beforeSetEdgeValue(PropertyInterface*, const edge){}
	virtual void afterSetEdgeValue(PropertyInterface*, const edge){}
	virtual void beforeSetAllNodeValue(PropertyInterface*){}
	virtual void afterSetAllNodeValue(PropertyInterface*){}
	virtual void beforeSetAllEdgeValue(PropertyInterface*){}
	virtual void afterSetAllEdgeValue(PropertyInterface*){}
	virtual void destroy(PropertyInterface*){}

	void addObservable(ObservableProperty *);
	void removeObservable(ObservableProperty *);
};
/*@}*/
}

namespace tlp {

/** \addtogroup properties */ 
/*@{*/
/**
 */
/// Observable object for Property
class  TLP_SCOPE ObservableProperty {
	friend class PropertyObserver;

public:
	virtual ~ObservableProperty() {removePropertyObservers();}
	/**
	 * Register a new observer
	 */
	void addPropertyObserver(PropertyObserver *) const;
	/**
	 * Returns the number of observers
	 */
	unsigned int countPropertyObservers();
	/**
	 * Remove an observer
	 */
	void removePropertyObserver(PropertyObserver *) const;
	/**
	 * Remove all observers
	 */
	void removePropertyObservers();

protected:
	void notifyBeforeSetNodeValue(PropertyInterface*,const node n);
	void notifyAfterSetNodeValue(PropertyInterface*,const node n);
	void notifyBeforeSetEdgeValue(PropertyInterface*,const edge e);
	void notifyAfterSetEdgeValue(PropertyInterface*,const edge e);
	void notifyBeforeSetAllNodeValue(PropertyInterface*);
	void notifyAfterSetAllNodeValue(PropertyInterface*);
	void notifyBeforeSetAllEdgeValue(PropertyInterface*);
	void notifyAfterSetAllEdgeValue(PropertyInterface*);
	void notifyDestroy(PropertyInterface*);
	void removeOnlyPropertyObserver(PropertyObserver *) const;
	mutable std::list<PropertyObserver*> observers;
};
/*@}*/

inline unsigned int ObservableProperty::countPropertyObservers() { 
	return observers.size();
}

inline void ObservableProperty::removePropertyObserver(PropertyObserver *item) const{
	if (!observers.empty() && std::find(observers.begin(), observers.end(), item) != observers.end()) {
		observers.remove(item);
		item->removeObservable((ObservableProperty*)this);
	}
}

inline void ObservableProperty::removeOnlyPropertyObserver(PropertyObserver *item) const{
	if (!observers.empty() && std::find(observers.begin(), observers.end(), item) != observers.end()) {
		observers.remove(item);
	}
}

}

#endif

