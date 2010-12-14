/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef ABSTRACT_PROPERTY_H
#define ABSTRACT_PROPERTY_H

#include <string>
#include <cstdlib>
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

//==============================================================
// define template iterator class to iterate over graph elts
// belonging to a given graph instance
template <typename ELT_TYPE>
class GraphEltIterator :public tlp::Iterator<ELT_TYPE> {
private:
  tlp::Iterator<ELT_TYPE> *it;
  const tlp::Graph* graph;
  ELT_TYPE curElt;
  bool _hasnext;

public:
  ELT_TYPE next() {
    ELT_TYPE tmp = curElt;
    if ((_hasnext = it->hasNext())) {
      curElt = it->next();
      while (!(_hasnext = (!graph || graph->isElement(curElt)))) {
  if (!it->hasNext()) break;
  curElt=it->next();
      }
    }
    return tmp;
  }
  GraphEltIterator(const tlp::Graph* g, tlp::Iterator<ELT_TYPE>* itN)
    :it(itN), graph(g), curElt(ELT_TYPE()), _hasnext(false) {
    next();
  }

  bool hasNext() {
    return (_hasnext);
  }
  ~GraphEltIterator() {
    delete it;
  }
};

template<typename T>
struct PropertyValueContainer :public tlp::DataMem {
  T value;
  PropertyValueContainer(const T& val) : value(val) {
    value = val;
  }
  ~PropertyValueContainer() {
  }
};

