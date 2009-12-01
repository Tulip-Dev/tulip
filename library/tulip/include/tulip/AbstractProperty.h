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

#include "tulip/Observable.h"
#include "tulip/ObservableProperty.h"

#include "tulip/Iterator.h"
#include "tulip/Types.h"
#include "tulip/PropertyAlgorithm.h"
#include "tulip/MutableContainer.h"
#include "tulip/ReturnType.h"
#include "tulip/tulipconf.h"
#include "tulip/TemplateFactory.h"
#include "tulip/Reflect.h"

namespace tlp {

/**
 * \defgroup properties
 */ 
/*@{*/
//=============================================================
class TLP_SCOPE PropertyInterface: public Observable, public ObservableProperty {
  friend class PropertyManager;
protected:
  // name of the property when registered as a property of a graph
  std::string name;
  // the graph for whom the property is registered
  Graph *graph;

public:
  virtual ~PropertyInterface();

  virtual void erase(const node) =0;
  virtual void erase(const edge) =0;
  virtual void copy(const node, const node, PropertyInterface *) =0;
  virtual void copy(const edge, const edge, PropertyInterface *) =0;
  virtual PropertyInterface* clonePrototype(Graph *, const std::string&) =0;
  //=================================================================================
  // Returns a string describing the type of the property.
  // i.e. "graph", "double", "layout", "string", "integer", "color", "size", ...
  virtual std::string getTypename() = 0;
  static  std::string getTypename( PropertyInterface * );

  // name management
  const std::string& getName() const {
    return name;
  }

  // Untyped accessors
  virtual std::string getNodeDefaultStringValue() = 0;
  virtual std::string getEdgeDefaultStringValue() = 0;
  virtual std::string getNodeStringValue( const node n ) = 0;
  virtual std::string getEdgeStringValue( const edge e ) = 0;
  virtual bool setNodeStringValue( const node n, const std::string & v ) = 0;
  virtual bool setEdgeStringValue( const edge e, const std::string & v ) = 0;
  virtual bool setAllNodeStringValue( const std::string & v ) = 0;
  virtual bool setAllEdgeStringValue( const std::string & v ) = 0;
  // the ones below are used by GraphUpdatesRecorder
  virtual Iterator<node>* getNonDefaultValuatedNodes()=0;
  virtual Iterator<edge>* getNonDefaultValuatedEdges()=0;
  virtual DataMem* getNodeDefaultDataMemValue() = 0;
  virtual DataMem* getEdgeDefaultDataMemValue() = 0;
  virtual DataMem* getNodeDataMemValue( const node n ) = 0;
  virtual DataMem* getEdgeDataMemValue( const edge e ) = 0;
  virtual DataMem* getNonDefaultDataMemValue( const node n ) = 0;
  virtual DataMem* getNonDefaultDataMemValue( const edge e ) = 0;
  virtual void setNodeDataMemValue( const node n, const DataMem* v) = 0;
  virtual void setEdgeDataMemValue( const edge e, const DataMem* v) = 0;
  virtual void setAllNodeDataMemValue(const DataMem* v ) = 0;
  virtual void setAllEdgeDataMemValue(const DataMem* v ) = 0;

