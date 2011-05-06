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
#include <tulip/tulipconf.h>
#include <tulip/StoredType.h>
#include <tulip/MutableContainer.h>
#include <tulip/PropertyInterface.h>
#include <tulip/Iterator.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/TemplateFactory.h>
#include <tulip/Reflect.h>

namespace tlp {

class GraphView;

//==============================================================
/**
 * \defgroup properties Properties
 */ 
/*@{*/

/**
 * @brief This class is used to store a property of a graph.
 * It implements PropertyInterface and add new methods
 * to deal with two distinct type of values :
 * - Tnode node value
 * - Tedge edge value
 *
 * An AbstractProperty can be connected or not to a PropertyAlgorithm.
 * In the first case it can be seen as buffer beetween the property and the user.
 * In the second case it is only a memory area for storing data.
 * An AbstractProperty is an Observable (through PropertyInterface), so it can be observed by others objects.
 */
template <class Tnode, class Tedge, class TPROPERTY = PropertyAlgorithm > 
class TLP_SCOPE AbstractProperty : public PropertyInterface {
  friend class Graph;
  friend class GraphView;

public:
  AbstractProperty(Graph *, std::string n = "");

  
  /**
   * @brief Returns the default node value of the property.
   *
   * @return :RealType The default value applied to nodes.
   **/
  typename Tnode::RealType getNodeDefaultValue() const;
  
  /**
   * @brief Returns the default edge value of the property.
   *
   * @return :RealType The default value applied to edges.
   **/
  typename Tedge::RealType getEdgeDefaultValue() const;

  /**
   * @brief Returns the value associated with the node n in this property.
   * If there is no value, it returns the default ndoe value.
   *
   * @param n The node for which we want to get the value of the property.
   * @return :StoredType< Tnode::RealType >::ReturnedConstValue The value of the property for this node.
   **/
  typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue getNodeValue(const node n ) const;
  
  /**
   * @brief Returns the value associated to the edge e in this property.
   * If there is no value, it returns the default edge value.
   *
   * @param e The edge for which we want to get the value of the property.
   * @return :StoredType< Tedge::RealType >::ReturnedConstValue The value of the property for this edge.
   **/
  typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue getEdgeValue(const edge e) const;
  
  /**
   * @brief Sets the value of a node and notify the observers of a modification.
   *
   * @param n The node to set the value of.
   * @param v The value to affect for this node.
   * @return void
   **/
  virtual void setNodeValue(const node n, const typename Tnode::RealType &v);
  
  /**
   * @brief Set the value of an edge and notify the observers of a modification.
   *
   * @param e The edge to set the value of.
   * @param v The value to affect for this edge.
   * @return void
   **/
  virtual void setEdgeValue(const edge e, const typename Tedge::RealType &v);
  
  /**
   * @brief Sets the value of all nodes and notify the observers.
   *
   * @param v The value to set to all nodes.
   * @return void
   **/
  virtual void setAllNodeValue(const typename Tnode::RealType &v);
  
  /**
   * @brief Sets the value of all edges and notify the observers.
   *
   * @param v The value to set to all edges.
   * @return void
   **/
  virtual void setAllEdgeValue(const typename Tedge::RealType &v);
  //=================================================================================
  
  /**
   * @brief Resets the value of a node to the default value.
   *
   * @param n The node to reset the value of.
   * @return void
   **/
  virtual void erase(const node n) { setNodeValue(n, nodeDefaultValue); }
  //=================================================================================
  
