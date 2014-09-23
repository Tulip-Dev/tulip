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

#include <tulip/ForEach.h>
#include <tulip/GraphAbstract.h>
#include <tulip/PropertyManager.h>
#include <tulip/PropertyInterface.h>
#include <tulip/GraphProperty.h>

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
    }

    delete it;
  }
}
//==============================================================
PropertyManager::~PropertyManager() {
  map<string,PropertyInterface*>::const_iterator itP;

  for (itP=localProperties.begin(); itP!=localProperties.end(); ++itP) {
    PropertyInterface *prop = (*itP).second;
    prop->graph = NULL;
    delete prop;
  }
}
//==============================================================
bool PropertyManager::existProperty(const string &str) const {
  return existLocalProperty(str) || existInheritedProperty(str);
}
//==============================================================
bool PropertyManager::existLocalProperty(const string &str) const {
  return (localProperties.find(str)!=localProperties.end());
}
//==============================================================
bool PropertyManager::existInheritedProperty(const string &str) const {
  return (inheritedProperties.find(str)!=inheritedProperties.end());
}
//==============================================================
void PropertyManager::setLocalProperty(const string &str,
                                       PropertyInterface *p) {
  bool hasInheritedProperty=false;

  if (existLocalProperty(str))
    // delete previously existing local property
    delete localProperties[str];
  else {
    // remove previously existing inherited property
    map<string, PropertyInterface *>::iterator it;
    hasInheritedProperty = ((it = inheritedProperties.find(str))!= inheritedProperties.end());

    if (hasInheritedProperty) {
      //Notify property destruction old state.
      notifyBeforeDelInheritedProperty(str);
      //Erase old inherited property
      inheritedProperties.erase(it);
    }
  }

  // register property as local
  localProperties[str] = p;

  //If we had an inherited property notify it's destruction.
  if(hasInheritedProperty) {
    ((GraphAbstract *) graph)->notifyAfterDelInheritedProperty(str);
  }

  // loop on subgraphs
  Graph* sg;
  forEach(sg, graph->getSubGraphs()) {
    // to set p as inherited property
    (((GraphAbstract *) sg)->propertyContainer)->setInheritedProperty(str, p);
  }
}
//==============================================================
bool PropertyManager::renameLocalProperty(PropertyInterface *prop,
    const string &newName) {
  assert(prop && prop->getGraph() == graph);

  if (existLocalProperty(newName))
    return false;

  std::string propName = prop->getName();
  map<string,PropertyInterface *>::iterator it;
  it = localProperties.find(propName);

  if (it == localProperties.end())
    return false;

  assert(it->second == prop);

  // before rename notification
  ((GraphAbstract *) graph)->notifyBeforeRenameLocalProperty(prop, newName);

  // loop in the ascendant hierarchy to get
  // an inherited property
  PropertyInterface* newProp = NULL;
  Graph* g = graph;

  while (g != g->getSuperGraph()) {
    g = g->getSuperGraph();

    if (g->existLocalProperty(propName)) {
      newProp = g->getProperty(propName);
      break;
    }
  }

  //Warn subgraphs for deletion.
  Graph* sg;
  forEach(sg, graph->getSubGraphs()) {
    (((GraphAbstract *) sg)->propertyContainer)->notifyBeforeDelInheritedProperty(propName);
  }

  //Remove property from map.
  localProperties.erase(it);
  //Set the inherited property in this graph and all it's subgraphs.
  (((GraphAbstract *) graph)->propertyContainer)->setInheritedProperty(propName, newProp);

  // remove previously existing inherited property
  bool hasInheritedProperty =
    ((it = inheritedProperties.find(newName))!= inheritedProperties.end());

  if (hasInheritedProperty) {
    //Notify property destruction old state.
    notifyBeforeDelInheritedProperty(newName);
    //Erase old inherited property
    inheritedProperties.erase(it);
  }

  // register property as local
  localProperties[newName] = prop;

  //If we had an inherited property notify it's destruction.
  if(hasInheritedProperty) {
    ((GraphAbstract *) graph)->notifyAfterDelInheritedProperty(newName);
  }

  // loop on subgraphs
  forEach(sg, graph->getSubGraphs()) {
    // to set p as inherited property
    (((GraphAbstract *) sg)->propertyContainer)->setInheritedProperty(newName, prop);
  }

  // update property name
  prop->name = newName;

  // after renaming notification
  ((GraphAbstract *) graph)->notifyAfterRenameLocalProperty(prop, propName);

  return true;
}
//==============================================================
void PropertyManager::setInheritedProperty(const string &str,
    PropertyInterface *p) {
  if (!existLocalProperty(str)) {
    bool hasInheritedProperty = inheritedProperties.find(str)!=inheritedProperties.end();

    if( p != NULL) {
      ((GraphAbstract *) graph)->notifyBeforeAddInheritedProperty(str);
      inheritedProperties[str] = p;

      if (str == metaGraphPropertyName)
        ((GraphAbstract *) graph)->metaGraphProperty = (GraphProperty *) p;
    }
    else {
      // no need for notification
      // already done thru notifyBeforeDelInheritedProperty(str);
      // see setLocalProperty
      inheritedProperties.erase(str);
    }

    if(hasInheritedProperty) {
      ((GraphAbstract *) graph)->notifyAfterDelInheritedProperty(str);
    }

    // graph observers notification
    if( p != NULL) {
      ((GraphAbstract *) graph)->notifyAddInheritedProperty(str);
    }

    // loop on subgraphs
    Graph* sg;
    forEach(sg, graph->getSubGraphs()) {
      // to set p as inherited property
      (((GraphAbstract *) sg)->propertyContainer)->setInheritedProperty(str, p);
    }
  }
}
//==============================================================
PropertyInterface* PropertyManager::getProperty(const string &str) const {
  assert(existProperty(str));

  if (existLocalProperty(str))
    return getLocalProperty(str);

  if (existInheritedProperty(str))
    return getInheritedProperty(str);

  return NULL;
}
//==============================================================
PropertyInterface* PropertyManager::getLocalProperty(const string &str) const {
  assert(existLocalProperty(str));
  return ((PropertyManager *)this)->localProperties[str];
}
//==============================================================
PropertyInterface* PropertyManager::getInheritedProperty(const string &str) const {
  assert(existInheritedProperty(str));
  return ((PropertyManager *)this)->inheritedProperties[str];
}
//==============================================================
void PropertyManager::delLocalProperty(const string &str) {
  map<string,PropertyInterface *>::iterator it;
  it = localProperties.find(str);

  // if found remove from local properties
  if (it!=localProperties.end()) {
    PropertyInterface* oldProp = (*it).second;

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

    //Warn subgraphs.
    Graph* sg;
    forEach(sg, graph->getSubGraphs()) {
      (((GraphAbstract *) sg)->propertyContainer)->notifyBeforeDelInheritedProperty(str);
    }

    //Remove property from map.
    localProperties.erase(it);
    //Set the inherited property in this graph and all it's subgraphs.
    (((GraphAbstract *) graph)->propertyContainer)->setInheritedProperty(str, newProp);

    //Delete property
    //Need to be done after subgraph notification.
    if (graph->canDeleteProperty(graph, oldProp))
      //if (!graph->canPop())
      delete oldProp;
    else
      oldProp->notifyDestroy();
  }
}
//==============================================================
void PropertyManager::notifyBeforeDelInheritedProperty(const string& str) {
  map<string,PropertyInterface *>::iterator it;
  it = inheritedProperties.find(str);

  // if found remove from inherited properties
  if (it != inheritedProperties.end()) {
    // graph observers notification
    ((GraphAbstract *) graph)->notifyBeforeDelInheritedProperty(str);
    // loop on subgraphs
    Graph* sg;
    forEach(sg, graph->getSubGraphs()) {
      // to remove as inherited property
      (((GraphAbstract *) sg)->propertyContainer)->notifyBeforeDelInheritedProperty(str);
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

  for (itP=localProperties.begin(); itP!=localProperties.end(); ++itP) {
    itP->second->erase(n);
  }
}
//===============================================================
void PropertyManager::erase(const edge e) {
  map<string,PropertyInterface*>::iterator itP;

  for (itP=localProperties.begin(); itP!=localProperties.end(); ++itP) {
    itP->second->erase(e);
  }
}
