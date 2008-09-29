#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableProperty.h"

using namespace std;
using namespace tlp;

void ObservableProperty::notifySetNodeValue(PropertyInterface* p,
					    const node n) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->setNodeValue(p, n);
}

void ObservableProperty::notifySetEdgeValue(PropertyInterface* p,
					    const edge e) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->setEdgeValue(p, e);
}

void ObservableProperty::notifySetAllNodeValue(PropertyInterface* p) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->setAllNodeValue(p);
}

void ObservableProperty::notifySetAllEdgeValue(PropertyInterface* p) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->setAllEdgeValue(p);
}

void ObservableProperty::notifyDestroy(PropertyInterface* p) {
  set<PropertyObserver *> copy(observers); //Used to preserve iterators
  for (set<PropertyObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->destroy(p);
}

