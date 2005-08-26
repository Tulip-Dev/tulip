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

class PProxy;
class SuperGraph;

/**
   This class is a pool of PropertyProxy, It also manages the inheritance
   of PropertyProxy beetween views.
*/
class PropertyManager {

public:
  //  PProxy* currentPropertyProxy;
  SuperGraph *superGraph;
  ///
  virtual ~PropertyManager(){};
  /**
     Return true if the propertyProxy is in the pool, or 
     in a ascendant SuperGraph
  */
  virtual  bool existProperty(const std::string&)=0;
  /**Return true if the propertyProxy is in the pool*/
  virtual  bool existLocalProperty(const std::string&)=0;
  /**Add a PropertyProxy in the pool*/
  virtual  void setLocalProxy(const std::string&,PProxy *)=0;
  /**Return a pointer to a propertyProxy which is in the pool or
     in a pool of an ascendant SuperGraph*/
  virtual  PProxy* getProperty(const std::string&)=0;
  /**Return a pointer to a propertyProxy which is in the pool*/
  virtual  PProxy* getLocalProperty(const std::string&)=0;
  /**Remove a PropertyProxy from the pool*/
  virtual  void delLocalProperty(const std::string&)=0;
  /**Used to inform the pool that a node doesn't belong anymore to the
     associated SuperGraph*/
  virtual void erase(const node )=0;
  /**Used to inform the pool that an edge doesn't belong anymore to the
     associated SuperGraph*/
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
  std::map<std::string,PProxy*>::iterator it,itEnd;
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
  std::map<std::string,PProxy*> propertyProxyMap;

public:
  explicit  PropertyManagerImpl(SuperGraph*);
  ~PropertyManagerImpl();
  //======================================================================================
  bool existProperty(const std::string&);
  bool existLocalProperty(const std::string&);
  void setProxy(const std::string&,PProxy *);
  void setLocalProxy(const std::string&,PProxy *);
  PProxy* getProperty(const std::string&);
  PProxy* getLocalProperty(const std::string&);
  void delProxy(const std::string&);
  void delLocalProperty(const std::string&);
  void erase(const node );
  void erase(const edge );
  //======================================================================================
  Iterator<std::string>* getLocalProperties();
  Iterator<std::string>* getInheritedProperties();
};

#endif
