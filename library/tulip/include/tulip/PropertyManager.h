//-*-c++-*
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 26/09/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef TLP_PROPERTYMANAGER_H
#define TLP_PROPERTYMANAGER_H

#include <map>
#include <set>
#include <string>
#include "Iterator.h"
#include "Node.h"
#include "Edge.h"

namespace tlp {

class PropertyInterface;
class Graph;

/**
   This class is a pool of AbstractProperty, It also manages the inheritance
   of AbstractProperty beetween views.
*/
class PropertyManager {

public:
  //  PropertyInterface* currentAbstractProperty;
  Graph *graph;
  ///
  virtual ~PropertyManager(){};
  /**
     Return true if the propertyProxy is in the pool, or 
     in a ascendant Graph
  */
  virtual  bool existProperty(const std::string&)=0;
  /**Return true if the propertyProxy is in the pool*/
  virtual  bool existLocalProperty(const std::string&)=0;
  /**Add a AbstractProperty in the pool*/
  virtual  void setLocalProxy(const std::string&,PropertyInterface *)=0;
  /**Return a pointer to a propertyProxy which is in the pool or
     in a pool of an ascendant Graph*/
  virtual  PropertyInterface* getProperty(const std::string&)=0;
  /**Return a pointer to a propertyProxy which is in the pool*/
  virtual  PropertyInterface* getLocalProperty(const std::string&)=0;
  /**Remove a AbstractProperty from the pool*/
  virtual  void delLocalProperty(const std::string&)=0;
  /**Used to inform the pool that a node doesn't belong anymore to the
     associated Graph*/
  virtual void erase(const node )=0;
  /**Used to inform the pool that an edge doesn't belong anymore to the
     associated Graph*/
  virtual void erase(const edge )=0;
  /**Return an iterator on the local properties*/
  virtual Iterator<std::string>* getLocalProperties()=0;
  /**Return an iterator on the inherited properties*/
  virtual Iterator<std::string>* getInheritedProperties()=0;
};

class PropertyManagerImpl;
//======================================================================================
class LocalPropertiesIterator: public Iterator<std::string> {
 public:
  LocalPropertiesIterator(PropertyManagerImpl *ppc);
  std::string next();
  bool hasNext();
 private:
  PropertyManagerImpl *ppc;
  std::map<std::string,PropertyInterface*>::iterator it,itEnd;
};
//======================================================================================
class InheritedPropertiesIterator: public Iterator<std::string> {
  struct ltstr {
    bool operator()(const std::string &s1, const std::string &s2) const {
      return s1.compare(s2) < 0;
    }
  };
 public:
  InheritedPropertiesIterator(PropertyManager *ppc);
  std::string next();
  bool hasNext();
 private:
  PropertyManager *ppc;
  std::set<std::string,ltstr> inhList; 
  std::set<std::string,ltstr>::iterator it,itEnd;
};
//======================================================================================
/**
   Implemantation of the interface PropertyManager.
 */
class PropertyManagerImpl: public PropertyManager {

  friend class LocalPropertiesIterator;

private:
  std::map<std::string,PropertyInterface*> propertyProxyMap;

public:
  explicit  PropertyManagerImpl(Graph*);
  ~PropertyManagerImpl();
  //======================================================================================
  bool existProperty(const std::string&);
  bool existLocalProperty(const std::string&);
  void setProxy(const std::string&,PropertyInterface *);
  void setLocalProxy(const std::string&,PropertyInterface *);
  PropertyInterface* getProperty(const std::string&);
  PropertyInterface* getLocalProperty(const std::string&);
  void delProxy(const std::string&);
  void delLocalProperty(const std::string&);
  void erase(const node );
  void erase(const edge );
  //======================================================================================
  Iterator<std::string>* getLocalProperties();
  Iterator<std::string>* getInheritedProperties();
};

}

#endif
