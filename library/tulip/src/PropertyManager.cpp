/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/PropertyManager.h"
#include "tulip/Graph.h"
#include "tulip/AbstractProperty.h"

namespace tlp {
//======================================================================================
class LocalPropertiesIterator: public Iterator<PropertyInterface*> {
 public:
  LocalPropertiesIterator(PropertyManagerImpl *ppc);
  PropertyInterface* next();
  bool hasNext();
 private:
  PropertyManagerImpl *ppc;
  std::map<std::string, PropertyInterface*>::iterator it,itEnd;
};
//======================================================================================
class InheritedPropertiesIterator: public Iterator<PropertyInterface*> {
  struct ltProp {
    bool operator()(PropertyInterface* const &p1,
		    PropertyInterface* const &p2) const {
      return p1->getName().compare(p2->getName()) < 0;
    }
  };
 public:
  InheritedPropertiesIterator(PropertyManager *ppc);
  PropertyInterface* next();
  bool hasNext();
 private:
  PropertyManager *ppc;
  std::set<PropertyInterface*,ltProp> inhList; 
  std::set<PropertyInterface*,ltProp>::iterator it,itEnd;
};
//======================================================================================
class PropertyNamesIterator: public Iterator<std::string> {
 public:
  PropertyNamesIterator(Iterator<PropertyInterface*> *it);
  ~PropertyNamesIterator();
  std::string next();
  bool hasNext();
 private:
  Iterator<PropertyInterface*> *itp;
};

}

using namespace std;
using namespace tlp;

//==============================================================
PropertyManagerImpl::PropertyManagerImpl(Graph *spGr) {
  graph=spGr;
}
//==============================================================
PropertyManagerImpl::~PropertyManagerImpl() {
  map<string,PropertyInterface*>::const_iterator itP;
  for (itP=propertyProxyMap.begin();itP!=propertyProxyMap.end();++itP) {
    PropertyInterface *prop = (*itP).second;
    prop->graph = NULL;
    delete prop;
  }
}
//==============================================================
bool PropertyManagerImpl::existProperty(const string &str) {
  if (existLocalProperty(str)) 
    return true;
  else {
    Graph* super = graph->getSuperGraph();
    if (super == graph)
      return false;
    else 
      return (super->existProperty(str));
  }
}
//==============================================================
bool PropertyManagerImpl::existLocalProperty(const string &str) {
  return (propertyProxyMap.find(str)!=propertyProxyMap.end());
}
//==============================================================
void PropertyManagerImpl::setLocalProxy(const string &str, PropertyInterface *p) {
  if (existLocalProperty(str))
    delete propertyProxyMap[str];
  propertyProxyMap[str]=p;
}
//==============================================================
PropertyInterface* PropertyManagerImpl::getProperty(const string &str) {
  assert(existProperty(str));
  //  cerr << "Get Proxy :" << str << endl;
  if (existLocalProperty(str))
    return this->getLocalProperty(str);
  else {
    Graph* super = graph->getSuperGraph();
    if (super != graph)
      return (super->getProperty(str));
  }
  return NULL;
}
//==============================================================
PropertyInterface* PropertyManagerImpl::getLocalProperty(const string &str) {
  assert(existLocalProperty(str));
  return (propertyProxyMap[str]);
}
//==============================================================
PropertyInterface* PropertyManagerImpl::delLocalProperty(const string &str) {
  map<string,PropertyInterface *>::iterator it;
  it=propertyProxyMap.find(str);
  if (it!=propertyProxyMap.end()) {
    PropertyInterface* prop = (*it).second;
    propertyProxyMap.erase(it);
    return prop;
  }
  return NULL;
}
Iterator<string>*  PropertyManagerImpl::getLocalProperties() {
  return (new PropertyNamesIterator(new LocalPropertiesIterator(this)));
}
Iterator<string>*  PropertyManagerImpl::getInheritedProperties() {
  return (new PropertyNamesIterator(new InheritedPropertiesIterator(this)));
}
Iterator<PropertyInterface*>*  PropertyManagerImpl::getLocalObjectProperties() {
  return (new LocalPropertiesIterator(this));
}
Iterator<PropertyInterface*>*  PropertyManagerImpl::getInheritedObjectProperties() {
  return (new InheritedPropertiesIterator(this));
}
//==============================================================
LocalPropertiesIterator::LocalPropertiesIterator(PropertyManagerImpl *ppc) {
  this->ppc=ppc;
  it=ppc->propertyProxyMap.begin();
  itEnd=ppc->propertyProxyMap.end();
}
PropertyInterface* LocalPropertiesIterator::next() {
  PropertyInterface* tmp=(*it).second;
  ++it;
  return tmp;
}
bool LocalPropertiesIterator::hasNext() {
  return (it!=itEnd);
}
//==============================================================
PropertyNamesIterator::PropertyNamesIterator(Iterator<PropertyInterface*>* it): itp(it) {}

PropertyNamesIterator::~PropertyNamesIterator() {
  delete itp;
}

string PropertyNamesIterator::next() {
  return itp->next()->getName();
}
bool PropertyNamesIterator::hasNext() {
  return itp->hasNext();
}
//===============================================================
InheritedPropertiesIterator::InheritedPropertiesIterator(PropertyManager *ppc) {
  this->ppc=ppc; 
  if (ppc->graph->getSuperGraph()!=ppc->graph) {
    //Récupération des propriétées locale du père.
    Iterator<PropertyInterface*> *itP=
      ppc->graph->getSuperGraph()->getLocalObjectProperties();
    for (;itP->hasNext();) {
      PropertyInterface* tmp=itP->next();
      if (!ppc->existLocalProperty(tmp->getName())) {
	inhList.insert(tmp);
      }
    } delete itP;
    //Récupération des propriétées héritées du père.
    itP=ppc->graph->getSuperGraph()->getInheritedObjectProperties();
    for (;itP->hasNext();) {
      PropertyInterface* tmp=itP->next();
      if (!ppc->existLocalProperty(tmp->getName())) {
	inhList.insert(tmp);
      }
    } delete itP;  
  }
  it=inhList.begin();
  itEnd=inhList.end();
}
PropertyInterface* InheritedPropertiesIterator::next() {
  PropertyInterface* tmp=(*it);
  ++it;
  return tmp;
}
bool InheritedPropertiesIterator::hasNext() {
  return (it!=itEnd);
}
//===============================================================
void PropertyManagerImpl::erase(const node n) {
  map<string,PropertyInterface*>::iterator itP;
  for (itP=propertyProxyMap.begin();itP!=propertyProxyMap.end();++itP) {
    itP->second->erase(n);
  }
}
//===============================================================
void PropertyManagerImpl::erase(const edge e) {
  map<string,PropertyInterface*>::iterator itP;
  for (itP=propertyProxyMap.begin();itP!=propertyProxyMap.end();++itP) {
    itP->second->erase(e);
  }
}