/**
 * \defgroup properties Properties
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
  static TemplateFactory< PropertyFactory<TPROPERTY>, TPROPERTY, PropertyContext > *factory;
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
  typename tlp::ReturnType<typename Tnode::RealType>::ConstValue getNodeValue(const node n ) const;
  /**
   * Returns the value associated to the edge e in this property.
   * If there is no value , it returns the default value
   * depending of the type.
   */
  typename tlp::ReturnType<typename Tedge::RealType>::ConstValue getEdgeValue(const edge e) const;
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
  virtual void erase(const node n) { setNodeValue(n, nodeDefaultValue); }
  //=================================================================================
  virtual void erase(const edge e) { setEdgeValue(e, edgeDefaultValue); }
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
  virtual std::string getTypename() const { return tlp::PropertyInterface::getTypename( this ); }
  // Untyped accessors
  virtual std::string getNodeDefaultStringValue() const {
    typename Tnode::RealType v = getNodeDefaultValue();
    return Tnode::toString( v );
  }
  virtual std::string getEdgeDefaultStringValue() const {
    typename Tedge::RealType v = getEdgeDefaultValue();
    return Tedge::toString( v );
  }
  virtual std::string getNodeStringValue( const node n ) const {
    typename Tnode::RealType v = getNodeValue( n );
    return Tnode::toString( v );
  }
  virtual std::string getEdgeStringValue( const edge e ) const {
    typename Tedge::RealType v = getEdgeValue( e );
    return Tedge::toString( v );
  }
  virtual bool setNodeStringValue( const node inN, const std::string & inV ) {
    typename Tnode::RealType v;
    if( !Tnode::fromString(v,inV) )
      return false;
    setNodeValue( inN, v );
    return true;
  }
  virtual bool setEdgeStringValue( const edge inE, const std::string & inV ) {
    typename Tedge::RealType v;
    if( !Tedge::fromString(v,inV) )
      return false;
    setEdgeValue( inE, v );
    return true;
  }
  virtual bool setAllNodeStringValue( const std::string & inV ) {
    typename Tnode::RealType v;
    if( !Tnode::fromString(v,inV) )
      return false;
    setAllNodeValue( v );
    return true;
  }
  virtual bool setAllEdgeStringValue( const std::string & inV ) {
    typename Tedge::RealType v;
    if( !Tedge::fromString(v,inV) )
      return false;
    setAllEdgeValue( v );
    return true;
  }
  // returns an iterator on all nodes (belonging to g) whose value is different
  // from the default value
  virtual tlp::Iterator<node>* getNonDefaultValuatedNodes(const Graph* g = NULL) const {
    tlp::Iterator<tlp::node> *it =
      new tlp::UINTIterator<tlp::node>(nodeProperties.findAll(nodeDefaultValue, false));
    if (name.empty())
      // we always need to check that nodes belong to graph
      // for non registered properties, because deleted nodes are not erased
      // from them
      return new GraphEltIterator<tlp::node>(g != NULL ? g : graph, it);
    return ((g == NULL) || (g == graph)) ? it : new GraphEltIterator<tlp::node>(g, it);
  }
  // returns an iterator on all edges (belonging to g) whose value is different
  // from the default value
  virtual tlp::Iterator<edge>* getNonDefaultValuatedEdges(const Graph* g = NULL) const {
    tlp::Iterator<tlp::edge>* it =
      new tlp::UINTIterator<tlp::edge>(edgeProperties.findAll(edgeDefaultValue, false));
    if (name.empty())
      // we always need to check that edges belong to graph
      // for non registered properties, because deleted edges are not erased
      // from them
      return new GraphEltIterator<tlp::edge>(g != NULL ? g : graph, it);
    return ((g == NULL) || (g == graph)) ? it : new GraphEltIterator<tlp::edge>(g, it);
  }
  /**
   * Set the value of a node (first argument) in the current property (this)
   * with the value of the node (second argument) defined in prop (third argument).
   * If the fourth argument is set to true, the value will be copied only if
   * it is not the default value.
   */
  virtual void copy(const node dst, const node src, PropertyInterface *prop,
		    bool ifNotDefault = false) {
          if (prop == NULL)
            return;
          tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>* tp =
            dynamic_cast<tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>*>(prop);
          assert(tp);
          bool notDefault;
          typename ReturnType<typename Tnode::RealType>::Value value =
            tp->nodeProperties.get(src.id, notDefault);
          if (ifNotDefault && !notDefault)
            return;
          setNodeValue(dst, value);
        }
  /**
   * Set the value of an edge (first argument) in the current property (this)
   * with the value of the edge (second argument) defined in prop (third argument).
   * If the fourth argument is set to true, the value will be copied only if
   * it is not the default value.
   */
  virtual void copy(const edge dst, const edge src, PropertyInterface *prop,
		    bool ifNotDefault = false) {
          if (prop == NULL)
            return;
          tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>* tp =
            dynamic_cast<tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>*>(prop);
          assert(tp);
          bool notDefault;
          typename ReturnType<typename Tedge::RealType>::Value value =
            tp->edgeProperties.get(src.id, notDefault);
          if (ifNotDefault && !notDefault)
            return;
          setEdgeValue(dst, value);
        }
  // for performance reason and use in GraphUpdatesRecorder
  virtual DataMem* getNodeDefaultDataMemValue() const {
    return new PropertyValueContainer<typename Tnode::RealType>(getNodeDefaultValue());
  }
  virtual DataMem* getEdgeDefaultDataMemValue() const {
    return new PropertyValueContainer<typename Tedge::RealType>(getEdgeDefaultValue());
  }
  virtual DataMem* getNodeDataMemValue(const node n) const {
    return new PropertyValueContainer<typename Tnode::RealType>(getNodeValue(n));
  }
  virtual DataMem* getEdgeDataMemValue(const edge e) const {
    return new PropertyValueContainer<typename Tedge::RealType>(getEdgeValue(e));
  }
  virtual DataMem* getNonDefaultDataMemValue( const node n ) const {
    bool notDefault;
    typename ReturnType<typename Tnode::RealType>::Value value = nodeProperties.get(n.id, notDefault);
    if (notDefault)
      return new PropertyValueContainer<typename Tnode::RealType>(value);
    return NULL;
  }
  virtual DataMem* getNonDefaultDataMemValue( const edge e ) const {
    bool notDefault;
    typename ReturnType<typename Tedge::RealType>::Value value = edgeProperties.get(e.id, notDefault);
    if (notDefault)
      return new PropertyValueContainer<typename Tedge::RealType>(value);
    return NULL;
  }
  virtual void setNodeDataMemValue( const node n, const DataMem* v) {
    setNodeValue(n, ((PropertyValueContainer<typename Tnode::RealType> *) v)->value);
  }
  virtual void setEdgeDataMemValue( const edge e, const DataMem* v) {
    setEdgeValue(e, ((PropertyValueContainer<typename Tedge::RealType> *) v)->value);
  }
  virtual void setAllNodeDataMemValue(const DataMem* v) {
    setAllNodeValue(((PropertyValueContainer<typename Tnode::RealType> *) v)->value);
  }
  virtual void setAllEdgeDataMemValue(const DataMem* v) {
    setAllEdgeValue(((PropertyValueContainer<typename Tedge::RealType> *) v)->value);
  }

  // PropertyInterface methods
  // mN is the meta node, sg is the corresponding subgraph
  // and mg is the graph owning mN
  virtual void computeMetaValue(node n, Graph* sg, Graph* mg) {
    if (metaValueCalculator)
      ((typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *) 
      metaValueCalculator)->computeMetaValue(this, n, sg, mg);
  }
  // mE is the meta edge, itE is an iterator on the underlying edges
  // mg is the graph owning mE
  virtual void computeMetaValue(edge e, tlp::Iterator<edge>* itE, Graph* mg) {
    if (metaValueCalculator)
    ((typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *)metaValueCalculator)->computeMetaValue(this, e, itE, mg);
  }
  virtual void setMetaValueCalculator(PropertyInterface::MetaValueCalculator *mvCalc) {
    if (mvCalc && !dynamic_cast<typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *>(mvCalc)) {
      std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... invalid conversion of " << typeid(mvCalc).name() << "into " << typeid(typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *).name() << std::endl;
      abort();
    }
    metaValueCalculator = mvCalc;
  }

  // This class is used to delegate the computation of the values associated
  // to meta nodes or edges
  class MetaValueCalculator :public PropertyInterface::MetaValueCalculator {
  public:
    // computes the value of the meta node mN of the graph mg
    // for the property prop, according to the values associated
    // to the underlying nodes i.e the nodes of the subgraph sg.
    virtual void computeMetaValue(AbstractProperty<Tnode, Tedge,
				  TPROPERTY>*, node,
				  Graph*, Graph*) {}
    // computes the value of the meta node mE of the graph mg
    // for the property prop, according to the values associated
    // to the underlying edges given by the iterator itE.
    // The method do not have to delete the iterator
    virtual void computeMetaValue(AbstractProperty<Tnode, Tedge,
				  TPROPERTY>*,
				  edge, tlp::Iterator<edge>*,
				  Graph*) {}
  };

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

 template <typename vectType,typename eltType > 
   class TLP_SCOPE AbstractVectorProperty : public AbstractProperty<vectType, vectType> {
 public:
  AbstractVectorProperty(Graph *, std::string name = "");
  /**
   * Set the value of the elt i of the vector associated to node n
   * and notify the observers of a modification.
   */
  void setNodeEltValue(const node n, unsigned int i, const eltType &v);
  /**
   * get the value of the elt i of the vector associated to node n
   * and notify the observers of a modification.
   */
  const eltType& getNodeEltValue(const node n, unsigned int i) const;
  /**
   * append a new value at the end of the vector associated to node n
   * and notify the observers of a modification.
   */
  void pushBackNodeEltValue(const node n, const eltType &v);
  /**
   * remove the value at the end of the vector associated to node n
   * and notify the observers of a modification.
   */
  void popBackNodeEltValue(const node n);
  /**
   * resize the vector associated to node n
   * and notify the observers of a modification.
   */
  void resizeNodeValue(const node n, size_t size, eltType elt = eltType());
  /**
   * Set the value of the elt i of the value associated to edge
   * and notify the observers of a modification.
   */
  void setEdgeEltValue(const edge e, unsigned int i, const eltType &v);
  /**
   * get the value of the elt i of the vector associated to edge e
   * and notify the observers of a modification.
   */
  const eltType& getEdgeEltValue(const edge n, unsigned int i) const;
  /**
   * append a new value at the end of the vector associated to edge e
   * and notify the observers of a modification.
   */
  void pushBackEdgeEltValue(const edge e, const eltType &v);
  /**
   * remove the value at the end of the vector associated to edge e
   * and notify the observers of a modification.
   */
  void popBackEdgeEltValue(const edge e);
  /**
   * resize the vector associated to edgee e
   * and notify the observers of a modification.
   */
  void resizeEdgeValue(const edge e, size_t size, eltType elt = eltType());
 };
/*@}*/
}
#if !defined(_MSC_VER) || defined(DLL_TULIP) //When using VC++, we only want to include this when we are in the TULIP dll. With any other compiler, include it all the time
#	include "./cxx/AbstractProperty.cxx"
#endif
#endif
