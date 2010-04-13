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

#include "tulip/ForEach.h"
#include "tulip/GraphAbstract.h"
#include "tulip/PropertyManager.h"
#include "tulip/AbstractProperty.h"
#include "tulip/GraphProperty.h"

namespace tlp {
//======================================================================================
class PropertiesIterator: public Iterator<PropertyInterface*> {
 public:
  PropertiesIterator(std::map<std::string, PropertyInterface*>::iterator,
		     std::map<std::string, PropertyInterface*>::iterator);
  PropertyInterface* next();
  bool hasNext();
  std::map<std::string, PropertyInterface*>::iterator it, itEnd;
};
//==============================================================
PropertiesIterator::PropertiesIterator(std::map<std::string, PropertyInterface*>::iterator itB,
				       std::map<std::string, PropertyInterface*>::iterator itE): it(itB), itEnd(itE) {
}
PropertyInterface* PropertiesIterator::next() {
  PropertyInterface* tmp=(*it).second;
  ++it;
  return tmp;
}
bool PropertiesIterator::hasNext() {
  return (it!=itEnd);
}
//======================================================================================
class PropertyNamesIterator: public Iterator<std::string> {
 public:
  PropertyNamesIterator(std::map<std::string, PropertyInterface*>::iterator itB,
			std::map<std::string, PropertyInterface*>::iterator itE)
    :itProp(itB, itE) {}
  std::string next() {
    std::string tmp=(*(itProp.it)).first;
    ++(itProp.it);
    return tmp;
  }
    
  bool hasNext() {
    return itProp.hasNext();
  }
  PropertiesIterator itProp;
};

}

using namespace std;
using namespace tlp;

const string metaGraphPropertyName = "viewMetaGraph";

//==============================================================
PropertyManager::PropertyManager(Graph *g): graph(g) {
  // get inherited properties
  if (graph != graph->getSuperGraph()) {
    Iterator<PropertyInterface *> *it = graph->getSuperGraph()->getObjectProperties();
    while(it->hasNext()) {
      PropertyInterface* prop = it->next();
      inheritedProperties[prop->getName()] = prop;
      if (prop->getName() == metaGraphPropertyName)
	((GraphAbstract *) graph)->metaGraphProperty = (GraphProperty *) prop;
    } delete it;
  }
}
//==============================================================
PropertyManager::~PropertyManager() {
  map<string,PropertyInterface*>::const_iterator itP;
  for (itP=localProperties.begin();itP!=localProperties.end();++itP) {
    PropertyInterface *prop = (*itP).second;
    prop->graph = NULL;
    delete prop;
  }
}
//==============================================================
bool PropertyManager::existProperty(const string &str) {
  return existLocalProperty(str) || existInheritedProperty(str);
}
//==============================================================
bool PropertyManager::existLocalProperty(const string &str) {
  return (localProperties.find(str)!=localProperties.end());
}
//==============================================================
bool PropertyManager::existInheritedProperty(const string &str) {
  return (inheritedProperties.find(str)!=inheritedProperties.end());
}
//==============================================================
void PropertyManager::setLocalProperty(const string &str,
				       PropertyInterface *p) {
  if (existLocalProperty(str))
    // delete previously existing local property
    delete localProperties[str];
  else {
    // remove previously existing inherited property
    map<string, PropertyInterface *>::iterator it;
    it = inheritedProperties.find(str);
    if (it != inheritedProperties.end())
      inheritedProperties.erase(it);
  }

  // register property as local
  localProperties[str] = p;

  // loop on subgraphs
  Graph* sg;
  forEach(sg, graph->getSubGraphs()) {
    // to set p as inherited property
    (((GraphAbstract *) sg)->propertyContainer)->setInheritedProperty(str, p);
  }
}
//==============================================================
void PropertyManager::setInheritedProperty(const string &str,
					   PropertyInterface *p) {
  if (!existLocalProperty(str)) {
    inheritedProperties[str] = p;
    if (str == metaGraphPropertyName)
      ((GraphAbstract *) graph)->metaGraphProperty = (GraphProperty *) p;

    // loop on subgraphs
    Graph* sg;
    forEach(sg, graph->getSubGraphs()) {
      // to set p as inherited property
      (((GraphAbstract *) sg)->propertyContainer)->setInheritedProperty(str, p);
    }
  }
}
//==============================================================
PropertyInterface* PropertyManager::getProperty(const string &str) {
  assert(existProperty(str));
  if (existLocalProperty(str))
    return getLocalProperty(str);
  if (existInheritedProperty(str))
    return getInheritedProperty(str);
  return NULL;
}
//==============================================================
PropertyInterface* PropertyManager::getLocalProperty(const string &str) {
  assert(existLocalProperty(str));
  return (localProperties[str]);
}
//==============================================================
PropertyInterface* PropertyManager::getInheritedProperty(const string &str) {
  assert(existInheritedProperty(str));
  return (inheritedProperties[str]);
}
//==============================================================
void PropertyManager::delLocalProperty(const string &str) {
  map<string,PropertyInterface *>::iterator it;
  it = localProperties.find(str);
  // if found remove from local properties
  if (it!=localProperties.end()) {
    PropertyInterface* oldProp = (*it).second;
    localProperties.erase(it);
    if (graph->canDeleteProperty(graph, oldProp))
      //if (!graph->canPop())
      delete oldProp;

    // loop in the ascendant hierarchy to get
    // an inherited property
    PropertyInterface* newProp = NULL;
    Graph* g = graph;
    while (g != g->getSuperGraph()) {
      g = g->getSuperGraph();
      if (g->existLocalProperty(str)) {
	newProp = g->getProperty(str);
	break;
      }
    }

    if (newProp)
      setInheritedProperty(str, newProp);
    else {
      // loop on subgraphs
     forEach(g, graph->getSubGraphs()) {
       // to remove old inherited property
       (((GraphAbstract *) g)->propertyContainer)->delInheritedProperty(str);
     }
    }
  }
}
//==============================================================
void PropertyManager::delInheritedProperty(const string &str) {
  map<string,PropertyInterface *>::iterator it;
  it = inheritedProperties.find(str);
  // if found remove from inherited properties
  if (it != inheritedProperties.end()) {
    inheritedProperties.erase(it);
    // loop on subgraphs
    Graph* sg;
    forEach(sg, graph->getSubGraphs()) {
      // to remove as inherited property
      (((GraphAbstract *) sg)->propertyContainer)->delInheritedProperty(str);
    }
  }
}
Iterator<string>*  PropertyManager::getLocalProperties() {
  return (new PropertyNamesIterator(localProperties.begin(),
				    localProperties.end()));
}
Iterator<string>*  PropertyManager::getInheritedProperties() {
  return (new PropertyNamesIterator(inheritedProperties.begin(),
				    inheritedProperties.end()));
}
Iterator<PropertyInterface*>*  PropertyManager::getLocalObjectProperties() {
  return (new PropertiesIterator(localProperties.begin(),
				 localProperties.end()));
}
Iterator<PropertyInterface*>*  PropertyManager::getInheritedObjectProperties() {
  return (new PropertiesIterator(inheritedProperties.begin(),
				 inheritedProperties.end()));
}
//===============================================================
void PropertyManager::erase(const node n) {
  map<string,PropertyInterface*>::iterator itP;
  for (itP=localProperties.begin();itP!=localProperties.end();++itP) {
    itP->second->erase(n);
  }
}
//===============================================================
void PropertyManager::erase(const edge e) {
  map<string,PropertyInterface*>::iterator itP;
  for (itP=localProperties.begin();itP!=localProperties.end();++itP) {
    itP->second->erase(e);
  }
}
