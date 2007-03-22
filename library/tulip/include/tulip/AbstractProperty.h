//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 26/09/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef TULIP_PROPERTIESPROXY_H
#define TULIP_PROPERTIESPROXY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Observable.h"
#include "Types.h"
#include "Property.h"
#include "MutableContainer.h"
#include "ReturnType.h"
#include "tulipconf.h"
#include "TemplateFactory.h"

namespace tlp {

/**
 * \defgroup properties
 */ 
/*@{*/
//=============================================================
class TLP_SCOPE PropertyInterface: public Observable {
public:
  virtual ~PropertyInterface(){}
  virtual void erase(const node) =0;
  virtual void erase(const edge) =0;
  virtual void copy(const node, const node, PropertyInterface *) =0;
  virtual void copy(const edge, const edge, PropertyInterface *) =0;
  virtual PropertyInterface* clonePrototype(Graph *, std::string) =0;
  //=================================================================================
  // Returns a string describing the type of the property.
  // i.e. "graph", "double", "layout", "string", "integer", "color", "size", ...
  virtual std::string getTypename() = 0;
  static  std::string getTypename( PropertyInterface * );

  // Untyped accessors
  virtual std::string getNodeDefaultStringValue() = 0;
  virtual std::string getEdgeDefaultStringValue() = 0;
  virtual std::string getNodeStringValue( const node n ) = 0;
  virtual std::string getEdgeStringValue( const edge e ) = 0;
  virtual bool setNodeStringValue( const node n, const std::string & v ) = 0;
  virtual bool setEdgeStringValue( const edge e, const std::string & v ) = 0;
  virtual bool setAllNodeStringValue( const std::string & v ) = 0;
  virtual bool setAllEdgeStringValue( const std::string & v ) = 0;
};

/**
 * This class is used to store a property of a graph.
 * A property is a couple of two functions:
 * - One from the set of nodes to a set of Tnode value
 * - One from the set of edges to a set of Tedge value
 *
 * A AbstractProperty can be connected or not to a PropertyAlgorithm.
 * In the first case it can be seen as buffer beetween the property and the user.
 * In the second case it is only a memory area for storing data.
 * A AbstractProperty is an observable, so it can be observed by others objects.
 */
template <class Tnode, class Tedge, class TPROPERTY = PropertyAlgorithm<Tnode,Tedge> > 
class TLP_SCOPE AbstractProperty : public PropertyInterface {
  friend class GraphView;

public:
  static TLP_SCOPE TemplateFactory< PropertyFactory<TPROPERTY>, TPROPERTY, PropertyContext > *factory;
  static void initFactory() {
    if (!factory) {
      factory = new TemplateFactory< PropertyFactory<TPROPERTY>, TPROPERTY, PropertyContext >;
    }
  }
  AbstractProperty(Graph *);
  virtual ~AbstractProperty() ;
  /** 
   * Returns the node default value of the property proxy
   * warnning: If the type is a pointer it can produce big memory
   * leak if the user doesn't manage the memory himself
   */
  typename Tnode::RealType getNodeDefaultValue();
  /** 
   * Returns the edge default value of the property proxy
   * warnning: If the type is a pointer it can produce big memory
   * leak if the user doesn't manage the memory himself
   */
  typename Tedge::RealType getEdgeDefaultValue();
  /**
   * Returns the value associated to the node n in this property.
   * If the value is already fixed it is done in constant time.
   * else if it is a computed property (plug-in), it calls the associated property
   * If there is no value and no algorithms it returns the default value
   * depending of the type.
   */
  typename ReturnType<typename Tnode::RealType>::Value getNodeValue(const node n );
  /**
   * Returns the value associated to the node n in this property.
   * If the value is already fixed it is done in constant time.
   * else if it is a computed property (plug-in), it calls the associated property
   * If there is no value and no algorithms it returns the default value
   * depending of the type.
   */
  typename ReturnType<typename Tedge::RealType>::Value getEdgeValue(const edge e);
  /**
   * Set the value of a node n and notify the observers of a modification.
   * Warning : When using computed property (plug-in), if one sets the value of
   * a node n, the plug-in won't be call for n.
   */
  void setNodeValue(const node n, const typename Tnode::RealType &v);
  /**
   * Set the value of an edge and notify the observers of a modification.
   * Warning : When using computed property (plug-in), if one sets the value of
   * a edge e, the plug-in won't be call for e.
   */
  void setEdgeValue(const edge e, const typename Tedge::RealType &v);
  /**
   * Set the value of all nodes and notify the observers
   */
  void setAllNodeValue(const typename Tnode::RealType &v);
  /**
   * Set the value of all edges and notify the observers
   */
  void setAllEdgeValue(const typename Tedge::RealType &v);
  //=================================================================================
  virtual void erase(const node n);
  //=================================================================================
  virtual void erase(const edge e);
  //=================================================================================
  // Because of compilation pb on Windows platform (g++ bug ???)
  // we include the code of the method below instead of having it
  // included in AbstractProperty.cpp
  virtual AbstractProperty<Tnode,Tedge,TPROPERTY>& operator =(AbstractProperty<Tnode,Tedge,TPROPERTY> &proxy) {
      if (this!= &proxy) {
    //=============================================================
    //The backup is necessary, if a proxy is a function which use the value of "*this"
    //Future implementation should take into account : recursive or not
    //It will enable to presserve the backup cost in a lot of case.
    //=============================================================
    if (graph==0) graph = proxy.graph;
    MutableContainer<typename Tnode::RealType> backupNode;
    MutableContainer<typename Tedge::RealType> backupEdge;
    backupNode.setAll(proxy.nodeDefaultValue);
    backupEdge.setAll(proxy.edgeDefaultValue);
    Iterator<node> *itN=graph->getNodes();
    while (itN->hasNext()) {
      node itn=itN->next();
      if (proxy.graph->isElement(itn))
	backupNode.set(itn.id,proxy.getNodeValue(itn));
    } delete itN;
    Iterator<edge> *itE=graph->getEdges();
    while (itE->hasNext()) {
      edge ite=itE->next();
      if (proxy.graph->isElement(ite))
	backupEdge.set(ite.id,proxy.getEdgeValue(ite));
    } delete itE;
    //==============================================================*
    if (graph==proxy.graph) {
      setAllNodeValue(proxy.getNodeDefaultValue());
      setAllEdgeValue(proxy.getEdgeDefaultValue());
    }
    itN=graph->getNodes();
    while (itN->hasNext()) {
      node itn=itN->next();
      if (proxy.graph->isElement(itn))
	  setNodeValue(itn,backupNode.get(itn.id));
    } delete itN;
    itE=graph->getEdges();
    while (itE->hasNext()) {
      edge ite=itE->next();
      if (proxy.graph->isElement(ite))
	setEdgeValue(ite,backupEdge.get(ite.id));
    } delete itE;
    clone_handler(proxy);
  }
  return *this;
  }
  //=================================================================================
  bool compute(const std::string &algorithm, std::string &msg, const PropertyContext&);
  //=================================================================================
  virtual std::string getTypename();
  // Untyped accessors
  virtual std::string getNodeDefaultStringValue();
  virtual std::string getEdgeDefaultStringValue();
  virtual std::string getNodeStringValue( const node n );
  virtual std::string getEdgeStringValue( const edge e );
  virtual bool setNodeStringValue( const node n, const std::string & v );
  virtual bool setEdgeStringValue( const edge e, const std::string & v );
  virtual bool setAllNodeStringValue( const std::string & v );
  virtual bool setAllEdgeStringValue( const std::string & v );

protected:
  //=================================================================================
  ///Enable to clone part of sub_class
  virtual void clone_handler(AbstractProperty<Tnode,Tedge,TPROPERTY> &){};
  /**
   * Handler function for inherited AbstractProperty. This function
   * is called after a setAllNodeValue operation.
   */
  virtual void setAllNodeValue_handler(const typename Tnode::RealType &){};
  /**
   * Handler function for inherited AbstractProperty. This function
   * is called after a setAllEdgeValue operation.
   */
  virtual void setAllEdgeValue_handler(const typename Tedge::RealType &){};
  /**
   * Handler function for inherited AbstractProperty. This function
   * is called after a setEdgeValue operation.
   */
  virtual void setEdgeValue_handler(const edge, const typename Tedge::RealType &){};
  /**
   * Handler function for inherited AbstractProperty. This function
   * is called after a setNodeValue operation.
   */
  virtual void setNodeValue_handler(const node, const typename Tnode::RealType &){};

protected:
  MutableContainer<typename Tnode::RealType> nodeProperties;
  MutableContainer<typename Tedge::RealType> edgeProperties;
  Graph *graph;
  bool circularCall;
  typename Tnode::RealType nodeDefaultValue;
  typename Tedge::RealType edgeDefaultValue;
};
/*@}*/

}
#include "./cxx/AbstractProperty.cxx"
#endif