  /**
   * @brief Resets the value of an edge to the default value.
   *
   * @param e The edge to reset the value of.
   * @return void
   **/
  virtual void erase(const edge e) { setEdgeValue(e, edgeDefaultValue); }
  //=================================================================================
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
  // Untyped accessors inherited from PropertyInterface 
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
  /**
   * @brief Returns an iterator on all nodes whose value is different from the default value. 
   * When the pointer to the graph is not NULL, only the nodes owned by this graph are returned by the iterator.
   * @warning Ownership of the iterator belongs to the caller, i.e. the caller must delete the operator.
   *
   * @param g If not NULL, only iterate on nodes from this Graph. Defaults to NULL.
   * @return :Iterator< tlp::node >* A new Iterator on nodes whose value is not default.
   **/
  virtual tlp::Iterator<node>* getNonDefaultValuatedNodes(const Graph* g = NULL) const;
  /**
   * @brief Returns an iterator on all edges whose value is different from the default value. 
   * When the pointer to the graph is not NULL, only the edges owned by this graph are returned by the iterator.
   * @warning Ownership of the iterator belongs to the caller, i.e. the caller must delete the operator.
   *
   * @param g If not NULL, only iterate on edges from this Graph. Defaults to NULL.
   * @return :Iterator< tlp::node >* A new Iterator on edges whose value is not default.
   **/
  virtual tlp::Iterator<edge>* getNonDefaultValuatedEdges(const Graph* g = NULL) const;
  /**
   * @brief Copies the value help by a property on a node to another node on this property.
   *
   * @param destination The node to copy the value to.
   * @param source The node to copy the value from.
   * @param property The property holding the value to copy.
   * @param ifNotDefault Whether to ignore default-valuated nodes or not. Defaults to false.
   * @return void
   **/
  virtual void copy(const node destination, const node source, PropertyInterface *property,
		    bool ifNotDefault = false) {
          if (property == NULL)
            return;
          tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>* tp =
            dynamic_cast<tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>*>(property);
          assert(tp);
          bool notDefault;
          typename StoredType<typename Tnode::RealType>::ReturnedValue value =
            tp->nodeProperties.get(source.id, notDefault);
          if (ifNotDefault && !notDefault)
            return;
          setNodeValue(destination, value);
        }
  /**
   * @brief Copies the value help by a property on an edge to another edge on this property.
   *
   * @param destination The edge to copy the value to.
   * @param source The edge to copy the value from.
   * @param property The property holding the value to copy.
   * @param ifNotDefault Whether to ignore default-valuated nodes or not. Defaults to false.
   * @return void
   **/
  virtual void copy(const edge destination, const edge source, PropertyInterface *property,
		    bool ifNotDefault = false) {
          if (property == NULL)
            return;
          tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>* tp =
            dynamic_cast<tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>*>(property);
          assert(tp);
          bool notDefault;
          typename StoredType<typename Tedge::RealType>::ReturnedValue value =
            tp->edgeProperties.get(source.id, notDefault);
          if (ifNotDefault && !notDefault)
            return;
          setEdgeValue(destination, value);
        }
  // for performance reason and use in GraphUpdatesRecorder
  virtual DataMem* getNodeDefaultDataMemValue() const {
    return new TypedValueContainer<typename Tnode::RealType>(getNodeDefaultValue());
  }
  virtual DataMem* getEdgeDefaultDataMemValue() const {
    return new TypedValueContainer<typename Tedge::RealType>(getEdgeDefaultValue());
  }
  virtual DataMem* getNodeDataMemValue(const node n) const {
    return new TypedValueContainer<typename Tnode::RealType>(getNodeValue(n));
  }
  virtual DataMem* getEdgeDataMemValue(const edge e) const {
    return new TypedValueContainer<typename Tedge::RealType>(getEdgeValue(e));
  }
  virtual DataMem* getNonDefaultDataMemValue( const node n ) const {
    bool notDefault;
    typename StoredType<typename Tnode::RealType>::ReturnedValue value = nodeProperties.get(n.id, notDefault);
    if (notDefault)
      return new TypedValueContainer<typename Tnode::RealType>(value);
    return NULL;
  }
  virtual DataMem* getNonDefaultDataMemValue( const edge e ) const {
    bool notDefault;
    typename StoredType<typename Tedge::RealType>::ReturnedValue value = edgeProperties.get(e.id, notDefault);
    if (notDefault)
      return new TypedValueContainer<typename Tedge::RealType>(value);
    return NULL;
  }
  virtual void setNodeDataMemValue( const node n, const DataMem* v) {
    setNodeValue(n, ((TypedValueContainer<typename Tnode::RealType> *) v)->value);
  }
  virtual void setEdgeDataMemValue( const edge e, const DataMem* v) {
    setEdgeValue(e, ((TypedValueContainer<typename Tedge::RealType> *) v)->value);
  }
  virtual void setAllNodeDataMemValue(const DataMem* v) {
    setAllNodeValue(((TypedValueContainer<typename Tnode::RealType> *) v)->value);
  }
  virtual void setAllEdgeDataMemValue(const DataMem* v) {
    setAllEdgeValue(((TypedValueContainer<typename Tedge::RealType> *) v)->value);
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


  /**
    * @brief Default implementation of PropertyInterface::compare(node n1,node n2)
    **/
  int compare(node n1,node n2);

  /**
    * @brief Default implementation of PropertyInterface::compare(edge e1,edge e2)
    **/
  int compare(edge e1,edge e2);


  /**
   * @brief This class is used to delegate the computation of the values associated to meta nodes or edges.
   **/
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
   * @brief Sets the value for node n, at index i, to v, and notify the observers of a modification.
   *
   * @param n The node to set a value of.
   * @param i The index at which the value should be set.
   * @param v The value to set.
   * @return void
   **/
  void setNodeEltValue(const node n, unsigned int i, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Gets the value associated to node n, at index i.
   *
   * @param n The node to set a value of.
   * @param i The index at which to set the value.
   * @return const eltType& The value at index i in the vector for node n.
   **/
  typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue getNodeEltValue(const node n, unsigned int i) const;
  /**
   * @brief Appends a new value at the end of the vector associated to node n, and notify the observers of a modification.
   *
   * @param n The node to add a value to.
   * @param v The value to append at the end of the vector.
   * @return void
   **/
  void pushBackNodeEltValue(const node n, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Removes the value at the end of the vector associated to node n, and notify the observers of a modification.
   *
   * @param n The node to remove a value from.
   * @return void
   **/
  void popBackNodeEltValue(const node n);
  /**
   * @brief Resizes the vector associated to node n, and notify the observers of a modification.
   *
   * @param n The node associated to the vector to resize.
   * @param size The new size of the vector.
   * @param elt The default value to set at indices where there was no value before. Defaults to eltType().
   * @return void
   **/
  void resizeNodeValue(const node n, size_t size, typename eltType::RealType elt = eltType::defaultValue());
  /**
   * @brief Sets the value for edge e, at index i, to v, and notify the observers of a modification.
   *
   * @param e The edge to set the value of.
   * @param i The index at which the value should be set.
   * @param v The value to set.
   * @return void
   **/
  void setEdgeEltValue(const edge e, unsigned int i, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Gets the value associated to edge e, at index i.
   *
   * @param e The edge to set a value of.
   * @param i The index at which to set the value.
   * @return const eltType& The value at index i in the vector for node n.
   **/
  typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue getEdgeEltValue(const edge n, unsigned int i) const;
  /**
   * @brief Appends a new value at the end of the vector associated to edge e, and notify the observers of a modification.
   *
   * @param e The node to add a value to.
   * @param v The value to append at the end of the vector.
   * @return void
   **/
  void pushBackEdgeEltValue(const edge e, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
    /**
   * @brief Removes the value at the end of the vector associated to edge e, and notify the observers of a modification.
   *
   * @param e The edge to remove a value from.
   * @return void
   **/
  void popBackEdgeEltValue(const edge e);
  /**
   * @brief Resizes the vector associated to edge e, and notify the observers of a modification.
   *
   * @param e The edge associated to the vector to resize.
   * @param size The new size of the vector.
   * @param elt The default value to set at indices where there was no value before. Defaults to eltType().
   * @return void
   **/
  void resizeEdgeValue(const edge e, size_t size, typename eltType::RealType elt = eltType::defaultValue());
 };
/*@}*/

}
#if !defined(_MSC_VER) || defined(DLL_TULIP) //When using VC++, we only want to include this when we are in the TULIP dll. With any other compiler, include it all the time
#	include "cxx/AbstractProperty.cxx"
#endif
#endif
