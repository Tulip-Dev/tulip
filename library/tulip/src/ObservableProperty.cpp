#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableProperty.h"

using namespace std;
using namespace tlp;

void ObservableProperty::notifyBeforeSetNodeValue(PropertyInterface* p,
						  const node n) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->beforeSetNodeValue(p, n);
}

void ObservableProperty::notifyAfterSetNodeValue(PropertyInterface* p,
						  const node n) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->afterSetNodeValue(p, n);
}

void ObservableProperty::notifyBeforeSetEdgeValue(PropertyInterface* p,
					    const edge e) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->beforeSetEdgeValue(p, e);
}

void ObservableProperty::notifyAfterSetEdgeValue(PropertyInterface* p,
					    const edge e) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->afterSetEdgeValue(p, e);
}

void ObservableProperty::notifyBeforeSetAllNodeValue(PropertyInterface* p) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->beforeSetAllNodeValue(p);
}

void ObservableProperty::notifyAfterSetAllNodeValue(PropertyInterface* p) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->afterSetAllNodeValue(p);
}

void ObservableProperty::notifyBeforeSetAllEdgeValue(PropertyInterface* p) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->beforeSetAllEdgeValue(p);
}

void ObservableProperty::notifyAfterSetAllEdgeValue(PropertyInterface* p) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->afterSetAllEdgeValue(p);
}

void ObservableProperty::notifyDestroy(PropertyInterface* p) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->destroy(p);
}

