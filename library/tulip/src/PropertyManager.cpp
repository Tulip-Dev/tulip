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

using namespace std;
using namespace tlp;

//==============================================================
PropertyManagerImpl::PropertyManagerImpl(Graph *spGr) {
  graph=spGr;
}
//==============================================================
PropertyManagerImpl::~PropertyManagerImpl() {
  map<string,PropertyInterface*>::iterator itP;
  for (itP=propertyProxyMap.begin();itP!=propertyProxyMap.end();++itP) {
    delete (*itP).second;
  }
}
//==============================================================
bool PropertyManagerImpl::existProperty(const string &str) {
  if (existLocalProperty(str)) 
    return true;
  else {
    if (graph->getFather()==graph)
        return false;
    else 
      return (graph->getFather()->existProperty(str));
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
      return (graph->getFather()->getProperty(str));
    }
  return 0;
}
//==============================================================
PropertyInterface* PropertyManagerImpl::getLocalProperty(const string &str) {
  assert(existLocalProperty(str));
  return (propertyProxyMap[str]);
}
//==============================================================
void PropertyManagerImpl::delLocalProperty(const string &str) {
  PropertyInterface *tmpM;
  map<string,PropertyInterface *>::iterator it;
  it=propertyProxyMap.find(str);
  if (it!=propertyProxyMap.end()) {
    tmpM=(*it).second;
    propertyProxyMap.erase(it);
    delete tmpM;
  }
}
Iterator<string>*  PropertyManagerImpl::getLocalProperties() {
  return (new LocalPropertiesIterator(this));
}
Iterator<string>*  PropertyManagerImpl::getInheritedProperties() {
  return (new InheritedPropertiesIterator(this));
}
//==============================================================
LocalPropertiesIterator::LocalPropertiesIterator(PropertyManagerImpl *ppc) {
  this->ppc=ppc;
  it=ppc->propertyProxyMap.begin();
  itEnd=ppc->propertyProxyMap.end();
}
string LocalPropertiesIterator::next() {
  string tmp=(*it).first;
  ++it;
  return tmp;
}
bool LocalPropertiesIterator::hasNext() {
  return (it!=itEnd);
}
//===============================================================
InheritedPropertiesIterator::InheritedPropertiesIterator(PropertyManager *ppc) {
  this->ppc=ppc; 
  if (ppc->graph->getFather()!=ppc->graph) {
    //Récupération des propriétées locale du père.
    Iterator<string> *itS=ppc->graph->getFather()->getLocalProperties();
    for (;itS->hasNext();) {
      string tmp=itS->next();
      if (!ppc->existLocalProperty(tmp)) {
	inhList.insert(tmp);
      }
    } delete itS;
    //Récupération des propriétées héritées du père.
    itS=ppc->graph->getFather()->getInheritedProperties();
    for (;itS->hasNext();) {
      string tmp=itS->next();
      if (!ppc->existLocalProperty(tmp)) {
	inhList.insert(tmp);
      }
    }
    delete itS;  
  }
  it=inhList.begin();
  itEnd=inhList.end();
}
string InheritedPropertiesIterator::next() {
  string tmp=(*it);
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
