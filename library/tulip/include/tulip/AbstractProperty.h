//-*-c++-*-
#ifndef ABSTRACT_PROPERTY_H
#define ABSTRACT_PROPERTY_H

#include <string>
#include "tulip/tulipconf.h"
#include "tulip/PropertyInterface.h"
#include "tulip/Iterator.h"
#include "tulip/Types.h"
#include "tulip/PropertyAlgorithm.h"
#include "tulip/MutableContainer.h"
#include "tulip/ReturnType.h"
#include "tulip/TemplateFactory.h"
#include "tulip/Reflect.h"

namespace tlp {

class GraphView;

/**
 * \defgroup properties
 */ 
/*@{*/
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
   * Returns the node default value of the property
   */
  typename Tnode::RealType getNodeDefaultValue() const;
  /** 
   * Returns the edge default value of the property
   */
  typename Tedge::RealType getEdgeDefaultValue() const;
  /**
   * Returns the value associated to the node n in this property.
   * If there is no value it returns the default value
   * depending of the type.
   */
  typename ReturnType<typename Tnode::RealType>::ConstValue getNodeValue(const node n ) const;
  /**
   * Returns the value associated to the node n in this property.
   * If there is no value it returns the default value
   * depending of the type.
   */
  typename ReturnType<typename Tedge::RealType>::ConstValue getEdgeValue(const edge e) const;
  /**
   * Set the value of a node n and notify the observers of a modification.
   */
  void setNodeValue(const node n, const typename Tnode::RealType &v);
  /**
   * Set the value of an edge and notify the observers of a modification.
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
    //It will enable to preserve the backup cost in a lot of case.
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
  virtual std::string getTypename() const;
  // Untyped accessors
  virtual std::string getNodeDefaultStringValue() const;
  virtual std::string getEdgeDefaultStringValue() const;
  virtual std::string getNodeStringValue( const node n ) const;
  virtual std::string getEdgeStringValue( const edge e ) const;
  virtual bool setNodeStringValue( const node n, const std::string & v );
  virtual bool setEdgeStringValue( const edge e, const std::string & v );
  virtual bool setAllNodeStringValue( const std::string & v );
  virtual bool setAllEdgeStringValue( const std::string & v );
  // returns an iterator on all nodes whose value is different
  // from the default value
  virtual Iterator<node>* getNonDefaultValuatedNodes() const;
  // returns an iterator on all edges whose value is different
  // from the default value
  virtual Iterator<edge>* getNonDefaultValuatedEdges() const;
  // for performance reason and use in GraphUpdatesRecorder
  virtual DataMem* getNodeDefaultDataMemValue() const;
  virtual DataMem* getEdgeDefaultDataMemValue() const;
  virtual DataMem* getNodeDataMemValue(const node n) const;
  virtual DataMem* getEdgeDataMemValue(const edge e) const;
  virtual DataMem* getNonDefaultDataMemValue( const node n ) const;
  virtual DataMem* getNonDefaultDataMemValue( const edge e ) const;
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