 protected:
  // redefinitions of ObservableProperty methods
  void notifyAfterSetNodeValue(PropertyInterface*,const node n);
  void notifyAfterSetEdgeValue(PropertyInterface*,const edge e);
  void notifyAfterSetAllNodeValue(PropertyInterface*);
  void notifyAfterSetAllEdgeValue(PropertyInterface*);
  void notifyDestroy(PropertyInterface*);
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
template <class Tnode, class Tedge, class TPROPERTY = PropertyAlgorithm > 
class TLP_SCOPE AbstractProperty : public PropertyInterface {
  friend class Graph;
  friend class GraphView;

public:
  static TLP_SCOPE TemplateFactory< PropertyFactory<TPROPERTY>, TPROPERTY, PropertyContext > *factory;
  static void initFactory() {
    if (!factory) {
      factory = new TemplateFactory< PropertyFactory<TPROPERTY>, TPROPERTY, PropertyContext >;
    }
  }
  AbstractProperty(Graph *, std::string n = "");

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
  typename ReturnType<typename Tnode::RealType>::ConstValue getNodeValue(const node n );
  /**
   * Returns the value associated to the node n in this property.
   * If the value is already fixed it is done in constant time.
   * else if it is a computed property (plug-in), it calls the associated property
   * If there is no value and no algorithms it returns the default value
   * depending of the type.
   */
  typename ReturnType<typename Tedge::RealType>::ConstValue getEdgeValue(const edge e);
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
  virtual AbstractProperty<Tnode,Tedge,TPROPERTY>& operator =(AbstractProperty<Tnode,Tedge,TPROPERTY> &prop) {
      if (this!= &prop) {
    //=============================================================
    //The backup is necessary, if a proxy is a function which use the value of "*this"
    //Future implementation should take into account : recursive or not
    //It will enable to presserve the backup cost in a lot of case.
    //=============================================================
    if (graph == 0) graph = prop.graph;
    if (graph == prop.graph) {
      setAllNodeValue(prop.getNodeDefaultValue());
      setAllEdgeValue(prop.getEdgeDefaultValue());
      Iterator<node> *itN = prop.getNonDefaultValuatedNodes();
      while (itN->hasNext()) {
	node itn = itN->next();
	setNodeValue(itn, prop.getNodeValue(itn));
      } delete itN;
      Iterator<edge> *itE = prop.getNonDefaultValuatedEdges();
      while (itE->hasNext()) {
	edge ite = itE->next();
	setEdgeValue(ite, prop.getEdgeValue(ite));
      } delete itE;
    } else {
      MutableContainer<typename Tnode::RealType> backupNode;
      MutableContainer<typename Tedge::RealType> backupEdge;
      backupNode.setAll(prop.nodeDefaultValue);
      backupEdge.setAll(prop.edgeDefaultValue);
      Iterator<node> *itN=graph->getNodes();
      while (itN->hasNext()) {
	node itn=itN->next();
	if (prop.graph->isElement(itn))
	  backupNode.set(itn.id,prop.getNodeValue(itn));
      } delete itN;
      Iterator<edge> *itE=graph->getEdges();
      while (itE->hasNext()) {
	edge ite=itE->next();
	if (prop.graph->isElement(ite))
	  backupEdge.set(ite.id,prop.getEdgeValue(ite));
      } delete itE;
      //==============================================================*
      itN=graph->getNodes();
      while (itN->hasNext()) {
	node itn=itN->next();
	if (prop.graph->isElement(itn))
	  setNodeValue(itn,backupNode.get(itn.id));
      } delete itN;
      itE=graph->getEdges();
      while (itE->hasNext()) {
	edge ite=itE->next();
	if (prop.graph->isElement(ite))
	  setEdgeValue(ite,backupEdge.get(ite.id));
      } delete itE;
    }
    clone_handler(prop);
  }
  return *this;
  }
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
  // returns an iterator on all nodes whose value is different
  // from the default value
  virtual Iterator<node>* getNonDefaultValuatedNodes();
  // returns an iterator on all edges whose value is different
  // from the default value
  virtual Iterator<edge>* getNonDefaultValuatedEdges();
  // for performance reason and use in GraphUpdatesRecorder
  virtual DataMem* getNodeDefaultDataMemValue();
  virtual DataMem* getEdgeDefaultDataMemValue();
  virtual DataMem* getNodeDataMemValue(const node n);
  virtual DataMem* getEdgeDataMemValue(const edge e);
  virtual DataMem* getNonDefaultDataMemValue( const node n );
  virtual DataMem* getNonDefaultDataMemValue( const edge e );
  virtual void setNodeDataMemValue( const node n, const DataMem* v);
  virtual void setEdgeDataMemValue( const edge e, const DataMem* v);
  virtual void setAllNodeDataMemValue(const DataMem* v);
  virtual void setAllEdgeDataMemValue(const DataMem* v);

protected:
  typedef PropertyAlgorithm PAlgorithm;
  //=================================================================================
  ///Enable to clone part of sub_class
  virtual void clone_handler(AbstractProperty<Tnode,Tedge,TPROPERTY> &){};

protected:
  MutableContainer<typename Tnode::RealType> nodeProperties;
  MutableContainer<typename Tedge::RealType> edgeProperties;
  typename Tnode::RealType nodeDefaultValue;
  typename Tedge::RealType edgeDefaultValue;
};
/*@}*/

}
#include "./cxx/AbstractProperty.cxx"
#endif
